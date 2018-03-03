#include "./jsonService.h"

String JsonService::convertSensorReadingsToJson(SensorReading sensorReading)
{
    String readingBuffer;
    DynamicJsonBuffer jsonBuffer(325);

    JsonObject &reading = jsonBuffer.createObject();

    reading["soilMoisture"] = sensorReading.soilMoisture;
    reading["waterLevel"] = sensorReading.waterLevel;
    reading["waterLevelUnit"] = sensorReading.waterLevelUnit;
    reading["humidity"] = sensorReading.humidity;
    reading["temperature"] = sensorReading.temperature;
    reading["dht11ErrorCode"] = sensorReading.dht11ErrorCode;
    reading["temperatureUnit"] = sensorReading.temperatureUnit;

    JsonObject &sensor = reading.createNestedObject("sensor");
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

Configuration JsonService::convertJsonToConfig(String &configJson)
{
    Configuration configuration;
    DynamicJsonBuffer buffer(500);
    JsonObject &config = buffer.parseObject(configJson);
    configuration.SoilMoistureThreshold = config["idealSoilMoistureValue"].as<int>();
    configuration.WateringTimeInSeconds = config["wateringTimeInSeconds"].as<int>();
    configuration.MeasuringIntervalInMinutes = config["measuringIntervalInMinutes"].as<int>();

    buffer.clear();

    return configuration;
}