#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef SENSOR_ID
#define SENSOR_ID "72fb5192-a399-4e74-96e1-1a40cf7f934d"
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