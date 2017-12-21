#include "./jsonService.h"

char* JsonService::convertSensorReadingsToJson(SensorReading sensorReading, Configuration configuration)
{
    char jsonBuffer[325];
    DynamicJsonBuffer readingBuffer(325);

    JsonObject &reading = readingBuffer.createObject();

    reading["soilMoisture"] = sensorReading.soilMoisture;
    reading["waterLevel"] = sensorReading.waterLevel;
    reading["waterLevelUnit"] = sensorReading.waterLevelUnit;
    reading["humidity"] = sensorReading.humidity;
    reading["temperature"] = sensorReading.temperature;
    reading["dht11ErrorCode"] = sensorReading.dht11ErrorCode;
    reading["temperatureUnit"] = sensorReading.temperatureUnit;

    JsonObject& sensor = reading.createNestedObject("sensor");
    sensor["sensorId"] = configuration.sensorId;

    reading.printTo(jsonBuffer);

    return jsonBuffer;
}

String JsonService::convertConfigToJson(Configuration configuration)
{
    String jsonMessage;
    DynamicJsonBuffer jsonBuffer(250);

    JsonObject &config = jsonBuffer.createObject();

    config.printTo(jsonMessage);

    return jsonMessage;
}

Configuration JsonService::convertJsonToConfig(String configJson)
{
    Configuration configuration = Configuration();
    String jsonMessage;
    DynamicJsonBuffer buffer(200);

    JsonObject &config = buffer.parseObject(configJson);

    return configuration;
}