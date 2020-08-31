#pragma mark - Depend btAudio library
/*

cd ~/Arduino/libraries
git clone https://github.com/tierneytim/btAudio


For more examples, please refer to btAudio

*/


#include "config.h"
#include <btAudio.h>

TTGOClass *ttgo;
TFT_eSPI *tft ;

// Sets the name of the audio device
btAudio audio = btAudio("ESP_Speaker");

void setup(void)
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    //! Create a new pointer to save the display object
    tft = ttgo->tft;
    tft->setTextFont(2);
    tft->println("bluetooth audio example");

    //! Open aduio power
    ttgo->enableLDO3();

    // start bluetooth audio
    audio.begin();

    //  attach to pins
    int bck = TWATCH_DAC_IIS_BCK;
    int ws = TWATCH_DAC_IIS_WS;
    int dout = TWATCH_DAC_IIS_DOUT;
    audio.I2S(bck, dout, ws);
}

void loop(void)
{
    // put your code here
}
