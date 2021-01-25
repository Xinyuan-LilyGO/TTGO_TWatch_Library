
# 1.Pinout table 

|    Peripherals    |  2019/TOUCH  | 2019/NO TOUCH |   T BLOCK    |
| :---------------: | :----------: | :-----------: | :----------: |
|       Core        | ESP32-DOWDQ6 | ESP32-DOWDQ6  | ESP32-DOWDQ6 |
|       Flash       |     16MB     |     16MB      |     16MB     |
|       SPRAM       |     8MB      |      8MB      |     8MB      |
|   TOUCH Driver    |    FT6236    |      N/A      |     N/A      |
|    TFT Driver     |    ST7789    |    ST7789     |     N/A      |
|     TFT Size      | 240x240/1.54 | 240x240/1.54  |     N/A      |
|      TFT RST      |     N/A      |      N/A      |     N/A      |
|     TFT MISO      |     N/A      |      N/A      |     N/A      |
|     TFT MOSI      |      19      |      19       |     N/A      |
|     TFT SCLK      |      18      |      18       |     N/A      |
|      TFT DC       |      27      |      27       |     N/A      |
|      TFT CS       |      5       |       5       |     N/A      |
|   TFT BackLight   |      12      |      12       |     N/A      |
|   SENSOR SDA[3]   |      21      |      21       |      21      |
|   SENSOR SCL[3]   |      22      |      22       |      22      |
|    FT6236 SDA     |      23      |      N/A      |     N/A      |
|    FT6236 SCL     |      32      |      N/A      |     N/A      |
|       MOTOR       |    N/A[1]    |    N/A[1]     |    N/A[1]    |
| BMA423 Interrupt  |      39      |      N/A      |     N/A      |
| FT6236 Interrupt  |      38      |     38[2]     |     N/A      |
| PCF8563 Interrupt |      37      |      37       |      37      |
|      BUTTON       |      36      |      36       |      36      |
| AXP202 Interrupt  |      35      |      35       |      35      |
|      I2S BCK      |    N/A[1]    |    N/A[1]     |    N/A[1]    |
|      I2S WS       |    N/A[1]    |    N/A[1]     |    N/A[1]    |
|     I2S DOUT      |    N/A[1]    |    N/A[1]     |    N/A[1]    |
|      IR Send      |     N/A      |      N/A      |     N/A      |
|      SD MISO      |     N/A      |      N/A      |     N/A      |
|      SD MOSI      |     N/A      |      N/A      |     N/A      |
|      SD SCLK      |     N/A      |      N/A      |     N/A      |
|       SD CS       |     N/A      |      N/A      |     N/A      |


[1]. According to different base plates

[2]. Capacitive touch button

[3]. SENSOR SDA and SCL share PCF8563, BMA423, AXP202,T-Block without BMA423, use MPU6050


## 2. AXP202 Power domain

| channel |      Explanation       |
| :-----: | :--------------------: |
|   DC2   |         NO USE         |
|   DC3   |   ESP32(Can't close)   |
|  LDO1   |     Can't control      |
|  LDO2   |       Backlight        |
|  LDO3   | Backplane power supply |
|  LDO4   | S76/78G Backplane only |


## 3. Bottom plate Pinout

**Standard**

| ESP32 Core | GPIO33 | GPIO25  | GPIO21 | GPIO22 |
| :--------: | :----: | :-----: | :----: | :----: |
|  Standard  | Motor  | Speaker |  SDA   |  SCL   |
* Onboard SD card slot
* The motor/speaker power domain is LDO3

**SIM800L**

| ESP32 Core | GPIO14 | GPIO15 |   GPIO4    | GPIO33 | GPIO34 |
| :--------: | :----: | :----: | :--------: | :----: | :----: |
|  SIM800L   |  RST   | PWRKEY | BOOST_CTRL |   TX   |   RX   |
* No onboard SD card slot
* The modem power domain is LDO3

**SIM868**

| ESP32 Core | GPIO33 | GPIO34 |
| :--------: | :----: | :----: |
|   SIM868   |   TX   |   RX   |
* Onboard SD card slot
* The gps power domain is LDO3

**MPR121**

| ESP32 Core | GPIO21 | GPIO22 |
| :--------: | :----: | :----: |
|   MPR121   |  SDA   |  SCL   |
* Onboard SD card slot
* The sensor power domain is LDO3

**S7XG_Lora & GPS**

| ESP32 Core | GPIO33 | GPIO34 |
| :--------: | :----: | :----: |
| S7XG_Lora  |   TX   |   RX   |
* Onboard SD card slot
* The S76/78G power domain is LDO3
* The gps power domain is LDO3

## 3.datasheet
- [Esp32](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [AXP202](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/AXP202)
- [BMA423](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/BMA423)
- [ST7789](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/blob/master/DISPLAY/ST7789V.pdf)
- [PCF8563](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/RTC)
- [FT6236](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/blob/master/TOUCHSCREEN/FT6236-FT6336-FT6436L-FT6436_Datasheet.pdf)
- [MPU6050](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/MPU6050)
- [ACSIP_S7678G](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/ACSIP_S7678G)
- [NFC/PN532](https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/NFC)
- [3D Shell](https://github.com/Xinyuan-LilyGO/LilyGo-TWatch-DataSheet/tree/master/3DSHELL)

## 4.other
- [Acsip S76/78 Library&Examples](https://github.com/lewisxhe/Acsip-S7xG-Library)


