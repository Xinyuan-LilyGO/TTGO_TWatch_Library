
#pragma mark - Depend arduino-dw1000
/*
cd ~/Arduino/libraries
git clone https://github.com/Xinyuan-LilyGO/arduino-dw1000.git
*/
#include "config.h"
#include "DW1000Ranging.h"

#define SPI_SCLK        14
#define SPI_MISO        33
#define SPI_MOSI        15

#define BU01_CS         13
#define BU01_RST        26
#define BU01_WAKEUP     25
#define BU01_IRQ        4//34

TTGOClass *watch;
SPIClass VSPI1(HSPI);
char buff[128];

#ifdef LILYGO_WATCH_HAS_DISPLAY
TFT_eSPI *tft;
TFT_eSprite *eSp;
#endif

void newRange()
{
    float range = DW1000Ranging.getDistantDevice()->getRange();
    uint16_t addr = DW1000Ranging.getDistantDevice()->getShortAddress();
    float dbm = DW1000Ranging.getDistantDevice()->getRXPower();

    Serial.print("from: "); Serial.print(addr, HEX);
    Serial.print("\t Range: "); Serial.print(range); Serial.print(" m");
    Serial.print("\t RX power: "); Serial.print(dbm); Serial.println(" dBm");

#ifdef LILYGO_WATCH_HAS_DISPLAY
    snprintf(buff, 128, "From:%x", addr);
    tft->drawCentreString(buff, 60, 30, 2);

    eSp->fillSprite(TFT_BLACK);
    eSp->setTextColor(TFT_GREEN, TFT_BLACK);
    eSp->drawCentreString(String(range), 120, 0, 7);
    eSp->pushSprite(0, 80);

    snprintf(buff, 128, "RX power:%.1f", dbm);
    tft->drawCentreString(buff, 120, 180, 2);
#endif
}

void newDevice(DW1000Device *device)
{
    Serial.print("ranging init; 1 device added ! -> ");
    Serial.print(" short:");
    Serial.println(device->getShortAddress(), HEX);
#ifdef LILYGO_WATCH_HAS_DISPLAY
    tft->fillScreen(TFT_BLACK);
#endif
}

void inactiveDevice(DW1000Device *device)
{
    Serial.print("delete inactive device: ");
    Serial.println(device->getShortAddress(), HEX);
#ifdef LILYGO_WATCH_HAS_DISPLAY
    tft->fillScreen(TFT_BLACK);
    tft->drawCentreString("delete inactive device: " + String(device->getShortAddress()), 120, 100, 2);
#endif
}

void setup()
{
    Serial.begin(115200);

    watch = TTGOClass::getWatch();

    watch->begin();

    watch->enableLDO3();

    VSPI1.begin(SPI_SCLK, SPI_MISO, SPI_MOSI);

    //init the configuration
    DW1000Ranging.initCommunication(BU01_RST, BU01_CS, BU01_IRQ, VSPI1); //Reset, CS, IRQ pin
    //define the sketch as anchor. It will be great to dynamically change the type of module
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.attachNewDevice(newDevice);
    DW1000Ranging.attachInactiveDevice(inactiveDevice);
    //Enable the filter to smooth the distance
    DW1000Ranging.useRangeFilter(true);

    //we start the module as a tag
    DW1000Ranging.startAsTag("7D:00:22:EA:82:60:3B:9C", DW1000.MODE_LONGDATA_RANGE_ACCURACY);

#ifdef LILYGO_WATCH_HAS_DISPLAY
    tft = watch->tft;
    watch->openBL();
    tft->drawCentreString("Start the module as a tag", 120, 100, 2);
    eSp = new TFT_eSprite(tft);
    eSp->createSprite(240, 60);
    eSp->setTextFont(2);
#endif
}

void loop()
{
    DW1000Ranging.loop();
}
