#include <SimpleDHT.h>
#include "temperatureSensor.h"

SimpleDHT11 dht11;

TemperatureSensor::TemperatureSensor() {}

TemperatureSensor::TemperatureSensor(int *temperatureSensorPin)
{
  this->temperatureSensorPin = *temperatureSensorPin;
}

TemperatureSensorReading TemperatureSensor::read()
{
  byte temperature = 0;
  byte humidity = 0;
  TemperatureSensorReading reading;
  delay(3000);
  
  reading.dht11ErrorCode = dht11.read(this->temperatureSensorPin, &temperature, &humidity, NULL);
  reading.temperatureInCelsius = (int)temperature;
  reading.humidity = (int)humidity;

  return reading;
}