#ifndef __HAL_MCP23S17_IO3_H__
#define __HAL_MCP23S17_IO3_H__

#include "main.h"

void MCP23S17_Init3(void);

uint8_t RYBD_1ON(uint8_t state);
uint8_t RYBD_2ON(uint8_t state);
uint8_t RYBD_3ON(uint8_t state);
uint8_t RYBD_4ON(uint8_t state);
uint8_t RYBD_5ON(uint8_t state);
uint8_t RYBD_6ON(uint8_t state);

//uint8_t VTSP_OFF(uint8_t state);
//uint8_t VUSB_OFF(uint8_t state);
uint8_t AVDD_ON(uint8_t state);
uint8_t DVDD_ON(uint8_t state);


uint8_t Extend_VDD_RY(uint8_t state);
uint8_t Extend_VDD(uint8_t state);

//void IO3_Write(unsigned char address, unsigned char value);
//unsigned char IO3_Read(unsigned char address);
uint8_t EX_PORT(uint8_t no, uint8_t state);

uint8_t Sen_Scan(void);
uint8_t CylinderSensorCheck(uint8_t no, uint8_t state, uint16_t time);

#endif //__HAL_SPI__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
