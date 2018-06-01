#ifndef SENSOR_READING
#define SENSOR_READING

#include <Arduino.h>

struct SensorReading
{
    int temperature;
    int humidity;
    int soilMoisture;
    int waterLevel;
    float batteryVoltage;
    const char* temperatureUnit;
    const char* waterLevelUnit;
};

#endif