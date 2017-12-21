#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef CONSTANTS
#define CONSTANTS

enum WaterTankType
{
    CYLINDER = 0,
    PRISM = 1,
    CUBE = 2
};

enum DeviceType{
    TYPE01 = 0,
    TYPE02 = 1
};

enum TemperatureUnitOfMeasure
{
    FAHRENHEIT = 0,
    CELSIUS = 1
};

enum SoilMoistureUnitOfMeasurement
{
    PERCENTAGE = 0
};

struct DimensionsInCentimeters
{
    double length;
    double width;
    double height;
    double radius;
};

#endif