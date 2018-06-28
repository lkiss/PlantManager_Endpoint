#include "../../constants.h"
#ifndef WATER_TANK
#define WATER_TANK
class WaterTank
{
private:
  double waterTankVolumeCC;
  double minimumWaterThresholdPercentage = 20;
  int waterTankType;
  DimensionsInCentimeters dimensions;
  void CalculateCapacityCC();
  double CalculateVolume(double height);

public:
  WaterTank();
  WaterTank(int tankType, DimensionsInCentimeters dimensions);
  WaterTank(int tankType, double lengthCM = 0, double widthCM = 0, double heightCM = 0, double radiusCM = 0);
  void updateWaterTresholdValue(double minimumWaterThresholdPercentage);
  bool isWaterLevelSufficient(double height);
  double GetRemainingInPercentage(double height);
};
#endif