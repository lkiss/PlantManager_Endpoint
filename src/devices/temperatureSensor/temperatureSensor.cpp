// #include <SimpleDHT.h>
#include "temperatureSensor.h"

DHT dht;

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
  dht.setup(this->temperatureSensorPin);

  delay(3000);
  
  reading.dht11ErrorCode = 0;
  reading.temperatureInCelsius = (int)dht.getTemperature();
  reading.humidity = (int)dht.getHumidity();

  return reading;
}