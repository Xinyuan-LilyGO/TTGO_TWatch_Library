#pragma mark - Depend ESP8266Audio and ESP8266_Spiram libraries
/*
cd ~/Arduino/libraries
git clone https://github.com/earlephilhower/ESP8266Audio
git clone https://github.com/Gianbacchio/ESP8266_Spiram
*/

#include "config.h"
#include <WiFi.h>
#include <HTTPClient.h>         //Remove Audio Lib error
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

TTGOClass *ttgo;
AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

void setup()
{
    WiFi.mode(WIFI_OFF);
    delay(500);

    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();

#ifdef LILYGO_WATCH_HAS_DISPLAY
    ttgo->openBL();
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextFont(2);
#endif

    while (1) {
        if (ttgo->sdcard_begin()) {
            Serial.println("sd begin pass");
            break;
        }
#ifdef LILYGO_WATCH_HAS_DISPLAY
        ttgo->tft->println("SD begin fail,wait 1 sec");
#endif
        Serial.println("sd begin fail,wait 1 sec");
        delay(1000);
    }
#ifdef LILYGO_WATCH_HAS_DISPLAY
    ttgo->tft->setCursor(0, 0);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->println("Sample MP3 playback begins");
#endif
    // pno_cs from https://ccrma.stanford.edu/~jos/pasp/Sound_Examples.html
    //!Turn on the audio power
    ttgo->enableLDO3();

    file = new AudioFileSourceSD();
    id3 = new AudioFileSourceID3(file);
    out = new AudioOutputI2S(0, 1);
    mp3 = new AudioGeneratorMP3();
    file->open("/pno-cs.mp3");
    mp3->begin(id3, out);
}

void loop()
{
    if (mp3->isRunning()) {
        if (!mp3->loop()) mp3->stop();
    } else {
        Serial.printf("MP3 done\n");
        delay(1000);
    }
}
