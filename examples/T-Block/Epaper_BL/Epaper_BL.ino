/*
* Based on the test code written by GxEPD2 / U8g2_for_Adafruit_GFX
* https://github.com/ZinggJM/GxEPD2
* https://github.com/olikraus/U8g2_for_Adafruit_GFX
*
*/

#include "config.h"

extern const unsigned char logoIcon[280];

#define DEFAULT_BRIGHTNESS                          45

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
TTGOClass *twatch = nullptr;
PCF8563_Class *rtc = nullptr;
AXP20X_Class *power = nullptr;
Button2 *btn = nullptr;

uint32_t seupCount = 0;
bool pwIRQ = false;


void setupDisplay()
{
    u8g2Fonts.begin(twatch->display); // connect u8g2 procedures to Adafruit GFX
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
    const char *weekChars[] = { "日", "一", "二", "三", "四", "五", "六"};

    if (lastSeupCount != seupCount) {
        lastSeupCount = seupCount;
        u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312a); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
        snprintf(buff, sizeof(buff), "%u 步", seupCount);
        u8g2Fonts.setCursor(getX - getW, getY);
        u8g2Fonts.print(buff);
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

        twatch->display.setFullWindow();

        twatch->display.fillScreen(GxEPD_WHITE);

        twatch->display.drawBitmap(5, 5, logoIcon, 75, 28, GxEPD_BLACK);

        //BATTERY ICON
        u8g2Fonts.setFont(u8g2_font_battery19_tn);
        u8g2Fonts.setCursor(175, 20);                // start writing at this position
        u8g2Fonts.setFontDirection(3);              // left to right (this is default)
        u8g2Fonts.print(4);
        u8g2Fonts.setFontDirection(0);              // left to right (this is default)

        twatch->display.drawFastHLine(10, 40,  twatch->display.width() - 20, GxEPD_BLACK);
        twatch->display.drawFastHLine(10, 150, twatch->display.width() - 20, GxEPD_BLACK);

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
        x = ((twatch->display.width() - tbw) / 2) ;
        y = ((twatch->display.height() - tbh) / 2) + 40  ;

        u8g2Fonts.setCursor(x, y);
        u8g2Fonts.print(buff);
        u8g2Fonts.setCursor(20, y + 50);

        u8g2Fonts.print("你今天已经走了:");
        u8g2Fonts.print("1000步");

        getX = u8g2Fonts.getCursorX();
        getY = u8g2Fonts.getCursorY();
        getH  = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent();
        getW = u8g2Fonts.getUTF8Width("1000步");

        twatch->display.nextPage();



    } else {
        u8g2Fonts.setFont(u8g2_font_inr38_mn); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
        twatch->display.setPartialWindow(lastX, lastY - u8g2Fonts.getFontAscent() - 3, lastW, lastH);
        twatch->display.firstPage();
        do {

            twatch->display.fillRect(lastX, lastY - u8g2Fonts.getFontAscent() - 3, lastW, lastH, GxEPD_WHITE);
            twatch->display.fillScreen(GxEPD_WHITE);
            twatch->display.setTextColor(GxEPD_BLACK);
            lastW = u8g2Fonts.getUTF8Width(buff);
            u8g2Fonts.setCursor(lastX, lastY);
            u8g2Fonts.print(buff);
        } while (twatch->display.nextPage());

    }
}

void setup()
{
    Serial.begin(115200);

    delay(500);

    // Get watch object
    twatch = TTGOClass::getWatch();

    twatch->begin();

    // Turn on the backlight
    twatch->openBL();

    rtc = twatch->rtc;

    power = twatch->power;

    btn = twatch->button;

    // Use compile time as RTC input time
    rtc->check();

    // Turn on power management button interrupt
    power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);

    // Clear power interruption
    power->clearIRQ();

    // Set MPU6050 to sleep
    twatch->mpu->setSleepEnabled(true);

    // Set Pin to interrupt
    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        pwIRQ = true;
    }, FALLING);

    btn->setPressedHandler([]() {

        delay(2000);

        esp_sleep_enable_ext1_wakeup(GPIO_SEL_36, ESP_EXT1_WAKEUP_ALL_LOW);

        esp_deep_sleep_start();
    });

    // Adjust the backlight to reduce current consumption
    twatch->setBrightness(DEFAULT_BRIGHTNESS);

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
