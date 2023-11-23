#ifndef _TASK_FUNC__H__
#define _TASK_FUNC__H__

#include "main.h"

//*************************
// Use Function Define
// 
//*************************



//#define LSI_IC
//#define STM_IC		
//#define SYNA_IC	
//#define ATMEL_IC	
//#define SiW_IC		
#define MFF_ISC
//#define SPIFLASH_IC
#define LXS_IC

uint8_t Func_test(uint8_t num, uint8_t st, uint8_t result);
uint8_t Func_Insp_Step(void);
uint8_t Func_Insp_Auto(void);
uint8_t Func_Test(uint16_t cnt, uint16_t type);

#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

