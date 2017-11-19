#include "./jsonService.h"

String JsonService::convertSensorReadingsToJson(SensorReading sensorReading)
{
    String jsonMessage;
    DynamicJsonBuffer buffer(100);

    JsonObject &reading = buffer.createObject();
    reading["soilMoisture"] = sensorReading.soilMoisture;
    reading["waterLevel"] = sensorReading.waterLevel;
    reading["waterLevelUnit"] = sensorReading.waterLevelUnit;
    reading["humidity"] = sensorReading.humidity;
    reading["temperature"] = sensorReading.temperature;
    reading["temperatureUnit"] = sensorReading.temperatureUnit;
    reading["dht11ErrorCode"] = sensorReading.dht11ErrorCode;

    reading.printTo(jsonMessage);
    return jsonMessage;
}

String JsonService::convertConfigToJson(Configuration configuration)
{
    String jsonMessage;
    DynamicJsonBuffer buffer(200);

    JsonObject &config = buffer.createObject();
    config["measuringInterval"] = configuration.measuringInterval;
    config["wateringTime"] = configuration.wateringTime;
    config["smtpPort"] = configuration.smtpPort;
    config["smtpServer"] = configuration.smtpServer;
    config["emailTo"] = configuration.emailTo;
    config["emailFrom"] = configuration.emailFrom;
    config["emailSubject"] = configuration.emailSubject;
    config["emailBody"] = configuration.emailBody;

    config.printTo(jsonMessage);

    return jsonMessage;
}

Configuration JsonService::convertJsonToConfig(String configJson)
{
    Configuration configuration = Configuration();
    String jsonMessage;
    DynamicJsonBuffer buffer(200);

    JsonObject &config = buffer.parseObject(configJson);

    configuration.measuringInterval = config["measuringInterval"];
    configuration.wateringTime = config["wateringTime"];
    configuration.smtpPort = config["smtpPort"];
    configuration.smtpServer = config["smtpServer"].as<String>();
    configuration.base64UserId = config["base64UserId"].as<String>();
    configuration.base64Password = config["base64Password"].as<String>();
    configuration.emailTo = config["emailTo"].as<String>();
    configuration.emailFrom = config["emailFrom"].as<String>();
    configuration.emailSubject = config["emailSubject"].as<String>();
    configuration.emailBody = config["emailBody"].as<String>();

    return configuration;
}