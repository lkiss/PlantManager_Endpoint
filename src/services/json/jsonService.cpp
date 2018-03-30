#include "./jsonService.h"

String JsonService::convertSensorReadingsToJson(SensorReading sensorReading)
{
    const size_t bufferSize = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(8);
    String readingBuffer;
    DynamicJsonBuffer jsonBuffer(bufferSize);

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
    const size_t bufferSize = JSON_OBJECT_SIZE(5) + 150;

    Configuration configuration;
    DynamicJsonBuffer buffer(bufferSize);
    JsonObject &config = buffer.parseObject(configJson);
    configuration.SoilMoistureThreshold = config["idealSoilMoistureValue"];
    configuration.WateringTimeInSeconds = config["wateringTimeInSeconds"];
    configuration.MeasuringIntervalInMinutes = config["measuringIntervalInMinutes"];

    buffer.clear();

    return configuration;
}