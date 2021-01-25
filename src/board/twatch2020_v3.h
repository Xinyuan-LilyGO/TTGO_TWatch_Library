#pragma once

#define TFT_WIDTH                   (240)
#define TFT_HEIGHT                  (240)

#define TWATCH_TFT_MISO             (GPIO_NUM_MAX)
#define TWATCH_TFT_MOSI             (GPIO_NUM_19)
#define TWATCH_TFT_SCLK             (GPIO_NUM_18)
#define TWATCH_TFT_CS               (GPIO_NUM_5)
#define TWATCH_TFT_DC               (GPIO_NUM_27)
#define TWATCH_TFT_RST              (GPIO_NUM_MAX)
#define TWATCH_TFT_BL               (GPIO_NUM_15)

#define TOUCH_SDA                   (GPIO_NUM_23)
#define TOUCH_SCL                   (GPIO_NUM_32)
#define TOUCH_INT                   (GPIO_NUM_38)
#define TOUCH_RST                   (GPIO_NUM_14)

#define SEN_SDA                     (GPIO_NUM_21)
#define SEN_SCL                     (GPIO_NUM_22)

#define RTC_INT_PIN                 (GPIO_NUM_37)
#define AXP202_INT                  (GPIO_NUM_35)
#define BMA423_INT1                 (GPIO_NUM_39)

#define TWATCH_2020_IR_PIN          (GPIO_NUM_13)


#define TWATCH_DAC_IIS_BCK          (GPIO_NUM_26)
#define TWATCH_DAC_IIS_WS           (GPIO_NUM_25)
#define TWATCH_DAC_IIS_DOUT         (GPIO_NUM_33)

#define MOTOR_PIN                   (GPIO_NUM_4)