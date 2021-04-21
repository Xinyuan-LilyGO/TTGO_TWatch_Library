/////////////////////////////////////////////////////////////////
/*
MIT License

Copyright (c) 2019 lewis he

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

pcf8563.h - Arduino library for NXP PCF8563 RTC chip.
Created by Lewis he on April 1, 2019.
github:https://github.com/lewisxhe/PCF8563_Library
*/
/////////////////////////////////////////////////////////////////
#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "../i2c/i2c_bus.h"

#define PCF8563_SLAVE_ADDRESS   (0x51) //7-bit I2C Address

//! REG MAP
#define PCF8563_STAT1_REG       (0x00)
#define PCF8563_STAT2_REG       (0x01)
#define PCF8563_SEC_REG         (0x02)
#define PCF8563_MIN_REG         (0x03)
#define PCF8563_HR_REG          (0x04)
#define PCF8563_DAY_REG         (0x05)
#define PCF8563_WEEKDAY_REG     (0x06)
#define PCF8563_MONTH_REG       (0x07)
#define PCF8563_YEAR_REG        (0x08)
#define PCF8563_ALRM_MIN_REG    (0x09)
#define PCF8563_SQW_REG         (0x0D)
#define PCF8563_TIMER1_REG      (0x0E)
#define PCF8563_TIMER2_REG      (0x0F)

#define PCF8563_VOL_LOW_MASK    (0x80)
#define PCF8563_minuteS_MASK    (0x7F)
#define PCF8563_HOUR_MASK       (0x3F)
#define PCF8563_WEEKDAY_MASK    (0x07)
#define PCF8563_CENTURY_MASK    (0x80)
#define PCF8563_DAY_MASK        (0x3F)
#define PCF8563_MONTH_MASK      (0x1F)
#define PCF8563_TIMER_CTL_MASK  (0x03)


#define PCF8563_ALARM_AF        (0x08)
#define PCF8563_TIMER_TF        (0x04)
#define PCF8563_ALARM_AIE       (0x02)
#define PCF8563_TIMER_TIE       (0x01)
#define PCF8563_TIMER_TE        (0x80)
#define PCF8563_TIMER_TD10      (0x03)

#define PCF8563_NO_ALARM        (0xFF)
#define PCF8563_ALARM_ENABLE    (0x80)
#define PCF8563_CLK_ENABLE      (0x80)

enum {
    PCF8563_CLK_32_768KHZ,
    PCF8563_CLK_1024KHZ,
    PCF8563_CLK_32HZ,
    PCF8563_CLK_1HZ,
    PCF8563_CLK_MAX
};

enum {
    PCF_TIMEFORMAT_HM,
    PCF_TIMEFORMAT_HMS,
    PCF_TIMEFORMAT_YYYY_MM_DD,
    PCF_TIMEFORMAT_MM_DD_YYYY,
    PCF_TIMEFORMAT_DD_MM_YYYY,
    PCF_TIMEFORMAT_YYYY_MM_DD_H_M_S,
};

class RTC_Date
{
public:
    RTC_Date();
    RTC_Date(const char *date, const char *time);
    RTC_Date(uint16_t year,
             uint8_t month,
             uint8_t day,
             uint8_t hour,
             uint8_t minute,
             uint8_t second
            );
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    bool operator==(RTC_Date d);
private:
    uint8_t StringToUint8(const char *pString);
};

class RTC_Alarm
{
public:
    RTC_Alarm(void);
    RTC_Alarm(
        uint8_t minute,
        uint8_t hour,
        uint8_t day,
        uint8_t weekday
    );
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t weekday;
};


class PCF8563_Class
{
public:
    PCF8563_Class(I2CBus &bus, uint8_t addr = PCF8563_SLAVE_ADDRESS);

    void check();

    void setDateTime(uint16_t year,
                     uint8_t month,
                     uint8_t day,
                     uint8_t hour,
                     uint8_t minute,
                     uint8_t second);

    void setDateTime(RTC_Date date);
    RTC_Date getDateTime();
    RTC_Alarm getAlarm();
    void enableAlarm();
    void disableAlarm();
    bool alarmActive();
    void resetAlarm();
    void setAlarm(RTC_Alarm alarm);
    void setAlarm(uint8_t hour, uint8_t minute, uint8_t day, uint8_t weekday);
    bool isValid();

    void setAlarmByWeekDay(uint8_t weekday);
    void setAlarmByHours(uint8_t hour);
    void setAlarmByDays(uint8_t day);
    void setAlarmByMinutes(uint8_t minute);

    bool isTimerEnable();
    bool isTimerActive();
    void enableTimer();
    void disableTimer();
    void setTimer(uint8_t val, uint8_t freq, bool enInterrupt);
    void clearTimer();

    bool enableCLK(uint8_t freq);
    void disableCLK();

    void syncToSystem();
    void syncToRtc();

    const char *formatDateTime(uint8_t style = PCF_TIMEFORMAT_HMS);
    uint32_t getDayOfWeek(uint32_t day, uint32_t month, uint32_t year);
    uint8_t status2();

private:
    uint8_t _bcd_to_dec(uint8_t val)
    {
        return ( (val / 16 * 10) + (val % 16) );
    }
    uint8_t _dec_to_bcd(uint8_t val)
    {
        return ( (val / 10 * 16) + (val % 10) );
    }
    int _readByte(uint8_t reg, uint8_t nbytes, uint8_t *data)
    {
        return (int)_bus->readBytes(_address, reg, data, nbytes);
        // _i2cPort->beginTransmission(_address);
        // _i2cPort->write(reg);
        // _i2cPort->endTransmission();
        // _i2cPort->requestFrom(_address, nbytes);
        // uint8_t index = 0;
        // while (_i2cPort->available())
        //     data[index++] = _i2cPort->read();
    }

    int _writeByte(uint8_t reg, uint8_t nbytes, uint8_t *data)
    {
        return (int)_bus->writeBytes(_address, reg, data, nbytes);
        // _i2cPort->beginTransmission(_address);
        // _i2cPort->write(reg);
        // for (uint8_t i = 0; i < nbytes; i++) {
        //     _i2cPort->write(data[i]);
        // }
        // _i2cPort->endTransmission();
    }

    uint8_t _isValid = false;
    uint8_t _address;
    bool _init = false;
    TwoWire *_i2cPort;
    uint8_t _data[16];
    bool _voltageLow;
    char format[128];
    I2CBus *_bus = nullptr;
};
