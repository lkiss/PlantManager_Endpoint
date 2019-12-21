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

void WaterTank::UpdateWaterTresholdValue(double WaterLevelInPercentage)
{
    this->WaterLevelInPercentage = WaterLevelInPercentage;
}

double WaterTank::CalculateVolume(double height)
{
    double volume = 0;

    // Serial.println("this->waterTankType");
    // Serial.println(this->waterTankType);

    // Serial.println("height");
    // Serial.println(height);

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

    // Serial.println("waterHeight");
    // Serial.println(waterHeight);

    // Serial.println("this->waterTankVolumeCC");
    // Serial.println(this->waterTankVolumeCC);

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
    return this->WaterLevelInPercentage >= remainingWaterInPercentage || remainingWaterInPercentage == -1 ? false : true;
}

void WaterTank::UpdateWaterTankDimensions(WaterTankType tankType, DimensionsInCentimeters dimensions)
{
    // Serial.println("Passed WaterTankType: ");
    // Serial.println(tankType);

    // Serial.println("Passed Length: ");
    // Serial.println(dimensions.Length);

    // Serial.println("Passed Width: ");
    // Serial.println(dimensions.Width);

    // Serial.println("Passed Height: ");
    // Serial.println(dimensions.Height);

    // Serial.println("Passed Radius: ");
    // Serial.println(dimensions.Radius);

    // Serial.println("Current WaterTankType: ");
    // Serial.println(this->waterTankType);

    // Serial.println("Current Length: ");
    // Serial.println(this->dimensions.Length);

    // Serial.println("Current Width: ");
    // Serial.println(this->dimensions.Width);

    // Serial.println("Current Height: ");
    // Serial.println(this->dimensions.Height);

    // Serial.println("Current Radius: ");
    // Serial.println(this->dimensions.Radius);

    if (dimensions.Length == 0 && dimensions.Width == 0 && dimensions.Height == 0 && dimensions.Radius == 0)
    {
        return;
    }

    this->waterTankType = tankType;
    this->dimensions.Height = dimensions.Height;
    this->dimensions.Length = dimensions.Length;
    this->dimensions.Width = dimensions.Width;
    this->dimensions.Radius = dimensions.Radius;

    // Serial.println("New WaterTankType: ");
    // Serial.println(this->waterTankType);

    // Serial.println("New Length: ");
    // Serial.println(this->dimensions.Length);

    // Serial.println("New Width: ");
    // Serial.println(this->dimensions.Width);

    // Serial.println("New Height: ");
    // Serial.println(this->dimensions.Height);

    // Serial.println("New Radius: ");
    // Serial.println(this->dimensions.Radius);

    CalculateCapacityCC();
}