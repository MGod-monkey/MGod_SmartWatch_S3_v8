#include "hal_sd.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"

static const char *HALSD_TAG = "Hal SD";

#define MOUNT_POINT "/sdcard"

sdmmc_card_t *hal_sdcard;


void hal_sd_test(void)
{


    const char *file_hello = MOUNT_POINT"/hello.txt";

    log_printf(HALSD_TAG, LOG_DEBUG, "打开文件 %s", file_hello);
    FILE *file = fopen(file_hello, "w");
    if (file == NULL)
    {
        log_printf(HALSD_TAG, LOG_DEBUG, "创建文件失败");
        return;
    }
    fprintf(file, "Helloword！\n");
    fclose(file);


    file = fopen(file_hello, "r");
    if (file == NULL)
    {
        log_printf(HALSD_TAG, LOG_DEBUG, "打开失败");
        return;
    }

    char line[64];
    fgets(line, sizeof(line), file);
    fclose(file);

    char *pos = strchr(line, '\n');
    if (pos)
    {
        *pos = '\0';
    }
    log_printf(HALSD_TAG, LOG_DEBUG, "读出内容: '%s'", line);


}



uint8_t hal_sd_init(void)
{
    esp_err_t ret;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
#ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .format_if_mount_failed = true,
#else
        .format_if_mount_failed = false,
#endif
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    const char mount_point[] = MOUNT_POINT;

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    slot_config.width = 4;
#ifdef SOC_SDMMC_USE_GPIO_MATRIX
    slot_config.clk = GPIO_NUM_46;
    slot_config.cmd = GPIO_NUM_3;
    slot_config.d0 = GPIO_NUM_9;
    slot_config.d1 = GPIO_NUM_10;
    slot_config.d2 = GPIO_NUM_18;
    slot_config.d3 = GPIO_NUM_8;
#endif


    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    log_printf(HALSD_TAG, LOG_DEBUG, "SDIO初始化成功");

    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &hal_sdcard);

    if (ret != ESP_OK)
    {

        log_printf(HALSD_TAG, LOG_DEBUG, "初始化失败");

        return 1;
    }
    log_printf(HALSD_TAG, LOG_DEBUG, "文件系统挂在成功");

    sdmmc_card_print_info(stdout, hal_sdcard);

    hal_sd_test();
    return 0;

}
