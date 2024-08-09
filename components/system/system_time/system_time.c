#include "system_time.h"
#include "main.h"
#include "lv_hint.h"

static const char *SYSTEMTIME_TAG = "system_time";
struct tm timeinfo = { 0 };

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT) {
        if (event_id == WIFI_EVENT_STA_START) {
            esp_wifi_connect();
        } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
            esp_wifi_connect();
            log_printf(SYSTEMTIME_TAG, LOG_DEBUG, "retry to connect to the AP");
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        log_printf(SYSTEMTIME_TAG, LOG_INFO, "got ip:%s", ip4addr_ntoa(&event->ip_info.ip));
        lv_hint_create("wifi connected", 250, 2);
    }
}

void wifi_init_sta(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    esp_log_level_set("wifi", ESP_LOG_ERROR);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    log_printf(SYSTEMTIME_TAG, LOG_DEBUG, "wifi_init_sta finished.");
    log_printf(SYSTEMTIME_TAG, LOG_DEBUG, "connect to ap SSID:%s password:%s", WIFI_SSID, WIFI_PASS);
}

static void update_timeinfo(void) {
    time_t now;
    time(&now);
    localtime_r(&now, &timeinfo);
}

void time_sync_notification_cb(struct timeval *tv)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        log_printf(SYSTEMTIME_TAG, LOG_ERROR, "Failed to open NVS");
        return;
    }

    time_t now;
    time(&now);
    err = nvs_set_i64(nvs_handle, "last_sync_time", now);
    if (err != ESP_OK) {
        log_printf(SYSTEMTIME_TAG, LOG_ERROR, "Failed to save time to NVS");
    } else {
        nvs_commit(nvs_handle);
        log_printf(SYSTEMTIME_TAG, LOG_DEBUG, "Time saved to NVS");
    }
    nvs_close(nvs_handle);
}

// SNTP 时间同步任务
void sntp_sync_task(void *pvParameters) {
    // 等待网络连接成功
    log_printf(SYSTEMTIME_TAG, LOG_DEBUG, "Starting SNTP sync task...");
    obtain_time();

    // 删除任务
    vTaskDelete(NULL);
}

void initialize_sntp(void)
{
    log_printf(SYSTEMTIME_TAG, LOG_DEBUG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "ntp.aliyun.com");
    sntp_setservername(1, "cn.pool.ntp.org");
    sntp_setservername(2, "ntp.tencent.com");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
}

bool obtain_time(void)
{
    initialize_sntp();

    time_t now = 0;
    int retry = 0;
    const int retry_count = 10;
    while (timeinfo.tm_year < (2022 - 1900) && ++retry < retry_count) {
        log_printf(SYSTEMTIME_TAG, LOG_WARN, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (retry == retry_count) {
        log_printf(SYSTEMTIME_TAG, LOG_ERROR, "Failed to obtain time");
        return false;
    } else {
        setenv("TZ", "CST-8", 1);
        tzset();
        log_printf(SYSTEMTIME_TAG, LOG_INFO, "Time is set to: %s", asctime(&timeinfo));
        lv_hint_create("time set successfully", 250, 2);
        return true;
    }
}

bool load_time_from_nvs(void)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        log_printf(SYSTEMTIME_TAG, LOG_ERROR, "Failed to open NVS");
        return false;
    }

    int64_t last_sync_time = 0;
    err = nvs_get_i64(nvs_handle, "last_sync_time", &last_sync_time);
    if (err != ESP_OK) {
        log_printf(SYSTEMTIME_TAG, LOG_ERROR, "No time found in NVS");
    } else {
        struct timeval tv = { .tv_sec = (time_t)last_sync_time };
        settimeofday(&tv, NULL);
        log_printf(SYSTEMTIME_TAG, LOG_DEBUG, "Time loaded from NVS");
    }
    nvs_close(nvs_handle);

    setenv("TZ", "CST-8", 1);
    tzset();
    return true;
}

struct tm system_time_get_timeinfo(void)
{
    update_timeinfo();
    return timeinfo;
}

int system_time_get_second(void)
{
    update_timeinfo();
    return timeinfo.tm_sec;
}

int system_time_get_minute(void)
{
    update_timeinfo();
    return timeinfo.tm_min;
}

int system_time_get_hour(void)
{
    update_timeinfo();
    return timeinfo.tm_hour;
}

int system_time_get_day(void)
{
    update_timeinfo();
    return timeinfo.tm_mday;
}

int system_time_get_month(void)
{
    update_timeinfo();
    return timeinfo.tm_mon + 1;
}

int system_time_get_year(void)
{
    update_timeinfo();
    return timeinfo.tm_year + 1900;
}

int system_time_get_weekday(void)
{
    update_timeinfo();
    return timeinfo.tm_wday;
}

int system_time_get_yearday(void)
{
    update_timeinfo();
    return timeinfo.tm_yday;
}

void system_time_print(void)
{
    update_timeinfo();
    printf("当前时间：%4d年%02d月%02d日 %02d:%02d:%02d\n", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}

void system_time_format_print(const char* format)
{
    update_timeinfo();
    char buffer[256];
    int pos = 0;

    for (const char *p = format; *p != '\0'; ++p) {
        if (*p == '%') {
            ++p;
            if (*p == '\0') break;

            switch (*p) {
                case 'Y': pos += sprintf(buffer + pos, "%d", timeinfo.tm_year + 1900); break;
                case 'M': pos += sprintf(buffer + pos, "%02d", timeinfo.tm_mon + 1); break;
                case 'D': pos += sprintf(buffer + pos, "%02d", timeinfo.tm_mday); break;
                case 'h': pos += sprintf(buffer + pos, "%02d", timeinfo.tm_hour); break;
                case 'm': pos += sprintf(buffer + pos, "%02d", timeinfo.tm_min); break;
                case 's': pos += sprintf(buffer + pos, "%02d", timeinfo.tm_sec); break;
                default: buffer[pos++] = '%'; buffer[pos++] = *p; break;
            }
        } else {
            buffer[pos++] = *p;
        }
    }

    buffer[pos] = '\0';
    printf("当前时间：%s\n", buffer);
}
