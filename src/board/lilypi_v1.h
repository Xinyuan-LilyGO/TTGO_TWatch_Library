
#pragma once

#define TFT_WIDTH                   240
#define TFT_HEIGHT                  240

#define TWATCH_TFT_MISO            23
#define TWATCH_TFT_MOSI            19
#define TWATCH_TFT_SCLK            18
#define TWATCH_TFT_CS              5
#define TWATCH_TFT_DC              27
#define TWATCH_TFT_RST             -1
#define TWATCH_TFT_BL              12

#define SD_CS               13
#define SD_MISO             2
#define SD_MOSI             15
#define SD_SCLK             14

#define SEN_SDA             21
#define SEN_SCL             22

// #define UART_TX             33
// #define UART_RX             34

#define USER_BUTTON         0

#define TOUCH_INT           39
#define RTC_INT_PIN             36

#define ADC_PIN             35

#define RELAY_PIN           32

#define EXTERN_USB_EN       4

#define HAL_ADC_UNIT        ADC_UNIT_1

#define EINK_BUSY           12
#define EINK_RESET          27
#define EINK_DC             19
#define EINK_SS             5
#define EINK_SPI_MOSI       23
#define EINK_SPI_MISO       -1 //elink no use
#define EINK_SPI_CLK        18