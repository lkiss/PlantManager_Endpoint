#ifndef WATER_PUMP_SENSOR
#define WATER_PUMP_SENSOR

#include <Arduino.h>
#include "../../constants.h"

class WaterPump
{
private:
  int newWateringTimeInSeconds = 5;
  int statusLedPin = -1;
  int waterPumpIndex = 0;

  void setupWaterPump();

public:
  WaterPump();
  WaterPump(int waterPumpIndex, int statusLedPin);
  bool activateWaterPump();
  void updateWateringTime(int &newWateringTimeInSeconds);
};
#endif