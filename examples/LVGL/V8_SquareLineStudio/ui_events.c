// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.4
// LVGL VERSION: 8.2
// PROJECT: TWatch2020

#include "ui.h"

void flower_anim_cb(void *flower, int32_t y)
{
  if (y < 220)
    lv_obj_set_y((lv_obj_t *)flower, (lv_coord_t)y);
  else
    lv_obj_set_y((lv_obj_t *)flower, 220);
}

void on_flower_released(lv_obj_t *flower)
{
  static lv_obj_t *ll_head = NULL;
  static lv_obj_t *ll_tail = NULL;
  static size_t ll_items = 0;

  int spawn_count = lv_rand(2, 6);

  for (int i = 0; i < spawn_count; i++)
  {
    lv_obj_t *image = lv_img_create(ui_Main);

    lv_obj_set_x(image, lv_rand(0, 208));
    lv_obj_set_y(image, -16);
    lv_obj_set_height(image, LV_SIZE_CONTENT);
    lv_obj_set_width(image, LV_SIZE_CONTENT);
    lv_img_set_src(image, lv_imgbtn_get_src_middle(flower, LV_IMGBTN_STATE_RELEASED));

    if (ll_tail)
      lv_obj_set_user_data(ll_tail, image);

    ll_tail = image;
    ll_items++;

    if (!ll_head)
      ll_head = image;

    if (ll_items > 100)
    {
      lv_obj_t *ll_future_head = lv_obj_get_user_data(ll_head);

      lv_anim_del(ll_head, flower_anim_cb);
      lv_obj_del(ll_head);

      ll_head = ll_future_head;
      ll_items--;
    }

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, flower_anim_cb);
    lv_anim_set_var(&a, image);
    lv_anim_set_time(&a, 1000);
    lv_anim_set_values(&a, lv_rand(-16 - 128, -16), lv_rand(256, 256 + 128));
    lv_anim_start(&a);
  }
}

void flower_rain_timer_cb(lv_timer_t *timer)
{
  lv_obj_t *flower_btns[] = {
    ui_flower0,
    ui_flower45,
    ui_flower90,
    ui_flower135,
    ui_flower180,
    ui_flower225,
    ui_flower270,
    ui_flower315,
  };

  on_flower_released(flower_btns[lv_rand(0, 7)]);

  lv_timer_set_period(timer, lv_rand(500, 1000));
}

void toggle_flower_rain()
{
  static lv_timer_t *auto_spawn_timer = NULL;

  if (auto_spawn_timer)
  {
    lv_timer_del(auto_spawn_timer);

    auto_spawn_timer = NULL;
  }
  else
    auto_spawn_timer = lv_timer_create(flower_rain_timer_cb, 0, NULL);
}

static bool kitty_longpressed = false;

void on_kitty_longpressed(lv_event_t *e)
{
  kitty_longpressed = true;
}

void on_kitty_released(lv_event_t *e)
{
  if (kitty_longpressed)
  {
    kitty_longpressed = false;

    toggle_flower_rain();
  }
}

void on_flower0_released(lv_event_t *e)
{
  on_flower_released(ui_flower0);
}

void on_flower45_released(lv_event_t *e)
{
  on_flower_released(ui_flower45);
}

void on_flower90_released(lv_event_t *e)
{
  on_flower_released(ui_flower90);
}

void on_flower135_released(lv_event_t *e)
{
  on_flower_released(ui_flower135);
}

void on_flower180_released(lv_event_t *e)
{
  on_flower_released(ui_flower180);
}

void on_flower225_released(lv_event_t *e)
{
  on_flower_released(ui_flower225);
}

void on_flower270_released(lv_event_t *e)
{
  on_flower_released(ui_flower270);
}

void on_flower315_released(lv_event_t *e)
{
  on_flower_released(ui_flower315);
}
