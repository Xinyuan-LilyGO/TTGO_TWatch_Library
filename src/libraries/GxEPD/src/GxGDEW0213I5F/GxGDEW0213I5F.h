// class GxGDEW0213I5F : Display class for GDEW0213I5F e-Paper from Dalian Good Display Co., Ltd.: http://www.e-paper-display.com/products_detail/productId=397.html
//
// based on Demo Example from Good Display, available here: http://www.e-paper-display.com/download_detail/downloadsId=597.html
// Controller: IL0373 : http://www.e-paper-display.com/download_detail/downloadsId=535.html
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD

#ifndef _GxGDEW0213I5F_H_
#define _GxGDEW0213I5F_H_

#include "../GxEPD.h"

#define GxGDEW0213I5F_WIDTH 104
#define GxGDEW0213I5F_HEIGHT 212

#define GxGDEW0213I5F_BUFFER_SIZE (uint32_t(GxGDEW0213I5F_WIDTH) * uint32_t(GxGDEW0213I5F_HEIGHT) / 8)

// divisor for AVR, should be factor of GxGDEW0213I5F_HEIGHT
#define GxGDEW0213I5F_PAGES 4

#define GxGDEW0213I5F_PAGE_HEIGHT (GxGDEW0213I5F_HEIGHT / GxGDEW0213I5F_PAGES)
#define GxGDEW0213I5F_PAGE_SIZE (GxGDEW0213I5F_BUFFER_SIZE / GxGDEW0213I5F_PAGES)

class GxGDEW0213I5F : public GxEPD
{
  public:
#if defined(ESP8266)
    //GxGDEW0213I5F(GxIO& io, int8_t rst = D4, int8_t busy = D2);
    // use pin numbers, other ESP8266 than Wemos may not use Dx names
    GxGDEW0213I5F(GxIO& io, int8_t rst = 2, int8_t busy = 4);
#else
    GxGDEW0213I5F(GxIO& io, int8_t rst = 9, int8_t busy = 7);
#endif
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void init(uint32_t serial_diag_bitrate = 0); // = 0 : disabled
    void fillScreen(uint16_t color); // to buffer
    void update(void);
    // to buffer, may be cropped, drawPixel() used, update needed
    void  drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t mode = bm_normal);
    // to full screen, filled with white if size is less, no update needed
    void drawBitmap(const uint8_t *bitmap, uint32_t size, int16_t mode = bm_normal); // only bm_normal, bm_invert, bm_partial_update modes implemented
    void eraseDisplay(bool using_partial_update = false);
    // partial update of rectangle from buffer to screen, does not power off
    void updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation = true);
    // partial update of rectangle at (xs,ys) from buffer to screen at (xd,yd), does not power off
    void updateToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation = true);
    // terminate cleanly updateWindow or updateToWindow before removing power or long delays
    void powerDown();
    // paged drawing, for limited RAM, drawCallback() is called GxGDEW0213I5F_PAGES times
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
    uint16_t _setPartialRamArea(uint16_t x, uint16_t y, uint16_t xe, uint16_t ye);
    void _writeData(uint8_t data);
    void _writeCommand(uint8_t command);
    void _wakeUp();
    void _sleep();
    void _Init_FullUpdate();
    void _Init_PartialUpdate();
    void _waitWhileBusy(const char* comment = 0);
    void _rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h);
  private:
#if defined(__AVR)
    uint8_t _buffer[GxGDEW0213I5F_PAGE_SIZE];
#else
    uint8_t _buffer[GxGDEW0213I5F_BUFFER_SIZE];
#endif
    GxIO& IO;
    int16_t _current_page;
    bool _using_partial_mode;
    bool _diag_enabled;
    int8_t _rst;
    int8_t _busy;
    static const unsigned char lut_20_vcomDC[];
    static const unsigned char lut_21_ww[];
    static const unsigned char lut_22_bw[];
    static const unsigned char lut_23_wb[];
    static const unsigned char lut_24_bb[];
    static const unsigned char lut_20_vcomDC_partial[];
    static const unsigned char lut_21_ww_partial[];
    static const unsigned char lut_22_bw_partial[];
    static const unsigned char lut_23_wb_partial[];
    static const unsigned char lut_24_bb_partial[];
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
#define GxEPD_Class GxGDEW0213I5F
#define GxEPD_WIDTH GxGDEW0213I5F_WIDTH
#define GxEPD_HEIGHT GxGDEW0213I5F_HEIGHT
#define GxEPD_BitmapExamples <GxGDEW0213I5F/BitmapExamples.h>
#define GxEPD_BitmapExamplesQ "GxGDEW0213I5F/BitmapExamples.h"
#endif

#endif
