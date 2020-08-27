// class GxGDEW075T7 : Display class for GDEW075T7 e-Paper from Dalian Good Display Co., Ltd.: http://www.e-paper-display.com/products_detail/productId=456.html
//
// based on Demo Example from Good Display, available here: http://www.e-paper-display.com/download_detail/downloadsId=891.html
// Controller: GD7965 : http://www.e-paper-display.com/download_detail/downloadsId=821.html
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD

#ifndef _GxGDEW075T7_H_
#define _GxGDEW075T7_H_

#include "../GxEPD.h"

#define GxGDEW075T7_WIDTH 800
#define GxGDEW075T7_HEIGHT 480

// pixel number expressed in bytes; this is neither the buffer size nor the size of the buffer in the controller
#define GxGDEW075T7_BYTE_SIZE (uint32_t(GxGDEW075T7_WIDTH) * uint32_t(GxGDEW075T7_HEIGHT) / 8)

// divisor for AVR or limited RAM, should be factor of GxGDEW075T7_HEIGHT
// add conditional case if the compiler complains for your target
#if defined(__AVR)
#define GxGDEW075T7_PAGES 48
#elif defined(ESP8266)
#define GxGDEW075T7_PAGES 1 // take 2 if you need more remaining RAM
#elif defined(ARDUINO_ARCH_STM32F1)
#define GxGDEW075T7_PAGES 4
#else
#define GxGDEW075T7_PAGES 1 // e.g. ESP32
#endif

#define GxGDEW075T7_PAGE_HEIGHT (GxGDEW075T7_HEIGHT / GxGDEW075T7_PAGES)
#define GxGDEW075T7_PAGE_SIZE (GxGDEW075T7_BYTE_SIZE / GxGDEW075T7_PAGES)
#define GxGDEW075T7_BUFFER_SIZE GxGDEW075T7_PAGE_SIZE

class GxGDEW075T7 : public GxEPD
{
  public:
#if defined(ESP8266)
    //GxGDEW075T7(GxIO& io, int8_t rst = D4, int8_t busy = D2);
    // use pin numbers, other ESP8266 than Wemos may not use Dx names
    GxGDEW075T7(GxIO& io, int8_t rst = 2, int8_t busy = 4);
#else
    GxGDEW075T7(GxIO& io, int8_t rst = 9, int8_t busy = 7);
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
    // paged drawing, for limited RAM, drawCallback() is called GxGDEW075T7_PAGES times
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
    void _writeDataPGM(const uint8_t* data, uint16_t n, int16_t fill_with_zeroes = 0);
    void _wakeUp();
    void _sleep();
    void _Init_FullUpdate();
    void _Init_PartialUpdate();
    void _waitWhileBusy(const char* comment = 0);
    void _rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h);
  private:
    uint8_t _buffer[GxGDEW075T7_BUFFER_SIZE];
    GxIO& IO;
    int16_t _current_page;
    bool _using_partial_mode;
    bool _diag_enabled;
    int8_t _rst;
    int8_t _busy;
    static const unsigned char lut_20_LUTC_partial[];
    static const unsigned char lut_21_LUTWW_partial[];
    static const unsigned char lut_22_LUTKW_partial[];
    static const unsigned char lut_23_LUTWK_partial[];
    static const unsigned char lut_24_LUTKK_partial[];
    static const unsigned char lut_25_LUTBD_partial[];
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
#define GxEPD_Class GxGDEW075T7
#define GxEPD_WIDTH GxGDEW075T7_WIDTH
#define GxEPD_HEIGHT GxGDEW075T7_HEIGHT
#define GxEPD_BitmapExamples <GxGDEW075T7/BitmapExamples.h>
#define GxEPD_BitmapExamplesQ "GxGDEW075T7/BitmapExamples.h"
#endif

#endif
