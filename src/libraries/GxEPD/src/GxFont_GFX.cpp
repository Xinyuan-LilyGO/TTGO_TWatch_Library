// class GxFont_GFX : Font Rendering Graphics Switch and Bridge Class
//
// This class allows to connect GxEPD to additional font rendering classes.
//
// U8G2_FOR_ADAFRUIT_GFX: Arduino Library that makes all U8G2 fonts available (Oliver Kraus)
// avaliable from: https://github.com/olikraus/U8g2_for_Adafruit_GFX
//
// GxFont_GFX_TFT_eSPI: fonts and font rendering of TFT_eSPI library (Bodmer)
// available here: https://github.com/ZinggJM/GxFont_GFX_TFT_eSPI
//
// Adafruit_ftGFX: a Adafruit_GFX variant with different fonts.
// need to use modified clone from: https://github.com/ZinggJM/Adafruit_ftGFX
// (no additional fonts, as all are now part of Adafruit_GFX)
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD

#include "GxFont_GFX.h"

//#define DIAG_UTF8(x) x
//#define DIAG(x) x

#ifndef DIAG_UTF8
#define DIAG_UTF8(x)
#endif
#ifndef DIAG
#define DIAG(x)
#endif

enum eFont_GFX
{
  U8g2_for_Adafruit_GFX_font_gfx, Adafruit_GFX_font_gfx, Adafruit_ftGFX_font_gfx, GxFont_GFX_TFT_eSPI_font_gfx
};

GxFont_GFX::GxFont_GFX(int16_t w, int16_t h) : Adafruit_GFX(w, h)
#if defined(U8g2_for_Adafruit_GFX_h)
  , _U8G2_FONTS_GFX(*this)
#endif
#if defined(_ADAFRUIT_TF_GFX_H_)
  , _GxF_Adafruit_ftGFX(*this, w, h)
#endif
#if defined(_GxFont_GFX_TFT_eSPI_H_)
  , _GxF_GxFont_GFX_TFT_eSPI(*this, w, h)
#endif
{
  _font_gfx = Adafruit_GFX_font_gfx;
}

void GxFont_GFX::setFont(const GFXfont *f)
{
  _font_gfx = Adafruit_GFX_font_gfx;
  Adafruit_GFX::setFont(f);
}

#if defined(U8g2_for_Adafruit_GFX_h)

void GxFont_GFX::setFont(const uint8_t *font)
{
  _font_gfx = U8g2_for_Adafruit_GFX_font_gfx;
  _U8G2_FONTS_GFX.setFont(font);
}

#endif

#if defined(_ADAFRUIT_TF_GFX_H_)

void GxFont_GFX::GxF_Adafruit_ftGFX::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  _container.drawPixel(x, y, color);
}

void GxFont_GFX::setFont(uint8_t f)
{
  _font_gfx = Adafruit_ftGFX_font_gfx;
  _GxF_Adafruit_ftGFX.setFont(f);
}

#endif

#if defined(_GxFont_GFX_TFT_eSPI_H_)

void GxFont_GFX::GxF_GxFont_GFX_TFT_eSPI::drawPixel(uint32_t x, uint32_t y, uint32_t color)
{
  //Serial.print("GxFont_GFX::GxF_GxFont_GFX_TFT_eSPI::drawPixel("); Serial.print(color); Serial.println(")"); yield();
  _container.drawPixel(x, y, color);
}

void GxFont_GFX::GxF_GxFont_GFX_TFT_eSPI::drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color)
{
  //Serial.print("GxFont_GFX::GxF_GxFont_GFX_TFT_eSPI::drawFastHLine("); Serial.print(color); Serial.println(")"); yield();
  _container.drawFastHLine(x, y, w, color);
}

void GxFont_GFX::GxF_GxFont_GFX_TFT_eSPI::fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
{
  //Serial.print("GxFont_GFX::GxF_GxFont_GFX_TFT_eSPI::fillRect("); Serial.print(color); Serial.println(")"); yield();
  _container.fillRect(x, y, w, h, color);
}

#ifdef LOAD_GFXFF
void GxFont_GFX::setFreeFont(const GFXfont *f)
{
  _font_gfx = GxFont_GFX_TFT_eSPI_font_gfx;
  _GxF_GxFont_GFX_TFT_eSPI.setFreeFont(f);
}

void GxFont_GFX::setTextFont(uint8_t font)
{
  _font_gfx = GxFont_GFX_TFT_eSPI_font_gfx;
  _GxF_GxFont_GFX_TFT_eSPI.setTextFont(font);
}

#else

void GxFont_GFX::setFreeFont(uint8_t font)
{
  _font_gfx = GxFont_GFX_TFT_eSPI_font_gfx;
  _GxF_GxFont_GFX_TFT_eSPI.setFreeFont(font);
}

void GxFont_GFX::setTextFont(uint8_t font)
{
  _font_gfx = GxFont_GFX_TFT_eSPI_font_gfx;
  _GxF_GxFont_GFX_TFT_eSPI.setTextFont(font);
}

#endif

#endif

#if defined(U8g2_for_Adafruit_GFX_h) || defined(_GxFont_GFX_TFT_eSPI_H_)|| defined(_ADAFRUIT_TF_GFX_H_)

void GxFont_GFX::setCursor(int16_t x, int16_t y)
{
  Adafruit_GFX::setCursor(x, y);
#if defined(U8g2_for_Adafruit_GFX_h)
  _U8G2_FONTS_GFX.setCursor(x, y);
#endif
#if defined(_ADAFRUIT_TF_GFX_H_)
  _GxF_Adafruit_ftGFX.setCursor(x, y);
#endif
#if defined(_GxFont_GFX_TFT_eSPI_H_)
  _GxF_GxFont_GFX_TFT_eSPI.setCursor(x, y);
#endif
}

size_t GxFont_GFX::write(uint8_t v)
{
  DIAG (Serial.write(v); Serial.println();)
  DIAG_UTF8(if (v > 127) Serial.println(v, HEX);)
    switch (_font_gfx)
    {
      case Adafruit_GFX_font_gfx:
        Adafruit_GFX::write(v);
        break;
#if defined(U8g2_for_Adafruit_GFX_h)
      case U8g2_for_Adafruit_GFX_font_gfx:
        _U8G2_FONTS_GFX.write(v);
        break;
#endif
#if defined(_ADAFRUIT_TF_GFX_H_)
      case Adafruit_ftGFX_font_gfx:
        _GxF_Adafruit_ftGFX.write(v);
        break;
#endif
#if defined(_GxFont_GFX_TFT_eSPI_H_)
      case GxFont_GFX_TFT_eSPI_font_gfx:
        _GxF_GxFont_GFX_TFT_eSPI.write(v);
        break;
#endif
    }
    return 0;
}

int16_t GxFont_GFX::getCursorX(void) const
{
  switch (_font_gfx)
  {
    case Adafruit_GFX_font_gfx:
      return Adafruit_GFX::getCursorX();
#if defined(U8g2_for_Adafruit_GFX_h)
    case U8g2_for_Adafruit_GFX_font_gfx:
      return const_cast<U8G2_FONTS_GFX&>(_U8G2_FONTS_GFX).getCursorX();
#endif
#if defined(_ADAFRUIT_TF_GFX_H_)
    case Adafruit_ftGFX_font_gfx:
      return _GxF_Adafruit_ftGFX.getCursorX();
#endif
#if defined(_GxFont_GFX_TFT_eSPI_H_)
    case GxFont_GFX_TFT_eSPI_font_gfx:
      return _GxF_GxFont_GFX_TFT_eSPI.getCursorX();
#endif
  }
  return 0;
}

int16_t GxFont_GFX::getCursorY(void) const
{
  switch (_font_gfx)
  {
    case Adafruit_GFX_font_gfx:
      return Adafruit_GFX::getCursorY();
#if defined(U8g2_for_Adafruit_GFX_h)
    case U8g2_for_Adafruit_GFX_font_gfx:
      return const_cast<U8G2_FONTS_GFX&>(_U8G2_FONTS_GFX).getCursorY();
#endif
#if defined(_ADAFRUIT_TF_GFX_H_)
    case Adafruit_ftGFX_font_gfx:
      return _GxF_Adafruit_ftGFX.getCursorY();
#endif
#if defined(_GxFont_GFX_TFT_eSPI_H_)
    case GxFont_GFX_TFT_eSPI_font_gfx:
      return _GxF_GxFont_GFX_TFT_eSPI.getCursorY();
#endif
  }
  return 0;
}

#endif

#if defined(U8g2_for_Adafruit_GFX_h)

void GxFont_GFX::home(void)
{
  _U8G2_FONTS_GFX.home();
}

void GxFont_GFX::setFontMode(uint8_t is_transparent)
{
  _U8G2_FONTS_GFX.setFontMode(is_transparent);
}

void GxFont_GFX::setFontDirection(uint8_t d)
{
  _U8G2_FONTS_GFX.setFontDirection(d);
}

void GxFont_GFX::setForegroundColor(uint16_t fg)
{
  _U8G2_FONTS_GFX.setForegroundColor(fg);
}

void GxFont_GFX::setBackgroundColor(uint16_t bg)
{
  _U8G2_FONTS_GFX.setBackgroundColor(bg);
}

int8_t GxFont_GFX::getFontAscent(void)
{
  return ((_font_gfx == U8g2_for_Adafruit_GFX_font_gfx) ? _U8G2_FONTS_GFX.getFontAscent() : 0);
}

int8_t GxFont_GFX::getFontDescent(void)
{
  return ((_font_gfx == U8g2_for_Adafruit_GFX_font_gfx) ? _U8G2_FONTS_GFX.getFontDescent() : 0);
}

int8_t GxFont_GFX::getFontHeight(void)
{
  return ((_font_gfx == U8g2_for_Adafruit_GFX_font_gfx) ? _U8G2_FONTS_GFX.u8g2.font_info.max_char_height : 0);
}

int16_t GxFont_GFX::drawGlyph(int16_t x, int16_t y, uint16_t e)
{
  return ((_font_gfx == U8g2_for_Adafruit_GFX_font_gfx) ? _U8G2_FONTS_GFX.drawGlyph(x, y, e) : 0);
}

int16_t GxFont_GFX::drawStr(int16_t x, int16_t y, const char *s)
{
  return ((_font_gfx == U8g2_for_Adafruit_GFX_font_gfx) ? _U8G2_FONTS_GFX.drawStr(x, y, s) : 0);
}

int16_t GxFont_GFX::drawUTF8(int16_t x, int16_t y, const char *str)
{
  return ((_font_gfx == U8g2_for_Adafruit_GFX_font_gfx) ? _U8G2_FONTS_GFX.drawUTF8(x, y, str) : 0);
}

int16_t GxFont_GFX::getUTF8Width(const char *str)
{
  return ((_font_gfx == U8g2_for_Adafruit_GFX_font_gfx) ? _U8G2_FONTS_GFX.getUTF8Width(str) : 0);
}

uint16_t GxFont_GFX::utf8_next(uint8_t b)
{
  return ((_font_gfx == U8g2_for_Adafruit_GFX_font_gfx) ? _U8G2_FONTS_GFX.utf8_next(b) : 0);
}

#endif

#if defined(_ADAFRUIT_TF_GFX_H_) || defined(_GxFont_GFX_TFT_eSPI_H_)

void GxFont_GFX::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{
  switch (_font_gfx)
  {
    case Adafruit_GFX_font_gfx:
      Adafruit_GFX::drawChar(x, y, c, color, bg, size);
      break;
#if defined(_ADAFRUIT_TF_GFX_H_)
    case Adafruit_ftGFX_font_gfx:
      _GxF_Adafruit_ftGFX.drawChar(x, y, c, color, bg, size);
      break;
#endif
#if defined(_GxFont_GFX_TFT_eSPI_H_)
    case GxFont_GFX_TFT_eSPI_font_gfx:
      _GxF_GxFont_GFX_TFT_eSPI.drawChar(x, y, c, color, bg, size);
      break;
#endif
  }
}

void GxFont_GFX::setTextColor(uint16_t c)
{
  Adafruit_GFX::setTextColor(c);
#if defined(_ADAFRUIT_TF_GFX_H_)
  _GxF_Adafruit_ftGFX.setTextColor(c);
#endif
#if defined(_GxFont_GFX_TFT_eSPI_H_)
  _GxF_GxFont_GFX_TFT_eSPI.setTextColor(c);
#endif
}

void GxFont_GFX::setTextColor(uint16_t c, uint16_t bg)
{
  Adafruit_GFX::setTextColor(c, bg);
#if defined(_ADAFRUIT_TF_GFX_H_)
  _GxF_Adafruit_ftGFX.setTextColor(c, bg);
#endif
#if defined(_GxFont_GFX_TFT_eSPI_H_)
  _GxF_GxFont_GFX_TFT_eSPI.setTextColor(c, bg);
#endif
}

void GxFont_GFX::setTextSize(uint8_t s)
{
  Adafruit_GFX::setTextSize(s);
#if defined(_ADAFRUIT_TF_GFX_H_)
  _GxF_Adafruit_ftGFX.setTextSize(s);
#endif
#if defined(_GxFont_GFX_TFT_eSPI_H_)
  _GxF_GxFont_GFX_TFT_eSPI.setTextSize(s);
#endif
}

void GxFont_GFX::setTextWrap(boolean w)
{
  Adafruit_GFX::setTextWrap(w);
#if defined(_ADAFRUIT_TF_GFX_H_)
  _GxF_Adafruit_ftGFX.setTextWrap(w);
#endif
#if defined(_GxFont_GFX_TFT_eSPI_H_)
  _GxF_GxFont_GFX_TFT_eSPI.setTextWrap(w);
#endif
}

#endif

#if defined(_GxFont_GFX_TFT_eSPI_H_)
void GxFont_GFX::setTextDatum(uint8_t datum)
{
  if (_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) _GxF_GxFont_GFX_TFT_eSPI.setTextDatum(datum);
}
void GxFont_GFX::setTextPadding(uint16_t x_width)
{
  if (_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) _GxF_GxFont_GFX_TFT_eSPI.setTextPadding(x_width);
}
uint8_t GxFont_GFX::getTextDatum()
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.getTextDatum() : 0);
}
uint8_t GxFont_GFX::color16to8(uint16_t color565) // Convert 16 bit colour to 8 bits
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.color16to8(color565) : 0);
}
uint16_t GxFont_GFX::fontsLoaded(void)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.fontsLoaded() : 0);
}
uint16_t GxFont_GFX::color565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.color565(r, g, b) : 0);
}
uint16_t GxFont_GFX::color8to16(uint8_t color332)  // Convert 8 bit colour to 16 bits
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.color8to16(color332) : 0);
}
int16_t GxFont_GFX::drawNumber(long long_num, int poX, int poY, int font)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.drawNumber(long_num, poX, poY, font) : 0);
}
int16_t GxFont_GFX::drawNumber(long long_num, int poX, int poY)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.drawNumber(long_num, poX, poY) : 0);
}
int16_t GxFont_GFX::drawFloat(float floatNumber, int decimal, int poX, int poY, int font)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.drawFloat(floatNumber, decimal, poX, poY, font) : 0);
}
int16_t GxFont_GFX::drawFloat(float floatNumber, int decimal, int poX, int poY)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.drawFloat(floatNumber, decimal, poX, poY) : 0);
}
// Handle char arrays
int16_t GxFont_GFX::drawString(const char *string, int poX, int poY, int font)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.drawString(string, poX, poY, font) : 0);
}
int16_t GxFont_GFX::drawString(const char *string, int poX, int poY)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.drawString(string, poX, poY) : 0);
}
// Handle String type
int16_t GxFont_GFX::drawString(const String& string, int poX, int poY, int font)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.drawString(string, poX, poY, font) : 0);
}
int16_t GxFont_GFX::drawString(const String& string, int poX, int poY)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.drawString(string, poX, poY) : 0);
}
int16_t GxFont_GFX::textWidth(const char *string, int font)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.textWidth(string, font) : 0);
}
int16_t GxFont_GFX::textWidth(const char *string)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.textWidth(string) : 0);
}
int16_t GxFont_GFX::textWidth(const String& string, int font)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.textWidth(string, font) : 0);
}
int16_t GxFont_GFX::textWidth(const String& string)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.textWidth(string) : 0);
}
int16_t GxFont_GFX::fontHeight(int16_t font)
{
  return ((_font_gfx == GxFont_GFX_TFT_eSPI_font_gfx) ? _GxF_GxFont_GFX_TFT_eSPI.fontHeight(font) : 0);
}
#endif
