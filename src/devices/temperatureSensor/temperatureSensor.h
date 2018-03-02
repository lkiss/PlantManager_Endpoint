#ifndef DHT11_SENSOR
#define DHT11_SENSOR

#include "./temperatureSensorReading.h"

class TemperatureSensor
{
private:
  int temperatureSensorPin;

public:
  TemperatureSensor();
  TemperatureSensor(int *temperatureSensorPin);
  TemperatureSensorReading read();
};
#endif