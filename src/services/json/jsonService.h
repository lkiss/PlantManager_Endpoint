#ifndef JSON_SERVICE
#define JSON_SERVICE

#include <ArduinoJson.h>
#include "../config/config.h"
#include "../sensor/sensorReading.h"

class JsonService
{
public:
  String convertSensorReadingsToJson(SensorReading sensorReading);
  String convertConfigToJson(Configuration config);
  Configuration convertJsonToConfig(String configJson);
};

#endif