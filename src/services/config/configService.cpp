#include "configService.h"

ConfigService::ConfigService()
{
}

ConfigService *ConfigService::getInstance()
{
  if (!instance)
    instance = new ConfigService();
  return instance;
}

String ConfigService::getConfigurationJson()
{
  return jsonService.convertConfigToJson(this->config);
}

void ConfigService::setConfigurationJson(String &config)
{
  jsonService.convertJsonToConfig(config, this->config);
}

Configuration *ConfigService::getConfiguration()
{
  return this->config;
}

void ConfigService::setConfiguration(Configuration config[])
{
  for (int deviceNumber = 0; deviceNumber < NUMBER_OF_DEVICES; deviceNumber++)
  {
    this->config[deviceNumber].WateringTimeInSeconds = config[deviceNumber].WateringTimeInSeconds;
    this->config[deviceNumber].SoilMoistureThreshold = config[deviceNumber].SoilMoistureThreshold;
    this->config[deviceNumber].MeasuringIntervalInMinutes = config[deviceNumber].MeasuringIntervalInMinutes;
  }
}

int ConfigService::getClosestWakeUpDelay()
{
  int closestWakeUpDelayInMinutes = 10080; // one week in minutes
  for (int deviceNumber = 0; deviceNumber < NUMBER_OF_DEVICES; deviceNumber++)
  {
    if (closestWakeUpDelayInMinutes > this->config[deviceNumber].MeasuringIntervalInMinutes)
    {
      closestWakeUpDelayInMinutes = this->config[deviceNumber].MeasuringIntervalInMinutes;
    }
  }

  return closestWakeUpDelayInMinutes;
}