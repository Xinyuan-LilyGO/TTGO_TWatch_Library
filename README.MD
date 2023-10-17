<h1 align = "center">🌟LilyGO T-Watch🌟</h1>

**English | [中文](docs/details_cn.md)**


<h2 align = "left">⭐ News </h2>

1. **T-Watch-S3** version is [here](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/tree/t-watch-s3)
2. In order to be compatible with multiple versions of T-Watch, all examples include a `config.h` file. For the first use, you need to define the **T-Watch** model you use in the `config.h` file
3. In the `config.h` file, you can also see similar definitions, such as **LILYGO_WATCH_LVGL**, **LILYGO_WATCH_HAS_MOTOR**, this type of definition, it will be responsible for opening the defined module function, all definitions Will be available here [View](./docs/defined_en.md)
4. Most of the examples are only used as hardware function demonstrations. This library only completes some initialization work and some demonstrations. For more advanced gameplay, please see [TTGO.h](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/blob/master/src/TTGO.h), to understand how to initialize, after being familiar with it, you can completely leave this library for more advanced gameplay
- About API, please check the source code
- Example [description](docs/examples_en.md)
- The latest factory firmware is made by [sharandac/My-TTGO-Watch](https://github.com/sharandac/My-TTGO-Watch)

![sharandac/My-TTGO-Watch](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen1.png)
![sharandac/My-TTGO-Watch](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen2.png)
![sharandac/My-TTGO-Watch](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen3.png)
![sharandac/My-TTGO-Watch](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen4.png)
![sharandac/My-TTGO-Watch](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen5.png)
![sharandac/My-TTGO-Watch](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen6.png)
![sharandac/My-TTGO-Watch](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen7.png)
![sharandac/My-TTGO-Watch](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen8.png)
![sharandac/My-TTGO-Watch](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen9.png)
![sharandac/My-TTGO-Watch](https://github.com/sharandac/My-TTGO-Watch/blob/master/images/screen10.png)

- Demonstration effect of T-Watch2020-V3 from lunokjod

![lunokjod/watch](https://github.com/lunokjod/watch/blob/devel/doc/watchface0.png)
![lunokjod/watch](https://github.com/lunokjod/watch/blob/devel/doc/day_watchface0.png)
![lunokjod/watch](https://github.com/lunokjod/watch/blob/devel/doc/playground0.png)
![lunokjod/watch](https://github.com/lunokjod/watch/blob/devel/doc/playground3.png)
![lunokjod/watch](https://github.com/lunokjod/watch/blob/devel/doc/settime.png)
![lunokjod/watch](https://github.com/lunokjod/watch/blob/devel/doc/battery.png)
![lunokjod/watch](https://github.com/lunokjod/watch/blob/devel/doc/ble_upload.png)
![lunokjod/watch](https://github.com/lunokjod/watch/blob/devel/doc/mainmenu_provisioning.png)
![lunokjod/watch](https://github.com/lunokjod/watch/blob/devel/doc/provisioning.png)


<h2 align = "left">🚀 Characteristics</h2>

1. The library already contains all the hardware drivers for `T-Watch`
2. Using **TFT_eSPI** as the display driver, you can directly call **TFT_eSPI** through the construction object.
3. Using **lvgl v7.7.2** as the display graphics framework, the driver method has been implemented, you only need to call lvgl api according to your own needs.
4. For the use of lvgl please refer to **[lvgl docs](https://docs.lvgl.io/master/)**


<h2 align = "left">🔷 Install</h2>

- Install the [Arduino IDE](https://www.arduino.cc/en/Main/Software). Note: Later instructions may not work if you use Arduino via Flatpak.
- Download a zipfile from github using the "Download ZIP" button and install it using the IDE ("Sketch" -> "Include Library" -> "Add .ZIP Library...", OR:
- Clone this git repository into your sketchbook/libraries folder. For more info, see https://www.arduino.cc/en/Guide/Libraries


<h2 align = "left">🔷 Note</h2>

- If you don't have the `TTGO T-Watch` option in your board manager, please update the esp32 board as follows:
  - Using Arduino IDE Boards Manager (preferred)
    + [Instructions for Boards Manager](docs/arduino-ide/boards_manager.md)
  - Using Arduino IDE with the development repository
    + [Instructions for Windows](docs/arduino-ide/windows.md)
    + [Instructions for Mac](docs/arduino-ide/mac.md)
    + [Instructions for Debian/Ubuntu Linux](docs/arduino-ide/debian_ubuntu.md)
    + [Instructions for Fedora](docs/arduino-ide/fedora.md)
    + [Instructions for openSUSE](docs/arduino-ide/opensuse.md)


<h2 align = "left">🔷 FAQ</h2>

- The following code is reported as an error when uploading. Please change the default upload baud rate in ArduinoIDE from '20000' to '921600'.
  ```
  A fatal error occurred: Failed to write to target RAM(result was 01070000)
  ```
- This error may also occur on MacOS if using a poorly compatible USB to serial driver.  The driver at [wch.cn](https://www.wch.cn/downloads/CH34XSER_MAC_ZIP.html) is a better match.  The webpage is in Chinese but the driver is digitally signed for security.

<h2 align = "left">🔶 How to find the sample program</h2>

* T-Watch & LilyPi
- In the Arduino board select `TTGO T-Watch`
- In the Arduino File -> Examples -> `TTGO_TWatch_Library`

 <h2 align = "left">🔶 Precautions</h2>

- T-Watch-2019 & LilyPi: Since uses a special IO as the SD interface, please remove the SD card when downloading the program.


 <h2 align = "left">🔷 Resources </h2>

- [LilyPi Pin mapping and other instructions](docs/lilypi_pinmap.md)
- [TWATCH 2019 Series pin mapping and other instructions](docs/watch_2019.md)
- [TWATCH 2020 V1 Pin mapping and other instructions](docs/watch_2020_v1.md)
- [TWATCH 2020 V2 Pin mapping and other instructions](docs/watch_2020_v2.md)
- [TWATCH 2020 V3 Pin mapping and other instructions](docs/watch_2020_v3.md)



<h2 align = "left">🚩 Version comparison </h2>


<table border="0" align="center">
<tr>
<td align="center">Product</td>
<td align="center"><a href="https://www.aliexpress.com/item/33038999162.html">T-Watch-2019</a></td>
<td align="center"><a href="https://www.aliexpress.com/item/4000971508364.html">T-Watch-2020-V1</a></td>
<td align="center"><a href="https://www.aliexpress.com/item/1005002264354524.html">T-Watch-2020-V2</a></td>
<td align="center"><a href="https://www.aliexpress.com/item/1005002053650442.html">T-Watch-2020-V3</a></td>
<td align="center"><a href="https://www.aliexpress.com/item/1005001824993604.html">T-Block/T-Block-V1</a></td>
<td align="center"><a href="https://www.aliexpress.com/item/1005001447548347.html">LilyPi</a></td>
</tr>


<tr>
<td align="center">Core </td>
<td align="center"><a href="https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_cn.pdf">ESP32-D0WDQ6</a></td>
<td align="center"><a href="https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_cn.pdf">ESP32-D0WDQ6</a></td>
<td align="center"><a href="https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_cn.pdf">ESP32-D0WDQ6</a></td>
<td align="center"><a href="https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_cn.pdf">ESP32-D0WDQ6</a></td>
<td align="center"><a href="https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_cn.pdf">ESP32-D0WDQ6</a></td>
<td align="center"><a href="https://www.espressif.com/sites/default/files/documentation/esp32-wrover-b_datasheet_cn.pdf">ESP32-WROVER-B</a></td>
</tr>

<tr>
<td align="center">PSRAM </td>
<!-- 2019 -->
<td align="center">16MBytes</a></td>
<!-- 2020 V1 -->
<td align="center">16MBytes</a></td>
<!-- 2020 V2 -->
<td align="center">16MBytes</a></td>
<!-- 2020 V3 -->
<td align="center">16MBytes</a></td>
<!-- TBLOCK -->
<td align="center">16MBytes</a></td>
<!-- LILYPI -->
<td align="center">16MBytes</a></td>
</tr>

<tr>
<td align="center">Flash </td>
<!-- 2019 -->
<td align="center">8MBytes</a></td>
<!-- 2020 V1 -->
<td align="center">8MBytes</a></td>
<!-- 2020 V2 -->
<td align="center">4MBytes</a></td>
<!-- 2020 V3 -->
<td align="center">8MBytes</a></td>
<!-- TBLOCK -->
<td align="center">8MBytes</a></td>
<!-- LILYPI -->
<td align="center">8MBytes</a></td>
</tr>


<tr>
<td align="center">PMU </td>
<!-- 2019 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/AXP202">AXP202</a></td>
<!-- 2020 V1 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/AXP202">AXP202</a></td>
<!-- 2020 V2 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/AXP202">AXP202</a></td>
<!-- 2020 V3 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/AXP202">AXP202</a></td>
<!-- TBLOCK -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/AXP202">AXP202</a></td>
<!-- LILYPI -->
<td align="center">❌</a></td>
</tr>

<tr>
<td align="center">IMU </td>
<!-- 2019 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/BMA423">BMA423</a></td>
<!-- 2020 V1 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/BMA423">BMA423</a></td>
<!-- 2020 V2 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/BMA423">BMA423</a></td>
<!-- 2020 V3 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/BMA423">BMA423</a></td>
<!-- TBLOCK -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/MPU6050">MPU6050</a></td>
<!-- LILYPI -->
<td align="center">❌</a></td>
</tr>


<tr>
<td align="center">TFT </td>
<!-- 2019 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/blob/master/DISPLAY/ST7789V.pdf">1.54"/240X240/ST7789V</a></td>
<!-- 2020 V1 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/blob/master/DISPLAY/ST7789V.pdf">1.54"/240X240/ST7789V</a></td>
<!-- 2020 V2 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/blob/master/DISPLAY/ST7789V.pdf">1.54"/240X240/ST7789V</a></td>
<!-- 2020 V3 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/blob/master/DISPLAY/ST7789V.pdf">1.54"/240X240/ST7789V</a></td>
<!-- TBLOCK -->
<td align="center">[1]</a></td>
<!-- LILYPI -->
<td align="center">[1]</a></td>
</tr>

<tr>
<td align="center">TouchScreen </td>
<!-- 2019 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/TOUCHSCREEN">FT6336</a></td>
<!-- 2020 V1 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/TOUCHSCREEN">FT6336</a></td>
<!-- 2020 V2 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/TOUCHSCREEN">FT6336</a></td>
<!-- 2020 V3 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/TOUCHSCREEN">FT6336</a></td>
<!-- TBLOCK -->
<td align="center">[1]</a></td>
<!-- LILYPI -->
<td align="center">[1]</a></td>
</tr>

<tr>
<td align="center">Real Time Clock </td>
<!-- 2019 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/RTC">PCF8563</a></td>
<!-- 2020 V1 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/RTC">PCF8563</a></td>
<!-- 2020 V2 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/RTC">PCF8563</a></td>
<!-- 2020 V3 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/RTC">PCF8563</a></td>
<!-- TBLOCK -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/RTC">PCF8563</a></td>
<!-- 2020 V3 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/RTC">PCF8563</a></td>
</tr>



<tr>
<td align="center">Infrared sensor  </td>
<!-- 2019 -->
<td align="center">❌</a></td>
<!-- 2020 V1 -->
<td align="center">✅</a></td>
<!-- 2020 V2 -->
<td align="center">✅</a></td>
<!-- 2020 V3 -->
<td align="center">✅</a></td>
<!-- TBLOCK -->
<td align="center">❌</a></td>
<!-- LILYPI -->
<td align="center">❌</a></td>
</tr>


<tr>
<td align="center">Scalable </td>
<!-- 2019 -->
<td align="center">✅</a></td>
<!-- 2020 V1 -->
<td align="center">❌</a></td>
<!-- 2020 V2 -->
<td align="center">✅</a></td>
<!-- 2020 V3 -->
<td align="center">❌</a></td>
<!-- TBLOCK -->
<td align="center">✅</a></td>
<!-- LILYPI -->
<td align="center">✅</a></td>
</tr>

<tr>
<td align="center">Microphone </td>
<!-- 2019 -->
<td align="center">[1]</a></td>
<!-- 2020 V1 -->
<td align="center">❌</a></td>
<!-- 2020 V2 -->
<td align="center">❌</a></td>
<!-- 2020 V3 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/MICROPHONE">SPM1423HM4H</a></td>
<!-- TBLOCK -->
<td align="center">[1]</a></td>
<!-- LILYPI -->
<td align="center">❌</a></td>
</tr>

<tr>
<td align="center">GPS </td>
<!-- 2019 -->
<td align="center">[1]</a></td>
<!-- 2020 V1 -->
<td align="center">❌</a></td>
<!-- 2020 V2 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/L76K"> Quectel L76K</a></td>
<!-- 2020 V3 -->
<td align="center">❌</a></td>
<!-- TBLOCK -->
<td align="center">[1]</a></td>
<!-- LILYPI -->
<td align="center">❌</a></td>
</tr>

<tr>
<td align="center">Decoder </td>
<!-- 2019 -->
<td align="center">[1]</a></td>
<!-- 2020 V1 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/MAX98357A">MAX98357A</a></td>
<!-- 2020 V2 -->
<td align="center">❌</a></td>
<!-- 2020 V3 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/MAX98357A">MAX98357A</a></td>
<!-- TBLOCK -->
<td align="center">[1]</a></td>
<!-- LILYPI -->
<td align="center">❌</a></td>
</tr>


<tr>
<td align="center">Tactile  </td>
<!-- 2019 -->
<td align="center">[1]</a></td>
<!-- 2020 V1 -->
<td align="center">✅(IO Control)</a></td>
<!-- 2020 V2 -->
<td align="center"><a href="https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/DRV2605">DRV2605(I2C Control)</a></td>
<!-- 2020 V3 -->
<td align="center">✅(IO Control)</td>
<!-- TBLOCK -->
<td align="center">[1]</a></td>
<!-- LILYPI -->
<td align="center">❌</a></td>
</tr>


<tr>
<td align="center">Button</td>
<!-- 2019 -->
<td align="center">✅</a></td>
<!-- 2020 V1 -->
<td align="center">✅[2]</a></td>
<!-- 2020 V2 -->
<td align="center">✅[2]</a></td>
<!-- 2020 V3 -->
<td align="center">✅[2]</a></td>
<!-- TBLOCK -->
<td align="center">✅[2]</a></td>
<!-- LILYPI -->
<td align="center">✅</a></td>
</tr>

</table>

- [1]:Need expansion board to support 
- [2]:The buttons are AXP202 PEK programmable buttons 

 <h2 align = "left">🔶 More interesting projects</h2>

- [lunokjod/watch](https://github.com/lunokjod/watch)
- [Micropython-twatch2020](https://y0no.fr/posts/micropython-ttgo-twatch2020/)
- [sharandac/My-TTGO-Watch](https://github.com/sharandac/My-TTGO-Watch)
- [lyusupov/Flight Recorder](https://github.com/lyusupov/SoftRF/wiki/Flight-Recorder)
- [lixy123/TTGO_T_Watch_Baidu_Rec](https://github.com/lixy123/TTGO_T_Watch_Baidu_Rec)
- [lixy123/TTGO_T_Watch_Alarm_Clock](https://github.com/lixy123/TTGO_T_Watch_Alarm_Clock)
- [AlexGoodyear/agoodWatch](https://github.com/AlexGoodyear/agoodWatch)
- [Adosis/TTGO_TWatch_WordClock](https://github.com/Adosis/TTGO_TWatch_WordClock)
- [SpectralCascade/FancyWatchOS](https://github.com/SpectralCascade/FancyWatchOS)


