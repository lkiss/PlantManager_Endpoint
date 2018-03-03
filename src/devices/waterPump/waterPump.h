#include <Arduino.h>
#ifndef WATER_PUMP_SENSOR
#define WATER_PUMP_SENSOR
class WaterPump
{
private:
  int waterPumpPin = 0;
  int newWateringTimeInSeconds = 5;

public:
  WaterPump();
  WaterPump(int *waterPumpPin);
  bool activateWaterPump();
  void updateWateringTime(int newWateringTimeInSeconds);
};
#endif