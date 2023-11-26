struct WavFile {
    // RIFF Chunk Descriptor
    char chunkID[4];
    int chunkSize;
    char format[4];

    // FMT Sub-Chunk
    char subChunk1ID[4];
    int subChunk1Size;
    short audioFormat;
    short numChannels;
    int sampleRate;
    int byteRate;
    short blockAlign;
    short bitsPerSample;

    // Data Sub-Chunk
    char subChunk2ID[4];
    int subChunk2Size;
    float* data;
};

WavFile* readWavFile(const char* filePath);
WavFile* createWavFile(float* data, int dataSize);
void writeWavFile(WavFile* wavFile, const char* outFileName);
void printWavFile(WavFile* wavFile);

