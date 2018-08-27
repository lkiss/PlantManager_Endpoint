#ifndef SENSOR_READING
#define SENSOR_READING

#include <Arduino.h>

struct SensorReading
{
    int deviceNumber;
    int temperature;
    int humidity;
    int soilMoisture;
    int waterLevel;
};

#endif