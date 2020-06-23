#pragma once

#include <Arduino.h>

#define S7XG_DEBUG(...) Serial.printf( __VA_ARGS__ )

#ifndef S7XG_DEBUG
#define S7XG_DEBUG(...)
#endif

#define S7XG_DEBUGW(x) Serial.write( x )

#ifndef S7XG_DEBUGW
#define S7XG_DEBUGW(...)
#endif


enum {
    GPS_DATA_TYPE_RAW,
    GPS_DATA_TYPE_DMS,
    GPS_DATA_TYPE_DD
};

enum {
    GPS_MODE_AUTO,
    GPS_MODE_MANUAL,
    GPS_MODE_IDLE
};

enum {
    GPS_UPLINK_FORMAT_RAW,
    GPS_UPLINK_FORMAT_IPSO,
    GPS_UPLINK_FORMAT_KIWI,
    GPS_UPLINK_FORMAT_UTC_POS
};

enum {
    GPS_STATE_SYS_GPS,
    GPS_STATE_SYS_GPS_GLONASS
};

class GPS_Class
{
public:
    GPS_Class(uint16_t year,
              uint8_t month,
              uint8_t day,
              uint8_t hour,
              uint8_t minute,
              uint8_t second,
              float lat,
              float lng):
        _year(year),
        _month(month),
        _day(day),
        _hour(hour),
        _minute(minute),
        _second(second),
        _lat(lat),
        _lng(lng)
    {
        _vaild = year == day ? false : true;
    }
    uint16_t year()
    {
        return _year;
    }
    uint8_t month()
    {
        return _month;
    }
    uint8_t day()
    {
        return _day;
    }
    uint8_t hour()
    {
        return _hour;
    }
    uint8_t minute()
    {
        return _minute;
    }
    uint8_t second()
    {
        return _second;
    }
    float lat()
    {
        return _lat;
    }
    float lng()
    {
        return _lng;
    }
    bool isVaild()
    {
        return _vaild;
    }
private:
    bool _vaild;
    uint16_t _year;
    uint8_t _month;
    uint8_t _day;
    uint8_t _hour;
    uint8_t _minute;
    uint8_t _second;
    float _lat;
    float _lng;
};


class S7XG_Class
{

public:
    S7XG_Class();
    void begin(HardwareSerial &port);
    void reset();
    void gpsReset();
    bool gpsSetLevelShift(bool en = true);
    bool gpsSetStart(bool hot = true);
    bool gpsSetSystem(uint8_t arg);
    bool gpsSetPositioningCycle(uint32_t ms);
    bool gpsSetPortUplink(uint8_t port);
    bool gpsSetFormatUplink(uint8_t format);
    bool gpsSetMode(uint8_t mode);
    bool gpsStop();
    GPS_Class gpsGetData(uint8_t mode = GPS_DATA_TYPE_DD);


    bool loraSetPingPongReceiver();
    bool loraSetPingPongSender();
    bool loraPingPongReceiverStop();
    bool loraPingPongSenderStop();
    void loraPingPongStop();
    String loraGetPingPongMessage();
    bool loraSetFrequency(uint32_t freq);
    bool loraSetPower(uint8_t dbm);
    bool loraSetSpreadingFactor(uint8_t sf);
    bool loraSetBandWidth(uint16_t bw);
    bool loraSetCodingRate(uint8_t cr);
    bool loraSetPreambleLength(uint16_t pl);
    bool loraSetCRC(bool en);
    bool loraSetIQInvert(bool en);
    bool loraSetSyncWord(uint8_t sw);
    bool loraSetFreqDeviation(uint16_t dev);
    bool loraTransmit(char *hex_data);
    bool loraReceiveContinuous(bool en);

    String getVersion();
    String getHardWareModel();

private:
    bool _setPingPongMode(uint8_t mode);

    bool _sendAndWaitForAck(const char *c, const char *resp, uint8_t timeout, bool anyAck = false);
    char _lastError[256];
    char _buffer[128];

    static const char *_gpsTypeArr[];
    static const char *_gpsModeArr[];
    static const char *_gpsFormatArr[];
    static const char *_gpsSystemArr[];
    static const char *_loraPingpongComm[];

    int _pingPong = -1;
    HardwareSerial *_port;
};