#include "./jsonService.h"

char* JsonService::convertSensorReadingsToJson(SensorReading sensorReading, Configuration configuration)
{
    char readingBuffer[325];
    DynamicJsonBuffer jsonBuffer(325);

    JsonObject &reading = jsonBuffer.createObject();

    reading["soilMoisture"] = sensorReading.soilMoisture;
    reading["waterLevel"] = sensorReading.waterLevel;
    reading["waterLevelUnit"] = sensorReading.waterLevelUnit;
    reading["humidity"] = sensorReading.humidity;
    reading["temperature"] = sensorReading.temperature;
    reading["dht11ErrorCode"] = sensorReading.dht11ErrorCode;
    reading["temperatureUnit"] = sensorReading.temperatureUnit;

    JsonObject& sensor = reading.createNestedObject("sensor");
    sensor["sensorId"] = SENSOR_ID;

    reading.printTo(readingBuffer);
    jsonBuffer.clear();

    return readingBuffer;
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

Configuration JsonService::convertJsonToConfig(String configJson)
{
    Configuration configuration;
    String jsonMessage;
    DynamicJsonBuffer buffer(200);
    JsonObject &config = buffer.parseObject(configJson);
    configuration.SoilMoistureThreshold = config["idealSoilMoistureValue"].as<int>();
    configuration.WateringTime = (config["wateringTimeInSeconds"].as<int>()) * 1000;

    buffer.clear();

    return configuration;
}