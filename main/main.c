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
#include "system_time.h"
#include "main.h"
// #include <dos.h>

// #include "lv_demos.h"
LV_IMG_DECLARE(bmp_tianqi_40x40_0)
void app_main(void)
{
    // printf("Hello world!\n");
    // esp_log_set_vprintf(custom_vprintf);
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    watchface_init();
    
    
    wifi_init_sta();
    if (!obtain_time())
        load_time_from_nvs();  // 从NVS中加载时间

    // // 如果时间未同步过，执行SNTP同步
    // if (timeinfo.tm_year < (2023 - 1900)) {
    //     obtain_time();
    // }
    // system_time_print();  // 打印时间
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
        watchface_update();
        lv_tick_inc(10);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
