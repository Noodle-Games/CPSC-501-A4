#pragma once
#include "WavReader.h"

void convolve_fft(WavFile* inputFile, WavFile* IRFile, const char* outputFileName);
void fourierTransform(double data[], int nn, int isign);
double* toDoubleArray(const float* array, int size, int paddedSize);
float* toFloatArray(const double* array, int size);
int nextPowerOf2(int x);