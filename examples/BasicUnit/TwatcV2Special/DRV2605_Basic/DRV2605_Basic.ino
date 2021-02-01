#include "config.h"

TTGOClass *ttgo;
TFT_eSPI *tft;
Adafruit_DRV2605 *drv;
uint8_t effect = 1;

void setup()
{
    Serial.begin(115200);

    //Get watch instance
    ttgo = TTGOClass::getWatch();

    // Initialize the hardware
    ttgo->begin();

    // Turn on the backlight
    ttgo->openBL();

    //  Receive as a local variable for easy writing
    tft = ttgo->tft;
    drv = ttgo->drv;

    // Some display setting
    tft->setCursor(0, 0);
    tft->setTextColor(TFT_GREEN, TFT_BLACK);

    //Enable DRV2605
    ttgo->enableDrv2650();

    drv->selectLibrary(1);

    // I2C trigger by sending 'go' command
    // default, internal trigger when sending GO command
    drv->setMode(DRV2605_MODE_INTTRIG);
}

void printTFT(const char *str)
{
    if (tft->getCursorY() > 240) {
        tft->fillScreen(TFT_BLACK);
        tft->setCursor(0, 0);
    }
    tft->println(str);
    Serial.println(str);
}

void loop()
{
//  Serial.print("Effect #"); printTFT(effect);

    if (effect == 1) {
        printTFT("11.2 Waveform Library Effects List");
    }

    if (effect == 1) {
        printTFT(("1 Strong Click 100%"));
    }
    if (effect == 2) {
        printTFT(("2 Strong Click 60%"));
    }
    if (effect == 3) {
        printTFT(("3 Strong Click 30%"));
    }
    if (effect == 4) {
        printTFT(("4 Sharp Click 100%"));
    }
    if (effect == 5) {
        printTFT(("5 Sharp Click 60%"));
    }
    if (effect == 6) {
        printTFT(("6 Sharp Click 30%"));
    }
    if (effect == 7) {
        printTFT(("7 Soft Bump 100%"));
    }
    if (effect == 8) {
        printTFT(("8 Soft Bump 60%"));
    }
    if (effect == 9) {
        printTFT(("9 Soft Bump 30%"));
    }
    if (effect == 10) {
        printTFT(("10 Double Click 100%"));
    }
    if (effect == 11) {
        printTFT(("11 Double Click 60%"));
    }
    if (effect == 12) {
        printTFT(("12 Triple Click 100%"));
    }
    if (effect == 13) {
        printTFT(("13 Soft Fuzz 60%"));
    }
    if (effect == 14) {
        printTFT(("14 Strong Buzz 100%"));
    }
    if (effect == 15) {
        printTFT(("15 750 ms Alert 100%"));
    }
    if (effect == 16) {
        printTFT(("16 1000 ms Alert 100%"));
    }
    if (effect == 17) {
        printTFT(("17 Strong Click 1 100%"));
    }
    if (effect == 18) {
        printTFT(("18 Strong Click 2 80%"));
    }
    if (effect == 19) {
        printTFT(("19 Strong Click 3 60%"));
    }
    if (effect == 20) {
        printTFT(("20 Strong Click 4 30%"));
    }
    if (effect == 21) {
        printTFT(("21 Medium Click 1 100%"));
    }
    if (effect == 22) {
        printTFT(("22 Medium Click 2 80%"));
    }
    if (effect == 23) {
        printTFT(("23 Medium Click 3 60%"));
    }
    if (effect == 24) {
        printTFT(("24 Sharp Tick 1 100%"));
    }
    if (effect == 25) {
        printTFT(("25 Sharp Tick 2 80%"));
    }
    if (effect == 26) {
        printTFT(("26 Sharp Tick 3 – 60%"));
    }
    if (effect == 27) {
        printTFT(("27 Short Double Click Strong 1 – 100%"));
    }
    if (effect == 28) {
        printTFT(("28 Short Double Click Strong 2 – 80%"));
    }
    if (effect == 29) {
        printTFT(("29 Short Double Click Strong 3 – 60%"));
    }
    if (effect == 30) {
        printTFT(("30 Short Double Click Strong 4 – 30%"));
    }
    if (effect == 31) {
        printTFT(("31 Short Double Click Medium 1 – 100%"));
    }
    if (effect == 32) {
        printTFT(("32 Short Double Click Medium 2 – 80%"));
    }
    if (effect == 33) {
        printTFT(("33 Short Double Click Medium 3 – 60%"));
    }
    if (effect == 34) {
        printTFT(("34 Short Double Sharp Tick 1 – 100%"));
    }
    if (effect == 35) {
        printTFT(("35 Short Double Sharp Tick 2 – 80%"));
    }
    if (effect == 36) {
        printTFT(("36 Short Double Sharp Tick 3 – 60%"));
    }
    if (effect == 37) {
        printTFT(("37 Long Double Sharp Click Strong 1 – 100%"));
    }
    if (effect == 38) {
        printTFT(("38 Long Double Sharp Click Strong 2 – 80%"));
    }
    if (effect == 39) {
        printTFT(("39 Long Double Sharp Click Strong 3 – 60%"));
    }
    if (effect == 40) {
        printTFT(("40 Long Double Sharp Click Strong 4 – 30%"));
    }
    if (effect == 41) {
        printTFT(("41 Long Double Sharp Click Medium 1 – 100%"));
    }
    if (effect == 42) {
        printTFT(("42 Long Double Sharp Click Medium 2 – 80%"));
    }
    if (effect == 43) {
        printTFT(("43 Long Double Sharp Click Medium 3 – 60%"));
    }
    if (effect == 44) {
        printTFT(("44 Long Double Sharp Tick 1 – 100%"));
    }
    if (effect == 45) {
        printTFT(("45 Long Double Sharp Tick 2 – 80%"));
    }
    if (effect == 46) {
        printTFT(("46 Long Double Sharp Tick 3 – 60%"));
    }
    if (effect == 47) {
        printTFT(("47 Buzz 1 – 100%"));
    }
    if (effect == 48) {
        printTFT(("48 Buzz 2 – 80%"));
    }
    if (effect == 49) {
        printTFT(("49 Buzz 3 – 60%"));
    }
    if (effect == 50) {
        printTFT(("50 Buzz 4 – 40%"));
    }
    if (effect == 51) {
        printTFT(("51 Buzz 5 – 20%"));
    }
    if (effect == 52) {
        printTFT(("52 Pulsing Strong 1 – 100%"));
    }
    if (effect == 53) {
        printTFT(("53 Pulsing Strong 2 – 60%"));
    }
    if (effect == 54) {
        printTFT(("54 Pulsing Medium 1 – 100%"));
    }
    if (effect == 55) {
        printTFT(("55 Pulsing Medium 2 – 60%"));
    }
    if (effect == 56) {
        printTFT(("56 Pulsing Sharp 1 – 100%"));
    }
    if (effect == 57) {
        printTFT(("57 Pulsing Sharp 2 – 60%"));
    }
    if (effect == 58) {
        printTFT(("58 Transition Click 1 – 100%"));
    }
    if (effect == 59) {
        printTFT(("59 Transition Click 2 – 80%"));
    }
    if (effect == 60) {
        printTFT(("60 Transition Click 3 – 60%"));
    }
    if (effect == 61) {
        printTFT(("61 Transition Click 4 – 40%"));
    }
    if (effect == 62) {
        printTFT(("62 Transition Click 5 – 20%"));
    }
    if (effect == 63) {
        printTFT(("63 Transition Click 6 – 10%"));
    }
    if (effect == 64) {
        printTFT(("64 Transition Hum 1 – 100%"));
    }
    if (effect == 65) {
        printTFT(("65 Transition Hum 2 – 80%"));
    }
    if (effect == 66) {
        printTFT(("66 Transition Hum 3 – 60%"));
    }
    if (effect == 67) {
        printTFT(("67 Transition Hum 4 – 40%"));
    }
    if (effect == 68) {
        printTFT(("68 Transition Hum 5 – 20%"));
    }
    if (effect == 69) {
        printTFT(("69 Transition Hum 6 – 10%"));
    }
    if (effect == 70) {
        printTFT(("70 Transition Ramp Down Long Smooth 1 – 100 to 0%"));
    }
    if (effect == 71) {
        printTFT(("71 Transition Ramp Down Long Smooth 2 – 100 to 0%"));
    }
    if (effect == 72) {
        printTFT(("72 Transition Ramp Down Medium Smooth 1 – 100 to 0%"));
    }
    if (effect == 73) {
        printTFT(("73 Transition Ramp Down Medium Smooth 2 – 100 to 0%"));
    }
    if (effect == 74) {
        printTFT(("74 Transition Ramp Down Short Smooth 1 – 100 to 0%"));
    }
    if (effect == 75) {
        printTFT(("75 Transition Ramp Down Short Smooth 2 – 100 to 0%"));
    }
    if (effect == 76) {
        printTFT(("76 Transition Ramp Down Long Sharp 1 – 100 to 0%"));
    }
    if (effect == 77) {
        printTFT(("77 Transition Ramp Down Long Sharp 2 – 100 to 0%"));
    }
    if (effect == 78) {
        printTFT(("78 Transition Ramp Down Medium Sharp 1 – 100 to 0%"));
    }
    if (effect == 79) {
        printTFT(("79 Transition Ramp Down Medium Sharp 2 – 100 to 0%"));
    }
    if (effect == 80) {
        printTFT(("80 Transition Ramp Down Short Sharp 1 – 100 to 0%"));
    }
    if (effect == 81) {
        printTFT(("81 Transition Ramp Down Short Sharp 2 – 100 to 0%"));
    }
    if (effect == 82) {
        printTFT(("82 Transition Ramp Up Long Smooth 1 – 0 to 100%"));
    }
    if (effect == 83) {
        printTFT(("83 Transition Ramp Up Long Smooth 2 – 0 to 100%"));
    }
    if (effect == 84) {
        printTFT(("84 Transition Ramp Up Medium Smooth 1 – 0 to 100%"));
    }
    if (effect == 85) {
        printTFT(("85 Transition Ramp Up Medium Smooth 2 – 0 to 100%"));
    }
    if (effect == 86) {
        printTFT(("86 Transition Ramp Up Short Smooth 1 – 0 to 100%"));
    }
    if (effect == 87) {
        printTFT(("87 Transition Ramp Up Short Smooth 2 – 0 to 100%"));
    }
    if (effect == 88) {
        printTFT(("88 Transition Ramp Up Long Sharp 1 – 0 to 100%"));
    }
    if (effect == 89) {
        printTFT(("89 Transition Ramp Up Long Sharp 2 – 0 to 100%"));
    }
    if (effect == 90) {
        printTFT(("90 Transition Ramp Up Medium Sharp 1 – 0 to 100%"));
    }
    if (effect == 91) {
        printTFT(("91 Transition Ramp Up Medium Sharp 2 – 0 to 100%"));
    }
    if (effect == 92) {
        printTFT(("92 Transition Ramp Up Short Sharp 1 – 0 to 100%"));
    }
    if (effect == 93) {
        printTFT(("93 Transition Ramp Up Short Sharp 2 – 0 to 100%"));
    }
    if (effect == 94) {
        printTFT(("94 Transition Ramp Down Long Smooth 1 – 50 to 0%"));
    }
    if (effect == 95) {
        printTFT(("95 Transition Ramp Down Long Smooth 2 – 50 to 0%"));
    }
    if (effect == 96) {
        printTFT(("96 Transition Ramp Down Medium Smooth 1 – 50 to 0%"));
    }
    if (effect == 97) {
        printTFT(("97 Transition Ramp Down Medium Smooth 2 – 50 to 0%"));
    }
    if (effect == 98) {
        printTFT(("98 Transition Ramp Down Short Smooth 1 – 50 to 0%"));
    }
    if (effect == 99) {
        printTFT(("99 Transition Ramp Down Short Smooth 2 – 50 to 0%"));
    }
    if (effect == 100) {
        printTFT(("100 Transition Ramp Down Long Sharp 1 – 50 to 0%"));
    }
    if (effect == 101) {
        printTFT(("101 Transition Ramp Down Long Sharp 2 – 50 to 0%"));
    }
    if (effect == 102) {
        printTFT(("102 Transition Ramp Down Medium Sharp 1 – 50 to 0%"));
    }
    if (effect == 103) {
        printTFT(("103 Transition Ramp Down Medium Sharp 2 – 50 to 0%"));
    }
    if (effect == 104) {
        printTFT(("104 Transition Ramp Down Short Sharp 1 – 50 to 0%"));
    }
    if (effect == 105) {
        printTFT(("105 Transition Ramp Down Short Sharp 2 – 50 to 0%"));
    }
    if (effect == 106) {
        printTFT(("106 Transition Ramp Up Long Smooth 1 – 0 to 50%"));
    }
    if (effect == 107) {
        printTFT(("107 Transition Ramp Up Long Smooth 2 – 0 to 50%"));
    }
    if (effect == 108) {
        printTFT(("108 Transition Ramp Up Medium Smooth 1 – 0 to 50%"));
    }
    if (effect == 109) {
        printTFT(("109 Transition Ramp Up Medium Smooth 2 – 0 to 50%"));
    }
    if (effect == 110) {
        printTFT(("110 Transition Ramp Up Short Smooth 1 – 0 to 50%"));
    }
    if (effect == 111) {
        printTFT(("111 Transition Ramp Up Short Smooth 2 – 0 to 50%"));
    }
    if (effect == 112) {
        printTFT(("112 Transition Ramp Up Long Sharp 1 – 0 to 50%"));
    }
    if (effect == 113) {
        printTFT(("113 Transition Ramp Up Long Sharp 2 – 0 to 50%"));
    }
    if (effect == 114) {
        printTFT(("114 Transition Ramp Up Medium Sharp 1 – 0 to 50%"));
    }
    if (effect == 115) {
        printTFT(("115 Transition Ramp Up Medium Sharp 2 – 0 to 50%"));
    }
    if (effect == 116) {
        printTFT(("116 Transition Ramp Up Short Sharp 1 – 0 to 50%"));
    }
    if (effect == 117) {
        printTFT(("117 Transition Ramp Up Short Sharp 2 – 0 to 50%"));
    }
    if (effect == 118) {
        printTFT(("118 Long buzz for programmatic stopping – 100%"));
    }
    if (effect == 119) {
        printTFT(("119 Smooth Hum 1 (No kick or brake pulse) – 50%"));
    }
    if (effect == 120) {
        printTFT(("120 Smooth Hum 2 (No kick or brake pulse) – 40%"));
    }
    if (effect == 121) {
        printTFT(("121 Smooth Hum 3 (No kick or brake pulse) – 30%"));
    }
    if (effect == 122) {
        printTFT(("122 Smooth Hum 4 (No kick or brake pulse) – 20%"));
    }
    if (effect == 123) {
        printTFT(("123 Smooth Hum 5 (No kick or brake pulse) – 10%"));
    }

    // set the effect to play
    drv->setWaveform(0, effect);  // play effect
    drv->setWaveform(1, 0);       // end waveform

    // play the effect!
    drv->go();

    // wait a bit
    delay(500);

    effect++;
    if (effect > 117) effect = 1;
}

