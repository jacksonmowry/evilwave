#include <cmath>
#include <cstring>
#include <iostream>
#include <iterator>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
typedef int int32;
typedef unsigned int uint32;
typedef short int16;
typedef unsigned short uint16;
typedef signed char int8;
typedef unsigned char uint8;

#define CLAMP(value, min, max)                                                 \
  {                                                                            \
    if (value < min) {                                                         \
      value = min;                                                             \
    } else if (value > max) {                                                  \
      value = max;                                                             \
    }                                                                          \
  }

using namespace std;

// // Minimal struct requred for a .wav file header
// struct SMinimalWaveFileHeader {
//   // main chunk
//   unsigned char m_szChunkID[4];
//   uint32 m_nChunkSize;
//   unsigned char m_szFormat[4];

//   // sub chunk 1 "fmt"
//   unsigned char m_szSubChunk1ID[4];
//   uint32 m_nsubChunk1Size;
//   uint16 m_nAudioFormat;
//   uint16 m_nNumChannels;
//   uint32 m_nSampleRate;
//   uint32 m_nByteRate;
//   uint16 m_nBlockAlign;
//   uint16 m_nBitsPerSample;

//   // sub chunk 2 "data"
//   unsigned char m_szSubChunk2ID[4];
//   uint32 m_nSubChunk2Size;

//   // then comes the data!
// };

// 0 to 255
void ConvertFloatToAudioSample(float fFloat, uint8 &nOut) {
  fFloat = (fFloat + 1.0f) * 127.5f;
  CLAMP(fFloat, 0.0f, 255.0f);
  nOut = (uint8)fFloat;
}

//–32,768 to 32,767
void ConvertFloatToAudioSample(float fFloat, int16 &nOut) {
  fFloat *= 32767.0f;
  CLAMP(fFloat, -32768.0f, 32767.0f);
  nOut = (int16)fFloat;
}

//–2,147,483,648 to 2,147,483,647
void ConvertFloatToAudioSample(float fFloat, int32 &nOut) {
  double dDouble = (double)fFloat;
  dDouble *= 2147483647.0;
  CLAMP(dDouble, -2147483648.0, 2147483647.0);
  nOut = (int32)dDouble;
}

// float CalcFrequency(float fOctave, float fNote) {
//   // frequency = 440*(2^(n/12))
//   // n=0, A4; n=1, A#4;
//   return (float)(440 * pow(2.0, ((double)((fOctave - 4) * 12 + fNote)) / 12.0));
// }

// template <typename T>
// bool WriteWaveFile(const char *szFileName, float *pRawData, int32 nNumSamples,
//                    int16 nNumChannels, int32 nSampleRate) {
//   // open the file if we can
//   FILE *File = fopen(szFileName, "w+b");
//   if (!File) {
//     return false;
//   }

//   // calculate bits per sample and the data size
//   int32 nBitsPerSample = sizeof(T) * 8;
//   int nDataSize = nNumSamples * sizeof(T);

//   SMinimalWaveFileHeader waveHeader;

//   // fill out the main chunk
//   memcpy(waveHeader.m_szChunkID, "RIFF", 4);
//   waveHeader.m_nChunkSize = nDataSize + 36;
//   memcpy(waveHeader.m_szFormat, "WAVE", 4);

//   // fill out sub chunk 1 "fmt"
//   memcpy(waveHeader.m_szSubChunk1ID, "fmt ", 4);
//   waveHeader.m_nsubChunk1Size = 16;
//   waveHeader.m_nAudioFormat = 1;
//   waveHeader.m_nNumChannels = nNumChannels;
//   waveHeader.m_nSampleRate = nSampleRate;
//   waveHeader.m_nByteRate = nSampleRate * nNumChannels * nBitsPerSample / 8;
//   waveHeader.m_nBlockAlign = nNumChannels * nBitsPerSample / 8;
//   waveHeader.m_nBitsPerSample = nBitsPerSample;

//   // fill out sub chunk 2 "data"
//   memcpy(waveHeader.m_szSubChunk2ID, "data", 4);
//   waveHeader.m_nSubChunk2Size = nDataSize;

//   // write the header
//   fwrite(&waveHeader, sizeof(SMinimalWaveFileHeader), 1, File);

//   // write the data, converting from float to specified type
//   T *pData = new T[nNumSamples];
//   for (int nIndex = 0; nIndex < nNumSamples; ++nIndex) {
//     ConvertFloatToAudioSample(pRawData[nIndex], pData[nIndex]);
//   }
//   fwrite(pData, nDataSize, 1, File);
//   delete[] pData;

//   // close the file and return success
//   fclose(File);
//   return true;
// }

// // SINE WAVE
// float AdvanceOscilator_Sine(float &fPhase, float fFrequency,
//                             float fSampleRate) {
//   fPhase += 2 * (float)M_PI * fFrequency / fSampleRate;
//   while (fPhase >= 2 * (float)M_PI) {
//     fPhase -= 2 * (float)M_PI;
//   }
//   while (fPhase < 0) {
//     fPhase += 2 * (float)M_PI;
//   }
//   // return cos(fPhase);
//   return sin(fPhase);
// }

// float AdvanceOscilator_Square(float &fPhase, float fFrequency,
//                               float fSampleRate) {
//   fPhase += fFrequency / fSampleRate;
//   while (fPhase > 1.0f)
//     fPhase -= 1.0f;
//   while (fPhase < 0.0f)
//     fPhase += 1.0f;
//   if (fPhase <= 0.5f)
//     return -1.0f;
//   else
//     return 1.0f;
// }

// float AdvanceOscilator_Saw(float &fPhase, float fFrequency,
//                            float fSampleRate) {
//   fPhase += fFrequency/fSampleRate;
//   while(fPhase > 1.0f) fPhase -= 1.0f;
//   while(fPhase < 0.0f) fPhase += 1.0f;
//   return (fPhase * 2.0f) - 1.0f;
// }

// float AdvanceOscilator_Triangle(float &fPhase, float fFrequency,
//                                 float fSampleRate) {
//   fPhase += fFrequency / fSampleRate;
//   while(fPhase > 1.0f) fPhase -= 1.0f;
//   while(fPhase < 0.0f) fPhase += 1.0f;
//   float fRet;
//   if(fPhase <= 0.5f) fRet = fPhase * 2;
//   else fRet = (1.0f - fPhase) * 2;
//   return (fRet * 2.0f) - 1.0f;
// }

// float AdvanceOscilator_Noise(float &fPhase, float fFrequency, float fSampleRate, float fLastValue) {
//   unsigned int nLastSeed = (unsigned int)fPhase;
//   fPhase += fFrequency/fSampleRate;
//   unsigned int nSeed = (unsigned int)fPhase;

//   while(fPhase > 2.0f) fPhase -= 1.0f;

//   if(nSeed != nLastSeed) {
//     float fValue = ((float)rand() / ((float)RAND_MAX));
//     fValue = (fValue * 2.0f) - 1.0f;
//     // if(fValue < 0) fValue = -1.0f;
//     // else fValue = 1.0f;
//     return fValue;
//   }
//   else {
//     return fLastValue;
//   }
// }

// float temp(int index, const std::vector<std::pair<float, bool>> &input, float &phase, float sampleRate) {
//   int oscilator = input[0].first;
//   switch (oscilator) {
//   case 1:
//     return AdvanceOscilator_Sine(phase, input[index + 1].first, sampleRate);
//   case 2:
//     return AdvanceOscilator_Triangle(phase, input[index + 1].first, sampleRate);
//   case 3:
//     return AdvanceOscilator_Saw(phase, input[index + 1].first, sampleRate);
//   case 4:
//     return AdvanceOscilator_Square(phase, input[index + 1].first, sampleRate);
//   case 5:
//     return AdvanceOscilator_Noise(phase, input[index + 1].first, sampleRate, 0.0f);
//   default:
//     return 0;
//   }
// }
