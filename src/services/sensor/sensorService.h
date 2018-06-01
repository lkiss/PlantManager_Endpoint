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
  WaterPump waterPump;
  SoilMoistureSensor soilMoistureSensor;
  WaterTank waterTank;
  TemperatureSensor temperatureSensor;
  BatteryMeter batterMeter;

public:
  SensorService();
  SensorService(
      const WaterTank &waterTank,
      const WaterLevelSensor &waterLevelSensor,
      const WaterPump &waterPump,
      const SoilMoistureSensor &soilMoistureSensor,
      const TemperatureSensor &temperatureSensor);
  SensorReading getSensorReadings(float batteryVoltage);
  bool water(SensorReading reading);
  void updateSensorsParamaters(Configuration config);
};

#endif