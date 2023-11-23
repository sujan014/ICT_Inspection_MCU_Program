#ifndef __HAL_SDRAM_H
#define __HAL_SDRAM_H

#ifdef __cplusplus
 extern "C" {
#endif
     
#include "main.h"

#define SDRAM_BANK_ADDR     ((uint32_t)0xD0000000)       
#define SDRAM_MEMORY_WIDTH  FMC_SDMemory_Width_16b  
#define SDRAM_CAS_LATENCY   FMC_CAS_Latency_3     
#define SDCLOCK_PERIOD      FMC_SDClock_Period_2      // Default configuration used with LCD     
#define SDRAM_READBURST     FMC_Read_Burst_Disable    // Default configuration used with LCD

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)      

void  Sdram_Initial(void);
void  SDRAM_GPIOConfig(void);
void  SDRAM_InitSequence(void);
void SDRAM_SetRefresh(void);
void  SDRAM_WriteBuffer(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize);
void  SDRAM_ReadBuffer(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize);

#ifdef __cplusplus
}
#endif

#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
