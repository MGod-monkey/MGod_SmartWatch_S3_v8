#ifndef __hal_key_h__
#define __hal_key_h__

#include "stdint.h"
#include "stdio.h"

#define KEY_POWER_PIN       GPIO_NUM_1

#define KEY_POWER_STA     gpio_get_level(KEY_POWER_PIN)

void hal_key_init(void);
uint8_t hal_key_read_power_gpio(void);
#endif
