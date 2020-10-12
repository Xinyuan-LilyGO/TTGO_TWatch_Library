
# 1.Pinout table 

|    Peripherals    |      T-Watch2020 V2      |
| :---------------: | :----------------------: |
|       Core        |       ESP32-DOWDQ6       |
|       Flash       |           16MB           |
|       SPRAM       |           8MB            |
|   TOUCH Driver    |          FT6236          |
|    TFT Driver     |          ST7789          |
|    GPS Module     |          Air530          |
|     TFT Size      |       240x240/1.54       |
|      TFT RST      |           N/A            |
|     TFT MISO      |           N/A            |
|     TFT MOSI      |            19            |
|     TFT SCLK      |            18            |
|      TFT DC       |            27            |
|      TFT CS       |            5             |
|   TFT BackLight   |            12            |
|    SENSOR SDA     |            21            |
|    SENSOR SCL     |            22            |
|    FT6236 SDA     |            23            |
|    FT6236 SCL     |            32            |
|   FT6236 RESET    | AXP202 EXTEN Pin control |
| BMA423 Interrupt  |            39            |
| FT6236 Interrupt  |            38            |
| PCF8563 Interrupt |            37            |
| AXP202 Interrupt  |            35            |
|      GPS TX       |            26            |
|      GPS RX       |            25            |
|    GPS WAKEUP     |            33            |
|      IR Send      |            4             |
|      SD MISO      |            2             |
|      SD MOSI      |            15            |
|      SD SCLK      |            14            |
|       SD CS       |            13            |


## 2. AXP202 Power domain

|   CHANNEL    |    Explanation     |
| :----------: | :----------------: |
| AXP202  DC2  |       NO USE       |
| AXP202  DC3  | ESP32(Can't close) |
| AXP202  LDO1 |   Can't control    |
| AXP202  LDO2 |     Backlight      |
| AXP202  LDO3 |     TFT/TOUCHT     |
| AXP202  LDO4 |     GPS Module     |
| AXP202 EXTEN |   TOUCH RESET EN   |

## 3.datasheet
[Esp32](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
[Air530](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/GPS-Air530)
[AXP202](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/AXP202)
[BMA423](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/BMA423)
[ST7789](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/blob/master/DISPLAY/ST7789V.pdf)
[PCF8563](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/RTC)
[FT6236](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/blob/master/TOUCHSCREEN/FT6236-FT6336-FT6436L-FT6436_Datasheet.pdf)





