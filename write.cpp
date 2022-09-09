#include <memory.h>
#include <iostream>
#include "main.hpp"

// Minimal struct requred for a .wav file header
struct SMinimalWaveFileHeader {
  // main chunk
  unsigned char m_szChunkID[4];
  uint32 m_nChunkSize;
  unsigned char m_szFormat[4];

  // sub chunk 1 "fmt"
  unsigned char m_szSubChunk1ID[4];
  uint32 m_nsubChunk1Size;
  uint16 m_nAudioFormat;
  uint16 m_nNumChannels;
  uint32 m_nSampleRate;
  uint32 m_nByteRate;
  uint16 m_nBlockAlign;
  uint16 m_nBitsPerSample;

  // sub chunk 2 "data"
  unsigned char m_szSubChunk2ID[4];
  uint32 m_nSubChunk2Size;

  // then comes the data!
};

template <typename T>
bool WriteWaveFile(const char *szFileName, float *pRawData, int nNumSamples,
                   short nNumChannels, int nSampleRate) {
  // open the file if we can
  FILE *File = fopen(szFileName, "w+b");
  if (!File) {
    return false;
  }

  // calculate bits per sample and the data size
  int32 nBitsPerSample = sizeof(T) * 8;
  int nDataSize = nNumSamples * sizeof(T);

  SMinimalWaveFileHeader waveHeader;

  // fill out the main chunk
  memcpy(waveHeader.m_szChunkID, "RIFF", 4);
  waveHeader.m_nChunkSize = nDataSize + 36;
  memcpy(waveHeader.m_szFormat, "WAVE", 4);

  // fill out sub chunk 1 "fmt"
  memcpy(waveHeader.m_szSubChunk1ID, "fmt ", 4);
  waveHeader.m_nsubChunk1Size = 16;
  waveHeader.m_nAudioFormat = 1;
  waveHeader.m_nNumChannels = nNumChannels;
  waveHeader.m_nSampleRate = nSampleRate;
  waveHeader.m_nByteRate = nSampleRate * nNumChannels * nBitsPerSample / 8;
  waveHeader.m_nBlockAlign = nNumChannels * nBitsPerSample / 8;
  waveHeader.m_nBitsPerSample = nBitsPerSample;

  // fill out sub chunk 2 "data"
  memcpy(waveHeader.m_szSubChunk2ID, "data", 4);
  waveHeader.m_nSubChunk2Size = nDataSize;

  // write the header
  fwrite(&waveHeader, sizeof(SMinimalWaveFileHeader), 1, File);

  // write the data, converting from float to specified type
  T *pData = new T[nNumSamples];
  for (int nIndex = 0; nIndex < nNumSamples; ++nIndex) {
    ConvertFloatToAudioSample(pRawData[nIndex], pData[nIndex]);
  }
  fwrite(pData, nDataSize, 1, File);
  delete[] pData;

  // close the file and return success
  fclose(File);
  return true;
}
