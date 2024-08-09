#include "main.h"
#include "hal_i2s.h"

#define I2S_MCK_IO      (GPIO_NUM_6)
#define I2S_BCK_IO      (GPIO_NUM_16)
#define I2S_WS_IO       (GPIO_NUM_17)
#define I2S_DO_IO       (GPIO_NUM_7)
#define I2S_DI_IO       (GPIO_NUM_15)

esp_err_t hal_i2s_init(void)
{
    esp_err_t ret=0;
    i2s_config_t i2s_cfg = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX,
        .sample_rate = 32000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .tx_desc_auto_clear = true,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .mclk_multiple = I2S_MCLK_MULTIPLE_128,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    };

    i2s_driver_install(I2S_NUM, &i2s_cfg, 0, NULL);
    i2s_pin_config_t i2s_pin_cfg = {
        .mck_io_num = I2S_MCK_IO,
        .bck_io_num = I2S_BCK_IO,
        .ws_io_num = I2S_WS_IO,
        .data_out_num = I2S_DO_IO,
        .data_in_num = I2S_DI_IO
    };
    i2s_set_pin(I2S_NUM, &i2s_pin_cfg);
	//hal_i2s_test();
    return ESP_OK;
}
