<h1 align = "center">LilyGO T-Watch</h1>

[English](../README.MD) | 中文


<h2 align = "left">新闻</h2>

1. 为了兼容多种版本的T-Watch，所有示例中都包含一个`config.h`文件,初次使用需要在`config.h`文件中定义你使用的**T-Watch**型号
2. 在`config.h`文件中,你还能看到类似的定义,如**LILYGO_WATCH_LVGL**,  **LILYGO_WATCH_HAS_MOTOR**，这类型的定义，它将负责开启所定义的模块功能,所有定义将在这里可以[查看](./defined_cn.md)
3. 大多数示例只是作为硬件功能演示,这个库只是完成了一些初始化工作和一些演示,更高级的玩法，请查看[TTGO.h](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/blob/master/src/TTGO.h)，了解如何初始化, 熟悉了之后,完全可以脱离本库，进行更高级的玩法
4. 关于API，请查看源码
5. [示例说明](examples_cn.md)
6. 关于出厂固件，由 [sharandac/My-TTGO-Watch](https://github.com/sharandac/My-TTGO-Watch) 编写

![screenshot](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen1.png)
![screenshot](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen2.png)
![screenshot](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen3.png)
![screenshot](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen4.png)
![screenshot](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen5.png)
![screenshot](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen6.png)
![screenshot](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen7.png)
![screenshot](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen8.png)
![screenshot](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen9.png)
![screenshot](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen10.png)


<h2 align = "left">特性</h2>

1. 该库已经包含`T-Watch`所有硬件驱动
2. 使用**TFT_eSPI** 作为显示屏驱动，可直接通过通过构造对象调用**TFT_eSPI**的方法
3. 使用**lvgl v7.3.1**作为显示图形框架,驱动方法已经实现，只需要按自己的需求调用lvgl api即可
4. 关于lvgl的使用请参考 **[lvgl 文档](https://docs.lvgl.io/v7/en/html/)**
  

<h2 align = "left">安装</h2>

- 在github中点击"Download ZIP",下载完成后,在Arduino 菜单栏中选择 项目 -> 加载库 -> 添加.ZIP库 
关于更多详细说明请参考 https://www.arduino.cc/en/Guide/Libraries

<h2 align = "left">如何找到示例程序</h2>

* T-Watch
- 在Arduino IDE 菜单中选择 工具 -> 开发板 ->  `TTGO T-Watch`
- 在Arduino IDE 菜单中选择 文件 -> 示例 -> `TTGO_TWatch_Library`

* LilyPi
- 在Arduino IDE 菜单中选择 工具 -> 开发板 ->  `ESP32 Dev Module`
- 在Arduino IDE 菜单中选择 文件 -> 示例 -> `TTGO_TWatch_Library`

  
<h2 align = "left">安装方法</h2>

如果你的板子管理器中没有`TTGO T-Watch`选项,请参照如下方法对esp32板子进行更新
  - Using Arduino IDE Boards Manager (preferred)
    + [从板子管理器中安装(推荐)](docs/arduino-ide/boards_manager.md)
  - Using Arduino IDE with the development repository
    + [Windows](docs/arduino-ide/windows.md)
    + [Mac](docs/arduino-ide/mac.md)
    + [Debian/Ubuntu Linux](docs/arduino-ide/debian_ubuntu.md)
    + [Fedora](docs/arduino-ide/fedora.md)
    + [openSUSE](docs/arduino-ide/opensuse.md)
  
<h2 align = "left">注意事项</h2>

- T-Watch-2019 : 由于Twatch使用了特殊IO作为SD接口，下载程序时候请移除SD卡
- LilyPi       : 由于LilyPi使用了特殊IO作为SD接口，下载程序时候请移除SD卡

<h2 align = "left">其他</h2>

- [功耗详情点击查看](../docs/power.md)
- [引脚映射点击查看](../docs/pinmap.md)
- [3D外壳](https://github.com/Xinyuan-LilyGO/LilyGo-TWatch-DataSheet/tree/master/3DSHELL)
- [硬件手册](https://github.com/Xinyuan-LilyGO/LilyGo-TWatch-DataSheet)
- [Acsip S76/78 Library&Examples](https://github.com/lewisxhe/Acsip-S7xG-Library)

