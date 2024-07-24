/**
 * @file lv_port_indev_templ.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void touchpad_init(void);
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
// static bool touchpad_is_pressed(void);
// static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);

// static void mouse_init(void);
// static void mouse_read(lv_indev_t * indev, lv_indev_data_t * data);
// static bool mouse_is_pressed(void);
// static void mouse_get_xy(int32_t * x, int32_t * y);

// static void keypad_init(void);
// static void keypad_read(lv_indev_t * indev, lv_indev_data_t * data);
// static uint32_t keypad_get_key(void);

// static void encoder_init(void);
// static void encoder_read(lv_indev_t * indev, lv_indev_data_t * data);
// static void encoder_handler(void);

// static void button_init(void);
// static void button_read(lv_indev_t * indev, lv_indev_data_t * data);
// static int8_t button_get_pressed_id(void);
// static bool button_is_pressed(uint8_t id);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_touchpad;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
    /**
     * Here you will find example implementation of input devices supported by LittelvGL:
     *  - Touchpad
     *  - Mouse (with cursor support)
     *  - Keypad (supports GUI usage only with key)
     *  - Encoder (supports GUI usage only with: left, right, push)
     *  - Button (external buttons to press points on the screen)
     *
     *  The `..._read()` function are only examples.
     *  You should shape them according to your hardware
     */

    static lv_indev_drv_t indev_drv;

    /*------------------
     * Touchpad
     * -----------------*/

    /*Initialize your touchpad if you have*/
    touchpad_init();

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_touchpad = lv_indev_drv_register(&indev_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Touchpad
 * -----------------*/

/*Initialize your touchpad*/
static void touchpad_init(void)
{
    /*Your code comes here*/
    cst816t_init();
}

/*Will be called by the library to read the touchpad*/
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint8_t tp_num = 0;
    static uint16_t x = 0, y = 0;
    // 返回触摸位置
    cst816t_read_pos(&tp_num, &x, &y);

    if (0 == tp_num)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        /* Swap xy order and invert y direction */
        data->point.x = x;
        data->point.y = y; // LV_VER_RES_MAX - x - 1;
        data->state = LV_INDEV_STATE_PR;
    }
}

/*Return true is the touchpad is pressed*/
// static bool touchpad_is_pressed(uint8_t tp_num)
// {
//     /*Your code comes here*/
//     if (tp_num == 0)
//         return false;
//     else
//         return true;
// }

/*Get the x and y coordinates if the touchpad is pressed*/
// static void touchpad_get_xy(int32_t * x, int32_t * y)
// {
//     /*Your code comes here*/
//     static uint8_t tp_num = 0;
//     cst816t_read_pos(&tp_num, &x, &y);
// }


/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
