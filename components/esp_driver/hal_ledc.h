#ifndef __hal_ledc_h__
#define __hal_ledc_h__

#include "stdint.h"
#include "stdio.h"

void hal_ledc_init(void);
void hal_ledc_set_duty(uint8_t dat);

#endif
