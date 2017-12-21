#include "dataService.h"

DataService::DataService(ConfigService configService, JsonService jsonService)
{
    this->configurationService = configService;
    this->jsonService = jsonService;
}

int DataService::sendSensorReadings(SensorReading reading)
{
    Configuration config = this->configurationService.getConfiguration();
    String jsonMessage = this->jsonService.convertSensorReadingsToJson(reading, this->configurationService.getConfiguration());

    return 200;
}

Configuration DataService::getConfigurationFromServer()
{
    Configuration config = this->configurationService.getConfiguration();

    return config;
}