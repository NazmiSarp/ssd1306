#ifndef __I2C_CONFIG_H
#define __I2C_CONFIG_H

#include "at32f413.h"
#include "i2c_application.h"

void i2c_config(i2c_handle_type *hi2c);
void wk_i2c_gpio_init(void);
void wk_i2c_init(void);

#endif
