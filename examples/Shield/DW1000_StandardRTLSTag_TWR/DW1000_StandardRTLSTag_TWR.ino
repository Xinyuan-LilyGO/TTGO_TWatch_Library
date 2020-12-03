/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/*
 *  #189 : Added SPI Port select . https://github.com/F-Army/arduino-dw1000-ng/pull/189
 *  PR has been submitted, if not merged, please download it in this warehouse
 *  https://github.com/lewisxhe/arduino-dw1000-ng
 */

/*
 * DW1000_StandardRTLSTag_TWR.ino
 *
 * This is an example tag in a RTLS using two way ranging ISO/IEC 24730-62_2013 messages
 */
#include "config.h"
#include <DW1000Ng.hpp>
#include <DW1000NgUtils.hpp>
#include <DW1000NgTime.hpp>
#include <DW1000NgConstants.hpp>
#include <DW1000NgRanging.hpp>
#include <DW1000NgRTLS.hpp>

#define SPI_SCLK        14
#define SPI_MISO        33
#define SPI_MOSI        15

#define DW1000_CS         13
#define DW1000_RST        26
#define DW1000_WAKEUP     25
#define DW1000_IRQ        34


// Extended Unique Identifier register. 64-bit device identifier. Register file: 0x01
char EUI[] = "AA:BB:CC:DD:EE:FF:00:00";

volatile uint32_t blink_rate = 50;

device_configuration_t DEFAULT_CONFIG = {
    false,
    true,
    true,
    true,
    false,
    SFDMode::STANDARD_SFD,
    Channel::CHANNEL_5,
    DataRate::RATE_6800KBPS,
    PulseFrequency::FREQ_64MHZ,
    PreambleLength::LEN_256,
    PreambleCode::CODE_3
};

frame_filtering_configuration_t TAG_FRAME_FILTER_CONFIG = {
    false,
    false,
    true,
    false,
    false,
    false,
    false,
    false
};

sleep_configuration_t SLEEP_CONFIG = {
    false,  // onWakeUpRunADC   reg 0x2C:00
    false,  // onWakeUpReceive
    false,  // onWakeUpLoadEUI
    true,   // onWakeUpLoadL64Param
    true,   // preserveSleep
    true,   // enableSLP    reg 0x2C:06
    false,  // enableWakePIN
    true    // enableWakeSPI
};


TTGOClass *watch;
SPIClass VSPI1(HSPI);

#ifdef LILYGO_WATCH_DRV2605
Adafruit_DRV2605 *drv;
#endif

#ifdef LILYGO_WATCH_HAS_DISPLAY
TFT_eSPI *tft;
TFT_eSprite *eSp;
#endif

uint32_t now_millis = 0;
uint8_t  state = 0;

void setup()
{
    Serial.begin(115200);

    watch = TTGOClass::getWatch();

    watch->begin();

    VSPI1.begin(SPI_SCLK, SPI_MISO, SPI_MOSI);

    // DW1000 Power
    watch->enableLDO3();

#ifdef LILYGO_WATCH_HAS_DISPLAY
    tft = watch->tft;
    watch->openBL();
    tft->drawCentreString("Start the module as a tag", 120, 100, 2);
    eSp = new TFT_eSprite(tft);
    eSp->createSprite(240, 60);
    eSp->setTextFont(2);
#endif


#ifdef LILYGO_WATCH_HAS_BUTTON
    drv = watch->drv;

    watch->enableDrv2650(true);

    drv->selectLibrary(1);
    // I2C trigger by sending 'go' command
    // default, internal trigger when sending GO command
    drv->setMode(DRV2605_MODE_INTTRIG);

    watch->button->setPressedHandler([]() {
        // set the effect to play
        drv->setWaveform(0, 75);  // play effect
        drv->setWaveform(1, 0);       // end waveform
        // play the effect!
        drv->go();
    });
#endif


    DW1000Ng::initialize(DW1000_CS, DW1000_IRQ, DW1000_RST, VSPI1);

    // DEBUG chip info and registers pretty printed
    char msg[128];
    DW1000Ng::getPrintableDeviceIdentifier(msg);
    Serial.print("Device ID: "); Serial.println(msg);
    if (!String(msg).startsWith("DECA")) {
        Serial.println("Failed to attch DW1000 Module");

#ifdef LILYGO_WATCH_HAS_DISPLAY
        tft->fillScreen(TFT_BLACK);
        tft->setTextColor(TFT_RED);
        tft->drawCentreString("Failed to attch DW1000 Module", 120, 100, 2);
#endif
        while (1);
    }
    DW1000Ng::getPrintableExtendedUniqueIdentifier(msg);
    Serial.print("Unique ID: "); Serial.println(msg);
    DW1000Ng::getPrintableNetworkIdAndShortAddress(msg);
    Serial.print("Network ID & Device Address: "); Serial.println(msg);
    DW1000Ng::getPrintableDeviceMode(msg);
    Serial.print("Device mode: "); Serial.println(msg);

    Serial.println("DW1000Ng initialized ...");

    // general configuration
    DW1000Ng::applyConfiguration(DEFAULT_CONFIG);
    DW1000Ng::enableFrameFiltering(TAG_FRAME_FILTER_CONFIG);

    DW1000Ng::setEUI(EUI);

    DW1000Ng::setNetworkId(RTLS_APP_ID);

    DW1000Ng::setAntennaDelay(16436);

    DW1000Ng::applySleepConfiguration(SLEEP_CONFIG);

    DW1000Ng::setPreambleDetectionTimeout(15);
    DW1000Ng::setSfdDetectionTimeout(273);
    DW1000Ng::setReceiveFrameWaitTimeoutPeriod(2000);

    Serial.println(F("Committed configuration ..."));
}


void loop()
{

#ifdef LILYGO_WATCH_HAS_BUTTON
    watch->button->loop();
#endif

    if (millis() - now_millis > blink_rate) {
        now_millis = millis();
        switch (state) {
        case 0:
            DW1000Ng::deepSleep();
            state = 1;
            break;
        case 1:
            DW1000Ng::spiWakeup();
            DW1000Ng::setEUI(EUI);
            RangeInfrastructureResult res = DW1000NgRTLS::tagTwrLocalize(1500);
            if (res.success)
                blink_rate = res.new_blink_rate;
            state = 0;
            break;
        }
    }
}
