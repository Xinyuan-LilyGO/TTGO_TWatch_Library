#ifndef _CLOCK_SCREEN_H_
#define _CLOCK_SCREEN_H_

#include <LilyGoLib.h> // Hardware-specific library
#include <LV_Helper.h>

class ClockView {
public:
    ClockView();

    static ClockView & getInstance();

    void setup();
    void loop();


    lv_obj_t* getScreen(); 
protected:
    static void event_onSleep(lv_event_t *e);


  
private:
    static ClockView * m_Instance;

    lv_obj_t *m_Screen;
    lv_obj_t *m_TopBar;
    lv_obj_t *m_SleepButton;
    lv_obj_t *m_Battery;
    lv_obj_t *m_ClockLabel;
    lv_obj_t *m_CenterBar;
    lv_obj_t *m_BottomBar;

    lv_style_t m_ClockStyle;

    uint32_t m_UpdateTime;

    int m_DummyCount;
};

#endif // _CLOCK_SCREEN_H_
