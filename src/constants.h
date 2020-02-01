#ifndef PI
#define PI 3.14
#endif

#ifndef DEVICE_ID
#define DEVICE_ID "a8997fcd-328e-43f7-98b0-4c75390f89c10"
#endif

#ifndef DEVICE_TYPE
#define DEVICE_TYPE 1
#endif

#ifndef CONSTANTS
#define CONSTANTS

#ifndef NUMBER_OF_DEVICES
#define NUMBER_OF_DEVICES 2
#endif

#ifndef MUX_DEVICE_SHIFT
#define MUX_DEVICE_SHIFT 8
#endif

#ifndef ANALOG_MAX
#define ANALOG_MAX 255
#endif

#ifndef ANALOG_MIN
#define ANALOG_MIN 0
#endif

#ifndef MUX_COMMON_PIN
#define MUX_COMMON_PIN A0
#endif

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
    double Length;
    double Width;
    double Height;
    double Radius;
};

#endif