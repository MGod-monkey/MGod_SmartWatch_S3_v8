#include "hal_i2c1.h"
#include "driver/i2c.h"
#include "main.h"

static const char *HALI2C_TAG = "Hal I2C";

static esp_err_t i2c1_master_init(void) {
	i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(I2C_MASTER_NUM, &conf);

    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

void i2c1_init(void)
{
    if(i2c1_master_init()==ESP_OK)
    {
        log_printf(HALI2C_TAG, LOG_INFO, "i2c1 init success");
    }else
    {
        log_printf(HALI2C_TAG, LOG_ERROR, "i2c1 init failed");
    }

}

esp_err_t hal_i2c_master_write_byte(uint8_t device_addr, uint8_t data_addr, uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, data_addr, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) {
		log_printf(HALI2C_TAG, LOG_DEBUG, "I2C write byte failed: %s", esp_err_to_name(ret));
    }
    return ret;
}

esp_err_t hal_i2c_master_read_byte(uint8_t device_addr, uint8_t data_addr, uint8_t *data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, data_addr, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    if (ret != ESP_OK) {
		log_printf(HALI2C_TAG, LOG_DEBUG, "I2C write byte failed: %s", esp_err_to_name(ret));
        i2c_cmd_link_delete(cmd);
        return ret;
    }

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_READ, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, data, NACK_VAL);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) {
        log_printf(HALI2C_TAG, LOG_DEBUG, "I2C read byte failed: %s", esp_err_to_name(ret));
    }
    return ret;
}

esp_err_t hal_i2c_master_read_byte_len(uint8_t device_addr, const uint8_t *reg, uint8_t *data, uint32_t len) {
    if (len == 0) return ESP_OK;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    if (reg) {
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, *reg, true);
    }

    if (data) {
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_READ, true);
        if (len > 1) {
            i2c_master_read(cmd, data, len - 1, I2C_MASTER_ACK);
        }
        i2c_master_read_byte(cmd, data + len - 1, I2C_MASTER_NACK);
        i2c_master_stop(cmd);
    }

    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return err;
}

esp_err_t hal_i2c_master_write_byte_len(uint8_t device_addr, const uint8_t *reg, uint8_t *data, uint32_t len) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, device_addr << 1 | I2C_MASTER_WRITE, true);
    
    if (reg) {
        i2c_master_write_byte(cmd, *reg, true);
    }
    
    if (data) {
        i2c_master_write(cmd, data, len, true);
    }
    
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    
    return err;
}