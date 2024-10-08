
// File generated by bin2lvgl
// developed by fbiego. 
// https://github.com/fbiego
// Watchface: DIAL_LOCKSCREEN

#ifndef _FACE_DIAL_LOCKSCREEN_H
#define _FACE_DIAL_LOCKSCREEN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

// #define ENABLE_FACE_DIAL_LOCKSCREEN // (dial lockscreen) uncomment to enable or define it elsewhere

#ifdef ENABLE_FACE_DIAL_LOCKSCREEN
    extern lv_obj_t *face_dial_lockscreen;
	extern lv_obj_t *face_dial_lockscreen_0_184;
	extern lv_obj_t *face_dial_lockscreen_2_76556;
	extern lv_obj_t *face_dial_lockscreen_3_81000;
	extern lv_obj_t *face_dial_lockscreen_4_81000;
	extern lv_obj_t *face_dial_lockscreen_5_82534;
	extern lv_obj_t *face_dial_lockscreen_6_82534;
	extern lv_obj_t *face_dial_lockscreen_7_82534;
	extern lv_obj_t *face_dial_lockscreen_8_82534;


	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_0_184_0);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_1_71734_0);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_1_71734_1);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_1_71734_2);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_1_71734_3);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_1_71734_4);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_1_71734_5);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_1_71734_6);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_2_76556_0);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_2_76556_1);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_2_76556_2);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_2_76556_3);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_2_76556_4);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_2_76556_5);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_2_76556_6);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_3_81000_0);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_3_81000_1);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_3_81000_2);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_3_81000_3);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_3_81000_4);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_3_81000_5);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_3_81000_6);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_3_81000_7);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_3_81000_8);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_3_81000_9);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_5_82534_0);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_5_82534_1);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_5_82534_2);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_5_82534_3);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_5_82534_4);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_5_82534_5);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_5_82534_6);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_5_82534_7);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_5_82534_8);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_5_82534_9);
	LV_IMG_DECLARE(face_dial_lockscreen_dial_img_preview_0);


#endif
    void onFaceEvent(lv_event_t * e);

    void init_face_dial_lockscreen(void (*callback)(const char *name, const lv_img_dsc_t *preview, lv_obj_t **watchface, int anim_img_num, lv_obj_t *anim_img, const lv_img_dsc_t **anim_img_group));
    void update_time_dial_lockscreen(int second, int minute, int hour, bool mode, bool am, int day, int month, int year, int weekday);
    void update_weather_dial_lockscreen(int temp, int icon);
    void update_status_dial_lockscreen(int battery, bool connection);
    void update_activity_dial_lockscreen(int steps, int distance, int kcal);
    void update_health_dial_lockscreen(int bpm, int oxygen);
    void update_all_dial_lockscreen(int second, int minute, int hour, bool mode, bool am, int day, int month, int year, int weekday, 
                int temp, int icon, int battery, bool connection, int steps, int distance, int kcal, int bpm, int oxygen);
    void update_check_dial_lockscreen(lv_obj_t *root, int second, int minute, int hour, bool mode, bool am, int day, int month, int year, int weekday, 
                int temp, int icon, int battery, bool connection, int steps, int distance, int kcal, int bpm, int oxygen);


#ifdef __cplusplus
}
#endif

#endif
