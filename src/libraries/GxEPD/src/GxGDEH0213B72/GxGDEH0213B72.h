// class GxGDEH0213B72 : Display class for GDEH0213B72 e-Paper from Dalian Good Display Co., Ltd.: http://www.e-paper-display.com
//
// based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
// Controller: IL3897 :
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD

#ifndef _GxGDEH0213B72_H_
#define _GxGDEH0213B72_H_

#include "../GxEPD.h"

// the physical number of pixels (for controller parameter)
#define GxGDEH0213B72_X_PIXELS 128
#define GxGDEH0213B72_Y_PIXELS 250

// the logical width and height of the display
#define GxGDEH0213B72_WIDTH GxGDEH0213B72_X_PIXELS
#define GxGDEH0213B72_HEIGHT GxGDEH0213B72_Y_PIXELS

// note: the visible number of display pixels is 122*250, see GDEH0213B72 V1.1 Specification.pdf
#define GxGDEH0213B72_VISIBLE_WIDTH 122

#define GxGDEH0213B72_BUFFER_SIZE (uint32_t(GxGDEH0213B72_WIDTH) * uint32_t(GxGDEH0213B72_HEIGHT) / 8)

// divisor for AVR, should be factor of GxGDEH0213B72_HEIGHT
#define GxGDEH0213B72_PAGES 5

#define GxGDEH0213B72_PAGE_HEIGHT (GxGDEH0213B72_HEIGHT / GxGDEH0213B72_PAGES)
#define GxGDEH0213B72_PAGE_SIZE (GxGDEH0213B72_BUFFER_SIZE / GxGDEH0213B72_PAGES)

class GxGDEH0213B72 : public GxEPD
{
  public:
#if defined(ESP8266)
    //GxGDEH0213B72(GxIO& io, int8_t rst = D4, int8_t busy = D2);
    // use pin numbers, other ESP8266 than Wemos may not use Dx names
    GxGDEH0213B72(GxIO& io, int8_t rst = 2, int8_t busy = 4);
#else
    GxGDEH0213B72(GxIO& io, int8_t rst = 9, int8_t busy = 7);
#endif
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void init(uint32_t serial_diag_bitrate = 0); // = 0 : disabled
    void fillScreen(uint16_t color); // 0x0 black, >0x0 white, to buffer
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
    // paged drawing, for limited RAM, drawCallback() is called GxGDEH0213B72_PAGES times
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
    void _writeData(uint8_t data);
    void _writeCommand(uint8_t command);
    void _writeData(const uint8_t* data, uint16_t n);
    void _writeCommandData(const uint8_t* pCommandData, uint8_t datalen);
    void _SetRamPointer(uint8_t addrX, uint8_t addrY, uint8_t addrY1);
    void _SetRamArea(uint8_t Xstart, uint8_t Xend, uint8_t Ystart, uint8_t Ystart1, uint8_t Yend, uint8_t Yend1);
    void _PowerOn(void);
    void _PowerOff(void);
    void _waitWhileBusy(const char* comment=0);
    void _setRamDataEntryMode(uint8_t em);
    void _InitDisplay(uint8_t em);
    void _Init_Full(uint8_t em);
    void _Init_Part(uint8_t em);
    void _Update_Full(void);
    void _Update_Part(void);
    void _rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h);
  protected:
#if defined(__AVR)
    uint8_t _buffer[GxGDEH0213B72_PAGE_SIZE];
#else
    uint8_t _buffer[GxGDEH0213B72_BUFFER_SIZE];
#endif
  private:
    GxIO& IO;
    int16_t _current_page;
    bool _using_partial_mode;
    bool _diag_enabled;
    int8_t _rst;
    int8_t _busy;
    static const uint8_t LUT_DATA_full[];
    static const uint8_t LUT_DATA_part[];
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
#define GxEPD_Class GxGDEH0213B72
#define GxEPD_WIDTH GxGDEH0213B72_WIDTH
#define GxEPD_HEIGHT GxGDEH0213B72_HEIGHT
#define GxEPD_BitmapExamples <GxGDEH0213B72/BitmapExamples.h>
#define GxEPD_BitmapExamplesQ "GxGDEH0213B72/BitmapExamples.h"
#endif

#endif
