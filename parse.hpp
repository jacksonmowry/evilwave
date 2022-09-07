#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <system_error>
#include <utility>
#include <vector>
#include <string>
std::vector<std::string> readFile(std::string fileName) {
  std::ifstream text(fileName);
  std::string note;
  std::vector<std::string> output(0);
  // while (!text.eof()) {
  while (text.peek() != EOF) {
    if (text >> note) output.push_back(note);
  }
  return output;
}

float CalcFrequency(float fOctave, float fNote) {
  // frequency = 440*(2^(n/12))
  // n=0, A4; n=1, A#4;
  return (float)(440 *
                 std::pow(2.0, ((double)((fOctave - 4) * 12 + fNote)) / 12.0));
}

std::pair<float, float> lookup(float octave, std::string note) {
  std::pair<float, float> pitch;
  float pitchNum;
  if (note == "A") pitchNum = 0;
  else if (note == "A#" || note == "Bb") pitchNum = 1.0f;
  else if (note == "B" || note == "Cb") pitchNum = 2;
  else if (note == "C" || note == "B#") pitchNum = 3;
  else if (note == "C#" || note == "Db") pitchNum = 4;
  else if (note == "D") pitchNum = 5;
  else if (note == "D#" || note == "Eb") pitchNum = 6;
  else if (note == "E" || note == "Fb") pitchNum = 7;
  else if (note == "F" || note == "E#") pitchNum = 8;
  else if (note == "F#" || note == "Gb") pitchNum = 9;
  else if (note == "G") pitchNum = 10;
  else if (note == "G#" || note == "Ab") pitchNum = 11;
  return std::make_pair(octave, pitchNum);
}

std::vector<std::vector<std::pair<float, bool>>> parseSheet(std::vector<std::string> text) {
  std::vector<std::vector<std::pair<float, bool>>> output(0);
  std::string part = "1";
  char partC = '1';
  std::string note;
  std::string octave;
  bool articulation;
  std::vector<std::pair<float, bool>> temp(0);
  for (std::string s : text) {
    if (s == part) {
      part = ++partC;
      if (temp.size() > 0) {
        output.push_back(temp);
      }
      temp.clear();
      continue;
    } else if (s == "SINE") {
      temp.push_back(std::make_pair(1, 0));
      continue;
    } else if (s == "TRIANGLE") {
      temp.push_back(std::make_pair(2, 0));
      continue;
    } else if (s == "SAW") {
      temp.push_back(std::make_pair(3, 0));
      continue;
    } else if (s == "SQUARE") {
      temp.push_back(std::make_pair(4, 0));
      continue;
    } else if (s == "NOISE") {
      temp.push_back(std::make_pair(5, 0));
      continue;
    } else if (s == "XX") {
      temp.push_back(std::make_pair(0, 0));
      continue;
    } else if (s == "--") {
      // Repeat last note
      articulation = false;
    } else if (s.length() == 2 && (s[0] > 64 && s[0] < 72) && (s[1] > 47 && s[1] < 58)) {
      note = s[0];
      octave = s[1];
      articulation = true;
    } else if (s.length() == 3 && (s[0] > 64 && s[0] < 72) && (s[1] == '#' || s[1] == 'b') && (s[2] > 47 && s[2] < 58)){
      note = s.substr(0, 2);
      octave = s[2];
      articulation = true;
    } else {
      fprintf(stderr, "Error parsing token: %s\n", s.c_str());
      exit(EXIT_FAILURE);
    }
    float octaveF = std::atof(&octave[0]);
    std::pair<float, float> pitch = lookup(octaveF, note);
    temp.push_back(std::make_pair(CalcFrequency(pitch.first, pitch.second), articulation));
  }
  output.push_back(temp);
  return output;
}

// std::vector<std::pair<int, int>> tempNotes;
// std::vector<std::vector<std::pair<int, int>>> notes;
