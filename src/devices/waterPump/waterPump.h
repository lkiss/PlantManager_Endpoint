#include <Arduino.h>
#ifndef WATER_PUMP_SENSOR
#define WATER_PUMP_SENSOR
class WaterPump
{
private:
  int waterPumpPin = 0;
  int wateringTime = 3000;

public:
  WaterPump();
  WaterPump(int *waterPumpPin);
  bool activateWaterPump();
  void updateWateringTime(int newWateringTime);
};
#endif