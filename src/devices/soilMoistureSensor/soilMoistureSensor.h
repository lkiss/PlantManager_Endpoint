#ifndef SOIL_MOISTURE_SENSOR
#define SOIL_MOISTURE_SENSOR

class SoilMoistureSensor
{
private:
  int soilMoistureDryTreshold = 920;
  int soilMoistureWetTreshold = 750;
  int soilMoisturePin = 0;
  int soilMoistureIndex = 0;

  bool isValidMoistureTreshold(int moistureTreshold);

public:
  SoilMoistureSensor();
  SoilMoistureSensor(int soilMoistureIndex);
  void updateTresholdValues(int &newTreshold);
  bool isDry();
  bool isDry(int soilMositureValue);
  int read();
};
#endif