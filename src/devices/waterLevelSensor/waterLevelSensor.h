#ifndef WATER_LEVEL_SENSOR
#define WATER_LEVEL_SENSOR

class WaterLevelSensor
{
private:
  int triggerPin = 0;
  int echoPin = 0;

public:
  WaterLevelSensor();
  WaterLevelSensor(int triggerPin, int echoPin);
  long getMissingWaterColumHeighCM();
};
#endif