#include "Convolve.h"
#include "WavReader.h"
#include <string>
#include <iostream>
#include <chrono>

using namespace std;

void convolve(WavFile* inputFile, WavFile* IRFile, const char* outputFileName){
    int N = inputFile->subChunk2Size;
    int M = IRFile->subChunk2Size;
    float* x = inputFile->data;
    float* h = IRFile->data;
    float* y = (float*) malloc(sizeof(float) * (N + M - 1));

    for(int n = 0; n < N; n++){
        for(int m = 0; m < M; m++){
            y[n + m] += x[n] * h[m];
        }
    }

    WavFile* outputFile = createWavFile(y, N + M - 1);
    writeWavFile(outputFile, outputFileName);
}

int main(int argc, char *argv[]){
    // Check argument list length
    if (argc != 4) return -1;

    // Print input
    cout << "----------------------------------------------------------------------------------------------";
    cout << "\nDry Recording: " + (string)argv[1];
    cout << "\nImpulse Response: " + (string)argv[2];
    cout << "\nOutput File: " + (string)argv[3];
    cout << "\n----------------------------------------------------------------------------------------------\n";

    // Get absolute path of files
    string programPath = (string)argv[0];
    programPath = programPath.substr(0, programPath.find("cpsc_501_a4")) + "cpsc_501_a4\\";
    string inputPath = (programPath + (string)argv[1]);
    string irPath = (programPath + (string)argv[2]);
    string outputPath = (programPath + (string)argv[3]);

    // Read input files into WaveFile struct
    WavFile* inputWav = readWavFile(inputPath.c_str());
    WavFile* irWav = readWavFile(irPath.c_str());

    // Convolve
    auto startTime = chrono::high_resolution_clock::now();
    convolve(inputWav, irWav, outputPath.c_str());
    auto endTime = chrono::high_resolution_clock::now();

    // Compute convolution processing time
    auto elapsedTime = (endTime - startTime);
    cout << "Convolution Processing Time:\n";
    cout << chrono::duration_cast<chrono::seconds>(elapsedTime).count() << "s / ";
    cout << chrono::duration_cast<chrono::milliseconds>(elapsedTime).count() << "ms / ";
    cout << chrono::duration_cast<chrono::nanoseconds>(elapsedTime).count() << "ns\n";
    cout << "----------------------------------------------------------------------------------------------";

    // Return success
    return 0;
}