#pragma once
#include "WavReader.h"

void convolve_fft(WavFile* inputFile, WavFile* IRFile, const char* outputFileName);