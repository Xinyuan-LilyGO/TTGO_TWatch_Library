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

#include "./Button2/src/Button2.h"
#include "./drive/axp/axp20x.h"
#include "./drive/fx50xx/FT5206.h"
#include "./drive/i2c/i2c_bus.h"
#include "./drive/rtc/pcf8563.h"
#include "./drive/bma423/bma.h"
#include "./drive/tft/bl.h"
#include "./drive/bma423/bma.h"
#include "./TFT_eSPI/TFT_eSPI.h"
#include "./font/font.h"
#include "board_def.h"
#include <Ticker.h>
#include <SD.h>
#include <FS.h>
#include <SPI.h>
#include "./PN532/Adafruit_PN532.h"
#include "./TinyGPSPlus/TinyGPS++.h"
#include "./S7xG_Library/src/s7xg.h"

class TTGOClass
{
public:
    void begin(bool tft = true, bool touchScreen = true)
    {
        bl->begin();
        int ret = power->begin(axpReadBytes, axpWriteBytes);
        if (ret == AXP_FAIL) {
            Serial.println("AXP Power begin failed");
        }
        if (touchScreen) {
            Wire1.begin(I2C_SDA, I2C_SCL);
            touch = new FT5206_Class(Wire1);
#ifdef LILYGO_TWATCH_2020_V1
            if (!touch->begin(LILYGO_TWATCH_CST026)) {
#else
            if (!touch->begin(LILYGO_TWATCH_FT62XX)) {
#endif
                Serial.println("Begin touch fail");
            }
        }
#ifdef LILYGO_TWATCH_2020_V1
        power->setPowerOutPut(AXP202_LDO2, AXP202_ON);
#endif
        if (tft) {
            eTFT = new TFT_eSPI();
            eTFT->init();
        }
    }

    void powerOff()
    {
#ifndef LILYGO_TWATCH_2020_V1
        power->setPowerOutPut(0xff, false);
#endif
    }

    void displayOff()
    {
        if (eTFT) {
            eTFT->writecommand(0x10);
        }
        if (touch) {
            touch->enterSleepMode();
        }
    }

    void displaySleep()
    {
        if (eTFT) {
            eTFT->writecommand(0x10);
        }
        if (touch) {
            touch->enterMonitorMode();
        }
    }

    void displayWakeup()
    {
        if (eTFT) {
            eTFT->writecommand(0x11);
        }
    }

    void openBL()
    {
#ifndef LILYGO_TWATCH_2020_V1
        power->setPowerOutPut(AXP202_LDO2, AXP202_ON);
#endif
        bl->on();
    }

    void closeBL()
    {
#ifndef LILYGO_TWATCH_2020_V1
        power->setPowerOutPut(AXP202_LDO2, AXP202_OFF);
#endif
        bl->off();
    }

    bool sdcard_begin()
    {
        if (!sdhander) {
            sdhander = new SPIClass(HSPI);
            sdhander->begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
        }
        if (!SD.begin(SD_CS, *sdhander)) {
            Serial.println("\nSD Card Mount Failed");
            return false;
        }
        return true;
    }

    void enableLDO3(bool en = true)
    {
        power->setLDO3Mode(1);
        power->setPowerOutPut(AXP202_LDO3, en);
    }

    bool lvgl_begin()
    {
        if (eTFT) {
            lv_init();
            lv_disp_drv_t disp_drv;
            lv_indev_drv_t indev_drv;
            lv_disp_drv_init(&disp_drv);
            static lv_disp_buf_t disp_buf;
            lv_color_t *buf1 = (lv_color_t *)heap_caps_calloc(LV_HOR_RES_MAX * 10, sizeof(lv_color_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT );
            if (!buf1) {
                Serial.println("alloc failed\n");
                return false;
            }
            lv_disp_buf_init(&disp_buf, buf1, NULL, LV_HOR_RES_MAX * 10);
            disp_drv.hor_res = 240;
            disp_drv.ver_res = 240;
            disp_drv.flush_cb = disp_flush;
            /*Set a display buffer*/
            disp_drv.buffer = &disp_buf;
            lv_disp_drv_register(&disp_drv);

            if (touch) {
                /*Register a touchpad input device*/
                lv_indev_drv_init(&indev_drv);
                indev_drv.type = LV_INDEV_TYPE_POINTER;
                indev_drv.read_cb = touchpad_read;
                lv_indev_drv_register(&indev_drv);
            }
            tickTicker = new Ticker();
            startLvglTick();
        }
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

    void rtcAttachInterrupt(void (*rtc_cb)(void))
    {
        pinMode(RTC_INT, INPUT_PULLUP); //need change to rtc_pin
        attachInterrupt(RTC_INT, rtc_cb, FALLING);
    }

    static TTGOClass *getWatch()
    {
        if (_ttgo == nullptr) {
            _ttgo = new TTGOClass();
        }
        return _ttgo;
    }

    void motor_begin()
    {
        if (motor == nullptr) {
#ifdef LILYGO_TWATCH_2020_V1
            motor = new Motor(TWATCH_2020_MOTOR_PIN);
#else
            motor = new Motor(MOTOR_PIN);
#endif
        }
        motor->begin();
    }

    void nfc_begin()
    {
        if (nfc == nullptr) {
            nfc = new Adafruit_PN532(TWATCH_PN532_IRQ, TWATCH_PN532_RESET);
        }
        nfc->begin();
        nfc->setI2CWrite(nfcWriteBytes);
        nfc->setI2CRead(nfcReadBytes);
    }

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

    S7XG_Class *s7xg = nullptr;
    HardwareSerial *hwSerial = nullptr;
    BackLight *bl = nullptr;
    PCF8563_Class *rtc = nullptr;
    AXP20X_Class *power = nullptr;
    TFT_eSPI *eTFT = nullptr;
    FT5206_Class *touch = nullptr;
    Buzzer *buzzer = nullptr;
    BMA *bma = nullptr;
    Button2 *button = nullptr;
    Button2 *gameControl = nullptr;
    Motor *motor = nullptr;
    Adafruit_PN532 *nfc = nullptr;
    TinyGPSPlus *gps = nullptr;

    void gameControlBegin()
    {
        if (eTFT) {
            eTFT->setRotation(3);
        }
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
        i2c = new I2CBus();
        rtc = new PCF8563_Class(*i2c);
        bl = new BackLight(TWATCH_TFT_BL);
        power = new AXP20X_Class();
        bma = new BMA(*i2c);
        //In the 2020 version, Button IO36 is not used. 
        //In order to be compatible with the original sample code, keep it here
        button = new Button2(USER_BUTTON);
    };

    ~TTGOClass()
    {

    };

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

    static void nfcWriteBytes(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint16_t len)
    {
        _ttgo->writeBytes(devAddress, regAddress, data, len);
    }

    static void nfcReadBytes(uint8_t devAddress, uint8_t *data, uint16_t len)
    {
        _ttgo->readBytes(devAddress, data, len, 1);
    }

    static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
    static bool touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
    SPIClass *sdhander = nullptr;
    I2CBus *i2c = nullptr;
    static TTGOClass *_ttgo;
    Ticker *tickTicker = nullptr;
};
