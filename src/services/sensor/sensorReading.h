#ifndef SENSOR_READING
#define SENSOR_READING

#include <Arduino.h>

struct SensorReading
{
    int temperature;
    int humidity;
    String dht11ErrorCode;
    int soilMoisture;
    int waterLevel;
    const char* temperatureUnit;
    const char* waterLevelUnit;
};

#endif