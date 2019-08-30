
#ifndef __BOARD_DEF_H
#define __BOARD_DEF_H

#define TFT_WIDTH           240
#define TFT_HEIGHT          240
#define TFT_MISO            (gpio_num_t)-1
#define TFT_MOSI            (gpio_num_t)19
#define TFT_SCLK            (gpio_num_t)18
#define TFT_CS              (gpio_num_t)5
#define TFT_DC              (gpio_num_t)27
#define TFT_RST             (gpio_num_t)-1
#define TFT_BL              (gpio_num_t)12

#define SD_CS               13
#define SD_MISO             2
#define SD_MOSI             15
#define SD_SCLK             14

#define I2C_SDA             23
#define I2C_SCL             32

#define SEN_SDA             21
#define SEN_SCL             22

#define UART_TX             33
#define UART_RX             34

#define UART_BANUD_RATE     9600

#define USER_BUTTON         36

#define TP_INT              38
#define RTC_INT             37
#define AXP202_INT          35
#define BMA423_INT1         39
#define BMA423_INT2         0

#define MOTOR_PIN           33

//!  PDM MIC BOARD
#define MIC_DATA            34
#define MIC_CLOCK           26

//! GAMECONTROL BOARD
#define GAMECONTROL_CONTS   5
#define GAMECONTROL_PINS    {13,14,25,26,15}
#define GAMECONTROL_BUZZER  4

//! MAX98357A + INMP441 DOUBLE I2S BOARD
#define TWATCH_DAC_IIS_BCK       26
#define TWATCH_DAC_IIS_WS        25
#define TWATCH_DAC_IIS_DOUT      33

#define TWATCH_INMP441_IIS_BCK  15
#define TWATCH_INMP441_IIS_WS   13
#define TWATCH_INMP441_IIS_DIN  14

//! PN532 BOARD
#define TWATCH_PN532_IRQ        34
#define TWATCH_PN532_RESET      33

//! M8M BOARD
#define GPS_RX                  34
#define GPS_TX                  33
#define GPS_BANUD_RATE          9600




#endif



