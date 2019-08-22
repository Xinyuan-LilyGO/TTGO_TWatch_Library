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



class TTGOClass
{
public:

    void begin()
    {
        bl->begin();
        power->begin();
        if (!touch->begin()) {
            Serial.println("Begin touch fail");
        }
        eTFT->init();
    }

    void openBL()
    {
        power->setPowerOutPut(AXP202_LDO2, AXP202_ON);
        bl->on();
    }

    void closeBL()
    {
        power->setPowerOutPut(AXP202_LDO2, AXP202_OFF);
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

    void lvgl_begin()
    {
        lv_init();
        lv_disp_drv_t disp_drv;
        lv_indev_drv_t indev_drv;
        lv_disp_drv_init(&disp_drv);
        static lv_disp_buf_t disp_buf;
        static lv_color_t buf1[LV_HOR_RES_MAX * 10];                        /*A buffer for 10 rows*/
        static lv_color_t buf2[LV_HOR_RES_MAX * 10];                        /*An other buffer for 10 rows*/
        lv_disp_buf_init(&disp_buf, buf1, buf2, LV_HOR_RES_MAX * 10);   /*Initialize the display buffer*/
        disp_drv.hor_res = 240;
        disp_drv.ver_res = 240;
        disp_drv.flush_cb = disp_flush;
        /*Set a display buffer*/
        disp_drv.buffer = &disp_buf;
        lv_disp_drv_register(&disp_drv);
        /*Register a touchpad input device*/
        lv_indev_drv_init(&indev_drv);
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = touchpad_read;
        lv_indev_drv_register(&indev_drv);
        tickTicker = new Ticker();
        tickTicker->attach_ms(5, []() {
            lv_tick_inc(5);
        });
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
        motor = new Motor(MOTOR_PIN);
        motor->begin();
    }

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
    void gameControlBegin()
    {
        eTFT->setRotation(3);
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




private:
    TTGOClass()
    {
        Wire1.begin(I2C_SDA, I2C_SCL);
        i2c = new I2CBus();
        rtc = new PCF8563_Class(*i2c);
        bl = new BackLight(TFT_BL);
        power = new AXP20X_Class(*i2c);
        touch = new FT5206_Class(Wire1);
        bma = new BMA(*i2c);
        button = new Button2(USER_BUTTON);
        eTFT = new TFT_eSPI();
    };

    ~TTGOClass()
    {

    };

    static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
    static bool touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
    SPIClass *sdhander = nullptr;
    I2CBus *i2c = nullptr;
    static TTGOClass *_ttgo;
    Ticker *tickTicker = nullptr;
};

