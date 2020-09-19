#pragma mark - Depend ESP8266Audio and ESP8266_Spiram libraries
/*
cd ~/Arduino/libraries
git clone https://github.com/earlephilhower/ESP8266Audio
git clone https://github.com/Gianbacchio/ESP8266_Spiram
*/

#include "config.h"
#include <WiFi.h>
#include <HTTPClient.h>         //Remove Audio Lib error
#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "pika.h"

//! set 1 to external decoder playback,  set 0 to internal decoder
#define EXTERNAL_DAC_PLAY   1

TTGOClass *ttgo;
AudioGeneratorMP3 *mp3;
AudioFileSourcePROGMEM *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

void setup(void)
{
    WiFi.mode(WIFI_OFF);
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextFont(2);

    ttgo->tft->setCursor(0, 0);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->println("Sample alarm clock begins");

    file = new AudioFileSourcePROGMEM(pika, sizeof(pika));
    id3 = new AudioFileSourceID3(file);

#if EXTERNAL_DAC_PLAY
    out = new AudioOutputI2S();
    out->SetPinout(TWATCH_DAC_IIS_BCK, TWATCH_DAC_IIS_WS, TWATCH_DAC_IIS_DOUT);
#else
    out = new AudioOutputI2S(0, 1);
#endif

    mp3 = new AudioGeneratorMP3();
    mp3->begin(id3, out);

    //!Turn on the audio power
    ttgo->enableLDO3();
}


void loop(void)
{
    if (mp3->isRunning()) {
        if (!mp3->loop()) mp3->stop();
    } else {

        ttgo->tft->fillScreen(TFT_BLACK);
        ttgo->tft->println("alarm clock play done , sleep now ..");

        delay(1000);

        ttgo->rtc->disableAlarm();
        ttgo->rtc->setDateTime(2019, 4, 7, 9, 5, 50);       //Set current time
        ttgo->rtc->setAlarmByMinutes(6);                    //Match minute timing
        ttgo->rtc->enableAlarm();                           //Enable RTC Alarm

        ttgo->displayOff();                                 //Turn off display
        ttgo->powerOff();                                   //Turn off all peripheral power supplies

        //!Setting PCF8563 interrupt wakeup
        esp_sleep_enable_ext1_wakeup( ((uint64_t)(((uint64_t)1) << RTC_INT_PIN)), ESP_EXT1_WAKEUP_ALL_LOW);
        esp_deep_sleep_start();
    }
}




