#include "configService.h"

ConfigService::ConfigService()
{
}

String ConfigService::getConfigurationJson()
{
  return jsonService.convertConfigToJson(this->config);
}

void ConfigService::setConfigurationJson(String &config)
{
  this->config = jsonService.convertJsonToConfig(config);
}

Configuration ConfigService::getConfiguration()
{
  return this->config;
}

void ConfigService::setConfiguration(Configuration config)
{
  for (int deviceNumber = 0; deviceNumber < NUMBER_OF_DEVICES; deviceNumber++)
  {
    this->config.WateringTimeInSeconds = config.WateringTimeInSeconds;
    this->config.SoilMoistureThreshold = config.SoilMoistureThreshold;
    this->config.MeasuringIntervalInMinutes = config.MeasuringIntervalInMinutes;
  }
}

void ConfigService::addWakeUpDelay(int wakeUpDelay, int indexToPush)
{
  this->wakeUpDelays[indexToPush] = wakeUpDelay;
}

int ConfigService::getNextWakeUpDelay()
{
  int closestWakeUpDelayInMinutes = 10080; // one week in minutes
  for (int deviceNumber = 0; deviceNumber < NUMBER_OF_DEVICES; deviceNumber++)
  {
    if (closestWakeUpDelayInMinutes > wakeUpDelays[deviceNumber])
    {
      closestWakeUpDelayInMinutes = wakeUpDelays[deviceNumber];
    }
  }
  // Serial.println("Closest wakeup delay");
  // Serial.println(closestWakeUpDelayInMinutes);
  return closestWakeUpDelayInMinutes;
}