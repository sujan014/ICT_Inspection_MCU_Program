#ifndef _SYS_UTIL__H__
#define _SYS_UTIL__H__

#include "main.h"

#define ID_UNIQUE_ADDRESS	0x1FFF7A10
#define CRC_INITIALVALUE 	0xFFFFFFFF

extern char current_page;
extern char pc_rx_enable;

extern unsigned long crc32_table[256]; 
extern unsigned long serial_num;
extern RTC_TimeTypeDef RTC_TimeStr; 
extern RTC_DateTypeDef RTC_DateStr; 

extern __IO uint64_t LocalTime;
extern __IO uint64_t timingdelay;

extern __IO uint64_t LocalTime_t;
extern __IO uint64_t timingdelay_t;

typedef  void (*pFunction)(void);
extern pFunction Jump_To_Application;

#define SYSTEMTICK_PERIOD_MS  10
#define SYSTEMTICK_PERIOD_US  1

unsigned long set_crc32_init_value(unsigned long start);
void BoardSerialRead(void);

void Time_Update(void);
void Delay_us(uint32_t nCount);
void Delay_ms(uint32_t nCount);

void Time_Update_Timer(void);
void ntDelay_us(uint32_t nCount);
void ntDelay_ms(uint32_t nCount);
void EscWait(void);
uint8_t LongKeySt1(void);
uint8_t SafeSensorCheck(void);

void NVIC_Initial(void);

void CustomStrCat(char* pBuf, const char* fmt, ...);
#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

