#ifndef DHT11_SENSOR
#define DHT11_SENSOR

#include "DHT.h"
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