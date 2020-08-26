#ifndef __ePaperCanvas__
#define __ePaperCanvas__
#include "../../Adafruit-GFX-Library/Adafruit_GFX.h"
#include "ePaperDeviceConfigurations.h"

// these are the color values supported
typedef uint8_t ePaperColorType;

const ePaperColorType ePaper_WHITE      = 0x00;
const ePaperColorType ePaper_GRAY1      = 0x01;     // use if ePaper display has gray scale
const ePaperColorType ePaper_GRAY2      = 0x02;     // use if ePaper display has gray scale

const ePaperColorType ePaper_BLACK      = 0x0F;
const ePaperColorType ePaper_COLOR      = 0x10;     // used if the ePaper display has a third color
const ePaperColorType ePaper_INVERSE1   = 0xF1;     // b -> w, w -> b, c -> w
const ePaperColorType ePaper_INVERSE2   = 0xF2;     // b -> c or w, w -> b, c -> b
const ePaperColorType ePaper_INVERSE3   = 0xF3;     // b -> w, w -> c or b, c -> b


class ePaperCanvas : public Adafruit_GFX
{
public:


private:
    static const uint8_t bitmasks[];

    size_t _bufferSize;
    uint8_t *_blackBuffer;      // used for b&w
    uint8_t *_colorBuffer;      // used for bit 2 in color or gray scale displays

    const ePaperColorMode   _mode;

    void getBitSettingsForColor(uint16_t color, bool &blackBit, bool &colorBit );
    void drawFastRawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void drawFastRawVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

protected:
    ePaperColorMode getColorMode(void) const
    {
        return _mode;
    }
    size_t getBufferrSize(void) const
    {
        return _bufferSize;
    }
    const uint8_t *getBuffer1(void) const
    {
        return _blackBuffer;
    }
    const uint8_t *getBuffer2(void) const
    {
        return _colorBuffer;
    }

public:
    ePaperCanvas(
        int16_t w,
        int16_t h,
        ePaperColorMode mode
    );

    virtual ~ePaperCanvas();


    //
    // Adafruit GFX support
    //

    void drawPixel(int16_t x, int16_t y, ePaperColorType color);
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color)
    {
        this->drawPixel(x, y, (ePaperColorType)color);
    }
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

    virtual void fillScreen(uint16_t color);

    virtual void invertDisplay(boolean i);

    // direct image
    void setDeviceImage(
        const uint8_t *blackBitMap,
        size_t blackBitMapSize,
        bool blackBitMapIsProgMem
    );

    void setDeviceImage(
        const uint8_t *blackBitMap,
        size_t blackBitMapSize,
        bool blackBitMapIsProgMem,
        const uint8_t *colorBitMap,
        size_t colorBitMapSize,
        bool colorBitMapIsProgMem
    );

    void drawBitImage(
        int16_t loc_x, int16_t loc_y,
        int16_t img_w, int16_t img_h,
        const uint8_t *blackBitMap,
        size_t blackBitMapSize,
        bool blackBitMapIsProgMem,
        const uint8_t *colorBitMap,
        size_t colorBitMapSize,
        bool colorBitMapIsProgMem
    );

};


#endif // __ePaperCanvas__
