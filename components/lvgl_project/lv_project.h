#ifndef LV_PROJECT_H
#define LV_PROJECT_H

#include "lvgl.h"

#if 1
#define lv_project_debug( tag, format, ... ) \
ESP_LOGW(tag,""); \
printf(format,##__VA_ARGS__);
#else
#define lv_project_debug( tag, format, ... ) ;
#endif

#ifdef __cplusplus
extern "C" {
#endif

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

#endif

#endif // LV_PROJECT_H