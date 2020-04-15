/*

  _       _   _            ____         
 | |     (_) | |  _   _   / ___|   ___  
 | |     | | | | | | | | | |  _   / _ \ 
 | |___  | | | | | |_| | | |_| | | (_) |
 |_____| |_| |_|  \__, |  \____|  \___/ 
                  |___/                 

website:https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library
Written by Lewis he //https://github.com/lewisxhe
*/

#include "TTGO.h"

TTGOClass *TTGOClass::_ttgo = nullptr;


void TTGOClass::disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1) ;
    _ttgo->eTFT->setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
    _ttgo->eTFT->pushColors(( uint16_t *)color_p, size, false);
    lv_disp_flush_ready(disp_drv);
}


bool TTGOClass::touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static TP_Point p;
    data->state = _ttgo->touch->touched() ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    if (data->state == LV_INDEV_STATE_PR) {
        p = _ttgo->touch->getPoint();
    }
    /*Set the coordinates (if released use the last pressed coordinates)*/
    data->point.x = p.x;
    data->point.y = p.y;
    return false; /*Return false because no moare to be read*/
}
