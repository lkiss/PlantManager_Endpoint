#include <RtcDS3231.h>
#include <Wire.h>
#include "LowPower.h"

#include "./devices/devices.h"
#include "./services/services.h"
#include "./constants.h"

int errorLedPin = LED_BUILTIN;
int numberOfWifiRestAttempts = 0;
int DHT11Pin = 11;
int waterPumpPin = 9;
int waterSensorEchoPin = 12;
int waterSensorTriggerPin = 13;
int sensorPowerPin = 10;
int soilMoisturePin = A0;
int wakeUpPin = INT0;
int wifiWakeupPin = 4;
int wifiReadyPin = 3;

bool isWifiPowered = false;
bool isSensorPowered = false;

int baudRate = 9600;

RtcDS3231<TwoWire> Rtc(Wire);
TemperatureSensor temperatureSensor(&DHT11Pin);
SoilMoistureSensor soilMoistureSensor(soilMoisturePin);
WaterPump waterPump(&waterPumpPin);
WaterLevelSensor waterLevelSensor(waterSensorTriggerPin, waterSensorEchoPin);
WaterTank waterTank(PRISM, 16, 6.5, 12);

JsonService jsonService;
ConfigService configService;
DataService dataService(configService, jsonService);
SensorService sensorService(waterTank, waterLevelSensor, waterPump, soilMoistureSensor, temperatureSensor);

void wakeUp() {}

void wakeUpWifi()
{
  digitalWrite(wifiWakeupPin, LOW);
  delay(100);
  digitalWrite(wifiWakeupPin, HIGH);
}

void toggleSensors()
{
  if (!isSensorPowered)
  {
    digitalWrite(sensorPowerPin, HIGH);
    isSensorPowered = true;
  }
  else
  {
    digitalWrite(sensorPowerPin, LOW);
    isSensorPowered = false;
  }
}

void updateTimer(int measuringIntervalInMinutes)
{
  RtcDateTime now = Rtc.GetDateTime();

  RtcDateTime wakeUpTime = now + (measuringIntervalInMinutes * 60);

  DS3231AlarmOne wakeTimer(
      wakeUpTime.Day(),
      wakeUpTime.Hour(),
      wakeUpTime.Minute(),
      wakeUpTime.Second(),
      DS3231AlarmOneControl_HoursMinutesSecondsMatch);
  Rtc.SetAlarmOne(wakeTimer);
  Rtc.LatchAlarmsTriggeredFlags();
}

void initializeRtc()
{
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (!Rtc.IsDateTimeValid())
  {
    Rtc.SetDateTime(compiled);
  }

  if (!Rtc.GetIsRunning())
  {
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Rtc.SetDateTime(compiled);
  }

  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
}

void setup()
{
  pinMode(errorLedPin, OUTPUT);
  pinMode(sensorPowerPin, OUTPUT);
  pinMode(wifiWakeupPin, OUTPUT);
  pinMode(wakeUpPin, INPUT);
  pinMode(wifiReadyPin, INPUT);
  initializeRtc();  
  wakeUpWifi();

  delay(3000);
}

void loop()
{
  if (digitalRead(wifiReadyPin) == HIGH)
  {
    Serial.begin(baudRate);
    Serial.setTimeout(10000);

    Serial.println(SENSOR_ID);

    while (!Serial.available())
    {
    }

    String configString = Serial.readStringUntil('\n');

    configService.setConfigurationJson(configString);
    Configuration config = configService.getConfiguration();

    sensorService.updateSensorsParamaters(config);

    toggleSensors();
    SensorReading reading = sensorService.getSensorReadings();
    toggleSensors();

    String jsonResult = jsonService.convertSensorReadingsToJson(reading);
    Serial.println(jsonResult);
    Serial.flush();

    sensorService.water(reading);

    attachInterrupt(wakeUpPin, wakeUp, FALLING);
    updateTimer(config.MeasuringIntervalInMinutes);

    // Serial.print("MeasuringIntervalInMinutes: ");
    // Serial.println(config.MeasuringIntervalInMinutes);
    // Serial.print("SoilMoistureThreshold: ");
    // Serial.println(config.SoilMoistureThreshold);
    // Serial.print("WateringTimeInSeconds: ");
    // Serial.println(config.WateringTimeInSeconds);
    // Serial.print("minimumWaterThresholdPercentage: ");
    // Serial.println(config.MinimumWaterThresholdPercentage);
    Serial.end();

    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(wakeUpPin);

    wakeUpWifi();
    delay(3000);
  }
}