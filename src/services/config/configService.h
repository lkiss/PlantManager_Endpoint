#ifndef CONFIG_SERVICE
#define CONFIG_SERVICE

#include <Arduino.h>
#include "config.h"
#include "../json/jsonService.h"

class ConfigService
{
private:
  JsonService jsonService;
  Configuration config;
  int wakeUpDelays[NUMBER_OF_DEVICES];

public:
  ConfigService();

  String getConfigurationJson();
  void setConfigurationJson(String &config);

  Configuration getConfiguration();
  void setConfiguration(Configuration config);

  void addWakeUpDelay(int wakeUpDelay, int indexToPush);
  int getNextWakeUpDelay();
};

#endif