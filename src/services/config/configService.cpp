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
  for (int endpointIndex = 0; endpointIndex < NUMBER_OF_DEVICES; endpointIndex++)
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
  for (int endpointIndex = 0; endpointIndex < NUMBER_OF_DEVICES; endpointIndex++)
  {
    if (closestWakeUpDelayInMinutes > wakeUpDelays[endpointIndex])
    {
      closestWakeUpDelayInMinutes = wakeUpDelays[endpointIndex];
    }
  }
  // Serial.println("Closest wakeup delay");
  // Serial.println(closestWakeUpDelayInMinutes);
  return closestWakeUpDelayInMinutes;
}