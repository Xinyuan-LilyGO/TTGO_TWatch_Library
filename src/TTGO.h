/*

  _       _   _            ____
 | |     (_) | |  _   _   / ___|   ___
 | |     | | | | | | | | | |  _   / _ \
 | |___  | | | | | |_| | | |_| | | (_) |
 |_____| |_| |_|  \__, |  \____|  \___/
                  |___/

website:https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library
Written by Lewis he //https://github.com/lewisxhe
*/

#pragma once

#include <SPI.h>

#if defined(LILYGO_WATCH_HAS_S76_S78G)
#define LILYGO_WATCH_HAS_SDCARD
#endif

#ifdef LILYGO_WATCH_LVGL
#include <Ticker.h>
#endif

#ifdef LILYGO_WATCH_HAS_SDCARD
#include <FS.h>
#include <SD.h>
#endif

#ifndef LILYGO_BLOCK_ILI9481_MODULE
#define LILYGO_TOUCH_DRIVER_FTXXX
#endif

#ifdef LILYGO_WATCH_HAS_TOUCH
#include "drive/fx50xx/focaltech.h"
#if defined(LILYGO_TOUCH_DRIVER_GTXXX)
typedef GT9xx_Class CapacitiveTouch ;
#elif defined(LILYGO_TOUCH_DRIVER_FTXXX)
typedef FocalTech_Class CapacitiveTouch ;
#endif
#endif  /*LILYGO_WATCH_HAS_TOUCH*/

#if !defined(EXTERNAL_TFT_ESPI_LIBRARY)
#if defined(LILYGO_WATCH_HAS_DISPLAY)   || defined(LILYGO_EINK_TOUCHSCREEN) || defined(LILYGO_WATCH_HAS_EINK)
#include "libraries/TFT_eSPI/TFT_eSPI.h"
#include "libraries/U8g2_for_Adafruit_GFX/src/U8g2_for_Adafruit_GFX.h"
#endif
#else   //EXTERNAL_TFT_ESPI_LIBRARY
#include <TFT_eSPI.h>
#endif  /*EXTERNAL_TFT_ESPI_LIBRARY*/

#ifdef LILYGO_WATCH_HAS_BMA423
#include "drive/bma423/bma.h"
#endif

#ifdef LILYGO_WATCH_HAS_NFC
#include "libraries/Adafruit-PN532/Adafruit_PN532.h"
#endif

#ifdef LILYGO_WATCH_HAS_BUTTON
#include "drive/button/Button2.h"
#endif

#ifdef LILYGO_WATCH_HAS_PCF8563
#include "drive/rtc/pcf8563.h"
#endif

#if defined(LILYGO_WATCH_AIR530_GPS)
#include "drive/gps/Air530.h"
#elif defined(LILYGO_WATCH_HAS_GPS)
#include "drive/gps/TinyGPS++.h"
#endif


#ifdef LILYGO_WATCH_HAS_S76_S78G
#include "drive/s7xg/s7xg.h"
#endif

#ifdef LILYGO_WATCH_LVGL
#include "lvgl/lvgl.h"
#endif

#ifdef LILYGO_WATCH_LVGL_FS
#include "libraries/lv_fs_if/lv_fs_if.h"
#endif

#ifdef LILYGO_WATCH_LVGL_DECODER
#include "libraries/lv_lib_png/lv_png.h"
#endif

#ifdef LILYGO_WATCH_HAS_AXP202
#include "drive/axp/axp20x.h"
#endif

#ifdef LILYGO_WATCH_HAS_MPU6050
#include "drive/mup6050/MPU6050.h"
#endif

#ifdef LILYGO_WATCH_HAS_ADC
#include "esp_adc_cal.h"
#endif  /*LILYGO_WATCH_HAS_ADC*/

#include "drive/i2c/i2c_bus.h"
#include "drive/tft/bl.h"

#ifdef LILYGO_EINK_GDEW0371W7
#define GDEW0371W7_WIDTH    416
#define GDEW0371W7_HEIGHT   240
#include "libraries/ePaperDriverLib/src/ePaperDriver.h"
#endif

#if defined(LILYGO_EINK_GDEP015OC1)
#include "libraries/GxEPD/src/GxEPD.h"
#include "libraries/GxEPD/src/GxIO/GxIO.h"
#include "libraries/GxEPD/src/GxIO/GxIO_SPI/GxIO_SPI.h"
#include "libraries/GxEPD/src/GxGDEP015OC1/GxGDEP015OC1.h"
#elif defined(LILYGO_EINK_DEPG0150BN)
#include "libraries/GxEPD/src/GxEPD.h"
#include "libraries/GxEPD/src/GxIO/GxIO.h"
#include "libraries/GxEPD/src/GxIO/GxIO_SPI/GxIO_SPI.h"
#include "libraries/GxEPD/src/GxDEPG0150BN/GxDEPG0150BN.h"
#elif  defined(LILYGO_EINK_GDEH0154D67_TP) || defined(LILYGO_EINK_GDEH0154D67_BL)
#include "libraries/GxEPD/src/GxEPD.h"
#include "libraries/GxEPD/src/GxIO/GxIO.h"
#include "libraries/GxEPD/src/GxIO/GxIO_SPI/GxIO_SPI.h"
#include "libraries/GxEPD/src/GxGDEH0154D67/GxGDEH0154D67.h"
#endif

#if defined(LILYGO_WATCH_DRV2605)
#include "libraries/Adafruit_DRV2605_Library/Adafruit_DRV2605.h"
#endif


#if !defined(EXTERNAL_TFT_ESPI_LIBRARY) && !defined(LILYGO_BLOCK_ILI9488_MODULE) && !defined(TWATCH_USE_PSRAM_ALLOC_LVGL)
// #define ENABLE_LVGL_FLUSH_DMA       //Use DMA for transmission by default
#endif



#ifndef LVGL_BUFFER_SIZE
#if defined(LILYGO_BLOCK_ST7796S_MODULE)  || defined(LILYGO_BLOCK_ILI9488_MODULE) || defined(LILYGO_BLOCK_ILI9481_MODULE)
#define LVGL_BUFFER_SIZE        (320*100)
#else
#define LVGL_BUFFER_SIZE        (240*100)
#endif
#endif  /*LVGL_BUFFER_SIZE*/

#define TOUCH_IRQ_BIT           (_BV(1))

/* Selectively disable some initialisation */
#define NO_HARDWARE     (_BV(0))
#define NO_POWER        (_BV(1))
#define NO_TFT          (_BV(2))
#define NO_TOUCH        (_BV(3))
#define NO_SENSOR       (_BV(4))
#define NO_BACKLIGHT    (_BV(5))



#if defined(LILYGO_WATCH_2020_V1) || defined(LILYGO_WATCH_2019_WITH_TOUCH) || defined(LILYGO_WATCH_2019_NO_TOUCH)
#define WATCH_SCREEN_TOP_EDGE           0
#define WATCH_SCREEN_LEFT_EDGE          1
#define WATCH_SCREEN_BOTTOM_EDGE        2
#define WATCH_SCREEN_RIGHT_EDGE         3
#elif defined(LILYGO_WATCH_2020_V2)
#define WATCH_SCREEN_TOP_EDGE           1
#define WATCH_SCREEN_LEFT_EDGE          2
#define WATCH_SCREEN_BOTTOM_EDGE        3
#define WATCH_SCREEN_RIGHT_EDGE         0
#elif defined(LILYGO_WATCH_2020_V3)
#define WATCH_SCREEN_TOP_EDGE           3
#define WATCH_SCREEN_LEFT_EDGE          0
#define WATCH_SCREEN_BOTTOM_EDGE        1
#define WATCH_SCREEN_RIGHT_EDGE         2
#endif



class TTGOClass
{
public:
    static TTGOClass *getWatch()
    {
        if (_ttgo == nullptr) {
            _ttgo = new TTGOClass();
            _tpEvent = xEventGroupCreate();
        }
        return _ttgo;
    }

#if defined(EXTERNAL_TFT_ESPI_LIBRARY)
    void setTftExternal(TFT_eSPI &handler )
    {
        tft = &handler;
    }
#endif

    void begin( uint8_t disable = 0 )
    {
        i2c = new I2CBus();

#ifdef LILYGO_WATCH_HAS_PCF8563
        rtc = new PCF8563_Class(*i2c);
#endif  /*LILYGO_WATCH_HAS_PCF8563*/

#ifdef LILYGO_WATCH_HAS_BACKLIGHT
        bl = new BackLight(TWATCH_TFT_BL);
#endif  /*LILYGO_WATCH_HAS_BACKLIGHT*/

#ifdef LILYGO_WATCH_HAS_BMA423
        bma = new BMA(*i2c);
#endif  /*LILYGO_WATCH_HAS_BMA423*/

#ifdef LILYGO_WATCH_HAS_BUTTON
        //In the 2020 version, Button IO36 is not used.
        button = new Button2(USER_BUTTON);
#endif  /*LILYGO_WATCH_HAS_BUTTON*/

#ifdef LILYGO_WATCH_HAS_AXP202
        power = new AXP20X_Class();
#endif  /*LILYGO_WATCH_HAS_AXP202*/

#ifdef LILYGO_WATCH_HAS_MPU6050
        mpu = new MPU6050();
#endif  /*LILYGO_WATCH_HAS_MPU6050*/

#ifdef LILYGO_WATCH_DRV2605
        drv = new Adafruit_DRV2605();
#endif  /*LILYGO_WATCH_DRV2605*/

        if (!(disable & NO_HARDWARE))
            initHardware();

        if (!(disable & NO_HARDWARE))
            initPower();

        if (!(disable & NO_HARDWARE))
            initTFT();

        if (!(disable & NO_HARDWARE))
            initTouch();

        if (!(disable & NO_HARDWARE))
            initSensor();

        if (!(disable & NO_HARDWARE))
            initBlacklight();
    }

#ifdef LILYGO_WATCH_HAS_BMA423
    void bma423AttachInterrupt(void (*handle)(void))
    {
        pinMode(BMA423_INT1, INPUT_PULLUP);
        attachInterrupt(BMA423_INT1, handle, RISING);
    }
#endif  /*LILYGO_WATCH_HAS_BMA423*/

    /******************************************
     *              TouchPad
     * ***************************************/
#ifdef LILYGO_WATCH_HAS_TOUCH
    bool touched()
    {
#if     defined(LILYGO_TOUCH_DRIVER_GTXXX)
        return (touch->scanPoint() > 0);
#elif   defined(LILYGO_TOUCH_DRIVER_FTXXX)
        return (touch->getTouched() > 0);
#endif
    }

    /**
     * @brief  Get the touch screen coordinates,
     *  return true if pressed, false otherwise
     */

    bool getTouch(int16_t &x, int16_t &y)
    {
        uint16_t __x = 0, __y = 0;
        if (touch == nullptr) {
            return false;
        }

#if     defined(LILYGO_TOUCH_DRIVER_GTXXX)
        if (!touch->scanPoint()) {
            return false;
        }
        touch->getPoint(__x, __y, 0);

#elif   defined(LILYGO_TOUCH_DRIVER_FTXXX)
        if (!touch->getPoint(__x, __y)) {
            return false;
        }
#endif  /*LILYGO_TOUCH_DRIVER_GTXXX LILYGO_TOUCH_DRIVER_FTXXX*/


#if     defined(LILYGO_BLOCK_ILI9481_MODULE)
        uint8_t rotation = tft->getRotation();
        switch (rotation) {
        case 1:
            x = __y;
            y = tft->height() - __x;
            break;
        case 2:
            x = tft->width() - __x;
            y = tft->height() - __y;
            break;
        case 3:
            x = tft->width() - __y;
            y = __x;
            break;
        case 0:
        default:
            x = __x;
            y = __y;
        }
#elif   defined(LILYGO_BLOCK_ST7796S_MODULE)
        uint8_t rotation = tft->getRotation();
        switch (rotation) {
        case 1:
            x = __y;
            y = tft->height() - __x;
            break;
        case 2:
            x = tft->width() - __x;
            y = tft->height() - __y;
            break;
        case 3:
            x = tft->width() - __y;
            y = __x;
            break;
        case 0:
        default:
            x = __x;
            y = __y;
        }
#elif   defined(LILYGO_WATCH_2019_WITH_TOUCH)
        uint8_t rotation = tft->getRotation();
        int16_t _x = map(__x, 0, 320, 0, 240);
        int16_t _y = map(__y, 0, 320, 0, 240);
        switch (rotation) {
        case 1:
            x = _y;
            y = TFT_HEIGHT - _x;
            break;
        case 2:
            x = TFT_WIDTH - _x;
            y = TFT_HEIGHT - _y;
            break;
        case 3:
            x = TFT_WIDTH - _y;
            y = _x;
            break;
        case 0:
        default:
            x = _x;
            y = _y;
        }
#elif   defined(LILYGO_WATCH_2020_V1) || defined(LILYGO_WATCH_2020_V2) || defined(LILYGO_WATCH_2020_V3)

#ifndef LILYGO_WATCH_2020_PANEL_V1
        uint8_t rotation = tft->getRotation();
        switch (rotation) {
        case 0:
            x = TFT_WIDTH - __x;
            y = TFT_HEIGHT - __y;
            break;
        case 1:
            x = TFT_WIDTH - __y;
            y = __x;
            break;
        case 3:
            x = __y;
            y = TFT_HEIGHT - __x;
            break;
        case 2:
        default:
            x = __x;
            y = __y;
        }
#else   /*LILYGO_WATCH_2020_PANEL_V1*/
        uint8_t rotation = tft->getRotation();
        switch (rotation) {
        case 2:
            x = TFT_WIDTH - __x;
            y = TFT_HEIGHT - __y;
            break;
        case 1:
            x =  __y;
            y = TFT_HEIGHT - __x;
            break;
        case 3:
            x = TFT_HEIGHT - __y;
            y = __x;
            break;
        case 0:
        default:
            x = __x;
            y = __y;
        }
#endif  /*LILYGO_WATCH_2020_PANEL_V1*/

#elif   defined(LILYGO_EINK_GDEW0371W7)
        uint8_t r = ePaper->getRotation();
        switch (r) {
        case 0:
            x = __x;
            y = __y;
            break;
        case 1:
            x =  __y;
            y = GDEW0371W7_HEIGHT - __x;
            break;
        case 2:
            x = GDEW0371W7_HEIGHT - __x;
            y = GDEW0371W7_WIDTH - __y;
            break;
        case 3:
            x = GDEW0371W7_WIDTH - __y;
            y = __x;
            break;
        default:
            x = __x;
            y = __y;
        }
#else
        x = __x;
        y = __y;
#endif
        return true;
    }

    /*@brief  Cannot wake up after entering sleep mode,
    * can only be solved by restarting the power supply
    */
    void touchToSleep()
    {
#if     defined(LILYGO_TOUCH_DRIVER_GTXXX)
        //TODO:
#elif   defined(LILYGO_TOUCH_DRIVER_FTXXX)
        touch->setPowerMode(FOCALTECH_PMODE_DEEPSLEEP);
#endif
    }

    /*
    * @brief Enter the monitoring mode can reduce the touch current,
    * when it monitors the touch, it will switch to the normal mode under the monitoring mode
    */
    void touchToMonitor()
    {
#if     defined(LILYGO_TOUCH_DRIVER_GTXXX)
        //TODO:
#elif   defined(LILYGO_TOUCH_DRIVER_FTXXX)
        touch->setPowerMode(FOCALTECH_PMODE_MONITOR);
#endif
    }

#if defined(LILYGO_WATCH_2020_V2) || defined(LILYGO_WATCH_2020_V3)
    /*
    In 2020V2, touch can be set to sleep state, control AXP202_EXTEN can wake up touch
    */
    void touchWakup()
    {
#ifdef LILYGO_WATCH_2020_V2
        power->setPowerOutPut(AXP202_EXTEN, false);
        delay(8);   //Trst Min = 5ms
        power->setPowerOutPut(AXP202_EXTEN, true);
#endif
#ifdef LILYGO_WATCH_2020_V3
        digitalWrite(TOUCH_RST, LOW);
        delay(8);
        digitalWrite(TOUCH_RST, HIGH);
#endif
    }
#endif


#endif  /*LILYGO_WATCH_HAS_TOUCH*/



    /******************************************
     *              Wire1
     * ***************************************/


#if defined(LILYGO_WATCH_2019_WITH_TOUCH) ||  defined(LILYGO_WATCH_2019_NO_TOUCH)  || defined(LILYGO_WATCH_BLOCK)

    I2CBus *wire;

    //Use the second set of I2C interfaces
    void beginWire(uint8_t sda = 25, uint8_t scl = 26)
    {
        wire = new I2CBus(Wire1, sda, scl);
    }

    TwoWire &getWire()
    {
        return (*wire->getHandler());
    }

#endif  /*TWI1*/


    /******************************************
     *              Power
     * ***************************************/
#ifdef LILYGO_WATCH_HAS_AXP202

    void enableAudio(void)
    {
#if     defined(LILYGO_WATCH_2020_V2)
        power->setPowerOutPut(AXP202_LDO3, AXP202_ON);
#elif   defined(LILYGO_WATCH_2020_V3)
        power->setPowerOutPut(AXP202_LDO4, AXP202_ON);
#endif
    }

    void disableAudio(void)
    {
#if     defined(LILYGO_WATCH_2020_V2)
        power->setPowerOutPut(AXP202_LDO3, AXP202_OFF);
#elif   defined(LILYGO_WATCH_2020_V3)
        power->setPowerOutPut(AXP202_LDO4, AXP202_OFF);
#endif
    }

#ifdef LILYGO_WATCH_DRV2605
    void enableDrv2650(bool en = true)
    {
#ifdef LILYGO_WATCH_2020_V2
        power->setGPIOMode(AXP_GPIO_0, en ? AXP_IO_OUTPUT_HIGH_MODE : AXP_IO_OUTPUT_LOW_MODE);
#else
        power->setPowerOutPut(AXP202_EXTEN, en);
#endif
    }
#endif


    void powerAttachInterrupt(void (*handle)(void))
    {
        pinMode(AXP202_INT, INPUT_PULLUP);
        attachInterrupt(AXP202_INT, handle, FALLING);
    }


    void trunOnGPS()
    {
#ifdef LILYGO_WATCH_2020_V2
        // 2020 v2 use axp202 ldo 4
        if (power)
            power->setPowerOutPut(AXP202_LDO4, true);
#else
        if (power)
            power->setPowerOutPut(AXP202_LDO3, true);
#endif
    }

    void turnOffGPS()
    {
#ifdef LILYGO_WATCH_2020_V2
        // 2020 v2 use axp202 ldo 4
        if (power)
            power->setPowerOutPut(AXP202_LDO4, false);
#else
        if (power)
            power->setPowerOutPut(AXP202_LDO3, false);
#endif
    }


    /*
    * @brief  It will turn off the power supply of all peripherals except ESP32
    * * */
    void powerOff()
    {
#ifndef LILYGO_WATCH_2020_V1
        power->setPowerOutPut(AXP202_EXTEN, false);
        power->setPowerOutPut(AXP202_LDO4, false);
        power->setPowerOutPut(AXP202_DCDC2, false);
        power->setPowerOutPut(AXP202_LDO3, false);
        power->setPowerOutPut(AXP202_LDO2, false);
#else
        power->setPowerOutPut(AXP202_EXTEN, false);
        power->setPowerOutPut(AXP202_LDO4, false);
        power->setPowerOutPut(AXP202_DCDC2, false);
        power->setPowerOutPut(AXP202_LDO3, false);
        // power->setPowerOutPut(AXP202_LDO2, false);
#endif  /*LILYGO_WATCH_2020_V1*/
    }

    /**
    * @brief Turn on the peripheral power,
    * usually control the power of different backplanes,
    * In 2020V1, it controls the power of the audio circuit
    */
    void enableLDO3(bool en = true)
    {
        power->setLDO3Mode(1);
        power->setPowerOutPut(AXP202_LDO3, en);
    }


    void shutdown()
    {
        power->shutdown();
    }
#endif  /*LILYGO_WATCH_HAS_AXP202*/

    /******************************************
     *              SIM Series
     * ***************************************/

#if     defined(LILYGO_WATCH_HAS_SIM800L) || defined(LILYGO_WATCH_HAS_SIM868)
#ifdef LILYGO_WATCH_HAS_SIM868
#undef  MOTOR_PIN
#define MOTOR_PIN                   4       //SIM868 MOTOR PIN,JUST ONLY SIM868
#define SIM868_MODEM_RI             26
#define SIM868_MODEM_DTR            25
#define SIM868_MODEM_BAUD           115200  //SIM868 BAUD 
#define SIM868_MODEM_TX             33      //SIM868 TX PIN
#define SIM868_MODEM_RX             34      //SIM868 RX PIN
    void enableModemGPSPower(bool en = true)
    {
        power->setPowerOutPut(AXP202_LDO3, en);
    }
#endif

#ifdef LILYGO_WATCH_HAS_SIM800L
#define SIM800_MODEM_BAUD           115200  //SIM800L BAUD 
#define SIM800_MODEM_TX             33      //SIM800L TX PIN
#define SIM800_MODEM_RX             34      //SIM800L RX PIN
#define SIM800_MODEM_RST            14      //SIM800L RESET PIN,JUST ONLY SIM800L
#define SIM800_MODEM_PWRKEY         15      //SIM800L PWRKEY PIN,JUST ONLY SIM800L

    void powerOnModem()
    {
        pinMode(SIM800_MODEM_RST, OUTPUT);
        digitalWrite(SIM800_MODEM_RST, HIGH);

        pinMode(SIM800_MODEM_PWRKEY, OUTPUT);
        digitalWrite(SIM800_MODEM_PWRKEY, HIGH);
        delay(200);
        digitalWrite(SIM800_MODEM_PWRKEY, LOW);
        delay(1000);
        digitalWrite(SIM800_MODEM_PWRKEY, HIGH);
    }

    void powerOffModem()
    {
        // 1.5s<T 1 <33s
        digitalWrite(SIM800_MODEM_PWRKEY, LOW);
        delay(1500);
    }

    void restartModem()
    {
        // PWRKEY pull-down 15s < T1 < 2s
        digitalWrite(SIM800_MODEM_PWRKEY, LOW);
        delay(1800);
        // T3 > 800ms
        digitalWrite(SIM800_MODEM_PWRKEY, HIGH);
        delay(900);
        // pull-down > 1s power on
        digitalWrite(SIM800_MODEM_PWRKEY, LOW);
        delay(1000);
        digitalWrite(SIM800_MODEM_PWRKEY, HIGH);
    }
#endif  /*LILYGO_WATCH_HAS_SIM800L*/

#endif  /*LILYGO_WATCH_HAS_SIM800L || LILYGO_WATCH_HAS_SIM868*/


    /******************************************
     *              DISPLAY
     * ***************************************/
#ifdef LILYGO_WATCH_HAS_DISPLAY
    void displayOff()
    {
        if (tft) {
            tft->writecommand(0x10);
        }
#ifdef LILYGO_WATCH_HAS_TOUCH
        touchToSleep();
#endif  /*LILYGO_WATCH_HAS_TOUCH*/
    }

    void displaySleep()
    {
        tft->writecommand(0x10);
#ifdef LILYGO_WATCH_HAS_TOUCH
        touchToMonitor();
#endif  /*LILYGO_WATCH_HAS_TOUCH*/
    }

    void displayWakeup()
    {
        tft->writecommand(0x11);
    }

#endif  /*LILYGO_WATCH_HAS_DISPLAY*/


    /******************************************
     *              BACKLIGHT
     * ***************************************/
#ifdef LILYGO_WATCH_HAS_BACKLIGHT
    void openBL()
    {
#if  !defined(LILYGO_WATCH_2020_V1) && defined(LILYGO_WATCH_HAS_AXP202)
        power->setPowerOutPut(AXP202_LDO2, AXP202_ON);
#endif  /*LILYGO_WATCH_2020_V1*/
        bl->on();
    }

    void closeBL()
    {
#if  !defined(LILYGO_WATCH_2020_V1) && defined(LILYGO_WATCH_HAS_AXP202)
        power->setPowerOutPut(AXP202_LDO2, AXP202_OFF);
#endif  /*LILYGO_WATCH_2020_V1*/
        bl->off();
    }

    void setBrightness(uint8_t level)
    {
        bl->adjust(level);
    }
#endif  /*LILYGO_WATCH_HAS_BACKLIGHT*/



    /******************************************
     *              LVGL
     * ***************************************/
#if  defined(LILYGO_WATCH_LVGL) && defined(LILYGO_WATCH_HAS_DISPLAY)
    void lvgl_whirling(uint8_t rot)
    {
        tft->setRotation(rot);
        disp_drv.hor_res = tft->width();
        disp_drv.ver_res = tft->height();
        lv_disp_drv_update(lv_disp_get_default(), &disp_drv);
    }


private:
#ifdef  TWATCH_USE_PSRAM_ALLOC_LVGL
    lv_color_t *buf1 = nullptr;
#ifdef  TWATCH_LVGL_DOUBLE_BUFFER
    lv_color_t *buf2 = nullptr;
#endif
#endif

public:
    bool lvgl_begin()
    {
        if (tft == nullptr) {
            return false;
        }
        lv_init();
        lv_indev_drv_t indev_drv;
        lv_disp_drv_init(&disp_drv);
        static lv_disp_buf_t disp_buf;

#ifdef  TWATCH_USE_PSRAM_ALLOC_LVGL
        if (psramFound()) {
            buf1 = (lv_color_t *)ps_calloc(sizeof(lv_color_t), LVGL_BUFFER_SIZE);
        } else {
            buf1 = (lv_color_t *)calloc(sizeof(lv_color_t), LVGL_BUFFER_SIZE);
        }
        if (!buf1) {
            log_e("buf1 alloc failed\n");
            return false;
        }
#ifdef  TWATCH_LVGL_DOUBLE_BUFFER
        if (psramFound()) {
            buf2 = (lv_color_t *)ps_calloc(sizeof(lv_color_t), LVGL_BUFFER_SIZE);
        } else {
            buf2 = (lv_color_t *)calloc(sizeof(lv_color_t), LVGL_BUFFER_SIZE);
        }
        if (!buf2) {
            log_e("buf2 alloc failed\n");
            free(buf1);
            return false;
        }
#endif  /*TWATCH_LVGL_DOUBLE_BUFFER*/
#else
        static lv_color_t buf1[LVGL_BUFFER_SIZE];
#endif  /*TWATCH_LVGL_DOUBLE_BUFFER*/


#ifdef  TWATCH_LVGL_DOUBLE_BUFFER
        lv_disp_buf_init(&disp_buf, buf1, buf2, LVGL_BUFFER_SIZE);
#else
        lv_disp_buf_init(&disp_buf, buf1, NULL, LVGL_BUFFER_SIZE);
#endif


        disp_drv.hor_res = tft->width();
        disp_drv.ver_res = tft->height();
        disp_drv.flush_cb = disp_flush;
        /*Set a display buffer*/
        disp_drv.buffer = &disp_buf;
        lv_disp_drv_register(&disp_drv);

#if  defined(LILYGO_WATCH_HAS_TOUCH)
        /*Register a touchpad input device*/
        lv_indev_drv_init(&indev_drv);
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = touchpad_read;
        lv_indev_drv_register(&indev_drv);
#endif  /*LILYGO_WATCH_HAS_TOUCH*/


#if defined(LILYGO_WATCH_2020_V1) || defined(LILYGO_WATCH_2020_V2) || defined(LILYGO_WATCH_2020_V3)
#define LILYGO_WATCH_LVGL_FS_SPIFFS
#endif

#ifdef LILYGO_WATCH_LVGL_FS
#if  defined(LILYGO_WATCH_LVGL_FS_SPIFFS)
        SPIFFS.begin(true, "/fs");
#else
        //TODO:
#endif  /*LILYGO_WATCH_LVGL_FS_SPIFFS*/

        lv_fs_if_init();

#ifdef LILYGO_WATCH_LVGL_DECODER
        lv_png_init();
#endif

#endif  /*LILYGO_WATCH_LVGL_FS*/

        tickTicker = new Ticker();
        startLvglTick();
        return true;
    }

    void startLvglTick()
    {
        tickTicker->attach_ms(5, []() {
            lv_tick_inc(5);
        });
    }

    void stopLvglTick()
    {
        tickTicker->detach();
    }

#endif  /*LILYGO_WATCH_LVGL*/

    /******************************************
     *              RTC
     * ***************************************/
#ifdef LILYGO_WATCH_HAS_PCF8563
    PCF8563_Class *rtc = nullptr;
    void rtcAttachInterrupt(void (*rtc_cb)(void))
    {
        pinMode(RTC_INT_PIN, INPUT_PULLUP); //need change to rtc_pin
        attachInterrupt(RTC_INT_PIN, rtc_cb, FALLING);
    }
#endif  /*LILYGO_WATCH_HAS_PCF8563*/


#ifdef LILYGO_WATCH_HAS_BACKLIGHT
    BackLight *bl = nullptr;
#endif

#ifdef LILYGO_WATCH_HAS_AXP202
    AXP20X_Class *power = nullptr;
#endif

#if defined(LILYGO_WATCH_HAS_DISPLAY)
    TFT_eSPI *tft = nullptr;
#endif

#ifdef LILYGO_WATCH_HAS_BUZZER
    Buzzer *buzzer = nullptr;
#endif

#ifdef LILYGO_WATCH_HAS_BMA423
    BMA *bma = nullptr;
#endif

#ifdef LILYGO_WATCH_HAS_BUTTON
    Button2 *button = nullptr;
#endif

#ifdef LILYGO_WATCH_HAS_MPU6050
    MPU6050 *mpu = nullptr;
#endif

#if defined(LILYGO_EINK_GDEW0371W7)
    ePaperDisplay *ePaper = nullptr;
#endif

#if defined(LILYGO_EINK_GDEH0154D67_TP) || defined(LILYGO_EINK_GDEH0154D67_BL) || defined(LILYGO_EINK_GDEP015OC1) || defined(LILYGO_EINK_DEPG0150BN)
    GxIO_Class  *io = nullptr;
    GxEPD_Class *ePaper = nullptr;
#endif

#ifdef LILYGO_WATCH_DRV2605
    Adafruit_DRV2605 *drv = nullptr;
#endif

#ifdef LILYGO_WATCH_HAS_MOTOR
    Motor *motor = nullptr;
    void motor_begin()
    {
        if (motor == nullptr) {
            motor = new Motor(MOTOR_PIN);
        }
        motor->begin();
    }

    void shake()
    {
        if (motor) {
            motor->onec();
        }
    }
#endif

#ifdef LILYGO_WATCH_HAS_NFC
    Adafruit_PN532 *nfc = nullptr;
    void nfc_begin()
    {
        if (nfc == nullptr) {
            nfc = new Adafruit_PN532(TWATCH_PN532_IRQ, TWATCH_PN532_RESET);
        }
        nfc->begin();
        nfc->setI2CWrite(nfcWriteBytes);
        nfc->setI2CRead(nfcReadBytes);
    }
#endif



#ifdef LILYGO_WATCH_HAS_SDCARD
    SPIClass *sdhander = nullptr;

    bool sdcard_begin()
    {
        if (!sdhander) {
            sdhander = new SPIClass(HSPI);
            pinMode(SD_MISO, INPUT_PULLUP);
            sdhander->begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
        }
        if (!SD.begin(SD_CS, *sdhander)) {
            log_e("SD Card Mount Failed");
            return false;
        }
        return true;
    }
#endif  /*LILYGO_WATCH_HAS_SDCARD*/

#if defined(LILYGO_WATCH_HAS_GPS) || defined(LILYGO_WATCH_HAS_S76_S78G) || defined(LILYGO_WATCH_AIR530_GPS)
    HardwareSerial *hwSerial = nullptr;
#endif


#if defined(LILYGO_WATCH_AIR530_GPS)
    //retain Air530
    Air530 *gps = nullptr;

    Air530 *gps_begin()
    {
        if (hwSerial == nullptr) {
            hwSerial = new HardwareSerial(1);
            hwSerial->begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX, GPS_TX);
        }
        if (gps == nullptr) {
            gps = new Air530(hwSerial, GPS_WAKE);
        }
        return gps;
    }
#elif defined(LILYGO_WATCH_HAS_GPS)
    TinyGPSPlus *gps = nullptr;
    void gps_begin()
    {
        if (gps == nullptr) {
            gps = new TinyGPSPlus();
        }
        if (hwSerial == nullptr) {
            hwSerial = new HardwareSerial(1);
        }
        hwSerial->begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX, GPS_TX);
    }


    bool gpsHandler()
    {
        if (gps == nullptr) return false;
        while (hwSerial->available()) {
            if (gps->encode(hwSerial->read())) {
                return true;
            }
        }
        return false;
    }
#endif  /*LILYGO_WATCH_HAS_GPS*/

#ifdef LILYGO_WATCH_HAS_S76_S78G
    S7XG_Class *s7xg = nullptr;

    /*
    * Only S76/78 need to use LDO 4, it is responsible for GPS reset power
    * * */
    void enableLDO4()
    {
        power->setLDO4Voltage(AXP202_LDO4_1800MV);
        power->setPowerOutPut(AXP202_LDO4, AXP202_ON);
    }

    void s7xg_begin()
    {
        if (hwSerial == nullptr) {
            hwSerial = new HardwareSerial(1);
        }
        if (s7xg == nullptr) {
            s7xg = new S7XG_Class();
        }
        hwSerial->begin(115200, SERIAL_8N1, GPS_RX, GPS_TX);
        s7xg->begin(*hwSerial);
    }
#endif  /*LILYGO_WATCH_HAS_S76_S78G*/

#ifdef LILYGO_WATCH_HAS_GAMEPAD
    Button2 *gameControl = nullptr;

    void gameControlBegin()
    {
#ifdef LILYGO_WATCH_HAS_DISPLAY
        tft->setRotation(3);
#endif

        buzzer = new Buzzer(GAMECONTROL_BUZZER);
        buzzer->begin();
        uint8_t pins[GAMECONTROL_CONTS] = GAMECONTROL_PINS;
        gameControl = new  Button2[GAMECONTROL_CONTS];
        for (int i = 0; i < GAMECONTROL_CONTS; i++) {
            gameControl[i] = Button2(pins[i]);
        }
    }

    void gameControlBuzzer()
    {
        if (buzzer == nullptr)return;
        buzzer->onec();
    }

    void gameControlHandler()
    {
        for (int i = 0; i < GAMECONTROL_CONTS; i++) {
            gameControl[i].loop();
        }
    }
    bool gameAisPressed()
    {
        return gameControl[0].isPressed();
    }
    bool gameBisPressed()
    {
        return gameControl[2].isPressed();
    }
    bool gameXisPressed()
    {
        return gameControl[1].isPressed();
    }
    bool gameYisPressed()
    {
        return gameControl[3].isPressed();
    }
    bool gameCisPressed()
    {
        return gameControl[4].isPressed();
    }
#endif  /*LILYGO_WATCH_HAS_GAMEPAD*/

    void writeByte(uint8_t devAddress, uint8_t regAddress, uint8_t data)
    {
        if (!i2c)return;
        i2c->writeBytes(devAddress, regAddress, &data, 1);;
    }

    void writeBytes(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint16_t len)
    {
        if (!i2c)return;
        i2c->writeBytes(devAddress, regAddress, data, len);;
    }

    uint8_t readByte(uint8_t devAddress, uint8_t regAddress)
    {
        if (!i2c)return 0;
        uint8_t data;
        i2c->readBytes(devAddress, regAddress, &data, 1);
        return data;
    }

    void readBytes(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint16_t len)
    {
        if (!i2c)return;
        i2c->readBytes(devAddress, regAddress, data, len);
    }

    void readBytes(uint8_t devAddress, uint8_t *data, uint16_t len, uint16_t delay_ms = 0)
    {
        if (!i2c)return;
        i2c->readBytes(devAddress, data, len, delay_ms);
    }

    bool deviceProbe(uint8_t addr)
    {
        if (!i2c)return false;
        return i2c->deviceProbe(addr);
    }

#ifdef LILYGO_WATCH_HAS_ADC
    float getVoltage()
    {
        uint16_t v = analogRead(ADC_PIN);
        return  ((float)v / 4095.0) * 2.0 * 3.3 * (adc_vref / 1000.0);
    }
#endif

#ifdef LILYGO_LILYPI_V1

    void turnOnUSB()
    {
        digitalWrite(EXTERN_USB_EN, HIGH);
    }

    void turnOffUSB()
    {
        digitalWrite(EXTERN_USB_EN, LOW);
    }

    void toggleRelay()
    {
        digitalWrite(RELAY_PIN, !digitalRead(RELAY_PIN));
    }

    void turnOnRelay()
    {
        digitalWrite(RELAY_PIN, HIGH);
    }

    void turnOffRelay()
    {
        digitalWrite(RELAY_PIN, LOW);
    }
#endif


#if (defined(LILYGO_WATCH_2020_V1) || defined(LILYGO_WATCH_2020_V2) || defined(LILYGO_WATCH_2020_V3)|| defined(LILYGO_WATCH_2019_WITH_TOUCH)) &&  defined(LILYGO_WATCH_LVGL)
    void disableTouchIRQ()
    {
        detachInterrupt(TOUCH_INT);
    }

    void enableTouchIRQ()
    {
        /*
            Interrupt polling is only compatible with 2020-V1, 2020-V2 ,2019 , others are not currently adapted
        */
        pinMode(TOUCH_INT, INPUT);
        attachInterrupt(TOUCH_INT, TOUCH_IRQ_HANDLE, FALLING);
    }
#endif  /*LILYGO_WATCH_2020_V1 & LILYGO_WATCH_2020_V2*/

private:
    TTGOClass()
    {
    };

    ~TTGOClass()
    {
    };

    void initHardware(void)
    {
#if defined(LILYGO_LILYPI_V1)
        pinMode(EXTERN_USB_EN, OUTPUT);
        pinMode(RELAY_PIN, OUTPUT);
#endif  /*LILYGO_LILYPI_V1*/

#ifdef LILYGO_WATCH_DRV2605
        drv->begin(i2cReadBytes, i2cWriteBytes);
#endif  /*LILYGO_WATCH_DRV2605*/
    }

    bool initSensor()
    {
#if defined(LILYGO_WATCH_HAS_BMA423)
        struct bma423_axes_remap remap_data;

#ifndef LILYGO_WATCH_HAS_NFC
        // The speed cannot be adjusted to 400K using the NFC module,
        // the speed needs to be adjusted to the default
        i2c->setClock(400000);
#endif

        if (!bma->begin()) {
            log_e("Begin BMA423 FAIL");
            return false;
        }

#if defined(LILYGO_WATCH_2020_V1)
        remap_data.x_axis = 0;
        remap_data.x_axis_sign = 1;
        remap_data.y_axis = 1;
        remap_data.y_axis_sign = 0;
        remap_data.z_axis  = 2;
        remap_data.z_axis_sign  = 1;

#elif defined(LILYGO_WATCH_2020_V2)
        remap_data.x_axis = 1;
        remap_data.x_axis_sign = 0;
        remap_data.y_axis = 0;
        remap_data.y_axis_sign = 0;
        remap_data.z_axis  = 2;
        remap_data.z_axis_sign  = 1;

#elif defined(LILYGO_WATCH_2020_V3)
        remap_data.x_axis = 1;
        remap_data.x_axis_sign = 1;
        remap_data.y_axis = 0;
        remap_data.y_axis_sign = 1;
        remap_data.z_axis  = 2;
        remap_data.z_axis_sign  = 1;
#else
        remap_data.x_axis = 0;
        remap_data.x_axis_sign = 1;
        remap_data.y_axis = 1;
        remap_data.y_axis_sign = 1;
        remap_data.z_axis  = 2;
        remap_data.z_axis_sign  = 0;
#endif
        bma->set_remap_axes(&remap_data);

#elif defined(LILYGO_WATCH_HAS_MPU6050)
        if (!mpu->begin(i2cReadBytes, i2cWriteBytes,
                        MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
            log_e("Begin MPU6050 FAIL");
            return false;
        }
#endif
        return true;
    }

    void initTFT()
    {
// #if (defined(LILYGO_WATCH_HAS_DISPLAY) && !defined(LILYGO_WATCH_HAS_EINK)) || defined(LILYGO_BLOCK_ILI9488_MODULE) || defined(LILYGO_BLOCK_ST7796S_MODULE)
#if defined(LILYGO_WATCH_HAS_DISPLAY)
#if defined(EXTERNAL_TFT_ESPI_LIBRARY)
        if (tft == nullptr) {
            log_e("TFT Handler is NULL!!!");
            return;
        }
#else   /* EXTERNAL_TFT_ESPI_LIBRARY */
        int16_t w = 240;
        int16_t h = 240;
        uint32_t drv = 0x7789;
        uint32_t freq = 40000000;
#if     defined(LILYGO_BLOCK_ST7796S_MODULE) && (defined(LILYGO_WATCH_BLOCK) || defined(LILYGO_LILYPI_V1))
        w = 320;
        h = 480;
        drv = 0x7796;
        freq = 27000000;
#elif   defined(LILYGO_BLOCK_ILI9488_MODULE) && (defined(LILYGO_WATCH_BLOCK) || defined(LILYGO_LILYPI_V1))
        w = 320;
        h = 480;
        drv = 0x9488;
        freq = 27000000;
#elif   defined(LILYGO_BLOCK_ILI9481_MODULE) && (defined(LILYGO_WATCH_BLOCK) || defined(LILYGO_LILYPI_V1))
        w = 320;
        h = 480;
        drv = 0x9481;
        freq = 27000000;
#elif   defined(LILYGO_GC9A01A_MODULE) && defined(LILYGO_WATCH_BLOCK)
        w = 240;
        h = 240;
        drv = 0x9A01;
        freq = 27000000;
        //Soft reset
        pinMode(TWATCH_TFT_RST, OUTPUT);
        digitalWrite(TWATCH_TFT_RST, HIGH);
        delay(5);
        digitalWrite(TWATCH_TFT_RST, LOW);
        delay(20);
        digitalWrite(TWATCH_TFT_RST, HIGH);

#endif  /* (LILYGO_BLOCK_ST7796S_MODULE) && defined(LILYGO_WATCH_BLOCK) */
        tft = new TFT_eSPI(w, h);
        tft->setDriver(drv, freq);
        tft->setPins(TWATCH_TFT_MOSI, TWATCH_TFT_MISO, TWATCH_TFT_SCLK, TWATCH_TFT_CS, TWATCH_TFT_DC);
#endif  /*EXTERNAL_TFT_ESPI_LIBRARY*/


        tft->init();

#if     defined(ENABLE_LVGL_FLUSH_DMA)
        tft->initDMA(); // To use SPI DMA you must call initDMA() to setup the DMA engine
#endif

#if     defined(LILYGO_WATCH_2020_V1) || defined(LILYGO_WATCH_2020_V2) || defined(LILYGO_WATCH_2020_V3)
        // Set default initial orientation
        tft->setRotation(2);
#endif  /*LILYGO_WATCH_2020_V1*/

        tft->fillScreen(TFT_BLACK);

        tft->setTextFont(1);


#elif defined(LILYGO_EINK_GDEW0371W7)
        /* sclk = 18 mosi = 23 miso = n/a */
        SPI.begin(EINK_SPI_CLK, EINK_SPI_MISO, EINK_SPI_MOSI);
        ePaper = new ePaperDisplay( GDEW0371W7, EINK_BUSY, EINK_RESET, EINK_DC, EINK_SS );
#elif defined(LILYGO_EINK_GDEH0154D67_BL) || defined(LILYGO_EINK_GDEH0154D67_TP) || defined(LILYGO_EINK_GDEP015OC1) || defined(LILYGO_EINK_DEPG0150BN)
        log_i("GDEH0154D67 Init...");

        /* sclk = 18 mosi = 23 miso = n/a */
        SPI.begin(EINK_SPI_CLK, EINK_SPI_MISO, EINK_SPI_MOSI);
        io = new GxIO_Class(SPI,  EINK_SS,  EINK_DC, EINK_RESET);
        ePaper = new GxEPD_Class(*io,  EINK_RESET,  EINK_BUSY);
        ePaper->init();
        ePaper->setRotation(0);
        ePaper->fillScreen(GxEPD_WHITE);
        ePaper->setTextColor(GxEPD_BLACK);
        ePaper->setCursor(0, 0);
#endif
    }

    void initTouch()
    {
#if defined(TOUCH_RST)
        pinMode(TOUCH_RST, OUTPUT);
        digitalWrite(TOUCH_RST, LOW);
        delay(8);
        digitalWrite(TOUCH_RST, HIGH);
#endif


#if defined(LILYGO_TOUCHSCREEN_CALLBACK_METHOD)
        touch = new CapacitiveTouch();
#if defined(LILYGO_TOUCH_DRIVER_GTXXX)
        uint8_t address = 0x5D;
        if (i2c->deviceProbe(0x5D)) {
            address = 0x5D;
        } else if (i2c->deviceProbe(0x14)) {
            address = 0x14;
        }
        if (!touch->begin(i2cReadBytes_u16, i2cWriteBytes_u16, address)) {
            log_e("Begin touch FAIL");
        }
#elif defined(LILYGO_TOUCH_DRIVER_FTXXX)
        if (!touch->begin(i2cReadBytes, i2cWriteBytes)) {
            log_e("Begin touch FAIL");
        }
        touch->enableINT();
        /*
        The time period of switching from active mode to monitor mode when there is no touching,
        unit position, the manual indicates that the default value is 0xA,
        and the default value is written here
        */
        touch->setMonitorTime(0x0A);
        /*
        Report rate in monitor mode,
        unit location, default value is 0x28, 40ms?
        */
        touch->setMonitorPeriod(0x28);

#endif  /*LILYGO_TOUCH_DRIVER_XXXXX*/

#elif defined(LILYGO_WATCH_HAS_TOUCH)
        touch = new CapacitiveTouch();
        Wire1.begin(TOUCH_SDA, TOUCH_SCL);
        if (!touch->begin(Wire1)) {
            log_e("Begin touch FAIL");
        }
#endif /*initTouch*/

#if (defined(LILYGO_WATCH_2020_V1) || defined(LILYGO_WATCH_2020_V2) || defined(LILYGO_WATCH_2020_V3)|| defined(LILYGO_WATCH_2019_WITH_TOUCH)) &&  defined(LILYGO_WATCH_LVGL)
        /*
            Interrupt polling is only compatible with 2020-V1, 2020-V2, others are not currently adapted
        */
        pinMode(TOUCH_INT, INPUT);
        attachInterrupt(TOUCH_INT, TOUCH_IRQ_HANDLE, FALLING);
#endif  /*LILYGO_WATCH_2020_V1 & LILYGO_WATCH_2020_V2*/

    }

#if (defined(LILYGO_WATCH_2020_V1) || defined(LILYGO_WATCH_2020_V2) || defined(LILYGO_WATCH_2020_V3)|| defined(LILYGO_WATCH_2019_WITH_TOUCH)) &&  defined(LILYGO_WATCH_LVGL)
    /*
    Interrupt polling is only compatible with 2020-V1, 2020-V2, others are not currently adapted
    */
    static void TOUCH_IRQ_HANDLE(void)
    {
        portBASE_TYPE task_woken;
        if (_ttgo->_tpEvent) {
            xEventGroupSetBitsFromISR(_ttgo->_tpEvent, TOUCH_IRQ_BIT, &task_woken);
            if ( task_woken == pdTRUE ) {
                portYIELD_FROM_ISR();
            }
        }
    }
#endif  /*LILYGO_WATCH_2020_V1 & LILYGO_WATCH_2020_V2*/


    void initPower()
    {
#ifdef LILYGO_WATCH_HAS_ADC
        esp_adc_cal_characteristics_t adc_chars;
        if (esp_adc_cal_characterize(HAL_ADC_UNIT,
                                     ADC_ATTEN_DB_11,
                                     ADC_WIDTH_BIT_12,
                                     1100,
                                     &adc_chars) == ESP_ADC_CAL_VAL_EFUSE_VREF) {
            adc_vref = adc_chars.vref;
        }
#endif  /*LILYGO_WATCH_HAS_ADC*/

#ifdef LILYGO_WATCH_HAS_AXP202
        int ret = power->begin(i2cReadBytes, i2cWriteBytes);
        if (ret == AXP_FAIL) {
            log_e("AXP Power begin failed");
        } else {
            //Change the shutdown time to 4 seconds
            power->setShutdownTime(AXP_POWER_OFF_TIME_4S);
            // Turn off the charging instructions, there should be no
            power->setChgLEDMode(AXP20X_LED_OFF);
            // Turn off external enable
            power->setPowerOutPut(AXP202_EXTEN, false);
            //axp202 allows maximum charging current of 1800mA, minimum 300mA
            power->setChargeControlCur(300);

#ifdef  LILYGO_WATCH_HAS_SIM868
            //Setting gps power
            power->setPowerOutPut(AXP202_LDO3, false);
            power->setLDO3Mode(0);
            power->setLDO3Voltage(3300);
#endif  /*LILYGO_WATCH_HAS_SIM868*/

            power->setLDO2Voltage(3300);

#ifdef  LILYGO_WATCH_2020_V1
            //In the 2020V1 version, the ST7789 chip power supply
            //is shared with the backlight, so LDO2 cannot be turned off
            power->setPowerOutPut(AXP202_LDO2, AXP202_ON);
#endif  /*LILYGO_WATCH_2020_V1*/


#ifdef  LILYGO_WATCH_2020_V2
            // New features of Twatch V2
            power->limitingOff();

            //GPS power domain is AXP202 LDO4
            power->setPowerOutPut(AXP202_LDO4, false);
            power->setLDO4Voltage(AXP202_LDO4_3300MV);

            //Adding a hardware reset can reduce the current consumption of the capacitive touch
            power->setPowerOutPut(AXP202_EXTEN, true);
            delay(10);
            power->setPowerOutPut(AXP202_EXTEN, false);
            delay(8);   //Trst Min = 5ms
            power->setPowerOutPut(AXP202_EXTEN, true);

            /*
            In 2020V2, LDO3 is allocated to the display and touch screen power.
            When ESP32 is set to sleep or other situations where power consumption needs to be saved,
            LDO3 can be turned off
            */
            power->setPowerOutPut(AXP202_LDO3, false);
            power->setLDO3Voltage(3300);
            power->setPowerOutPut(AXP202_LDO3, true);

#endif  /*LILYGO_WATCH_2020_V2*/

#ifdef  LILYGO_WATCH_2020_V3
            // New features of Twatch V3
            power->limitingOff();

            //Audio power domain is AXP202 LDO4
            power->setPowerOutPut(AXP202_LDO4, false);
            power->setLDO4Voltage(AXP202_LDO4_3300MV);
            power->setPowerOutPut(AXP202_LDO4, true);
            // No use
            power->setPowerOutPut(AXP202_LDO3, false);
#endif  /*LILYGO_WATCH_2020_V3*/

        }
#endif /*LILYGO_WATCH_HAS_AXP202*/
    }

    void initBlacklight()
    {
#ifdef LILYGO_WATCH_HAS_BACKLIGHT
        bl->begin();
#endif /*LILYGO_WATCH_HAS_BACKLIGHT*/
    }

    /**
     * @brief  8-bit register interface write function
     */
    static uint8_t i2cWriteBytes(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint8_t len)
    {
        _ttgo->writeBytes(devAddress, regAddress, data, len);
        return true;
    }

    /**
      * @brief  8-bit register interface read function
      */
    static uint8_t i2cReadBytes(uint8_t devAddress, uint8_t regAddress,  uint8_t *data, uint8_t len)
    {
        _ttgo->readBytes(devAddress, regAddress, data, len);
        return true;
    }

    /**
     * @brief  16-bit register interface write function
     */
    static uint8_t i2cWriteBytes_u16(int devAddress, uint16_t regAddress, uint8_t *data, int len)
    {
        configASSERT(_ttgo->i2c);
        return _ttgo->i2c->writeBytes_u16(devAddress, regAddress, data, len);
    }

    /**
     * @brief  16-bit register interface read function
     */
    static uint8_t i2cReadBytes_u16(int devAddress, uint16_t regAddress,  uint8_t *data, int len)
    {
        configASSERT(_ttgo->i2c);
        return _ttgo->i2c->readBytes_u16(devAddress, regAddress, data, len);
    }

#ifdef LILYGO_WATCH_HAS_NFC
    static void nfcWriteBytes(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint16_t len)
    {
        _ttgo->writeBytes(devAddress, regAddress, data, len);
    }

    static void nfcReadBytes(uint8_t devAddress, uint8_t *data, uint16_t len)
    {
        _ttgo->readBytes(devAddress, data, len, 1);
    }
#endif  /*LILYGO_WATCH_HAS_NFC*/

    I2CBus *i2c = nullptr;
    static TTGOClass *_ttgo;
    static EventGroupHandle_t _tpEvent;

#if  defined(LILYGO_WATCH_LVGL)
    Ticker *tickTicker = nullptr;
#endif  /*LILYGO_WATCH_LVGL*/

public: /*Compatible with MY-TTGO-TWATCH https://github.com/sharandac/My-TTGO-Watch*/
#ifdef LILYGO_WATCH_HAS_TOUCH
    CapacitiveTouch *touch = nullptr;
#endif
private:
#if  defined(LILYGO_WATCH_LVGL) && defined(LILYGO_WATCH_HAS_DISPLAY)
    lv_disp_drv_t disp_drv;
#endif

#ifdef LILYGO_WATCH_HAS_ADC
    uint16_t adc_vref = 1100;
#endif

protected:

#if defined(LILYGO_WATCH_LVGL) && defined(LILYGO_WATCH_HAS_DISPLAY)
    static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
    {
        uint32_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1) ;
        // Use DMA for transfer
#if defined(ENABLE_LVGL_FLUSH_DMA)
        _ttgo->tft->startWrite();
        _ttgo->tft->setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
        _ttgo->tft->pushPixelsDMA(( uint16_t *)color_p, size);
        _ttgo->tft->endWrite();
#else
        _ttgo->tft->setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
        _ttgo->tft->pushColors(( uint16_t *)color_p, size, false);
#endif
        lv_disp_flush_ready(disp_drv);
    }

#if defined(LILYGO_WATCH_LVGL) && defined(LILYGO_WATCH_HAS_TOUCH)
    static bool touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
    {

#if (defined(LILYGO_WATCH_2020_V1) || defined(LILYGO_WATCH_2020_V2) || defined(LILYGO_WATCH_2020_V3)|| defined(LILYGO_WATCH_2019_WITH_TOUCH)) &&  defined(LILYGO_WATCH_LVGL)
        /*
            Interrupt polling is only compatible with 2020-V1, 2020-V2,2019, others are not currently adapted
        */
        static int16_t x, y;
        if (xEventGroupGetBits(_ttgo->_tpEvent) & TOUCH_IRQ_BIT) {
            xEventGroupClearBits(_ttgo->_tpEvent, TOUCH_IRQ_BIT);
            data->state = _ttgo->getTouch(x, y) ?  LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
            data->point.x = x;
            data->point.y = y;
        } else {
            data->state = LV_INDEV_STATE_REL;
            data->point.x = x;
            data->point.y = y;
        }
#else
        data->state = _ttgo->getTouch(data->point.x, data->point.y) ?  LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
#endif /*LILYGO_WATCH_2020_V1 & LILYGO_WATCH_2020_V2*/

        return false; /*Return false because no moare to be read*/
    }

#endif /*LILYGO_WATCH_LVGL , LILYGO_WATCH_HAS_TOUCH*/

#endif /*LILYGO_WATCH_LVGL , LILYGO_WATCH_HAS_DISPLAY*/


};




