/*
 Adapted from the Adafruit and Xark's PDQ graphicstest sketch.

 This sketch uses the GLCD font only.

 Make sure all the display driver and pin comnenctions are correct by
 editting the User_Setup.h file in the TFT_eSPI library folder.

 Note that yield() or delay(0) must be called in long duration for/while
 loops to stop the ESP8266 watchdog triggering.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */

#include "config.h"

TTGOClass *ttgo;

// These are used to get information about static SRAM and flash memory sizes
extern "C" char __data_start[];     // start of SRAM data
extern "C" char _end[];         // end of SRAM data (used to check amount of SRAM this program's variables use)
extern "C" char __data_load_end[];  // end of FLASH (used to check amount of Flash this program's code and data uses)



void printnice(int32_t v)
{
    char  str[32] = { 0 };
    sprintf(str, "%ld", v);
    for (char *p = (str + strlen(str)) - 3; p > str; p -= 3) {
        memmove(p + 1, p, strlen(p) + 1);
        *p = ',';

    }
    while (strlen(str) < 10) {
        memmove(str + 1, str, strlen(str) + 1);
        *str = ' ';
    }
    ttgo->tft->print(str);
}

static inline uint32_t micros_start() __attribute__ ((always_inline));
static inline uint32_t micros_start()
{
    uint8_t oms = millis();
    while ((uint8_t)millis() == oms)
        ;
    return micros();
}

uint32_t testHaD()
{
    // pseudo-code for cheesy RLE
    // start with color1
    // while more input data remaining
    //  count =  0nnnnnnn = 1 byte or 1nnnnnnn nnnnnnnn 2 bytes (0 - 32767)
    //  repeat color count times
    //  toggle color1/color2
    static const uint8_t HaD_128x160[] PROGMEM = {
        0x85, 0x91, 0x09, 0x4b, 0x09, 0x24, 0x0a, 0x47, 0x09, 0x27, 0x0a, 0x44, 0x0a, 0x29, 0x0a, 0x42,
        0x0a, 0x2b, 0x0a, 0x41, 0x0a, 0x2c, 0x0a, 0x3e, 0x0b, 0x2f, 0x09, 0x3d, 0x09, 0x32, 0x08, 0x3c,
        0x09, 0x33, 0x09, 0x3b, 0x08, 0x33, 0x0a, 0x3a, 0x0a, 0x31, 0x0b, 0x3a, 0x0c, 0x1d, 0x01, 0x10,
        0x0d, 0x39, 0x0c, 0x1d, 0x01, 0x10, 0x0d, 0x39, 0x0d, 0x0f, 0x01, 0x0c, 0x03, 0x0d, 0x0e, 0x39,
        0x0e, 0x0c, 0x03, 0x0c, 0x04, 0x0b, 0x0f, 0x39, 0x0f, 0x0a, 0x04, 0x0c, 0x05, 0x09, 0x10, 0x39,
        0x10, 0x08, 0x05, 0x0c, 0x06, 0x07, 0x11, 0x39, 0x11, 0x06, 0x06, 0x0d, 0x07, 0x04, 0x13, 0x37,
        0x12, 0x05, 0x07, 0x0d, 0x08, 0x02, 0x15, 0x34, 0x15, 0x03, 0x08, 0x0d, 0x20, 0x32, 0x20, 0x0e,
        0x21, 0x31, 0x20, 0x0f, 0x21, 0x2e, 0x22, 0x10, 0x22, 0x2b, 0x22, 0x12, 0x22, 0x12, 0x05, 0x12,
        0x22, 0x14, 0x22, 0x0c, 0x0f, 0x0c, 0x22, 0x16, 0x22, 0x08, 0x15, 0x08, 0x22, 0x18, 0x22, 0x05,
        0x19, 0x05, 0x21, 0x1c, 0x1f, 0x04, 0x1c, 0x05, 0x1f, 0x1f, 0x1c, 0x04, 0x1e, 0x04, 0x1d, 0x2b,
        0x11, 0x04, 0x21, 0x03, 0x12, 0x36, 0x0f, 0x03, 0x24, 0x03, 0x10, 0x38, 0x0d, 0x03, 0x26, 0x03,
        0x0d, 0x3b, 0x0b, 0x03, 0x28, 0x03, 0x0b, 0x3d, 0x0a, 0x03, 0x29, 0x03, 0x09, 0x40, 0x07, 0x03,
        0x2b, 0x03, 0x07, 0x42, 0x05, 0x03, 0x2c, 0x04, 0x04, 0x45, 0x04, 0x03, 0x2d, 0x03, 0x04, 0x46,
        0x02, 0x03, 0x2e, 0x03, 0x03, 0x48, 0x01, 0x03, 0x2f, 0x03, 0x01, 0x4c, 0x31, 0x4e, 0x32, 0x4e,
        0x33, 0x4c, 0x34, 0x4c, 0x34, 0x4c, 0x35, 0x4b, 0x35, 0x4a, 0x0e, 0x03, 0x14, 0x04, 0x0d, 0x4a,
        0x0b, 0x09, 0x0e, 0x0a, 0x0a, 0x4a, 0x0a, 0x0b, 0x0c, 0x0c, 0x09, 0x4a, 0x09, 0x0d, 0x0a, 0x0e,
        0x09, 0x49, 0x08, 0x0f, 0x09, 0x0e, 0x09, 0x49, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x49, 0x08, 0x0f,
        0x09, 0x0f, 0x08, 0x49, 0x07, 0x0f, 0x0a, 0x0f, 0x08, 0x49, 0x07, 0x0f, 0x0b, 0x0e, 0x08, 0x49,
        0x07, 0x0d, 0x0e, 0x0d, 0x08, 0x49, 0x07, 0x0b, 0x13, 0x0a, 0x08, 0x49, 0x08, 0x07, 0x18, 0x08,
        0x08, 0x49, 0x08, 0x06, 0x1b, 0x06, 0x08, 0x49, 0x09, 0x04, 0x1c, 0x05, 0x08, 0x4a, 0x09, 0x04,
        0x1d, 0x04, 0x08, 0x4a, 0x0a, 0x03, 0x1d, 0x03, 0x09, 0x4b, 0x19, 0x02, 0x1a, 0x4b, 0x19, 0x03,
        0x19, 0x4b, 0x18, 0x04, 0x18, 0x4d, 0x17, 0x05, 0x17, 0x4a, 0x01, 0x02, 0x17, 0x05, 0x16, 0x4a,
        0x02, 0x02, 0x17, 0x05, 0x16, 0x02, 0x03, 0x44, 0x03, 0x03, 0x16, 0x02, 0x01, 0x02, 0x16, 0x02,
        0x03, 0x43, 0x05, 0x03, 0x15, 0x01, 0x03, 0x01, 0x15, 0x03, 0x04, 0x41, 0x06, 0x03, 0x15, 0x01,
        0x03, 0x01, 0x14, 0x03, 0x07, 0x3d, 0x09, 0x03, 0x2d, 0x03, 0x08, 0x3b, 0x0a, 0x04, 0x2b, 0x03,
        0x0a, 0x39, 0x0c, 0x03, 0x2a, 0x04, 0x0b, 0x37, 0x0e, 0x03, 0x28, 0x03, 0x0e, 0x2e, 0x04, 0x03,
        0x10, 0x03, 0x27, 0x03, 0x10, 0x03, 0x03, 0x24, 0x19, 0x03, 0x26, 0x03, 0x1a, 0x1e, 0x1d, 0x03,
        0x24, 0x03, 0x1e, 0x19, 0x20, 0x04, 0x21, 0x03, 0x20, 0x17, 0x22, 0x04, 0x1f, 0x03, 0x22, 0x15,
        0x22, 0x04, 0x21, 0x04, 0x21, 0x13, 0x22, 0x05, 0x15, 0x01, 0x0b, 0x05, 0x21, 0x12, 0x21, 0x06,
        0x15, 0x01, 0x0b, 0x06, 0x21, 0x10, 0x21, 0x07, 0x0a, 0x01, 0x0a, 0x01, 0x0b, 0x07, 0x21, 0x0e,
        0x20, 0x0a, 0x09, 0x02, 0x09, 0x02, 0x09, 0x09, 0x20, 0x0e, 0x08, 0x02, 0x15, 0x0b, 0x08, 0x03,
        0x08, 0x03, 0x08, 0x0b, 0x15, 0x03, 0x08, 0x0d, 0x07, 0x04, 0x13, 0x0d, 0x06, 0x05, 0x06, 0x06,
        0x05, 0x0d, 0x14, 0x04, 0x07, 0x0c, 0x07, 0x06, 0x11, 0x38, 0x12, 0x06, 0x06, 0x0c, 0x06, 0x08,
        0x10, 0x39, 0x10, 0x08, 0x05, 0x0c, 0x04, 0x0b, 0x0f, 0x39, 0x0f, 0x0a, 0x04, 0x0c, 0x03, 0x0d,
        0x0e, 0x39, 0x0e, 0x0c, 0x03, 0x0c, 0x02, 0x0e, 0x0e, 0x39, 0x0d, 0x0f, 0x01, 0x0c, 0x01, 0x10,
        0x0d, 0x39, 0x0d, 0x0f, 0x01, 0x1e, 0x0c, 0x39, 0x0c, 0x30, 0x0a, 0x3a, 0x0a, 0x33, 0x09, 0x3b,
        0x08, 0x34, 0x09, 0x3b, 0x09, 0x32, 0x09, 0x3c, 0x0a, 0x2f, 0x0a, 0x3e, 0x0a, 0x2d, 0x0b, 0x3f,
        0x0a, 0x2b, 0x0b, 0x41, 0x0a, 0x29, 0x0b, 0x43, 0x0a, 0x27, 0x0a, 0x46, 0x0a, 0x25, 0x0a, 0x49,
        0x09, 0x23, 0x08, 0x4e, 0x08, 0x96, 0x12
    };

    ttgo->tft->fillScreen(TFT_BLACK);

    uint32_t start = micros_start();

    ttgo->tft->startWrite();

    for (int i = 0; i < 0x10; i++) {
        ttgo->tft->setAddrWindow(0, 0, ttgo->tft->width(), ttgo->tft->height());

        uint16_t cnt = 0;
        uint16_t color = ttgo->tft->color565((i << 4) | i, (i << 4) | i, (i << 4) | i);
        uint16_t curcolor = 0;

        const uint8_t *cmp = &HaD_128x160[0];
        ttgo->tft->startWrite();
        while (cmp < &HaD_128x160[sizeof(HaD_128x160)]) {
            cnt = pgm_read_byte(cmp++);
            if (cnt & 0x80)
                cnt = ((cnt & 0x7f) << 8) | pgm_read_byte(cmp++);

            ttgo->tft->pushColor(curcolor, cnt); // PDQ_GFX has count

            curcolor ^= color;
        }
        ttgo->tft->endWrite();
    }

    ttgo->tft->endWrite();

    uint32_t t = micros() - start;

    ttgo->tft->setTextColor(TFT_YELLOW);
    ttgo->tft->setCursor(0, 145);
    ttgo->tft->print(F(" http://hackaday.io/"));
    ttgo->tft->print(F("         Xark"));

    delay(3 * 1000L);

    return t;
}

uint32_t testFillScreen()
{
    uint32_t start = micros_start();

    for (uint8_t i = 0; i < 12; i++) {
        ttgo->tft->fillScreen(TFT_WHITE);
        ttgo->tft->fillScreen(TFT_RED);
        ttgo->tft->fillScreen(TFT_GREEN);
        ttgo->tft->fillScreen(TFT_BLUE);
        ttgo->tft->fillScreen(TFT_BLACK);
    }

    return micros() - start;
}

uint32_t testText()
{
    ttgo->tft->fillScreen(TFT_BLACK);
    uint32_t start = micros_start();
    ttgo->tft->setCursor(0, 0);
    ttgo->tft->setTextColor(TFT_WHITE); ttgo->tft->setTextSize(1);
    ttgo->tft->println(F("Hello World!"));
    ttgo->tft->setTextColor(ttgo->tft->color565(0xff, 0x00, 0x00));
    ttgo->tft->print(F("RED "));
    ttgo->tft->setTextColor(ttgo->tft->color565(0x00, 0xff, 0x00));
    ttgo->tft->print(F("GREEN "));
    ttgo->tft->setTextColor(ttgo->tft->color565(0x00, 0x00, 0xff));
    ttgo->tft->println(F("BLUE"));
    ttgo->tft->setTextColor(TFT_YELLOW);
    ttgo->tft->println(1234.56);
    ttgo->tft->setTextColor(TFT_RED);
    ttgo->tft->println(0xDEADBEEF, HEX);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setTextSize(2);
    ttgo->tft->println(F("Groop"));
    ttgo->tft->println(F("I implore thee,"));
    ttgo->tft->setTextSize(1);
    ttgo->tft->println(F("my foonting turlingdromes."));
    ttgo->tft->println(F("And hooptiously drangle me"));
    ttgo->tft->println(F("with crinkly bindlewurdles,"));
    ttgo->tft->println(F("Or I will rend thee"));
    ttgo->tft->println(F("in the gobberwarts"));
    ttgo->tft->println(F("with my blurglecruncheon,"));
    ttgo->tft->println(F("see if I don't!"));
    ttgo->tft->println(F(""));
    ttgo->tft->setTextColor(TFT_MAGENTA);
    ttgo->tft->println(F("Woot!"));
    uint32_t t = micros() - start;
    delay(1000);
    return t;
}

uint32_t testPixels()
{
    int32_t w = ttgo->tft->width();
    int32_t h = ttgo->tft->height();

    uint32_t start = micros_start();
    ttgo->tft->startWrite();
    for (uint16_t y = 0; y < h; y++) {
        for (uint16_t x = 0; x < w; x++) {
            ttgo->tft->drawPixel(x, y, ttgo->tft->color565(x << 3, y << 3, x * y));
        }
    }
    ttgo->tft->endWrite();
    return micros() - start;
}


uint32_t testLines(uint16_t color)
{
    uint32_t start, t;
    int32_t x1, y1, x2, y2;
    int32_t w = ttgo->tft->width();
    int32_t h = ttgo->tft->height();

    ttgo->tft->fillScreen(TFT_BLACK);

    x1 = y1 = 0;
    y2 = h - 1;

    start = micros_start();

    for (x2 = 0; x2 < w; x2 += 6) {
        ttgo->tft->drawLine(x1, y1, x2, y2, color);
    }

    x2 = w - 1;

    for (y2 = 0; y2 < h; y2 += 6) {
        ttgo->tft->drawLine(x1, y1, x2, y2, color);
    }

    t = micros() - start; // fillScreen doesn't count against timing

    ttgo->tft->fillScreen(TFT_BLACK);

    x1 = w - 1;
    y1 = 0;
    y2 = h - 1;

    start = micros_start();

    for (x2 = 0; x2 < w; x2 += 6) {
        ttgo->tft->drawLine(x1, y1, x2, y2, color);
    }

    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6) {
        ttgo->tft->drawLine(x1, y1, x2, y2, color);
    }

    t += micros() - start;

    ttgo->tft->fillScreen(TFT_BLACK);

    x1 = 0;
    y1 = h - 1;
    y2 = 0;

    start = micros_start();

    for (x2 = 0; x2 < w; x2 += 6) {
        ttgo->tft->drawLine(x1, y1, x2, y2, color);
    }
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) {
        ttgo->tft->drawLine(x1, y1, x2, y2, color);
    }
    t += micros() - start;

    ttgo->tft->fillScreen(TFT_BLACK);

    x1 = w - 1;
    y1 = h - 1;
    y2 = 0;

    start = micros_start();

    for (x2 = 0; x2 < w; x2 += 6) {
        ttgo->tft->drawLine(x1, y1, x2, y2, color);
    }

    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6) {
        ttgo->tft->drawLine(x1, y1, x2, y2, color);
    }

    t += micros() - start;

    return t;
}

uint32_t testFastLines(uint16_t color1, uint16_t color2)
{
    uint32_t start;
    int32_t x, y;
    int32_t w = ttgo->tft->width();
    int32_t h = ttgo->tft->height();

    ttgo->tft->fillScreen(TFT_BLACK);

    start = micros_start();

    for (y = 0; y < h; y += 5)
        ttgo->tft->drawFastHLine(0, y, w, color1);
    for (x = 0; x < w; x += 5)
        ttgo->tft->drawFastVLine(x, 0, h, color2);

    return micros() - start;
}

uint32_t testRects(uint16_t color)
{
    uint32_t start;
    int32_t n, i, i2;
    int32_t cx = ttgo->tft->width() / 2;
    int32_t cy = ttgo->tft->height() / 2;

    ttgo->tft->fillScreen(TFT_BLACK);
    n = min(ttgo->tft->width(), ttgo->tft->height());
    start = micros_start();
    for (i = 2; i < n; i += 6) {
        i2 = i / 2;
        ttgo->tft->drawRect(cx - i2, cy - i2, i, i, color);
    }

    return micros() - start;
}

uint32_t testFilledRects(uint16_t color1, uint16_t color2)
{
    uint32_t start, t = 0;
    int32_t n, i, i2;
    int32_t cx = ttgo->tft->width() / 2 - 1;
    int32_t cy = ttgo->tft->height() / 2 - 1;

    ttgo->tft->fillScreen(TFT_BLACK);
    n = min(ttgo->tft->width(), ttgo->tft->height());
    for (i = n; i > 0; i -= 6) {
        i2 = i / 2;

        start = micros_start();

        ttgo->tft->fillRect(cx - i2, cy - i2, i, i, color1);

        t += micros() - start;

        // Outlines are not included in timing results
        ttgo->tft->drawRect(cx - i2, cy - i2, i, i, color2);
    }

    return t;
}

uint32_t testFilledCircles(uint8_t radius, uint16_t color)
{
    uint32_t start;
    int32_t x, y, w = ttgo->tft->width(), h = ttgo->tft->height(), r2 = radius * 2;

    ttgo->tft->fillScreen(TFT_BLACK);

    start = micros_start();

    for (x = radius; x < w; x += r2) {
        for (y = radius; y < h; y += r2) {
            ttgo->tft->fillCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

uint32_t testCircles(uint8_t radius, uint16_t color)
{
    uint32_t start;
    int32_t x, y, r2 = radius * 2;
    int32_t w = ttgo->tft->width() + radius;
    int32_t h = ttgo->tft->height() + radius;

    // Screen is not cleared for this one -- this is
    // intentional and does not affect the reported time.
    start = micros_start();

    for (x = 0; x < w; x += r2) {
        for (y = 0; y < h; y += r2) {
            ttgo->tft->drawCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

uint32_t testTriangles()
{
    uint32_t start;
    int32_t n, i;
    int32_t cx = ttgo->tft->width() / 2 - 1;
    int32_t cy = ttgo->tft->height() / 2 - 1;

    ttgo->tft->fillScreen(TFT_BLACK);
    n = min(cx, cy);

    start = micros_start();

    for (i = 0; i < n; i += 5) {
        ttgo->tft->drawTriangle(
            cx, cy - i,     // peak
            cx - i, cy + i, // bottom left
            cx + i, cy + i, // bottom right
            ttgo->tft->color565(0, 0, i));
    }

    return micros() - start;
}

uint32_t testFilledTriangles()
{
    uint32_t start, t = 0;
    int32_t i;
    int32_t cx = ttgo->tft->width() / 2 - 1;
    int32_t cy = ttgo->tft->height() / 2 - 1;

    ttgo->tft->fillScreen(TFT_BLACK);

    start = micros_start();

    for (i = min(cx, cy); i > 10; i -= 5) {
        start = micros_start();
        ttgo->tft->fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                                ttgo->tft->color565(0, i, i));
        t += micros() - start;
        ttgo->tft->drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                                ttgo->tft->color565(i, i, 0));
    }

    return t;
}

uint32_t testRoundRects()
{
    uint32_t start;
    int32_t w, i, i2;
    int32_t cx = ttgo->tft->width() / 2 - 1;
    int32_t cy = ttgo->tft->height() / 2 - 1;

    ttgo->tft->fillScreen(TFT_BLACK);

    w = min(ttgo->tft->width(), ttgo->tft->height());

    start = micros_start();

    for (i = 0; i < w; i += 6) {
        i2 = i / 2;
        ttgo->tft->drawRoundRect(cx - i2, cy - i2, i, i, i / 8, ttgo->tft->color565(i, 0, 0));
    }

    return micros() - start;
}

uint32_t testFilledRoundRects()
{
    uint32_t start;
    int32_t i, i2;
    int32_t cx = ttgo->tft->width() / 2 - 1;
    int32_t cy = ttgo->tft->height() / 2 - 1;

    ttgo->tft->fillScreen(TFT_BLACK);

    start = micros_start();

    for (i = min(ttgo->tft->width(), ttgo->tft->height()); i > 20; i -= 6) {
        i2 = i / 2;
        ttgo->tft->fillRoundRect(cx - i2, cy - i2, i, i, i / 8, ttgo->tft->color565(0, i, 0));
    }

    return micros() - start;
}


void setup()
{
    Serial.begin(115200);
    Serial.println(F("TFT 1.8\" SPI TFT Test!     "));
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->tft->setTextFont(1);
}

// NOTE: This demo should run, but may look odd on 128x128 LCD (vs 128x160)

void loop(void)
{

    Serial.print(F(__DATE__ " " __TIME__ " - Flash=0x"));
    // These are not compatible with the ESP8266 core library
    //Serial.print((uint16_t)__data_load_end, HEX);
    //Serial.print(F(" RAM=0x"));
    //Serial.println((uint16_t)_end - (uint16_t)__data_start, HEX);
    Serial.println(F("Benchmark                Time (microseconds)"));

    uint32_t usecHaD = testHaD();
    Serial.print(F("HaD pushColor             "));
    Serial.println(usecHaD);
    delay(100);


    uint32_t usecFillScreen = testFillScreen();
    Serial.print(F("Screen fill              "));
    Serial.println(usecFillScreen);
    delay(100);

    ttgo->tft->setRotation(1);
    uint32_t usecText = testText();
    Serial.print(F("Text                     "));
    Serial.println(usecText);
    delay(100);
    ttgo->tft->setRotation(0);

    uint32_t usecPixels = testPixels();
    Serial.print(F("Pixels                   "));
    Serial.println(usecPixels);
    delay(100);

    uint32_t usecLines = testLines(TFT_BLUE);
    Serial.print(F("Lines                    "));
    Serial.println(usecLines);
    delay(100);

    uint32_t usecFastLines = testFastLines(TFT_RED, TFT_BLUE);
    Serial.print(F("Horiz/Vert Lines         "));
    Serial.println(usecFastLines);
    delay(100);

    uint32_t usecRects = testRects(TFT_GREEN);
    Serial.print(F("Rectangles (outline)     "));
    Serial.println(usecRects);
    delay(100);

    uint32_t usecFilledRects = testFilledRects(TFT_YELLOW, TFT_MAGENTA);
    Serial.print(F("Rectangles (filled)      "));
    Serial.println(usecFilledRects);
    delay(100);

    uint32_t usecFilledCircles = testFilledCircles(10, TFT_MAGENTA);
    Serial.print(F("Circles (filled)         "));
    Serial.println(usecFilledCircles);
    delay(100);

    uint32_t usecCircles = testCircles(10, TFT_WHITE);
    Serial.print(F("Circles (outline)        "));
    Serial.println(usecCircles);
    delay(100);

    uint32_t usecTriangles = testTriangles();
    Serial.print(F("Triangles (outline)      "));
    Serial.println(usecTriangles);
    delay(100);

    uint32_t usecFilledTrangles = testFilledTriangles();
    Serial.print(F("Triangles (filled)       "));
    Serial.println(usecFilledTrangles);
    delay(100);

    uint32_t usecRoundRects = testRoundRects();
    Serial.print(F("Rounded rects (outline)  "));
    Serial.println(usecRoundRects);
    delay(100);

    uint32_t usedFilledRoundRects = testFilledRoundRects();
    Serial.print(F("Rounded rects (filled)   "));
    Serial.println(usedFilledRoundRects);
    delay(100);

    Serial.println(F("Done!"));

    uint16_t c = 4;
    int8_t d = 1;
    for (int32_t i = 0; i < ttgo->tft->height(); i++) {
        ttgo->tft->drawFastHLine(0, i, ttgo->tft->width(), c);
        c += d;
        if (c <= 4 || c >= 11)
            d = -d;
    }

    ttgo->tft->setCursor(0, 0);
    ttgo->tft->setTextColor(TFT_MAGENTA);

    ttgo->tft->println(F("Bodmer's TFT_eSPI"));

    ttgo->tft->setTextSize(1);
    ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->println(F("SPI TFT on ESP8266"));
    ttgo->tft->println(F(""));
    ttgo->tft->setTextSize(1);
    ttgo->tft->setTextColor(ttgo->tft->color565(0x80, 0x80, 0x80));

    // These are not compatible with the ESP8266 core library
    //ttgo->tft->print(F("Flash=0x"));
    //ttgo->tft->print((uint16_t)__data_load_end, HEX);
    //ttgo->tft->print(F(" RAM=0x"));
    //ttgo->tft->print((uint16_t)_end - (uint16_t)__data_start, HEX);

    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->print(F("Benchmark        usec"));

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("HaD logo   "));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usecHaD);

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("Clear      "));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usecFillScreen);

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("Text       "));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usecText);

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("Pixels     "));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usecPixels);

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("Lines      "));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usecLines);

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("H/V Lines  "));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usecFastLines);

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("Rectangles "));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usecRects);

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("Rects-Fill "));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usecFilledRects);

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("Circles    "));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usecCircles);

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("CircleFill "));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usecFilledCircles);

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("Triangles  "));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usecTriangles);

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("Tris-Fill  "));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usecFilledTrangles);

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("Rnd-Rects  "));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usecRoundRects);

    ttgo->tft->setTextColor(TFT_CYAN);
    ttgo->tft->print(F("RRects-Fill"));
    ttgo->tft->setTextColor(TFT_YELLOW);
    printnice(usedFilledRoundRects);

    ttgo->tft->setTextSize(1);
    ttgo->tft->println(F(""));
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->print(F("Benchmark Completed!"));

    delay(15 * 1000L);
}