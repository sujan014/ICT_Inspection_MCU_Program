#ifndef _LX_SEMICON_H_
#define _LX_SEMICON_H_

#include "main.h"


uint8_t LXS_Module(uint16_t index);

int LXS_Initialize(uint16_t index);
int LXS_FirmwareWrite(uint16_t index);
int LXS_FirmwareVerRead(uint16_t index);
int LXS_PID_Check(uint16_t index);
int LXS_VID_Check(uint16_t index);
int LXS_Boot_Ver_Check(uint16_t index);
int LXS_Core_Ver_Check(uint16_t index);
int LXS_App_Ver_Check(uint16_t index);
int LXS_Para_Ver_Check(uint16_t index);
int LXS_CRC_Check(uint16_t index);
int LXS_End_Flow(uint16_t index);
	

int I2C_Read(uint16_t address, uint8_t *read_buf, unsigned int read_len);
int I2C_Write(uint16_t address, uint8_t *write_buf, unsigned int write_len);

uint8_t CheckDFUP(void);
uint8_t WaitForReadyStatus(void);
uint8_t slaveAddress_Change(void);
int PostDownload(void);
int DownloadProgram(unsigned char bBoostMode);	//bool : True, False 
uint8_t CheckBoostMode(void);

#endif
