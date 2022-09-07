#include "main.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include "parse.hpp"
#include "oscilators.cpp"

int main(int argc, char *argv[]) {
  // filename
  std::string filename;
  if (argc > 1) {
    filename = argv[1];
  }
  else {
      fprintf(stderr, "Please add an input file\n");
      exit(EXIT_FAILURE);
  }

  float fSampleRate = 44100;
  int nNumSeconds = 45;
  int nNumChannels = 1;

  int nNumSamples = fSampleRate * nNumChannels * nNumSeconds;
  float *pData = new float[nNumSamples];

  // phase ocilator
  float fPhase = 0;
  float fPhase2 = 0;
  // float fPhase3 = 0;
  // float fPhase4 = 0;
  // float fPhase5 = 0;


  int notePtr = 0;
  float tracker = 0.0f;

  // Text note input
  std::vector<std::string> output = readFile(filename);
  std::vector<std::vector<std::pair<float, bool>>> sheet = parseSheet(output);



  for (int nIndex = 0; nIndex < nNumSamples; nIndex += 1) {
    // if (tracker == 0 && sheet[0][notePtr].second == true && notePtr != 0) {
    //   int backwardStep = 1000; // 500 samples backwards
    //   for (int i = backwardStep; i > 0; i--) {
    //     float tmp = pData[nIndex-i];
    //     float data = (float)tmp * (float)(((float)i/(float)backwardStep));
    //     pData[nIndex-i] = data;
    //   }
    //   fPhase = 0;
    // }
    // printf("Mine: %f\n", temp(notePtr, sheet[0], fPhase, fSampleRate));
    // printf("Other: %f\n", AdvanceOscilator_Sine(fPhase2, sheet[0][notePtr].first, fSampleRate));
    // pData[nIndex] = temp(notePtr, sheet[0], fPhase, fSampleRate);
    // pData[nIndex] = AdvanceOscilator_Sine(fPhase, sheet[0][notePtr].first, fSampleRate);
    // pData[nIndex+1] = temp(notePtr, sheet[1], &fPhase2, fSampleRate) * 0.1f;
    // pData[nIndex+2] = temp(notePtr, sheet[2], &fPhase3, fSampleRate) * 0.1f;
    // pData[nIndex+3] = temp(notePtr, sheet[3], &fPhase4, fSampleRate) * 0.1f;
    // pData[nIndex+4] = temp(notePtr, sheet[4], &fPhase5, fSampleRate) * 0.1f;
    tracker++;                  //
    if (tracker >= (5512.5f)) {
      tracker = 0;
      notePtr++;
    }
    if (notePtr >= (int)sheet[0].size() - 1) {
      break;
    }
  }

  WriteWaveFile<int32>("sine.wav", pData, nNumSamples, nNumChannels,
                       fSampleRate);

  delete[] pData;
}
