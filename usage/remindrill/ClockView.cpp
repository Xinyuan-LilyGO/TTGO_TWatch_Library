#include "ClockView.h"
#include <lv_conf.h>

LV_FONT_DECLARE(font_firacode_60);


ClockView *ClockView::m_Instance = nullptr;

ClockView::ClockView()
    : m_Screen(nullptr)
    , m_TopBar(nullptr)
    , m_CenterBar(nullptr)
    , m_BottomBar(nullptr)
    , m_SleepButton(nullptr)
    , m_Battery(nullptr)
    , m_DummyCount(1)
    , m_UpdateTime(0)
{
    m_Instance = this;
}

ClockView &ClockView::getInstance()
{
    return *m_Instance;
}

void ClockView::setup()
{

    m_Screen = lv_obj_create(nullptr);
    lv_obj_set_style_bg_color(m_Screen, lv_color_black(), 0);

    m_TopBar = lv_obj_create(m_Screen);
    //lv_obj_set_layout(m_TopBar, LV_LAYOUT_FLEX);
    lv_obj_set_size(m_TopBar, LV_HOR_RES, 60);
    lv_obj_align(m_TopBar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(m_TopBar, lv_color_black(), 0);
    lv_obj_set_style_border_opa(m_TopBar, LV_OPA_TRANSP, 0);
    //lv_obj_set_flex_flow(m_TopBar, LV_FLEX_FLOW_ROW);




    m_SleepButton = lv_btn_create(m_TopBar);
    lv_obj_align( m_SleepButton, LV_ALIGN_LEFT_MID, 0, 0 );
    lv_obj_set_style_text_font( m_SleepButton, &lv_font_montserrat_16, 0 );
    lv_obj_set_size(m_SleepButton, 40, 40);
    lv_obj_add_event_cb(m_SleepButton, event_onSleep, LV_EVENT_CLICKED, NULL);

    lv_obj_t *sleepLabel = lv_label_create(m_SleepButton);
    lv_label_set_text(sleepLabel, LV_SYMBOL_POWER);
    lv_obj_center(sleepLabel);

//    lv_obj_t *gap = lv_label_create(m_TopBar);
//    lv_obj_set_flex_grow( gap, 1);
//    lv_obj_set_flex_align(gap, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    m_Battery = lv_label_create(m_TopBar);
    lv_obj_set_size(m_Battery, 100, 40);
    lv_obj_align( m_Battery, LV_ALIGN_RIGHT_MID, 0, 0 );
    //lv_obj_set_flex_align(m_Battery, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_text_font(m_Battery, &lv_font_montserrat_16, 0);
    lv_obj_set_style_bg_color(m_Battery, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_text_color(m_Battery, lv_palette_main(LV_PALETTE_YELLOW), 0);
    lv_obj_set_style_text_align(m_Battery, LV_TEXT_ALIGN_RIGHT, 0);
    lv_label_set_text(m_Battery, LV_SYMBOL_BATTERY_FULL "%");

    
    lv_style_init(&m_ClockStyle);
    lv_style_set_text_font(&m_ClockStyle, &font_firacode_60);
    lv_style_set_text_color(&m_ClockStyle, lv_color_make(0xFF,0xFF,0));
    lv_style_set_text_opa(&m_ClockStyle, LV_OPA_COVER);
    lv_style_set_bg_opa(&m_ClockStyle, LV_OPA_TRANSP);
    lv_style_set_border_color(&m_ClockStyle, lv_color_make(0x0,0xFF,0));
    lv_style_set_text_align(&m_ClockStyle, LV_TEXT_ALIGN_CENTER );

    m_CenterBar = lv_obj_create(m_Screen);
    lv_obj_set_size(m_CenterBar, LV_HOR_RES, LV_VER_RES-60-60);
    lv_obj_align(m_CenterBar, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(m_CenterBar, LV_OPA_TRANSP, 0);

    m_ClockLabel = lv_label_create(m_CenterBar);
    lv_obj_set_size(m_ClockLabel, 200, 70);
    lv_obj_align( m_ClockLabel, LV_ALIGN_CENTER, 0, 0 );
    lv_obj_add_style(m_ClockLabel, &m_ClockStyle, 0);
    lv_label_set_text(m_ClockLabel, "12:00");

    m_BottomBar = lv_obj_create(m_Screen);
    lv_obj_set_size(m_BottomBar, LV_HOR_RES, 60);
    lv_obj_align(m_BottomBar, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(m_BottomBar, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(m_BottomBar, LV_OPA_TRANSP, 0);

    m_AlarmsTuneButton = lv_btn_create(m_BottomBar);
    lv_obj_align( m_AlarmsTuneButton, LV_ALIGN_LEFT_MID, 0, 0 );
    lv_obj_set_style_text_font( m_AlarmsTuneButton, &lv_font_montserrat_16, 0 );
    lv_obj_set_size(m_AlarmsTuneButton, 40, 40);
    lv_obj_add_event_cb(m_AlarmsTuneButton, event_onAlarmsTune, LV_EVENT_CLICKED, NULL);

    lv_obj_t *alarmsTuneLabel = lv_label_create(m_AlarmsTuneButton);
    lv_label_set_text(alarmsTuneLabel, LV_SYMBOL_BELL);
    lv_obj_center(alarmsTuneLabel);


}

void ClockView::loop()
{
    if( m_UpdateTime < millis() )
    {
        m_UpdateTime = millis() + 1000;

        const auto batteryLife = watch.getBatteryPercent();
        const char * batterySymbol = LV_SYMBOL_BATTERY_FULL;
        if(batteryLife <= 5)
            batterySymbol = LV_SYMBOL_BATTERY_EMPTY;
        else if (batteryLife <= 35)
            batterySymbol = LV_SYMBOL_BATTERY_1;
        else if (batteryLife <= 65)
            batterySymbol = LV_SYMBOL_BATTERY_2;
        else if (batteryLife <= 95)
            batterySymbol = LV_SYMBOL_BATTERY_3;

        // Update battery indicator
        lv_label_set_text_fmt(m_Battery, "%s%3d%%", batterySymbol, batteryLife);

        char time_text[] = "00:00";
        strncpy_P(time_text, watch.strftime(DATETIME_FORMAT_HM), sizeof(time_text));

        // Blink colon in the string of RTC time
        time_text[2] = watch.hwClockRead() % 2 ? ':' : ' ';
        lv_label_set_text(m_ClockLabel, time_text);
    }
}

lv_obj_t *ClockView::getScreen()
{
    return m_Screen;
}

void ClockView::event_onSleep(lv_event_t *e)
{
    LV_LOG_USER("Sleep Clicked");

    // Set to wake by pressing the button on the crown
    watch.setSleepMode(PMU_BTN_WAKEUP);

    watch.sleep();
}

void ClockView::event_onAlarmsTune(lv_event_t *e)
{
    auto &inst = getInstance();
    const auto pal = lv_palette_t(inst.m_DummyCount % _LV_PALETTE_LAST);
    inst.m_DummyCount++;
    lv_obj_set_style_text_color(inst.m_ClockLabel, lv_palette_main(pal), 0);
}