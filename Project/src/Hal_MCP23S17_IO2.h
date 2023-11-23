#ifndef __HAL_MCP23S17_IO2_H__
#define __HAL_MCP23S17_IO2_H__

#include "main.h"

void MCP23S17_Init2(void);
//void IO2_Write(unsigned char address, unsigned char value);
//unsigned char IO2_Read(unsigned char address);
	
uint8_t VCC1_ON(uint8_t state);
uint8_t VCC2_ON(uint8_t state);
uint8_t VCC3_ON(uint8_t state);


uint8_t INT_ON(uint8_t state);
uint8_t WP_ON(uint8_t state);

uint8_t FuncRYControl(uint8_t no, uint8_t state, uint16_t time);

#endif //__HAL_SPI__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
