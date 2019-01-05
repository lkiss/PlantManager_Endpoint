#ifndef UTILITY
#define UTILITY

#include <Arduino.h>

class Utility
{
  public:
    Utility();

    void oscillatePin(uint8_t pinNumber, int frequencyInMilliseconds, uint8_t repetitions);
};

#endif