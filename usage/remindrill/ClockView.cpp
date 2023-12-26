#include "ClockView.h"

#define TFT_GREY 0x5AEB

ClockView *ClockView::m_Instance = nullptr;

ClockView::ClockView()
    : m_Screen(nullptr)
    , m_ClickButton(nullptr)
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
    //lv_obj_set_flex_flow(m_TopBar, LV_FLEX_FLOW_ROW);




    m_ClickButton = lv_btn_create(m_TopBar);
    lv_obj_align( m_ClickButton, LV_ALIGN_TOP_LEFT, 0, 0 );
    //lv_obj_set_flex_align(m_ClickButton, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    lv_obj_set_size(m_ClickButton, 100, 50);
    lv_obj_add_event_cb(m_ClickButton, event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label = lv_label_create(m_ClickButton);
    lv_label_set_text(label, "Click me!");
    lv_obj_center(label);

//    lv_obj_t *gap = lv_label_create(m_TopBar);
//    lv_obj_set_flex_grow( gap, 1);
//    lv_obj_set_flex_align(gap, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    m_Battery = lv_label_create(m_TopBar);
    lv_obj_set_size(m_Battery, 100, 50);
    lv_obj_align( m_Battery, LV_ALIGN_TOP_RIGHT, 0, 0 );
    //lv_obj_set_flex_align(m_Battery, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
#if LV_FONT_MONTSERRAT_22
    lv_obj_set_style_text_font(m_Battery, &lv_font_montserrat_22, 0);
#endif
    lv_obj_set_style_bg_color(m_Battery, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_text_color(m_Battery, lv_palette_main(LV_PALETTE_YELLOW), 0);
    lv_obj_set_style_text_align(m_Battery, LV_TEXT_ALIGN_RIGHT, 0);
    lv_label_set_text(m_Battery, LV_SYMBOL_BATTERY_FULL "%");


}

void ClockView::loop()
{
    if( m_UpdateTime < millis() )
    {
        m_UpdateTime = millis() + 1000;

        // Update battery indicator
        lv_label_set_text_fmt(m_Battery, "%s%3d%%", LV_SYMBOL_BATTERY_FULL, watch.getBatteryPercent());
    }
}

lv_obj_t *ClockView::getScreen()
{
    return m_Screen;
}

void ClockView::event_cb(lv_event_t *e)
{
    LV_LOG_USER("Clicked");

    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_child(btn, 0);
    lv_label_set_text_fmt(label, "%" LV_PRIu32, getInstance().m_DummyCount++);
}
