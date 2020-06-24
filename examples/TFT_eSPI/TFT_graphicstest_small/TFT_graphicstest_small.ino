/*
 Adapted from the Adafruit graphicstest sketch.

 This sketch uses the GLCD font (font 1) only. Disable other fonts to make
 the sketch fit in an UNO!

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

float p = 3.1415926;


void testlines(uint16_t color)
{
    ttgo->tft->fillScreen(TFT_BLACK);
    for (int16_t x = 0; x < ttgo->tft->width(); x += 6) {
        ttgo->tft->drawLine(0, 0, x, ttgo->tft->height() - 1, color);
    }
    for (int16_t y = 0; y < ttgo->tft->height(); y += 6) {
        ttgo->tft->drawLine(0, 0, ttgo->tft->width() - 1, y, color);
    }

    ttgo->tft->fillScreen(TFT_BLACK);
    for (int16_t x = 0; x < ttgo->tft->width(); x += 6) {
        ttgo->tft->drawLine(ttgo->tft->width() - 1, 0, x, ttgo->tft->height() - 1, color);
    }
    for (int16_t y = 0; y < ttgo->tft->height(); y += 6) {
        ttgo->tft->drawLine(ttgo->tft->width() - 1, 0, 0, y, color);
    }

    ttgo->tft->fillScreen(TFT_BLACK);
    for (int16_t x = 0; x < ttgo->tft->width(); x += 6) {
        ttgo->tft->drawLine(0, ttgo->tft->height() - 1, x, 0, color);
    }
    for (int16_t y = 0; y < ttgo->tft->height(); y += 6) {
        ttgo->tft->drawLine(0, ttgo->tft->height() - 1, ttgo->tft->width() - 1, y, color);
    }

    ttgo->tft->fillScreen(TFT_BLACK);
    for (int16_t x = 0; x < ttgo->tft->width(); x += 6) {
        ttgo->tft->drawLine(ttgo->tft->width() - 1, ttgo->tft->height() - 1, x, 0, color);
    }
    for (int16_t y = 0; y < ttgo->tft->height(); y += 6) {
        ttgo->tft->drawLine(ttgo->tft->width() - 1, ttgo->tft->height() - 1, 0, y, color);
    }
}

void testdrawtext(char *text, uint16_t color)
{
    ttgo->tft->setCursor(0, 0);
    ttgo->tft->setTextColor(color);
    ttgo->tft->setTextWrap(true);
    ttgo->tft->print(text);
}

void testfastlines(uint16_t color1, uint16_t color2)
{
    ttgo->tft->fillScreen(TFT_BLACK);
    for (int16_t y = 0; y < ttgo->tft->height(); y += 5) {
        ttgo->tft->drawFastHLine(0, y, ttgo->tft->width(), color1);
    }
    for (int16_t x = 0; x < ttgo->tft->width(); x += 5) {
        ttgo->tft->drawFastVLine(x, 0, ttgo->tft->height(), color2);
    }
}

void testdrawrects(uint16_t color)
{
    ttgo->tft->fillScreen(TFT_BLACK);
    for (int16_t x = 0; x < ttgo->tft->width(); x += 6) {
        ttgo->tft->drawRect(ttgo->tft->width() / 2 - x / 2, ttgo->tft->height() / 2 - x / 2, x, x, color);
    }
}

void testfillrects(uint16_t color1, uint16_t color2)
{
    ttgo->tft->fillScreen(TFT_BLACK);
    for (int16_t x = ttgo->tft->width() - 1; x > 6; x -= 6) {
        ttgo->tft->fillRect(ttgo->tft->width() / 2 - x / 2, ttgo->tft->height() / 2 - x / 2, x, x, color1);
        ttgo->tft->drawRect(ttgo->tft->width() / 2 - x / 2, ttgo->tft->height() / 2 - x / 2, x, x, color2);
    }
}

void testfillcircles(uint8_t radius, uint16_t color)
{
    for (int16_t x = radius; x < ttgo->tft->width(); x += radius * 2) {
        for (int16_t y = radius; y < ttgo->tft->height(); y += radius * 2) {
            ttgo->tft->fillCircle(x, y, radius, color);
        }
    }
}

void testdrawcircles(uint8_t radius, uint16_t color)
{
    for (int16_t x = 0; x < ttgo->tft->width() + radius; x += radius * 2) {
        for (int16_t y = 0; y < ttgo->tft->height() + radius; y += radius * 2) {
            ttgo->tft->drawCircle(x, y, radius, color);
        }
    }
}

void testtriangles()
{
    ttgo->tft->fillScreen(TFT_BLACK);
    int color = 0xF800;
    int t;
    int w = ttgo->tft->width() / 2;
    int x = ttgo->tft->height() - 1;
    int y = 0;
    int z = ttgo->tft->width();
    for (t = 0 ; t <= 15; t += 1) {
        ttgo->tft->drawTriangle(w, y, y, x, z, x, color);
        x -= 4;
        y += 4;
        z -= 4;
        color += 100;
    }
}

void testroundrects()
{
    ttgo->tft->fillScreen(TFT_BLACK);
    int color = 100;
    int i;
    int t;
    for (t = 0 ; t <= 4; t += 1) {
        int x = 0;
        int y = 0;
        int w = ttgo->tft->width() - 2;
        int h = ttgo->tft->height() - 2;
        for (i = 0 ; i <= 16; i += 1) {
            ttgo->tft->drawRoundRect(x, y, w, h, 5, color);
            x += 2;
            y += 3;
            w -= 4;
            h -= 6;
            color += 1100;
        }
        color += 100;
    }
}

void tftPrintTest()
{
    ttgo->tft->setTextWrap(false);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setCursor(0, 30);
    ttgo->tft->setTextColor(TFT_RED);
    ttgo->tft->setTextSize(1);
    ttgo->tft->println("Hello World!");
    ttgo->tft->setTextColor(TFT_YELLOW);
    ttgo->tft->setTextSize(2);
    ttgo->tft->println("Hello World!");
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setTextSize(3);
    ttgo->tft->println("Hello World!");
    ttgo->tft->setTextColor(TFT_BLUE);
    ttgo->tft->setTextSize(4);
    ttgo->tft->print(1234.567);
    delay(1500);
    ttgo->tft->setCursor(0, 0);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->setTextSize(0);
    ttgo->tft->println("Hello World!");
    ttgo->tft->setTextSize(1);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->print(p, 6);
    ttgo->tft->println(" Want pi?");
    ttgo->tft->println(" ");
    ttgo->tft->print(8675309, HEX); // print 8,675,309 out in HEX!
    ttgo->tft->println(" Print HEX!");
    ttgo->tft->println(" ");
    ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->println("Sketch has been");
    ttgo->tft->println("running for: ");
    ttgo->tft->setTextColor(TFT_MAGENTA);
    ttgo->tft->print(millis() / 1000);
    ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->print(" seconds.");
}

void mediabuttons()
{
    // play
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->fillRoundRect(25, 10, 78, 60, 8, TFT_WHITE);
    ttgo->tft->fillTriangle(42, 20, 42, 60, 90, 40, TFT_RED);
    delay(500);
    // pause
    ttgo->tft->fillRoundRect(25, 90, 78, 60, 8, TFT_WHITE);
    ttgo->tft->fillRoundRect(39, 98, 20, 45, 5, TFT_GREEN);
    ttgo->tft->fillRoundRect(69, 98, 20, 45, 5, TFT_GREEN);
    delay(500);
    // play color
    ttgo->tft->fillTriangle(42, 20, 42, 60, 90, 40, TFT_BLUE);
    delay(50);
    // pause color
    ttgo->tft->fillRoundRect(39, 98, 20, 45, 5, TFT_RED);
    ttgo->tft->fillRoundRect(69, 98, 20, 45, 5, TFT_RED);
    // play color
    ttgo->tft->fillTriangle(42, 20, 42, 60, 90, 40, TFT_GREEN);
}


void setup(void)
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->tft->setTextFont(1);

    Serial.println("Initialized");

    uint16_t time = millis();
    ttgo->tft->fillScreen(TFT_BLACK);
    time = millis() - time;

    Serial.println(time, DEC);
    delay(500);

    // large block of text
    ttgo->tft->fillScreen(TFT_BLACK);
    testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", TFT_WHITE);
    delay(1000);

    // tft print function!
    tftPrintTest();
    delay(4000);

    // a single pixel
    ttgo->tft->drawPixel(ttgo->tft->width() / 2, ttgo->tft->height() / 2, TFT_GREEN);
    delay(500);

    // line draw test
    testlines(TFT_YELLOW);
    delay(500);

    // optimized lines
    testfastlines(TFT_RED, TFT_BLUE);
    delay(500);

    testdrawrects(TFT_GREEN);
    delay(500);

    testfillrects(TFT_YELLOW, TFT_MAGENTA);
    delay(500);

    ttgo->tft->fillScreen(TFT_BLACK);
    testfillcircles(10, TFT_BLUE);
    testdrawcircles(10, TFT_WHITE);
    delay(500);

    testroundrects();
    delay(500);

    testtriangles();
    delay(500);

    mediabuttons();
    delay(500);

    Serial.println("done");
    delay(1000);
}

void loop()
{
    ttgo->tft->invertDisplay(true);
    delay(500);
    ttgo->tft->invertDisplay(false);
    delay(500);
}