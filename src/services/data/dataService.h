#ifndef DATA_SERVICE
#define DATA_SERVICE

#include "../sensor/sensorReading.h"
#include "../config/configService.h"
#include "../json/JsonService.h"

class DataService
{
private:
  ConfigService configurationService;
  JsonService jsonService;

public:
  DataService(ConfigService configService, JsonService jsonService);
  int sendSensorReadings(SensorReading reading);
  Configuration getConfigurationFromServer();
};

#endif