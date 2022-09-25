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
  cout<<notesPerLine("A4 D4 E4 F4 D4 A5 G4")<<endl;
  // filename
  std::string filename;
  if (argc > 1) {
    filename = argv[1];
  }
  else {
      fprintf(stderr, "Please add an input file\n");
      exit(EXIT_FAILURE);
  }
  // phase ocilator
  float fPhase = 0;
  // Note and Sample tracker
  int notePtr = 0;
  float tracker = 0.0f;
  // Text note input
  textMusic output = readFile(filename); //TODO: extract notes per line/bar here
  // int notesPerBar =
  std::vector<std::vector<std::pair<float, bool>>> sheet = parseSheet(output.notes);
  std::vector<std::vector<float>> sheetHD = toHD(sheet);
  // .wav parameters
  float sampleRate = 44100;
  // int numSeconds = 34; // TODO: Rewrite so this is auto calculated, # of notes * samples per note / samplerate
  unsigned int numSeconds = (float)(sheet[0].size() - 1) * (5512.5f*2.0f) / sampleRate;
  printf("seconds: %i\n",numSeconds);
  int channels = sheetHD.size();
  // Creating the data chunk
  int nNumSamples = sampleRate * channels * numSeconds;
  printf("here\n");
  float *pData = new float[nNumSamples];
  printf("after here\n");
  // Writing each track in its own pass
  for (int track = 0; track < sheetHD.size(); track++) {
    for (int nIndex = 0; nIndex < nNumSamples; nIndex += sheetHD.size()) {
      pData[nIndex + track] = temp(notePtr, sheetHD[track], fPhase, sampleRate);
      tracker += 1;
      // if (tracker >= (5512.5f / (float)2)) {
      if (tracker >= (5512.5f)) {
        tracker = 0;
        notePtr++;
      }
      if (notePtr >= (int)sheetHD[track].size() - 1) {
        break;
      }
    }
    // Reset values specific to each track
    tracker = 0;
    notePtr = 0;
    fPhase = 0;
  }

  WriteWaveFile<int32>("sine.wav", pData, nNumSamples, channels,
                       sampleRate);

  delete[] pData;
}
