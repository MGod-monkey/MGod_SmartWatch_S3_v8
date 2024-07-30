/*
 * @Descripttion: 文件描述
 * @version: 文件版本
 * @Author: MGodmonkey
 * @Date: 2024-06-05 10:15:53
 * @LastEditors: MGodmonkey
 * @LastEditTime: 2024-07-24 16:59:53
 * @FilePath: \lvgl\main\main.c
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_game_2048.h"
#include "lv_project.h"


// #include "lv_demos.h"
LV_IMG_DECLARE(bmp_tianqi_40x40_0)
void app_main(void)
{
    // printf("Hello world!\n");
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    watchface_init();

    // watch.setConnectionCallback(connectionCallback);
    // watch.setNotificationCallback(notificationCallback);
    // watch.setConfigurationCallback(configCallback);
    // watch.begin();
    // watch.set24Hour(true);
    // watch.setBattery(70);
    // lv_game_2048_simple_test();
    // lv_hint_create(lv_scr_act(),"task", 200, 2);
    // test();
    // lvgl_hint_create(lv_scr_act(),"task",200,20);
    // // 在画板上写上文字
    // lv_obj_t *label = lv_label_create( lv_scr_act() );
    // // 在画板上写上文字
    // lv_label_set_text( label, "Hello World!I'm fine!" );
    // // 设置画板上的对齐方式，也就是布局
    // lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
    while(1)
    {
        lv_task_handler();
        lv_tick_inc(10);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// void update_faces()
// {
//   int second = watch.getSecond();
//   int minute = watch.getMinute();
//   int hour = watch.getHourC();
//   bool mode = watch.is24Hour();
//   bool am = watch.getHour(true) < 12;
//   int day = watch.getDay();
//   int month = watch.getMonth() + 1;
//   int year = watch.getYear();
//   int weekday = watch.getDayofWeek();

//   int temp = watch.getWeatherAt(0).temp;
//   int icon = watch.getWeatherAt(0).icon;

//   int battery = watch.getPhoneBattery();
//   int connection = watch.isConnected();

//   int steps = 2735;
//   int distance = 17;
//   int kcal = 348;
//   int bpm = 76;
//   int oxygen = 97;

//   update_check_34_2_dial(ui_home, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
//   update_check_analog(ui_home, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
//   update_check_blue_dial(ui_home, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
//   update_check_116_2_dial(ui_home, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
//   update_check_756_2_dial(ui_home, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
//   update_check_b_w_resized(ui_home, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
//   update_check_kenya(ui_home, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
//   update_check_pixel_resized(ui_home, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
//   update_check_radar(ui_home, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
//   update_check_smart_resized(ui_home, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
//   update_check_tix_resized(ui_home, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
//   update_check_wfb_resized(ui_home, second, minute, hour, mode, am, day, month, year, weekday, temp, icon, battery, connection, steps, distance, kcal, bpm, oxygen);
// }
