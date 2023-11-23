#ifndef __HAL_MCP23S17_IO4_H__
#define __HAL_MCP23S17_IO4_H__

#include "main.h"

void MCP23S17_Init4(void);

uint8_t Discharge1_ON(uint8_t state);
uint8_t Discharge2_ON(uint8_t state);

//void IO4_Write(unsigned char address, unsigned char value);
//unsigned char IO4_Read(unsigned char address);
#endif //__HAL_SPI__H__
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
