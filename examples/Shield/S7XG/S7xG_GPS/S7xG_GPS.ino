// Old time example, not recommended
#include "config.h"

TTGOClass *ttgo;
TFT_eSPI *tft ;
S7XG_Class *s7xg;
char buff[256];

void setup(void)
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    //! Create a new pointer to save the display object
    tft = ttgo->tft;

    tft->fillScreen(TFT_BLACK);
    tft->setTextFont(2);
    tft->println("Begin S7XG Module...");

    //! Open s7xg power
    ttgo->enableLDO4();
    ttgo->enableLDO3();

    ttgo->s7xg_begin();
    s7xg = ttgo->s7xg;

    int len = 0;
    int retry = 0;
    do {
        len = s7xg->getHardWareModel().length();
        if (len == 0 && retry++ == 5) {
            s7xg->reset();
            retry = 0;
            Serial.println("Reset s7xg chip");
        }
        if (len == 0)
            delay(1000);
    } while (len == 0);

    Serial.println("Found s7xg module,Start gps module");

    s7xg->gpsReset();
    s7xg->gpsSetLevelShift(true);
    s7xg->gpsSetStart();
    s7xg->gpsSetSystem(0);
    s7xg->gpsSetPositioningCycle(1000);
    s7xg->gpsSetPortUplink(20);
    s7xg->gpsSetFormatUplink(1);
    s7xg->gpsSetMode(1);

    tft->setCursor(0, 0);
    tft->fillScreen(TFT_BLACK);
    tft->println("Positioning...");

}


void loop(void)
{
    GPS_Class gps =  s7xg->gpsGetData();
    if (gps.isVaild()) {
        tft->setCursor(0, 0);
        tft->fillRect(0, 0, 240, 50, TFT_BLACK);
        sprintf(buff, "Location: lat:%.02f lng:%.02f", gps.lat(), gps.lng());
        tft->println(buff);
        sprintf(buff, "Date: %d/%d/%d",  gps.year(), gps.month(), gps.day());
        tft->println(buff);
        sprintf(buff, "Time: %d:%d:%d",  gps.hour(), gps.minute(), gps.second());
        tft->println(buff);
    }
    delay(1000);
}


