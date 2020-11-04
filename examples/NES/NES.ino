/*
* Base on https://github.com/moononournation/arduino-nofrendo , by moononournation
* - Adapt to LilyGo Twatch series
* - Added Dabble App Bluetooth control method for watch without handle
* - Dabble App download link : https://play.google.com/store/apps/details?id=io.dabbleapp&hl=en_US&gl=US
*   Adapted to Twatch by Lewis 04/11/2020
* */

#pragma mark - Depend arduino-nofrendo  & DabbleESP32 library

/*
cd ~/Arduino/libraries
git clone https://github.com/Xinyuan-LilyGO/arduino-nofrendo.git

If you don’t use Dabble App, you don’t need it clone DabbleESP32
git clone https://github.com/Xinyuan-LilyGO/DabbleESP32.git
*/

#include <SPIFFS.h>
#include <FS.h>
#include <esp_wifi.h>
#include <esp_task_wdt.h>
#include "config.h"

extern "C"
{
#include <nofrendo.h>
#include <nes/nes.h>
}

#define FSROOT "/fs"

TTGOClass *watch;
TFT_eSPI *tft;
int16_t bg_color;

static int16_t frame_x, frame_y, frame_x_offset, frame_width, frame_height, frame_line_pixels;
extern uint16_t myPalette[];

void setup()
{
    Serial.begin(115200);

    watch = TTGOClass::getWatch();

    watch->begin();
    watch->openBL();

    tft = watch->tft;

    // turn off WiFi
    esp_wifi_deinit();

    // disable Core 0 WDT
    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
    esp_task_wdt_delete(idle_0);

    bg_color = tft->color565(24, 28, 24); // DARK DARK GREY
    tft->fillScreen(bg_color);
    tft->setCursor(0, 0);

#if defined(HW_CONTROLLER_GAMEPAD) || defined(LILYGO_WATCH_2019_WITH_TOUCH) || defined(LILYGO_WATCH_2019_NO_TOUCH)
    tft->setRotation(3);
#endif

    SPIFFS.begin(false, FSROOT);

    FS filesystem = SPIFFS;

    // find first rom file (*.nes)
    File root = filesystem.open("/");
    char *argv[1];
    if (!root) {
        Serial.println("Filesystem mount failed!");
        tft->println("Filesystem mount failed!");
    } else {
        bool foundRom = false;

        File file = root.openNextFile();
        while (file) {
            if (file.isDirectory()) {
                // skip
            } else {
                char *filename = (char *)file.name();
                int8_t len = strlen(filename);
                if (strstr(strlwr(filename + (len - 4)), ".nes")) {
                    foundRom = true;
                    char fullFilename[256];
                    sprintf(fullFilename, "%s%s", FSROOT, filename);
                    Serial.println(fullFilename);
                    argv[0] = fullFilename;
                    break;
                }
            }

            file = root.openNextFile();
        }

        if (!foundRom) {
            Serial.println("Failed to find rom file, please copy rom file to data folder and upload with \"ESP32 Sketch Data Upload\"");
            tft->println("Failed to find rom file, please copy rom file to data folder and upload with \"ESP32 Sketch Data Upload\"");
            argv[0] = "/";
        }

        Serial.println("NoFrendo start!\n");
        nofrendo_main(1, argv);
        Serial.println("NoFrendo end!\n");
    }
}

void loop()
{
}



extern "C" void display_init()
{
    frame_x = 0;
    frame_x_offset = (NES_SCREEN_WIDTH - tft->width()) / 2;
    frame_width = tft->width();
    frame_height = NES_SCREEN_HEIGHT;
    frame_line_pixels = frame_width;
    frame_y = (tft->height() - NES_SCREEN_HEIGHT) / 2;
}

extern "C" void display_write_frame(const uint8_t *data[])
{
    tft->setAddrWindow(frame_x, frame_y, frame_width, frame_height);
    tft->startWrite();
    for (int32_t i = 0; i < NES_SCREEN_HEIGHT; i++) {
        tft->writeIndexedPixels((uint8_t *)(data[i] + frame_x_offset), myPalette, frame_line_pixels);
    }
    tft->endWrite();
}

extern "C" void display_clear()
{
    tft->fillScreen(bg_color);
}