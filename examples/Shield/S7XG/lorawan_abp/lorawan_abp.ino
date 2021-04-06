#pragma mark - Depend CayenneLPP and ArduinoJson libraries , Acsip-S7xG-Library
/*
cd ~/Arduino/libraries
git clone https://github.com/ElectronicCats/CayenneLPP.git
git clone https://github.com/bblanchon/ArduinoJson.git
git clone https://github.com/lewisxhe/Acsip-S7xG-Library.git
*/

// Example source: https://github.com/lewisxhe/Acsip-S7xG-Library



#include "config.h"
#include <acsip.h>
#include "acsiplogo.h"
#include <CayenneLPP.h>
#include <ArduinoJson.h>


TTGOClass *watch = nullptr;
TFT_eSPI *tft;
HardwareSerial *hwSerial = nullptr;
Acsip s76g;
CayenneLPP lpp(160);

struct GPSDataStruct data;
uint32_t utimerStart = 0;

#define CHECK_ERROR(ret)        do{                                                     \
                                    if(ret != S7XG_OK){                                 \
                                        Serial.printf("%d failed\n", __LINE__);         \
                                        while (1);                                      \
                                    }                                                   \
                                }while(0)


const char *deveui = "your ttn deveui";
const char *appeui = "your ttn appeui";
const char *devadr = "your ttn devaddr";
const char *nwskey = "your ttn network session key";
const char *appskey = "your ttn app session key";

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

void lorawanBegin()
{
    char cla =  s76g.getClass();
    Serial.printf("getClass -> %c\n", cla);

    //Set Class A
    ret = s76g.setClass('A');
    CHECK_ERROR(ret);

    bool isJoin = s76g.isJoin();
    Serial.printf("S76G is %s\n", isJoin ? "joined" : "unjoined");

    int pwr = 0;
    ret = s76g.getPower(pwr);
    CHECK_ERROR(ret);
    Serial.printf("S76G power level is %d\n", pwr);

    //Set Device Address
    ret = s76g.setDevAddr(devadr);
    CHECK_ERROR(ret);

    //Set Network Session Key
    ret = s76g.setNetworkSessionKey(nwskey);
    CHECK_ERROR(ret);
    //Set App Session Key
    ret = s76g.setAppSessionKey(appskey);
    CHECK_ERROR(ret);

    //Set the corresponding channel frequency
    s76g.setChannelFreq(0, 868100000);
    s76g.setChannelFreq(1, 868300000);
    s76g.setChannelFreq(2, 868500000);
    s76g.setChannelFreq(3, 867100000);
    s76g.setChannelFreq(4, 867300000);
    s76g.setChannelFreq(5, 867500000);
    s76g.setChannelFreq(6, 867700000);
    s76g.setChannelFreq(7, 867900000);
    s76g.setChannelFreq(8, 868800000);

    //Connection method using ABP
    s76g.joinABP();

    //Waiting to connect to TTN
    while (1) {
        if (s76g.isJoin()) {
            break;
        }
        Serial.println("Wait for join to TTN");
        delay(1500);
    }

    Serial.println();

    Serial.println();

    Serial.println("Join to TTN");
}


void setup()
{
    setupWatch();
    lorawanBegin();
}


void loop()
{
    if (millis() - utimerStart > 20000) {

        //Upload random data
        lpp.reset();
        lpp.addTemperature(1, rand() % 30);
        lpp.addBarometricPressure(2,  rand() % 3000);
        lpp.addAnalogOutput(3, rand() % 100 + 10);

        int uplinkPort = 1;
        ret = s76g.send(uplinkPort, lpp.getBuffer(), lpp.getSize());
        Serial.printf("Send [%u] Byte\n", lpp.getSize());
        if (ret != S7XG_OK) {
            hwSerial->flush();
        }
        utimerStart = millis();
    }
}



