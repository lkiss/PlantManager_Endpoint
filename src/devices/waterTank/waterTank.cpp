#include "./waterTank.h"
#include "math.h"

WaterTank::WaterTank() {}

WaterTank::WaterTank(WaterTankType tankType, DimensionsInCentimeters dimensions)
{
    this->UpdateWaterTankDimensions(tankType, dimensions);
}

WaterTank::WaterTank(WaterTankType tankType, double lengthCM, double widthCM, double heightCM, double radiusCM)
{
    DimensionsInCentimeters dimensions;
    dimensions.Length = lengthCM;
    dimensions.Width = widthCM;
    dimensions.Height = heightCM;
    dimensions.Radius = radiusCM;

    this->waterTankType = tankType;
    this->dimensions = dimensions;

    CalculateCapacityCC();
}

void WaterTank::UpdateWaterTresholdValue(double minimumWaterThresholdPercentage)
{
    this->minimumWaterThresholdPercentage = minimumWaterThresholdPercentage;
}

double WaterTank::CalculateVolume(double height)
{
    double volume = 0;

    switch (this->waterTankType)
    {
    case CYLINDER: //CYLINDER
    {
        volume = pow(this->dimensions.Radius, 2) * PI * height;
        break;
    }

    case CUBE: //CUBE
    {
        volume = pow(this->dimensions.Width, 2) * height;
        break;
    }

    case PRISM: //PRISM
    {
        volume = this->dimensions.Width * this->dimensions.Length * height;
        break;
    }
    }

    return volume;
}

void WaterTank::CalculateCapacityCC()
{
    double volume = this->CalculateVolume(this->dimensions.Height);

    this->waterTankVolumeCC = volume;
}

double WaterTank::GetRemainingInPercentage(double height)
{
    double waterHeight = height <= 2 ? this->dimensions.Height : this->dimensions.Height - height;
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

bool WaterTank::IsWaterLevelSufficient(double remainingWaterInPercentage)
{
    return this->minimumWaterThresholdPercentage >= remainingWaterInPercentage || remainingWaterInPercentage == -1 ? false : true;
}

void WaterTank::UpdateWaterTankDimensions(WaterTankType tankType, DimensionsInCentimeters dimensions)
{
    this->waterTankType = tankType;
    this->dimensions = dimensions;

    CalculateCapacityCC();
}