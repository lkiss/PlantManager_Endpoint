#ifndef CONFIGURATION
#define CONFIGURATION

#include "../../constants.h"

struct Configuration
{
  int SoilMoistureThreshold;
  int WateringTimeInSeconds;
  int MeasuringIntervalInMinutes;
  double MinimumWaterThresholdPercentage;
  DimensionsInCentimeters WaterTankDimensions;
  WaterTankType TankType;
};

#endif