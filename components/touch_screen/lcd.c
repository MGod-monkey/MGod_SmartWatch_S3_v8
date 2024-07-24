
#include "lcd.h"

/*!< SPI device handle of LCD, used for SPI transmition */
static spi_device_handle_t lcd_dev_handle = NULL;

/*!< LCD initialize command(s) */
static DRAM_ATTR const lcd_init_cmd_t st7789_init_cmd[] = {
        {0x11, 0, {0x00}},

	#if USE_HORIZONTAL==0
		{0x36, 1, {0x00}},
	#elif USE_HORIZONTAL==1
		{0x36, 1, {0xC0}},
	#elif USE_HORIZONTAL==2
		{0x36, 1, {0x70}},
	#else
        {0x36, 1, {0xA0}},
    #endif

        {0x3A, 1, {0x55}},
        {0xB2, 7, {0x0c, 0x0c, 0x00, 0x33, 0x33, 0xb7, 0x75}},
        {0xC2, 1, {0x01}},
        {0xC3, 1, {0x16}},
        {0xC4, 1, {0x20}},
        {0xC6, 1, {0x0F}},
        {0xD0, 2, {0xA4, 0xA1}},
        {0xD6, 1, {0xA1}},
        {0xBB, 1, {0x3B}},
        {0xE0, 4, {0xf0, 0x0b, 0x11, 0x0e, 0x0d, 0x19, 0x36, 0x33, 0x4b, 0x07, 0x14, 0x14, 0x2c, 0x2e}},
        {0xE1, 4, {0xf0, 0x0d, 0x12, 0x0b, 0x09, 0x03, 0x32, 0x44, 0x48, 0x39, 0x16, 0x16, 0x2d, 0x30}},
        {0x2A, 4, {0x00, 0x00, 0x00, 239}}, //宽度
        {0x2B, 4, {0x00, 000, 0x00, 239}},  //高度
        {0x21, 0, {0x00}},
        {0x29, 0, {0x00}},
        {0x2C, 0, {0x00}},



};





static esp_err_t lcd_write_byte(uint8_t data)
{
    spi_transaction_t spi_transaction;

    memset(&spi_transaction, 0, sizeof(spi_transaction));

    spi_transaction.length = 8;
    spi_transaction.tx_buffer = &data;

    return spi_device_polling_transmit(lcd_dev_handle, &spi_transaction);
}

static esp_err_t lcd_write_bytes(const uint8_t *data, size_t data_len)
{
    spi_transaction_t spi_transaction;

    memset(&spi_transaction, 0, sizeof(spi_transaction));

    spi_transaction.length = data_len * 8;
    spi_transaction.tx_buffer = data;

    return spi_device_polling_transmit(lcd_dev_handle, &spi_transaction);
}

static esp_err_t lcd_write_cmd_byte(uint8_t cmd)
{
    gpio_set_level(GPIO_LCD_DC, 0);
    return lcd_write_byte(cmd);
}

static esp_err_t lcd_write_data_bytes(const uint8_t *data, size_t data_len)
{
    if (0 == data_len) {
        return ESP_OK;
    }

    gpio_set_level(GPIO_LCD_DC, 1);
    return lcd_write_bytes(data, data_len);
}

static void lcd_reset(void)
{
    gpio_set_level(GPIO_LCD_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(2));
    gpio_set_level(GPIO_LCD_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(2));
}

esp_err_t lcd_init(spi_host_device_t spi_host_device, int clock_speed)
{   bsp_ledc_init();
    // 管脚设置
    spi_bus_config_t spi_bus_cfg = {
		.miso_io_num = GPIO_LCD_DOUT,
		.mosi_io_num = GPIO_LCD_DIN,
		.sclk_io_num = GPIO_LCD_CLK,
		.quadwp_io_num = GPIO_NUM_NC,
		.quadhd_io_num = GPIO_NUM_NC,
		.max_transfer_sz = 240 * 280 * 2 + 8
    };
// 设备初始化
    spi_device_interface_config_t spi_dev_cfg = {
		.clock_speed_hz = clock_speed,
		.mode = 0,
		.spics_io_num = GPIO_LCD_CS,
		.queue_size = 7,
		.pre_cb = NULL,
	};

    /*!< Initialize SPI bus and add LCD to SPI bus */
    // 初始化spi和lcd总线
	spi_bus_initialize(spi_host_device, &spi_bus_cfg, DMA_CHAN);
    spi_bus_add_device(spi_host_device, &spi_dev_cfg, &lcd_dev_handle);

    /*!< Initialize LCD control IOs */
    // 初始化lcd管脚
    gpio_set_direction(GPIO_LCD_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_LCD_RST, GPIO_MODE_OUTPUT);

    // gpio_set_pull_mode(GPIO_LCD_BL,GPIO_PULLUP_ONLY);
    // gpio_set_direction(GPIO_LCD_BL, GPIO_MODE_OUTPUT);
// reset
    /*!< Reset LCD by hardware RST pin */
    lcd_reset();
// 初始化命令
    /*!< Send initialize command(s) */
    for (size_t i = 0; i < sizeof(st7789_init_cmd) / sizeof(st7789_init_cmd[0]); i++) {
        lcd_write_cmd_byte(st7789_init_cmd[i].cmd);
        lcd_write_data_bytes(st7789_init_cmd[i].data, st7789_init_cmd[i].data_len);
    }

    /*!< Turn on backlight after initialized */
   // gpio_set_level(GPIO_LCD_BL, 0);

    return ESP_OK;
}

static esp_err_t lcd_set_region(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    /*!< (320 - 240) / 2 = 0x50 */
    // x1 += 0x50;
	// x2 += 0x50;
    y1 += 20;
	y2 += 20;

    /* Endian convert */
    uint8_t x_arr[4] = {
        x1 >> 8, x1,
        x2 >> 8, x2,
    };

    uint8_t y_arr[4] = {
        y1 >> 8, y1,
        y2 >> 8, y2,
    };

    /*!< Set X coordinates */
	lcd_write_cmd_byte(0x2A);
	lcd_write_data_bytes(x_arr, sizeof(x_arr));

    /*!< Set Y coordinates */
	lcd_write_cmd_byte(0x2B);
    lcd_write_data_bytes(y_arr, sizeof(y_arr));

    /* Data start command, followed by data */
	lcd_write_cmd_byte(0x2C);

    return ESP_OK;
}

esp_err_t lcd_flush_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t *data)
{
    size_t data_size = (x2 - x1 + 1) * (y2 - y1 + 1) * sizeof(uint16_t);

    lcd_set_region(x1, y1, x2, y2);

	lcd_write_data_bytes(data, data_size);

    return ESP_OK;
}
#include "esp_log.h"
#include "esp_err.h"
#include <esp_log.h>


#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "main.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (45) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_8_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (255/10) // 最大255
#define LEDC_FREQUENCY          (5000) // Frequency in Hertz. Set frequency at 5 kHz


void bsp_ledc_set_duty(uint8_t dat)
{

    dat = dat*2.5;

    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, dat));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}

void bsp_ledc_init(void)
{
    // Set the LEDC peripheral configuration
   // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    // Set duty to 50%
    bsp_ledc_set_duty(5);
    // Update duty to apply the new value
    printf("led init\n");

}





