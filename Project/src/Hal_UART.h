#ifndef __HAL_UART__H__
#define __HAL_UART__H__

#include "main.h"

void Usart1_Initial(uint32_t baudrate);
void Usart2_Initial(uint32_t baudrate);
void Usart3_Initial(uint32_t baudrate);

void txpc(const char *fmt,...);
void Uart1_Receive_Check(void);

void Uart2_Printf(const char *fmt,...);
void Uart2_Receive_Check(void);

void dbgmsg(const char *fmt,...);
	
#endif //__HAL_UART__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

