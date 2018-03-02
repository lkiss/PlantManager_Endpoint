#include "./sensorService.h"

SensorService::SensorService() {}

SensorReading SensorService::getSensorReadings()
{
    SensorReading reading;
    TemperatureSensorReading temperatureSensorReading;

    temperatureSensorReading = this->temperatureSensor.read();

    reading.soilMoisture = this->soilMoistureSensor.read();
    reading.waterLevel = this->waterTank.GetRemainingInPercentage(this->waterLevelSensor.getMissingWaterColumHeighCM());
    reading.waterLevelUnit = "%";
    reading.humidity = temperatureSensorReading.humidity;
    reading.temperature = temperatureSensorReading.temperatureInCelsius;
    reading.temperatureUnit = "C";
    reading.dht11ErrorCode = temperatureSensorReading.dht11ErrorCode;

    return reading;
}

SensorService::SensorService(const WaterTank &waterTank, const WaterLevelSensor &waterLevelSensor, const WaterPump &waterPump, const SoilMoistureSensor &soilMoistureSensor, const TemperatureSensor &temperatureSensor)
{
    this->waterLevelSensor = waterLevelSensor;
    this->waterPump = waterPump;
    this->soilMoistureSensor = soilMoistureSensor;
    this->temperatureSensor = temperatureSensor;
    this->waterTank = waterTank;
}

bool SensorService::water(SensorReading reading)
{
    if (this->waterTank.isWaterLevelSufficient(reading.waterLevel))
    {
        if (this->soilMoistureSensor.isDry(reading.soilMoisture))
        {
            this->waterPump.activateWaterPump();
        }
    }
    else
    {
        return false;
    }

    return true;
}