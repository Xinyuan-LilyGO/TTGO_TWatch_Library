/*

Use ePaperDriverLib, a great e-paper driver library
    -> https://github.com/michaelkamprath/ePaperDriverLib.git

*/
#include "config.h"
#include "SPI.h"

TTGOClass *twatch = nullptr;
ePaperDisplay *ePaper = nullptr;
int16_t x, y;

const char *str = "LilyGo";
const char *str1 = "ePaper Demo";
const char *str2 = "An individual human existence should be like a river—small at first, narrowly contained within its banks, and rushing passionately past boulders and over waterfalls. Gradually the river grows wider, the banks recede, the waters flow more quietly, and in the end, without any visible break, they become merged in the sea, and painlessly lose their individual being.";
const char *str3 = "Youth means a temperamental predominance of courage over timidity,of the appetite for adventure over the love of ease.This often exists in a man of sixty more than a boy of twenty.Nobody grows old merely by a number of years.We grow old by deserting our ideals.";
const char *str4 = "Whether sixty or sixteen, there is in every human being's heart the lure of wonders, the unfailing childlike appetite of what's next and the joy of the game of living. In the center of your heart and my heart there is a wireless station: so long as it receives messages of beauty, hope, cheer, courage and power from men and from the infinite, so long are you young.";

void drawLogo(void);


void setup()
{
    Serial.begin(115200);

    delay(500);

    // Get watch object
    twatch = TTGOClass::getWatch();

    //Call it to initialize the peripheral
    twatch->begin();

    //Get e-paper handle
    ePaper = twatch->ePaper;

    ePaper->setRotation(1);

    // Draw a logo
    drawLogo();

    //Wait for touch to be pressed
    while (!twatch->getTouch(x, y));

}

void drawLogo(void)
{
    int16_t x1, y1;
    uint16_t w, h;
    ePaper->setFont(&FreeSansBold24pt7b);
    ePaper->setTextColor(ePaper_INVERSE1);
    ePaper->setRotation(1);
    ePaper->clearDisplay();
    ePaper->fillTriangle(
        0, 0,
        ePaper->width() - 1, 0,
        0, ePaper->height() - 1,
        ePaper_GRAY2
    );
    ePaper->fillTriangle(
        0, ePaper->height() - 1,
        ePaper->width() - 1, 0,
        ePaper->width() - 1, ePaper->height() - 1,
        ePaper_GRAY1
    );

    ePaper->getTextBounds(str, 0, 0, &x1, &y1, &w, &h);
    ePaper->setCursor(ePaper->width() / 2 - w / 2, ePaper->height() / 2 - 14 );
    ePaper->print(str);

    ePaper->getTextBounds(str1, 0, 0, &x1, &y1, &w, &h);
    ePaper->setCursor(ePaper->width() / 2 - w / 2, ePaper->height() / 2 + 14 + h );
    ePaper->print(str1);

    ePaper->refreshDisplay();

}


void drawNext(const char *str)
{
    int16_t x1, y1;
    uint16_t w, h;
    ePaper->setFont(&FreeMono9pt7b);
    ePaper->setTextColor(ePaper_BLACK);
    ePaper->clearDisplay();
    ePaper->setCursor(20, 20);
    ePaper->println(str);

    ePaper->drawRoundRect(340, 200, 60, 25, 3, ePaper_BLACK);

    const char *btn = "Next";
    ePaper->getTextBounds(btn, 0, 0, &x1, &y1, &w, &h);
    ePaper->setCursor(340 + 10, 200 + 16);
    ePaper->print(btn);

    ePaper->refreshDisplay();

    while (1) {
        //Determine the touch area, and then the button will pop out
        if (twatch->getTouch( x1, y1)) {
            if ((x1 > 340 && x1 < 400) && (y1 > 200 && y1 < 225)) {
                break;
            }
        }
    }
}


void loop()
{
    drawNext(str2);
    drawNext(str3);
    drawNext(str4);
}
