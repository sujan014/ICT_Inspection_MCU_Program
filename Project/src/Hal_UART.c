#include "Hal_Uart.h"

void Usart1_Initial(uint32_t baudrate)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//////////////////////////	
	USART_InitTypeDef USART_InitStructure;		
    USART_InitStructure.USART_BaudRate      = baudrate;	
    USART_InitStructure.USART_WordLength    = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits      = USART_StopBits_1;
    USART_InitStructure.USART_Parity        = USART_Parity_No;
    USART_InitStructure.USART_Mode          = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//////////////////////////	
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	//////////////////////////	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1); 
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);//1 
	//////////////////////////	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9|GPIO_Pin_10;     
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    //////////////////////////	
	USART_Init(USART1, &USART_InitStructure);    
	USART_Cmd(USART1, ENABLE);	// Enable USART
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	// Enable Receive Interrupt	
};	
void Usart2_Initial(uint32_t baudrate)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
    USART_InitTypeDef USART_InitStructure;		
    USART_InitStructure.USART_BaudRate      = baudrate;	
    USART_InitStructure.USART_WordLength    = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits      = USART_StopBits_1;
    USART_InitStructure.USART_Parity        = USART_Parity_No;
    USART_InitStructure.USART_Mode          = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    //////////////////////////	
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    //////////////////////////	
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_USART2); 
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,  GPIO_AF_USART2);//2  
    //////////////////////////	
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3;     
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    //////////////////////////	
	USART_Init(USART2, &USART_InitStructure);    
	USART_Cmd(USART2, ENABLE);	// Enable USART
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	// Enable Receive Interrupt
};	
void Usart3_Initial(uint32_t baudrate)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,  ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    //////////////////////////	
    USART_InitTypeDef USART_InitStructure;	
    USART_InitStructure.USART_BaudRate      = baudrate;	
    USART_InitStructure.USART_WordLength    = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits      = USART_StopBits_1;
    USART_InitStructure.USART_Parity        = USART_Parity_No;
    USART_InitStructure.USART_Mode          = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    //////////////////////////	
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    //////////////////////////	
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3); 
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);//3
    //////////////////////////	
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_11;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    //////////////////////////	
	USART_Init(USART3, &USART_InitStructure);    
	USART_Cmd(USART3, ENABLE);	// Enable USART
	//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	// Enable Receive Interrupt
};	
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Uart1_PutChar(uint16_t data)
{
    USART_SendData(USART1,data);
    while( USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

void Uart1_PutStr(char *string)
{
    while(*string != '\0') Uart1_PutChar(*(string++));
}

void txpc(const char *fmt,...)
{
    va_list ap;
    va_start(ap,fmt);    vsprintf(uart_str1,fmt,ap);    va_end(ap);
    Uart1_PutStr(uart_str1);
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Uart2_PutChar(char data)
{
    USART_SendData(USART2,data);
    while( USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

void Uart2_PutStr(char *string)
{
    while(*string != '\0') Uart2_PutChar(*(string++));
}

void Uart2_Printf(const char *fmt,...)
{
    va_list ap;
    va_start(ap,fmt);    vsprintf(uart_str2,fmt,ap);    va_end(ap);
    Uart2_PutStr(uart_str2);
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Uart3_PutChar(char data)
{
    USART_SendData(USART3,data);
    while( USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
}

void Uart3_PutStr(char *string)
{
    while(*string != '\0') Uart3_PutChar(*(string++));
}

void dbgmsg(const char *fmt,...)
{
	if(RD_DBG_MD == 1) return;
		
    va_list ap;
    va_start(ap,fmt);   
	vsprintf(uart_str3,fmt,ap);    
	va_end(ap);
    Uart3_PutStr(uart_str3);
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Uart1_Receive_Check(void)	
{ 
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		//txpc(" UART1 Receive data !\n");
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
	}
}

void Uart2_Receive_Check(void)	
{ 
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		//Uart2_Printf(" UART2 Receive data !\n");
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);	
	}
}

/*
void Uart3_Receive_Check(void)
{ 
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		//Uart3_Printf(" UART3 Receive data !\n");
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);	
	}
}
*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
