#include "dataService.h"

DataService::DataService(ConfigService configService, JsonService jsonService)
{
    this->configService = configService;
    this->jsonService = jsonService;
}

int DataService::sendSensorReadings(SensorReading reading)
{
    String jsonMessage = this->jsonService.convertSensorReadingsToJson(reading);

    return 200;
}

Configuration DataService::getConfigurationFromServer()
{
    Configuration config = this->configService.getConfiguration();

    return config;
}