#include "./jsonService.h"

void JsonService::printSensorReadingJson(SensorReading sensorReading)
{
    const size_t bufferSize = JSON_OBJECT_SIZE(5);
    DynamicJsonDocument jsonDocument(bufferSize);

    JsonObject reading = jsonDocument.to<JsonObject>();

    reading["soilMoisture"] = sensorReading.soilMoisture;
    reading["waterLevel"] = sensorReading.waterLevel;
    reading["humidity"] = sensorReading.humidity;
    reading["temperature"] = sensorReading.temperature;

    serializeJson(jsonDocument, Serial);

    jsonDocument.clear();
}

String JsonService::convertConfigToJson(Configuration configuration)
{
    String jsonMessage;
    DynamicJsonDocument jsonDocument(250);

    serializeJson(jsonDocument, Serial);
    jsonDocument.clear();

    return jsonMessage;
}

Configuration JsonService::convertJsonToConfig(String &configJson)
{
    Configuration configuration;
    const size_t capacity  = JSON_OBJECT_SIZE(9);

    DynamicJsonDocument jsonDocument(capacity);
    const DeserializationError error = deserializeJson(jsonDocument, const_cast<char *>(configJson.c_str()));

    if (!error)
    {
        configuration.SoilMoistureThreshold = jsonDocument["idealSoilMoistureValue"].as<int>();
        configuration.WateringTimeInSeconds = jsonDocument["wateringTimeInSeconds"].as<int>();
        configuration.MeasuringIntervalInMinutes = jsonDocument["measuringIntervalInMinutes"].as<int>();
        configuration.MinimumWaterThresholdPercentage = jsonDocument["minimumWaterThresholdPercentage"].as<double>();

        configuration.WaterTankDimensions.Width = jsonDocument["waterTankWidthInCM"].as<double>();
        configuration.WaterTankDimensions.Length = jsonDocument["waterTankLengthInCM"].as<double>();
        configuration.WaterTankDimensions.Height = jsonDocument["waterTankHeightInCM"].as<double>();
        configuration.WaterTankDimensions.Radius = jsonDocument["waterTankRadiusInCM"].as<double>();
        configuration.TankType = jsonDocument["waterTankType"].as<int>();
    }
    else
    {
        Serial.println("Parse failed for configJson");
        Serial.println(configJson);
    }

    jsonDocument.clear();

    return configuration;
}