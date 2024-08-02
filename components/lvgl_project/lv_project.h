#ifndef LV_PROJECT_H
#define LV_PROJECT_H

#include "lvgl.h"
#include "lv_hint.h"
#include "lv_setup.h"
#include "lv_watchfaces.h"
#include "lv_rollmsg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LV_HOR_RES_MAX 240
#define LV_VER_RES_MAX 280

void msg_roll(lv_obj_t * parent);//一定要先运行一次这个玩意，再添加新的消息

//添加新的消息 
//icon：图标，  
//title：标题，     
//txt：文本内容，  
//event_cb：点击的回调函数，不需要可填写0
bool lv_add_newmsg(const lv_img_dsc_t * icon,const char * title,const char * txt,lv_event_cb_t event_cb);
void test();


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // LV_PROJECT_H