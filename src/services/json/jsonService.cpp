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

    Serial.print("*");
    serializeJson(jsonDocument, Serial);
    Serial.print("*");

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
    const size_t capacity = JSON_OBJECT_SIZE(9);

    DynamicJsonDocument jsonDocument(capacity);
    const String originalJson = configJson.c_str();
    const DeserializationError error = deserializeJson(jsonDocument, const_cast<char *>(configJson.c_str()));

    if (!error)
    {
        configuration.IdealSoilMoistureValue = jsonDocument["ismv"].as<int>();
        configuration.WateringTimeInSeconds = jsonDocument["wtis"].as<int>();
        configuration.MeasuringIntervalInMinutes = jsonDocument["miim"].as<int>();
        configuration.WaterLevelInPercentage = jsonDocument["mwtp"].as<double>();

        configuration.WaterTankDimensions.Width = jsonDocument["wtw"].as<double>();
        configuration.WaterTankDimensions.Length = jsonDocument["wtl"].as<double>();
        configuration.WaterTankDimensions.Height = jsonDocument["wth"].as<double>();
        configuration.WaterTankDimensions.Radius = jsonDocument["wtr"].as<double>();
        configuration.TankType = jsonDocument["wtt"].as<int>();
    }
    else
    {
        const String errorMessage = "Parse failed for configJson: ";
        Serial.print(errorMessage + originalJson);
    }

    // Serial.println("Watering time in seconds");
    // Serial.println(configuration.WateringTimeInSeconds);

    jsonDocument.clear();

    return configuration;
}
