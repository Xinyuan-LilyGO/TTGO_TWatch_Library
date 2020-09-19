#pragma once

#define TFT_WIDTH                   240
#define TFT_HEIGHT                  240

#define TWATCH_TFT_MISO             (gpio_num_t)0
#define TWATCH_TFT_MOSI             (gpio_num_t)19
#define TWATCH_TFT_SCLK             (gpio_num_t)18
#define TWATCH_TFT_CS               (gpio_num_t)5
#define TWATCH_TFT_DC               (gpio_num_t)27
#define TWATCH_TFT_RST              (gpio_num_t)-1
#define TWATCH_TFT_BL               (gpio_num_t)12

#define TOUCH_SDA                   23
#define TOUCH_SCL                   32
#define TOUCH_INT                   38

#define SEN_SDA                     21
#define SEN_SCL                     22

#define RTC_INT_PIN                     37
#define AXP202_INT                  35
#define BMA423_INT1                 39

#define MOTOR_PIN                   4


#define TWATCH_2020_IR_PIN          13

#define TWATCH_DAC_IIS_BCK          26
#define TWATCH_DAC_IIS_WS           25
#define TWATCH_DAC_IIS_DOUT         33

