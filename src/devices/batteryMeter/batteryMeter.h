#ifndef BATTERY_METER
#define BATTERY_METER

class BatteryMeter
{
  private:
    float voltageDividerResistor_1 = 30; // kOhm
    float voltageDividerResistor_2 = 10; // kOhm
    float voltageResolution = 1.1 / 1024; // INTERNAL voltage which is 1.1V divided by 1024 bit resolution
    int numberOfvoltageSamples = 10; 
    int batteryMeterPin = A1;
    
  public:
    BatteryMeter();
    BatteryMeter(int batteryMeterPin);
    float getBatteryVoltage();
};
#endif