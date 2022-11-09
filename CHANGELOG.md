# Changelog
## (11.9.22)
- <examples/TFT_eSPI/IOTA_Price> Update library description and usage examples


## (8.30.22)
- <examples/Shield/S7XG/S7xG_LoRa> Added SDCard test


## (8.19.2022)
- Update wifi event macro name <examples/LVGL/SimpleWatch>


## (7.27.2022)
- Add T-Block Epaper DEPG0150BN driver

## (06.4.2022)
- Fixed increasing the I2C rate causing the backlight to turn on abnormally, only increasing the rate when initializing the sensor

## (05.27.2022)
- Reduce the PWM frequency to 1000HZ


## (05.14.2022)
- Update GxEPD to GxEPD2 , Update e-paper example to fit new screen
- Remove <examples/Sensor/VEMl6075> I2C scan



## (05.10.2022)
- Come back again . Fix BMA423_Direction rotate in wrong direction in V1/V2
- Optimize BMA423 startup time
  * Before optimization: Start  using 2151 millis
  * After optimization :
  * Cold Start: Start  using 987 millis
  * Sleep Wake: Start  using 603 millis


## (10.13.2020)
- Change 2020V1, 2020V2 LVGL touch I2C polling to interrupt polling, others are not currently adapted
- Added TTGOClass mutex example under multitasking <examples\BasicUnit\TouchMultitask>
- Added setting touch default monitoring report rate in touch initialization and enter monitoring mode in active state Description
- Added 2020V2 touch wakeup method, touch can be set to sleep in 2020V2, control AXP202_EXTEN can wake up touch

## (10.19.2020)
- Add a method to turn off touch interrupt and turn on
- Cancel AXP202 USB current limit in 2020 V2 version

## (10.21.2020)
- Added MAX30208 Temperature sensor example

## (10.23.2020)
- Added axp202&bma423 interrupt registered function

## (10.24.2020)
- Added TQuick and TBao example <examples/T_Bao> <examples/T_Quick>

## (11.04.2020)
- Added NES Emulator example <examples/NES> ,Base on https://github.com/moononournation/arduino-nofrendo
- Added ESP32Face example <examples/TFT_eSPI/ESPFace>,Base on https://github.com/luisllamasbinaburo/ESP32_Faces
- Added TFT_eSPI writeIndexedPixelsDouble,writeIndexedPixels

## (11.07.2020)
- Added TBao example <examples/T_Bao2>
- Adapt to 2020 new touch panel

## (11.09.2020)
- Added TBlock example <examples/T-Block/Epaper_Badge>

## (11.11.2020)
- Added TBlock example <examples/T-Block/Epaper>

## (20.11.2020)
- Update TBlock example <examples/T-Block/Epaper_Badge>, Compatible with three ink screens (GDEH0154D67_BL, GDEH0154D67_TP,GDEP015OC1)
- Set AXP202 LDO2's default turn-on voltage to 3.3V
- Update example description
- Added MAX30208 Temperature example <examples/Sensor/MAX30208>
- Added `GC9A01A` display driver , Suitable for T-Block round 240x240 screen
- Added DW1000 examples <examples/Shield/DW1000_BasicSender>,<examples/Shield/DW1000Ranging_ANCHOR>,<examples/Shield/DW1000Ranging_TAG>,<examples/Shield/DW1000_BasicReceiver>,<examples/Shield/DW1000_RangingAnchor>,<examples/Shield/DW1000_RangingTag>

## (1.12.2020)
- Added UnitTest directory, Store hardware test code
- Release TTGO_TWatch_Library V1.4.0 version
- Update lvgl to 7.7.2

## (9.12.2020)
- Turn on Twatch 2019 interrupt function
- Remove warnings caused by upgrading lvgl

## (10.12.2020)
- Added DRV2605 enable for V2

## (14.12.2020)
- Fix the problem that cannot be compiled in SimpleFramework

## (24.12.2020)
- Fix <UnitTest/ScreenRotation> coordinate residue

## (29.12.2020)
- Add Air530 driver for `TWATCH V2`
- Add Air530Allfunction example <BasicUnit/Air530Allfunction>
- Add Air530Display example <BasicUnit/Air530Display>

## (3/1/2021)
- Repair warning

## (5/1/2021)
- Add TWatch V3 board definition
- Check the return value before setting the power output <src/drive/axp/axp202.cpp>
- Move the Air530 example in <BasicUnit/TwatcV2Special/Air530*>

## (25/1/2021)
- Add Twatch-V2, Twatch-V3 BMA423 different mapping
- Update BaseUnit example


## (28/1/2021)
- Add touch three mode test example <BasicUnit/TouchpanelMode>
- Delete Air530 example
- Add basic Twatch 2020 V2 GPS example
- Update TWATCH 2020 V2 pin definition
- Update pin definition document

## (23/2/2021)
- <BasicUnit/AXP20x_ADC> example compatible with T-Block

## (27/3/2021)
- [Merged#119](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/pull/119)
- Update `library.properties`,up to `1.4.2` version
- Added <examples/Shield/S7xG_Debug> examples

## (4/1/2021)
- Update TWATCH-N touch to IO33
- Added <examples/UnitTest/HardwareTest> examples


## (4/6/2021)
- Add S7XG sample code <examples/Shield/S7xG/*>


## (4/13/2021)
- Add PN532 NFC examples, The example comes from <https://github.com/adafruit/Adafruit-PN532>