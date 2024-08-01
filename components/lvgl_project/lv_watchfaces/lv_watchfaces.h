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
#include "./dial_blackboard/dial_blackboard.h"
#include "./dial_dog/dial_dog.h"
#include "./dial_filled/dial_filled.h"
#include "./dial_lockscreen/dial_lockscreen.h"
#include "./dial_oldperson/dial_oldperson.h"
#include "./dial_rabbit/dial_rabbit.h"

#define ANIMATION_FRAME_RATE 30 // 动画帧率
static const char *WATCHFACE_TAG = "Watchfaces";
#define LV_WATCHFACES_MAX_FACES 10

lv_obj_t *ui_faceSelect = NULL;
lv_obj_t *ui_watchface = NULL;
int ui_anim_img_num = 0;
static int numFaces = 0;            // 当前表盘索引
static int current_frame_index = 0; // 当前动画帧索引
lv_timer_t *timer_anim = NULL;
bool is_flushTime = false;

struct Face
{
    const char *name;            // watchface name
    const lv_img_dsc_t *preview; // watchface preview image
    lv_obj_t *anim_img;          // animation image object
    lv_img_dsc_t **anim_img_group; // animation image group
    int anim_img_num;            // number of animation images
    lv_obj_t **watchface;        // watchface root object pointer
};

struct Face faces[LV_WATCHFACES_MAX_FACES];

void onFaceSelected(lv_event_t *e)
{
    int index = (int)lv_event_get_user_data(e);
    if (index >= LV_WATCHFACES_MAX_FACES)
        return;

    numFaces = index;
    ui_watchface = *faces[index].watchface;
    ui_anim_img_num = faces[index].anim_img_num;
    lv_scr_load_anim(ui_watchface, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
    lv_timer_resume(timer_anim);
    is_flushTime = true;
}

void onFaceEvent(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_LONG_PRESSED)
    {
        is_flushTime = false;
        lv_timer_pause(timer_anim);
        lv_scr_load_anim(ui_faceSelect, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
    }
}

void addWatchface(const char *name, const lv_img_dsc_t *src, int index)
{
    lv_obj_t *ui_faceItem = lv_obj_create(ui_faceSelect);
    lv_obj_set_size(ui_faceItem, 160, 180);
    lv_obj_align(ui_faceItem, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(ui_faceItem, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_faceItem, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_faceItem, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_faceItem, lv_color_hex(0x142ABC), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_faceItem, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_faceItem, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_faceItem, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *ui_facePreview = lv_img_create(ui_faceItem);
    lv_img_set_src(ui_facePreview, src);
    lv_obj_align(ui_facePreview, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_flag(ui_facePreview, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_facePreview, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *ui_faceLabel = lv_label_create(ui_faceItem);
    lv_obj_set_size(ui_faceLabel, 160, LV_SIZE_CONTENT);
    lv_obj_align(ui_faceLabel, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_label_set_long_mode(ui_faceLabel, LV_LABEL_LONG_DOT);
    lv_label_set_text(ui_faceLabel, name);
    lv_obj_set_style_text_align(ui_faceLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_faceLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_faceItem, onFaceSelected, LV_EVENT_CLICKED, (void *)index);
}

void init_face_select()
{
    ui_faceSelect = lv_obj_create(NULL);
    lv_obj_set_size(ui_faceSelect, 240, 280);
    lv_obj_align(ui_faceSelect, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_flex_flow(ui_faceSelect, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_faceSelect, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_faceSelect, LV_OBJ_FLAG_SNAPPABLE);
    lv_obj_set_scrollbar_mode(ui_faceSelect, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_radius(ui_faceSelect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_faceSelect, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_faceSelect, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_faceSelect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_faceSelect, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_faceSelect, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_faceSelect, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void registerWatchface_cb(const char *name, const lv_img_dsc_t *preview, lv_obj_t **watchface, int anim_img_num, lv_obj_t *anim_img, const lv_img_dsc_t **anim_img_group)
{
    if (numFaces >= LV_WATCHFACES_MAX_FACES)
    {
        log_printf(WATCHFACE_TAG, LOG_ERROR, "Max watchfaces reached");
        return;
    }

    faces[numFaces] = (struct Face){
        .name = name,
        .preview = preview,
        .watchface = watchface,
        .anim_img_num = anim_img_num,
        .anim_img = anim_img,
        .anim_img_group = anim_img_group
    };

    addWatchface(faces[numFaces].name, faces[numFaces].preview, numFaces);
    log_printf(WATCHFACE_TAG, LOG_DEBUG, "add watchface: %s", faces[numFaces].name);
    numFaces++;
}

static void animation_timer_cb(lv_timer_t *timer)
{
    if (ui_anim_img_num == 0)
        return;

    current_frame_index = (current_frame_index + 1) % ui_anim_img_num;
    lv_img_set_src(faces[numFaces].anim_img, faces[numFaces].anim_img_group[current_frame_index]);
}

void watchface_init()
{
    ui_watchface = lv_obj_create(NULL);
    init_face_select();

    #define INIT_WATCHFACE(func) func(registerWatchface_cb)
    INIT_WATCHFACE(init_face_dial_astronaut);
    INIT_WATCHFACE(init_face_dial_astronaut2);
    INIT_WATCHFACE(init_face_dial_blackboard);
    INIT_WATCHFACE(init_face_dial_filled);
    INIT_WATCHFACE(init_face_dial_lockscreen);
    INIT_WATCHFACE(init_face_dial_oldperson);
    INIT_WATCHFACE(init_face_dial_rabbit);
    INIT_WATCHFACE(init_face_analog_81);
    INIT_WATCHFACE(init_face_analog_starcraft2);
    #undef INIT_WATCHFACE

    if (numFaces == 0)
    {
        lv_obj_t *label1 = lv_label_create(ui_watchface);
        lv_obj_set_size(label1, 210, LV_SIZE_CONTENT);
        lv_obj_align(label1, LV_ALIGN_TOP_MID, 0, 100);
        lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);
        lv_label_set_text(label1, "No watchfaces detected. Check that they are enabled");

        lv_obj_t *slider1 = lv_slider_create(ui_watchface);
        lv_obj_set_width(slider1, 200);
        lv_obj_align_to(slider1, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 50);
    }
    else
    {
        ui_watchface = *faces[1].watchface;
        ui_anim_img_num = faces[1].anim_img_num;
        numFaces = 1;
    }

    lv_disp_load_scr(ui_watchface);
    int refresh_interval = (int)(1000.0 / ANIMATION_FRAME_RATE + 0.5);
    timer_anim = lv_timer_create(animation_timer_cb, refresh_interval, NULL);
    is_flushTime = true;
}

void watchface_update()
{
    if (!is_flushTime)
        return;

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

    #define UPDATE_WATCHFACE(func) func(ui_watchface, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen)
    UPDATE_WATCHFACE(update_check_analog_81);
    UPDATE_WATCHFACE(update_check_analog_nike);
    UPDATE_WATCHFACE(update_check_analog_starcraft2);
    UPDATE_WATCHFACE(update_check_dial_astronaut);
    UPDATE_WATCHFACE(update_check_dial_astronaut2);
    UPDATE_WATCHFACE(update_check_dial_blackboard);
    UPDATE_WATCHFACE(update_check_dial_dog);
    UPDATE_WATCHFACE(update_check_dial_filled);
    UPDATE_WATCHFACE(update_check_dial_lockscreen);
    UPDATE_WATCHFACE(update_check_dial_oldperson);
    UPDATE_WATCHFACE(update_check_dial_rabbit);
    #undef UPDATE_WATCHFACE
}

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // LV_WATCHFACES_H
