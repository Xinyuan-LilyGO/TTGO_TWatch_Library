
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
volatile boolean received = false;
volatile boolean error = false;
volatile int16_t numReceived = 0; // todo check int type
String message;


void handleReceived()
{
    // status change on reception success
    received = true;
}

void handleError()
{
    error = true;
}

void receiver()
{
    DW1000.newReceive();
    DW1000.setDefaults();
    // so we don't need to restart the receiver manually
    DW1000.receivePermanently(true);
    DW1000.startReceive();
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
    DW1000.setDeviceAddress(6);
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
    // attach callback for (successfully) received messages
    DW1000.attachReceivedHandler(handleReceived);
    DW1000.attachReceiveFailedHandler(handleError);
    DW1000.attachErrorHandler(handleError);
    // start reception
    receiver();


#ifdef LILYGO_WATCH_HAS_DISPLAY
    tft = watch->tft;
    tft->drawCentreString("Start Receiver", 120, 100, 2);
    watch->openBL();
#endif
}

void loop()
{
    // enter on confirmation of ISR status change (successfully received)
    if (received) {
        numReceived++;
        // get data as string
        DW1000.getData(message);
        Serial.print("Received message ... #");
        Serial.println(numReceived);
        Serial.print("Data is ... ");
        Serial.println(message);
        Serial.print("FP power is [dBm] ... ");
        Serial.println(DW1000.getFirstPathPower());
        Serial.print("RX power is [dBm] ... ");
        Serial.println(DW1000.getReceivePower());
        Serial.print("Signal quality is ... ");
        Serial.println(DW1000.getReceiveQuality());
#ifdef LILYGO_WATCH_HAS_DISPLAY
        tft->setCursor(0, 0);
        tft->print("Received message ... #");
        tft->println(numReceived);
        tft->print("Data is ... ");
        tft->println(message);
        tft->print("FP power is [dBm] ... ");
        tft->println(DW1000.getFirstPathPower());
        tft->print("RX power is [dBm] ... ");
        tft->println(DW1000.getReceivePower());
        tft->print("Signal quality is ... ");
        tft->println(DW1000.getReceiveQuality());
#endif
        received = false;
    }
    if (error) {
        Serial.println("Error receiving a message");
        error = false;
        DW1000.getData(message);
        Serial.print("Error data is ... "); Serial.println(message);
    }
}
