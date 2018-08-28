#ifndef JSON_SERVICE
#define JSON_SERVICE

#include <ArduinoJson.h>
#include <Arduino.h>
#include "../config/config.h"
#include "../sensor/sensorReading.h"
#include "../../constants.h"

class JsonService
{
public:
  void printSensorReadingJson(SensorReading *sensorReadings);
  String convertConfigToJson(Configuration *config);
  void convertJsonToConfig(String &configJson, Configuration *configuration);
};

#endif