#ifndef DATA_SERVICE
#define DATA_SERVICE

#include "../sensor/sensorReading.h"
#include "../config/configService.h"
#include "../json/JsonService.h"

class DataService
{
private:
  ConfigService configService;
  JsonService jsonService;

public:
  DataService(ConfigService configService, JsonService jsonService);
  Configuration getConfigurationFromServer();
};

#endif