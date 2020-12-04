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
 * DW1000_StandardRTLSAnchorMain_TWR.ino
 *
 * This is an example master anchor in a RTLS using two way ranging ISO/IEC 24730-62_2013 messages
 */
#include "config.h"
#include <DW1000Ng.hpp>
#include <DW1000NgUtils.hpp>
#include <DW1000NgRanging.hpp>
#include <DW1000NgRTLS.hpp>

#define SPI_SCLK        14
#define SPI_MISO        33
#define SPI_MOSI        15

#define DW1000_CS         13
#define DW1000_RST        26
#define DW1000_WAKEUP     25
#define DW1000_IRQ        34

typedef struct Position {
    double x;
    double y;
} Position;


// Extended Unique Identifier register. 64-bit device identifier. Register file: 0x01
char EUI[] = "AA:BB:CC:DD:EE:FF:00:01";

Position position_self = {0, 0};
Position position_B = {3, 0};
Position position_C = {3, 2.5};

double range_self;
double range_B;
double range_C;

boolean received_B = false;

byte target_eui[8];
byte tag_shortAddress[] = {0x05, 0x00};

byte anchor_b[] = {0x02, 0x00};
uint16_t next_anchor = 2;
byte anchor_c[] = {0x03, 0x00};

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

frame_filtering_configuration_t ANCHOR_FRAME_FILTER_CONFIG = {
    false,
    false,
    true,
    false,
    false,
    false,
    false,
    true /* This allows blink frames */
};


TTGOClass *watch;
SPIClass VSPI1(HSPI);
char buff[128];

#ifdef LILYGO_WATCH_DRV2605
Adafruit_DRV2605 *drv;
#endif

#ifdef LILYGO_WATCH_HAS_DISPLAY
TFT_eSPI *tft;
TFT_eSprite *eSp;
#endif


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
    tft->drawCentreString("Start the module as a anchor", 120, 100, 2);
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

    // initialize the driver
    DW1000Ng::initialize(DW1000_CS, DW1000_IRQ, DW1000_RST, VSPI1);
    Serial.println(F("DW1000Ng initialized ..."));

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

    // general configuration
    DW1000Ng::applyConfiguration(DEFAULT_CONFIG);
    DW1000Ng::enableFrameFiltering(ANCHOR_FRAME_FILTER_CONFIG);

    DW1000Ng::setEUI(EUI);
    DW1000Ng::setPreambleDetectionTimeout(64);
    DW1000Ng::setSfdDetectionTimeout(273);
    DW1000Ng::setReceiveFrameWaitTimeoutPeriod(5000);

    DW1000Ng::setNetworkId(RTLS_APP_ID);
    DW1000Ng::setDeviceAddress(1);

    DW1000Ng::setAntennaDelay(16436);

    Serial.println(F("Committed configuration ..."));

}

/* using https://math.stackexchange.com/questions/884807/find-x-location-using-3-known-x-y-location-using-trilateration */
void calculatePosition(double &x, double &y)
{
    /* This gives for granted that the z plane is the same for anchor and tags */
    double A = ( (-2 * position_self.x) + (2 * position_B.x) );
    double B = ( (-2 * position_self.y) + (2 * position_B.y) );
    double C = (range_self * range_self) - (range_B * range_B) - (position_self.x * position_self.x) + (position_B.x * position_B.x) - (position_self.y * position_self.y) + (position_B.y * position_B.y);
    double D = ( (-2 * position_B.x) + (2 * position_C.x) );
    double E = ( (-2 * position_B.y) + (2 * position_C.y) );
    double F = (range_B * range_B) - (range_C * range_C) - (position_B.x * position_B.x) + (position_C.x * position_C.x) - (position_B.y * position_B.y) + (position_C.y * position_C.y);

    x = (C * E - F * B) / (E * A - B * D);
    y = (C * D - A * F) / (B * D - A * E);
}

void loop()
{
#ifdef LILYGO_WATCH_HAS_BUTTON
    watch->button->loop();
#endif
    if (DW1000NgRTLS::receiveFrame()) {
        size_t recv_len = DW1000Ng::getReceivedDataLength();
        byte recv_data[recv_len];
        DW1000Ng::getReceivedData(recv_data, recv_len);


        if (recv_data[0] == BLINK) {
            DW1000NgRTLS::transmitRangingInitiation(&recv_data[2], tag_shortAddress);
            DW1000NgRTLS::waitForTransmission();

            RangeAcceptResult result = DW1000NgRTLS::anchorRangeAccept(NextActivity::RANGING_CONFIRM, next_anchor);
            if (!result.success) return;
            range_self = result.range;

            float dbm = DW1000Ng::getReceivePower();
            String rangeString = "Range: ";
            rangeString += range_self;
            rangeString += " m";
            rangeString += "\t RX power: ";
            rangeString += dbm;
            rangeString += " dBm";
            Serial.println(rangeString);

#ifdef LILYGO_WATCH_HAS_DISPLAY
            eSp->fillSprite(TFT_BLACK);
            eSp->setTextColor(TFT_GREEN, TFT_BLACK);
            eSp->drawCentreString(String(range_self), 120, 0, 7);
            eSp->pushSprite(0, 80);

            snprintf(buff, 128, "RX power:%.1f", dbm);
            tft->drawCentreString(buff, 120, 180, 2);
#endif


        } else if (recv_data[9] == 0x60) {
            double range = static_cast<double>(DW1000NgUtils::bytesAsValue(&recv_data[10], 2) / 1000.0);
            String rangeReportString = "Range from: "; rangeReportString += recv_data[7];
            rangeReportString += " = "; rangeReportString += range;
            Serial.println(rangeReportString);
            if (received_B == false && recv_data[7] == anchor_b[0] && recv_data[8] == anchor_b[1]) {
                range_B = range;
                received_B = true;
            } else if (received_B == true && recv_data[7] == anchor_c[0] && recv_data[8] == anchor_c[1]) {
                range_C = range;
                double x, y;
                calculatePosition(x, y);
                String positioning = "Found position - x: ";
                positioning += x; positioning += " y: ";
                positioning += y;
                Serial.println(positioning);
                received_B = false;
            } else {
                received_B = false;
            }
        }
    }
}