<h1 align = "center">🌟LilyGO T-Watch🌟</h1>


[![PlatformIO CI](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/actions/workflows/platformio.yml/badge.svg)](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/actions/workflows/platformio.yml)
[![Arduino_CI](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/actions/workflows/arduino_ci.yml/badge.svg?branch=t-watch-s3)](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/actions/workflows/arduino_ci.yml)

**English | [中文](README_CN.MD)**

## News:
- Currently `T-Watch-Lib` is only compatible with `T-Watch S3` version, esp32 version is planned to support

## 1️⃣ PlatformIO Quick Start <Recommended>

1. Install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/)
2. Search for the `PlatformIO` plugin in the `VisualStudioCode` extension and install it.
3. After the installation is complete, you need to restart `VisualStudioCode`
4. After restarting `VisualStudioCode`, select `File` in the upper left corner of `VisualStudioCode` -> `Open Folder` -> select the `TTGO_TWatch_Library` directory
5. Wait for the installation of third-party dependent libraries to complete
6. Click on the `platformio.ini` file, and in the `platformio` column, cancel the sample line that needs to be used, please make sure that only one line is valid
7. Click the (✔) symbol in the lower left corner to compile
8. Connect the board to the computer USB
9. Click (→) to upload firmware
10. Click (plug symbol) to monitor serial output


## 2️⃣ Arduino IDE Quick Start

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Install `Arduino ESP32 2.0.9` ,`Tools` -> `Board` -> `Boards Manager`
    ![InstallArduino](./images/InstallArduino.jpg)
    * **Please use 2.0.9. The new version has changed too much and has not yet been adapted.**
3. Install [TTGO_TWatch_Library](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library)
   1. Download a zipfile from github using the "Download ZIP" button and install it using the IDE ("Sketch" -> "Include Library" -> "Add .ZIP Library...")
   2. Clone this git repository into your sketchbook/libraries folder. For more info, see https://www.arduino.cc/en/Guide/Libraries
4. Install [T-Watch-Deps](https://github.com/Xinyuan-LilyGO/T-Watch-Deps)
    - Copy all directories in [T-Watch-Deps](https://github.com/Xinyuan-LilyGO/T-Watch-Deps) to `<C:\Users\UserName\Documents\Arduino\libraries>` , if there is no `libraries` directory, please create it.
    - Please note that instead of copying the `T-Watch-Deps` directory, copy the folders in the `T-Watch-Deps` directory to <libraries>
    - Please note that currently only <TFT_eSPI> has been [preconfigured](https://github.com/Xinyuan-LilyGO/T-Watch-Deps/blob/be311130018708903d5ed1e524b73d670a2e18f1/TFT_eSPI/User_Setup_Select.h#L143) , after upgrading <TFT_eSPI>, you need to re-move <extras/Setup212_LilyGo_T_Watch_S3.h> to <TFT_eSPI/User_Setups> directory, and in [TFT_eSPI/User_Setup_Select.h](https://github.com/Xinyuan-LilyGO/T-Watch-Deps/blob/be311130018708903d5ed1e524b73d670a2e18f1/TFT_eSPI/User_Setup_Select.h#L143) add `#include <User_Setups/Setup212_LilyGo_T_Watch_S3.h>`
5. Open ArduinoIDE -> Tools
   - Board -> ESP32S3 Dev Module
   - USB CDC On Boot -> Enable  ## Note that you need to change Enable to Disable when USB is not connected, so USB CDC will not prevent the board from starting
   - CPU Frequency -> 240MHz
   - USB DFU On Boot -> Disable
   - Flash Mode -> QIO 80MHz
   - Flash Size -> 16MB(128Mb)
   - USB Firmware MSC On Boot -> Disable
   - PSRAM -> OPI PSRAM
   - Partition Scheme -> 16M Flash(3MB APP/9.9MB FATFS)
   - USB Mode -> Hardware CDC and JIAG
   - Upload Mode -> UART0/Hardware CDC
   - Upload Speed -> 921600
6. Insert USB into the PC and click Upload <If the upload fails, View the FAQ below>


<h2 align = "left">4️⃣ Example Source </h2>

- **display directory**  examples are all from [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI/tree/master/examples) internal examples
- **lvgl directory** examples are all from [lvgl](https://github.com/lvgl/lvgl/tree/master/examples)  internal examples
- **radio directory** examples are all from [RadioLib](https://github.com/jgromes/RadioLib/tree/master/examples/SX126x) internal examples
- **peripheral directory** examples are all from [XPowersLib](https://github.com/lewisxhe/XPowersLib/tree/master/examples) & [SensorsLib](https://github.com/lewisxhe/SensorsLib/tree/master/examples) internal examples


<h2 align = "left">5️⃣ ESP32 basic examples </h2>

- [BLE Examples](https://github.com/espressif/arduino-esp32/tree/master/libraries/BLE)
- [WiFi Examples](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)
- [SPIFFS Examples](https://github.com/espressif/arduino-esp32/tree/master/libraries/SPIFFS)
- [OTA Examples](https://github.com/espressif/arduino-esp32/tree/master/libraries/ArduinoOTA)
- [FFat Examples](https://github.com/espressif/arduino-esp32/tree/master/libraries/FFat)
- For more examples of esp32 chip functions, please refer to [arduino-esp32-libraries](https://github.com/espressif/arduino-esp32/tree/master/libraries)


<h2 align = "left">6️⃣ FAQ </h2>

1. Unable to upload to watch
    1. Make sure that the T-Watch is turned on, you can check it according to the following method, open the computer device manager, check the port, plug the USB port into the computer, and if the new COM device is displayed, it has been turned on, if it is not displayed, press the crown Press the button on the button for one second, and then the device port will pop up, click upload at this time
2. The USB port keeps flashing in the computer
* This is a phenomenon caused by the abnormal operation of the program, or the selection of the wrong configuration, and the continuous restart of the esp32. At this time, the problem of not being able to upload can only be solved by manually entering the download mode of the watch
Please follow the steps below
   1. Remove the back cover
   2. Insert Micro-USB
   3. Open Windows Device Manager
   4. Press and hold the crown of the watch until the USB device does not appear in the Windows COM port
   5. Press the button in the picture below and keep pressing
    ![](./images/BUTTON.jpg)
   6. Press the crown button for one second
   7. Now the COM port is fixed
   8. Select Port in Arduino IDE
   9. Click Upload
3. Where to query the pin definition?
    1. Look [here](./src/utilities.h)
4. The screen is not displayed after uploading the sketch?
    1. Please check the fourth line of [Arduino IDE Quick Start]()
5. Power Domain 
    | Power Domain | Role                          |
    | ------------ | ----------------------------- |
    | ALDO1        | RTC backup battery (3.1-3.3v) |
    | ALDO2        | Backlight                     |
    | ALDO3        | 3V3 for FT6336 and st7889     |
    | ALDO4        | SX1262                        |
    | BLDO2        | DRV2605 Enable pin            |
    | DC1          | ESP32 3V3                     |
    | VRTC         | Nothing                       |

6. Battery 
   1. Lilygo T-Watch fits 502530 size (5x25x30mm) batteries of any chemistry supported by the AXP2101.
7. **esp_vad.h: No such file or directory**
   * **Please use 2.0.9. The new version has changed too much and has not yet been adapted.**

