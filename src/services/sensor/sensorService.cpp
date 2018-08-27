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

void SensorService::updateSensorsParamaters(Configuration config[], int deviceNumber)
{
    this->waterPumps[deviceNumber].updateWateringTime(config[deviceNumber].WateringTimeInSeconds);
    this->soilMoistureSensors[deviceNumber].updateTresholdValues(config[deviceNumber].SoilMoistureThreshold);
    this->waterTank.updateWaterTresholdValue(config[deviceNumber].MinimumWaterThresholdPercentage);

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

bool SensorService::water(SensorReading *readings, int deviceNumber)
{
    if (this->waterTank.isWaterLevelSufficient(readings[deviceNumber].waterLevel))
    {
        if (this->soilMoistureSensors[deviceNumber].isDry(readings[deviceNumber].soilMoisture))
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