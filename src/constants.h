#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef SENSOR_ID
#define SENSOR_ID "a8997fcd-328e-43f7-98b0-4c75390f89c6"
#endif

#ifndef SENSOR_TYPE
#define SENSOR_TYPE TYPE01
#endif

#ifndef CONSTANTS
#define CONSTANTS

enum WaterTankType
{
    CYLINDER = 0,
    PRISM = 1,
    CUBE = 2
};

enum DeviceType
{
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