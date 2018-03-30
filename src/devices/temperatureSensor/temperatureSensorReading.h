#ifndef TEMPERATURE_SENSOR_READING
#define TEMPERATURE_SENSOR_READING

#include <Arduino.h>

struct TemperatureSensorReading {
    int humidity;
    int temperatureInCelsius;
    int dht11ErrorCode;
};

#endif