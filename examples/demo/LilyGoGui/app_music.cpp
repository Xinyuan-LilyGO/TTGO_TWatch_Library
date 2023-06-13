#include "app_music.h"
#include "SD_MMC.h"
#include "global_flags.h"

static struct {
  lv_obj_t *music_list;

  lv_obj_t *play;
  lv_obj_t *pause;
  lv_obj_t *stop;
  lv_obj_t *progress_bar;
  lv_obj_t *time_label;
} music_param;

extern QueueHandle_t play_music_queue;
extern QueueHandle_t play_time_queue;
extern EventGroupHandle_t global_event_group;
extern void suspend_playMP3Handler(void);
extern void resume_playMP3Handler(void);

static lv_obj_t *create_music_btn(lv_obj_t *parent, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, char *symbol);
static void find_file(fs::FS &fs, const char *dirname, uint8_t levels, String &output, char *file_type);

static void play_event_cb(lv_event_t *e);
//static void drag_music_time_event_cb(lv_event_t *e);
static void pause_event_cb(lv_event_t *e);
static void stop_event_cb(lv_event_t *e);

void app_music_load(lv_obj_t *cont) {

  /*Create a list*/
  lv_obj_t *music_list = music_param.music_list = lv_roller_create(cont);
  lv_obj_set_size(music_list, LV_PCT(95), 120);
  lv_obj_align(music_list, LV_ALIGN_TOP_RIGHT, -5, 45);
  lv_obj_set_style_outline_color(music_list, lv_color_white(), LV_STATE_FOCUS_KEY);

  lv_list_add_text(music_list, "Music name");
  String name_list;
  
  name_list += "ring_1.mp3";
  name_list += "\n";
  name_list += "ring_setup.mp3";
  name_list += "\n";

  lv_roller_set_options(music_list, name_list.c_str(), LV_ROLLER_MODE_NORMAL);

  music_param.play = create_music_btn(cont, LV_ALIGN_LEFT_MID, 65, 70, LV_SYMBOL_PLAY);
  music_param.pause = create_music_btn(cont, LV_ALIGN_LEFT_MID, 105, 70, LV_SYMBOL_PAUSE);
  music_param.stop = create_music_btn(cont, LV_ALIGN_LEFT_MID, 145, 70, LV_SYMBOL_STOP);
  lv_obj_set_style_bg_color(music_param.stop, lv_palette_main(LV_PALETTE_PINK), 0);

  lv_obj_add_event_cb(music_param.play, play_event_cb, LV_EVENT_CLICKED, music_list);
  lv_obj_add_event_cb(music_param.pause, pause_event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(music_param.stop, stop_event_cb, LV_EVENT_CLICKED, NULL);
}

static lv_obj_t *create_music_btn(lv_obj_t *parent, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, char *symbol) {
  lv_obj_t *btn = lv_btn_create(parent);
  lv_obj_set_size(btn, 30, 30);
  lv_obj_set_style_radius(btn, 90, 0);
  lv_obj_align(btn, align, x_ofs, y_ofs);
  lv_obj_set_style_outline_color(btn, lv_color_white(), LV_STATE_FOCUS_KEY);

  lv_obj_t *label = lv_label_create(btn);
  lv_obj_center(label);
  lv_label_set_text(label, symbol);

  return btn;
}

// Traverse SD card MP3 files
static void find_file(fs::FS &fs, const char *dirname, uint8_t levels, String &output, char *file_type) {

  File root = fs.open(dirname);
  if (!root) {
    return;
  }
  if (!root.isDirectory()) {
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      if (levels) {
        find_file(fs, file.path(), levels - 1, output, file_type);
      }
    } else {
      if (strstr(file.name(), file_type) != nullptr) {
        output += file.path();
        output += "\n";
      }
    }
    file = root.openNextFile();
  }
}

static void play_event_cb(lv_event_t *e) {
  lv_obj_t *list = (lv_obj_t *)lv_event_get_user_data(e);
  char play_music_path[50];
  lv_roller_get_selected_str(list, play_music_path, 0);
  String path = play_music_path;
  Serial.println(path);
  xQueueSend(play_music_queue, &path, 0);
}
/*
static void drag_music_time_event_cb(lv_event_t *e) {
  lv_obj_t *instance = lv_event_get_target(e);
  lv_event_code_t c = lv_event_get_code(e);
  if (c == LV_EVENT_RELEASED) {
    uint32_t pos = lv_slider_get_value(instance);
    xQueueSend(play_time_queue, &pos, 0);
  }
}*/

static void pause_event_cb(lv_event_t *e) { xEventGroupSetBits(global_event_group, RING_PAUSE); }

static void stop_event_cb(lv_event_t *e) { xEventGroupSetBits(global_event_group, RING_STOP); }


app_t app_music = {
    .setup_func_cb = app_music_load,
    .exit_func_cb = nullptr,
    .user_data= nullptr,
};