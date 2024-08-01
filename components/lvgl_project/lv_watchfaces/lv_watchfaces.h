#ifndef LV_WATCHFACES_H
#define LV_WATCHFACES_H

#include "lvgl.h"
#include "main.h"
#include "system_time.h"
#include "./analog_81/analog_81.h"
#include "./analog_nike/analog_nike.h"
#include "./analog_starcraft2/analog_starcraft2.h"
#include "./dial_astronaut/dial_astronaut.h"
#include "./dial_astronaut2/dial_astronaut2.h"
// #include "./dial_astronaut3/dial_astronaut3.h"
#include "./dial_blackboard/dial_blackboard.h"
#include "./dial_dog/dial_dog.h"
#include "./dial_filled/dial_filled.h"
#include "./dial_lockscreen/dial_lockscreen.h"
#include "./dial_oldperson/dial_oldperson.h"
#include "./dial_rabbit/dial_rabbit.h"

#if 1
	#define lv_watchface_debug(format, ...) lv_project_debug("[表盘]- ",format,##__VA_ARGS__);
#else
	#define lv_watchface_debug(format, ...);
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LV_WATCHFACES_MAX_FACES 15
lv_obj_t *ui_faceSelect;
lv_obj_t *ui_watchface;

int numFaces = 0;

struct Face
{
    const char *name;            // watchface name
    const lv_img_dsc_t *preview; // watchface preview image
    lv_obj_t **watchface;        // watchface root object pointer
};

struct Face faces[LV_WATCHFACES_MAX_FACES];


void onFaceSelected(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    int index = (int)lv_event_get_user_data(e);

    if (event_code == LV_EVENT_CLICKED)
    {
        if (index >= numFaces)
            return;
        ui_watchface = *faces[index].watchface;
        lv_scr_load_anim(ui_watchface, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
    }
}

void onFaceEvent(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (event_code == LV_EVENT_LONG_PRESSED)
        lv_scr_load_anim(ui_faceSelect, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
}

void addWatchface(const char *name, const lv_img_dsc_t *src, int index)
{
    lv_obj_t *ui_faceItem = lv_obj_create(ui_faceSelect);
    lv_obj_set_width(ui_faceItem, 160);
    lv_obj_set_height(ui_faceItem, 180);
    lv_obj_set_align(ui_faceItem, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_faceItem, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_faceItem, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_faceItem, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_faceItem, lv_color_hex(0x142ABC), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_faceItem, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_faceItem, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_faceItem, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *ui_facePreview = lv_img_create(ui_faceItem);
    lv_img_set_src(ui_facePreview, src);
    lv_obj_set_width(ui_facePreview, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_facePreview, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_facePreview, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(ui_facePreview, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_facePreview, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    lv_obj_t *ui_faceLabel = lv_label_create(ui_faceItem);
    lv_obj_set_width(ui_faceLabel, 160);
    lv_obj_set_height(ui_faceLabel, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_faceLabel, LV_ALIGN_BOTTOM_MID);
    lv_label_set_long_mode(ui_faceLabel, LV_LABEL_LONG_DOT);
    lv_label_set_text(ui_faceLabel, name);
    lv_obj_set_style_text_align(ui_faceLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_faceLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_faceItem, onFaceSelected, LV_EVENT_ALL, (void *)index);
}

void init_face_select()
{
    ui_faceSelect = lv_obj_create(NULL);
    lv_obj_set_width(ui_faceSelect, 240);
    lv_obj_set_height(ui_faceSelect, 280);
    lv_obj_set_align(ui_faceSelect, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_faceSelect, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_faceSelect, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_faceSelect, LV_OBJ_FLAG_SNAPPABLE); /// Flags
    lv_obj_set_scrollbar_mode(ui_faceSelect, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_radius(ui_faceSelect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_faceSelect, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_faceSelect, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_faceSelect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_faceSelect, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_faceSelect, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_faceSelect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_faceSelect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_faceSelect, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_faceSelect, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void registerWatchface_cb(const char *name, const lv_img_dsc_t *preview, lv_obj_t **watchface)
{
    if (numFaces >= LV_WATCHFACES_MAX_FACES)
    {
        return;
    }
    faces[numFaces].name = name;
    faces[numFaces].preview = preview;
    faces[numFaces].watchface = watchface;
    addWatchface(faces[numFaces].name, faces[numFaces].preview, numFaces);
    lv_watchface_debug("添加表盘: %s\r\n", faces[numFaces].name);
    numFaces++;
}

void watchface_init()
{
    // lv_disp_t *disp = lv_disp_get_default();
    // lv_theme_t *theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    // lv_disp_set_theme(disp, theme);

    ui_watchface = lv_obj_create(NULL);

    init_face_select();
    init_face_dial_astronaut(registerWatchface_cb);
    init_face_dial_astronaut2(registerWatchface_cb);
    // init_face_dial_astronaut3(registerWatchface_cb);
    init_face_dial_blackboard(registerWatchface_cb);
    // init_face_dial_dog(registerWatchface_cb);
    init_face_dial_filled(registerWatchface_cb);
    init_face_dial_lockscreen(registerWatchface_cb);
    init_face_dial_oldperson(registerWatchface_cb);
    init_face_dial_rabbit(registerWatchface_cb);
    init_face_analog_81(registerWatchface_cb);
    init_face_analog_starcraft2(registerWatchface_cb);
    // init_face_analog_nike(registerWatchface_cb);

    if (numFaces == 0)
    {
        lv_obj_t *label1 = lv_label_create(ui_watchface);
        lv_obj_align(label1, LV_ALIGN_TOP_MID, 0, 100);
        lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);
        lv_obj_set_width(label1, 210);
        lv_label_set_text(label1, "No watchfaces detected. Check that they are enabled");

        lv_obj_t *slider1 = lv_slider_create(ui_watchface);
        lv_obj_set_width(slider1, 200);
        lv_obj_align_to(slider1, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 50);
    }
    else
    {
        ui_watchface = *faces[1].watchface;
    }

    lv_disp_load_scr(ui_watchface);
}

void watchface_update()
{
    int second = system_time_get_second();
    int minute = system_time_get_minute();
    int hour = system_time_get_hour();
    bool mode = 1;
    bool am = hour < 12;
    int day = system_time_get_day();
    int month = system_time_get_month();
    int year = system_time_get_year();
    int weekday = system_time_get_weekday();

    int temp = 0;
    int icon = 0;

    int battery = 69;
    int connection = 1;

    int steps = 2735;
    int distance = 17;
    int kcal = 348;
    int bpm = 76;
    int oxygen = 97;

    update_check_analog_81(ui_watchface, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
    update_check_analog_starcraft2(ui_watchface, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
    update_check_dial_astronaut(ui_watchface, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
    update_check_dial_astronaut2(ui_watchface, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
    update_check_dial_blackboard(ui_watchface, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
    update_check_dial_filled(ui_watchface, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
    update_check_dial_lockscreen(ui_watchface, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
    update_check_dial_oldperson(ui_watchface, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
    update_check_dial_rabbit(ui_watchface, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
}

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // LV_WATCHFACES_H