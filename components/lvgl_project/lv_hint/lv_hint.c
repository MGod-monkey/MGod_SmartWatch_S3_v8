#include "lv_hint.h"

_hint hint;
lv_style_t lv_tishi_style;

static void lv_hint_event_cb(lv_event_t * e)
{
    // lv_obj_t * obj = lv_event_get_target(e);
	lv_hint_debug("删除窗口\r\n");
	lv_hint_close();
}

void lv_hint_create(lv_obj_t * scr, const char *text, uint8_t touming, uint8_t delay)
{
    lv_style_init(&lv_tishi_style);
    lv_style_set_radius(&lv_tishi_style, 10);//圆角
    /*背景色*/
    lv_style_set_bg_opa(&lv_tishi_style, touming);//背景透明度
    lv_style_set_bg_color(&lv_tishi_style, lv_color_make(0x00, 0x00, 0x00));//背景上面颜色
    lv_style_set_bg_grad_color(&lv_tishi_style, lv_color_make(0x00, 0x00, 0x00));//背景上面颜色
    lv_style_set_bg_grad_dir(&lv_tishi_style, LV_GRAD_DIR_VER);//渐变方向
    lv_style_set_align(&lv_tishi_style, LV_ALIGN_TOP_MID);

    //----创建容器----//
    hint.lv_hint = lv_obj_create(scr);
    lv_obj_set_pos(hint.lv_hint, 0, 250);
    lv_obj_set_size(hint.lv_hint, 120, 30);
    lv_obj_add_flag(hint.lv_hint, LV_OBJ_FLAG_CLICKABLE); //启用可点击
    lv_obj_add_flag(hint.lv_hint, LV_OBJ_FLAG_SCROLLABLE); //启用可拖动
    lv_obj_set_scroll_dir(hint.lv_hint, LV_DIR_ALL); //设置拖动方向
    lv_obj_set_scroll_snap_x(hint.lv_hint, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scroll_snap_y(hint.lv_hint, LV_SCROLL_SNAP_CENTER);

    lv_obj_clear_flag(hint.lv_hint, LV_OBJ_FLAG_SCROLL_CHAIN); //禁用父对象可拖动
    lv_obj_add_style(hint.lv_hint, &lv_tishi_style, 0); //设置样式
    lv_obj_add_event_cb(hint.lv_hint, lv_hint_event_cb, LV_EVENT_CLICKED | LV_EVENT_PRESSED, NULL); //设置回调函数

    lv_obj_t* lv_Label_text = lv_label_create(hint.lv_hint);
    lv_obj_set_style_text_color(lv_Label_text, lv_color_white(), 0);
    lv_label_set_long_mode(lv_Label_text, LV_LABEL_LONG_SCROLL_CIRCULAR);
    // lv_obj_set_height(lv_Label_text, 30);
    lv_obj_set_align(lv_Label_text, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(lv_Label_text, &lv_font_montserrat_14, 0);
    lv_label_set_text(lv_Label_text, text);

    if (delay != 0)
        hint.lv_task = lv_timer_create(lv_hint_taskCb, delay*1000, &hint); //创建定时器
    lv_hint_debug("创建窗口\r\n");

	lv_obj_add_flag(hint.lv_hint, LV_OBJ_FLAG_FLOATING); // 置顶

	lv_hint_anim_jin();
}

void lv_hint_taskCb(lv_timer_t *t)
{
	lv_hint_close();
}

void lv_hint_close(void)
{
	lv_hint_anim_chu();
	lv_hint_debug("关闭窗口\r\n");
	lv_timer_del(hint.lv_task);
	hint.lv_task = NULL;
    lv_obj_del_async(hint.lv_hint);
}

// 提示窗口进入动效
void lv_hint_anim_jin(void)
{
	lv_anim_init(&hint.lv_anim_jin);
	lv_anim_set_exec_cb(&hint.lv_anim_jin, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_values(
		&hint.lv_anim_jin,
		lv_obj_get_height(lv_scr_act()),
		lv_obj_get_height(lv_scr_act())-50
	);

	lv_anim_set_time(&hint.lv_anim_jin, 300);
	lv_anim_set_var(&hint.lv_anim_jin, hint.lv_hint);
	lv_anim_set_path_cb(&hint.lv_anim_jin, lv_anim_path_overshoot);

	lv_anim_start(&hint.lv_anim_jin);
}

// 提示窗口出去动效
void lv_hint_anim_chu(void)
{
	lv_anim_init(&hint.lv_anim_chu);
	lv_anim_set_exec_cb(&hint.lv_anim_chu, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_values(
		&hint.lv_anim_chu,
		lv_obj_get_y(hint.lv_hint),
		lv_obj_get_height(lv_scr_act())
	);

	lv_anim_set_time(&hint.lv_anim_chu, 300);
	lv_anim_set_var(&hint.lv_anim_chu, hint.lv_hint);
	lv_anim_set_path_cb(&hint.lv_anim_chu, lv_anim_path_overshoot);
	
	lv_anim_start(&hint.lv_anim_chu);
}

