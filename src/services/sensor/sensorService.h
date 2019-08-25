#ifndef SENSOR_SERVICE
#define SENSOR_SERVICE

#include <CD74HC4067.h>
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
  CD74HC4067 *mux;
public:
  SensorService();
  SensorService(
      const WaterTank &waterTank,
      const WaterLevelSensor &waterLevelSensor,
      WaterPump *waterPumps,
      SoilMoistureSensor *soilMoistureSensors,
      const TemperatureSensor &temperatureSensor,
      CD74HC4067 &mux);
  SensorReading getSensorReadings(int endpointIndex);
  bool water(SensorReading reading, int endpointIndex);
  void updateSensorParamaters(Configuration config, int endpointIndex);
};

#endif