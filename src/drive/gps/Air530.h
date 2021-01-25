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

Air530.h - Arduino library for Air530 gps module.
Created by Lewis he on December 28, 2020.
github:https://github.com/lewisxhe/Air530_Library
*/
/////////////////////////////////////////////////////////////////


#pragma once

#include <Arduino.h>
#include "TinyGPS++.h"

typedef enum {
    AIR530_NORMAL,                  /*Normal operation mode*/
    AIR530_CYCLE_TRACKING_MODE,     /*Cycle ultra-low power tracking mode, need to pull up WAKE to wake up*/
    AIR530_CYCLE_LP_MODE,           /*Cycle low power mode*/
    AIR530_TRACKING_MODE,           /*Ultra-low power tracking mode, need to pull up WAKE to wake up*/
    AIR530_AUTO_LP_MODE,            /*Automatic low power consumption mode, can wake up via serial port*/
    AIR530_AUTO_TRACKING_MODE,      /*Automatic ultra-low power tracking mode, need to pull up WAKE to wake up*/
} Air530_GPS_Mode;

/**
 * @brief   System restart command
 */
typedef enum {
    AIR530_HOT_START = 1,
    AIR530_WARM_START,
    AIR530_COLD_START,
} Air530_RestartMode;


/**
 * @brief   Enter standby low power mode
 */
typedef enum {
    AIR530_ENTER_STOP = 0,
    AIR530_ENTER_SLEEP,
} Air530_StandbyMode;


/**
 * @brief   1PPS setting arg
 */
typedef enum {
    AIR530_1PPS_OFF,        //Disable PPS output
    AIR530_1PPS_FIRST_FIX,  //First fix
    AIR530_1PPS_3D_FIX,     //3D fix
    AIR530_1PPS_2D3D_FIX,   //2D or 3D fix
    AIR530_1PPS_ALWAYS,     //Always on
} Air530_1PPS_Mode;


class Air530 : public TinyGPSPlus
{
public:
    /**
     * @brief   Air530
     * @param  *ser:        uart stream
     * @param  wakeupPin:   Wake-up pin
     * @retval None
     */
    Air530(Stream *ser, uint8_t wakeupPin = 0U);
    ~Air530();

    /**
     * @brief   restart
     * @note    System restart command
     * @param   mode: See Air530_RestartMode
     * @retval  None
     */
    void restart(Air530_RestartMode mode);


    /**
     * @brief   erase
     * @note    Erase auxiliary positioning data in flash
     * @retval  None
     */
    void erase(void);

    /**
     * @brief   sleep
     * @note    Set Air530 to sleep
     * @retval  None
     */
    void sleep(void);

    /**
     * @brief   stop
     * @note    Set Air530 to stop
     * @retval  None
     */
    void stop(void);

    /**
     * @brief   wakeup
     * @note    wakeup Air530
     * @retval  None
     */
    void wakeup(void);

    /**
     * @brief   setNormalMode
     * @note    Normal operation mode
     * @retval  None
     */
    void setNormalMode(void);

    /**
     * @brief   setCycleTrackingMode
     * @note    Cycle ultra-low power tracking mode, need to pull up WAKE to wake up
     * @param   runMillis: Running time (ms)
     * @param   sleepMillis: Sleep time (ms)
     * @retval  None
     */
    void setCycleTrackingMode(uint32_t runMillis, uint32_t sleepMillis);

    /**
     * @brief   setCycleLowPowerMode
     * @note    Cycle low power mode
     * @param   runMillis: Running time (ms)
     * @param   sleepMillis: Sleep time (ms)
     * @retval  None
     */
    void setCycleLowPowerMode(uint32_t runMillis, uint32_t sleepMillis);

    /**
     * @brief   setTrackingMode
     * @note    Ultra-low power tracking mode, need to pull up WAKE to wake up
     * @retval  None
     */
    void setTrackingMode(void);

    /**
     * @brief   setAutoLowPowerMode
     * @note    Automatic low power consumption mode, can wake up via serial port
     * @retval  None
     */
    void setAutoLowPowerMode(void);

    /**
     * @brief   setAutoTrackingMode
     * @note    Automatic ultra-low power tracking mode, need to pull up WAKE to wake up
     * @retval  None
     */
    void setAutoTrackingMode(void);

    /**
     * @brief   enableQZSS_NMEA
     * @note    Enable QZSS NMEA format output
     * @retval  None
     */
    void enableQZSS_NMEA(void);

    /**
     * @brief   disableQZSS_NMEA
     * @note    Disable QZSS NMEA format output
     * @retval  None
     */
    void disableQZSS_NMEA(void);

    /**
     * @brief   enableQZSS
     * @note    Enable QZSS function
     * @retval  None
     */
    void enableQZSS(void);

    /**
     * @brief   disableQZSS
     * @note    Disable QZSS function
     * @retval  None
     */
    void disableQZSS(void);

    /**
     * @brief  setSearchMode
     * @note   Command: 115 Set search mode
     * @param  gps:     true is GPS on , false is GPS off
     * @param  glonass: true is Glonass on,false is Glonass off
     * @param  beidou:  true is Beidou on,false is Beidou off
     * @param  galieo:  true is Galieo on,false is Galieo off
     * @retval None
     */
    void setSearchMode(bool gps, bool glonass, bool beidou, bool galieo);

    /**
      * @brief  setNMEABaud
      * @note   Set NMEA output baud rate
      * @param  baud: 9600,19200,38400,57600,115200,921600
      * @retval None
      */
    void setNMEABaud(uint32_t baud);

    /**
     * @brief   setBaud
     * @note    Set NMEA serial port parameters
     * @param   baud: 9600,19200,38400,57600,115200,921600
     * @retval  None
     */
    void setBaud(uint32_t baud);

    /**
     * @brief   setPPS
     * @note    PPS settings
     * @param   mode:        see Air530_1PPS_Mode
     * @param   ppsWidth:    PPS pulse width (ms), the requirement is less than 999
     * @param   ppsPeriod:   PPS period (ms), which must be greater than the PPS pulse width
     * @retval
     */
    bool setPPS(Air530_1PPS_Mode mode, uint16_t ppsWidth, uint16_t ppsPeriod = 1000);

    /**
     * @brief   get NMEA Interval
     * @note    NMEA message interval
     * @retval  NMEA Interval(millisecond)
     */
    uint32_t getNMEAInterval(void);


    /**
    * @brief    setInterval
    * @note     Configure the interval for outputting NMEA messages (in ms)
    * @param    ms: millisecond
    * @retval   None
    */
    void setNMEAInterval(uint16_t ms);


    /**
     * @brief   enableSBAS
     * @note    Turn on SBAS function
     * @retval  None
     */
    void enableSBAS(void);

    /**
     * @brief   disableSBAS
     * @note    Turn off SBAS function
     * @retval None
     */
    void disableSBAS(void);

    /**
     * @brief   getSBASEnable
     * @note    Query whether SBAS is enabled
     * @retval  true is enable ,false is disable
     */
    bool getSBASEnable(void);

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
    void setNMEAStatement(bool gll, bool rmc, bool vtg, bool gga, bool gsa, bool gsv, bool grs, bool gst);

    /**
     * @brief   disableNMEAOutput
     * @note    Disable NMEA Output
     * @retval  None
     */
    void disableNMEAOutput(void);

    /**
     * @brief   enableNMEAOutput
     * @note    Enable NMEA Output
     * @retval  None
     */
    void enableNMEAOutput(void);

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
    void setDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);


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
    bool getDateTime(uint16_t &year, uint8_t &month, uint8_t  &day, uint8_t  &hour, uint8_t &min, uint8_t  &sec);

    //Command: 284  Set speed threshold
    //Command: 356  Set HDOP threshold
    //Command: 357  Get HDOP threshold

    /**
     * @brief   getSoftVersion
     * @note    Query the version number of the current software
     * @retval  version
     */
    const char *getSoftVersion(void);


    /**
     * @brief  setProbablyLoaction
     * @note   Set approximate location information and time information to speed up positioning
     * @example:  setProbablyLoaction(28.166450,120.389700,0,2017,3,15,12,0,0)
     * @retval None
     */
    void setProbablyLoaction(float lat, float lng, uint16_t altitude, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);

    /**
      * @brief   process
      * @note    Processing sentences
      * @retval  True is running  ,False is abnormal
      */
    bool process(/*Stream *serial = nullptr*/void);

    /**
     * @brief   sendCmd
     * @note
     * @param  *format:
     * @retval  true success ,false failed
     */
    bool sendCmd(const char *format, ...);

private:
    char buffer[256];
    Stream *stream = nullptr;
    uint8_t wakeup_pin = 0;

};








