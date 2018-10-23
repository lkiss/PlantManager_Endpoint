#ifndef SENSOR_SERVICE
#define SENSOR_SERVICE

#include "../../devices/devices.h"
#include "./sensorReading.h"
#include "../../constants.h"
#include "../config/configService.h"

class SensorService
{
private:
  ConfigService configService;
  WaterLevelSensor waterLevelSensor;
  WaterPump *waterPumps;
  SoilMoistureSensor *soilMoistureSensors;
  WaterTank waterTank;
  TemperatureSensor temperatureSensor;

public:
  SensorService();
  SensorService(
      const WaterTank &waterTank,
      const WaterLevelSensor &waterLevelSensor,
      WaterPump *waterPumps,
      SoilMoistureSensor *soilMoistureSensors,
      const TemperatureSensor &temperatureSensor);
  SensorReading getSensorReadings(int deviceNumber);
  bool water(SensorReading reading, int deviceNumber);
  void updateSensorParamaters(Configuration config, int deviceNumber);
};

#endif