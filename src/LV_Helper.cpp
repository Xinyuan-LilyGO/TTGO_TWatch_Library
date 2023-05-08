/**
 * @file      LV_Helper.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-04-28
 *
 */

#include "LilyGoLib.h"
#include <lvgl.h>

static lv_disp_drv_t disp_drv;
static lv_indev_drv_t  indev_drv;
static lv_disp_draw_buf_t draw_buf;

void updateLvglHelper()
{
    lv_disp_drv_update(lv_disp_get_default(), &disp_drv);
}


/* Display flushing */
#if LV_VERSION_CHECK(9,0,0)
void disp_flush( lv_disp_t *disp, const lv_area_t *area, lv_color_t *color_p )
#else
void disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
#endif
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    watch.startWrite();
    watch.setAddrWindow( area->x1, area->y1, w, h );

#if LV_VERSION_CHECK(9,0,0)
    // int32_t x;
    // int32_t y;
    // for (y = area->y1; y <= area->y2; y++) {
    //     for (x = area->x1; x <= area->x2; x++) {
    //         watch.drawPixel(x, y, *(uint16_t *)(color_p));
    //         color_p++;
    //     }
    // }
    watch.pushColors(( uint16_t * )color_p, w * h);
#else
    watch.pushColors(( uint16_t * )&color_p->full, w * h, true );
#endif
    watch.endWrite();

    lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
#if LV_VERSION_CHECK(9,0,0)
void touchpad_read( lv_indev_t *indev_driver, lv_indev_data_t *data )
#else
void touchpad_read( lv_indev_drv_t *indev_driver, lv_indev_data_t *data )
#endif
{
    int16_t x, y;
    bool touched = watch.getPoint(&x, &y);
    if ( !touched ) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        switch (watch.getRotation()) {
        case 0:
            data->point.x = TFT_WIDTH - x;
            data->point.y = TFT_HEIGHT - y;
            break;
        case 1:
            data->point.x = TFT_WIDTH - y;
            data->point.y =  x;
            break;
        case 3:
            data->point.x = y;
            data->point.y = TFT_HEIGHT - x;
            break;
        case 2:
        default:
            data->point.x = x;
            data->point.y = y;
        }
        // Serial.printf("X:%d Y:%d\n", data->point.x, data->point.y);
        data->state = LV_INDEV_STATE_PR;
    }
}

#ifndef BOARD_HAS_PSRAM
static const size_t lv_buffer_size = (BOARD_TFT_WIDTH * 50);
static lv_color_t buf[ lv_buffer_size ];
static lv_color_t buf1[ lv_buffer_size ];
#else
static const size_t lv_buffer_size = BOARD_TFT_WIDTH * BOARD_TFT_HEIHT * sizeof(lv_color_t);
static lv_color_t *buf  = NULL;
static lv_color_t *buf1  = NULL;
#endif

#if LV_USE_LOG
#if LV_VERSION_CHECK(9,0,0)
void lv_log_print_g_cb(lv_log_level_t level, const char *buf)
#else
void lv_log_print_g_cb(const char *buf)
#endif
{
    Serial.println(buf);
    Serial.flush();
}
#endif //LV_USE_LOG

void beginLvglHelper(bool debug)
{
    lv_init();

#if LV_USE_LOG
    if (debug) {
        lv_log_register_print_cb(lv_log_print_g_cb);
    }
#endif //LV_USE_LOG

#ifdef BOARD_HAS_PSRAM
    buf = (lv_color_t *)ps_malloc(lv_buffer_size);
    if (!buf) {
        while (1) {
            delay(1000);
            Serial.println("Memory buffer failed!");
        }
    }
    buf1 = (lv_color_t *)ps_malloc(lv_buffer_size);
    if (!buf1) {
        while (1) {
            delay(1000);
            Serial.println("Memory buffer failed!");
        }
    }
#endif //BOARD_HAS_PSRAM

#if LV_VERSION_CHECK(9,0,0)
    lv_disp_t *disp =  lv_disp_create(BOARD_TFT_WIDTH, BOARD_TFT_HEIHT);

#ifdef BOARD_HAS_PSRAM
    lv_disp_set_draw_buffers(disp, buf, buf1, BOARD_TFT_WIDTH * BOARD_TFT_HEIHT, LV_DISP_RENDER_MODE_PARTIAL);
#else
    lv_disp_set_draw_buffers(disp, buf, NULL, sizeof(buf), LV_DISP_RENDER_MODE_PARTIAL);
#endif
    // lv_disp_set_driver_data()/
    lv_disp_set_res(disp, BOARD_TFT_WIDTH, BOARD_TFT_HEIHT);
    lv_disp_set_physical_res(disp, BOARD_TFT_WIDTH, BOARD_TFT_HEIHT);
    lv_disp_set_flush_cb(disp, disp_flush);

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_read_cb(indev, touchpad_read);
    lv_indev_set_disp(indev, disp);
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    // lv_indev_set_driver_data();

#else

#ifdef BOARD_HAS_PSRAM
    lv_disp_draw_buf_init( &draw_buf, buf, buf1, lv_buffer_size);
#else
    lv_disp_draw_buf_init( &draw_buf, buf, buf1, lv_buffer_size );
#endif

    /*Initialize the display*/
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = BOARD_TFT_WIDTH;
    disp_drv.ver_res = BOARD_TFT_HEIHT;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;

    lv_disp_drv_register( &disp_drv );

    /*Initialize the input device driver*/
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register( &indev_drv );

#endif
}