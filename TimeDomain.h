#pragma once
#include "WavReader.h"

void convolve_timeDomain(WavFile* inputFile, WavFile* IRFile, const char* outputFileName);