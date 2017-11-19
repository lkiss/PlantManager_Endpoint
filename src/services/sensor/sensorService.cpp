#include "./sensorService.h"

SensorService::SensorService() {}

SensorReading SensorService::getSensorReadings()
{
    SensorReading reading;
    Dht11SensorReading dht11SensorReading;

    dht11SensorReading = this->dht11Sensor.read();

    reading.soilMoisture = this->soilMoistureSensor.read();
    reading.waterLevel = this->waterTank.GetRemainingInPercentage(this->waterLevelSensor.getMissingWaterColumHeighCM());
    reading.waterLevelUnit = "%";
    reading.humidity = dht11SensorReading.humidity;
    reading.temperature = dht11SensorReading.temperatureInCelsius;
    reading.temperatureUnit = "C";
    reading.dht11ErrorCode = dht11SensorReading.dht11ErrorCode;

    return reading;
}

SensorService::SensorService(const WaterTank &waterTank, const WaterLevelSensor &waterLevelSensor, const WaterPump &waterPump, const SoilMoistureSensor &soilMoistureSensor, const Dht11Sensor &dht11Sensor)
{
    this->waterLevelSensor = waterLevelSensor;
    this->waterPump = waterPump;
    this->soilMoistureSensor = soilMoistureSensor;
    this->dht11Sensor = dht11Sensor;
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