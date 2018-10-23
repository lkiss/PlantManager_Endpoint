#include "./jsonService.h"

void JsonService::printSensorReadingJson(SensorReading sensorReading)
{
    const size_t bufferSize = JSON_OBJECT_SIZE(5);
    DynamicJsonBuffer jsonBuffer(bufferSize);

    JsonObject &reading = jsonBuffer.createObject();

    reading["soilMoisture"] = sensorReading.soilMoisture;
    reading["waterLevel"] = sensorReading.waterLevel;
    reading["humidity"] = sensorReading.humidity;
    reading["temperature"] = sensorReading.temperature;

    reading.printTo(Serial);

    jsonBuffer.clear();
}

String JsonService::convertConfigToJson(Configuration configuration)
{
    String jsonMessage;
    DynamicJsonBuffer jsonBuffer(250);

    JsonObject &config = jsonBuffer.createObject();

    config.printTo(jsonMessage);
    jsonBuffer.clear();

    return jsonMessage;
}

Configuration JsonService::convertJsonToConfig(String &configJson)
{
    Configuration configuration;
    const size_t bufferSize = JSON_OBJECT_SIZE(4) + 130;

    DynamicJsonBuffer buffer(bufferSize);
    JsonObject &config = buffer.parseObject(const_cast<char *>(configJson.c_str()));

    if (config.success())
    {
        configuration.SoilMoistureThreshold = config["idealSoilMoistureValue"].as<int>();
        configuration.WateringTimeInSeconds = config["wateringTimeInSeconds"].as<int>();
        configuration.MeasuringIntervalInMinutes = config["measuringIntervalInMinutes"].as<int>();
        configuration.MinimumWaterThresholdPercentage = config["minimumWaterThresholdPercentage"].as<double>();
    }
    else
    {
        config.invalid().prettyPrintTo(Serial);
        Serial.println("Parse failed for configJson");
        Serial.println(configJson);
    }

    buffer.clear();

    return configuration;
}