#include "configService.h"

ConfigService::ConfigService()
{
  this->config.measuringInterval = 10e6;
  this->config.smtpPort = 465;
  this->config.appServer = "http://192.168.1.103/szilahome/sensor";
  this->config.appServerPort = 8080;
  this->config.smtpServer = "smtp.gmail.com";
  this->config.base64UserId = "a2lzc2xhYzg4QGdtYWlsLmNvbQ==";
  this->config.base64Password = "NTExMzExMV9Db25z";
  this->config.emailTo = "kisslac1988@hotmail.com";
  this->config.emailFrom = "kisslac88@gmail.com";
  this->config.emailSubject = "Water tank empty!";
  this->config.emailBody = "Refill water tank!";
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