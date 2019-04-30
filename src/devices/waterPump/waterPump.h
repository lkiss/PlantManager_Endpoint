#include <Arduino.h>
#ifndef WATER_PUMP_SENSOR
#define WATER_PUMP_SENSOR
class WaterPump
{
private:
  int waterPumpPin = 0;
  int newWateringTimeInSeconds = 5;
  int statusLedPin = -1;

  void setupWaterPump();

public:
  WaterPump();
  WaterPump(int waterPumpPin);
  WaterPump(int waterPumpPin, int statusLedPin);
  bool activateWaterPump();
  void updateWateringTime(int &newWateringTimeInSeconds);
};
#endif