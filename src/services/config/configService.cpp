#include "configService.h"

ConfigService::ConfigService()
{
  this->config.sensorId = "72fb5192-a399-4e74-96e1-1a40cf7f934d";
  this->config.sensorType = TYPE01;
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