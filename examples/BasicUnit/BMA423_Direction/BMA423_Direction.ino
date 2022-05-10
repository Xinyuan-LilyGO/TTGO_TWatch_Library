/**
 * @file      BMA423_Direction.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2022  深圳市芯元电子科技有限公司
 * @date      2022-05-10
 *
 */

#include "config.h"

TTGOClass *watch;
TFT_eSPI *tft;
BMA *sensor;
uint8_t prevRotation;

void setup()
{
    Serial.begin(115200);

    // Get TTGOClass instance
    watch = TTGOClass::getWatch();

    // Initialize the hardware, the BMA423 sensor has been initialized internally
    watch->begin();

    // Turn on the backlight
    watch->openBL();

    //Receive objects for easy writing
    tft = watch->tft;
    sensor = watch->bma;

    // Accel parameter structure
    Acfg cfg;
    /*!
        Output data rate in Hz, Optional parameters:
            - BMA4_OUTPUT_DATA_RATE_0_78HZ
            - BMA4_OUTPUT_DATA_RATE_1_56HZ
            - BMA4_OUTPUT_DATA_RATE_3_12HZ
            - BMA4_OUTPUT_DATA_RATE_6_25HZ
            - BMA4_OUTPUT_DATA_RATE_12_5HZ
            - BMA4_OUTPUT_DATA_RATE_25HZ
            - BMA4_OUTPUT_DATA_RATE_50HZ
            - BMA4_OUTPUT_DATA_RATE_100HZ
            - BMA4_OUTPUT_DATA_RATE_200HZ
            - BMA4_OUTPUT_DATA_RATE_400HZ
            - BMA4_OUTPUT_DATA_RATE_800HZ
            - BMA4_OUTPUT_DATA_RATE_1600HZ
    */
    cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
    /*!
        G-range, Optional parameters:
            - BMA4_ACCEL_RANGE_2G
            - BMA4_ACCEL_RANGE_4G
            - BMA4_ACCEL_RANGE_8G
            - BMA4_ACCEL_RANGE_16G
    */
    cfg.range = BMA4_ACCEL_RANGE_2G;
    /*!
        Bandwidth parameter, determines filter configuration, Optional parameters:
            - BMA4_ACCEL_OSR4_AVG1
            - BMA4_ACCEL_OSR2_AVG2
            - BMA4_ACCEL_NORMAL_AVG4
            - BMA4_ACCEL_CIC_AVG8
            - BMA4_ACCEL_RES_AVG16
            - BMA4_ACCEL_RES_AVG32
            - BMA4_ACCEL_RES_AVG64
            - BMA4_ACCEL_RES_AVG128
    */
    cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;

    /*! Filter performance mode , Optional parameters:
        - BMA4_CIC_AVG_MODE
        - BMA4_CONTINUOUS_MODE
    */
    cfg.perf_mode = BMA4_CONTINUOUS_MODE;

    // Configure the BMA423 accelerometer
    sensor->accelConfig(cfg);

    // Enable BMA423 accelerometer
    // Warning : Need to use feature, you must first enable the accelerometer
    // Warning : Need to use feature, you must first enable the accelerometer
    // Warning : Need to use feature, you must first enable the accelerometer
    sensor->enableAccel();
}



void loop()
{
    // Obtain the BMA423 direction,
    // so that the screen orientation is consistent with the sensor
    uint8_t rotation = sensor->direction();
    if (prevRotation != rotation) {
        prevRotation = rotation;
        Serial.printf("tft:%u sens:%u ", tft->getRotation(), rotation);
        switch (rotation) {
        case DIRECTION_DISP_DOWN:
            //No use
            break;
        case DIRECTION_DISP_UP:
            //No use
            break;
        case DIRECTION_BOTTOM_EDGE:
            Serial.printf(" set %u\n", WATCH_SCREEN_BOTTOM_EDGE);
            tft->setRotation(WATCH_SCREEN_BOTTOM_EDGE);
            break;
        case DIRECTION_TOP_EDGE:
            Serial.printf(" set %u\n", WATCH_SCREEN_TOP_EDGE);
            tft->setRotation(WATCH_SCREEN_TOP_EDGE);
            break;
        case DIRECTION_RIGHT_EDGE:
            Serial.printf(" set %u\n", WATCH_SCREEN_RIGHT_EDGE);
            tft->setRotation(WATCH_SCREEN_RIGHT_EDGE);
            break;
        case DIRECTION_LEFT_EDGE:
            Serial.printf(" set %u\n", WATCH_SCREEN_LEFT_EDGE);
            tft->setRotation(WATCH_SCREEN_LEFT_EDGE);
            break;
        default:
            break;
        }
        tft->fillScreen(TFT_BLACK);
        tft->drawCentreString("T-Watch", 120, 120, 4);
    }
    delay(500);
}


