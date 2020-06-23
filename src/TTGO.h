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

#include "DBG.h"

#include <SPI.h>

#ifdef LILYGO_WATCH_LVGL
#include <Ticker.h>
#endif

#ifdef LILYGO_WATCH_HAS_SDCARD
#include <FS.h>
#include <SD.h>
#endif

#ifdef LILYGO_WATCH_HAS_TOUCH
#include "drive/fx50xx/FT5206.h"
#endif

#if defined(LILYGO_WATCH_HAS_DISPLAY)   || defined(LILYGO_EINK_TOUCHSCREEN)
#include "TFT_eSPI/TFT_eSPI.h"
#endif

#ifdef LILYGO_WATCH_HAS_BMA423
#include "drive/bma423/bma.h"
#endif

#ifdef LILYGO_WATCH_HAS_NFC
#include "drive/nfc/Adafruit_PN532.h"
#endif

#ifdef LILYGO_WATCH_HAS_BUTTON
#include "drive/button/Button2.h"
#endif

#ifdef LILYGO_WATCH_HAS_PCF8563
#include "drive/rtc/pcf8563.h"
#endif

#ifdef LILYGO_WATCH_HAS_GPS
#include "drive/gps/TinyGPS++.h"
#endif

#ifdef LILYGO_WATCH_HAS_S76_S78G
#include "drive/s7xg/s7xg.h"
#endif

#ifdef LILYGO_WATCH_LVGL
#include "lvgl/lvgl.h"
// #include "./font/font.h"
#endif

#ifdef LILYGO_WATCH_HAS_AXP202
#include "./drive/axp/axp20x.h"
#endif

#include "./drive/i2c/i2c_bus.h"
#include "./drive/tft/bl.h"


class TTGOClass
{
public:

    static TTGOClass *getWatch()
    {
        if (_ttgo == nullptr) {
            _ttgo = new TTGOClass();
        }
        return _ttgo;
    }

    void begin()
    {

        i2c = new I2CBus();

#ifdef LILYGO_WATCH_HAS_PCF8563
        rtc = new PCF8563_Class(*i2c);
#endif  /*LILYGO_WATCH_HAS_PCF8563*/

#ifdef LILYGO_WATCH_HAS_BLACKLIGHT
        bl = new BackLight(TWATCH_TFT_BL);
#endif  /*LILYGO_WATCH_HAS_BLACKLIGHT*/

#ifdef LILYGO_WATCH_HAS_BMA423
        bma = new BMA(*i2c);
#endif  /*LILYGO_WATCH_HAS_BMA423*/

#ifdef LILYGO_WATCH_HAS_BUTTON
        //In the 2020 version, Button IO36 is not used.
        //In order to be compatible with the original sample code, keep it here
        button = new Button2(USER_BUTTON);
#endif  /*LILYGO_WATCH_HAS_BUTTON*/

#ifdef LILYGO_WATCH_HAS_AXP202
        power = new AXP20X_Class();
#endif  /*LILYGO_WATCH_HAS_AXP202*/

        initPower();
        initTFT();
        initTouch();
        initBlacklight();
    }


    void initTFT()
    {
#ifdef LILYGO_WATCH_HAS_DISPLAY

#ifdef LILYGO_WATCH_2020_V1
        //In the 2020V1 version, the ST7789 chip power supply
        //is shared with the backlight, so LDO2 cannot be turned off
        power->setPowerOutPut(AXP202_LDO2, AXP202_ON);
#endif

        tft = new TFT_eSPI();

        tft->init();

#ifdef  LILYGO_WATCH_2020_V1
        // Set default initial orientation
        tft->setRotation(2);
#endif  /*LILYGO_WATCH_2020_V1*/

        tft->fillScreen(TFT_BLACK);
#endif
    }

    void initTouch()
    {
#if defined(LILYGO_WATCH_HAS_TOUCH) && !defined(LILYGO_EINK_TOUCHSCREEN)
        touch = new FT5206_Class();
        Wire1.begin(TP_SDA, TP_SCL);
        if (!touch->begin(Wire1)) {
            DBG("Begin touch FAIL");
        }
#elif defined(LILYGO_EINK_TOUCHSCREEN)
        touch = new FT5206_Class();
        // This is just a sign
        touch->setType(FT5X0X_VENDID);
        if (!touch->begin(axpReadBytes, axpWriteBytes)) {
            DBG("Begin touch FAIL");
        } else {
            DBG("Begin touch PASS");
        }
#endif
    }

    void initPower()
    {
#ifdef LILYGO_WATCH_HAS_AXP202
        int ret = power->begin(axpReadBytes, axpWriteBytes);
        if (ret == AXP_FAIL) {
            DBG("AXP Power begin failed");
        } else {
            //Change the button boot time to 4 seconds
            power->setShutdownTime(AXP_POWER_OFF_TIME_4S);
            // Turn off the charging instructions, there should be no
            power->setChgLEDMode(AXP20X_LED_OFF);
            // Turn off external enable
            power->setPowerOutPut(AXP202_EXTEN, false);
            //axp202 allows maximum charging current of 1800mA, minimum 300mA
            power->setChargeControlCur(300);
        }
#endif /*LILYGO_WATCH_HAS_AXP202*/
    }

    void initBlacklight()
    {
#ifdef LILYGO_WATCH_HAS_BLACKLIGHT
        bl->begin();
#endif /*LILYGO_WATCH_HAS_BLACKLIGHT*/
    }



#ifdef LILYGO_WATCH_HAS_AXP202
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

    void enableLDO3(bool en = true)
    {
        power->setLDO3Mode(1);
        power->setPowerOutPut(AXP202_LDO3, en);
    }
#endif  /*LILYGO_WATCH_HAS_AXP202*/


#ifdef LILYGO_WATCH_HAS_DISPLAY
    void displayOff()
    {
        if (tft) {
            tft->writecommand(0x10);
        }
#ifdef LILYGO_WATCH_HAS_TOUCH
        touch->enterSleepMode();
#endif  /*LILYGO_WATCH_HAS_TOUCH*/
    }

    void displaySleep()
    {
        tft->writecommand(0x10);
#ifdef LILYGO_WATCH_HAS_TOUCH
        touch->enterMonitorMode();
#endif  /*LILYGO_WATCH_HAS_TOUCH*/
    }

    void displayWakeup()
    {
        tft->writecommand(0x11);
    }

#endif  /*LILYGO_WATCH_HAS_DISPLAY*/



#ifdef LILYGO_WATCH_HAS_BLACKLIGHT
    void openBL()
    {
#if  defined(LILYGO_WATCH_2020_V1) && defined(LILYGO_WATCH_HAS_AXP202)
        power->setPowerOutPut(AXP202_LDO2, AXP202_ON);
#endif  /*LILYGO_WATCH_2020_V1*/
        bl->on();
    }
    void closeBL()
    {
#if  defined(LILYGO_WATCH_2020_V1) && defined(LILYGO_WATCH_HAS_AXP202)
        power->setPowerOutPut(AXP202_LDO2, AXP202_OFF);
#endif  /*LILYGO_WATCH_2020_V1*/
        bl->off();
    }

#endif  /*LILYGO_WATCH_HAS_BLACKLIGHT*/



    /******************************************
     *              LVGL
     * ***************************************/
#if  defined(LILYGO_WATCH_LVGL) && defined(LILYGO_WATCH_HAS_DISPLAY)
    bool lvgl_begin()
    {
        lv_init();
        lv_disp_drv_t disp_drv;
        lv_indev_drv_t indev_drv;
        lv_disp_drv_init(&disp_drv);
        static lv_disp_buf_t disp_buf;

#ifdef TWATCH_USE_PSRAM_ALLOC_LVGL
        lv_color_t *buf1 = (lv_color_t *)heap_caps_calloc(LV_HOR_RES_MAX * 100, sizeof(lv_color_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT );
        if (!buf1) {
            DBG("alloc failed\n");
            return false;
        }
#else
        static lv_color_t buf1[LV_HOR_RES_MAX * 100];
#endif  /*TWATCH_USE_PSRAM_ALLOC_LVGL*/

        lv_disp_buf_init(&disp_buf, buf1, NULL, LV_HOR_RES_MAX * 100);
        disp_drv.hor_res = 240;
        disp_drv.ver_res = 240;
        disp_drv.flush_cb = disp_flush;
        /*Set a display buffer*/
        disp_drv.buffer = &disp_buf;
        lv_disp_drv_register(&disp_drv);

#if  defined(LILYGO_WATCH_LVGL) && defined(LILYGO_WATCH_HAS_TOUCH)
        /*Register a touchpad input device*/
        lv_indev_drv_init(&indev_drv);
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = touchpad_read;
        lv_indev_drv_register(&indev_drv);
#endif  /*LILYGO_WATCH_HAS_TOUCH*/

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
        pinMode(RTC_INT, INPUT_PULLUP); //need change to rtc_pin
        attachInterrupt(RTC_INT, rtc_cb, FALLING);
    }
#endif  /*LILYGO_WATCH_HAS_PCF8563*/





#ifdef LILYGO_WATCH_HAS_BLACKLIGHT
    BackLight *bl = nullptr;
#endif

#ifdef LILYGO_WATCH_HAS_AXP202
    AXP20X_Class *power = nullptr;
#endif

#ifdef LILYGO_WATCH_HAS_DISPLAY
    TFT_eSPI *tft = nullptr;
#endif

#ifdef LILYGO_WATCH_HAS_TOUCH
    FT5206_Class *touch = nullptr;
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

#ifdef LILYGO_WATCH_HAS_MOTOR
    Motor *motor = nullptr;
    void motor_begin()
    {
        if (motor == nullptr) {
            motor = new Motor(MOTOR_PIN);
        }
        motor->begin();
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

#ifdef LILYGO_WATCH_HAS_GPS
    TinyGPSPlus *gps = nullptr;
    HardwareSerial *hwSerial = nullptr;
    void enableLDO4()
    {
        power->setLDO4Voltage(AXP202_LDO4_1800MV);
        power->setPowerOutPut(AXP202_LDO4, AXP202_ON);
    }
#endif


#ifdef LILYGO_WATCH_HAS_SDCARD
    SPIClass *sdhander = nullptr;

    bool sdcard_begin()
    {
        if (!sdhander) {
            sdhander = new SPIClass(HSPI);
            sdhander->begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
        }
        if (!SD.begin(SD_CS, *sdhander)) {
            DBG("SD Card Mount Failed");
            return false;
        }
        return true;
    }
#endif  /*LILYGO_WATCH_HAS_SDCARD*/


#ifdef LILYGO_WATCH_HAS_GPS
    void gps_begin()
    {
        if (gps == nullptr) {
            gps = new TinyGPSPlus();
        }
        if (hwSerial == nullptr) {
            hwSerial = new HardwareSerial(1);
        }
        hwSerial->begin(GPS_BANUD_RATE, SERIAL_8N1, GPS_RX, GPS_TX);
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
private:
    TTGOClass()
    {
    };

    ~TTGOClass()
    {
    };
#ifdef LILYGO_WATCH_HAS_AXP202
    static uint8_t axpWriteBytes(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint8_t len)
    {
        _ttgo->writeBytes(devAddress, regAddress, data, len);
        return 0;
    }

    static uint8_t axpReadBytes(uint8_t devAddress, uint8_t regAddress,  uint8_t *data, uint8_t len)
    {
        _ttgo->readBytes(devAddress, regAddress, data, len);
        return 0;
    }
#endif  /*LILYGO_WATCH_HAS_AXP202*/


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


#if defined(LILYGO_WATCH_LVGL) && defined(LILYGO_WATCH_HAS_DISPLAY)
    static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
    static bool touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
#endif  /*LILYGO_WATCH_LVGL , LILYGO_WATCH_HAS_DISPLAY*/



    I2CBus *i2c = nullptr;

    static TTGOClass *_ttgo;

#if  defined(LILYGO_WATCH_LVGL)
    Ticker *tickTicker = nullptr;
#endif  /*LILYGO_WATCH_LVGL*/
};



TTGOClass *TTGOClass::_ttgo = nullptr;


#if defined(LILYGO_WATCH_LVGL) && defined(LILYGO_WATCH_HAS_DISPLAY)

void TTGOClass::disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1) ;
    _ttgo->tft->setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
    _ttgo->tft->pushColors(( uint16_t *)color_p, size, false);
    lv_disp_flush_ready(disp_drv);
}

#if defined(LILYGO_WATCH_LVGL) && defined(LILYGO_WATCH_HAS_TOUCH)
bool TTGOClass::touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static TP_Point p;
    uint8_t rotation = _ttgo->tft->getRotation();
    data->state = _ttgo->touch->touched() ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    if (data->state == LV_INDEV_STATE_PR) {
        p = _ttgo->touch->getPoint(0, rotation);
    }
    /*Set the coordinates (if released use the last pressed coordinates)*/
    data->point.x = p.x;
    data->point.y = p.y;
    return false; /*Return false because no moare to be read*/
}
#endif /*LILYGO_WATCH_LVGL , LILYGO_WATCH_HAS_TOUCH*/

#endif /*LILYGO_WATCH_LVGL , LILYGO_WATCH_HAS_DISPLAY*/



