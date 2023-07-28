<h1 align = "center">🌟LilyGO T-Watch🌟</h1>

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
2. Install [Arduino ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/) 
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

1. Unable to upload sample
    1. Make sure that the T-Watch is turned on, you can check it according to the following method, open the computer device manager, check the port, plug the USB port into the computer, and if the new COM device is displayed, it has been turned on, if it is not displayed, press the crown Press the button on the button for one second, and then the device port will pop up, click upload at this time
2. The USB port of the device keeps connecting and disconnecting repeatedly
    1. This is the abnormal operation of the program and the phenomenon caused by the continuous restart of the esp32. First, open the Arduino IDE, select the corresponding port, and then shut down the T-Watch first (keep the USB connected to the computer, press and hold the crown, and then until the USB device is not in the list), then click upload, wait for the sketch compilation to be completed, and press the crown for one second to start the machine. At this time, it should be able to upload normally. This method needs to grasp the time
    2. If the first method cannot grasp the time, you can use the second method, first plug in the USB, shut down the T-Watch, and then remove the USB cable after seeing that there is no device displayed in the USB port, and disassemble the T-Watch On the back cover, there is a button on the PCB, press and hold the button, and then insert the USB cable, at this time T-Watch enters the download mode, click upload, after the upload is complete, you need to shut down the T-Watch, and then turn it on again, the program will be normal run.
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
