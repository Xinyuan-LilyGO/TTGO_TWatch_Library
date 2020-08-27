// class GxGDEW027W3 : Display class for GDEW027W3 e-Paper from Dalian Good Display Co., Ltd.: www.e-paper-display.com
//
// based on Demo Example from Good Display, available here: http://www.e-paper-display.com/download_detail/downloadsId=515.html
// Controller: IL91874 : http://www.e-paper-display.com/download_detail/downloadsId=539.html
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD

#ifndef _GxGDEW027W3_H_
#define _GxGDEW027W3_H_

#include "../GxEPD.h"

#define GxGDEW027W3_WIDTH 176
#define GxGDEW027W3_HEIGHT 264

#define GxGDEW027W3_BUFFER_SIZE (uint32_t(GxGDEW027W3_WIDTH) * uint32_t(GxGDEW027W3_HEIGHT) / 8)

// divisor for AVR, should be factor of GxGDEW027W3_HEIGHT
#define GxGDEW027W3_PAGES 6

#define GxGDEW027W3_PAGE_HEIGHT (GxGDEW027W3_HEIGHT / GxGDEW027W3_PAGES)
#define GxGDEW027W3_PAGE_SIZE (GxGDEW027W3_BUFFER_SIZE / GxGDEW027W3_PAGES)

class GxGDEW027W3 : public GxEPD
{
  public:
#if defined(ESP8266)
    GxGDEW027W3(GxIO& io, int8_t rst = 2, int8_t busy = 4);
#else
    GxGDEW027W3(GxIO& io, int8_t rst = 9, int8_t busy = 7);
#endif
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void init(uint32_t serial_diag_bitrate = 0); // = 0 : disabled
    void fillScreen(uint16_t color); // to buffer
    void update(void);
    // to buffer, may be cropped, drawPixel() used, update needed
    void  drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t mode = bm_invert);
    // to full screen, filled with white if size is less, no update needed
    void drawBitmap(const uint8_t *bitmap, uint32_t size, int16_t mode = bm_normal); // only bm_normal, bm_invert, bm_partial_update modes implemented
    void eraseDisplay(bool using_partial_update = false);
    // partial update of rectangle from buffer to screen, does not power off
    void updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation = true);
    // partial update of rectangle at (xs,ys) from buffer to screen at (xd,yd), does not power off
    void updateToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation = true);
    // terminate cleanly, not needed as all screen drawing methods of this class do power down
    void powerDown();
    // paged drawing, for limited RAM, drawCallback() is called GxGDEW027W3_PAGES times
    // each call of drawCallback() should draw the same
    void drawPaged(void (*drawCallback)(void));
    void drawPaged(void (*drawCallback)(uint32_t), uint32_t);
    void drawPaged(void (*drawCallback)(const void*), const void*);
    void drawPaged(void (*drawCallback)(const void*, const void*), const void*, const void*);
    // paged drawing to screen rectangle at (x,y) using partial update
    void drawPagedToWindow(void (*drawCallback)(void), uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void drawPagedToWindow(void (*drawCallback)(uint32_t), uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t);
    void drawPagedToWindow(void (*drawCallback)(const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void*);
    void drawPagedToWindow(void (*drawCallback)(const void*, const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void*, const void*);
    void drawCornerTest(uint8_t em = 0x01);
  private:
    template <typename T> static inline void
    swap(T& a, T& b)
    {
      T t = a;
      a = b;
      b = t;
    }
    void _writeToWindow(uint8_t command, uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h);
    void _setPartialRamArea(uint8_t command, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void _refreshWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void _writeData(uint8_t data);
    void _writeCommand(uint8_t command);
    void _writeLUT();
    void _wakeUp();
    void _sleep();
    void _Init_FullUpdate();
    void _Init_PartialUpdate();
    void _waitWhileBusy(const char* comment = 0);
    void _rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h);
  private:
#if defined(__AVR)
    uint8_t _buffer[GxGDEW027W3_PAGE_SIZE];
#else
    uint8_t _buffer[GxGDEW027W3_BUFFER_SIZE];
#endif
    GxIO& IO;
    int16_t _current_page;
    bool _initial, _using_partial_mode;
    bool _diag_enabled;
    int8_t _rst;
    int8_t _busy;
    static const uint8_t lut_20_vcomDC[];
    static const uint8_t lut_21_ww[];
    static const uint8_t lut_22_bw[];
    static const uint8_t lut_23_wb[];
    static const uint8_t lut_24_bb[];
    static const uint8_t lut_20_vcomDC_partial[];
    static const uint8_t lut_21_ww_partial[];
    static const uint8_t lut_22_bw_partial[];
    static const uint8_t lut_23_wb_partial[];
    static const uint8_t lut_24_bb_partial[];
#if defined(ESP8266) || defined(ESP32)
  public:
    // the compiler of these packages has a problem with signature matching to base classes
    void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
    {
      Adafruit_GFX::drawBitmap(x, y, bitmap, w, h, color);
    };
#endif
};

#ifndef GxEPD_Class
#define GxEPD_Class GxGDEW027W3
#define GxEPD_WIDTH GxGDEW027W3_WIDTH
#define GxEPD_HEIGHT GxGDEW027W3_HEIGHT
#define GxEPD_BitmapExamples <GxGDEW027W3/BitmapExamples.h>
#define GxEPD_BitmapExamplesQ "GxGDEW027W3/BitmapExamples.h"
#endif

#endif
