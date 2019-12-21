#ifndef CONFIGURATION
#define CONFIGURATION

#include "../../constants.h"

struct Configuration
{
  int IdealSoilMoistureValue;
  int WateringTimeInSeconds;
  int MeasuringIntervalInMinutes;
  double WaterLevelInPercentage;
  DimensionsInCentimeters WaterTankDimensions;
  WaterTankType TankType;
};

#endif