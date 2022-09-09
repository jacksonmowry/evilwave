#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include "parse.hpp"
#include "write.cpp"
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
  // float fPhase2 = 0;
  // float fPhase3 = 0;
  // float fPhase4 = 0;
  // float fPhase5 = 0;


  int notePtr = 0;
  float tracker = 0.0f;

  // Text note input
  std::vector<std::string> output = readFile(filename);
  std::vector<std::vector<std::pair<float, bool>>> sheet = parseSheet(output);
  std::vector<float> sheetHD;
  printf("%f\n", CalcFrequency(-5, 0));
  exit(1);
  for (int i = 0; i < sheet.size(); i++)  {
    for (int j = 0; j < sheet[i].size(); j++) {
      if (j==0) {
        sheetHD.push_back(sheet[i][j].first);
        continue;
      }
      printf("Freq: %f Artic: %d\n", sheet[i][j].first, sheet[i][j].second);
      sheetHD.push_back((sheet[i][j].first));
      if (j < (sheet[i].size()-1) && sheet[i][j+1].second == true && sheet[i][j].first != 0) {
        printf("Cutting note short for artic\n");
        sheetHD.push_back(CalcFrequency(0, 0));
      }
      else {
        sheetHD.push_back(sheet[i][j].first);
      }
    }
  }

  for (int nIndex = 0; nIndex < nNumSamples; nIndex += 1) {
    // Selects the oscilator from text input
    // outputs frequency @ time
    // tracker++ increments the pointer of samples per desired note length
    pData[nIndex] = temp(notePtr, sheetHD, fPhase, fSampleRate);
    tracker++;
    // Moves to the next note based on desired note length
    if (tracker >= (5512.5f/(float)2)) {
      tracker = 0;
      notePtr++;
    }
    // Stop when we reach the end of the sheet
    if (notePtr >= (int)sheetHD.size() - 1) {
      break;
    }
  }

  WriteWaveFile<int32>("sine.wav", pData, nNumSamples, nNumChannels,
                       fSampleRate);

  delete[] pData;
}
