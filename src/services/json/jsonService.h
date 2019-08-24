#ifndef JSON_SERVICE
#define JSON_SERVICE

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../config/config.h"
#include "../sensor/sensorReading.h"
#include "../../constants.h"

class JsonService
{
public:
  void printSensorReadingJson(SensorReading sensorReadings);
  String convertConfigToJson(Configuration config);
  Configuration convertJsonToConfig(String &configJson);
};

#endif