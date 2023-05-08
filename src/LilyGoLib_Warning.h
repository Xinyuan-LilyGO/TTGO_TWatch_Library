/**
 * @file      LilyGoLib_Warning.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-04-28
 *
 */

#pragma once


#if CONFIG_IDF_TARGET_ESP32S3

#if ARDUINO_USB_CDC_ON_BOOT != 1
#warning "If you need to monitor printed data, be sure to set USB CDC On boot to ENABLE, otherwise you will not see any data in the serial monitor"
#endif

#ifndef BOARD_HAS_PSRAM
#error "Detected that PSRAM is not turned on. Please set PSRAM to OPI PSRAM in ArduinoIDE"
#endif //BOARD_HAS_PSRAM

#elif CONFIG_IDF_TARGET_ESP32

#error "The current version only supports the LilyGo T-Watch S3 version, and other versions are not currently supported"


#ifndef BOARD_HAS_PSRAM
#error "Detected that PSRAM is not turned on. Please set PSRAM to ENABLE in ArduinoIDE"
#endif //BOARD_HAS_PSRAM


#endif //CONFIG_IDF_TARGET_ESP32S3

