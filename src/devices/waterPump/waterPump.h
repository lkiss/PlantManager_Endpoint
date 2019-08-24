#ifndef WATER_PUMP_SENSOR
#define WATER_PUMP_SENSOR

#include <Arduino.h>
#include "../../constants.h"

class WaterPump
{
private:
  int waterPumpPin = MUX_COMMON_PIN;
  int newWateringTimeInSeconds = 5;
  int statusLedPin = -1;
  int waterPumpIndex = 0;

  void setupWaterPump();

public:
  WaterPump();
  WaterPump(int waterPumpPin);
  WaterPump(int waterPumpIndex, int statusLedPin);
  bool activateWaterPump();
  void updateWateringTime(int &newWateringTimeInSeconds);
};
#endif