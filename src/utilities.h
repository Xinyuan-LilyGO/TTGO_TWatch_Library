#pragma once


#if defined(USING_TWATCH_2020_V1)

#define BOARD_TFT_WIDTH            (240)
#define BOARD_TFT_HEIHT            (240)

#define BOARD_TFT_MISO             (0)
#define BOARD_TFT_MOSI             (19)
#define BOARD_TFT_SCLK             (18)
#define BOARD_TFT_CS               (5)
#define BOARD_TFT_DC               (27)
#define BOARD_TFT_RST              (-1)
#define BOARD_TFT_BL               (12)

#define BOARD_TOUCH_SDA            (23)
#define BOARD_TOUCH_SCL            (32)
#define BOARD_TOUCH_INT            (38)

#define BOARD_I2C_SDA              (21)
#define BOARD_I2C_SCL              (22)

#define BOARD_RTC_INT_PIN          (37)
#define BOARD_PMU_INT              (35)
#define BOARD_BMA423_INT1          (39)

#define MOTOR_PIN                  (4)


#define BOARD_IR_PIN               (13)

#define BOARD_DAC_IIS_BCK          (26)
#define BOARD_DAC_IIS_WS           (25)
#define BOARD_DAC_IIS_DOUT         (33)

#elif defined(USING_TWATCH_2020_V2)

#define BOARD_TFT_WIDTH            (240)
#define BOARD_TFT_HEIHT            (240)

#define BOARD_TFT_MISO             (0)
#define BOARD_TFT_MOSI             (19)
#define BOARD_TFT_SCLK             (18)
#define BOARD_TFT_CS               (5)
#define BOARD_TFT_DC               (27)
#define BOARD_TFT_RST              (-1)
#define BOARD_TFT_BL               (25)

#define BOARD_TOUCH_SDA            (23)
#define BOARD_TOUCH_SCL            (32)
#define BOARD_TOUCH_INT            (38)
// #define BOARD_TOUCH_RST                Use AXP202 EXTEN Pin control

#define BOARD_I2C_SDA              (21)
#define BOARD_I2C_SCL              (22)

#define BOARD_RTC_INT_PIN          (37)
#define BOARD_PMU_INT              (35)
#define BOARD_BMA423_INT1          (39)

#define BOARD_IR_PIN                (2)

//GPS power domain is AXP202 LDO4
#define BOARD_GPS_1PPS              (34)
#define BOARD_GPS_RX                (36)
#define BOARD_GPS_TX                (26)
#define BOARD_GPS_WAKE              (33)
#define BOARD_GPS_BAUDRATE          (9600)

#define BOARD_SD_CS                 (13)
#define BOARD_SD_MISO               (4)
#define BOARD_SD_MOSI               (15)
#define BOARD_SD_SCLK               (14)

#elif defined(USING_TWATCH_2020_V3)

#define BOARD_TFT_WIDTH            (240)
#define BOARD_TFT_HEIHT            (240)

#define BOARD_TFT_MISO             (-1)
#define BOARD_TFT_MOSI             (19)
#define BOARD_TFT_SCLK             (18)
#define BOARD_TFT_CS               (5)
#define BOARD_TFT_DC               (27)
#define BOARD_TFT_RST              (-1)
#define BOARD_TFT_BL               (15)

#define BOARD_TOUCH_SDA            (23)
#define BOARD_TOUCH_SCL            (32)
#define BOARD_TOUCH_INT            (38)
#define BOARD_TOUCH_RST            (14)

#define BOARD_I2C_SDA              (21)
#define BOARD_I2C_SCL              (22)

#define BOARD_RTC_INT_PIN          (37)
#define BOARD_PMU_INT              (35)
#define BOARD_BMA423_INT1          (39)

#define BOARD_IR_PIN               (13)


#define BOARD_DAC_IIS_BCK          (26)
#define BOARD_DAC_IIS_WS           (25)
#define BOARD_DAC_IIS_DOUT         (33)

#define MOTOR_PIN                   (4)

//!  PDM MIC BOARD
#define BOARD_MIC_DATA             (2)
#define BOARD_MIC_CLOCK            (0)

#elif defined(USING_TWATCH_S3)

#define BOARD_TFT_WIDTH             (240)
#define BOARD_TFT_HEIHT             (240)

// ST7789
#define BOARD_TFT_MISO              (-1)
#define BOARD_TFT_MOSI              (13)
#define BOARD_TFT_SCLK              (18)
#define BOARD_TFT_CS                (12)
#define BOARD_TFT_DC                (38)
#define BOARD_TFT_RST               (-1)
#define BOARD_TFT_BL                (45)

// Touch
#define BOARD_TOUCH_SDA             (39)
#define BOARD_TOUCH_SCL             (40)
#define BOARD_TOUCH_INT             (16)

//BMA423,PCF8563,AXP2101,DRV2605L
#define BOARD_I2C_SDA               (10)
#define BOARD_I2C_SCL               (11)

// PCF8563 Interrupt
#define BOARD_RTC_INT_PIN           (17)
// AXP2101 Interrupt
#define BOARD_PMU_INT               (21)
// BMA423 Interrupt
#define BOARD_BMA423_INT1           (14)

// IR Transmission
#define BOARD_IR_PIN                (2)

// MAX98357A
#define BOARD_DAC_IIS_BCK           (48)
#define BOARD_DAC_IIS_WS            (15)
#define BOARD_DAC_IIS_DOUT          (46)

// SX1262 Radio Pins
#define BOARD_RADIO_SCK              (3)
#define BOARD_RADIO_MISO             (4)
#define BOARD_RADIO_MOSI             (1)
#define BOARD_RADIO_SS               (5)
#define BOARD_RADIO_DI01             (9)
#define BOARD_RADIO_RST              (8)
#define BOARD_RADIO_BUSY             (7)
#define BOARD_RADIO_DI03             (6)

// PDM Microphone
#define BOARD_MIC_DATA              (47)
#define BOARD_MIC_CLOCK             (44)

#define SHIELD_GPS_TX               (42)
#define SHIELD_GPS_RX               (41)

#endif