#pragma once

#define TFT_WIDTH                   (240)
#define TFT_HEIGHT                  (240)

#define TWATCH_TFT_MISO             (GPIO_NUM_MAX)
#define TWATCH_TFT_MOSI             (GPIO_NUM_19)
#define TWATCH_TFT_SCLK             (GPIO_NUM_18)
#define TWATCH_TFT_CS               (GPIO_NUM_5)
#define TWATCH_TFT_DC               (GPIO_NUM_27)
#define TWATCH_TFT_RST              (GPIO_NUM_MAX)
#define TWATCH_TFT_BL               (GPIO_NUM_12)

#define SD_CS                       (GPIO_NUM_13)
#define SD_MISO                     (GPIO_NUM_2)
#define SD_MOSI                     (GPIO_NUM_15)
#define SD_SCLK                     (GPIO_NUM_14)

#define SEN_SDA                     (GPIO_NUM_21)
#define SEN_SCL                     (GPIO_NUM_22)

#define UART_TX                     (GPIO_NUM_33)
#define UART_RX                     (GPIO_NUM_34)

#define USER_BUTTON                 (GPIO_NUM_36)

#define RTC_INT_PIN                 (GPIO_NUM_37)
#define AXP202_INT                  (GPIO_NUM_35)

#define TBLOCK_IMU_INT              (GPIO_NUM_39)

#define EINK_BUSY                   (GPIO_NUM_38)
#define EINK_RESET                  (GPIO_NUM_27)
#define EINK_DC                     (GPIO_NUM_19)
#define EINK_SS                     (GPIO_NUM_5)
#define EINK_SPI_MOSI               (GPIO_NUM_23)
#define EINK_SPI_MISO               -1 //elink no use
#define EINK_SPI_CLK                (GPIO_NUM_18)

#define TWATCH_LORA_SCK             (GPIO_NUM_14)
#define TWATCH_LORA_MISO            (GPIO_NUM_2)
#define TWATCH_LORA_MOSI            (GPIO_NUM_15)
#define TWATCH_LORA_SS              (GPIO_NUM_13)
#define TWATCH_LORA_DI0             (GPIO_NUM_26)
#define TWATCH_LORA_RST             (GPIO_NUM_25)




//!  PDM MIC BOARD
#define MIC_DATA                    34
#define MIC_CLOCK                   26



//! MAX98357A + INMP441 DOUBLE I2S BOARD
#define TWATCH_DAC_IIS_BCK          26
#define TWATCH_DAC_IIS_WS           25
#define TWATCH_DAC_IIS_DOUT         33

#define TWATCH_INMP441_IIS_BCK      15
#define TWATCH_INMP441_IIS_WS       13
#define TWATCH_INMP441_IIS_DIN      14


//! PN532 BOARD
#define TWATCH_PN532_IRQ            34
#define TWATCH_PN532_RESET          33

//! M8M BOARD
#define GPS_RX                      34
#define GPS_TX                      33
#define GPS_BAUD_RATE               9600


