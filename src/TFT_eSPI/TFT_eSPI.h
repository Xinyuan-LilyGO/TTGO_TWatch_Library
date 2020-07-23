/***************************************************
  Arduino TFT graphics library targeted at ESP8266
  and ESP32 based boards.

  This is a standalone library that contains the
  hardware driver, the graphics functions and the
  proportional fonts.

  The built-in fonts 4, 6, 7 and 8 are Run Length
  Encoded (RLE) to reduce the FLASH footprint.

  Last review/edit by Bodmer: 26/01/20
 ****************************************************/

// Stop fonts etc being loaded multiple times
#ifndef _TFT_eSPIH_
#define _TFT_eSPIH_

#define TFT_ESPI_VERSION "2.2.15"

/***************************************************************************************
**                         Section 1: Load required header files
***************************************************************************************/

//Standard support
#include <Arduino.h>
#include <Print.h>
#include <SPI.h>

/***************************************************************************************
**                         Section 2: Load library and processor specific header files
***************************************************************************************/
// Include header file that defines the fonts loaded, the TFT drivers
// available and the pins to be used, etc, etc
#include "User_Setups/Setup45_TTGO_T_Watch.h"     // Setup file for ESP32 and TTGO T-Watch ST7789 SPI bus TFT  240x240
#include "TFT_Drivers/ST7789_Defines.h"
#define  TFT_DRIVER 0x7789

// Handle FLASH based storage e.g. PROGMEM
#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#define PROGMEM
#endif

// Include the processor specific drivers
#if defined (ESP32)
////////////////////////////////////////////////////
// TFT_eSPI driver functions for ESP32 processors //
////////////////////////////////////////////////////

#ifndef _TFT_eSPI_ESP32H_
#define _TFT_eSPI_ESP32H_

// Processor ID reported by getSetup()
#define PROCESSOR_ID 0x32

// Include processor specific header
#include "soc/spi_reg.h"
#include "driver/spi_master.h"

// Processor specific code used by SPI bus transaction startWrite and endWrite functions
#define SET_BUS_WRITE_MODE // Not used
#define SET_BUS_READ_MODE  // Not used

// SUPPORT_TRANSACTIONS is mandatory for ESP32 so the hal mutex is toggled
#if !defined (SUPPORT_TRANSACTIONS)
#define SUPPORT_TRANSACTIONS
#endif

// ESP32 specific SPI port selection
#ifdef USE_HSPI_PORT
#define SPI_PORT HSPI
#else
#define SPI_PORT VSPI
#endif

#ifdef RPI_DISPLAY_TYPE
#define CMD_BITS (16-1)
#else
#define CMD_BITS (8-1)
#endif

// Initialise processor specific SPI functions, used by init()
#define INIT_TFT_DATA_BUS // Not used

// Define a generic flag for 8 bit parallel
#if defined (ESP32_PARALLEL) // Specific to ESP32 for backwards compatibility
#if !defined (TFT_PARALLEL_8_BIT)
#define TFT_PARALLEL_8_BIT // Generic parallel flag
#endif
#endif

// Ensure ESP32 specific flag is defined for 8 bit parallel
#if defined (TFT_PARALLEL_8_BIT)
#if !defined (ESP32_PARALLEL)
#define ESP32_PARALLEL
#endif
#endif

// Code to check if DMA is busy, used by SPI bus transaction transaction and endWrite functions
#if !defined(TFT_PARALLEL_8_BIT) && !defined(ILI9488_DRIVER) && !defined (RPI_DISPLAY_TYPE)
#define ESP32_DMA
// Code to check if DMA is busy, used by SPI DMA + transaction + endWrite functions
#define DMA_BUSY_CHECK  dmaWait()
#else
#define DMA_BUSY_CHECK
#endif

// If smooth font is used then it is likely SPIFFS will be needed
#ifdef SMOOTH_FONT
// Call up the SPIFFS (SPI FLASH Filing System) for the anti-aliased fonts
#define FS_NO_GLOBALS
#include <FS.h>
#include "SPIFFS.h" // ESP32 only
#define FONT_FS_AVAILABLE
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Define the DC (TFT Data/Command or Register Select (RS))pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#ifndef TFT_DC
#define DC_C // No macro allocated so it generates no code
#define DC_D // No macro allocated so it generates no code
#else
#if defined (TFT_PARALLEL_8_BIT)
#define DC_C GPIO.out_w1tc = (1 << TFT_DC)
#define DC_D GPIO.out_w1ts = (1 << TFT_DC)
#else
#if TFT_DC >= 32
#ifdef RPI_DISPLAY_TYPE  // RPi displays need a slower DC change
#define DC_C GPIO.out1_w1ts.val = (1 << (TFT_DC - 32)); \
                     GPIO.out1_w1tc.val = (1 << (TFT_DC - 32))
#define DC_D GPIO.out1_w1tc.val = (1 << (TFT_DC - 32)); \
                     GPIO.out1_w1ts.val = (1 << (TFT_DC - 32))
#else
#define DC_C GPIO.out1_w1tc.val = (1 << (TFT_DC - 32))//;GPIO.out1_w1tc.val = (1 << (TFT_DC - 32))
#define DC_D GPIO.out1_w1ts.val = (1 << (TFT_DC - 32))//;GPIO.out1_w1ts.val = (1 << (TFT_DC - 32))
#endif
#elif TFT_DC >= 0
#ifdef RPI_DISPLAY_TYPE  // RPi ILI9486 display needs a slower DC change
#define DC_C GPIO.out_w1tc = (1 << TFT_DC); \
                     GPIO.out_w1tc = (1 << TFT_DC)
#define DC_D GPIO.out_w1tc = (1 << TFT_DC); \
                     GPIO.out_w1ts = (1 << TFT_DC)
#elif defined (RPI_DISPLAY_TYPE)  // Other RPi displays need a slower C->D change
#define DC_C GPIO.out_w1tc = (1 << TFT_DC)
#define DC_D GPIO.out_w1tc = (1 << TFT_DC); \
                     GPIO.out_w1ts = (1 << TFT_DC)
#else
#define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
#define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
#endif
#else
#define DC_C
#define DC_D
#endif
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Define the CS (TFT chip select) pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#ifndef TFT_CS
#define TFT_CS -1  // Keep DMA code happy
#define CS_L       // No macro allocated so it generates no code
#define CS_H       // No macro allocated so it generates no code
#else
#if defined (TFT_PARALLEL_8_BIT)
#if TFT_CS >= 32
#define CS_L GPIO.out1_w1tc.val = (1 << (TFT_CS - 32))
#define CS_H GPIO.out1_w1ts.val = (1 << (TFT_CS - 32))
#elif TFT_CS >= 0
#define CS_L GPIO.out_w1tc = (1 << TFT_CS)
#define CS_H GPIO.out_w1ts = (1 << TFT_CS)
#else
#define CS_L
#define CS_H
#endif
#else
#if TFT_CS >= 32
#ifdef RPI_DISPLAY_TYPE  // RPi ILI9486 display needs a slower CS change
#define CS_L GPIO.out1_w1ts.val = (1 << (TFT_CS - 32)); \
                     GPIO.out1_w1tc.val = (1 << (TFT_CS - 32))
#define CS_H GPIO.out1_w1tc.val = (1 << (TFT_CS - 32)); \
                     GPIO.out1_w1ts.val = (1 << (TFT_CS - 32))
#else
#define CS_L GPIO.out1_w1tc.val = (1 << (TFT_CS - 32)); GPIO.out1_w1tc.val = (1 << (TFT_CS - 32))
#define CS_H GPIO.out1_w1ts.val = (1 << (TFT_CS - 32))//;GPIO.out1_w1ts.val = (1 << (TFT_CS - 32))
#endif
#elif TFT_CS >= 0
#ifdef RPI_DISPLAY_TYPE  // RPi ILI9486 display needs a slower CS change
#define CS_L GPIO.out_w1ts = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
#define CS_H GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1ts = (1 << TFT_CS)
#else
#define CS_L GPIO.out_w1tc = (1 << TFT_CS);GPIO.out_w1tc = (1 << TFT_CS)
#define CS_H GPIO.out_w1ts = (1 << TFT_CS)//;GPIO.out_w1ts = (1 << TFT_CS)
#endif
#else
#define CS_L
#define CS_H
#endif
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Define the WR (TFT Write) pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#ifdef TFT_WR
#define WR_L GPIO.out_w1tc = (1 << TFT_WR)
#define WR_H GPIO.out_w1ts = (1 << TFT_WR)
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Define the touch screen chip select pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#ifndef TOUCH_CS
#define T_CS_L // No macro allocated so it generates no code
#define T_CS_H // No macro allocated so it generates no code
#else // XPT2046 is slow, so use slower digitalWrite here
#define T_CS_L digitalWrite(TOUCH_CS, LOW)
#define T_CS_H digitalWrite(TOUCH_CS, HIGH)
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Make sure SPI default pins are assigned if not specified by user or set to -1
////////////////////////////////////////////////////////////////////////////////////////
#if !defined (TFT_PARALLEL_8_BIT)

#ifdef USE_HSPI_PORT

#ifndef TFT_MISO
#define TFT_MISO 12
#endif
#if (TFT_MISO == -1)
#undef TFT_MISO
#define TFT_MISO 12
#endif

#ifndef TFT_MOSI
#define TFT_MOSI 13
#endif
#if (TFT_MOSI == -1)
#undef TFT_MOSI
#define TFT_MOSI 13
#endif

#ifndef TFT_SCLK
#define TFT_SCLK 14
#endif
#if (TFT_SCLK == -1)
#undef TFT_SCLK
#define TFT_SCLK 14
#endif

#else // VSPI port

#ifndef TFT_MISO
#define TFT_MISO 19
#endif
#if (TFT_MISO == -1)
#undef TFT_MISO
#define TFT_MISO 19
#endif

#ifndef TFT_MOSI
#define TFT_MOSI 23
#endif
#if (TFT_MOSI == -1)
#undef TFT_MOSI
#define TFT_MOSI 23
#endif

#ifndef TFT_SCLK
#define TFT_SCLK 18
#endif
#if (TFT_SCLK == -1)
#undef TFT_SCLK
#define TFT_SCLK 18
#endif

#endif

#endif

////////////////////////////////////////////////////////////////////////////////////////
// Define the parallel bus interface chip pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#if defined (TFT_PARALLEL_8_BIT)

// Create a bit set lookup table for data bus - wastes 1kbyte of RAM but speeds things up dramatically
// can then use e.g. GPIO.out_w1ts = set_mask(0xFF); to set data bus to 0xFF
#define CONSTRUCTOR_INIT_TFT_DATA_BUS            \
  for (int32_t c = 0; c<256; c++)                  \
  {                                                \
    xset_mask[c] = 0;                              \
    if ( c & 0x01 ) xset_mask[c] |= (1 << TFT_D0); \
    if ( c & 0x02 ) xset_mask[c] |= (1 << TFT_D1); \
    if ( c & 0x04 ) xset_mask[c] |= (1 << TFT_D2); \
    if ( c & 0x08 ) xset_mask[c] |= (1 << TFT_D3); \
    if ( c & 0x10 ) xset_mask[c] |= (1 << TFT_D4); \
    if ( c & 0x20 ) xset_mask[c] |= (1 << TFT_D5); \
    if ( c & 0x40 ) xset_mask[c] |= (1 << TFT_D6); \
    if ( c & 0x80 ) xset_mask[c] |= (1 << TFT_D7); \
  }                                                \

// Mask for the 8 data bits to set pin directions
#define dir_mask ((1 << TFT_D0) | (1 << TFT_D1) | (1 << TFT_D2) | (1 << TFT_D3) | (1 << TFT_D4) | (1 << TFT_D5) | (1 << TFT_D6) | (1 << TFT_D7))

// Data bits and the write line are cleared to 0 in one step
#define clr_mask (dir_mask | (1 << TFT_WR))

// A lookup table is used to set the different bit patterns, this uses 1kByte of RAM
#define set_mask(C) xset_mask[C] // 63fps Sprite rendering test 33% faster, graphicstest only 1.8% faster than shifting in real time

// Real-time shifting alternative to above to save 1KByte RAM, 47 fps Sprite rendering test
/*#define set_mask(C) (((C)&0x80)>>7)<<TFT_D7 | (((C)&0x40)>>6)<<TFT_D6 | (((C)&0x20)>>5)<<TFT_D5 | (((C)&0x10)>>4)<<TFT_D4 | \
                      (((C)&0x08)>>3)<<TFT_D3 | (((C)&0x04)>>2)<<TFT_D2 | (((C)&0x02)>>1)<<TFT_D1 | (((C)&0x01)>>0)<<TFT_D0
//*/

// Write 8 bits to TFT
#define tft_Write_8(C)  GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t)(C)); WR_H

// Write 16 bits to TFT
#define tft_Write_16(C) GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t)((C) >> 8)); WR_H; \
                          GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t)((C) >> 0)); WR_H

// 16 bit write with swapped bytes
#define tft_Write_16S(C) GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 0)); WR_H; \
                           GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 8)); WR_H

// Write 32 bits to TFT
#define tft_Write_32(C) GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 24)); WR_H; \
                          GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 16)); WR_H; \
                          GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >>  8)); WR_H; \
                          GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >>  0)); WR_H

// Write two concatenated 16 bit values to TFT
#define tft_Write_32C(C,D) GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 8)); WR_H; \
                             GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 0)); WR_H; \
                             GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((D) >> 8)); WR_H; \
                             GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((D) >> 0)); WR_H

// Write 16 bit value twice to TFT - used by drawPixel()
#define tft_Write_32D(C) GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 8)); WR_H; \
                           GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 0)); WR_H; \
                           GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 8)); WR_H; \
                           GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 0)); WR_H

// Read pin
#ifdef TFT_RD
#define RD_L GPIO.out_w1tc = (1 << TFT_RD)
//#define RD_L digitalWrite(TFT_WR, LOW)
#define RD_H GPIO.out_w1ts = (1 << TFT_RD)
//#define RD_H digitalWrite(TFT_WR, HIGH)
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Macros to write commands/pixel colour data to an ILI9488 TFT
////////////////////////////////////////////////////////////////////////////////////////
#elif  defined (ILI9488_DRIVER) // 16 bit colour converted to 3 bytes for 18 bit RGB

// Write 8 bits to TFT
#define tft_Write_8(C)   spi.transfer(C)

// Convert 16 bit colour to 18 bit and write in 3 bytes
#define tft_Write_16(C)  spi.transfer(((C) & 0xF800)>>8); \
                           spi.transfer(((C) & 0x07E0)>>3); \
                           spi.transfer(((C) & 0x001F)<<3)

// Convert swapped byte 16 bit colour to 18 bit and write in 3 bytes
#define tft_Write_16S(C) spi.transfer((C) & 0xF8); \
                           spi.transfer(((C) & 0xE000)>>11 | ((C) & 0x07)<<5); \
                           spi.transfer(((C) & 0x1F00)>>5)

// Write 32 bits to TFT
#define tft_Write_32(C)  spi.write32(C)

// Write two concatenated 16 bit values to TFT
#define tft_Write_32C(C,D) spi.write32((C)<<16 | (D))

// Write 16 bit value twice to TFT
#define tft_Write_32D(C)  spi.write32((C)<<16 | (C))

////////////////////////////////////////////////////////////////////////////////////////
// Macros to write commands/pixel colour data to an Raspberry Pi TFT
////////////////////////////////////////////////////////////////////////////////////////
#elif  defined (RPI_DISPLAY_TYPE)

// ESP32 low level SPI writes for 8, 16 and 32 bit values
// to avoid the function call overhead
#define TFT_WRITE_BITS(D, B) \
  WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), B-1); \
  WRITE_PERI_REG(SPI_W0_REG(SPI_PORT), D); \
  SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR); \
  while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);

// Write 8 bits
#define tft_Write_8(C) TFT_WRITE_BITS((C)<<8, 16)

// Write 16 bits with corrected endianess for 16 bit colours
#define tft_Write_16(C) TFT_WRITE_BITS((C)<<8 | (C)>>8, 16)

// Write 16 bits
#define tft_Write_16S(C) TFT_WRITE_BITS(C, 16)

// Write 32 bits
#define tft_Write_32(C) TFT_WRITE_BITS(C, 32)

// Write two address coordinates
#define tft_Write_32C(C,D)  TFT_WRITE_BITS((C)<<24 | (C), 32); \
                              TFT_WRITE_BITS((D)<<24 | (D), 32)

// Write same value twice
#define tft_Write_32D(C) tft_Write_32C(C,C)

////////////////////////////////////////////////////////////////////////////////////////
// Macros for all other SPI displays
////////////////////////////////////////////////////////////////////////////////////////
#else

// ESP32 low level SPI writes for 8, 16 and 32 bit values
// to avoid the function call overhead
#define TFT_WRITE_BITS(D, B) \
  WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), B-1); \
  WRITE_PERI_REG(SPI_W0_REG(SPI_PORT), D); \
  SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR); \
  while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);

// Write 8 bits
#define tft_Write_8(C) TFT_WRITE_BITS(C, 8)

// Write 16 bits with corrected endianess for 16 bit colours
#define tft_Write_16(C) TFT_WRITE_BITS((C)<<8 | (C)>>8, 16)

// Write 16 bits
#define tft_Write_16S(C) TFT_WRITE_BITS(C, 16)

// Write 32 bits
#define tft_Write_32(C) TFT_WRITE_BITS(C, 32)

// Write two address coordinates
#define tft_Write_32C(C,D)  TFT_WRITE_BITS((uint16_t)((D)<<8 | (D)>>8)<<16 | (uint16_t)((C)<<8 | (C)>>8), 32)

// Write same value twice
#define tft_Write_32D(C) TFT_WRITE_BITS((uint16_t)((C)<<8 | (C)>>8)<<16 | (uint16_t)((C)<<8 | (C)>>8), 32)

#endif

////////////////////////////////////////////////////////////////////////////////////////
// Macros to read from display using SPI or software SPI
////////////////////////////////////////////////////////////////////////////////////////
#if !defined (TFT_PARALLEL_8_BIT)
// Read from display using SPI or software SPI
// Use a SPI read transfer
#define tft_Read_8() spi.transfer(0)
#endif

// Concatenate a byte sequence A,B,C,D to CDAB, P is a uint8_t pointer
#define DAT8TO32(P) ( (uint32_t)P[0]<<8 | P[1] | P[2]<<24 | P[3]<<16 )

#endif // Header end

#endif

/***************************************************************************************
**                         Section 3: Interface setup
***************************************************************************************/
#ifndef TAB_COLOUR
#define TAB_COLOUR 0
#endif

// If the SPI frequency is not defined, set a default
#ifndef SPI_FREQUENCY
#define SPI_FREQUENCY  20000000
#endif

// If the SPI read frequency is not defined, set a default
#ifndef SPI_READ_FREQUENCY
#define SPI_READ_FREQUENCY 10000000
#endif

// Some ST7789 boards do not work with Mode 0
#if defined(ST7789_DRIVER) || defined(ST7789_2_DRIVER)
#define TFT_SPI_MODE SPI_MODE3
#else
#define TFT_SPI_MODE SPI_MODE0
#endif

// If the XPT2046 SPI frequency is not defined, set a default
#ifndef SPI_TOUCH_FREQUENCY
#define SPI_TOUCH_FREQUENCY  2500000
#endif

/***************************************************************************************
**                         Section 4: Setup fonts
***************************************************************************************/
// Use GLCD font in error case where user requests a smooth font file
// that does not exist (this is a temporary fix to stop ESP32 reboot)
#ifdef SMOOTH_FONT
#ifndef LOAD_GLCD
#define LOAD_GLCD
#endif
#endif

// Only load the fonts defined in User_Setup.h (to save space)
// Set flag so RLE rendering code is optionally compiled
#ifdef LOAD_GLCD
#include "Fonts/glcdfont.c"
#endif

#ifdef LOAD_FONT2
#include "Fonts/Font16.h"
#endif

#ifdef LOAD_FONT4
#include "Fonts/Font32rle.h"
#define LOAD_RLE
#endif

#ifdef LOAD_FONT6
#include "Fonts/Font64rle.h"
#ifndef LOAD_RLE
#define LOAD_RLE
#endif
#endif

#ifdef LOAD_FONT7
#include "Fonts/Font7srle.h"
#ifndef LOAD_RLE
#define LOAD_RLE
#endif
#endif

#ifdef LOAD_FONT8
#include "Fonts/Font72rle.h"
#ifndef LOAD_RLE
#define LOAD_RLE
#endif
#elif defined LOAD_FONT8N // Optional narrower version
#define LOAD_FONT8
#include "Fonts/Font72x53rle.h"
#ifndef LOAD_RLE
#define LOAD_RLE
#endif
#endif

#ifdef LOAD_GFXFF
// We can include all the free fonts and they will only be built into
// the sketch if they are used
#include "Fonts/GFXFF/gfxfont.h"
// Call up any user custom fonts
#include "User_Setups/User_Custom_Fonts.h"
#endif // #ifdef LOAD_GFXFF

// Create a null default font in case some fonts not used (to prevent crash)
const  uint8_t widtbl_null[1] = {0};
PROGMEM const uint8_t chr_null[1] = {0};
PROGMEM const uint8_t *const chrtbl_null[1] = {chr_null};

// This is a structure to conveniently hold information on the default fonts
// Stores pointer to font character image address table, width table and height
typedef struct {
    const uint8_t *chartbl;
    const uint8_t *widthtbl;
    uint8_t height;
    uint8_t baseline;
} fontinfo;

// Now fill the structure
const PROGMEM fontinfo fontdata [] = {
#ifdef LOAD_GLCD
    { (const uint8_t *)font, widtbl_null, 0, 0 },
#else
    { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
#endif
    // GLCD font (Font 1) does not have all parameters
    { (const uint8_t *)chrtbl_null, widtbl_null, 8, 7 },

#ifdef LOAD_FONT2
    { (const uint8_t *)chrtbl_f16, widtbl_f16, chr_hgt_f16, baseline_f16},
#else
    { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
#endif

    // Font 3 current unused
    { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },

#ifdef LOAD_FONT4
    { (const uint8_t *)chrtbl_f32, widtbl_f32, chr_hgt_f32, baseline_f32},
#else
    { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
#endif

    // Font 5 current unused
    { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },

#ifdef LOAD_FONT6
    { (const uint8_t *)chrtbl_f64, widtbl_f64, chr_hgt_f64, baseline_f64},
#else
    { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
#endif

#ifdef LOAD_FONT7
    { (const uint8_t *)chrtbl_f7s, widtbl_f7s, chr_hgt_f7s, baseline_f7s},
#else
    { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
#endif

#ifdef LOAD_FONT8
    { (const uint8_t *)chrtbl_f72, widtbl_f72, chr_hgt_f72, baseline_f72}
#else
    { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 }
#endif
};

/***************************************************************************************
**                         Section 5: Font datum enumeration
***************************************************************************************/
//These enumerate the text plotting alignment (reference datum point)
#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right
#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
#define C_BASELINE 10 // Centre character baseline
#define R_BASELINE 11 // Right character baseline

/***************************************************************************************
**                         Section 6: Colour enumeration
***************************************************************************************/
// Default color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F      
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
#define TFT_SILVER      0xC618      /* 192, 192, 192 */
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
#define TFT_VIOLET      0x915C      /* 180,  46, 226 */

// Next is a special 16 bit colour value that encodes to 8 bits
// and will then decode back to the same 16 bit value.
// Convenient for 8 bit and 16 bit transparent sprites.
#define TFT_TRANSPARENT 0x0120 // This is actually a dark green

// Default palette for 4 bit colour sprites
static const uint16_t default_4bit_palette[] PROGMEM = {
    TFT_BLACK,    //  0  ^
    TFT_BROWN,    //  1  |
    TFT_RED,      //  2  |
    TFT_ORANGE,   //  3  |
    TFT_YELLOW,   //  4  Colours 0-9 follow the resistor colour code!
    TFT_GREEN,    //  5  |
    TFT_BLUE,     //  6  |
    TFT_PURPLE,   //  7  |
    TFT_DARKGREY, //  8  |
    TFT_WHITE,    //  9  v
    TFT_CYAN,     // 10  Blue+green mix
    TFT_MAGENTA,  // 11  Blue+red mix
    TFT_MAROON,   // 12  Darker red colour
    TFT_DARKGREEN,// 13  Darker green colour
    TFT_NAVY,     // 14  Darker blue colour
    TFT_PINK      // 15
};

/***************************************************************************************
**                         Section 7: Diagnostic support
***************************************************************************************/
// #define TFT_eSPI_DEBUG     // Switch on debug support serial messages  (not used yet)
// #define TFT_eSPI_FNx_DEBUG // Switch on debug support for function "x" (not used yet)

// This structure allows sketches to retrieve the user setup parameters at runtime
// by calling getSetup(), zero impact on code size unless used, mainly for diagnostics
typedef struct {
    String  version = TFT_ESPI_VERSION;
    int32_t esp;         // Processor code
    uint8_t trans;       // SPI transaction supoort
    uint8_t serial;      // Serial (SPI) or parallel
    uint8_t overlap;     // ESP8266 overlap mode

#if defined (ESP32)  // TODO: make generic for other processors
#if defined (USE_HSPI_PORT)
    uint8_t  port = HSPI;
#else
    uint8_t  port = VSPI;
#endif
#endif

    uint16_t tft_driver; // Hexadecimal code
    uint16_t tft_width;  // Rotation 0 width and height
    uint16_t tft_height;

    uint8_t r0_x_offset; // Display offsets, not all used yet
    uint8_t r0_y_offset;
    uint8_t r1_x_offset;
    uint8_t r1_y_offset;
    uint8_t r2_x_offset;
    uint8_t r2_y_offset;
    uint8_t r3_x_offset;
    uint8_t r3_y_offset;

    int8_t pin_tft_mosi; // SPI pins
    int8_t pin_tft_miso;
    int8_t pin_tft_clk;
    int8_t pin_tft_cs;

    int8_t pin_tft_dc;   // Control pins
    int8_t pin_tft_rd;
    int8_t pin_tft_wr;
    int8_t pin_tft_rst;

    int8_t pin_tft_d0;   // Parallel port pins
    int8_t pin_tft_d1;
    int8_t pin_tft_d2;
    int8_t pin_tft_d3;
    int8_t pin_tft_d4;
    int8_t pin_tft_d5;
    int8_t pin_tft_d6;
    int8_t pin_tft_d7;

    int8_t pin_tft_led;
    int8_t pin_tft_led_on;

    int8_t pin_tch_cs;   // Touch chip select pin

    int16_t tft_spi_freq;// TFT write SPI frequency
    int16_t tft_rd_freq; // TFT read  SPI frequency
    int16_t tch_spi_freq;// Touch controller read/write SPI frequency
} setup_t;

/***************************************************************************************
**                         Section 8: Class member and support functions
***************************************************************************************/
// Swap any type
template <typename T> static inline void
swap_coord(T &a, T &b)
{
    T t = a;
    a = b;
    b = t;
}

// Callback prototype for smooth font pixel colour read
typedef uint16_t (*getColorCallback)(uint16_t x, uint16_t y);

// Class functions and variables
class TFT_eSPI : public Print
{

//--------------------------------------- public ------------------------------------//
public:

    TFT_eSPI(int16_t _W = TFT_WIDTH, int16_t _H = TFT_HEIGHT);

    // init() and begin() are equivalent, begin() included for backwards compatibility
    // Sketch defined tab colour option is for ST7735 displays only
    void     init(uint8_t tc = TAB_COLOUR), begin(uint8_t tc = TAB_COLOUR);

    // These are virtual so the TFT_eSprite class can override them with sprite specific functions
    virtual void     drawPixel(int32_t x, int32_t y, uint32_t color),
            drawChar(int32_t x, int32_t y, uint16_t c, uint32_t color, uint32_t bg, uint8_t size),
            drawLine(int32_t xs, int32_t ys, int32_t xe, int32_t ye, uint32_t color),
            drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color),
            drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color),
            fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);

    virtual int16_t  drawChar(uint16_t uniCode, int32_t x, int32_t y, uint8_t font),
            drawChar(uint16_t uniCode, int32_t x, int32_t y),
            height(void),
            width(void);

    void     setRotation(uint8_t r); // Set the display image orientation to 0, 1, 2 or 3
    uint8_t  getRotation(void);      // Read the current rotation

    void     invertDisplay(bool i);  // Tell TFT to invert all displayed colours


    // The TFT_eSprite class inherits the following functions (not all are useful to Sprite class
    void     setAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h), // Note: start coordinates + width and height
             setWindow(int32_t xs, int32_t ys, int32_t xe, int32_t ye);   // Note: start + end coordinates

    // Push (aka write pixel) colours to the TFT (use setAddrWindow() first)
    void     pushColor(uint16_t color),
             pushColor(uint16_t color, uint32_t len),  // Deprecated, use pushBlock()
             pushColors(uint16_t  *data, uint32_t len, bool swap = true), // With byte swap option
             pushColors(uint8_t  *data, uint32_t len); // Deprecated, use pushPixels()

    // Write a solid block of a single colour
    void     pushBlock(uint16_t color, uint32_t len);

    // Write a set of pixels stored in memory, use setSwapBytes(true/false) function to correct endianess
    void     pushPixels(const void *data_in, uint32_t len);

    // Read the colour of a pixel at x,y and return value in 565 format
    uint16_t readPixel(int32_t x, int32_t y);

    // Support for half duplex (bi-directional SDA) SPI bus where MOSI must be switched to input
#ifdef TFT_SDA_READ
#if defined (TFT_eSPI_ENABLE_8_BIT_READ)
    uint8_t  tft_Read_8(void);     // Read 8 bit value from TFT command register
#endif
    void     begin_SDA_Read(void); // Begin a read on a half duplex (bi-directional SDA) SPI bus - sets MOSI to input
    void     end_SDA_Read(void);   // Restore MOSI to output
#endif

    // Graphics drawing
    void     fillScreen(uint32_t color),
             drawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color),
             drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color),
             fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color);


    void     drawCircle(int32_t x, int32_t y, int32_t r, uint32_t color),
             drawCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, uint32_t color),
             fillCircle(int32_t x, int32_t y, int32_t r, uint32_t color),
             fillCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, int32_t delta, uint32_t color),

             drawEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color),
             fillEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color),

             //                 Corner 1               Corner 2               Corner 3
             drawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color),
             fillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color);

    // Image rendering
    // Swap the byte order for pushImage() and pushPixels() - corrects endianness
    void     setSwapBytes(bool swap);
    bool     getSwapBytes(void);

    // Draw bitmap
    void     drawBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor),
             drawBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor, uint16_t bgcolor),
             drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor),
             drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor, uint16_t bgcolor),
             setBitmapColor(uint16_t fgcolor, uint16_t bgcolor); // Define the 2 colours for 1bpp sprites

    // Set TFT pivot point (use when rendering rotated sprites)
    void     setPivot(int16_t x, int16_t y);
    int16_t  getPivotX(void), // Get pivot x
             getPivotY(void); // Get pivot y

    // The next functions can be used as a pair to copy screen blocks (or horizontal/vertical lines) to another location
    // Read a block of pixels to a data buffer, buffer is 16 bit and the size must be at least w * h
    void     readRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);
    // Write a block of pixels to the screen which have been read by readRect()
    void     pushRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);

    // These are used to render images or sprites stored in RAM arrays (used by Sprite class for 16bpp Sprites)
    void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);
    void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, uint16_t transparent);

    // These are used to render images stored in FLASH (PROGMEM)
    void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data, uint16_t transparent);
    void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data);

    // These are used by Sprite class pushSprite() member function for 1, 4 and 8 bits per pixel (bpp) colours
    // They are not intended to be used with user sketches (but could be)
    // Set bpp8 true for 8bpp sprites, false otherwise. The cmap pointer must be specified for 4bpp
    void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, bool bpp8 = true, uint16_t *cmap = nullptr);
    void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, uint8_t  transparent, bool bpp8 = true, uint16_t *cmap = nullptr);

    // This next function has been used successfully to dump the TFT screen to a PC for documentation purposes
    // It reads a screen area and returns the 3 RGB 8 bit colour values of each pixel in the buffer
    // Set w and h to 1 to read 1 pixel's colour. The data buffer must be at least w * h * 3 bytes
    void     readRectRGB(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t *data);

    // Text rendering - value returned is the pixel width of the rendered text
    int16_t  drawNumber(long intNumber, int32_t x, int32_t y, uint8_t font), // Draw integer using specified font number
             drawNumber(long intNumber, int32_t x, int32_t y),               // Draw integer using current font

             // Decimal is the number of decimal places to render
             // Use with setTextDatum() to position values on TFT, and setTextPadding() to blank old displayed values
             drawFloat(float floatNumber, uint8_t decimal, int32_t x, int32_t y, uint8_t font), // Draw float using specified font number
             drawFloat(float floatNumber, uint8_t decimal, int32_t x, int32_t y),               // Draw float using current font

             // Handle char arrays
             // Use with setTextDatum() to position string on TFT, and setTextPadding() to blank old displayed strings
             drawString(const char *string, int32_t x, int32_t y, uint8_t font),  // Draw string using specifed font number
             drawString(const char *string, int32_t x, int32_t y),                // Draw string using current font
             drawString(const String &string, int32_t x, int32_t y, uint8_t font),// Draw string using specifed font number
             drawString(const String &string, int32_t x, int32_t y),              // Draw string using current font

             drawCentreString(const char *string, int32_t x, int32_t y, uint8_t font),  // Deprecated, use setTextDatum() and drawString()
             drawRightString(const char *string, int32_t x, int32_t y, uint8_t font),   // Deprecated, use setTextDatum() and drawString()
             drawCentreString(const String &string, int32_t x, int32_t y, uint8_t font),// Deprecated, use setTextDatum() and drawString()
             drawRightString(const String &string, int32_t x, int32_t y, uint8_t font); // Deprecated, use setTextDatum() and drawString()

    // Text rendering and font handling support funtions
    void     setCursor(int16_t x, int16_t y),                 // Set cursor for tft.print()
             setCursor(int16_t x, int16_t y, uint8_t font);   // Set cursor and font number for tft.print()

    int16_t  getCursorX(void),                                // Read current cursor x position (moves with tft.print())
             getCursorY(void);                                // Read current cursor y position

    void     setTextColor(uint16_t color),                    // Set character (glyph) color only (background not over-written)
             setTextColor(uint16_t fgcolor, uint16_t bgcolor),// Set character (glyph) foreground and backgorund colour
             setTextSize(uint8_t size);                       // Set character size multiplier (this increases pixel size)

    void     setTextWrap(bool wrapX, bool wrapY = false);     // Turn on/off wrapping of text in TFT width and/or height

    void     setTextDatum(uint8_t datum);                     // Set text datum position (default is top left), see Section 6 above
    uint8_t  getTextDatum(void);

    void     setTextPadding(uint16_t x_width);                // Set text padding (background blanking/over-write) width in pixels
    uint16_t getTextPadding(void);                            // Get text padding

#ifdef LOAD_GFXFF
    void     setFreeFont(const GFXfont *f = NULL),            // Select the GFX Free Font
             setTextFont(uint8_t font);                       // Set the font number to use in future
#else
    void     setFreeFont(uint8_t font),                       // Not used, historical fix to prevent an error
             setTextFont(uint8_t font);                       // Set the font number to use in future
#endif

    int16_t  textWidth(const char *string, uint8_t font),     // Returns pixel width of string in specified font
             textWidth(const char *string),                   // Returns pixel width of string in current font
             textWidth(const String &string, uint8_t font),   // As above for String types
             textWidth(const String &string),
             fontHeight(int16_t font),                        // Returns pixel height of string in specified font
             fontHeight(void);                                // Returns pixel width of string in current font

    // Used by library and Smooth font class to extract Unicode point codes from a UTF8 encoded string
    uint16_t decodeUTF8(uint8_t *buf, uint16_t *index, uint16_t remaining),
             decodeUTF8(uint8_t c);

    // Support function to UTF8 decode and draw characters piped through print stream
    size_t   write(uint8_t);

    // Used by Smooth font class to fetch a pixel colour for the anti-aliasing
    void     setCallback(getColorCallback getCol);

    uint16_t fontsLoaded(void); // Each bit in returned value represents a font type that is loaded - used for debug/error handling only

    // Low level read/write
    void     spiwrite(uint8_t);        // legacy support only

    void     writecommand(uint8_t c),  // Send a command, function resets DC/RS high ready for data
             writedata(uint8_t d);     // Send data with DC/RS set high

    void     commandList(const uint8_t *addr); // Send a initialisation sequence to TFT stored in FLASH

    uint8_t  readcommand8( uint8_t cmd_function, uint8_t index = 0); // read 8 bits from TFT
    uint16_t readcommand16(uint8_t cmd_function, uint8_t index = 0); // read 16 bits from TFT
    uint32_t readcommand32(uint8_t cmd_function, uint8_t index = 0); // read 32 bits from TFT


    // Colour conversion
    // Convert 8 bit red, green and blue to 16 bits
    uint16_t color565(uint8_t red, uint8_t green, uint8_t blue);

    // Convert 8 bit colour to 16 bits
    uint16_t color8to16(uint8_t color332);
    // Convert 16 bit colour to 8 bits
    uint8_t  color16to8(uint16_t color565);

    // Convert 16 bit colour to/from 24 bit, R+G+B concatenated into LS 24 bits
    uint32_t color16to24(uint16_t color565);
    uint32_t color24to16(uint32_t color888);

    // Alpha blend 2 colours, see generic "alphaBlend_Test" example
    // alpha =   0 = 100% background colour
    // alpha = 255 = 100% foreground colour
    uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);
    // 16 bit colour alphaBlend with alpha dither (dither reduces colour banding)
    uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither);
    // 24 bit colour alphaBlend with optional alpha dither
    uint32_t alphaBlend24(uint8_t alpha, uint32_t fgc, uint32_t bgc, uint8_t dither = 0);


    // DMA support functions - these are currently just for SPI writes whe using the STM32 processors
    // Bear in mind DMA will only be of benefit in particular circumstances and can be tricky
    // to manage by noobs. The functions have however been designed to be noob friendly and
    // avoid a few DMA behaviour "gotchas".
    //
    // At best you will get a 2x TFT rendering performance improvement when using DMA because
    // this library handles the SPI bus so efficiently during normal (non DMA) transfers. The best
    // performance improvement scenario is the DMA transfer time is exactly the same as the time it
    // takes for the processor to prepare the next image buffer and initiate another DMA transfer.
    //
    // DMA transfer to the TFT is done while the processor moves on to handle other tasks. Bear
    // this in mind and watch out for "gotchas" like the image buffer going out of scope as the
    // processor leaves a function or its content being changed while the DMA engine is reading it.
    //
    // The compiler MAY change the implied scope of a buffer which has been set aside by creating
    // and an array. For example a buffer defined before a "for-next" loop may get de-allocated when
    // the loop ends. To avoid this use, for example, malloc() and free() to take control of when
    // the buffer space is available and ensure it is not released until DMA is complete.
    //
    // Clearly you should not modify a buffer that is being DMA'ed to the TFT until the DMA is over.
    // Use the dmaBusy() function to check this.  Use tft.startWrite() before invoking DMA so the
    // TFT chip select stays low. If you use tft.endWrite() before DMA is complete then the endWrite
    // function will wait for the DMA to complete, so this may defeat any DMA performance benefit.
    //

    bool     initDMA(void);     // Initialise the DMA engine and attach to SPI bus - typically used in setup()
    void     deInitDMA(void);   // De-initialise the DMA engine and detach from SPI bus - typically not used

    // Push an image to the TFT using DMA, buffer is optional and grabs (double buffers) a copy of the image
    // Use the buffer if the image data will get over-written or destroyed while DMA is in progress
    // If swapping colour bytes is defined, and the double buffer option is NOT used then the bytes
    // in the original data image will be swapped by the function before DMA is initiated.
    // The function will wait for the last DMA to complete if it is called while a previous DMA is still
    // in progress, this simplifies the sketch and helps avoid "gotchas".
    void     pushImageDMA(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, uint16_t *buffer = nullptr);

    // Push a block of pixels into a window set up using setAddrWindow()
    void     pushPixelsDMA(uint16_t *image, uint32_t len);

    // Check if the DMA is complete - use while(tft.dmaBusy); for a blocking wait
    bool     dmaBusy(void); // returns true if DMA is still in progress
    void     dmaWait(void); // wait until DMA is complete

    bool     DMA_Enabled = false;   // Flag for DMA enabled state
    uint8_t  spiBusyCheck = 0;      // Number of ESP32 transfer buffers to check

    // Bare metal functions
    void     startWrite(void);                         // Begin SPI transaction
    void     writeColor(uint16_t color, uint32_t len); // Deprecated, use pushBlock()
    void     endWrite(void);                           // End SPI transaction

    // Set/get an arbitrary library configuration attribute or option
    //       Use to switch ON/OFF capabilities such as UTF8 decoding - each attribute has a unique ID
    //       id = 0: reserved - may be used in fuuture to reset all attributes to a default state
    //       id = 1: Turn on (a=true) or off (a=false) GLCD cp437 font character error correction
    //       id = 2: Turn on (a=true) or off (a=false) UTF8 decoding
    //       id = 3: Enable or disable use of ESP32 PSRAM (if available)
#define CP437_SWITCH 1
#define UTF8_SWITCH  2
#define PSRAM_ENABLE 3
    void     setAttribute(uint8_t id = 0, uint8_t a = 0); // Set attribute value
    uint8_t  getAttribute(uint8_t id = 0);                // Get attribute value

    // Used for diagnostic sketch to see library setup adopted by compiler, see Section 7 above
    void     getSetup(setup_t &tft_settings); // Sketch provides the instance to populate

    // Global variables
    static   SPIClass &getSPIinstance(void); // Get SPI class handle

    int32_t  cursor_x, cursor_y, padX;       // Text cursor x,y and padding setting
    uint32_t textcolor, textbgcolor;         // Text foreground and background colours

    uint32_t bitmap_fg, bitmap_bg;           // Bitmap foreground (bit=1) and background (bit=0) colours

    uint8_t  textfont,  // Current selected font number
             textsize,  // Current font size multiplier
             textdatum, // Text reference datum
             rotation;  // Display rotation (0-3)

    int16_t  _xpivot;   // TFT x pivot point coordinate for rotated Sprites
    int16_t  _ypivot;   // TFT x pivot point coordinate for rotated Sprites

    uint8_t  decoderState = 0;   // UTF8 decoder state        - not for user access
    uint16_t decoderBuffer;      // Unicode code-point buffer - not for user access

//--------------------------------------- private ------------------------------------//
private:
    // Legacy begin and end prototypes - deprecated TODO: delete
    void     spi_begin();
    void     spi_end();

    void     spi_begin_read();
    void     spi_end_read();

    // New begin and end prototypes
    // begin/end a TFT write transaction
    // For SPI bus the transmit clock rate is set
    inline void begin_tft_write()      __attribute__((always_inline));
    inline void end_tft_write()        __attribute__((always_inline));

    // begin/end a TFT read transaction
    // For SPI bus: begin lowers SPI clock rate, end reinstates transmit clock rate
    inline void begin_tft_read() __attribute__((always_inline));
    inline void end_tft_read()   __attribute__((always_inline));

    // Temporary  library development function  TODO: remove need for this
    void     pushSwapBytePixels(const void *data_in, uint32_t len);

    // Same as setAddrWindow but exits with CGRAM in read mode
    void     readAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h);

    // Byte read prototype
    uint8_t  readByte(void);

    // GPIO parallel bus input/output direction control
    void     busDir(uint32_t mask, uint8_t mode);

    // Single GPIO input/output direction control
    void     gpioMode(uint8_t gpio, uint8_t mode);

    // Display variant settings
    uint8_t  tabcolor,                   // ST7735 screen protector "tab" colour (now invalid)
             colstart = 0, rowstart = 0; // Screen display area to CGRAM area coordinate offsets

    // Port and pin masks for control signals (ESP826 only) - TODO: remove need for this
    volatile uint32_t *dcport, *csport;
    uint32_t cspinmask, dcpinmask, wrpinmask, sclkpinmask;

#if defined(ESP32_PARALLEL)
    // Bit masks for ESP32 parallel bus interface
    uint32_t xclr_mask, xdir_mask; // Port set/clear and direction control masks

    // Lookup table for ESP32 parallel bus interface uses 1kbyte RAM,
    uint32_t xset_mask[256]; // Makes Sprite rendering test 33% faster, for slower macro equivalent
    // see commented out #define set_mask(C) within TFT_eSPI_ESP32.h
#endif

    //uint32_t lastColor = 0xFFFF; // Last colour - used to minimise bit shifting overhead

    getColorCallback getColor = nullptr; // Smooth font callback function pointer

//-------------------------------------- protected ----------------------------------//
protected:

    //int32_t  win_xe, win_ye;          // Window end coords - not needed

    int32_t  _init_width, _init_height; // Display w/h as input, used by setRotation()
    int32_t  _width, _height;           // Display w/h as modified by current rotation
    int32_t  addr_row, addr_col;        // Window position - used to minimise window commands

    uint32_t fontsloaded;               // Bit field of fonts loaded

    uint8_t  glyph_ab,   // Smooth font glyph delta Y (height) above baseline
             glyph_bb;   // Smooth font glyph delta Y (height) below baseline

    bool     isDigits;   // adjust bounding box for numbers to reduce visual jiggling
    bool     textwrapX, textwrapY;  // If set, 'wrap' text at right and optionally bottom edge of display
    bool     _swapBytes; // Swap the byte order for TFT pushImage()
    bool     locked, inTransaction; // SPI transaction and mutex lock flags

    bool     _booted;    // init() or begin() has already run once

    // User sketch manages these via set/getAttribute()
    bool     _cp437;        // If set, use correct CP437 charset (default is ON)
    bool     _utf8;         // If set, use UTF-8 decoder in print stream 'write()' function (default ON)
    bool     _psram_enable; // Enable PSRAM use for library functions (TBD) and Sprites

    uint32_t _lastColor; // Buffered value of last colour used

#ifdef LOAD_GFXFF
    GFXfont  *gfxFont;
#endif

    /***************************************************************************************
    **                         Section 9: TFT_eSPI class conditional extensions
    ***************************************************************************************/
// Load the Touch extension


// Load the Anti-aliased font extension
#ifdef SMOOTH_FONT
// Coded by Bodmer 10/2/18, see license in root directory.
// This is part of the TFT_eSPI class and is associated with anti-aliased font functions

public:

    // These are for the new antialiased fonts
    void     loadFont(const uint8_t array[]);
#ifdef FONT_FS_AVAILABLE
    void     loadFont(String fontName, fs::FS &ffs);
#endif
    void     loadFont(String fontName, bool flash = true);
    void     unloadFont( void );
    bool     getUnicodeIndex(uint16_t unicode, uint16_t *index);

    virtual void drawGlyph(uint16_t code);

    void     showFont(uint32_t td);

// This is for the whole font
    typedef struct {
        const uint8_t *gArray;           //array start pointer
        uint16_t gCount;                 // Total number of characters
        uint16_t yAdvance;               // Line advance
        uint16_t spaceWidth;             // Width of a space character
        int16_t  ascent;                 // Height of top of 'd' above baseline, other characters may be taller
        int16_t  descent;                // Offset to bottom of 'p', other characters may have a larger descent
        uint16_t maxAscent;              // Maximum ascent found in font
        uint16_t maxDescent;             // Maximum descent found in font
    } fontMetrics;

    fontMetrics gFont = { nullptr, 0, 0, 0, 0, 0, 0, 0 };

    // These are for the metrics for each individual glyph (so we don't need to seek this in file and waste time)
    uint16_t *gUnicode = NULL;  //UTF-16 code, the codes are searched so do not need to be sequential
    uint8_t  *gHeight = NULL;   //cheight
    uint8_t  *gWidth = NULL;    //cwidth
    uint8_t  *gxAdvance = NULL; //setWidth
    int16_t  *gdY = NULL;       //topExtent
    int8_t   *gdX = NULL;       //leftExtent
    uint32_t *gBitmap = NULL;   //file pointer to greyscale bitmap

    bool     fontLoaded = false; // Flags when a anti-aliased font is loaded

#ifdef FONT_FS_AVAILABLE
    fs::File fontFile;
    fs::FS   &fontFS  = SPIFFS;
    bool     spiffs   = true;
    bool     fs_font = false;    // For ESP32/8266 use smooth font file or FLASH (PROGMEM) array

#else
    bool     fontFile = true;
#endif

private:

    void     loadMetrics(void);
    uint32_t readInt32(void);

    uint8_t *fontPtr = nullptr;


#endif

}; // End of class TFT_eSPI

/***************************************************************************************
**                         Section 10: Additional extension classes
***************************************************************************************/

// Load the Sprite Class
/***************************************************************************************
// The following class creates Sprites in RAM, graphics can then be drawn in the Sprite
// and rendered quickly onto the TFT screen. The class inherits the graphics functions
// from the TFT_eSPI class. Some functions are overridden by this class so that the
// graphics are written to the Sprite rather than the TFT.
***************************************************************************************/

class TFT_eSprite : public TFT_eSPI
{

public:

    TFT_eSprite(TFT_eSPI *tft);

    // Create a sprite of width x height pixels, return a pointer to the RAM area
    // Sketch can cast returned value to (uint16_t*) for 16 bit depth if needed
    // RAM required is:
    //  - 1 bit per pixel for 1 bit colour depth
    //  - 1 nibble per pixel for 4 bit colour
    //  - 1 byte per pixel for 8 bit colour
    //  - 2 bytes per pixel for 16 bit color depth
    ~TFT_eSprite(void);

    void    *createSprite(int16_t width, int16_t height, uint8_t frames = 1);

    // Returns true if sprite has been created
    bool     created(void);

    // Delete the sprite to free up the RAM
    void     deleteSprite(void);

    // Select the frame buffer for graphics write (for 2 colour ePaper and DMA toggle buffer)
    // Returns a pointer to the Sprite frame buffer
    void    *frameBuffer(int8_t f);

    // Set or get the colour depth to 4, 8 or 16 bits. Can be used to change depth an existing
    // sprite, but clears it to black, returns a new pointer if sprite is re-created.
    void    *setColorDepth(int8_t b);
    int8_t   getColorDepth(void);

    // Set the palette for a 4 bit depth sprite.  Only the first 16 colours in the map are used.
    void     createPalette(uint16_t *palette = nullptr, uint8_t colors = 16);       // Palette in RAM
    void     createPalette(const uint16_t *palette = nullptr, uint8_t colors = 16); // Palette in FLASH

    // Set a single palette index to the given color
    void     setPaletteColor(uint8_t index, uint16_t color);

    // Get the color at the given palette index
    uint16_t getPaletteColor(uint8_t index);

    // Set foreground and background colours for 1 bit per pixel Sprite
    void     setBitmapColor(uint16_t fg, uint16_t bg);

    void     drawPixel(int32_t x, int32_t y, uint32_t color);

    void     drawChar(int32_t x, int32_t y, uint16_t c, uint32_t color, uint32_t bg, uint8_t font),

             // Fill Sprite with a colour
             fillSprite(uint32_t color),

             // Define a window to push 16 bit colour pixels into in a raster order
             // Colours are converted to the set Sprite colour bit depth
             setWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1),
             // Push a color (aka singe pixel) to the screen
             pushColor(uint32_t color),
             // Push len colors (pixels) to the screen
             pushColor(uint32_t color, uint16_t len),
             // Push a pixel preformatted as a 8 or 16 bit colour (avoids conversion overhead)
             writeColor(uint16_t color),

             // Set the scroll zone, top left corner at x,y with defined width and height
             // The colour (optional, black is default) is used to fill the gap after the scroll
             setScrollRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color = TFT_BLACK),
             // Scroll the defined zone dx,dy pixels. Negative values left,up, positive right,down
             // dy is optional (default is then no up/down scroll).
             // The sprite coordinate frame does not move because pixels are moved
             scroll(int16_t dx, int16_t dy = 0),

             // Draw lines
             drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color),
             drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color),
             drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color),

             // Fill a rectangular area with a color (aka draw a filled rectangle)
             fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);

    // Set the sprite text cursor position for print class (does not change the TFT screen cursor)
    //setCursor(int16_t x, int16_t y); // Not needed, so uses TFT class function

    // Set the coordinate rotation of the Sprite (for 1bpp Sprites only)
    // Note: this uses coordinate rotation and is primarily for ePaper which does not support
    // CGRAM rotation (like TFT drivers do) within the displays internal hardware
    void     setRotation(uint8_t rotation);
    uint8_t  getRotation(void);

    // Push a rotated copy of Sprite to TFT with optional transparent colour
    bool     pushRotated(int16_t angle, int32_t transp = -1);   // Using fixed point maths
    // Push a rotated copy of Sprite to another different Sprite with optional transparent colour
    bool     pushRotated(TFT_eSprite *spr, int16_t angle, int32_t transp = -1);   // Using fixed point maths

    // Set and get the pivot point for this Sprite
    void     setPivot(int16_t x, int16_t y);
    int16_t  getPivotX(void),
             getPivotY(void);

    // Get the TFT bounding box for a rotated copy of this Sprite
    bool     getRotatedBounds(int16_t angle, int16_t *min_x, int16_t *min_y, int16_t *max_x, int16_t *max_y);
    // Get the destination Sprite bounding box for a rotated copy of this Sprite
    bool     getRotatedBounds(TFT_eSprite *spr, int16_t angle, int16_t *min_x, int16_t *min_y,
                              int16_t *max_x, int16_t *max_y);
    // Bounding box support function
    void     getRotatedBounds(int16_t angle, int16_t w, int16_t h, int16_t xp, int16_t yp,
                              int16_t *min_x, int16_t *min_y, int16_t *max_x, int16_t *max_y);

    // Read the colour of a pixel at x,y and return value in 565 format
    uint16_t readPixel(int32_t x0, int32_t y0);

    // return the numerical value of the pixel at x,y (used when scrolling)
    // 16bpp = colour, 8bpp = byte, 4bpp = colour index, 1bpp = 1 or 0
    uint16_t readPixelValue(int32_t x, int32_t y);

    // Write an image (colour bitmap) to the sprite.
    void     pushImage(int32_t x0, int32_t y0, int32_t w, int32_t h, uint16_t *data);
    void     pushImage(int32_t x0, int32_t y0, int32_t w, int32_t h, const uint16_t *data);

    // Swap the byte order for pushImage() - corrects different image endianness
    void     setSwapBytes(bool swap);
    bool     getSwapBytes(void);

    // Push the sprite to the TFT screen, this fn calls pushImage() in the TFT class.
    // Optionally a "transparent" colour can be defined, pixels of that colour will not be rendered
    void     pushSprite(int32_t x, int32_t y);
    void     pushSprite(int32_t x, int32_t y, uint16_t transparent);

    int16_t  drawChar(uint16_t uniCode, int32_t x, int32_t y, uint8_t font),
             drawChar(uint16_t uniCode, int32_t x, int32_t y);

    // Return the width and height of the sprite
    int16_t  width(void),
             height(void);

    // Used by print class to print text to cursor position
    size_t   write(uint8_t);

    // Functions associated with anti-aliased fonts
    void     drawGlyph(uint16_t code);
    void     printToSprite(String string);
    void     printToSprite(char *cbuffer, uint16_t len);
    int16_t  printToSprite(int16_t x, int16_t y, uint16_t index);

private:

    TFT_eSPI *_tft;

    // Reserve memory for the Sprite and return a pointer
    void    *callocSprite(int16_t width, int16_t height, uint8_t frames = 1);

protected:

    uint8_t  _bpp;     // bits per pixel (1, 8 or 16)
    uint16_t *_img;    // pointer to 16 bit sprite
    uint8_t  *_img8;   // pointer to  8 bit sprite
    uint8_t  *_img4;   // pointer to 4 bit sprite (uses color map)
    uint8_t  *_img8_1; // pointer to  frame 1
    uint8_t  *_img8_2; // pointer to  frame 2

    uint16_t *_colorMap; // color map: 16 entries, used with 4 bit color map.

    int16_t  _xpivot;   // x pivot point coordinate
    int16_t  _ypivot;   // y pivot point coordinate
    int32_t  _sinra;
    int32_t  _cosra;

    bool     _created;    // A Sprite has been created and memory reserved
    bool     _gFont = false;

//  int32_t  _icursor_x, _icursor_y;
    uint8_t  _rotation = 0;
    int32_t  _xs, _ys, _xe, _ye, _xptr, _yptr; // for setWindow
    int32_t  _sx, _sy; // x,y for scroll zone
    uint32_t _sw, _sh; // w,h for scroll zone
    uint32_t _scolor;  // gap fill colour for scroll zone

    bool     _iswapBytes; // Swap the byte order for Sprite pushImage()

    int32_t  _iwidth, _iheight; // Sprite memory image bit width and height (swapped during rotations)
    int32_t  _dwidth, _dheight; // Real display width and height (for <8bpp Sprites)
    int32_t  _bitwidth;         // Sprite image bit width for drawPixel (for <8bpp Sprites, not swapped)

};


#endif // ends #ifndef _TFT_eSPIH_
