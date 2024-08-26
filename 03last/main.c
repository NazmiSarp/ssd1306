#include "at32f413.h"                 
#include "at32f413_conf.h"             
#include "i2c_applicaiton.h"
#include "i2c_config.h"
#include "ssd1306.h"
#include "ssd1306_conf_template.h"
#define SSD1306_I2C_ADDR (0x3C)

//struct i2c_handle_type i2c_handle; // I2C handle yapisi

void wk_system_clock_config(void);
void wk_periph_clock_config(void);
void wk_i2c2_init(void);
void ssd1306_WriteCommand(uint8_t byte);

int main(void)
{    
    wk_system_clock_config();
    wk_periph_clock_config();
    wk_i2c2_init();

    while(1)
    {
    }
}
void i2c_lowlevel_init(i2c_handle_type* hi2c) {
    gpio_init_type gpio_init_struct;
    gpio_default_para_init(&gpio_init_struct);

    // SCL pini yapilandirmasi
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init_struct.gpio_pins = GPIO_PINS_10;
    gpio_init(GPIOB, &gpio_init_struct);

    // SDA pini yapilandirmasi
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init_struct.gpio_pins = GPIO_PINS_11;
    gpio_init(GPIOB, &gpio_init_struct);

    // I2C yapilandirmasi
    i2c_init(I2C2, I2C_FSMODE_DUTY_2_1, 400000); // 400kHz I2C hizi
    i2c_own_address1_set(I2C2, I2C_ADDRESS_MODE_7BIT, 0x0);
    i2c_ack_enable(I2C2, TRUE);
    i2c_clock_stretch_enable(I2C2, TRUE);
    i2c_general_call_enable(I2C2, FALSE);

    i2c_enable(I2C2, TRUE);
}


void wk_periph_clock_config(void)
{
    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_I2C2_PERIPH_CLOCK, TRUE);
}

void wk_system_clock_config(void)
{
    crm_reset();
    crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);

    while(crm_flag_get(CRM_LICK_STABLE_FLAG) != SET)
    {
    }

    crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);

    while(crm_hext_stable_wait() == ERROR)
    {
    }

    crm_clock_source_enable(CRM_CLOCK_SOURCE_HICK, TRUE);

    while(crm_flag_get(CRM_HICK_STABLE_FLAG) != SET)
    {
    }

    crm_pll_config(CRM_PLL_SOURCE_HICK, CRM_PLL_MULT_50, CRM_PLL_OUTPUT_RANGE_GT72MHZ);
    crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

    while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
    {
    }

    crm_ahb_div_set(CRM_AHB_DIV_1);
    crm_apb2_div_set(CRM_APB2_DIV_2);
    crm_apb1_div_set(CRM_APB1_DIV_2);

    crm_auto_step_mode_enable(TRUE);
    crm_sysclk_switch(CRM_SCLK_PLL);

    while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
    {
    }

    crm_auto_step_mode_enable(FALSE);
    system_core_clock_update();
}
void ssd1306_WriteCommand(uint8_t byte) {
    i2c_start(I2C2);
    i2c_7bit_address_send(I2C2, SSD1306_I2C_ADDR, I2C_DIRECTION_TRANSMIT);
    i2c_data_send(I2C2, 0x30); 
    i2c_data_send(I2C2, byte);  
    i2c_stop(I2C2);
}


