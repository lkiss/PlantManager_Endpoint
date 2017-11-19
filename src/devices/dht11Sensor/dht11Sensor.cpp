#include <SimpleDHT.h>
#include "dht11Sensor.h"

SimpleDHT11 dht11;

Dht11Sensor::Dht11Sensor() {}

Dht11Sensor::Dht11Sensor(int *DHT11Pin)
{
  this->DHT11Pin = *DHT11Pin;
}

Dht11SensorReading Dht11Sensor::read()
{
  byte temperature = 0;
  byte humidity = 0;
  Dht11SensorReading reading;
  delay(3000);
  
  reading.dht11ErrorCode = dht11.read(this->DHT11Pin, &temperature, &humidity, NULL);
  reading.temperatureInCelsius = (int)temperature;
  reading.humidity = (int)humidity;

  return reading;
}