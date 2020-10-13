/*
* The example shows that calling TTGOClass objects in multitasking must
* implement a mutex lock,otherwise it will cause panic
* Copyright 2020 Lewis he
*/

#include "config.h"

TTGOClass *ttgo = nullptr;

char buf[128];
int16_t x, y;

static SemaphoreHandle_t xSemaphores = NULL;

void touch_task(void *args)
{
    for (;;) {
        if ( xSemaphoreTake( xSemaphores, ( TickType_t ) 10 ) == pdTRUE ) {
            if (ttgo->getTouch(x, y)) {
                sprintf(buf, "[task] x:%03d  y:%03d", x, y);
                ttgo->tft->drawString(buf, 60, 40);
                Serial.println(buf);
            }
            xSemaphoreGive( xSemaphores );
        }
        delay(20);
    }
}

void touch_task1(void *args)
{
    for (;;) {
        if ( xSemaphoreTake( xSemaphores, ( TickType_t ) 10 ) == pdTRUE ) {
            if (ttgo->getTouch(x, y)) {
                sprintf(buf, "[task1] x:%03d  y:%03d", x, y);
                ttgo->tft->drawString(buf, 60, 60);
                Serial.println(buf);
            }
            xSemaphoreGive( xSemaphores );
        }
        delay(20);
    }
}

void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextFont(2);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->drawString("Touch task test", 62, 20);

    xSemaphores = xSemaphoreCreateMutex();
    xTaskCreate(touch_task, "touch", 2048, NULL, 5, NULL);
    xTaskCreate(touch_task1, "touch1", 2048, NULL, 5, NULL);
}

void loop()
{
    if ( xSemaphoreTake( xSemaphores, ( TickType_t ) 10 ) == pdTRUE ) {
        if (ttgo->getTouch(x, y)) {
            sprintf(buf, "[loop] x:%03d  y:%03d", x, y);
            ttgo->tft->drawString(buf, 60, 80);
            Serial.println(buf);
        }
        xSemaphoreGive( xSemaphores );
    }
    delay(20);
}
