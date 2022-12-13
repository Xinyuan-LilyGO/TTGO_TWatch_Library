/*

  Sketch to show how a Sprite can use a four-bit image with
  a palette to change the appearance of an image while rendering
  it only once.

  Example for library:
  https://github.com/Bodmer/TFT_eSPI

  A Sprite is notionally an invisible graphics screen that is
  kept in the processors RAM. Graphics can be drawn into the
  Sprite just as it can be drawn directly to the screen. Once
  the Sprite is completed it can be plotted onto the screen in
  any position. If there is sufficient RAM then the Sprite can
  be the same size as the screen and used as a frame buffer.

  A 16 bit Sprite occupies (2 * width * height) bytes in RAM.

  On a ESP8266 Sprite sizes up to 126 x 160 can be accommodated,
  this size requires 40kBytes of RAM for a 16 bit color depth.

  When 8 bit color depth sprites are created they occupy
  (width * height) bytes in RAM, so larger sprites can be
  created, or the RAM required is halved.

*/


// Set delay after plotting the sprite
#define DELAY 30

// Width and height of sprite
#define SWIDTH  240
#define SHEIGHT 240
#include "image.h"
#include "config.h"                 // Include the graphics library (this includes the sprite functions)


// ImageConverter  : http://rinkydinkelectronics.com/t_imageconverter565.php
extern const unsigned short logoColor1[57600]  ;

TTGOClass   *watch  = NULL;
TFT_eSprite *spr    = NULL; // Sprite for meter reading
TFT_eSPI    *tft = NULL;

void setup()
{
    Serial.begin(115200);
    Serial.println();

    watch = TTGOClass::getWatch();

    watch->begin();

    watch->openBL();
    //Create a new pointer to save the display object
    tft = watch->tft;

    spr   = new TFT_eSprite(tft); // Sprite object for eSpDate

    spr->setColorDepth(8);

    // Create a sprite of defined size
    spr->createSprite(SWIDTH, SHEIGHT);

    // Clear the TFT screen to black
    tft->fillScreen(TFT_BLACK);

    // push the image - only need to do this once.
    spr->pushImage(0, 0, 240, 240, (uint16_t *)logoColor1);

    spr->pushSprite(0, 0);

}

void loop(void)
{
    delay(DELAY);

}
