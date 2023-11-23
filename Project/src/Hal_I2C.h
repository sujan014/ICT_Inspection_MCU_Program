#ifndef __HAL_I2C__H__
#define __HAL_I2C__H__

#include "main.h"

void I2C1_Init(void);
void 	i2c1_start(void);
void 	i2c1_stop(void);
uint8_t i2c1_tx(uint8_t data);
uint8_t i2c1_rx(uint8_t ack);

void I2C2_Init(void);
void i2c2_start(void);
void i2c2_stop(void);
uint8_t i2c2_tx(uint8_t data);
uint8_t i2c2_rx(uint8_t ack);


//-------------------melfas
uint8_t melfas_i2c1_tx(unsigned char d);
uint8_t melfas_i2c1_rx(char ack);
//-------------------

void 	LED_I2C_Init(void);
void 	led_i2c_start(void);
void 	led_i2c_stop(void);
uint8_t led_i2c_tx(uint8_t data);
uint8_t led_i2c_rx(uint8_t ack);
uint8_t olb_led_control(uint8_t ch, uint8_t mes);

void touch_i2c_sda(uint8_t i);
void touch_i2c_scl(uint8_t i);
void touch_i2c_start(void);
void touch_i2c_stop(void);
uint8_t touch_i2c_tx(unsigned char d);
uint8_t touch_i2c_rx(char ack);

void switch_Hold(void);

#endif //__HAL_I2C__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

