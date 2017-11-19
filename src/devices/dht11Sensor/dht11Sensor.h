#ifndef DHT11_SENSOR
#define DHT11_SENSOR

#include "./dht11SensorReading.h"

class Dht11Sensor
{
private:
  int DHT11Pin;

public:
  Dht11Sensor();
  Dht11Sensor(int *DHT11Pin);
  Dht11SensorReading read();
};
#endif