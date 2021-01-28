
# 1.Pinout table 

|    Peripherals    | T-Watch2020 V3 |
| :---------------: | :------------: |
|       Core        |  ESP32-DOWDQ6  |
|       Flash       |      16MB      |
|       SPRAM       |      8MB       |
|   TOUCH Driver    |     FT6336     |
|    TFT Driver     |     ST7789     |
|     TFT Size      |  240x240/1.54  |
|      TFT RST      |      N/A       |
|     TFT MISO      |      N/A       |
|     TFT MOSI      |       19       |
|     TFT SCLK      |       18       |
|      TFT DC       |       27       |
|      TFT CS       |       5        |
|   TFT BackLight   |       15       |
|    SENSOR SDA     |       21       |
|    SENSOR SCL     |       22       |
|    FT6336 SDA     |       23       |
|    FT6336 SCL     |       32       |
|    FT6336 RST     |       14       |
|       MOTOR       |       4        |
| BMA423 Interrupt  |       39       |
| FT6336 Interrupt  |       38       |
| PCF8563 Interrupt |       37       |
| AXP202 Interrupt  |       35       |
|      I2S BCK      |       26       |
|      I2S WS       |       25       |
|     I2S DOUT      |       33       |
|     PDM DATA      |       2        |
|      PDM CLK      |       0        |
|      IR Send      |       13       |


## 2. AXP202 Power domain

|   CHANNEL    |    Explanation     |
| :----------: | :----------------: |
| AXP202  DC2  |       NO USE       |
| AXP202  DC3  | ESP32(Can't close) |
| AXP202  LDO1 |   Can't control    |
| AXP202  LDO2 |     Backlight      |
| AXP202  LDO3 |       NO USE       |
| AXP202  LDO4 |    Audio Module    |
| AXP202 EXTEN |       NO USE       |


## 3.datasheet
- [Esp32](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [MAX98357A(Audio)](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/MAX98357A)
- [AXP202](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/AXP202)
- [BMA423](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/BMA423)
- [ST7789](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/blob/master/DISPLAY/ST7789V.pdf)
- [PCF8563](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/RTC)
- [FT6336](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/TOUCHSCREEN)
- [SPM1423HM4H-B](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/MICROPHONE)

