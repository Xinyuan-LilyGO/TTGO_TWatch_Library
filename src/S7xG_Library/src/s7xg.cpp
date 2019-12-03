#include "s7xg.h"
#define GPS_RAW_FORMAT          "\n\r>> RAW UTC( %d/%d/%d %d:%d:%d ) LAT( %f %c ) LONG( %f %c ) POSITIONING( %fs )"
#define GPS_DMS_FORMAT          "\n\r>> DMS UTC( %d/%d/%d %d:%d:%d ) LAT( %d*%d'%f\" %c ) LONG( %d*%d'%f\" %c ) POSITIONING( %fs )"
#define GPS_DD_FORMAT           "\n\r>> DD UTC( %d/%d/%d %d:%d:%d ) LAT( %f %c ) LONG( %f %c ) POSITIONING( %fs )"
#define DMS_POSITIONING_DONE    "\n\r>> DMS"
#define RAW_POSITIONING_DONE    "\n\r>> RAW"
#define DD_POSITIONING_DONE     "\n\r>> DD"

#define DEFALUT_ACK             "Ok"
#define DEFALUT_TIMEOUT         5
#define ARR_SIZE(x)             (sizeof(x)/sizeof(x[0]))

const char *S7XG_Class::_gpsTypeArr[] = {
    "gps get_data raw",
    "gps get_data dms",
    "gps get_data dd"
};

const char *S7XG_Class::_gpsModeArr[] = {
    "gps set_mode auto",
    "gps set_mode manual",
    "gps set_mode idle",
};

const char *S7XG_Class::_gpsFormatArr[] = {
    "gps set_format_uplink raw",
    "gps set_format_uplink ipso",
    "gps set_format_uplink kiwi",
    "gps set_format_uplink utc_pos"
};

const char *S7XG_Class::_gpsSystemArr[] = {
    "gps set_satellite_system gps",
    "gps set_satellite_system hybrid"
};

const char *S7XG_Class::_loraPingpongComm[] = {
    "rf lora_rx_start 11223344556677889900",
    "rf lora_tx_start 0 10 11223344556677889900",
    "rf lora_rx_stop",
    "rf lora_tx_stop"
};

S7XG_Class::S7XG_Class()
{

}

void S7XG_Class::begin(HardwareSerial &port)
{
    _port = &port;
}

bool S7XG_Class::_sendAndWaitForAck(const char *c, const char *resp, uint8_t timeout, bool anyAck)
{
    int len = strlen(resp);
    int sum = 0;
    unsigned long timerStart, timerEnd;
    timerStart = millis();
    _port->print(c);
    S7XG_DEBUG("Send %s\n", c);
    while (1) {
        if (_port->available()) {
            if (anyAck) {
                break;
            }
            char c = _port->read();
            S7XG_DEBUGW(c);
            sum = (c == resp[sum]) ? sum + 1 : 0;
            if (sum == len)break;
        }
        timerEnd = millis();
        if (timerEnd - timerStart > 1000 * timeout) {
            snprintf(_lastError, sizeof(_lastError), "%s TIMEOUT", c);
            return false;
        }
    }
    if (!anyAck) {
        while (_port->available()) {
            _port->read();
        }
    }
    return true;
}

/**
 * *********************************************
 *              GNSS COMMAND
 * *********************************************
 */
void S7XG_Class::gpsReset()
{
    _sendAndWaitForAck("gps reset", DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::gpsSetLevelShift(bool en)
{
    snprintf(_buffer, sizeof(_buffer), "gps set_level_shift %s", en ? "on" : "off");
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::gpsSetStart(bool hot)
{
    snprintf(_buffer, sizeof(_buffer), "gps set_start %s", hot ? "hot" : "cold");
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::gpsSetSystem(uint8_t arg)
{
    return _sendAndWaitForAck(_gpsSystemArr[arg], DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::gpsSetPositioningCycle(uint32_t ms)
{
    if (ms < 1000) {
        ms = 1000;
    }
    if (ms > 600000) {
        ms = 600000;
    }
    snprintf(_buffer, sizeof(_buffer), "gps set_positioning_cycle %u", ms);
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::gpsSetPortUplink(uint8_t port)
{
    snprintf(_buffer, sizeof(_buffer), "gps set_port_uplink %u", port);
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::gpsSetFormatUplink(uint8_t format)
{
    if (format > ARR_SIZE(_gpsFormatArr)) {
        strcpy(_lastError, "format slect error");
        return false;
    }
    return _sendAndWaitForAck(_gpsFormatArr[format], DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::gpsSetMode(uint8_t mode)
{
    if (mode > ARR_SIZE(_gpsModeArr)) {
        strcpy(_lastError, "mode slect error");
        return false;
    }
    return _sendAndWaitForAck(_gpsModeArr[mode], DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::gpsStop()
{
    return gpsSetMode(GPS_MODE_IDLE);
}

GPS_Class S7XG_Class::gpsGetData(uint8_t type)
{
    int year, month, day, hour, minute, second;
    float lat, lng;
    char latC, lngC;
    float timeStamp;


    if (type > ARR_SIZE(_gpsTypeArr)) {
        return GPS_Class(0, 0, 0, 0, 0, 0, 0, 0);
    }

    _port->print(_gpsTypeArr[type]);

    while (_port->available()) {

        const char *data = _port->readString().c_str();

        S7XG_DEBUG("%s\n", data);

        switch (type) {
        case GPS_DATA_TYPE_RAW:
            if (0 == strncmp(data, RAW_POSITIONING_DONE, strlen(RAW_POSITIONING_DONE))) {
                 sscanf(data, GPS_RAW_FORMAT,
                             &year, &month, &day, &hour, &minute, &second,
                             &lat, &latC, &lng, &lngC, &timeStamp);
                return GPS_Class(year, month, day, hour, minute, second, lat, lng);
            }
            break;
        case GPS_DATA_TYPE_DMS:
            // >> DMS UTC( 2019/08/30 07:36:23 ) LAT( 22*39'39.29" N ) LONG( 114*3'37.73" E ) POSITIONING( 0.4s )
            // if (0 == strncmp(data, DMS_POSITIONING_DONE, strlen(DMS_POSITIONING_DONE))) {
            //     ret = sscanf(data, GPS_DMS_FORMAT,
            //                  &year, &month, &day, &hour, &minute, &second,
            //                  &lat, &latC, &lng, &lngC, &timeStamp
            //                 );
            //     return GPS_Class(year, month, day, hour, minute, second, lat, lng);
            // }
            return GPS_Class(0, 0, 0, 0, 0, 0, 0, 0);
            break;
        case GPS_DATA_TYPE_DD:
            if (0 == strncmp(data, DD_POSITIONING_DONE, strlen(DD_POSITIONING_DONE))) {
                sscanf(data, GPS_DD_FORMAT,
                             &year, &month, &day, &hour, &minute, &second,
                             &lat, &latC, &lng, &lngC, &timeStamp
                            );
                return GPS_Class(year, month, day, hour, minute, second, lat, lng);
            }
            break;
        default:
            break;
        }
    }
    return GPS_Class(0, 0, 0, 0, 0, 0, 0, 0);
}

/**
 * *********************************************
 *              LORA COMMAND
 * *********************************************
 */
bool S7XG_Class::loraSetPingPongReceiver()
{
    _pingPong = 0;
    return _setPingPongMode(0);
}

bool S7XG_Class::loraSetPingPongSender()
{
    _pingPong = 1;
    return _setPingPongMode(1);
}


bool S7XG_Class::_setPingPongMode(uint8_t mode)
{
    return  _sendAndWaitForAck(_loraPingpongComm[mode], DEFALUT_ACK, DEFALUT_TIMEOUT);
}

void S7XG_Class::loraPingPongStop()
{
    if (_pingPong < 0)return;
    switch (_pingPong) {
    case 0:
        _setPingPongMode(2);
        break;
    case 1:
        _setPingPongMode(3);
        break;
    default:
        break;
    }
    _pingPong = -1;
}

String S7XG_Class::loraGetPingPongMessage()
{
    if (_port->available()) {
        return _port->readString();
    }
    return String();
}

bool S7XG_Class::loraSetFrequency(uint32_t freq)
{
    snprintf(_buffer, sizeof(_buffer), "rf set_freq %u", freq);
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::loraSetPower(uint8_t dbm)
{
    snprintf(_buffer, sizeof(_buffer), "rf set_pwr %u", dbm);
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::loraSetSpreadingFactor(uint8_t sf)
{
    snprintf(_buffer, sizeof(_buffer), "rf set_sf %u", sf);
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::loraSetBandWidth(uint16_t bw)
{
    snprintf(_buffer, sizeof(_buffer), "rf set_bw %u", bw);
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::loraSetCodingRate(uint8_t cr)
{
    snprintf(_buffer, sizeof(_buffer), "rf set_cr 4/%u", cr);
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::loraSetPreambleLength(uint16_t pl)
{
    snprintf(_buffer, sizeof(_buffer), "rf set_prlen %u", pl);
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::loraSetCRC(bool en)
{
    snprintf(_buffer, sizeof(_buffer), "rf set_crc %s", en ? "on" : "off");
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::loraSetIQInvert(bool en)
{
    snprintf(_buffer, sizeof(_buffer), "rf set_iqi %s", en ? "on" : "off");
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::loraSetSyncWord(uint8_t sw)
{
    snprintf(_buffer, sizeof(_buffer), "rf set_sync %x", sw);
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::loraSetFreqDeviation(uint16_t dev)
{
    snprintf(_buffer, sizeof(_buffer), "rf set_fdev %u", dev);
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::loraTransmit(char *hex_data)
{
    snprintf(_buffer, sizeof(_buffer), "rf tx %s", hex_data);
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

bool S7XG_Class::loraReceiveContinuous(bool en)
{
    snprintf(_buffer, sizeof(_buffer), "rf rx_con %s", en ? "on" : "off");
    return _sendAndWaitForAck(_buffer, DEFALUT_ACK, DEFALUT_TIMEOUT);
}

/**
 * *********************************************
 *              SIP COMMAND
 * *********************************************
 */
void S7XG_Class::reset()
{
    _sendAndWaitForAck("sip reset", "", DEFALUT_TIMEOUT, true);
}

String S7XG_Class::getVersion()
{
    const char *ver;

    if (_sendAndWaitForAck("sip get_ver", DEFALUT_ACK, DEFALUT_TIMEOUT, true)) {
        if (_port->available()) {
            ver =  _port->readStringUntil('\r').c_str();
            S7XG_DEBUG("%s\n", ver);
            /* get rid of garbage data input */
            if (strlen(ver) < 3) {
                ver =  _port->readStringUntil('\r').c_str();
                S7XG_DEBUG("%s\n", ver);
            }
            sscanf(ver, ">> %s\n", _buffer);
            return String(_buffer);
        }
    }
    return String();
}

String S7XG_Class::getHardWareModel()
{
    const char *model;

    if (_sendAndWaitForAck("sip get_hw_model", DEFALUT_ACK, DEFALUT_TIMEOUT, true)) {
        if (_port->available()) {
            model =  _port->readStringUntil('\r').c_str();
            S7XG_DEBUG("%s\n", model);
            /* get rid of garbage data input */
            if (strlen(model) < 3) {
                model =  _port->readStringUntil('\r').c_str();
                S7XG_DEBUG("%s\n", model);
            }
            sscanf(model, ">> %s\n", _buffer);
            return String(_buffer);
        }
    }
    return String();
}
