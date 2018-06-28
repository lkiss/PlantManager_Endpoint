#include "configService.h"

ConfigService::ConfigService()
{
  this->config.WateringTimeInSeconds = 5;
  this->config.SoilMoistureThreshold = 750;
  this->config.MeasuringIntervalInMinutes = 1;
  this->config.MaximumNumberOfWifiRestAttempts = 5;
  this->config.ShouldUseCloud = true;
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
  this->config = jsonService.convertJsonToConfig(config);
}

Configuration ConfigService::getConfiguration()
{
  return this->config;
}

void ConfigService::setConfiguration(Configuration config)
{
  this->config = config;
}