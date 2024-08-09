#include "hal_power.h"
#include "driver/gpio.h"
#include "main.h"
#include "esp_sleep.h"
#include "freertos/semphr.h"
#include "system_config.h"
#include "lv_hint.h"
#include "hal_ledc.h"

// 假设这些函数和变量在其他文件中定义
extern int hal_key_read_power_gpio(void);
extern _system_data system_data;
static const char *HALPOWER_TAG = "Hal Power";

void hal_power_on(void)
{
    gpio_set_level(IO_POWER_PIN, 1);
    gpio_set_level(IO_POWER_PIN, 1);
}

void hal_power_off(void)
{
    gpio_set_level(IO_POWER_PIN, 0);
}

xSemaphoreHandle hal_power_Semaphore;

void hal_power_send_off(void)
{
    xSemaphoreGive(hal_power_Semaphore); // 释放信号量
}

static portMUX_TYPE lock = portMUX_INITIALIZER_UNLOCKED;
static uint8_t power_flag = 0;

void hal_power_handler(void* params)
{
    uint32_t count = 0;
    while (1)
    {
        hal_power_on();

        if (power_flag == 0)
        {
            if (hal_key_read_power_gpio() == 1)
            {
                if (++count > 2)
                {
                    power_flag = 1;
                    log_printf(HALPOWER_TAG, LOG_DEBUG, "进入1");
                }
            }
        }
        else if (power_flag == 1)
        {
            if (hal_key_read_power_gpio() == 0)
            {
                if (++count > 2)
                {
                    power_flag = 2;
                    log_printf(HALPOWER_TAG, LOG_DEBUG, "进入2");
                }
            }
        }
        else if (power_flag == 2)
        {
            if (hal_key_read_power_gpio() == 1)
            {
                log_printf(HALPOWER_TAG, LOG_DEBUG, "关机计数:%d", count);
                if (++count > 20)
                {
                    lv_hint_create("shotdown", 200, 20);

                    while (hal_key_read_power_gpio())
                    {
                        vTaskDelay(pdMS_TO_TICKS(10));
                    }

                    hal_ledc_set_duty(0);
                    system_save_config();
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    hal_power_off();

                    // wifi_lianjie(0, system_data.wifi_name, system_data.wifi_password, NULL);
                    vTaskDelay(pdMS_TO_TICKS(2000));
                    esp_restart();

                    while (1)
                    {
                        hal_power_off();
                        vTaskDelay(pdMS_TO_TICKS(100));
                    }
                }
            }
            else
            {
                count = 0;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelete(NULL);
}

#define SLEEP_TIME (1000LL * 1000LL * 3600LL * 24LL * 30LL) // 30天

void hal_power_sleep(void)
{
    esp_sleep_enable_ext1_wakeup((1ULL << IO_POWER_PIN), ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_sleep_enable_timer_wakeup(SLEEP_TIME);
    esp_deep_sleep_start();
}

void hal_power_init(void)
{
    log_printf(HALPOWER_TAG, LOG_DEBUG, "开始初始化");

    gpio_set_direction(IO_POWER_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(IO_LCD_BL_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(IO_LCD_BL_PIN, 0);

    gpio_set_pull_mode(IO_POWER_PIN, GPIO_PULLUP_ONLY);

    hal_power_on();

    if (hal_key_read_power_gpio() == 0)
    {
        power_flag = 2;
        log_printf(HALPOWER_TAG, LOG_DEBUG, "USB供电开机");
    }
    else
    {
        power_flag = 0;
        log_printf(HALPOWER_TAG, LOG_DEBUG, "按键电池开机");
    }

    hal_power_Semaphore = xSemaphoreCreateCounting(10, 0); // 动态创建一个计数型信号量
    xTaskCreate(hal_power_handler, "hal_power_handler", 1024 * 10, NULL, 4, NULL);

    log_printf(HALPOWER_TAG, LOG_DEBUG, "初始化结束");
}

void hal_power_off_task_start(void)
{
    // hal_power_Semaphore = xSemaphoreCreateCounting(10, 0); // 动态创建一个计数型信号量
    // xTaskCreate(hal_power_handler, "hal_power_handler", 1024 * 4, NULL, 4, NULL);
}