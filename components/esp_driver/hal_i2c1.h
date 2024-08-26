#ifndef __bsp_i2c_h__
#define __bsp_i2c_h__

#include "driver/i2c.h"
#include "bsp_board.h"
#include "main.h"

#define I2C_MASTER_NUM         I2C_NUM_0
#define I2C_MASTER_SCL_IO      GPIO_I2C1_SCL
#define I2C_MASTER_SDA_IO      GPIO_I2C1_SDA
#define I2C_MASTER_FREQ_HZ     100000
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define ACK_CHECK_EN           0x1
#define ACK_CHECK_DIS          0x0
#define ACK_VAL                0x0
#define NACK_VAL               0x1

#ifdef __cplusplus
extern "C" {
#endif

// 初始化 I2C 设备
void i2c1_init(void);

// I2C 写一个字节到指定设备地址
esp_err_t hal_i2c_master_write_byte(uint8_t device_addr, uint8_t data_addr, uint8_t data);
esp_err_t hal_i2c_master_write_byte_len(uint8_t device_addr, const uint8_t *reg, uint8_t *data, uint32_t len);

// I2C 从指定设备地址读取一个字节
esp_err_t hal_i2c_master_read_byte(uint8_t device_addr, uint8_t data_addr, uint8_t *data);
esp_err_t hal_i2c_master_read_byte_len(uint8_t device_addr, const uint8_t *reg, uint8_t *data, uint32_t len);

#ifdef __cplusplus
}
#endif


#endif
