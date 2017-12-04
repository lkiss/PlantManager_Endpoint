#include "./jsonService.h"

String JsonService::convertSensorReadingsToJson(SensorReading sensorReading, Configuration configuration)
{
    String jsonMessage;
    DynamicJsonBuffer readingBuffer(300);
    DynamicJsonBuffer sensorBuffer(100);

    JsonObject &sensor = sensorBuffer.createObject();
    JsonObject &reading = readingBuffer.createObject();

    sensor["id"] = configuration.sensorId;

    reading["soilMoisture"] = sensorReading.soilMoisture;
    reading["waterLevel"] = sensorReading.waterLevel;
    reading["waterLevelUnit"] = sensorReading.waterLevelUnit;
    reading["humidity"] = sensorReading.humidity;
    reading["temperature"] = sensorReading.temperature;
    reading["dht11ErrorCode"] = sensorReading.dht11ErrorCode;
    reading["temperatureUnit"] = sensorReading.temperatureUnit;
    reading["sensor"] = sensor;
    
    reading.printTo(jsonMessage);

    return jsonMessage;
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