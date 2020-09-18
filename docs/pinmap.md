
The following only applies to T-Watch, please [click here to view LilyPi](lilypi_pinmap.md)

# 1.Pinout table 

|    Peripherals    |  2019/TOUCH  | 2019/NO TOUCH |   T BLOCK    | T-Watch2020 V1 |
| :---------------: | :----------: | :-----------: | :----------: | :------------: |
|       Core        | ESP32-DOWDQ6 | ESP32-DOWDQ6  | ESP32-DOWDQ6 |  ESP32-DOWDQ6  |
|       Flash       |     16MB     |     16MB      |     16MB     |      16MB      |
|       SPRAM       |     8MB      |      8MB      |     8MB      |      8MB       |
|   TOUCH Driver    |    FT6236    |      N/A      |     N/A      |     FT6236     |
|    TFT Driver     |    ST7789    |    ST7789     |     N/A      |     ST7789     |
|     TFT Size      | 240x240/1.54 | 240x240/1.54  |     N/A      |  240x240/1.54  |
|      TFT RST      |     N/A      |      N/A      |     N/A      |      N/A       |
|     TFT MISO      |     N/A      |      N/A      |     N/A      |      N/A       |
|     TFT MOSI      |      19      |      19       |     N/A      |       19       |
|     TFT SCLK      |      18      |      18       |     N/A      |       18       |
|      TFT DC       |      27      |      27       |     N/A      |       27       |
|      TFT CS       |      5       |       5       |     N/A      |       5        |
|   TFT BackLight   |      12      |      12       |     N/A      |       12       |
|   SENSOR SDA[3]   |      21      |      21       |      21      |       21       |
|   SENSOR SCL[3]   |      22      |      22       |      22      |       22       |
|    FT6236 SDA     |      23      |      N/A      |     N/A      |       23       |
|    FT6236 SCL     |      32      |      N/A      |     N/A      |       32       |
|       MOTOR       |    N/A[1]    |    N/A[1]     |    N/A[1]    |       4        |
| BMA423 Interrupt  |      39      |      N/A      |     N/A      |       39       |
| FT6236 Interrupt  |      38      |     38[2]     |     N/A      |       38       |
| PCF8563 Interrupt |      37      |      37       |      37      |       37       |
|      BUTTON       |      36      |      36       |      36      |      N/A       |
| AXP192 Interrupt  |      35      |      35       |      35      |       35       |
|      I2S BCK      |    N/A[1]    |    N/A[1]     |    N/A[1]    |       26       |
|      I2S WS       |    N/A[1]    |    N/A[1]     |    N/A[1]    |       25       |
|     I2S DOUT      |    N/A[1]    |    N/A[1]     |    N/A[1]    |       33       |
|      IR Send      |     N/A      |      N/A      |     N/A      |       13       |
|      SD MISO      |     N/A      |      N/A      |     N/A      |      N/A       |
|      SD MOSI      |     N/A      |      N/A      |     N/A      |      N/A       |
|      SD SCLK      |     N/A      |      N/A      |     N/A      |      N/A       |
|       SD CS       |     N/A      |      N/A      |     N/A      |      N/A       |


[1]. According to different base plates

[2]. Capacitive touch button

[3]. SENSOR SDA and SCL share PCF8563, BMA423, AXP192,T-Block without BMA423, use MPU6050

[4]. T-Watch2020 V1, Audio audio power domain is controlled by AXP202 LDO3


## 3. AXP202 power management channel

| channel |      Explanation       |
| :-----: | :--------------------: |
|   DC2   |         NO USE         |
|   DC3   |   ESP32(Can't close)   |
|  LDO1   |     Can't control      |
|  LDO2   |       Backlight        |
|  LDO3   | Backplane power supply |
|  LDO4   | S76/78G Backplane only |


- In T-Watch 2020, LDO3 is the audio power domain, and the remaining power channels are not applicable

## 2. Bottom plate Pinout(The following does not apply to T-Watch2020)

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
