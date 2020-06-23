#pragma mark - Depend ESP8266Audio and ESP8266_Spiram libraries
/*
cd ~/Arduino/libraries
git clone https://github.com/earlephilhower/ESP8266Audio
git clone https://github.com/Gianbacchio/ESP8266_Spiram
*/


// => Hardware select
// #define LILYGO_WATCH_2019_WITH_TOUCH     // To use T-Watch2019 with touchscreen, please uncomment this line
// #define LILYGO_WATCH_2019_NO_TOUCH       // To use T-Watch2019 Not touchscreen , please uncomment this line
// #define LILYGO_WATCH_BLOCK               // To use T-Watch Block , please uncomment this line
// #define LILYGO_WATCH_2020_V1             // To use T-Watch2020 , please uncomment this line

// This example requires an external IIS decoding module, otherwise it will not work

#include <LilyGoWatch.h>
#include <WiFi.h>
#include <HTTPClient.h>         //Remove Audio Lib error
#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "image.h"

TTGOClass *ttgo;
AudioGeneratorMP3 *mp3;
AudioFileSourcePROGMEM *file;
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
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextFont(2);
    ttgo->tft->setCursor(0, 0);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->println("Sample MP3 playback begins");

    //!Turn on the audio power
    ttgo->enableLDO3();

    file = new AudioFileSourcePROGMEM(image, sizeof(image));
    id3 = new AudioFileSourceID3(file);
    out = new AudioOutputI2S();
    //! External DAC decoding
    out->SetPinout(TWATCH_DAC_IIS_BCK, TWATCH_DAC_IIS_WS, TWATCH_DAC_IIS_DOUT);
    mp3 = new AudioGeneratorMP3();
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
