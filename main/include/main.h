#ifndef __main__h
#define __main__h

#include "stdint.h"
#include <stdio.h>
#include <math.h>
#include <os.h>
#include "nvs_flash.h"
#include "esp_spi_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include <esp_log.h>
#include <arpa/inet.h>
#include "esp_wifi.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_system.h"
#include "cJSON.h"
#include "esp_timer.h"
#include "esp_event.h"
#include "driver/ledc.h"
#include "esp_spiffs.h"
#include "esp_sleep.h"
#include "esp_task_wdt.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"

#include "stdarg.h"
#include "nvs.h"
// #include "my_spiffs.h"
#include "lvgl.h"
// #include "lvgl_port.h"
// #include "my_system.h"
// #include "system_config_data.h"
// #include "mynvs.h"


// #include "cst816t.h"
// #include "wifi.h"
// #include "multi_button_task.h"
// #include "multi_button_callback.h"
#include "lcd.h"
// #include "lv_port_fs.h"
// #include "bsp_i2c.h"
// #include "bsp_sd.h"
// #include "bsp_key.h"
// #include "bsp_ledc.h"
// #include "bsp_power.h"
// #include "bsp_i2s.h"
// #include "bsp_i2c1.h"

// #include "webserver.h"

// #include "https_laohuangli.h"
// #include "https_shijian.h"
// #include "https_tianqi.h"
// #include "https_riluo.h"
// #include "https_kongqi.h"
// #include "https_bilibili.h"
// #include "https_chengshi.h"
// #include "https_ip.h"
// #include "https.h"
// #include "user_http_s.h"

// #include "wm8978.h"
//#include "iis.h"
// #include "i2s_test.h"

typedef enum
{
    LOG_NONE  = 0,   /*!< No log output */
    LOG_ERROR = 1,  /*!< Critical errors, software module can not recover on its own */
    LOG_WARN  = 2,   /*!< Error conditions from which recovery measures have been taken */
    LOG_INFO  = 3,   /*!< Information messages which describe normal flow of events */
    LOG_DEBUG = 4,  /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    LOG_VERBOSE = 5 /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} log_level_t;

#define COLOR_RESET   "\033[0m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_CYAN    "\033[36m"

static const char* get_color(esp_log_level_t level) {
    switch (level) {
        case LOG_ERROR:
            return COLOR_RED;
        case LOG_WARN:
            return COLOR_YELLOW;
        case LOG_DEBUG:
            return COLOR_WHITE;
        case LOG_INFO:
            return COLOR_GREEN;
        case LOG_VERBOSE:
            return COLOR_BLUE;
        default:
            return COLOR_RESET;
    }
}

// 自定义日志打印函数，添加文件名、行号和颜色
static void custom_log(log_level_t level, const char* tag, const char* file, int line, const char* format, ...) {
    const char* color = get_color(level);

    // 获取可变参数
    va_list args;
    va_start(args, format);

    // 打印日志信息
    printf("%s[%s] %s:%d: ", color, tag, file, line); // 打印调用 log_printf 的文件名和行号
    vprintf(format, args);
    printf(COLOR_RESET "\n");

    va_end(args);
}

// 定义控制日志级别的宏
#define SHOW_DEBUG 1  // 0: 不显示日志, 1: 仅显示 ERROR 和 INFO, 2: 显示所有日志

#if SHOW_DEBUG == 0
    #define log_printf(tag, level, format, ...)
#elif SHOW_DEBUG == 1
    #define log_printf(tag, level, format, ...) do { \
        if ((level) == LOG_ERROR || (level) == LOG_INFO) { \
            custom_log(level, tag, __FILE__, __LINE__, format, ##__VA_ARGS__); \
        } \
    } while (0)
#elif SHOW_DEBUG == 2
    #define log_printf(tag, level, format, ...) do { \
        custom_log(level, tag, __FILE__, __LINE__, format, ##__VA_ARGS__); \
    } while (0)
#else
    #define log_printf(tag, level, format, ...)
#endif

#define LV_LVGL_H_INCLUDE_SIMPLE

#endif
