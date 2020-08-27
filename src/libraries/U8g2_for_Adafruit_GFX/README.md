
[tocstart]: # (toc start)

  * [U8g2_for_Adafruit_GFX](#u8g2_for_adafruit_gfx)
    * [What is U8g2_for_Adafruit_GFX?](#what-is-u8g2_for_adafruit_gfx)
    * [Which Adafruit GFX Libraries are Supported?](#which-adafruit-gfx-libraries-are-supported)
    * [How to use U8g2_for_Adafruit_GFX?](#how-to-use-u8g2_for_adafruit_gfx)
  * [Reference](#reference)
    * [Command Reference](#command-reference)
    * [Differences to U8g2](#differences-to-u8g2)

[tocend]: # (toc end)

# U8g2_for_Adafruit_GFX

Download: [https://github.com/olikraus/U8g2_for_Adafruit_GFX/archive/master.zip](https://github.com/olikraus/U8g2_for_Adafruit_GFX/archive/master.zip)

## What is U8g2_for_Adafruit_GFX?
 - Arduino Library
 - Adds a the [U8g2](https://github.com/olikraus/u8g2) text drawing engine to all Adafruit GFX based Arduino librarys.
 - All [U8g2 fonts](https://github.com/olikraus/u8g2/wiki/fntlistall) can be used 
 - Support for UTF-8 and Unicode
 - Support for Arduino print() command and F() Macro

U8g2 is a graphics library for monochrome displays. U8g2 supports many
displays, some of them are also supported by Adafruit GFX based libraries.
Others are supported by Adafruit GFX libraries, but are not supported by U8g2.
`U8g2_for_Adafruit_GFX` connects to an existing Adafruit Library and adds support for
U8g2 fonts to all Adafruit GFX based libraries.

## Which Adafruit GFX Libraries are Supported?

All libraries based on the Adafruit GFX engine are supported. This is a incomplete
list of such libraries. Probably many more are supported:

 - [https://github.com/adafruit/Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306) (tested) 
 - [https://github.com/adafruit/Adafruit-ST7735-Library](https://github.com/adafruit/Adafruit-ST7735-Library) (not tested)
 - [https://github.com/adafruit/Adafruit-Graphic-VFD-Display-Library](https://github.com/adafruit/Adafruit-Graphic-VFD-Display-Library) (not tested)
 - [https://github.com/adafruit/Adafruit-HX8340B](https://github.com/adafruit/Adafruit-HX8340B) (not tested)
 - [https://github.com/adafruit/Adafruit_HX8357_Library](https://github.com/adafruit/Adafruit_HX8357_Library) (not tested)
 - [https://github.com/adafruit/Adafruit_ILI9340](https://github.com/adafruit/Adafruit_ILI9340) (not tested)
 - [https://github.com/adafruit/Adafruit_ILI9341](https://github.com/adafruit/Adafruit_ILI9341)  (not tested)
 - [https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library](https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library) (not tested)
 
 
## How to use U8g2_for_Adafruit_GFX?

This is a complete example for `U8g2_for_Adafruit_GFX` connected to Adafruit SSD1306 
library:

```
#include <Adafruit_SSD1306.h>
#include <U8g2_for_Adafruit_GFX.h>

Adafruit_SSD1306 display(/*MOSI*/ 11, /*CLK*/ 13, /*DC*/ 9, /*RESET*/ 8, /*CS*/ 10);
U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC);
  u8g2_for_adafruit_gfx.begin(display);                 // connect u8g2 procedures to Adafruit GFX
}

void loop() {  
  display.clearDisplay();                               // clear the graphcis buffer  
  u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
  u8g2_for_adafruit_gfx.setForegroundColor(WHITE);      // apply Adafruit GFX color
  u8g2_for_adafruit_gfx.setFont(u8g2_font_helvR14_tf);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2_for_adafruit_gfx.setCursor(0,20);                // start writing at this position
  u8g2_for_adafruit_gfx.print(F("Hello World"));
  u8g2_for_adafruit_gfx.setCursor(0,40);                // start writing at this position
  u8g2_for_adafruit_gfx.print(F("Umlaut ÄÖÜ"));            // UTF-8 string with german umlaut chars
  display.display();                                    // make everything visible
  delay(2000);
} 
```

# Reference

## Command Reference

 - `U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx`: Constructor call. This must be
   the first command. No paramenters are required.
 - `void u8g2_for_adafruit_gfx.begin(Adafruit_GFX &gfx)`: Connect to Adafruit GFX library
 - `void u8g2_for_adafruit_gfx.setFont(const uint8_t *font)`: Assign u8g2 font, see  [here](https://github.com/olikraus/u8g2/wiki/fntlistall) for a list. 
    Note: The list from the u8g2 project might be a little bit newer, so maybe not allways all fonts are available for this project. 
    See also: [https://github.com/olikraus/u8g2/wiki/u8g2reference#setfont](https://github.com/olikraus/u8g2/wiki/u8g2reference#setfont)
 - `void u8g2_for_adafruit_gfx.setFontMode(uint8_t is_transparent)`: Define, whether the background color should be drawn or not. 
    See also: [https://github.com/olikraus/u8g2/wiki/u8g2reference#setfontmode](https://github.com/olikraus/u8g2/wiki/u8g2reference#setfontmode)
 - `void u8g2_for_adafruit_gfx.setFontDirection(uint8_t dir)`: Identical to the u8g2 command 
     [https://github.com/olikraus/u8g2/wiki/u8g2reference#setfontdirection](https://github.com/olikraus/u8g2/wiki/u8g2reference#setfontdirection)
 - `void u8g2_for_adafruit_gfx.setForegroundColor(uint8_t dir)`: Set the foreground color for the text.
 - `void u8g2_for_adafruit_gfx.setBackgroundColor(uint8_t dir)`: Set the background color for the text. This is only required if the font mode is not set to transparent.
 - `void u8g2_for_adafruit_gfx.setCursor(int16_t x, int16_t y)`: Specify the x/y position for the next print() command. This x/y position is the lower left baseline of a character.
   For "Arduino" this will be the lower left corner of the A character.
 - `void u8g2_for_adafruit_gfx.print(...)`: This is the standard Arduino print function. Any text given here will be output on the display via Adafruit GFX library. 
    See [here](https://www.arduino.cc/en/Serial/Print) for further description. Note: This function supports UTF-8.
 - `int8_t u8g2_for_adafruit_gfx.getFontAscent(void)`: The height of the character 'A' in the current font.
 - `int8_t u8g2_for_adafruit_gfx.getFontDescent(void)`: The extend of 'g' below baseline. This value usually is negative.
 - `int16_t u8g2_for_adafruit_gfx.getUTF8Width(const char *str)`: Return the width of the provided string. 
   It is assumed, that `str` is encoded in UTF8 (this is default for Arduino IDE and gcc/g++).
    
 
## Differences to U8g2
  There are some minor differences to the U8g2 library:
  - For U8g2 the color is specified with the `setDrawColor()` function. 
  - The `print()` command supports UTF-8 by default. It also can not be disabled. For U8g2 the UTF-8 support must be enabled first.
  - This library does not support the change of the reference position. The reference position is the lower left baseline of a character.
 
## Release Procedure
  - Copy font file u8g2_fonts.c to "src" folder
  - Change the include file in u8g2_fonts.c from "u8g2.h" to "u8g2_fonts.h"
  - Update the list of fonts in "U8g2_for_Adafruit_GFX.h"
  - Update keywords.txt
  - Update version in library.properties 
 
 
 
