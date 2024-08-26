#include "hal_key.h"

#include "driver/gpio.h"

uint8_t hal_key_read_power_gpio(void)
{
	return KEY_POWER_STA;
}


void hal_key_init(void)
{

	gpio_set_direction(KEY_POWER_PIN, GPIO_MODE_INPUT);
	gpio_set_pull_mode(KEY_POWER_PIN,GPIO_FLOATING);

}
