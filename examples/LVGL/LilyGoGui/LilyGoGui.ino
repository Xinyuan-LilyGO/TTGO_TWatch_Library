
/**
 * Simple clock display interface
 * Written by lewishe
 * */

#include "config.h"

LV_IMG_DECLARE(arrow_left_png);
LV_IMG_DECLARE(arrow_right_png);
LV_FONT_DECLARE(fn1_32);
LV_FONT_DECLARE(robot_ightItalic_16);
LV_FONT_DECLARE(robot_light_16);
LV_FONT_DECLARE(liquidCrystal_nor_64);
LV_FONT_DECLARE(liquidCrystal_nor_32);
LV_FONT_DECLARE(liquidCrystal_nor_24);
LV_FONT_DECLARE(hansans_cn_24);
LV_FONT_DECLARE(digital_play_st_48);
LV_FONT_DECLARE(quostige_16);
LV_FONT_DECLARE(digital_play_st_24);
LV_FONT_DECLARE(gracetians_32);
LV_FONT_DECLARE(exninja_22);

TTGOClass *watch = nullptr;


lv_obj_t *setupGUI()
{
    static lv_style_t cont_style;
    lv_style_init(&cont_style);
    lv_style_init(&cont_style);
    lv_style_set_radius(&cont_style, LV_OBJ_PART_MAIN, 12);
    lv_style_set_bg_color(&cont_style, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_bg_opa(&cont_style, LV_OBJ_PART_MAIN, LV_OPA_COVER);
    lv_style_set_border_width(&cont_style, LV_OBJ_PART_MAIN, 0);

    lv_obj_t *view = lv_cont_create(lv_scr_act(), nullptr);
    lv_obj_set_size(view, 240, 240);
    lv_obj_add_style(view, LV_OBJ_PART_MAIN, &cont_style);

    static lv_style_t onestyle;
    lv_style_init(&onestyle);
    lv_style_set_text_color(&onestyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(&onestyle, LV_STATE_DEFAULT, &fn1_32);

    //Upper left corner logo
    lv_obj_t *casio = lv_label_create(view, nullptr);
    lv_obj_add_style(casio, LV_OBJ_PART_MAIN, &onestyle);
    lv_label_set_text(casio, "LilyGo");
    lv_obj_align(casio, view, LV_ALIGN_IN_TOP_LEFT, 10, 10);

    //Upper right corner model
    static lv_style_t model_style;
    lv_style_init(&model_style);
    lv_style_set_text_color(&model_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(&model_style, LV_STATE_DEFAULT, &robot_ightItalic_16);

    lv_obj_t *model = lv_label_create(view, nullptr);
    lv_obj_add_style(model, LV_OBJ_PART_MAIN, &model_style);
    lv_label_set_text(model, "ESP32-S2");
    lv_obj_align(model, view, LV_ALIGN_IN_TOP_RIGHT, -10, 15);


    //Line style
    static lv_style_t line_style;
    lv_style_init(&line_style);
    lv_style_set_line_color(&line_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_line_width(&line_style, LV_STATE_DEFAULT, 2);
    lv_style_set_line_rounded(&line_style, LV_STATE_DEFAULT, 1);

    //Top horizontal line
    static lv_point_t line_points[] = { {10, 0}, {230, 0} };
    lv_obj_t *line1;
    line1 = lv_line_create(view, NULL);
    lv_line_set_points(line1, line_points, 2);     /*Set the points*/
    lv_obj_add_style(line1, LV_OBJ_PART_MAIN, &line_style);
    lv_obj_align(line1, NULL, LV_ALIGN_IN_TOP_MID, 0, 45);

    //Slogan below the top horizontal line
    lv_obj_t *row_down_line = lv_label_create(view, nullptr);
    lv_obj_add_style(row_down_line, LV_OBJ_PART_MAIN, &model_style);
    lv_label_set_text(row_down_line, "Designed by lewis");
    lv_obj_align(row_down_line, line1, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 2);


    //Bottom horizontal line left
    static lv_point_t line_points1[] = { {0, 0}, {75, 0} };
    lv_obj_t *line2;
    line2 = lv_line_create(view, NULL);
    lv_line_set_points(line2, line_points1, 2);     /*Set the points*/
    lv_obj_add_style(line2, LV_OBJ_PART_MAIN, &line_style);
    lv_obj_align(line2, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 10, -30);

    //! Bottom line right
    static lv_point_t line_points2[] = { {0, 0}, {75, 0} };
    lv_obj_t *line3;
    line3 = lv_line_create(view, NULL);
    lv_line_set_points(line3, line_points2, 2);     /*Set the points*/
    lv_obj_add_style(line3, LV_OBJ_PART_MAIN, &line_style);
    lv_obj_align(line3, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -30);

    // Below the horizontal line in the upper left corner
    static lv_style_t text_style;
    lv_style_init(&text_style);
    lv_style_set_text_color(&text_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(&text_style, LV_STATE_DEFAULT, &robot_ightItalic_16);

    //! light
    lv_obj_t *light_text = lv_label_create(view, nullptr);
    lv_obj_add_style(light_text, LV_OBJ_PART_MAIN, &text_style);
    lv_label_set_text(light_text, "LIGHT");
    lv_obj_align(light_text, line1, LV_ALIGN_OUT_BOTTOM_RIGHT, -30, 5);

    //! arrow -> right
    lv_obj_t *img1 = lv_img_create(view, NULL);
    lv_img_set_src(img1, &arrow_right_png);
    lv_obj_align(img1, line1, LV_ALIGN_OUT_BOTTOM_RIGHT, -10, 5);

    //! arrow down -> left
    lv_obj_t *img2 = lv_img_create(view, NULL);
    lv_img_set_src(img2, &arrow_left_png);
    lv_obj_align(img2, line2, LV_ALIGN_OUT_TOP_LEFT, 0, -5);

    //! arrow down -> right
    lv_obj_t *img3 = lv_img_create(view, NULL);
    lv_img_set_src(img3, &arrow_right_png);
    lv_obj_align(img3, line3, LV_ALIGN_OUT_TOP_RIGHT, 0, -5);

    //Intermediate clock time division font
    static lv_style_t time_style;
    lv_style_init(&time_style);
    lv_style_set_text_color(&time_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(&time_style, LV_STATE_DEFAULT, &digital_play_st_48);


    lv_obj_t *hour = lv_label_create(view, nullptr);
    lv_obj_add_style(hour, LV_OBJ_PART_MAIN, &time_style);
    lv_label_set_text(hour, "10");
    lv_obj_align(hour, view, LV_ALIGN_CENTER, -50, 10);

    //semicolon
    static lv_style_t dot_style;
    lv_style_init(&dot_style);
    lv_style_set_text_color(&dot_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(&dot_style, LV_STATE_DEFAULT, &liquidCrystal_nor_64);


    lv_obj_t *dot = lv_label_create(view, nullptr);
    lv_obj_add_style(dot, LV_OBJ_PART_MAIN, &dot_style);
    lv_label_set_text(dot, ":");
    lv_obj_align(dot, hour, LV_ALIGN_OUT_RIGHT_MID, 5, -10);

    //minute
    lv_obj_t *minute = lv_label_create(view, nullptr);
    lv_obj_add_style(minute, LV_OBJ_PART_MAIN, &time_style);
    lv_label_set_text(minute, "14");
    lv_obj_align(minute, hour, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

    //Intermediate clock second digit
    static lv_style_t second_style;
    lv_style_init(&second_style);
    lv_style_set_text_color(&second_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(&second_style, LV_STATE_DEFAULT, &liquidCrystal_nor_32);

    lv_obj_t *second = lv_label_create(view, nullptr);
    lv_obj_add_style(second, LV_OBJ_PART_MAIN, &second_style);
    lv_label_set_text(second, "34");
    lv_obj_align(second, minute, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, -10);

    //date
    static lv_style_t year_style;
    lv_style_init(&year_style);
    lv_style_set_text_color(&year_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(&year_style, LV_STATE_DEFAULT, &liquidCrystal_nor_24);


    lv_obj_t *year = lv_label_create(view, nullptr);
    lv_obj_add_style(year, LV_OBJ_PART_MAIN, &year_style);
    lv_label_set_text(year, "2020");
    lv_obj_align(year, view, LV_ALIGN_CENTER, 0, 55);

    //Chinese font
    static lv_style_t chinese_style;
    lv_style_init(&chinese_style);
    lv_style_set_text_color(&chinese_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(&chinese_style, LV_STATE_DEFAULT, &hansans_cn_24);


    lv_obj_t *month = lv_label_create(view, nullptr);
    lv_obj_add_style(month, LV_OBJ_PART_MAIN, &chinese_style);
    lv_label_set_text(month, "三月");
    lv_obj_align(month, year, LV_ALIGN_OUT_LEFT_MID, -5, 0);

    lv_obj_t *state = lv_label_create(view, nullptr);
    lv_obj_add_style(state, LV_OBJ_PART_MAIN, &chinese_style);
    lv_label_set_text(state, "上午");
    lv_obj_align(state, year, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    lv_obj_t *week = lv_label_create(view, nullptr);
    lv_obj_add_style(week, LV_OBJ_PART_MAIN, &chinese_style);
    lv_label_set_text(week, "周二");
    lv_obj_align(week, view, LV_ALIGN_CENTER, 45, -35);

    lv_obj_t *day = lv_label_create(view, nullptr);
    lv_obj_add_style(day, LV_OBJ_PART_MAIN, &year_style);
    lv_label_set_text(day, "3");
    lv_obj_align(day, week, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    //! Battery string
    lv_obj_t *bat_text = lv_label_create(view, nullptr);
    lv_obj_add_style(bat_text, LV_OBJ_PART_MAIN, &text_style);
    lv_label_set_text(bat_text, "BAT");
    lv_obj_align(bat_text, view, LV_ALIGN_IN_LEFT_MID, 20, -40);

    // temperature
    static lv_style_t temp_style;
    lv_style_init(&temp_style);
    lv_style_set_text_color(&temp_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(&temp_style, LV_STATE_DEFAULT, &quostige_16);


    lv_obj_t *temp_text = lv_label_create(view, nullptr);
    lv_obj_add_style(temp_text, LV_OBJ_PART_MAIN, &temp_style);
    lv_label_set_text(temp_text, "24*C");
    lv_obj_align(temp_text, bat_text, LV_ALIGN_OUT_BOTTOM_MID, 0, -5);

    //Power
    static lv_style_t bat_style;
    lv_style_init(&bat_style);
    lv_style_set_text_color(&bat_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(&bat_style, LV_STATE_DEFAULT, &digital_play_st_24);


    lv_obj_t *bat = lv_label_create(view, nullptr);
    lv_obj_add_style(bat, LV_OBJ_PART_MAIN, &bat_style);
    lv_label_set_text(bat, "100%");
    lv_obj_align(bat, view, LV_ALIGN_CENTER, -35, -35);


    static lv_point_t line_points3[] = {
        {0, 0}, {50, 0},
        {50, 30}, {50, 30},
        {50, 35}, {35, 39},
        {35, 39}, {15, 39},
        {15, 39}, {0, 35},
        {0, 35}, {0, 0},
    };

    lv_obj_t *line4;
    line4 = lv_line_create(view, NULL);
    lv_line_set_points(line4, line_points3, sizeof(line_points3) / sizeof(line_points3[0]));
    lv_obj_add_style(line4, LV_OBJ_PART_MAIN, &line_style);
    lv_obj_align(line4, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -2);

    // Bottom Go
    static lv_style_t key_style;
    lv_style_init(&key_style);
    lv_style_set_text_color(&key_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(&key_style, LV_STATE_DEFAULT, &gracetians_32);


    lv_obj_t *key = lv_label_create(view, nullptr);
    lv_obj_add_style(key, LV_OBJ_PART_MAIN, &key_style);
    lv_label_set_text(key, "Go");
    lv_obj_align(key, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);


    static lv_style_t bot_style;
    lv_style_init(&bot_style);
    lv_style_set_text_color(&bot_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(&bot_style, LV_STATE_DEFAULT, &exninja_22);


    lv_obj_t *str1 = lv_label_create(view, nullptr);
    lv_obj_add_style(str1, LV_OBJ_PART_MAIN, &bot_style);
    lv_label_set_text(str1, "WATER");
    lv_obj_align(str1, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 10, -2);

    lv_obj_t *str2 = lv_label_create(view, nullptr);
    lv_obj_add_style(str2, LV_OBJ_PART_MAIN, &bot_style);
    lv_label_set_text(str2, "RESIST");
    lv_obj_align(str2, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -2);

    return view;
}

void setup()
{
    Serial.begin(115200);
    watch = TTGOClass::getWatch();
    watch->begin();
    watch->openBL();
    watch->lvgl_begin();
    setupGUI();
}

void loop()
{
    lv_task_handler();
    delay(5);
}