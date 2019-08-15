#pragma mark - Depend ESP8266Audio and ESP8266_Spiram libraries
/*
cd ~/Arduino/libraries
git clone https://github.com/earlephilhower/ESP8266Audio
git clone https://github.com/Gianbacchio/ESP8266_Spiram
*/

#include <TTGO.h>
#include <WiFi.h>
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
    ttgo->openBL();
    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setTextFont(2);

    while (1) {
        if (ttgo->sdcard_begin()) {
            Serial.println("sd begin pass");
            break;
        }
        ttgo->eTFT->println("SD begin fail,wait 1 sec");
        Serial.println("sd begin fail,wait 1 sec");
        delay(1000);
    }
    
    ttgo->eTFT->setCursor(0, 0);
    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->println("Sample MP3 playback begins");
    
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
