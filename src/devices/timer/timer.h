#ifndef TIMER
#define TIMER

#include <Wire.h>
#include <RtcDS3231.h>

class Timer
{
  private:
    RtcDS3231<TwoWire>* timer;

  public:
    Timer();
    void setTimeOffsetInMinutes(int timeOffsetInMinutes);
};
#endif