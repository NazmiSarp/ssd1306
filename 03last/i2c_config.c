#include "i2c_config.h"
#include "at32f413.h"
#include "i2c_application.h"
#include "at32f413_conf.h"
#define I2C_TIMEOUT                      0xFFFFFFFF

#define I2Cx_SPEED                       100000
#define I2Cx_ADDRESS                     0x30

#define I2Cx_PORT                        I2C2
#define I2Cx_CLK                         CRM_I2C2_PERIPH_CLOCK

#define I2Cx_SCL_PIN                     GPIO_PINS_10
#define I2Cx_SCL_GPIO_PORT               GPIOB
#define I2Cx_SCL_GPIO_CLK                CRM_GPIOB_PERIPH_CLOCK

#define I2Cx_SDA_PIN                     GPIO_PINS_11
#define I2Cx_SDA_GPIO_PORT               GPIOB
#define I2Cx_SDA_GPIO_CLK                CRM_GPIOB_PERIPH_CLOCK

#define BUF_SIZE                         8


uint8_t tx_buf[BUF_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uint8_t rx_buf[BUF_SIZE] = {0};

i2c_handle_type hi2cx;

//void error_handler(uint32_t error_code);
uint32_t buffer_compare(uint8_t* buffer1, uint8_t* buffer2, uint32_t len);
void i2c_lowlevel_init(i2c_handle_type* hi2c);


uint32_t buffer_compare(uint8_t* buffer1, uint8_t* buffer2, uint32_t len)
{
  uint32_t i;

  for(i = 0; i < len; i++)
  {
    if(buffer1[i] != buffer2[i])
    {
      return 1;
    }
  }

  return 0;
}
    

void i2c_config(i2c_handle_type *hi2c)
{
    // GPIO pinlerini I2C için yapilandir
    wk_i2c_gpio_init();

    // I2C modülünü baslat
    wk_i2c_init();

    // I2C handle yapisini yapilandir
    hi2c->i2cx = I2C2;  // Örnegin, I2C2 kullaniliyor
    hi2c->timeout = 10000;  // Timeout degeri (örnek amaçli)
    hi2c->mode = I2C_INT_MA_TX;  // Iletisim modu
    // Daha fazla yapilandirma yapilabilir (DMA kanallari, vb.)
}

void wk_i2c_gpio_init(void)
{
    gpio_init_type gpio_init_struct;

    // SCL pinini yapilandir
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init_struct.gpio_pins = GPIO_PINS_10;
    gpio_init(GPIOB, &gpio_init_struct);

    // SDA pinini yapilandir
    gpio_init_struct.gpio_pins = GPIO_PINS_11;
    gpio_init(GPIOB, &gpio_init_struct);
}

void wk_i2c_init(void)
{
    // I2C modülünü baslat
    i2c_init(I2C2, I2C_FSMODE_DUTY_2_1, 400000);
    i2c_own_address1_set(I2C2, I2C_ADDRESS_MODE_7BIT, 0x0);
    i2c_ack_enable(I2C2, TRUE);
    i2c_clock_stretch_enable(I2C2, TRUE);
    i2c_general_call_enable(I2C2, FALSE);
    i2c_enable(I2C2, TRUE);
}
