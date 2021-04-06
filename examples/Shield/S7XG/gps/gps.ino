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

struct GPSDataStruct data;
uint32_t utimerStart = 0;

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

void gpsBegin()
{
    // To reset GPS
    ret = s76g.gpsReset();
    CHECK_ERROR(ret);

    // Representing whether S76G enables the internal level shift IC
    ret = s76g.setLevelShift(true);
    CHECK_ERROR(ret);

    // Representing the way of how S76G/S78G starts to get position
    // S7XG_GPS_START_HOT
    // S7XG_GPS_START_WARM
    // S7XG_GPS_START_COLD
    ret = s76g.setStart(S7XG_GPS_START_HOT);
    CHECK_ERROR(ret);

    //representing S76G/S78G GPS satellite system is set to either
    // S7XG_SATELLITE_GPS
    // S7XG_SATELLITE_GPS_GLONASS
    ret = s76g.setSatelliteSystem(S7XG_SATELLITE_GPS);
    CHECK_ERROR(ret);

    //Representing the SONY CXD5603GF GPS positioning cycle time,
    //it can be 1000 to 600000 milliseconds.
    ret = s76g.setPositioningCycle(1000);
    CHECK_ERROR(ret);

    // Representing S76G/S78G GPS mode is set to either
    // S7XG_GPS_MODE_AUTO,
    // S7XG_GPS_MODE_MANUAL,
    // S7XG_GPS_MODE_IDLE,
    ret = s76g.setMode(S7XG_GPS_MODE_MANUAL);
    CHECK_ERROR(ret);

    GPSModeStruct mode;
    ret = s76g.getMode(mode);
    CHECK_ERROR(ret);

    //To show the TTFF (Time To First Fix) value of the last GPS positioning.
    float sec = 0;
    ret = s76g.getTtff(sec);
    CHECK_ERROR(ret);
    Serial.printf("TTFF is %f\n", sec);
}


void setup()
{
    setupWatch();
    gpsBegin();
}


void loop()
{
    if (millis() - utimerStart > 1000) {

        //GPS data type, which can be S7XG_GPS_DATA_DMS , S7XG_GPS_DATA_RAW , S7XG_GPS_DATA_DD
        GPSDataType type = S7XG_GPS_DATA_RAW;

        ret = s76g.getData(data, type);

        if (data.isValid) {
            tft->fillScreen(TFT_BLACK);
            tft->setCursor(0, 0);
            switch (type) {
            case S7XG_GPS_DATA_DMS:
                tft->print("date:");
                tft->print(data.dms.datetime.year);
                tft->print("/");
                tft->print(data.dms.datetime.month);
                tft->print("/");
                tft->print(data.dms.datetime.day);
                tft->print(" ");

                tft->print("time:");
                tft->print(data.dms.datetime.hour);
                tft->print(":");
                tft->print(data.dms.datetime.minute);
                tft->print(":");
                tft->print(data.dms.datetime.second);
                tft->println();

                tft->print("lat:");
                tft->print(data.dms.lat.dd);
                tft->print("/");
                tft->print(data.dms.lat.mm);
                tft->print("/");
                tft->print(data.dms.lat.ss);
                tft->println();

                tft->print("lng:");
                tft->print(data.dms.lng.dd);
                tft->print("/");
                tft->print(data.dms.lng.mm);
                tft->print("/");
                tft->print(data.dms.lng.ss);
                tft->println();


                Serial.printf("ret:%d  %hu/%hhu/%hhu %hhu:%hhu:%hhu dd:%d mm:%d ss:%lf dd:%d mm:%d ss:%lf [%f]\n",
                              ret,
                              data.dms.datetime.year,
                              data.dms.datetime.month,
                              data.dms.datetime.day,
                              data.dms.datetime.hour,
                              data.dms.datetime.minute,
                              data.dms.datetime.second,
                              data.dms.lat.dd, data.dms.lat.mm, data.dms.lat.ss,
                              data.dms.lng.dd, data.dms.lng.mm, data.dms.lng.ss,
                              data.second);
                break;
            case S7XG_GPS_DATA_RAW:

                tft->print("date:");
                tft->print(data.raw.datetime.year);
                tft->print("/");
                tft->print(data.raw.datetime.month);
                tft->print("/");
                tft->print(data.raw.datetime.day);
                tft->print(" ");

                tft->print("time:");
                tft->print(data.raw.datetime.hour);
                tft->print(":");
                tft->print(data.raw.datetime.minute);
                tft->print(":");
                tft->print(data.raw.datetime.second);
                tft->println();

                tft->print("lat:");
                tft->print(data.raw.lat);

                tft->print("lng:");
                tft->print(data.raw.lng);


                Serial.printf("ret:%d %u/%hhu/%hhu %hhu:%hhu:%hhu lat:%f lng:%f [%f]\n",
                              ret,
                              data.raw.datetime.year,
                              data.raw.datetime.month,
                              data.raw.datetime.day,
                              data.raw.datetime.hour,
                              data.raw.datetime.minute,
                              data.raw.datetime.second,
                              data.raw.lat,
                              data.raw.lng,
                              data.second);
                break;

            case S7XG_GPS_DATA_DD:
                tft->print("date:");
                tft->print(data.dd.datetime.year);
                tft->print("/");
                tft->print(data.dd.datetime.month);
                tft->print("/");
                tft->print(data.dd.datetime.day);
                tft->print(" ");

                tft->print("time:");
                tft->print(data.dd.datetime.hour);
                tft->print(":");
                tft->print(data.dd.datetime.minute);
                tft->print(":");
                tft->print(data.dd.datetime.second);
                tft->println();

                tft->print("lat:");
                tft->print(data.dd.lat);

                tft->print("lng:");
                tft->print(data.dd.lng);

                Serial.printf("ret:%d %hu/%hhu/%hhu %hhu:%hhu:%hhu lat:%f lng:%f [%f]\n",
                              ret,
                              data.dd.datetime.year,
                              data.dd.datetime.month,
                              data.dd.datetime.day,
                              data.dd.datetime.hour,
                              data.dd.datetime.minute,
                              data.dd.datetime.second,
                              data.dd.lat,
                              data.dd.lng,
                              data.second);
                break;

            default:

                break;
            }
        } else
            Serial.println(millis());
        utimerStart = millis();
    }
}



