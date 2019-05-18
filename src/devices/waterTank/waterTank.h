#include "../../constants.h"
#ifndef WATER_TANK
#define WATER_TANK
class WaterTank
{
private:
  double waterTankVolumeCC;
  double minimumWaterThresholdPercentage = 20;
  WaterTankType waterTankType;
  DimensionsInCentimeters dimensions;
  void CalculateCapacityCC();
  double CalculateVolume(double height);

public:
  WaterTank();
  WaterTank(WaterTankType tankType, DimensionsInCentimeters dimensions);
  WaterTank(WaterTankType tankType, double lengthCM = 0, double widthCM = 0, double heightCM = 0, double radiusCM = 0);
  void UpdateWaterTresholdValue(double minimumWaterThresholdPercentage);
  bool IsWaterLevelSufficient(double height);
  double GetRemainingInPercentage(double height);
  void UpdateWaterTankDimensions(WaterTankType tankType, DimensionsInCentimeters dimensions);
};
#endif