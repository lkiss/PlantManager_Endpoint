#include <Arduino.h>

#include "SoilMoistureSensor.h"

SoilMoistureSensor::SoilMoistureSensor() {}
SoilMoistureSensor::SoilMoistureSensor(int soilMoisturePin)
{
  this->soilMoisturePin = soilMoisturePin;
}

bool SoilMoistureSensor::isDry()
{
  int sensorReading = this->read();

  if (sensorReading >= this->soilMoistureWetTreshold)
  {
    return true;
  }
  return false;
}

bool SoilMoistureSensor::isDry(int soilMoistureValue)
{
  if (soilMoistureValue >= this->soilMoistureWetTreshold)
  {
    return true;
  }
  return false;
}

bool SoilMoistureSensor::isValidMoistureTreshold(int moistureTreshold)
{
  if (moistureTreshold > 1024 || moistureTreshold < 0)
  {
    return false;
  }
  return true;
}

int SoilMoistureSensor::read()
{
  //Log debug
  int result = 0;
  delay(50);
  result = analogRead(this->soilMoisturePin);
  delay(50);
  return result;
}

void SoilMoistureSensor::updateTresholdValues(int newTreshold)
{
  if (this->isValidMoistureTreshold(newTreshold) && this->isValidMoistureTreshold(newTreshold))
  {
    this->soilMoistureWetTreshold = newTreshold;
  }
}