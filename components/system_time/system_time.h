#ifndef SYSTEM_TIME_H
#define SYSTEM_TIME_H

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_sntp.h"
#include "nvs_flash.h"
#include "time.h"
#include "nvs.h"

static const char *SYSTEMTIME_TAG = "system_time";

#define WIFI_SSID      "INSS-Lab_2.4G"
#define WIFI_PASS      "G-STAR1211"

extern struct tm timeinfo;
// 定义 SNTP 同步任务堆栈大小和任务优先级
#define SNTP_TASK_STACK_SIZE 4096
#define SNTP_TASK_PRIORITY   5

void wifi_init_sta(void);
bool obtain_time(void);
bool load_time_from_nvs(void);
void sntp_sync_task(void *pvParameters);
struct tm system_time_get_timeinfo(void);
int system_time_get_second(void);
int system_time_get_minute(void);
int system_time_get_hour(void);
int system_time_get_day(void);
int system_time_get_month(void);
int system_time_get_year(void);
int system_time_get_weekday(void);
int system_time_get_yearday(void);
void system_time_print(void);
void system_time_format_print(const char* format);

#endif