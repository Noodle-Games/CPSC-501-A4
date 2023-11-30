#include "TestConvolve.h"
#include "Convolve.h"
#include "WavReader.h"
#include <iostream>

using namespace std;

void testFileReadSmall(){
    WavFile* wavFile = readWavFile("sounds/ir_bigHall.wav");
    if (wavFile == nullptr) cout << "testFileReadSmall: Failed - File failed to open successfully\n";
    else cout << "testFileReadSmall: Passed\n";
}

void testFileReadLarge(){
    WavFile* wavFile = readWavFile("sounds/dry_guitar.wav");
    if (wavFile == nullptr) cout << "testFileReadLarge: Failed - File failed to open successfully\n";
    else cout << "testFileReadLarge: Passed\n";
}

void testConvolveSmall(){
    const char* arg1 = "convolve";
    const char* arg2 = "sounds/dry_guitarShort.wav";
    const char* arg3 = "sounds/ir_bigHall.wav";
    const char* arg4 = "sounds/test_convolveShort.wav";
    char* argv[] = {(char*)arg1, (char*)arg2, (char*)arg3, (char*)arg4};
    convolve(argv);

    cout << "\n";
    WavFile* wavFile = readWavFile("sounds/test_convolveShort.wav");
    if (wavFile == nullptr) cout << "testConvolveSmall: Failed - Output file failed to open successfully\n";
    else if (wavFile->subChunk2Size != 524288) cout << "testConvolveSmall: Failed - subChunk2Size expected: 524288 actual: " << wavFile->subChunk2Size << "\n";
    else cout << "testConvolveSmall: Passed\n";
}

void testConvolveLarge(){
    const char* arg1 = "convolve";
    const char* arg2 = "sounds/dry_guitar.wav";
    const char* arg3 = "sounds/ir_bigHall.wav";
    const char* arg4 = "sounds/test_convolveLarge.wav";
    char* argv[] = {(char*)arg1, (char*)arg2, (char*)arg3, (char*)arg4};
    convolve(argv);

    cout << "\n";
    WavFile* wavFile = readWavFile("sounds/test_convolveLarge.wav");
    if (wavFile == nullptr) cout << "testConvolveLarge: Failed - Output file failed to open successfully\n";
    else if (wavFile->subChunk2Size != 4194304) cout << "testConvolveLarge: Failed - subChunk2Size expected: 4194304 actual: " << wavFile->subChunk2Size << "\n";
    else cout << "testConvolveLarge: Passed\n";
}

void unitTest(){
    cout << "----------------------------------------------------------------------------------------------\n";
    cout << "Unit Testing Mode\n";
    cout << "----------------------------------------------------------------------------------------------\n";
    testFileReadSmall();
    cout << "----------------------------------------------------------------------------------------------\n";
    testFileReadLarge();
    testConvolveSmall();
    testConvolveLarge();
    cout << "----------------------------------------------------------------------------------------------\n";
}