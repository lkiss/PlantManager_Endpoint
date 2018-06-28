#include <Wire.h>
#include <RtcDS3231.h>
#include "LowPower.h"

#include "./devices/devices.h"
#include "./services/services.h"
#include "./constants.h"

int errorLedPin = LED_BUILTIN;
int numberOfWifiRestAttempts = 0;
int DHT11Pin = 5;
int waterPumpPin = 6;
int waterSensorEchoPin = 7;
int waterSensorTriggerPin = 8;
int sensorPowerPin = 4;
int soilMoisturePin = A0;
int bateryMeterPin = A1;
int wakeUpPin = INT0;
int wifiWakeupPin = 10;
int wifiReadyPin = 9;
int solarPanelConnectionPin = 11;

float batteryVoltage = 0;

bool isWifiPowered = false;
bool isSensorPowered = false;

int baudRate = 9600;

TemperatureSensor temperatureSensor(&DHT11Pin);
SoilMoistureSensor soilMoistureSensor(soilMoisturePin);
WaterPump waterPump(&waterPumpPin);
WaterLevelSensor waterLevelSensor(waterSensorTriggerPin, waterSensorEchoPin);
WaterTank waterTank(PRISM, 16, 6.5, 12);
RtcDS3231<TwoWire> Rtc(Wire);
BatteryMeter batteryMeter(bateryMeterPin);

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

void setup()
{
  pinMode(errorLedPin, OUTPUT);
  pinMode(sensorPowerPin, OUTPUT);
  pinMode(wifiWakeupPin, OUTPUT);
  pinMode(solarPanelConnectionPin, OUTPUT);
  pinMode(wakeUpPin, INPUT);
  pinMode(wifiReadyPin, INPUT);

  batteryVoltage = batteryMeter.getBatteryVoltage();

  initializeTimer();
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
    SensorReading reading = sensorService.getSensorReadings(batteryVoltage);
    toggleSensors();

    if (config.ShouldUseCloud)
    {
      String jsonResult = jsonService.convertSensorReadingsToJson(reading);
      Serial.println(jsonResult);
      Serial.flush();
    }

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
    numberOfWifiRestAttempts = 0;

    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(wakeUpPin);

    batteryVoltage = batteryMeter.getBatteryVoltage();
    wakeUpWifi();
    delay(3000);
  }
  else
  {
    if (numberOfWifiRestAttempts == configService.getConfiguration().MaximumNumberOfWifiRestAttempts)
    {
      for (int i = 0; i < 10; i++)
      {
        digitalWrite(errorLedPin, HIGH);
        delay(1000);
        digitalWrite(errorLedPin, LOW);
        delay(1000);
      }
      digitalWrite(errorLedPin, HIGH);
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    }
    numberOfWifiRestAttempts++;
    wakeUpWifi();
    delay(5000);
  }
}