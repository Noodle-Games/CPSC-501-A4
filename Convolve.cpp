#include "Convolve.h"
#include "TimeDomain.h"
#include "FastFourier.h"
#include <string>
#include <iostream>
#include <chrono>

using namespace std;

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
    // Used when running as a CMAKE project from CLion, but not needed for command line execution
    /* string programPath = (string)argv[0];
    programPath = programPath.substr(0, programPath.find("cpsc_501_a4")) + "cpsc_501_a4\\";
    string inputPath = (programPath + (string)argv[1]);
    string irPath = (programPath + (string)argv[2]);
    string outputPath = (programPath + (string)argv[3]); */

    string inputPath = ((string)argv[1]);
    string irPath = ((string)argv[2]);
    string outputPath = ((string)argv[3]);

    // Read input files into WaveFile struct
    WavFile* inputWav = readWavFile(inputPath.c_str());
    WavFile* irWav = readWavFile(irPath.c_str());

    // Convolve
    auto startTime = chrono::high_resolution_clock::now();
    //convolve_timeDomain(inputWav, irWav, outputPath.c_str());     // Time Domain Convolution
    convolve_fft(inputWav, irWav, outputPath.c_str());              // Fast Fourier Convolution
    auto endTime = chrono::high_resolution_clock::now();

    // Compute convolution processing time
    auto elapsedTime = (endTime - startTime);
    cout << "Convolution Processing Time:\n";
    cout << chrono::duration_cast<chrono::seconds>(elapsedTime).count() << "s / ";
    cout << chrono::duration_cast<chrono::milliseconds>(elapsedTime).count() << "ms\n";
    cout << "----------------------------------------------------------------------------------------------";

    // Return success
    return 0;
}