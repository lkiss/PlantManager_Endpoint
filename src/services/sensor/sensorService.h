#ifndef SENSOR_SERVICE
#define SENSOR_SERVICE

#include "../../devices/devices.h"
#include "./sensorReading.h"
#include "../../constants.h"

class SensorService
{
  private:
    WaterLevelSensor waterLevelSensor;
    WaterPump waterPump;
    SoilMoistureSensor soilMoistureSensor;
    WaterTank waterTank;
    Dht11Sensor dht11Sensor;

  public:
    SensorService();
    SensorService(const WaterTank& waterTank, const WaterLevelSensor& waterLevelSensor, const WaterPump& waterPump, const SoilMoistureSensor& soilMoistureSensor, const Dht11Sensor& dht11Sensor);
    SensorReading getSensorReadings();
    bool water(SensorReading reading);
};

#endif