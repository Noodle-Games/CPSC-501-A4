#include "WavReader.h"
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

size_t fwriteIntLSB(int data, FILE *stream){
    unsigned char array[4];
    array[3] = (unsigned char)((data >> 24) & 0xFF);
    array[2] = (unsigned char)((data >> 16) & 0xFF);
    array[1] = (unsigned char)((data >> 8) & 0xFF);
    array[0] = (unsigned char)(data & 0xFF);
    return fwrite(array, sizeof(unsigned char), 4, stream);
}

size_t fwriteShortLSB(short int data, FILE *stream){
    unsigned char array[2];
    array[1] = (unsigned char)((data >> 8) & 0xFF);
    array[0] = (unsigned char)(data & 0xFF);
    return fwrite(array, sizeof(unsigned char), 2, stream);
}

WavFile* readWavFile(const char* filePath){
    WavFile* wavFile = new WavFile();
    FILE* inputFile = fopen(filePath, "r");

    fread(wavFile->chunkID, sizeof(char), 4, inputFile);
    fread(&wavFile->chunkSize, sizeof(int), 1, inputFile);
    fread(wavFile->format, sizeof(char), 4, inputFile);
    fread(wavFile->subChunk1ID, sizeof(char), 4, inputFile);
    fread(&wavFile->subChunk1Size, sizeof(int), 1, inputFile);
    fread(&wavFile->audioFormat, sizeof(short), 1, inputFile);
    fread(&wavFile->numChannels, sizeof(short), 1, inputFile);
    fread(&wavFile->sampleRate, sizeof(int), 1, inputFile);
    fread(&wavFile->byteRate, sizeof(int), 1, inputFile);
    fread(&wavFile->blockAlign, sizeof(short), 1, inputFile);
    if (wavFile->subChunk1Size == 16) fread(&wavFile->bitsPerSample, sizeof(short), 1, inputFile);
    else fread(&wavFile->bitsPerSample, sizeof(int), 1, inputFile);
    fread(wavFile->subChunk2ID, sizeof(char), 4, inputFile);
    fread(&wavFile->subChunk2Size, sizeof(int), 1, inputFile);

    short* rawData = (short*) malloc(sizeof(short) * wavFile->subChunk2Size);
    fread(rawData, sizeof(short), wavFile->subChunk2Size, inputFile);

    wavFile->data = (float*) malloc(sizeof(float) * wavFile->subChunk2Size);
    for(int i = 0; i < wavFile->subChunk2Size; i++){
        wavFile->data[i] = (float) rawData[i] / 32768.0f;
    }

    cout << "Input File: " + (string)filePath + "\n";
    printWavFile(wavFile);
    return wavFile;
}

WavFile* createWavFile(float* data, int dataSize) {
    WavFile *wavFile = new WavFile();
    strcpy(wavFile->chunkID, "RIFF");
    wavFile->chunkSize = 38 + dataSize;
    strcpy(wavFile->format, "WAVE");
    strcpy(wavFile->subChunk1ID, "fmt ");
    wavFile->subChunk1Size = 16;
    wavFile->audioFormat = 1;
    wavFile->numChannels = 1;
    wavFile->sampleRate = 44100;
    wavFile->byteRate = 88200;
    wavFile->blockAlign = 2;
    wavFile->bitsPerSample = 16;
    strcpy(wavFile->subChunk2ID, "data");
    wavFile->subChunk2Size = dataSize;
    wavFile->data = data;
    return wavFile;
}

void writeWavFile(WavFile* wavFile, const char* outFileName){
    // Write Header
    FILE* outputFile = fopen(outFileName, "wb");
    if (outputFile == nullptr) cout << "Failed to create output file!\n";
    fputs("RIFF", outputFile);
    fwriteIntLSB(38 + wavFile->subChunk2Size, outputFile);
    fputs("WAVE", outputFile);
    fputs("fmt ", outputFile);
    fwriteIntLSB(16, outputFile);       // ****
    fwriteShortLSB(1, outputFile);
    fwriteShortLSB((short)1, outputFile);
    fwriteIntLSB((int)44100, outputFile);
    fwriteIntLSB(88200, outputFile);
    fwriteShortLSB(2, outputFile);
    fwriteShortLSB(16, outputFile);
    fputs("data", outputFile);
    fwriteIntLSB(wavFile->subChunk2Size, outputFile);

    // Write Data
    for(int i = 0; i < wavFile->subChunk2Size; i++){
        fwriteShortLSB((short)(wavFile->data[i] * 32768.f * 0.4f), outputFile);
    }
    fclose(outputFile);
    cout << "Output File: " + (string)outFileName + "\n";
    printWavFile(wavFile);

}

void printWavFile(WavFile* wavFile){
    cout << "Chunk ID: ";
    cout << wavFile->chunkID[0];
    cout << wavFile->chunkID[1];
    cout << wavFile->chunkID[2];
    cout << wavFile->chunkID[3];
    cout << "\n";
    cout << "Chunk Size: " + to_string(wavFile->chunkSize) + "\n";
    cout << "Format: ";
    cout << wavFile->format[0];
    cout << wavFile->format[1];
    cout << wavFile->format[2];
    cout << wavFile->format[3];
    cout << "\n";
    cout << "---------\n";
    cout << "SubChunk1 ID: ";
    cout << wavFile->subChunk1ID[0];
    cout << wavFile->subChunk1ID[1];
    cout << wavFile->subChunk1ID[2];
    cout << wavFile->subChunk1ID[3];
    cout << "\n";
    cout << "SubChunk1 Size: " + to_string(wavFile->subChunk1Size) + "\n";
    cout << "Audio Format: " + to_string(wavFile->audioFormat) + "\n";
    cout << "Num Channels: " + to_string(wavFile->numChannels) + "\n";
    cout << "Sample Rate: " + to_string(wavFile->sampleRate) + "\n";
    cout << "Byte Rate: " + to_string(wavFile->byteRate) + "\n";
    cout << "Block Align: " + to_string(wavFile->blockAlign) + "\n";
    cout << "Bits Per Sample: " + to_string(wavFile->bitsPerSample) + "\n";
    cout << "---------\n";
    cout << "SubChunk2 ID: ";
    cout << wavFile->subChunk2ID[0];
    cout << wavFile->subChunk2ID[1];
    cout << wavFile->subChunk2ID[2];
    cout << wavFile->subChunk2ID[3];
    cout << "\n";
    cout << "SubChunk2 Size: " + to_string(wavFile->subChunk2Size) + "\n";
    cout << "----------------------------------------------------------------------------------------------\n";
}