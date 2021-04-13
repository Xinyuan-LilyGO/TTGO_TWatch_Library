# Adafruit-PN532 [![Build Status](https://github.com/adafruit/Adafruit-PN532/workflows/Arduino%20Library%20CI/badge.svg)](https://github.com/adafruit/Adafruit-PN532/actions)


This is a library for the Adafruit PN532 NFC/RFID breakout boards
This library works with the Adafruit NFC breakout 

  * https://www.adafruit.com/products/364
 
Check out the links above for our tutorials and wiring diagrams 
These chips use I2C or SPI to communicate.


<!-- START COMPATIBILITY TABLE -->

## Compatibility

MCU                | Tested Works | Doesn't Work | Not Tested  | Notes
------------------ | :----------: | :----------: | :---------: | -----
Atmega328 @ 16MHz  |      X       |             |            | SPI &amp; I2C Works
Atmega328 @ 12MHz  |      X       |             |            | SPI &amp; I2C Works
Atmega32u4 @ 16MHz |      X       |             |            | Follow instructions at https://learn.adafruit.com/adafruit-pn532-rfid-nfc/shield-wiring#using-with-the-arduino-leonardo-and-yun to move pin 2.
Atmega32u4 @ 8MHz  |      X       |             |            | SPI &amp; I2C Works
ESP8266            |             |      X       |            | SPI only, I2C clock stretching not supported
ESP32              |      X       |             |            | SPI works; I2C works using IRQ and without sharing the I2C bus.
Atmega2560 @ 16MHz |      X       |             |            | SPI &amp; I2C Works
ATSAM3X8E          |      X       |             |            | SPI &amp; I2C Works
ATSAM21D           |             |      X       |            | SPI only, I2C clock stretching not supported. Use programming port.
ATtiny85 @ 16MHz   |             |             |     X       | 
ATtiny85 @ 8MHz    |             |             |     X       | 
Intel Curie @ 32MHz |             |             |     X       | 
STM32F2            |             |             |     X       | 

  * ATmega328 @ 16MHz : Arduino UNO, Adafruit Pro Trinket 5V, Adafruit Metro 328, Adafruit Metro Mini
  * ATmega328 @ 12MHz : Adafruit Pro Trinket 3V
  * ATmega32u4 @ 16MHz : Arduino Leonardo, Arduino Micro, Arduino Yun, Teensy 2.0
  * ATmega32u4 @ 8MHz : Adafruit Flora, Bluefruit Micro
  * ESP8266 : Adafruit Huzzah
  * ESP32 : WROOM
  * ATmega2560 @ 16MHz : Arduino Mega
  * ATSAM3X8E : Arduino Due
  * ATSAM21D : Arduino Zero, M0 Pro
  * ATtiny85 @ 16MHz : Adafruit Trinket 5V
  * ATtiny85 @ 8MHz : Adafruit Gemma, Arduino Gemma, Adafruit Trinket 3V

<!-- END COMPATIBILITY TABLE -->
# Dependencies
* [Adafruit_BusIO](https://github.com/adafruit/Adafruit_BusIO)


# Contributing

Contributions are welcome! Please read our [Code of Conduct](https://github.com/adafruit/Adafruit-PN532/blob/master/CODE_OF_CONDUCT.md>)
before contributing to help this project stay welcoming.

## Documentation and doxygen
Documentation is produced by doxygen. Contributions should include documentation for any new code added.

Some examples of how to use doxygen can be found in these guide pages:

https://learn.adafruit.com/the-well-automated-arduino-library/doxygen

https://learn.adafruit.com/the-well-automated-arduino-library/doxygen-tips

## Formatting and clang-format
This library uses [`clang-format`](https://releases.llvm.org/download.html) to standardize the formatting of `.cpp` and `.h` files.
Contributions should be formatted using `clang-format`:

The `-i` flag will make the changes to the file.
```bash
clang-format -i *.cpp *.h
```
If you prefer to make the changes yourself, running `clang-format` without the `-i` flag will print out a formatted version of the file. You can save this to a file and diff it against the original to see the changes.

Note that the formatting output by `clang-format` is what the automated formatting checker will expect. Any diffs from this formatting will result in a failed build until they are addressed. Using the `-i` flag is highly recommended.

### clang-format resources
  * [Binary builds and source available on the LLVM downloads page](https://releases.llvm.org/download.html)
  * [Documentation and IDE integration](https://clang.llvm.org/docs/ClangFormat.html)
Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada & Kevin Townsend for Adafruit Industries.  
BSD license, check license.txt for more information
All text above must be included in any redistribution

To install, use the Arduino Library Manager and search for "Adafruit PN532" and install the library. Adafruit BusIO library is also required
