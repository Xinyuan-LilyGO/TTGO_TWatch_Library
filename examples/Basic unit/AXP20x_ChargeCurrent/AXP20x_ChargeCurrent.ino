
// => Hardware select
// #define LILYGO_WATCH_2019_WITH_TOUCH        // To use T-Watch2019 with touchscreen, please uncomment this line
// #define LILYGO_WATCH_2019_NO_TOUCH       // To use T-Watch2019 Not touchscreen , please uncomment this line
// #define LILYGO_WATCH_BLOCK               // To use T-Watch Block , please uncomment this line
//#define LILYGO_WATCH_2020_V1              // To use T-Watch2020 , please uncomment this line

#include <LilyGoWatch.h>

TTGOClass *ttgo;
TFT_eSPI *tft;
void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    tft = ttgo->tft;

    tft->fillScreen(TFT_BLACK);
    tft->drawString("T-Watch AXP202",  0, 50, 2);
    tft->setTextFont(2);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);


    int cur =  ttgo->power->getChargeControlCur();
    tft->setCursor(0, 100);
    tft->print("Default Current: "); tft->print(cur); tft->println(" mA");

    tft->setCursor(0, 130);
    //axp202 allows maximum charging current of 1800mA, minimum 300mA
    tft->print("Set charge control current 500 mA");
    ttgo->power->setChargeControlCur(500);

    tft->setCursor(0, 160);
    cur =  ttgo->power->getChargeControlCur();
    tft->print("Setting Current: "); tft->print(cur); tft->println(" mA");

}

void loop()
{
}
