#include "Hal_MCP23S17_IO4.h"

// PORTA0(OUT) 	: IO4_GPA_0(TBD)
// PORTA1(OUT) 	: IO4_GPA_1(TBD)
// PORTA2(OUT) 	: IO4_GPA_2(TBD)
// PORTA3(OUT) 	: IO4_GPA_3(TBD)
// PORTA4(OUT) 	: IO4_GPA_4(TBD)
// PORTA5(OUT) 	: IO4_GPA_5(TBD)
// PORTA6(OUT) 	: IO4_GPA_6(TBD)
// PORTA7(OUT) 	: IO4_GPA_7(TBD)

// PORTB0(OUT) 	: IO4_GPB_0(TBD)
// PORTB1(OUT) 	: IO4_GPB_1(TBD)
// PORTB2(OUT) 	: IO4_GPB_2(TBD)
// PORTB3(OUT) 	: IO4_GPB_3(TBD)
// PORTB4(OUT) 	: IO4_GPB_4(TBD)
// PORTB5(OUT) 	: IO4_GPB_5(TBD)
// PORTB6(OUT) 	: IO4_GPB_6(TBD)
// PORTB7(OUT) 	: IO4_GPB_7(TBD)

static void IO4_Write(unsigned char address, unsigned char value);
static unsigned char IO4_Read(unsigned char address);
	
void MCP23S17_Init4(void) 
{	
	IO4_Write(IOCON,  0x38); 
	IO4_Write(IOCON+1,0x38); 
    IO4_Write(IODIRA, 0x00);   // 0 -> output, 1-> input
    IO4_Write(IODIRB, 0x00);   // 0 -> output, 1-> input
    IO4_Write(OLATA,  0x00);   // porta all low
    IO4_Write(OLATB,  0x00);   // portb			
}

static void IO4_Write(unsigned char address, unsigned char value) 
{    
    IO_EXPAND_CS(0);    
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};    SPI_I2S_SendData(SPI2,IO4_ADDRESS_WRITE); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};    SPI_I2S_SendData(SPI2,address); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};    SPI_I2S_SendData(SPI2,value); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    SPI_I2S_ReceiveData(SPI2);     
    IO_EXPAND_CS(1);
}

static unsigned char IO4_Read(unsigned char address)       
{    
    uint8_t received_data = 0;
    
    IO_EXPAND_CS(0);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};   SPI_I2S_SendData(SPI2,IO4_ADDRESS_READ);  
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};   SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};   SPI_I2S_SendData(SPI2,address); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};   SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};   SPI_I2S_SendData(SPI2,0xFF);          //dummy	
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};   received_data = SPI_I2S_ReceiveData(SPI2);
    IO_EXPAND_CS(1);     
		
    return received_data;   
}

uint8_t Discharge1_ON(uint8_t state)
{
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 1;
	
	CurValue = IO4_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO4_Write(OLATB, RstValue);
			read_value = IO4_Read(GPIOBBB);						
			
			if((RstValue) == (read_value))	
			{
				RET = true;
				dbgmsg("Discharge1_ON = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 100)	
				{
					dbgmsg("Discharge1_ON = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
					break;
				}
			}
		}
	}
	else
	{
		//dbgmsg("LED2   = %02d(OK) %02X/%02X\n",state,(RstValue),(CurValue));
	}
	return RET;
}
uint8_t Discharge2_ON(uint8_t state)
{
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 2;
	
	CurValue = IO4_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO4_Write(OLATB, RstValue);
			read_value = IO4_Read(GPIOBBB);						
			
			if((RstValue) == (read_value))	
			{
				RET = true;
				dbgmsg("Discharge2_ON = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 100)	
				{
					dbgmsg("Discharge2_ON = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
					break;
				}
			}
		}
	}
	else
	{
		//dbgmsg("LED2   = %02d(OK) %02X/%02X\n",state,(RstValue),(CurValue));
	}
	return RET;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
