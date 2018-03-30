#include <Wire.h>
#include <RtcDS3231.h>
#include "LowPower.h"

#include "./devices/devices.h"
#include "./services/services.h"
#include "./constants.h"

int DHT11Pin = 5;
int waterPumpPin = 6;
int waterSensorEchoPin = 7;
int waterSensorTriggerPin = 8;
int sensorPowerPin = 4;
int soilMoisturePin = A0;
int wakeUpPin = INT0;
int wifiWakeupPin = 10;
int wifiReadyPin = 9;

bool isWifiPowered = false;
bool isSensorPowered = false;

int baudRate = 9600;

TemperatureSensor temperatureSensor(&DHT11Pin);
SoilMoistureSensor soilMoistureSensor(soilMoisturePin);
WaterPump waterPump(&waterPumpPin);
WaterLevelSensor waterLevelSensor(waterSensorTriggerPin, waterSensorEchoPin);
WaterTank waterTank(PRISM, 16, 6.5, 12);
RtcDS3231<TwoWire> Rtc(Wire);

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

void initializeTimer()
{
  Rtc.Begin();

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

void updateDevices(Configuration config)
{

  waterPump.updateWateringTime(config.WateringTimeInSeconds);
  soilMoistureSensor.updateTresholdValues(config.SoilMoistureThreshold);
}

void setup()
{
  pinMode(sensorPowerPin, OUTPUT);
  pinMode(wifiWakeupPin, OUTPUT);
  pinMode(wakeUpPin, INPUT);
  pinMode(wifiReadyPin, INPUT);
  initializeTimer();
  wakeUpWifi();
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

    updateDevices(config);

    toggleSensors();

    SensorReading reading = sensorService.getSensorReadings();
    sensorService.water(reading);

    toggleSensors();

    String jsonResult = jsonService.convertSensorReadingsToJson(reading);

    Serial.println(jsonResult);

    attachInterrupt(wakeUpPin, wakeUp, FALLING);
    updateTimer(config.MeasuringIntervalInMinutes);
    Serial.end();
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(wakeUpPin);
    wakeUpWifi();
  }
}