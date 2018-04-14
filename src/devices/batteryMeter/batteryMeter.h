#ifndef BATTERY_METER
#define BATTERY_METER

class BatteryMeter
{
  private:
    float VoltageDividerResistor_1; // kOhm
    float VoltageDividerResistor_2; // kOhm
    
  public:
    BatteryMeter();
};
#endif