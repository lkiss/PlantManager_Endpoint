#ifndef CONFIG_SERVICE
#define CONFIG_SERVICE

#include <Arduino.h>
#include "config.h"
#include "../json/jsonService.h"

class ConfigService
{
private:
  static ConfigService *instance;
  JsonService jsonService;
  Configuration config[NUMBER_OF_DEVICES] = {{5, 750, 1}, {5, 750, 1}};

public:
  ConfigService();
  static ConfigService *getInstance();

  String getConfigurationJson();
  void setConfigurationJson(String &config);

  Configuration *getConfiguration();
  void setConfiguration(Configuration config[]);

  int getClosestWakeUpDelay();
};

#endif