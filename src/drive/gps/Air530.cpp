/////////////////////////////////////////////////////////////////
/*
MIT License

Copyright (c) 2020 lewis he

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

Air530.cpp - Arduino library for Air530 gps module.
Created by Lewis he on December 28, 2020.
github:https://github.com/lewisxhe/Air530_Library
*/
/////////////////////////////////////////////////////////////////



#include "Air530.h"

#define _LOOP_TO_TIMEOUT(func)      do{                             \
                                    uint32_t start = millis() + 500;\
                                    while (millis() < start ){      \
                                            func;                   \
                                    }                               \
                                } while (0);



/**
 * @brief   Air530
 * @param  *ser:        uart stream
 * @param  wakeupPin:   Wake-up pin
 * @retval None
 */
Air530::Air530(Stream *ser, uint8_t wakeupPin)
{
    stream = ser;
    wakeup_pin = wakeupPin;
    if (wakeup_pin > 0) {
        pinMode(wakeup_pin, OUTPUT);
    }
    wakeup();
}


Air530::~Air530()
{
    stream = nullptr;
}

/**
 * @brief   restart
 * @note    System restart command
 * @param   mode: See Air530_RestartMode
 * @retval  None
 */
void Air530::restart(Air530_RestartMode mode)
{
    sendCmd("$PGKC030,%d,1", mode);
}


/**
 * @brief   erase
 * @note    Erase auxiliary positioning data in flash
 * @retval  None
 */
void Air530::erase(void)
{
    sendCmd("$PGKC040*2B");
}

/**
 * @brief   sleep
 * @note    Set Air530 to sleep
 * @retval  None
 */
void Air530::sleep(void)
{
    sendCmd("$PGKC051,0*37");
}

/**
 * @brief   stop
 * @note    Set Air530 to stop
 * @retval  None
 */
void Air530::stop(void)
{
    sendCmd("$PGKC051,1*36");
}

/**
 * @brief   wakeup
 * @note    wakeup Air530
 * @retval  None
 */
void Air530::wakeup(void)
{
    if (wakeup_pin > 0) {
        digitalWrite(wakeup_pin, HIGH); delay(60);
        digitalWrite(wakeup_pin, LOW); delay(200);
        digitalWrite(wakeup_pin, HIGH);
    }
}


/**
 * @brief   setNormalMode
 * @note    Normal operation mode
 * @retval  None
 */
void Air530::setNormalMode(void)
{
    sendCmd("$PGKC105,0*37");
}

/**
 * @brief   setCycleTrackingMode
 * @note    Cycle ultra-low power tracking mode, need to pull up WAKE to wake up
 * @param   runMillis: Running time (ms)
 * @param   sleepMillis: Sleep time (ms)
 * @retval  None
 */
void Air530::setCycleTrackingMode(uint32_t runMillis, uint32_t sleepMillis)
{
    //TODO:Unknown error parameter
    sendCmd("$PGKC105,1,%u,%u", runMillis, sleepMillis);
}

/**
 * @brief   setCycleLowPowerMode
 * @note    Cycle low power mode
 * @param   runMillis: Running time (ms)
 * @param   sleepMillis: Sleep time (ms)
 * @retval  None
 */
void Air530::setCycleLowPowerMode(uint32_t runMillis, uint32_t sleepMillis)
{
    //TODO:Unknown error parameter
    sendCmd("$PGKC105,2,%u,%u", runMillis, sleepMillis);
}

/**
 * @brief   setTrackingMode
 * @note    Ultra-low power tracking mode, need to pull up WAKE to wake up
 * @retval  None
 */
void Air530::setTrackingMode(void)
{
    sendCmd("$PGKC105,4*33");
}

/**
 * @brief   setAutoLowPowerMode
 * @note    Automatic low power consumption mode, can wake up via serial port
 * @retval  None
 */
void Air530::setAutoLowPowerMode(void)
{
    sendCmd("$PGKC105,8*3F");
}

/**
 * @brief   setAutoTrackingMode
 * @note    Automatic ultra-low power tracking mode, need to pull up WAKE to wake up
 * @retval  None
 */
void Air530::setAutoTrackingMode(void)
{
    sendCmd("$PGKC105,9*3E");
}

/**
 * @brief   enableQZSS_NMEA
 * @note    Enable QZSS NMEA format output
 * @retval  None
 */
void Air530::enableQZSS_NMEA(void)
{
    sendCmd("$PGKC113,0*30");
}

/**
 * @brief   disableQZSS_NMEA
 * @note    Disable QZSS NMEA format output
 * @retval  None
 */
void Air530::disableQZSS_NMEA(void)
{
    sendCmd("$PGKC113,1*31");
}

/**
 * @brief   enableQZSS
 * @note    Enable QZSS function
 * @retval  None
 */
void Air530::enableQZSS(void)
{
    sendCmd("$PGKC114,0*37");
}

/**
 * @brief   disableQZSS
 * @note    Disable QZSS function
 * @retval  None
 */
void Air530::disableQZSS(void)
{
    sendCmd("$PGKC114,1*36");
}

/**
 * @brief  setSearchMode
 * @note   Command: 115 Set search mode
 * @param  gps:     true is GPS on , false is GPS off
 * @param  glonass: true is Glonass on,false is Glonass off
 * @param  beidou:  true is Beidou on,false is Beidou off
 * @param  galieo:  true is Galieo on,false is Galieo off
 * @retval None
 */
void Air530::setSearchMode(bool gps, bool glonass, bool beidou, bool galieo)
{
    sendCmd("$PGKC115,%u,%u,%u,%u", gps, glonass, beidou, galieo);
}


/**
  * @brief  setNMEABaud
  * @note   Set NMEA output baud rate
  * @param  baud: 9600,19200,38400,57600,115200,921600
  * @retval None
  */
void Air530::setNMEABaud(uint32_t baud)
{
    (void)baud;
    // TODO: Don't change
}

/**
 * @brief   setBaud
 * @note    Set NMEA serial port parameters
 * @param   baud: 9600,19200,38400,57600,115200,921600
 * @retval  None
 */
void Air530::setBaud(uint32_t baud)
{
    sendCmd("$PGKC147,%u", baud);
}

/**
 * @brief   setPPS
 * @note    PPS settings
 * @param   mode:        see Air530_1PPS_Mode
 * @param   ppsWidth:    PPS pulse width (ms), the requirement is less than 999
 * @param   ppsPeriod:   PPS period (ms), which must be greater than the PPS pulse width
 * @retval
 */
bool Air530::setPPS(Air530_1PPS_Mode mode, uint16_t ppsWidth, uint16_t ppsPeriod)
{
    if (ppsWidth >= 999) {
        ppsWidth = 998;
    }
    if (ppsWidth < ppsPeriod) {
        return false;
    }
    sendCmd("$PGKC161,%u,%u,%u", mode, ppsWidth, ppsPeriod);
    return true;
}

/**
 * @brief   get NMEA Interval
 * @note    NMEA message interval
 * @retval  NMEA Interval(millisecond)
 */
uint32_t Air530::getNMEAInterval(void)
{
    sendCmd("$PGKC201*2C");
    _LOOP_TO_TIMEOUT(
    while (stream->available()) {
    if (stream->read() == '$') {
            if (!stream->readStringUntil(',').startsWith("PGKC202")) {
                break;
            } else {
                return stream->readStringUntil('*').toInt();
            }
        }
    });
    return 0;
}

/**
* @brief    setInterval
* @note     Configure the interval for outputting NMEA messages (in ms)
* @param    ms: millisecond
* @retval   None
*/
void Air530::setNMEAInterval(uint16_t ms)
{
    if (ms < 200) {
        ms = 200;
    }
    if (ms > 10000) {
        ms = 10000;
    }
    sendCmd("$PGKC101,%u", ms);
}


/**
 * @brief   enableSBAS
 * @note    Turn on SBAS function
 * @retval  None
 */
void Air530::enableSBAS(void)
{
    sendCmd("$PGKC239,0*3B");
}

/**
 * @brief   disableSBAS
 * @note    Turn off SBAS function
 * @retval None
 */
void Air530::disableSBAS(void)
{
    sendCmd("$PGKC239,1*3A");
}

/**
 * @brief   getSBASEnable
 * @note    Query whether SBAS is enabled
 * @retval  true is enable ,false is disable
 */
bool Air530::getSBASEnable(void)
{
    sendCmd("$PGKC240*29");
    _LOOP_TO_TIMEOUT(
    while (stream->available()) {
    if (stream->read() == '$') {
            if (!stream->readStringUntil(',').startsWith("PGKC241")) {
                break;
            } else {
                return stream->readStringUntil('*').toInt();
            }
        }
    });
    return false;
}


/**
 * @brief  setNMEAStatement
 * @note   Set NMEA sentence output enable
 * @param  gll: GLL  false off , true on
 * @param  rmc: RMC  false off , true on
 * @param  vtg: VTG  false off , true on
 * @param  gga: GGA  false off , true on
 * @param  gsa: GSA  false off , true on
 * @param  gsv: GSV  false off , true on
 * @param  grs: GRS  false off , true on
 * @param  gst: GST  false off , true on
 * @retval None
 */
void Air530::setNMEAStatement(bool gll, bool rmc, bool vtg, bool gga, bool gsa, bool gsv, bool grs, bool gst)
{
    sendCmd("$PGKC242,%d,%d,%d,%d,%d,%d,%d,%d,0,0,0,0,0,0,0,0,0,0,0", gll, rmc, vtg, gga, gsa, gsv, grs, gst);
}

/**
 * @brief   disableNMEAOutput
 * @note    Disable NMEA Output
 * @retval  None
 */
void Air530::disableNMEAOutput(void)
{
    setNMEAStatement(false, false, false, false, false, false, false, false);
}

/**
 * @brief   enableNMEAOutput
 * @note    Enable NMEA Output
 * @retval  None
 */
void Air530::enableNMEAOutput(void)
{
    setNMEAStatement(true, true, true, true, true, true, true, true);
}

// Command: 243
// Command: 244

/**
 * @brief   setDateTime
 * @note    Set RTC time
 * @param  year:
 * @param  month:1~12
 * @param  day:  1~31
 * @param  hour: 0~23
 * @param  min:  0~59
 * @param  sec:  0~59
 * @retval None
 */
void Air530::setDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
    sendCmd("$PGKC278,%u,%u,%u,%u,%u,%u", year, month, day, hour, min, sec);
}


/**
 * @brief   getDateTime
 * @note
 * @param   &year:
 * @param   &month:
 * @param   &day:
 * @param   &hour:
 * @param   &min:
 * @param   &sec:
 * @retval  None
 */
bool Air530::getDateTime(uint16_t &year, uint8_t &month, uint8_t  &day, uint8_t  &hour, uint8_t &min, uint8_t  &sec)
{
    sendCmd("$PGKC279*23");
    _LOOP_TO_TIMEOUT(
    while (stream->available()) {
    if (stream->read() == '$') {
            if (!stream->readStringUntil(',').startsWith("PGKC280")) {
                break;
            } else {
                year    = stream->readStringUntil(',').toInt();
                month   = stream->readStringUntil(',').toInt();
                day     = stream->readStringUntil(',').toInt();
                hour    = stream->readStringUntil(',').toInt();
                min     = stream->readStringUntil(',').toInt();
                sec     = stream->readStringUntil('*').toInt();
                return true;
            }
        }
    });
    return false;
}

//Command: 284  Set speed threshold
//Command: 356  Set HDOP threshold
//Command: 357  Get HDOP threshold

/**
 * @brief   getSoftVersion
 * @note    Query the version number of the current software
 * @retval  version
 */
const char *Air530::getSoftVersion(void)
{
    sendCmd("$PGKC462*2F");
    _LOOP_TO_TIMEOUT(
    while (stream->available()) {
    if (stream->read() == '$') {
            if (!stream->readStringUntil(',').startsWith("PGKC463")) {
                break;
            } else {
                return stream->readStringUntil(',').c_str();
            }
        }
    });
    return "None";
}

/**
 * @brief  setProbablyLoaction
 * @note   Set approximate location information and time information to speed up positioning
 * @example:  setProbablyLoaction(28.166450,120.389700,0,2017,3,15,12,0,0)
 * @retval None
 */
void Air530::setProbablyLoaction(float lat, float lng, uint16_t altitude, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
    sendCmd("$PGKC639,%f,%f,%u,%u,%u,%u,%u,%u,%u", lat, lng, altitude, year, month, day, hour, min, sec);
}

/**
  * @brief   process
  * @note    Processing sentences
  * @retval  True is running  ,False is abnormal
  */
bool Air530::process(/*Stream *serial = nullptr*/void)
{
    if (!stream)return false;
    while (stream->available()) {
        char r = stream->read();
        // if (serial) {
        //     serial->write(r);
        // }
        encode(r);
    }

    if (charsProcessed() < 10) {
        // Serial.println(F("WARNING: No GPS data.  Check wiring."));
        return false;
    }
    return true;
}

bool Air530::sendCmd(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int err = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    if (err < 0) {
        return false;
    }
    uint8_t checksum = buffer[1];
    int i = 2;
    while (buffer[i] != '\0') {
        checksum ^= buffer[i++];
    }
    while (stream->available()) {
        stream->flush();
    }
    stream->write(buffer);
    stream->write("*");
    stream->println(checksum, HEX);
    return true;
}










