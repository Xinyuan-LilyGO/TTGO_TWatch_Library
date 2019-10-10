
[English](../README.MD) | 中文

该代码仓将演示T-Watch的基础功能，包含T-Watch所有板载芯片,该项目使用最新lvgl6.0作为显示图形框架

# 特性:
1. 该库已经包含`T-Watch`所有硬件驱动
1. 使用TFT_eSPI 作为显示屏驱动，可直接通过通过构造对象调用TFT_eSPI的方法
1. 使用lvgl 6.0作为显示图形框架,驱动方法已经实现，只需要按自己的需求调用lvgl api即可
1. 关于lvgl的使用请参考 **[lvgl 文档](https://docs.littlevgl.com/en/html/index.html)**
  

# 安装
- 在github中点击"Download ZIP",下载完成后,在Arduino 菜单栏中选择 项目 -> 加载库 -> 添加.ZIP库 
关于更多详细说明请参考 https://www.arduino.cc/en/Guide/Libraries

# 如何找到示例程序
- 在Arduino IDE 菜单中选择 工具 -> 开发板 ->  `TTGO T-Watch`
- 在Arduino IDE 菜单中选择 文件 -> 示例 -> TTGO T-Watch
  
# Note
如果你的板子管理器中没有`TTGO T-Watch`选项,请参照如下方法对esp32板子进行更新
  - Using Arduino IDE Boards Manager (preferred)
    + [从板子管理器中安装(推荐)](docs/arduino-ide/boards_manager.md)
  - Using Arduino IDE with the development repository
    + [Windows](docs/arduino-ide/windows.md)
    + [Mac](docs/arduino-ide/mac.md)
    + [Debian/Ubuntu Linux](docs/arduino-ide/debian_ubuntu.md)
    + [Fedora](docs/arduino-ide/fedora.md)
    + [openSUSE](docs/arduino-ide/opensuse.md)
  
# 注意事项:
- 由于Twatch使用了特殊IO作为SD接口，下载程序时候请移除SD卡

# 架构图
![](../images/pins.png)


# 主板引脚映射
**TFT**

| ESP32 Core | GPIO5 | GPIO19 | GPIO18 | GPIO27 |
| :--------: | :---: | :----: | :----: | :----: |
|  ST7789V   |  CS   |  MOSI  |  SCLK  |   DC   |

**Button**

| ESP32 Core  | GPIO36 |
| :---------: | :----: |
| User Button | Button |

**Sensor**

| ESP32 Core | GPIO21 | GPIO22 |  GPIO39   |
| :--------: | :----: | :----: | :-------: |
|   BMA423   |  SDA   |  SCL   | Interrupt |

**PMU**

| ESP32 Core | GPIO21 | GPIO22 |  GPIO35   |
| :--------: | :----: | :----: | :-------: |
|   AXP202   |  SDA   |  SCL   | Interrupt |

**RTC**

| ESP32 Core | GPIO21 | GPIO22 |  GPIO37   |
| :--------: | :----: | :----: | :-------: |
|  PCF8563   |  SDA   |  SCL   | Interrupt |

**TOUCH**

| ESP32 Core | GPIO23 | GPIO32 |  GPIO38   |
| :--------: | :----: | :----: | :-------: |
|  FT6236U   |  SDA   |  SCL   | Interrupt |

**TF Card**

| ESP32 Core | GPIO13 | GPIO15 | GPIO2 | GPIO14 |
| :--------: | :----: | :----: | :---: | :----: |
|  TF Card   |   CS   |  MOSI  | MISO  |  SCLK  |

# 底板引脚映射

**Standard**

| ESP32 Core | GPIO33 | GPIO25  | GPIO21 | GPIO22 |
| :--------: | :----: | :-----: | :----: | :----: |
|  Standard  | Motor  | Speaker |  SDA   |  SCL   |
* 板载SD卡槽

**SIM800L**

| ESP32 Core | GPIO14 | GPIO15 |   GPIO4    | GPIO33 | GPIO34 |
| :--------: | :----: | :----: | :--------: | :----: | :----: |
|  SIM800L   |  RST   | PWKEY  | BOOST_CTRL |   TX   |   RX   |
* 无板载SD卡槽

**MPR121**

| ESP32 Core | GPIO21 | GPIO22 |
| :--------: | :----: | :----: |
|   MPR121   |  SDA   |  SCL   |
* 板载SD卡槽
  
**S7XG_Lora & GPS**

| ESP32 Core | GPIO33 | GPIO34 |
| :--------: | :----: | :----: |
| S7XG_Lora  |   TX   |   RX   |
* 板载SD卡槽