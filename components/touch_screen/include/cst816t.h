#ifndef __cst816t__h
#define __cst816t__h

#include "driver/i2c.h"

#define CST816T_ADDR 0X15

uint8_t cst816t_init(void);
esp_err_t cst816t_read_pos(uint8_t *touch_points_num, uint16_t *x, uint16_t *y);
#endif
