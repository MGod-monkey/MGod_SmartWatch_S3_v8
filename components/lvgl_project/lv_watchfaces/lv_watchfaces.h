#ifndef LV_WATCHFACES_H
#define LV_WATCHFACES_H

#include "lvgl.h"
struct Face
{
    const char *name;            // watchface name
    const lv_img_dsc_t *preview; // watchface preview image
    lv_obj_t *anim_img;          // animation image object
    lv_img_dsc_t **anim_img_group; // animation image group
    int anim_img_num;            // number of animation images
    lv_obj_t **watchface;        // watchface root object pointer
};

#define LV_WATCHFACES_MAX_FACES 10
extern struct Face faces[LV_WATCHFACES_MAX_FACES];
void watchface_update();
void watchface_init();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // LV_WATCHFACES_H
