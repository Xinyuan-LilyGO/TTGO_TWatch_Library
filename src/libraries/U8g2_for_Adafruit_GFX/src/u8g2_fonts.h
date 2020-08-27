/*

  u8g2_fonts.h
  
  Several definitions for the fonts, included and required for u8g2_fonts.c.
  If the u8g2_fonts.c file is copied from u8g2 project, replace the include 
  from "u8g2.h" to "u8g2_fonts.h"
  
  U8g2 for Adafruit GFX Lib (https://github.com/olikraus/U8g2_for_Adafruit_GFX)

  Copyright (c) 2018, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

#ifndef _u8g2_fonts_h
#define _u8g2_fonts_h

#include <stdint.h>

#ifdef __GNUC__
#  define U8X8_NOINLINE __attribute__((noinline))
#  define U8X8_SECTION(name) __attribute__ ((section (name)))
#  define U8X8_UNUSED __attribute__((unused))
#else
#  define U8X8_SECTION(name)
#  define U8X8_NOINLINE
#  define U8X8_UNUSED
#endif

#if defined(__GNUC__) && defined(__AVR__)
#  define U8X8_FONT_SECTION(name) U8X8_SECTION(".progmem." name)
#  define u8x8_pgm_read(adr) pgm_read_byte_near(adr)
#  define U8X8_PROGMEM PROGMEM
#endif

#ifndef U8X8_FONT_SECTION
#  define U8X8_FONT_SECTION(name) 
#endif

#ifndef u8x8_pgm_read
#  define u8x8_pgm_read(adr) (*(const uint8_t *)(adr)) 
#endif

#ifndef U8X8_PROGMEM
#  define U8X8_PROGMEM
#endif

#define U8G2_FONT_SECTION(name) U8X8_FONT_SECTION(name) 

/* the macro U8G2_USE_LARGE_FONTS enables large fonts (>32K) */
/* it can be enabled for those uC supporting larger arrays */
#if defined(unix) || defined(__arm__) || defined(__arc__) || defined(ESP8266) || defined(ESP_PLATFORM)
#ifndef U8G2_USE_LARGE_FONTS
#define U8G2_USE_LARGE_FONTS
#endif 
#endif


#endif /* _u8g2_fonts_h */

