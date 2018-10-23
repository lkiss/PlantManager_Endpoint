#include "./sensorService.h"

SensorService::SensorService() {}

SensorReading SensorService::getSensorReadings(int deviceNumber)
{
    SensorReading reading;
    TemperatureSensorReading temperatureSensorReading;

    temperatureSensorReading = this->temperatureSensor.read();

    reading.deviceNumber = deviceNumber;
    reading.soilMoisture = this->soilMoistureSensors[deviceNumber].read();
    reading.waterLevel = this->waterTank.GetRemainingInPercentage(this->waterLevelSensor.getMissingWaterColumHeighCM());
    reading.humidity = temperatureSensorReading.humidity;
    reading.temperature = temperatureSensorReading.temperatureInCelsius;

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
    this->waterPumps[deviceNumber].updateWateringTime(config.WateringTimeInSeconds);
    this->soilMoistureSensors[deviceNumber].updateTresholdValues(config.SoilMoistureThreshold);
    this->waterTank.updateWaterTresholdValue(config.MinimumWaterThresholdPercentage);

    // Serial.print("WateringTimeInSeconds: ");
    // Serial.println(config[deviceNumber].WateringTimeInSeconds);

    // Serial.print("SoilMoistureThreshold: ");
    // Serial.println(config[deviceNumber].SoilMoistureThreshold);

    // Serial.print("TankType: ");
    // Serial.println(config[deviceNumber].TankType);
    // Serial.print("WaterTankLength: ");
    // Serial.println(config[deviceNumber].WaterTankLength);
    // Serial.print("WaterTankWidth: ");
    // Serial.println(config[deviceNumber].WaterTankWidth);
    // Serial.print("WaterTankHeight: ");
    // Serial.println(config[deviceNumber].WaterTankHeight);
    // Serial.print("WaterTankRadius: ");
    // Serial.println(config[deviceNumber].WaterTankRadius);
    // Serial.print("minimumWaterThresholdPercentage: ");
    // Serial.println(config[deviceNumber].MinimumWaterThresholdPercentage);
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