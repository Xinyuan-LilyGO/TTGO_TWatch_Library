#pragma mark - Depend on the following libraries

/*

// This code is only suitable for T-Block
// This code is only suitable for T-Block
// This code is only suitable for T-Block

cd ~/Arduino/libraries
git clone https://github.com/ZinggJM/GxEPD2.git
git clone https://github.com/olikraus/U8g2_for_Adafruit_GFX.git
git clone https://github.com/adafruit/Adafruit-GFX-Library.git


// Delete the Fonts folder because it conflicts with TWatch
rm -rf ./Adafruit-GFX-Library/Fonts

* * */


#include <TTGO.h>

extern const unsigned char logoIcon[280];

/******************************************************************
 *                      GXEPD2
 * ***************************************************************/
// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>

#define INK_BUSY                                    34
#define INK_RESET                                   27
#define INK_DC                                      19
#define INK_SS                                      5
#define INK_MOSI                                    23
#define INK_MISO                                    19 //eink no use
#define INK_CLK                                     18

#define DEFAULT_BRIGHTNESS                          45

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(INK_SS,  INK_DC,  INK_RESET, INK_BUSY)); // GDEH0154D67

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
TTGOClass *twatch = nullptr;
PCF8563_Class *rtc = nullptr;
BMA *sensor = nullptr;
AXP20X_Class *power = nullptr;
Button2 *btn = nullptr;
uint32_t seupCount = 0;
bool bmaIRQ = false;
bool pwIRQ = false;


void setupDisplay()
{
    SPI.begin(INK_CLK, INK_MISO, INK_MOSI);
    display.init(); // enable diagnostic output on Serial
    display.setRotation(0);
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(0, 0);
    display.setFont(&FreeSerif24pt7b);

    u8g2Fonts.begin(display); // connect u8g2 procedures to Adafruit GFX
    u8g2Fonts.setFontMode(1);                   // use u8g2 transparent mode (this is default)
    u8g2Fonts.setFontDirection(0);              // left to right (this is default)
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // apply Adafruit GFX color
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // apply Adafruit GFX color
    u8g2Fonts.setFont(u8g2_font_inr38_mn); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
}

void mainPage(bool fullScreen)
{
    uint16_t tbw, tbh;
    static int16_t lastX, lastY;
    static uint16_t lastW, lastH;
    static uint8_t hh = 0, mm = 0;
    static uint8_t lastWeek = 0;
    static uint8_t lastDay = 0;
    static uint32_t lastSeupCount = 0;
    static int16_t getX, getY;
    static uint16_t getW, getH;
    char buff[64] = "00:00";
    bool reverse = false;
    const char *weekChars[] = { "日", "一", "二", "三", "四", "五", "六"};

    if (lastSeupCount != seupCount) {
        lastSeupCount = seupCount;
        u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312a); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
        display.setPartialWindow(getX - getW, getY  - getH  + 1, getW + 1, getH + 1);
        display.firstPage();
        do {
            snprintf(buff, sizeof(buff), "%u 步", seupCount);
            u8g2Fonts.setCursor(getX - getW, getY);
            u8g2Fonts.print(buff);

        } while (display.nextPage());
    }

    RTC_Date d = rtc->getDateTime();
    if (mm == d.minute && !fullScreen) {
        return ;
    }

    mm = d.minute;
    hh = d.hour;
    if (lastDay != d.day) {
        lastDay = d.day;
        lastWeek  = rtc->getDayOfWeek(d.day, d.month, d.year);
        fullScreen = true;
    }

    snprintf(buff, sizeof(buff), "%02d:%02d", hh, mm);

    if (fullScreen) {
        lastX = 25;
        lastY = 100;
        display.setFullWindow();
        display.firstPage();
        do {
            display.drawBitmap(5, 5, logoIcon, 75, 28, GxEPD_BLACK);

            //BATTERY ICON
            u8g2Fonts.setFont(u8g2_font_battery19_tn);
            u8g2Fonts.setCursor(175, 20);                // start writing at this position
            u8g2Fonts.setFontDirection(3);              // left to right (this is default)
            u8g2Fonts.print(4);
            u8g2Fonts.setFontDirection(0);              // left to right (this is default)


            display.drawFastHLine(10, 40, display.width() - 20, GxEPD_BLACK);
            display.drawFastHLine(10, 150, display.width() - 20, GxEPD_BLACK);

            u8g2Fonts.setFont(u8g2_font_inr38_mn  ); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall

            u8g2Fonts.setCursor(lastX, lastY);                // start writing at this position
            u8g2Fonts.print(buff);

            /* calculate the size of the box into which the text will fit */
            lastH = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent();
            lastW = u8g2Fonts.getUTF8Width(buff);

            u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312a); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall

            snprintf(buff, sizeof(buff), "星期%s", weekChars[lastWeek]);

            tbh = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent();
            tbw = u8g2Fonts.getUTF8Width(buff);

            int16_t x, y;
            x = ((display.width() - tbw) / 2) ;
            y = ((display.height() - tbh) / 2) + 40  ;

            u8g2Fonts.setCursor(x, y);
            u8g2Fonts.print(buff);
            u8g2Fonts.setCursor(20, y + 50);

            u8g2Fonts.print("你今天已经走了:");
            u8g2Fonts.print("1000步");

            getX = u8g2Fonts.getCursorX();
            getY = u8g2Fonts.getCursorY();
            getH  = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent();
            getW = u8g2Fonts.getUTF8Width("1000步");

        } while (display.nextPage());

    } else {
        u8g2Fonts.setFont(u8g2_font_inr38_mn); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
        display.setPartialWindow(lastX, lastY - u8g2Fonts.getFontAscent() - 3, lastW, lastH);
        display.firstPage();
        do {

            if (reverse) {
                display.fillScreen(GxEPD_BLACK);
                display.setTextColor(GxEPD_WHITE);

            } else {
                display.fillScreen(GxEPD_WHITE);
                display.setTextColor(GxEPD_BLACK);
            }


            lastW = u8g2Fonts.getUTF8Width(buff);

            u8g2Fonts.setCursor(lastX, lastY);
            u8g2Fonts.print(buff);
        } while (display.nextPage());
    }
}

void setup()
{
    Serial.begin(115200);

    delay(500);

    // Get watch object
    twatch = TTGOClass::getWatch();

    // The default initialization is the basic watch,
    // given two parameters to skip initializing the touch screen and TFT
    // void begin(bool tft = true, bool touchScreen = true)
    twatch->begin(false, false);

    // Turn on the backlight
    twatch->openBL();

    rtc = twatch->rtc;

    power = twatch->power;

    sensor = twatch->bma;

    btn = twatch->button;

    // Use compile time as RTC input time
    rtc->check();

    // Turn on power management button interrupt
    power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);

    // Clear power interruption
    power->clearIRQ();

    // Set Pin to interrupt
    pinMode(BMA423_INT1, INPUT);
    attachInterrupt(BMA423_INT1, [] {
        bmaIRQ = true;
    }, RISING);

    // Set Pin to interrupt
    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        pwIRQ = true;
    }, FALLING);

    btn->setPressedHandler([]() {
        esp_restart();
    });

    // Adjust the backlight to reduce current consumption
    twatch->bl->adjust(DEFAULT_BRIGHTNESS);

    // Initialize the accelerometer
    sensor->begin();

    // Turn on accelerometer interrupt
    sensor->attachInterrupt();

    // Initialize the ink screen
    setupDisplay();

    // Initialize the interface
    mainPage(true);

    // Reduce CPU frequency
    setCpuFrequencyMhz(40);
}

uint32_t loopMillis = 0;

void loop()
{
    btn->loop();

    if (pwIRQ) {
        pwIRQ = false;

        // Get interrupt status
        power->readIRQ();

        if (power->isPEKShortPressIRQ()) {
            twatch->bl->isOn() ? twatch->bl->off() : twatch->bl->adjust(DEFAULT_BRIGHTNESS);
        }
        // After the interruption, you need to manually clear the interruption status
        power->clearIRQ();
    }

    if (bmaIRQ) {
        bmaIRQ = false;
        bool  rlst;
        // Need to wait for the interrupt status to return true
        do {
            rlst =  sensor->readInterrupt();
        } while (!rlst);

        if (sensor->isStepCounter()) {
            seupCount =  sensor->getCounter();
        }
    }

    if (millis() - loopMillis > 1000) {
        loopMillis = millis();
        // Partial refresh
        mainPage(false);
    }
}



const unsigned char logoIcon[280] =  {
    0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
    0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X01, 0X80,
    0X00, 0X00, 0X00, 0X00, 0XF8, 0X00, 0X00, 0X00, 0X03, 0XC0, 0X00, 0X00, 0X00, 0X03, 0XFF, 0X00,
    0X00, 0X00, 0X07, 0XE0, 0X00, 0X00, 0X00, 0X0F, 0XFF, 0X80, 0X00, 0X00, 0X07, 0XE0, 0X02, 0X30,
    0X00, 0X0F, 0XC3, 0X80, 0X00, 0X00, 0X07, 0XE0, 0X07, 0X78, 0X00, 0X1F, 0X81, 0X80, 0X00, 0X00,
    0X07, 0XE0, 0X0F, 0XF8, 0X00, 0X3F, 0X80, 0X00, 0X00, 0X00, 0X07, 0XE0, 0X07, 0X78, 0X00, 0X3F,
    0X80, 0X00, 0X00, 0X00, 0X07, 0XE0, 0X02, 0X7B, 0XC3, 0XBF, 0X0F, 0X87, 0XF0, 0X00, 0X07, 0XE0,
    0X07, 0X7B, 0XC7, 0XFF, 0X1F, 0XCF, 0XF8, 0X00, 0X07, 0XE0, 0X0F, 0X7B, 0XC7, 0XFF, 0X1F, 0XDE,
    0X3C, 0X00, 0X07, 0XE0, 0X0F, 0XFB, 0XC7, 0XBF, 0X07, 0XDE, 0X3E, 0X00, 0X07, 0XE0, 0X7F, 0X7B,
    0XC7, 0XBF, 0X83, 0XFE, 0X3E, 0X00, 0X07, 0XE0, 0X7F, 0XFB, 0XC7, 0XBF, 0X83, 0XFE, 0X3E, 0X00,
    0X07, 0XE0, 0X7F, 0XFB, 0XC7, 0X9F, 0X83, 0XDE, 0X3E, 0X00, 0X07, 0XE0, 0XEF, 0X7B, 0XC7, 0X9F,
    0XC3, 0XDE, 0X3C, 0X00, 0X07, 0XFF, 0XEF, 0XFB, 0XEF, 0X8F, 0XFF, 0XDF, 0X7C, 0X00, 0X03, 0XFF,
    0XC7, 0X79, 0XFF, 0X83, 0XFF, 0XCF, 0XF8, 0X00, 0X01, 0XFF, 0X86, 0X30, 0XE7, 0X80, 0X79, 0X81,
    0XE0, 0X00, 0X00, 0X00, 0X00, 0X00, 0X07, 0X80, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X03,
    0X07, 0X80, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X03, 0XDF, 0X80, 0X00, 0X00, 0X00, 0X00,
    0X00, 0X00, 0X00, 0X03, 0XFF, 0X80, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X01, 0XFF, 0X00,
    0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
    0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
};
