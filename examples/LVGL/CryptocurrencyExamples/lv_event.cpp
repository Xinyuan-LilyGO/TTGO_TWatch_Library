/**
 * @file      lv_event.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2022  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2022-11-09
 *
 */
#include "lvgl/lvgl.h"
#include <Arduino.h>
#include <WiFi.h>
#include "lv_event.h"

LV_IMG_DECLARE(img_refresh);
LV_IMG_DECLARE(img_return);
LV_IMG_DECLARE(img_iota);




static void lv_wifi_auth_init(const char *name);
static void lv_wifi_kb_event(lv_obj_t *obj, lv_event_t e);
lv_obj_t *lv_loader_run(void *user_data,  const char *title, lv_anim_ready_cb_t anim_ready);


return_event_cb __returnEvent = NULL;
refresh_event_cb __refreshEvent = NULL;
static lv_obj_t *loader = NULL;
lv_obj_t *bagImg;

void setupGUI()
{
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    bagImg = lv_img_create(scr, NULL);
    lv_img_set_src(bagImg, &img_iota);
}

void startWindows(const char *title, uint32_t anim_time)
{
    lv_obj_t *child = lv_obj_get_child_back(lv_scr_act(), NULL);
    while (child) {
        if (child != lv_scr_act() && child != bagImg) {
            lv_obj_del(child);
        }
        child = lv_obj_get_child_back(lv_scr_act(), child);
    }

    lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label,  title);
    lv_obj_set_style_local_text_color(label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    // lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 48);
    lv_obj_align(label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -12);

    lv_obj_t *bar = lv_bar_create(lv_scr_act(), NULL);
    lv_obj_set_size(bar, lv_disp_get_hor_res(NULL) / 3, 10);
    lv_bar_set_anim_time(bar, anim_time);
    lv_bar_set_value(bar, 100, LV_ANIM_ON);
    // lv_obj_align(bar, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -18);
    lv_obj_align(bar, NULL, LV_ALIGN_CENTER, 0, 50);
    lv_obj_set_style_local_bg_color(bar, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
}

void menu_windows_return_handler(lv_obj_t *obj, lv_event_t e)
{
    if (e == LV_EVENT_CLICKED) {
        printf("img_refresh....\n");
        if (__returnEvent) {
            __returnEvent(NULL);
        }
    }
}

void menu_windows_refresh_handler(lv_obj_t *obj, lv_event_t e)
{
    if (e == LV_EVENT_CLICKED) {
        printf("img_refresh....\n");
        if (__refreshEvent) {
            __refreshEvent(NULL);
        }
    }
}


lv_obj_t *lv_menu_windows_init(lv_obj_t *part, const char *title_name, return_event_cb returnEvent, refresh_event_cb refreshEvent)
{
    lv_obj_t *scr = lv_cont_create(part, NULL);

    lv_obj_set_style_local_bg_opa(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_100);
    lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lv_obj_set_style_local_border_width(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_size(scr, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_align(scr, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *btn = lv_imgbtn_create(scr, NULL);
    lv_imgbtn_set_src(btn, LV_BTN_STATE_RELEASED, &img_refresh);
    lv_imgbtn_set_src(btn, LV_BTN_STATE_PRESSED,  &img_refresh);
    lv_obj_align(btn, NULL, LV_ALIGN_IN_TOP_LEFT, 15, 15);

    lv_obj_set_event_cb(btn, menu_windows_refresh_handler);

    __refreshEvent = refreshEvent;
    __returnEvent = returnEvent;


    lv_obj_t *label = lv_label_create(scr, NULL);
    lv_label_set_text(label, title_name);
    lv_obj_set_style_local_text_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_obj_align(label, btn, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

    return scr;
}

void wlan_input_event(lv_obj_t *obj, lv_event_t e)
{
    if (e == LV_EVENT_CLICKED) {
        const char *txt = lv_list_get_btn_text(obj);
        lv_wifi_auth_init(txt);
        lv_obj_t *ptr = lv_obj_get_parent(lv_obj_get_parent(lv_obj_get_parent(obj)));
        if (ptr) {
            printf("wlan_input_event : %p\n", ptr);
            lv_obj_del(ptr);
        }
    }
}

lv_obj_t *lv_menu_list_init(lv_obj_t *par,
                            const char *title,
                            return_event_cb returnEvent,
                            refresh_event_cb refreshEvent)
{
    lv_obj_t *scr = lv_menu_windows_init(par, title, returnEvent, refreshEvent);
    printf("lv_wifi_list_init : %p\n", scr);

    lv_obj_t *list = lv_list_create(scr, NULL);
    lv_obj_set_size(list, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL) - 50);
    lv_obj_set_drag_parent(list, true);
    lv_obj_align(list, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    lv_list_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_local_border_width(list, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(list, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_opa(list, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);

    return list;
}

lv_obj_t *lv_menu_list_add(lv_obj_t *par, const void *img, const char *ssid, int rssi)
{
    lv_obj_t *btns = lv_list_add_btn(par, img, ssid);
    lv_obj_set_style_local_bg_opa(btns, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
    lv_obj_set_style_local_text_color(btns, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(btns, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_obj_set_style_local_image_recolor(btns, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_drag(btns, true);
    lv_obj_set_event_cb(btns, wlan_input_event);
    return btns;
}


static void lv_wifi_auth_init(const char *name)
{
    lv_obj_t *scr = lv_menu_windows_init(lv_scr_act(), "WiFi", NULL, NULL);
    printf("lv_wifi_auth_init : %p\n", scr);
    lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_set_style_local_bg_opa(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_100);

    lv_obj_t *ta = lv_textarea_create(scr, NULL);
    lv_obj_set_size(ta, 230, 40);
    lv_textarea_set_one_line(ta, true);
    lv_textarea_set_pwd_mode(ta, false);
    lv_textarea_set_text(ta, "");
    lv_obj_set_style_local_bg_color(ta, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_set_style_local_text_color(ta, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(ta, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_obj_set_style_local_border_color(ta, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_border_color(ta, LV_OBJ_PART_MAIN, LV_STATE_FOCUSED, LV_COLOR_WHITE);
    lv_obj_set_style_local_border_side(ta, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_BORDER_SIDE_BOTTOM);
    lv_obj_align(ta, scr, LV_ALIGN_IN_TOP_MID, 0, 70);

    lv_obj_t *label = lv_label_create(scr, NULL);
    lv_label_set_text_fmt(label, "Connect \"%s\"", name);
    lv_obj_set_style_local_text_color(label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 50);

    /*Create a keyboard and apply the styles*/
    lv_obj_t *kb = lv_keyboard_create(scr, NULL);
    lv_keyboard_set_cursor_manage(kb, true);

    /*Assign the text area to the keyboard*/
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_set_user_data(kb, (void *)name);

    lv_obj_set_event_cb(kb, lv_wifi_kb_event);
}


static void lv_wifi_kb_event(lv_obj_t *obj, lv_event_t e)
{
    if (e == LV_EVENT_CLICKED) {
        const char *txt = lv_btnmatrix_get_active_btn_text(obj);
        if (txt == NULL) {
            return;
        }
        if (strcmp(txt, LV_SYMBOL_OK) == 0) {
            lv_obj_t   *ta =  lv_keyboard_get_textarea(obj);
            const char *c = lv_textarea_get_text(ta);
            if (strlen(c) < 8) {
                static const char *btns[] = {"Close", ""};
                lv_obj_t *mbox = lv_msgbox_create(lv_scr_act(), NULL);
                lv_msgbox_set_text(mbox, "Password too short!");
                lv_msgbox_add_btns(mbox, btns);
                lv_obj_set_width(mbox, 200);
                lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, 0);
                return;
            } else if (c) {
#ifdef ESP32
                const char *ssid = (const char *)lv_obj_get_user_data(obj);
                printf("SSID :%s PASSWORD:%s\n", ssid, c);
                WiFi.begin(ssid, c);
                extern EventGroupHandle_t  wifiEvent;
                xEventGroupSetBits(wifiEvent, (1 << 3));
#endif
                lv_obj_del(lv_obj_get_parent(obj));
            }
        }
    } else {
        lv_keyboard_def_event_cb(obj, e);
    }
}



//////////////////////////////////////
typedef struct {
    lv_obj_t *par;
    lv_obj_t *keyboard;

} lv_set_ext_t;

static lv_set_ext_t __set_ext;

static void kb_event_cb(lv_obj_t *kb, lv_event_t e)
{
    lv_keyboard_def_event_cb(kb, e);
    if (e == LV_EVENT_CANCEL || e == LV_EVENT_APPLY) {
        if (__set_ext.keyboard) {
            lv_obj_del(__set_ext.keyboard);
            __set_ext.keyboard = NULL;
        }
    }
}

static void ta_event_cb(lv_obj_t *ta, lv_event_t e)
{
    if (e == LV_EVENT_RELEASED) {
        if (__set_ext.keyboard == NULL) {
            __set_ext.keyboard = lv_keyboard_create(lv_scr_act(), NULL);
            lv_obj_set_event_cb(__set_ext.keyboard, kb_event_cb);
            lv_indev_wait_release(lv_indev_get_act());
        }
        lv_textarea_set_cursor_hidden(ta, false);
        lv_keyboard_set_textarea(__set_ext.keyboard, ta);
    } else if (e == LV_EVENT_DEFOCUSED) {
        lv_textarea_set_cursor_hidden(ta, true);
    }
}

static void lv_loader_anim_cb(void *arc, lv_anim_value_t v)
{
    if (v > 100) v = 100;
    lv_arc_set_end_angle((lv_obj_t *)arc, v * 360 / 100 + 270);
}


lv_obj_t *lv_loader_run(void *user_data,  const char *title, lv_anim_ready_cb_t anim_ready)
{
    lv_obj_t *scr = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_set_style_local_border_width(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_size(scr, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_align(scr, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *arc = lv_arc_create(scr, NULL);
    lv_obj_set_style_local_bg_opa(arc, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
    lv_obj_set_style_local_border_width(arc, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    lv_arc_set_bg_angles(arc, 0, 0);
    lv_arc_set_start_angle(arc, 270);
    lv_obj_align(arc, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label = lv_label_create(scr, NULL);
    lv_label_set_text(label, title);
    lv_obj_set_style_local_text_color(label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_obj_align(label, arc, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_obj_set_user_data(arc, user_data);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, lv_loader_anim_cb);
    lv_anim_set_ready_cb(&a, anim_ready);
    lv_anim_set_values(&a, 0, 110);
    lv_anim_set_time(&a, 5000);
    lv_anim_set_var(&a, arc);
    lv_anim_start(&a);

    loader = scr;
    return scr;
}


void lv_loader_remove()
{
    if (loader) {
        lv_obj_del(loader);
        loader = NULL;
    }
}

