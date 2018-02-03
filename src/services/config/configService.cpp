#include "configService.h"

ConfigService::ConfigService()
{
  this->config.WateringTime = 5;
  this->config.SoilMoistureThreshold = 750;
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

void ConfigService::setConfigurationJson(String config)
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