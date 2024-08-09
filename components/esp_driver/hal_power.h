#ifndef __hal_power_h__
#define __hal_power_h__

#include "stdint.h"
#include "stdio.h"

#define IO_POWER_PIN         GPIO_NUM_2
#define IO_LCD_BL_PIN       GPIO_NUM_45

void hal_power_init(void);
void hal_power_on(void);
void hal_power_off(void);
void hal_power_send_off(void);
void hal_power_off_task_start(void);
void hal_power_sleep(void);

#endif
