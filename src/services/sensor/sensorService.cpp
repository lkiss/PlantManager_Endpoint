#include "./sensorService.h"

SensorService::SensorService() {}

SensorReading SensorService::getSensorReadings(int deviceNumber)
{
    // Serial.println("Before Reading");
    SensorReading reading;
    TemperatureSensorReading temperatureSensorReading;

    temperatureSensorReading = this->temperatureSensor.read();

    // Serial.println(temperatureSensorReading.humidity);
    // Serial.println(temperatureSensorReading.temperatureInCelsius);

    reading.deviceNumber = deviceNumber;
    reading.soilMoisture = this->soilMoistureSensors[deviceNumber].read();
    reading.waterLevel = this->waterTank.GetRemainingInPercentage(this->waterLevelSensor.getMissingWaterColumHeighCM());
    reading.humidity = temperatureSensorReading.humidity;
    reading.temperature = temperatureSensorReading.temperatureInCelsius;

    // Serial.println("Readings");
    // Serial.println(reading.deviceNumber);
    // Serial.println(reading.soilMoisture);
    // Serial.println(reading.waterLevel);
    // Serial.println(reading.humidity);
    // Serial.println(reading.temperature);

    return reading;
}

SensorService::SensorService(
    const WaterTank &waterTank,
    const WaterLevelSensor &waterLevelSensor,
    WaterPump *waterPumps,
    SoilMoistureSensor *soilMoistureSensors,
    const TemperatureSensor &temperatureSensor)
{
    this->waterTank = waterTank;
    this->waterLevelSensor = waterLevelSensor;
    this->waterPumps = waterPumps;
    this->soilMoistureSensors = soilMoistureSensors;
    this->temperatureSensor = temperatureSensor;
    this->configService = configService;
}

void SensorService::updateSensorParamaters(Configuration config, int deviceNumber)
{
    // Serial.println("Config");
    // Serial.println(config.WateringTimeInSeconds);
    // Serial.println(config.SoilMoistureThreshold);
    // Serial.println(config.MinimumWaterThresholdPercentage);
    // Serial.println("-------------");
    // Serial.println("DeviceNumber");
    // Serial.println(deviceNumber);

    this->waterPumps[deviceNumber].updateWateringTime(config.WateringTimeInSeconds);
    this->soilMoistureSensors[deviceNumber].updateTresholdValues(config.SoilMoistureThreshold);
    this->waterTank.updateWaterTresholdValue(config.MinimumWaterThresholdPercentage);

    // Serial.println("After updateSensorParamaters");
}

bool SensorService::water(SensorReading reading, int deviceNumber)
{
    if (this->waterTank.isWaterLevelSufficient(reading.waterLevel))
    {
        if (this->soilMoistureSensors[deviceNumber].isDry(reading.soilMoisture))
        {
            this->waterPumps[deviceNumber].activateWaterPump();
        }
    }
    else
    {
        return false;
    }

    return true;
}