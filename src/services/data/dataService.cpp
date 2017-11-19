#include "dataService.h"

DataService::DataService(ConfigService configService, JsonService jsonService)
{
    this->configurationService = configService;
    this->jsonService = jsonService;
}

int DataService::sendSensorReadings(SensorReading reading)
{
    Configuration config = this->configurationService.getConfiguration();
    String jsonMessage = this->jsonService.convertSensorReadingsToJson(reading);

    return 200;
}

void DataService::getConfigurationFromServer()
{
}