#ifndef CONFIG_SERVICE
#define CONFIG_SERVICE

#include <Arduino.h>
#include "config.h"
#include "constants.h"
#include "../json/jsonService.h"

class ConfigService
{
private:
  static ConfigService *instance;
  JsonService jsonService;
  Configuration config;

public:
  ConfigService();
  static ConfigService *getInstance();

  String getConfigurationJson();
  void setConfigurationJson(String config);

  Configuration getConfiguration();
  void setConfiguration(Configuration config);
};

#endif