#pragma mark - Depend Acsip-S7xG-Library
/*
cd ~/Arduino/libraries
git clone https://github.com/lewisxhe/Acsip-S7xG-Library.git
*/


// Example source: https://github.com/lewisxhe/Acsip-S7xG-Library
#include "config.h"
#include <acsip.h>
#include "acsiplogo.h"

TTGOClass *watch = nullptr;
TFT_eSPI *tft;
HardwareSerial *hwSerial = nullptr;
Acsip s76g;

#define CHECK_ERROR(ret)        do{                                                     \
                                    if(ret != S7XG_OK){                                 \
                                        Serial.printf("%d failed\n", __LINE__);         \
                                        while (1);                                      \
                                    }                                                   \
                                }while(0)

int ret = 0;

void showResult(bool r)
{
    tft->print(" [");
    tft->setTextColor(r ? TFT_GREEN : TFT_RED);
    tft->print(r ? "OK" : "FAIL");
    tft->setTextColor(TFT_WHITE);
    tft->println("]");
}


void setupWatch()
{

    Serial.begin(115200);

    watch = TTGOClass::getWatch();
    watch->begin();
    tft = watch->tft;

    tft->fillScreen(TFT_BLACK);

    tft->setTextFont(2);

    watch->openBL();

    tft->setAddrWindow(30, 25, 168, 77);
    tft->pushColors((uint16_t *)acsiplogo, 0x3288);

    watch->enableLDO4();
    watch->enableLDO3();

    hwSerial = new HardwareSerial(1);
    hwSerial->begin(115200, SERIAL_8N1, GPS_RX, GPS_TX);
    if (!s76g.begin(*hwSerial)) {
        tft->setTextColor(TFT_RED);
        tft->println("Initialization 'S76/78G' failed");
        while (1);
    }

    tft->setCursor(0, 100);
    tft->print("FW version:");
    tft->println(s76g.getVersion());

    tft->print("Hardware:");
    tft->println(s76g.getHardWareVer());

    tft->print("UUID:");
    tft->println(s76g.getUUID());

    uint32_t r1, r2;
    tft->print("ADC resistor: ");
    ret = s76g.getBatteryResistor(r1, r2);
    tft->print("R1:");
    tft->print(r1);
    tft->print(" ");
    tft->print("R2:");
    tft->print(r2);
    showResult(ret == S7XG_OK);

    uint16_t volt;
    tft->print("ADC Volt: ");
    ret = s76g.getBatteryVoltage(volt);
    tft->print(volt);
    tft->print(" mV");
    showResult(ret == S7XG_OK);

    tft->print("Set gpio [PB6] OUTPUT");
    ret = s76g.setGPIOMode(S7XG_GPIO_GROUP_B, 6, OUTPUT);

    if (ret == S7XG_OK) {
        showResult(true);
        tft->print("Set gpio [PB6] to HIGH");
        ret = s76g.setGPIOValue(S7XG_GPIO_GROUP_B, 6, HIGH);
        showResult(ret == S7XG_OK);

        tft->print("Get gpio [PB6] :");
        int val = s76g.getGPIOValue(S7XG_GPIO_GROUP_B, 6);
        tft->print(val ? "HIGH" : "LOW");
        showResult(ret == S7XG_OK);
    }

    delay(3000);

}

void rfReceiveCallback(const char *data, int rssi, int snr)
{
    Serial.print("receive:");
    Serial.print(data);
    Serial.print(" ");
    Serial.print("rssi:");
    Serial.print(rssi);
    Serial.print(" ");
    Serial.print("snr:");
    Serial.print(snr);
    Serial.println();
}

void rfBegin()
{
    uint32_t freq = 0;
    ret = s76g.getRfFreq(freq);
    CHECK_ERROR(ret);

    //Representing communication frequency in Hz, it can be values from 862000000 to 932000000 (S76S); 137000000 to 525000000 (S78S).
    freq = 915000000;
    ret = s76g.setRfFreq(freq);
    CHECK_ERROR(ret);

    ret = s76g.getRfFreq(freq);
    CHECK_ERROR(ret);
    Serial.printf("freq = %u\n", freq);


    //Representing transmitting power in dBm, it can be from 2 to 20.
    uint8_t dBm = 20;
    ret = s76g.setRfPower(dBm);
    CHECK_ERROR(ret);

    ret = s76g.getRfPower(dBm);
    CHECK_ERROR(ret);
    Serial.printf("dBm = %u\n", dBm);

    uint8_t sf = 0;
    s76g.getRfSpreadingFactor(sf);
    CHECK_ERROR(ret);
    Serial.printf("SpreadingFactor = %u\n", sf);

    //Representing signal bandwidth in kHz, it can be: 125, 250, 500.
    uint16_t bw = 125;
    s76g.setRfBandWitdth(bw);

    //Set the receive callback function
    s76g.setRFCallback(rfReceiveCallback);

    //Turn on continuous reception mode
    ret = s76g.setReceiveContinuous(true);
    CHECK_ERROR(ret);
}


void setup()
{
    setupWatch();
    rfBegin();
}

void loop()
{
    // rf service function
    s76g.service();
}



