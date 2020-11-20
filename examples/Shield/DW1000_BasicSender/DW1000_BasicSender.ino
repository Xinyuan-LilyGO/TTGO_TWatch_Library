
#pragma mark - Depend arduino-dw1000
/*
cd ~/Arduino/libraries
git clone https://github.com/Xinyuan-LilyGO/arduino-dw1000.git
*/
#include "config.h"
#include <DW1000.h>

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
#endif

// DEBUG packet sent status and count
boolean sent = false;
volatile boolean sentAck = false;
volatile unsigned long delaySent = 0;
int16_t sentNum = 0; // todo check int type
DW1000Time sentTime;


void handleSent()
{
    // status change on sent success
    sentAck = true;
}

void transmitter()
{
    // transmit some data
    Serial.print("Transmitting packet ... #"); Serial.println(sentNum);
    DW1000.newTransmit();
    DW1000.setDefaults();
    String msg = "Hello DW1000, it's #"; msg += sentNum;
    DW1000.setData(msg);
    // delay sending the message for the given amount
    DW1000Time deltaTime = DW1000Time(10, DW1000Time::MILLISECONDS);
    DW1000.setDelay(deltaTime);
    DW1000.startTransmit();
    delaySent = millis();
}


void setup()
{
    Serial.begin(115200);

    watch = TTGOClass::getWatch();

    watch->begin();

    watch->enableLDO3();

    VSPI1.begin(SPI_SCLK, SPI_MISO, SPI_MOSI);

    Serial.println(F("### DW1000-arduino-receiver-test ###"));
    // initialize the driver
    DW1000.begin(BU01_IRQ, BU01_RST, VSPI1);
    DW1000.select(BU01_CS);
    Serial.println(F("DW1000 initialized ..."));
    // general configuration
    DW1000.newConfiguration();
    DW1000.setDefaults();
    DW1000.setDeviceAddress(5);
    DW1000.setNetworkId(10);
    DW1000.enableMode(DW1000.MODE_LONGDATA_RANGE_LOWPOWER);
    DW1000.commitConfiguration();
    Serial.println(F("Committed configuration ..."));
    // DEBUG chip info and registers pretty printed
    char msg[128];
    DW1000.getPrintableDeviceIdentifier(msg);
    Serial.print("Device ID: "); Serial.println(msg);
    DW1000.getPrintableExtendedUniqueIdentifier(msg);
    Serial.print("Unique ID: "); Serial.println(msg);
    DW1000.getPrintableNetworkIdAndShortAddress(msg);
    Serial.print("Network ID & Device Address: "); Serial.println(msg);
    DW1000.getPrintableDeviceMode(msg);
    Serial.print("Device mode: "); Serial.println(msg);
    // attach callback for (successfully) sent messages
    DW1000.attachSentHandler(handleSent);
    // start a transmission
    transmitter();


#ifdef LILYGO_WATCH_HAS_DISPLAY
    tft = watch->tft;
    watch->openBL();
    tft->drawCentreString("Start BasicSender example", 120, 100, 2);
#endif
}

void loop()
{
    if (!sentAck) {
        return;
    }
    // continue on success confirmation
    // (we are here after the given amount of send delay time has passed)
    sentAck = false;
    // update and print some information about the sent message
    Serial.print("ARDUINO delay sent [ms] ... ");
    Serial.println(millis() - delaySent);
    DW1000Time newSentTime;
    DW1000.getTransmitTimestamp(newSentTime);
    Serial.print("Processed packet ... #");
    Serial.println(sentNum);
    Serial.print("Sent timestamp ... ");
    Serial.println(newSentTime.getAsMicroSeconds());

#ifdef LILYGO_WATCH_HAS_DISPLAY
    tft->setCursor(0, 0);
    tft->print("Processed packet ... #");
    tft->println(sentNum);
    tft->print("Sent timestamp ... ");
    tft->println(newSentTime.getAsMicroSeconds());
#endif
    // note: delta is just for simple demo as not correct on system time counter wrap-around
    Serial.print("DW1000 delta send time [ms] ... ");
    Serial.println((newSentTime.getAsMicroSeconds() - sentTime.getAsMicroSeconds()) * 1.0e-3);
    sentTime = newSentTime;
    sentNum++;
    // again, transmit some data
    transmitter();
}
