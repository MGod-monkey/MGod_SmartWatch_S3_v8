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
// #include "lv_game_2048.h"
#include "lv_project.h"
#include "system_time.h"
#include "system_config.h"
#include "hal_sd.h"
// #include "main.h"
// #include <dos.h>

// #include "lv_demos.h"
// LV_IMG_DECLARE(bmp_tianqi_40x40_0)
void app_main(void)
{
    // printf("Hello world!\n");
    // esp_log_set_vprintf(custom_vprintf);
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    system_init();
    hal_sd_init();
    // setupView_init();
    // wifi_init_sta();
    // load_time_from_nvs();  // 从NVS中加载时间
    // xTaskCreate(sntp_sync_task, "SNTP Sync Task", SNTP_TASK_STACK_SIZE, NULL, SNTP_TASK_PRIORITY, NULL);
    // test();
    // lv_game_2048_simple_test();
    while(1)
    {
        lv_task_handler();
        // watchface_update();
        lv_tick_inc(10);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
