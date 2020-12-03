/*
 * MIT License
 *
 * Copyright (c) 2018 Michele Biondi, Andrea Salvatori
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

/*
 * Copyright (c) 2015 by Thomas Trojer <thomas@trojer.net>
 * Decawave DW1000Ng library for arduino.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file BasicSender.ino
 * Use this to test simple sender/receiver functionality with two
 * DW1000Ng:: Complements the "BasicReceiver" example sketch.
 *
 * @todo
 *  - move strings to flash (less RAM consumption)
 *
 */

/*
 *  DW1000_BasicSender.ino
 *  #189 : Added SPI Port select . https://github.com/F-Army/arduino-dw1000-ng/pull/189
 *  PR has been submitted, if not merged, please download it in this warehouse
 *  https://github.com/lewisxhe/arduino-dw1000-ng
 */

/*
 *  #189 : Added SPI Port select . https://github.com/F-Army/arduino-dw1000-ng/pull/189
 *  PR has been submitted, if not merged, please download it in this warehouse
 *  https://github.com/lewisxhe/arduino-dw1000-ng
 */


#include "config.h"
#include <DW1000Ng.hpp>

#define SPI_SCLK            14
#define SPI_MISO            33
#define SPI_MOSI            15
#define DW1000_CS           13
#define DW1000_RST          26
#define DW1000_WAKEUP       25
#define DW1000_IRQ          34

// DEBUG packet sent status and count
volatile unsigned long delaySent = 0;
int16_t sentNum = 0; // todo check int type

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


device_configuration_t DEFAULT_CONFIG = {
    false,
    true,
    true,
    true,
    false,
    SFDMode::STANDARD_SFD,
    Channel::CHANNEL_5,
    DataRate::RATE_850KBPS,
    PulseFrequency::FREQ_16MHZ,
    PreambleLength::LEN_256,
    PreambleCode::CODE_3
};


void transmit()
{
    // transmit some data
    Serial.print("Transmitting packet ... #"); Serial.println(sentNum);
    String msg = "Hello DW1000Ng, it's #"; msg += sentNum;
    DW1000Ng::setTransmitData(msg);
    // delay sending the message for the given amount
    delay(1000);
    DW1000Ng::startTransmit(TransmitMode::IMMEDIATE);
    delaySent = millis();
    while (!DW1000Ng::isTransmitDone()) {
    }
    sentNum++;
    DW1000Ng::clearTransmitStatus();
}

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
    tft->drawCentreString("DW1000Ng-arduino-sender-test", 120, 100, 2);
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
    //DW1000Ng::applyInterruptConfiguration(DEFAULT_INTERRUPT_CONFIG);

    DW1000Ng::setDeviceAddress(5);
    DW1000Ng::setNetworkId(10);

    DW1000Ng::setAntennaDelay(16436);
    Serial.println(F("Committed configuration ..."));

    // attach callback for (successfully) sent messages
    //DW1000Ng::attachSentHandler(handleSent);
    // start a transmission
    transmit();

}

void loop()
{
#ifdef LILYGO_WATCH_HAS_BUTTON
    watch->button->loop();
#endif

    transmit();

    // update and print some information about the sent message
    Serial.print("ARDUINO delay sent [ms] ... ");
    Serial.println(millis() - delaySent);
    uint64_t newSentTime = DW1000Ng::getTransmitTimestamp();
    Serial.print("Processed packet ... #");
    Serial.println(sentNum);

#ifdef LILYGO_WATCH_HAS_DISPLAY
    tft->fillRect(0, 0, 240, 60, TFT_BLACK);
    tft->setTextFont(2);
    tft->setCursor(0, 0);
    tft->print("ARDUINO delay sent [ms] ... ");
    tft->println(millis() - delaySent);
    tft->print("Processed packet ... #");
    tft->println(sentNum);
#endif
}