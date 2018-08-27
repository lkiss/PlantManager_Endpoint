#include "./jsonService.h"

String JsonService::convertSensorReadingsToJson(SensorReading sensorReadings[])
{
    const size_t bufferSize = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(7);
    String readingBuffer;
    DynamicJsonBuffer jsonBuffer(bufferSize);

    JsonArray &reading = jsonBuffer.createArray();

    JsonObject &reading00 = reading.createNestedObject();
    reading00["deviceNumber"] = sensorReadings[0].deviceNumber;
    reading00["soilMoisture"] = sensorReadings[0].soilMoisture;
    reading00["waterLevel"] = sensorReadings[0].waterLevel;
    reading00["humidity"] = sensorReadings[0].humidity;
    reading00["temperature"] = sensorReadings[0].temperature;

    JsonObject &sensor00 = reading00.createNestedObject("sensor");
    sensor00["sensorId"] = SENSOR_ID;

    JsonObject &reading01 = reading.createNestedObject();
    reading01["deviceNumber"] = sensorReadings[1].deviceNumber;
    reading01["soilMoisture"] = sensorReadings[1].soilMoisture;
    reading01["waterLevel"] = sensorReadings[1].waterLevel;
    reading01["humidity"] = sensorReadings[1].humidity;
    reading01["temperature"] = sensorReadings[1].temperature;

    JsonObject &sensor01 = reading01.createNestedObject("sensor");
    sensor01["sensorId"] = SENSOR_ID;

    reading.printTo(readingBuffer);
    jsonBuffer.clear();

    return readingBuffer;
}

String JsonService::convertConfigToJson(Configuration *configuration)
{
    String jsonMessage;
    DynamicJsonBuffer jsonBuffer(250);

    JsonObject &config = jsonBuffer.createObject();

    config.printTo(jsonMessage);
    jsonBuffer.clear();

    return jsonMessage;
}

void JsonService::convertJsonToConfig(String &configJson, Configuration configuration[])
{
    const size_t bufferSize = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(4) + 230;

    DynamicJsonBuffer buffer(bufferSize);
    JsonArray &config = buffer.parseArray(const_cast<char *>(configJson.c_str()));

    if (config.success())
    {
        JsonObject &config00 = config[0];
        JsonObject &config01 = config[1];
        if (config00.success())
        {
            configuration[0].SoilMoistureThreshold = config00["idealSoilMoistureValue"].as<int>();
            configuration[0].WateringTimeInSeconds = config00["wateringTimeInSeconds"].as<int>();
            configuration[0].MeasuringIntervalInMinutes = config00["measuringIntervalInMinutes"].as<int>();
            configuration[0].MinimumWaterThresholdPercentage = config00["minimumWaterThresholdPercentage"].as<double>();
        }
        else
        {
            Serial.println("Parse failed for config[0]");
        }

        if (config00.success())
        {
            configuration[1].SoilMoistureThreshold = config01["idealSoilMoistureValue"].as<int>();
            configuration[1].WateringTimeInSeconds = config01["wateringTimeInSeconds"].as<int>();
            configuration[1].MeasuringIntervalInMinutes = config01["measuringIntervalInMinutes"].as<int>();
            configuration[1].MinimumWaterThresholdPercentage = config01["minimumWaterThresholdPercentage"].as<double>();
        }
        else
        {
            Serial.println("Parse failed for config[1]");
        }
    }
    else
    {
        config.invalid().prettyPrintTo(Serial);
        Serial.println("Parse failed for configJson");
        Serial.println(configJson);
    }

    buffer.clear();
}