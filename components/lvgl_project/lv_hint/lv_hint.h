#ifndef __lv_hint_H__
#define __lv_hint_H__

#include "lvgl.h"
#include "main.h"
// #include "lv_Project.h"
// #include "my_system.h"

#if 1
	#define lv_hint_anim_debug(format, ...) lv_project_debug("[提示-动效]- ",format,##__VA_ARGS__);
#else
	#define lv_hint_anim_debug(format, ...);
#endif

#if 1
	#define hint_debug(format, ...) lv_project_debug("[提示]- ",format,##__VA_ARGS__);
#else
	#define hint_debug(format, ...) ;
#endif

typedef struct
{
	lv_anim_t lv_anim_jin;
	lv_anim_t lv_anim_chu;

	lv_timer_t * lv_task;

	lv_obj_t * lv_hint;
	lv_obj_t * lv_btn_tuichu;
}_hint;

extern _hint hint;


// typedef struct
// {
// 	char buf[20];

// 	int shijian;
// 	int DongHua_Flag;

// 	lv_point_t point;
// 	uint8_t delete_flag;

// }_lv_tishi_GongNeng_Data;

// extern _lv_tishi_GongNeng_Data lv_tishi_GongNeng_Data;


// extern lv_obj_t *lv_tishi_main_cont;

void lv_hint_anim_jin(void);
void lv_hint_anim_chu(void);
void lv_hint_create(lv_obj_t * scr, const char *text, uint8_t touming, uint8_t delay);
void lv_hint_close(void);
void lv_hint_taskCb(lv_timer_t *t);


#endif