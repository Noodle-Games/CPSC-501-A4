#include "TimeDomain.h"
#include <iostream>

void convolve_timeDomain(WavFile* inputFile, WavFile* IRFile, const char* outputFileName){
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