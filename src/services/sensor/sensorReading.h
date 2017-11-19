#ifndef SENSOR_READING
#define SENSOR_READING

struct SensorReading
{
    int temperature;
    int humidity;
    int dht11ErrorCode;
    int soilMoisture;
    int waterLevel;
    const char* temperatureUnit;
    const char* waterLevelUnit;
};

#endif