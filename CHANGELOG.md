# Changelog

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

