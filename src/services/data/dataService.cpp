#include "dataService.h"

DataService::DataService(ConfigService configService, JsonService jsonService)
{
    this->configService = configService;
    this->jsonService = jsonService;
}

Configuration DataService::getConfigurationFromServer()
{
    Configuration config = this->configService.getConfiguration();

    return config;
}