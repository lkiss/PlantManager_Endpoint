#ifndef RTC
#define RTC

#include <RtcDS3231.h>
#include <Wire.h>

class RealTimeClock
{
private:
  RtcDS3231<TwoWire>* Rtc;

public:
  RealTimeClock();
  void UpdateTimer(int mearuringIntervalInMinutes);
  void InitializeRtc();
};
#endif