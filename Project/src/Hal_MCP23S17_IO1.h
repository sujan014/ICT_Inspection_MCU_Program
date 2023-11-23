#ifndef __HAL_MCP23S17_IO1_H__
#define __HAL_MCP23S17_IO1_H__

#include "main.h"

void MCP23S17_Init1(void);
//void IO1_Write(unsigned char address, unsigned char value);
//unsigned char IO1_Read(unsigned char address);
	
#define ATS_ON		0xFF
#define ST1_ON		0x01
#define ST2_ON   	0x02 
#define MOD_ON   	0x04 
#define ESC_ON   	0x08 // key State

#define ST1_LON		0x11
#define ST2_LON   	0x12 
#define MOD_LON   	0x14 
#define ESC_LON   	0x18 // long key State

uint8_t SwitchCheck(void);
uint8_t OKLED(uint8_t state);
uint8_t NGLED(uint8_t state);
uint8_t BUZZER(uint8_t state);
uint8_t EVCC_RY(uint8_t state);
void Beep(int time);
uint8_t CylinderControl(uint8_t no, uint8_t state, uint16_t time);
uint8_t KeyScan(void);
uint8_t KeyRead(void);

uint8_t mfs_test(void);

#endif //__HAL_SPI__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
