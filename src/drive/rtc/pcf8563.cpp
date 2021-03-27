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

pcf8563.cpp - Arduino library for NXP PCF8563 RTC chip.
Created by Lewis he on April 1, 2019.
github:https://github.com/lewisxhe/PCF8563_Library
*/
/////////////////////////////////////////////////////////////////
#include "pcf8563.h"
#include <sys/time.h>

PCF8563_Class::PCF8563_Class(I2CBus &bus, uint8_t addr)
{
    _bus = &bus;
    _address = addr;
}

void PCF8563_Class::check()
{
    RTC_Date now = getDateTime();
    RTC_Date compiled = RTC_Date(__DATE__, __TIME__);

    // Serial.printf("%d:%d:%d - %d:%d:%d\n", compiled.year, compiled.month, compiled.day, compiled.hour, compiled.minute, compiled.second);

    if (now.year < compiled.year ||
            (now.year == compiled.year && now.month < compiled.month ) ||
            (now.year == compiled.year && now.month == compiled.month && now.day < compiled.day)) {
        setDateTime(compiled);
        log_i("reset rtc date time");
    }
}

void PCF8563_Class::setDateTime(RTC_Date date)
{
    setDateTime(date.year, date.month, date.day, date.hour, date.minute, date.second);
}

uint32_t PCF8563_Class::getDayOfWeek(uint32_t day, uint32_t month, uint32_t year)
{
    uint32_t val;
    if (month < 3) {
        month = 12u + month;
        year--;
    }
    val = (day + (((month + 1u) * 26u) / 10u) + year + (year / 4u) + (6u * (year / 100u)) + (year / 400u)) % 7u;
    if (0u == val) {
        val = 7;
    }
    return (val - 1);
}

void PCF8563_Class::setDateTime(uint16_t year,
                                uint8_t month,
                                uint8_t day,
                                uint8_t hour,
                                uint8_t minute,
                                uint8_t second)
{
    _data[0] = _dec_to_bcd(second) & (~PCF8563_VOL_LOW_MASK);
    _data[1] = _dec_to_bcd(minute);
    _data[2] = _dec_to_bcd(hour);
    _data[3] = _dec_to_bcd(day);
    _data[4] = getDayOfWeek(day, month, year);
    _data[5] = _dec_to_bcd(month);
    _data[6] = _dec_to_bcd(year % 100);

    if ( year < 2000) {
        _data[4] |= PCF8563_CENTURY_MASK;
    } else {
        _data[4] &= (~PCF8563_CENTURY_MASK);
    }
    _writeByte(PCF8563_SEC_REG, 7, _data);
}

bool PCF8563_Class::isValid()
{
    _readByte(PCF8563_SEC_REG, 1, &_isValid);
    return !(_isValid  & (1 << 7));
}

RTC_Date PCF8563_Class::getDateTime()
{
    uint16_t year;
    uint8_t century = 0;
    _readByte(PCF8563_SEC_REG, 7, _data);
    _voltageLow = (_data[0] & PCF8563_VOL_LOW_MASK);
    _data[0] = _bcd_to_dec(_data[0] & (~PCF8563_VOL_LOW_MASK));
    _data[1] = _bcd_to_dec(_data[1] & PCF8563_minuteS_MASK);
    _data[2] = _bcd_to_dec(_data[2] & PCF8563_HOUR_MASK);
    _data[3] = _bcd_to_dec(_data[3] & PCF8563_DAY_MASK);
    _data[4] = _bcd_to_dec(_data[4] & PCF8563_WEEKDAY_MASK);
    century = _data[5] & PCF8563_CENTURY_MASK;
    _data[5] = _bcd_to_dec(_data[5] & PCF8563_MONTH_MASK);
    year = _bcd_to_dec(_data[6]);
    year = century ? 1900 + year : 2000 + year;
    return RTC_Date(
               year,
               _data[5],
               _data[3],
               _data[2],
               _data[1],
               _data[0]
           );
}

RTC_Alarm PCF8563_Class::getAlarm()
{
    _readByte(PCF8563_ALRM_MIN_REG, 4, _data);
    _data[0] = _bcd_to_dec(_data[0] & (~PCF8563_minuteS_MASK));
    _data[1] = _bcd_to_dec(_data[1] & (~PCF8563_HOUR_MASK));
    _data[2] = _bcd_to_dec(_data[2] & (~PCF8563_DAY_MASK));
    _data[3] = _bcd_to_dec(_data[3] & (~PCF8563_WEEKDAY_MASK));
    return RTC_Alarm(_data[0], _data[1], _data[2], _data[3]);
}


void PCF8563_Class::enableAlarm()
{
    _readByte(PCF8563_STAT2_REG, 1, _data);
    _data[0] &= ~PCF8563_ALARM_AF;
    _data[0] |= (PCF8563_TIMER_TF | PCF8563_ALARM_AIE);
    _writeByte(PCF8563_STAT2_REG, 1, _data);
}

void PCF8563_Class::disableAlarm()
{
    _readByte(PCF8563_STAT2_REG, 1, _data);
    _data[0] &= ~(PCF8563_ALARM_AF | PCF8563_ALARM_AIE);
    _data[0] |= PCF8563_TIMER_TF;
    _writeByte(PCF8563_STAT2_REG, 1, _data);
}

void PCF8563_Class::resetAlarm()
{
    _readByte(PCF8563_STAT2_REG, 1, _data);
    _data[0] &= ~(PCF8563_ALARM_AF);
    _data[0] |= PCF8563_TIMER_TF;
    _writeByte(PCF8563_STAT2_REG, 1, _data);
}

bool PCF8563_Class::alarmActive()
{
    _readByte(PCF8563_STAT2_REG, 1, _data);
    return (bool)(_data[0] & PCF8563_ALARM_AF);
}

void PCF8563_Class::setAlarm(RTC_Alarm alarm)
{
    setAlarm(alarm.minute, alarm.hour, alarm.day, alarm.weekday);
}

void PCF8563_Class::setAlarm(uint8_t hour, uint8_t minute, uint8_t day, uint8_t weekday)
{
    if (minute != PCF8563_NO_ALARM) {
        _data[0] = _dec_to_bcd(constrain(minute, 0, 59));
        _data[0] &= ~PCF8563_ALARM_ENABLE;
    } else {
        _data[0] = PCF8563_ALARM_ENABLE;
    }

    if (hour != PCF8563_NO_ALARM) {
        _data[1] = _dec_to_bcd(constrain(hour, 0, 23));
        _data[1] &= ~PCF8563_ALARM_ENABLE;
    } else {
        _data[1] = PCF8563_ALARM_ENABLE;
    }
    if (day != PCF8563_NO_ALARM) {
        _data[2] = _dec_to_bcd(constrain(day, 1, 31));
        _data[2] &= ~PCF8563_ALARM_ENABLE;
    } else {
        _data[2] = PCF8563_ALARM_ENABLE;
    }
    if (weekday != PCF8563_NO_ALARM) {
        _data[3] = _dec_to_bcd(constrain(weekday, 0, 6));
        _data[3] &= ~PCF8563_ALARM_ENABLE;
    } else {
        _data[3] = PCF8563_ALARM_ENABLE;
    }
    _writeByte(PCF8563_ALRM_MIN_REG, 4, _data);
}

void PCF8563_Class::setAlarmByMinutes(uint8_t minute)
{
    setAlarm(PCF8563_NO_ALARM, minute, PCF8563_NO_ALARM, PCF8563_NO_ALARM);
}
void PCF8563_Class::setAlarmByDays(uint8_t day)
{
    setAlarm(PCF8563_NO_ALARM, PCF8563_NO_ALARM, day, PCF8563_NO_ALARM);
}
void PCF8563_Class::setAlarmByHours(uint8_t hour)
{
    setAlarm(hour, PCF8563_NO_ALARM, PCF8563_NO_ALARM, PCF8563_NO_ALARM);
}
void PCF8563_Class::setAlarmByWeekDay(uint8_t weekday)
{
    setAlarm(PCF8563_NO_ALARM, PCF8563_NO_ALARM, PCF8563_NO_ALARM, weekday);
}

bool PCF8563_Class::isTimerEnable()
{
    _readByte(PCF8563_STAT2_REG, 1, &_data[0]);
    _readByte(PCF8563_TIMER1_REG, 1, &_data[1]);
    if (_data[0] & PCF8563_TIMER_TIE) {
        return _data[1] & PCF8563_TIMER_TE ? true : false;
    }
    return false;
}

bool PCF8563_Class::isTimerActive()
{
    _readByte(PCF8563_STAT2_REG, 1, _data);
    return (bool) _data[0] & PCF8563_TIMER_TF;
}

void PCF8563_Class::enableTimer()
{
    _readByte(PCF8563_STAT2_REG, 1, &_data[0]);
    _readByte(PCF8563_TIMER1_REG, 1, &_data[1]);
    _data[0] &= ~PCF8563_TIMER_TF;
    _data[0] |= (PCF8563_ALARM_AF | PCF8563_TIMER_TIE);
    _data[1] |= PCF8563_TIMER_TE;
    _writeByte(PCF8563_STAT2_REG, 1, &_data[0]);
    _writeByte(PCF8563_TIMER1_REG, 1, &_data[1]);
}

void PCF8563_Class::disableTimer()
{
    _readByte(PCF8563_STAT2_REG, 1, _data);
    _data[0] &= ~PCF8563_TIMER_TF;
    _data[1] |= PCF8563_ALARM_AF;
    _writeByte(PCF8563_STAT2_REG, 1, _data);
}

void PCF8563_Class::setTimer(uint8_t val, uint8_t freq, bool enInterrupt)
{
    _readByte(PCF8563_STAT2_REG, 1, &_data[0]);
    _readByte(PCF8563_TIMER1_REG, 1, &_data[1]);
    if (enInterrupt) {
        _data[0] |= 1 << 4;
    } else {
        _data[0] &= ~(1 << 4);
    }
    _data[1] |= (freq &  PCF8563_TIMER_TD10);
    _data[2] = val;
    _writeByte(PCF8563_STAT2_REG, 1, &_data[0]);
    _writeByte(PCF8563_TIMER1_REG, 1, &_data[1]);
    _writeByte(PCF8563_TIMER2_REG, 1, &_data[2]);
}

void PCF8563_Class::clearTimer()
{
    _readByte(PCF8563_STAT2_REG, 1, _data);
    _data[0] &= ~(PCF8563_TIMER_TF | PCF8563_TIMER_TIE);
    _data[0] |= PCF8563_ALARM_AF;
    _data[1] = 0x00;
    _writeByte(PCF8563_STAT2_REG, 1, &_data[0]);
    _writeByte(PCF8563_TIMER1_REG, 1, &_data[1]);
}

bool PCF8563_Class::enableCLK(uint8_t freq)
{
    if (freq >= PCF8563_CLK_MAX) return false;
    _data[0] = freq | PCF8563_CLK_ENABLE;
    _writeByte(PCF8563_SQW_REG, 1, _data);
    return true;
}

void PCF8563_Class::disableCLK()
{
    _data[0] = 0x00;
    _writeByte(PCF8563_SQW_REG, 1, _data);
}

const char *PCF8563_Class::formatDateTime(uint8_t style)
{
    RTC_Date t = getDateTime();
    switch (style) {
    case PCF_TIMEFORMAT_HM:
        snprintf(format, sizeof(format), "%02d:%02d", t.hour, t.minute);
        break;
    case PCF_TIMEFORMAT_YYYY_MM_DD:
        snprintf(format, sizeof(format), "%02d-%02d-%02d", t.year, t.month, t.day);
        break;
    case PCF_TIMEFORMAT_MM_DD_YYYY:
        snprintf(format, sizeof(format), "%02d-%02d-%02d", t.month, t.day, t.year);
        break;
    case PCF_TIMEFORMAT_DD_MM_YYYY:
        snprintf(format, sizeof(format), "%02d-%02d-%02d", t.day, t.month, t.year);
        break;
    case PCF_TIMEFORMAT_YYYY_MM_DD_H_M_S:
        snprintf(format, sizeof(format), "%02d-%02d-%02d/%02d:%02d:%02d", t.year, t.month, t.day, t.hour, t.minute, t.second);
        break;
    case PCF_TIMEFORMAT_HMS:
    default:
        snprintf(format, sizeof(format), "%02d:%02d:%02d", t.hour, t.minute, t.second);
        break;
    }
    return format;
}


void PCF8563_Class::syncToSystem()
{
    struct tm t_tm;
    struct timeval val;
    RTC_Date dt = getDateTime();
    // log_i("syncToSystem: %d %d %d - %d %d %d \n",  dt.year, dt.month, dt.day,  dt.hour, dt.minute, dt.second);
    t_tm.tm_hour = dt.hour;
    t_tm.tm_min = dt.minute;
    t_tm.tm_sec = dt.second;
    t_tm.tm_year = dt.year - 1900;    //Year, whose value starts from 1900
    t_tm.tm_mon = dt.month - 1;       //Month (starting from January, 0 for January) - Value range is [0,11]
    t_tm.tm_mday = dt.day;
    val.tv_sec = mktime(&t_tm);
    val.tv_usec = 0;

    settimeofday(&val, NULL);
    // log_i("syncToSystem: %d %d %d - %d %d %d \n", t_tm.tm_year, t_tm.tm_mon + 1, t_tm.tm_mday, t_tm.tm_hour, t_tm.tm_min, t_tm.tm_sec);
}

void PCF8563_Class::syncToRtc()
{
    time_t now;
    struct tm  info;
    time(&now);
    localtime_r(&now, &info);
    setDateTime(info.tm_year + 1900, info.tm_mon + 1, info.tm_mday, info.tm_hour, info.tm_min, info.tm_sec);
    // Serial.printf("syncToRtc: %d %d %d - %d %d %d \n", info.tm_year, info.tm_mon + 1, info.tm_mday, info.tm_hour, info.tm_min, info.tm_sec);
}

RTC_Date::RTC_Date(
) : year(0), month(0), day(0), hour(0), minute(0), second(0)
{

}


RTC_Date::RTC_Date(uint16_t y,
                   uint8_t m,
                   uint8_t d,
                   uint8_t h,
                   uint8_t mm,
                   uint8_t s
                  ) : year(y), month(m), day(d), hour(h), minute(mm), second(s)
{

}

uint8_t RTC_Date::StringToUint8(const char *pString)
{
    uint8_t value = 0;

    // skip leading 0 and spaces
    while ('0' == *pString || *pString == ' ') {
        pString++;
    }

    // calculate number until we hit non-numeral char
    while ('0' <= *pString && *pString <= '9') {
        value *= 10;
        value += *pString - '0';
        pString++;
    }
    return value;
}


RTC_Date::RTC_Date(const char *date, const char *time)
{
    // sample input: date = "Dec 26 2009", time = "12:34:56"
    year = 2000 + StringToUint8(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (date[0]) {
    case 'J':
        if ( date[1] == 'a' )
            month = 1;
        else if ( date[2] == 'n' )
            month = 6;
        else
            month = 7;
        break;
    case 'F':
        month = 2;
        break;
    case 'A':
        month = date[1] == 'p' ? 4 : 8;
        break;
    case 'M':
        month = date[2] == 'r' ? 3 : 5;
        break;
    case 'S':
        month = 9;
        break;
    case 'O':
        month = 10;
        break;
    case 'N':
        month = 11;
        break;
    case 'D':
        month = 12;
        break;
    }
    day = StringToUint8(date + 4);
    hour = StringToUint8(time);
    minute = StringToUint8(time + 3);
    second = StringToUint8(time + 6);
}


RTC_Alarm::RTC_Alarm(
    uint8_t m,
    uint8_t h,
    uint8_t d,
    uint8_t w
): minute(m), hour(h), day(d), weekday(w)
{

}


uint8_t PCF8563_Class::status2()
{
    _readByte(PCF8563_STAT2_REG, 1, _data);
    return _data[0];
}


bool RTC_Date::operator==(RTC_Date d)
{
    return ((d.year == year) && (d.month == month) && (d.day == day) && (d.hour == hour) &&  (d.minute == minute));
}
