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
#pragma once

struct textMusic {
  std::vector<std::string> notes;
  float bpm;
  int subdivision;
};

int notesPerLine(const std::string& line) {
  std::stringstream ss(line);
  int count = 0;
  std::string s;
  while(ss>>s) count++;
  return count;
}

textMusic readFile(std::string fileName) {
  textMusic output;
  std::ifstream text(fileName);
  std::string note;
  std::vector<std::string> noteVec(0);
  text >> output.bpm;
  text >> output.subdivision;
  while (text.peek() != EOF) {
    if (text >> note) noteVec.push_back(note);
  }
  output.notes = noteVec;
  text.close();
  return output;
}

float CalcFrequency(float fOctave, float fNote) {
  // frequency = 440*(2^(n/12))
  // n=0, A4; n=1, A#4;
  return (float)(440 *
                 std::pow(2.0, ((double)((fOctave - 4) * 12 + fNote)) / 12.0));
}

std::pair<float, float> lookup(float octave, const std::string& note) {
  // std::pair<float, float> pitch;
  float pitchNum;
  if (note == "A" || note == "a") pitchNum = 0;
  else if (note == "A#" || note == "a#" || note == "Bb" || note == "bb") pitchNum = 1.0f;
  else if (note == "B" || note == "b" || note == "Cb" || note == "cb") pitchNum = 2;
  else if (note == "C" || note == "c" || note == "B#" || note == "b#") pitchNum = 3;
  else if (note == "C#" || note == "c#" || note == "Db" || note == "db") pitchNum = 4;
  else if (note == "D" || note == "d") pitchNum = 5;
  else if (note == "D#" || note == "d#" || note == "Eb" || note == "eb") pitchNum = 6;
  else if (note == "E" || note == "e" || note == "Fb" || note == "fb") pitchNum = 7;
  else if (note == "F" || note == "f" || note == "E#" || note == "e#") pitchNum = 8;
  else if (note == "F#" || note == "f#" || note == "Gb" || note == "gb") pitchNum = 9;
  else if (note == "G" || note == "g") pitchNum = 10;
  else if (note == "G#" || note == "g#" || note == "Ab" || note == "ab") pitchNum = 11;
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
      articulation = false;
    } else if (s.length() == 3 && (s[0] > 64 && s[0] < 72) && (s[1] == '#' || s[1] == 'b') && (s[2] > 47 && s[2] < 58)){
      note = s.substr(0, 2);
      octave = s[2];
      articulation = false;
    } else if (s.length() == 2 && (s[0] > 96 && s[0] < 104) && (s[1] > 47 && s[1] < 58)) {
      note = s[0];
      octave = s[1];
      articulation = true;
    } else if (s.length() == 3 && (s[0] > 96 && s[0] < 104) && (s[1] == '#' || s[1] == 'b') && (s[2] > 47 && s[2] < 58)){
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

std::vector<std::vector<float>> toHD(std::vector<std::vector<std::pair<float,bool>>> sheet) {
  std::vector<std::vector<float>> hd(0);
  // Outer loop through vectors of pairs
  for (int i = 0; i < sheet.size(); i++)  {
    // Inner loop for pairs
    std::vector<float> temp(0);
    for (int j = 0; j < sheet[i].size(); j++) {
      // If we are at the beginning we need to push
      // back the number for each oscialtor
      if (j==0) {
        // hd[i].push_back(sheet[i][j].first);
        temp.push_back(sheet[i][j].first);
        continue;
      }
      // Push back the original note, then if we have an
      // upcoming articulation we cut it short, otherwise
      // repeat the last note
      // hd[i].push_back((sheet[i][j].first));
      temp.push_back((sheet[i][j].first));
      if (j < (sheet[i].size()-1) && sheet[i][j+1].second == true && sheet[i][j].first != 0) {
        // hd[i].push_back(0);
        temp.push_back(0);
      }
      else {
        // hd[i].push_back(sheet[i][j].first);
        temp.push_back(sheet[i][j].first);
      }
    }
    hd.push_back(temp);
  }
  return hd;
}

