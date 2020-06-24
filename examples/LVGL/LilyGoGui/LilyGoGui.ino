
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
    lv_style_copy(&cont_style, &lv_style_pretty_color);
    cont_style.body.radius = 12;
    cont_style.body.main_color = LV_COLOR_WHITE;
    cont_style.body.grad_color = LV_COLOR_WHITE;
    cont_style.body.opa = 255;
    cont_style.body.border.width = 0;

    lv_obj_t *view = lv_cont_create(lv_scr_act(), nullptr);
    lv_obj_set_size(view, 240, 240);
    lv_obj_set_style(view, &cont_style);

    static lv_style_t onestyle;
    lv_style_copy(&onestyle, &lv_style_pretty_color);
    onestyle.text.font  = &fn1_32;
    onestyle.text.color = LV_COLOR_BLACK;

    //Upper left corner logo
    lv_obj_t *casio = lv_label_create(view, nullptr);
    lv_label_set_style(casio, LV_LABEL_STYLE_MAIN, &onestyle);
    lv_label_set_text(casio, "LilyGo");
    lv_obj_align(casio, view, LV_ALIGN_IN_TOP_LEFT, 10, 10);

    //Upper right corner model
    static lv_style_t model_style;
    lv_style_copy(&model_style, &lv_style_pretty_color);
    model_style.text.font  = &robot_ightItalic_16;
    model_style.text.color = LV_COLOR_BLACK;

    lv_obj_t *model = lv_label_create(view, nullptr);
    lv_label_set_style(model, LV_LABEL_STYLE_MAIN, &model_style);
    lv_label_set_text(model, "ESP32-S2");
    lv_obj_align(model, view, LV_ALIGN_IN_TOP_RIGHT, -10, 15);


    //Line style
    static lv_style_t style_line;
    lv_style_copy(&style_line, &lv_style_plain);
    style_line.line.color = LV_COLOR_MAKE(0x00, 0x3b, 0x75);
    style_line.line.width = 3;
    style_line.line.rounded = 1;

    //Top horizontal line
    static lv_point_t line_points[] = { {10, 0}, {230, 0} };
    lv_obj_t *line1;
    line1 = lv_line_create(view, NULL);
    lv_line_set_points(line1, line_points, 2);     /*Set the points*/
    lv_line_set_style(line1, LV_LINE_STYLE_MAIN, &style_line);
    lv_obj_align(line1, NULL, LV_ALIGN_IN_TOP_MID, 0, 45);

    //Slogan below the top horizontal line
    lv_obj_t *row_down_line = lv_label_create(view, nullptr);
    lv_label_set_style(row_down_line, LV_LABEL_STYLE_MAIN, &model_style);
    lv_label_set_text(row_down_line, "Designed by lewis");
    lv_obj_align(row_down_line, line1, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 2);


    //Bottom horizontal line left
    static lv_point_t line_points1[] = { {0, 0}, {75, 0} };
    lv_obj_t *line2;
    line2 = lv_line_create(view, NULL);
    lv_line_set_points(line2, line_points1, 2);     /*Set the points*/
    lv_line_set_style(line2, LV_LINE_STYLE_MAIN, &style_line);
    lv_obj_align(line2, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 10, -30);

    //! Bottom line right
    static lv_point_t line_points2[] = { {0, 0}, {75, 0} };
    lv_obj_t *line3;
    line3 = lv_line_create(view, NULL);
    lv_line_set_points(line3, line_points2, 2);     /*Set the points*/
    lv_line_set_style(line3, LV_LINE_STYLE_MAIN, &style_line);
    lv_obj_align(line3, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -30);

    // Below the horizontal line in the upper left corner
    static lv_style_t text_style;
    lv_style_copy(&text_style, &lv_style_pretty_color);
    text_style.text.font  = &robot_light_16;
    text_style.text.color = LV_COLOR_BLACK;
    //! light
    lv_obj_t *light_text = lv_label_create(view, nullptr);
    lv_label_set_style(light_text, LV_LABEL_STYLE_MAIN, &text_style);
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
    lv_style_copy(&time_style, &lv_style_pretty_color);
    time_style.text.font  = &digital_play_st_48;
    time_style.text.color = LV_COLOR_BLACK;

    lv_obj_t *hour = lv_label_create(view, nullptr);
    lv_label_set_style(hour, LV_LABEL_STYLE_MAIN, &time_style);
    lv_label_set_text(hour, "10");
    lv_obj_align(hour, view, LV_ALIGN_CENTER, -50, 10);

    //semicolon
    static lv_style_t dot_style;
    lv_style_copy(&dot_style, &lv_style_pretty_color);
    dot_style.text.font  = &liquidCrystal_nor_64;   //liquidCrystal_nor_48
    dot_style.text.color = LV_COLOR_BLACK;

    lv_obj_t *dot = lv_label_create(view, nullptr);
    lv_label_set_style(dot, LV_LABEL_STYLE_MAIN, &dot_style);
    lv_label_set_text(dot, ":");
    lv_obj_align(dot, hour, LV_ALIGN_OUT_RIGHT_MID, 5, -10);

    //minute
    lv_obj_t *minute = lv_label_create(view, nullptr);
    lv_label_set_style(minute, LV_LABEL_STYLE_MAIN, &time_style);
    lv_label_set_text(minute, "14");
    lv_obj_align(minute, hour, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

    //Intermediate clock second digit
    static lv_style_t second_style;
    lv_style_copy(&second_style, &lv_style_pretty_color);
    second_style.text.font  = &liquidCrystal_nor_32;
    second_style.text.color = LV_COLOR_BLACK;

    lv_obj_t *second = lv_label_create(view, nullptr);
    lv_label_set_style(second, LV_LABEL_STYLE_MAIN, &second_style);
    lv_label_set_text(second, "34");
    lv_obj_align(second, minute, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, -10);

    //date
    static lv_style_t year_style;
    lv_style_copy(&year_style, &lv_style_pretty_color);
    year_style.text.font  = &liquidCrystal_nor_24;
    year_style.text.color = LV_COLOR_BLACK;

    lv_obj_t *year = lv_label_create(view, nullptr);
    lv_label_set_style(year, LV_LABEL_STYLE_MAIN, &year_style);
    lv_label_set_text(year, "2020");
    lv_obj_align(year, view, LV_ALIGN_CENTER, 0, 55);

    //Chinese font
    static lv_style_t chinese_style;
    lv_style_copy(&chinese_style, &lv_style_pretty_color);
    chinese_style.text.font  = &hansans_cn_24;
    chinese_style.text.color = LV_COLOR_BLACK;

    lv_obj_t *month = lv_label_create(view, nullptr);
    lv_label_set_style(month, LV_LABEL_STYLE_MAIN, &chinese_style);
    lv_label_set_text(month, "三月");
    lv_obj_align(month, year, LV_ALIGN_OUT_LEFT_MID, -5, 0);

    lv_obj_t *state = lv_label_create(view, nullptr);
    lv_label_set_style(state, LV_LABEL_STYLE_MAIN, &chinese_style);
    lv_label_set_text(state, "上午");
    lv_obj_align(state, year, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    lv_obj_t *week = lv_label_create(view, nullptr);
    lv_label_set_style(week, LV_LABEL_STYLE_MAIN, &chinese_style);
    lv_label_set_text(week, "周二");
    lv_obj_align(week, view, LV_ALIGN_CENTER, 45, -35);

    lv_obj_t *day = lv_label_create(view, nullptr);
    lv_label_set_style(day, LV_LABEL_STYLE_MAIN, &year_style);
    lv_label_set_text(day, "3");
    lv_obj_align(day, week, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    //! Battery string
    lv_obj_t *bat_text = lv_label_create(view, nullptr);
    lv_label_set_style(bat_text, LV_LABEL_STYLE_MAIN, &text_style);
    lv_label_set_text(bat_text, "BAT");
    lv_obj_align(bat_text, view, LV_ALIGN_IN_LEFT_MID, 20, -40);

    // temperature
    static lv_style_t temp_style;
    lv_style_copy(&temp_style, &lv_style_pretty_color);
    temp_style.text.font  = &quostige_16;
    temp_style.text.color = LV_COLOR_BLACK;

    lv_obj_t *temp_text = lv_label_create(view, nullptr);
    lv_label_set_style(temp_text, LV_LABEL_STYLE_MAIN, &temp_style);
    lv_label_set_text(temp_text, "24*C");
    lv_obj_align(temp_text, bat_text, LV_ALIGN_OUT_BOTTOM_MID, 0, -5);

    //Power
    static lv_style_t bat_style;
    lv_style_copy(&bat_style, &lv_style_pretty_color);
    bat_style.text.font  = &digital_play_st_24;
    bat_style.text.color = LV_COLOR_BLACK;

    lv_obj_t *bat = lv_label_create(view, nullptr);
    lv_label_set_style(bat, LV_LABEL_STYLE_MAIN, &bat_style);
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
    lv_line_set_style(line4, LV_LINE_STYLE_MAIN, &style_line);
    lv_obj_align(line4, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -2);

    // Bottom Go
    static lv_style_t key_style;
    lv_style_copy(&key_style, &lv_style_pretty_color);
    key_style.text.font  = &gracetians_32;
    key_style.text.color = LV_COLOR_BLACK;

    lv_obj_t *key = lv_label_create(view, nullptr);
    lv_label_set_style(key, LV_LABEL_STYLE_MAIN, &key_style);
    lv_label_set_text(key, "Go");
    lv_obj_align(key, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);


    static lv_style_t bot_style;
    lv_style_copy(&bot_style, &lv_style_pretty_color);
    bot_style.text.font  = &exninja_22;
    bot_style.text.color = LV_COLOR_BLACK;

    lv_obj_t *str1 = lv_label_create(view, nullptr);
    lv_label_set_style(str1, LV_LABEL_STYLE_MAIN, &bot_style);
    lv_label_set_text(str1, "WATER");
    lv_obj_align(str1, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 10, -2);

    lv_obj_t *str2 = lv_label_create(view, nullptr);
    lv_label_set_style(str2, LV_LABEL_STYLE_MAIN, &bot_style);
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