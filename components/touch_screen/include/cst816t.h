#ifndef __cst816t__h
#define __cst816t__h

#include "driver/i2c.h"

#define CST816T_ADDR 0X15

#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

uint8_t cst816t_init(void);
esp_err_t cst816t_read_pos(uint8_t *touch_points_num, uint16_t *x, uint16_t *y);
#endif
