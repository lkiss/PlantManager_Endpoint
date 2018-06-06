#ifndef CONFIGURATION
#define CONFIGURATION

struct Configuration
{
  int SoilMoistureThreshold;
  int WateringTimeInSeconds;
  int MeasuringIntervalInMinutes;
  double WaterTankLength;
  double WaterTankWidth;
  double WaterTankHeight;
  double WaterTankRadius;
  double minimumWaterThresholdPercentage;
  int TankType;
};

#endif