/**
 * @file      lv_event.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2022  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2022-11-09
 *
 */

#pragma once


typedef void (*return_event_cb)(void *args);
typedef void (*refresh_event_cb)(void *args);

void setupGUI();
void startWindows(const char *title, uint32_t anim_time);
lv_obj_t *lv_menu_list_init(lv_obj_t *par,
                            const char *title,
                            return_event_cb returnEvent,
                            refresh_event_cb refreshEvent);


lv_obj_t *lv_menu_list_add(lv_obj_t *par, const void *img, const char *ssid, int rssi);

