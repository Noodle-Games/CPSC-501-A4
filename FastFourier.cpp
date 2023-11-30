#include "FastFourier.h"
#include <cmath>

using namespace std;

#define SWAP(a,b)  tempr=(a);(a)=(b);(b)=tempr

void convolve_fft(WavFile* inputFile, WavFile* IRFile, const char* outputFileName){
    // Convert data to padded, power of 2 sized double arrays
    int dataSize = nextPowerOf2(inputFile->subChunk2Size * 2);
    int halfDataSize = dataSize / 2;
    double* X = toDoubleArray(inputFile->data, inputFile->subChunk2Size, dataSize);
    double* H = toDoubleArray(IRFile->data, IRFile->subChunk2Size, dataSize);

    // Fourier Transform
    fourierTransform(X, halfDataSize, 1);
    fourierTransform(H, halfDataSize, 1);

    // Complex Multiplication
    double* Y = (double*) malloc(sizeof(double) * dataSize);
    for(int i = 0; i < dataSize; i += 2){
        Y[i] = X[i] * H[i] - X[i+1] * H[i+1];
        Y[i+1] = X[i+1] * H[i] - X[i] * H[i+1];
    }

    // Inverse Fourier Transform
    fourierTransform(Y, halfDataSize, -1);

    // Scale by N
    const double N = 3276800.0;
    for(int i = 0; i < dataSize; i++){
        Y[i] /= N;
    }

    // Write to Wave File
    WavFile* outputFile = createWavFile(toFloatArray(Y, dataSize), halfDataSize);
    writeWavFile(outputFile, outputFileName);
}

double* toDoubleArray(const float* array, int size, int paddedSize){
    int newSize = size * 2;
    double* newArray = (double*) malloc(sizeof(double) * paddedSize);
    int j = 0;
    for (int i = 0; i < paddedSize; i++){
        if (i % 2 == 0 && i < newSize){
            newArray[i] = array[j];
            j++;
        }
        else newArray[i] = 0.0;
    }
    return newArray;
}

float* toFloatArray(const double* array, int size){
    float* newArray = (float*) malloc(sizeof(float) * (size / 2));
    int j = 0;
    for (int i = 0; i < size; i += 2){
        newArray[j] = (float)array[i];
        j++;
    }
    return newArray;
}

int nextPowerOf2(int x){
    int power = 1;
    while (power < x) {
        power += power;
    }
    return power;
}

// Adapted from provided code in D2L: DFT/FFT test program
// Four1 FFT Algorithm from Numerical Recipes in C, p. 507 - 508
void fourierTransform(double data[], int nn, int isign){
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    double tempr, tempi;

    n = nn << 1;
    j = 1;

    for (i = 1; i < n; i += 2) {
        if (j > i) {
            SWAP(data[j], data[i]);
            SWAP(data[j+1], data[i+1]);
        }
        m = nn;
        while (m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }

    mmax = 2;
    while (n > mmax) {
        istep = mmax << 1;
        theta = isign * (6.28318530717959 / mmax);
        wtemp = sin(0.5 * theta);
        wpr = -2.0 * wtemp * wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m = 1; m < mmax; m += 2) {
            for (i = m; i <= n; i += istep) {
                j = i + mmax;
                tempr = wr * data[j] - wi * data[j+1];
                tempi = wr * data[j+1] + wi * data[j];
                data[j] = data[i] - tempr;
                data[j+1] = data[i+1] - tempi;
                data[i] += tempr;
                data[i+1] += tempi;
            }
            wr = (wtemp = wr) * wpr - wi * wpi + wr;
            wi = wi * wpr + wtemp * wpi + wi;
        }
        mmax = istep;
    }
}