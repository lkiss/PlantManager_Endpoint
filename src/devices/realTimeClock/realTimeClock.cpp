#include "./realTimeClock.h"

RealTimeClock::RealTimeClock()
{
    this->Rtc = new RtcDS3231<TwoWire>(Wire);
    this->InitializeRtc();
}

void RealTimeClock::UpdateTimer(int measuringIntervalInMinutes)
{
    RtcDateTime now = this->Rtc->GetDateTime();

    RtcDateTime wakeUpTime = now + (measuringIntervalInMinutes * 60);

    DS3231AlarmOne wakeTimer(
        wakeUpTime.Day(),
        wakeUpTime.Hour(),
        wakeUpTime.Minute(),
        wakeUpTime.Second(),
        DS3231AlarmOneControl_HoursMinutesSecondsMatch);
    this->Rtc->SetAlarmOne(wakeTimer);
    this->Rtc->LatchAlarmsTriggeredFlags();
}

void RealTimeClock::InitializeRtc()
{
    this->Rtc->Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    if (!this->Rtc->IsDateTimeValid())
    {
        this->Rtc->SetDateTime(compiled);
    }

    if (!this->Rtc->GetIsRunning())
    {
        this->Rtc->SetIsRunning(true);
    }

    RtcDateTime now = this->Rtc->GetDateTime();
    if (now < compiled)
    {
        this->Rtc->SetDateTime(compiled);
    }

    this->Rtc->Enable32kHzPin(false);
    this->Rtc->SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
}