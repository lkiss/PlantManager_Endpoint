#ifndef DHT11_READING
#define DHT11_READING

struct Dht11SensorReading {
    int humidity;
    int temperatureInCelsius;
    int dht11ErrorCode;
};

#endif