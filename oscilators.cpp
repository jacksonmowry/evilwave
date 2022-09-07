#include "oscilators.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

// SINE WAVE
float AdvanceOscilator_Sine(float &fPhase, float fFrequency,
                            float fSampleRate) {
  fPhase += 2 * (float)M_PI * fFrequency / fSampleRate;
  while (fPhase >= 2 * (float)M_PI) {
    fPhase -= 2 * (float)M_PI;
  }
  while (fPhase < 0) {
    fPhase += 2 * (float)M_PI;
  }
  // return cos(fPhase);
  return sin(fPhase);
}

float AdvanceOscilator_Square(float &fPhase, float fFrequency,
                              float fSampleRate) {
  fPhase += fFrequency / fSampleRate;
  while (fPhase > 1.0f)
    fPhase -= 1.0f;
  while (fPhase < 0.0f)
    fPhase += 1.0f;
  if (fPhase <= 0.5f)
    return -1.0f;
  else
    return 1.0f;
}

float AdvanceOscilator_Saw(float &fPhase, float fFrequency,
                           float fSampleRate) {
  fPhase += fFrequency/fSampleRate;
  while(fPhase > 1.0f) fPhase -= 1.0f;
  while(fPhase < 0.0f) fPhase += 1.0f;
  return (fPhase * 2.0f) - 1.0f;
}

float AdvanceOscilator_Triangle(float &fPhase, float fFrequency,
                                float fSampleRate) {
  fPhase += fFrequency / fSampleRate;
  while(fPhase > 1.0f) fPhase -= 1.0f;
  while(fPhase < 0.0f) fPhase += 1.0f;
  float fRet;
  if(fPhase <= 0.5f) fRet = fPhase * 2;
  else fRet = (1.0f - fPhase) * 2;
  return (fRet * 2.0f) - 1.0f;
}

float AdvanceOscilator_Noise(float &fPhase, float fFrequency, float fSampleRate, float fLastValue) {
  unsigned int nLastSeed = (unsigned int)fPhase;
  fPhase += fFrequency/fSampleRate;
  unsigned int nSeed = (unsigned int)fPhase;

  while(fPhase > 2.0f) fPhase -= 1.0f;

  if(nSeed != nLastSeed) {
    float fValue = ((float)rand() / ((float)RAND_MAX));
    fValue = (fValue * 2.0f) - 1.0f;
    // if(fValue < 0) fValue = -1.0f;
    // else fValue = 1.0f;
    return fValue;
  }
  else {
    return fLastValue;
  }
}
