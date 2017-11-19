#include "./waterTank.h"
#include "math.h"

WaterTank::WaterTank() {}

WaterTank::WaterTank(WaterTankType tankType, DimensionsInCentimeters dimensions)
{
    this->waterTankType = tankType;
    this->dimensions = dimensions;

    CalculateCapacityCC();
}

WaterTank::WaterTank(WaterTankType tankType, double lengthCM, double widthCM, double heightCM, double radiusCM)
{
    DimensionsInCentimeters dimensions;
    dimensions.length = lengthCM;
    dimensions.width = widthCM;
    dimensions.height = heightCM;
    dimensions.radius = radiusCM;

    this->waterTankType = tankType;
    this->dimensions = dimensions;

    CalculateCapacityCC();
}

double WaterTank::CalculateVolume(double height)
{
    double volume = 0;

    switch (this->waterTankType)
    {
    case WaterTankType::CYLINDER:
    {
        volume = pow(this->dimensions.radius, 2) * PI * height;
        break;
    }

    case WaterTankType::CUBE:
    {
        volume = pow(this->dimensions.width, 2) * height;
        break;
    }

    case WaterTankType::PRISM:
    {
        volume = this->dimensions.width * this->dimensions.length * height;
        break;
    }
    }

    return volume;
}

void WaterTank::CalculateCapacityCC()
{
    double volume = this->CalculateVolume(this->dimensions.height);

    this->waterTankVolumeCC = volume;
}

double WaterTank::GetRemainingInPercentage(double height)
{
    double waterHeight = height <= 2 ? this->dimensions.height : this->dimensions.height - height;
    double waterVolume = this->CalculateVolume(waterHeight);

    double result = (waterVolume / this->waterTankVolumeCC) * 100;

    if (result > 100)
    {
        return 100;
    }
    else if (result < 0)
    {
        return -1;
    }

    return result;
}

bool WaterTank::isWaterLevelSufficient(double remainingWaterInPercentage)
{
    return this->minimumWaterThresholdPercentage >= remainingWaterInPercentage || remainingWaterInPercentage == -1 ? false : true;
}