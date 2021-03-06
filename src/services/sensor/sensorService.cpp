#include "./sensorService.h"

SensorService::SensorService() {}

SensorReading SensorService::getSensorReadings(int endpointIndex)
{
    // Serial.println("Before Reading");
    SensorReading reading;
    TemperatureSensorReading temperatureSensorReading;

    temperatureSensorReading = this->temperatureSensor.read();

    // Serial.println(temperatureSensorReading.humidity);
    // Serial.println(temperatureSensorReading.temperatureInCelsius);

    reading.endpointIndex = endpointIndex;
    this->mux->channel(endpointIndex);
    reading.soilMoisture = this->soilMoistureSensors[endpointIndex].read();
    reading.waterLevel = this->waterTank.GetRemainingInPercentage(this->waterLevelSensor.getMissingWaterColumHeighCM());
    reading.humidity = temperatureSensorReading.humidity;
    reading.temperature = temperatureSensorReading.temperatureInCelsius;

    // Serial.println("Readings");
    // Serial.println(reading.endpointIndex);
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
    const TemperatureSensor &temperatureSensor,
    CD74HC4067 &mux)
{
    this->waterTank = waterTank;
    this->waterLevelSensor = waterLevelSensor;
    this->waterPumps = waterPumps;
    this->soilMoistureSensors = soilMoistureSensors;
    this->temperatureSensor = temperatureSensor;
    this->configService = configService;
    this->mux = &mux;
}

void SensorService::updateSensorParamaters(Configuration config, int endpointIndex)
{
    // Serial.println("Config");
    // Serial.println(config.WateringTimeInSeconds);
    // Serial.println(config.IdealSoilMoistureValue);
    // Serial.println(config.WaterLevelInPercentage);
    // Serial.println("-------------");
    // Serial.println("endpointIndex");
    // Serial.println(endpointIndex);

    this->waterPumps[endpointIndex].updateWateringTime(config.WateringTimeInSeconds);
    this->soilMoistureSensors[endpointIndex].updateTresholdValues(config.IdealSoilMoistureValue);
    this->waterTank.UpdateWaterTresholdValue(config.WaterLevelInPercentage);
    this->waterTank.UpdateWaterTankDimensions(config.TankType, config.WaterTankDimensions);

    // Serial.println("After updateSensorParamaters");
}

bool SensorService::water(SensorReading reading, int endpointIndex)
{
    if (this->waterTank.IsWaterLevelSufficient(reading.waterLevel))
    {
        if (this->soilMoistureSensors[endpointIndex].isDry(reading.soilMoisture))
        {
            this->mux->channel(endpointIndex + MUX_DEVICE_SHIFT);
            this->waterPumps[endpointIndex].activateWaterPump();
        }
    }
    else
    {
        return false;
    }

    return true;
}