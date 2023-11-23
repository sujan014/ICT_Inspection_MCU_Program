#include "Hal_MCP23S17_IO2.h"

// PORTA0(OUT) 	: FUNC RELAY1
// PORTA1(OUT) 	: FUNC RELAY2
// PORTA2(OUT) 	: FUNC RELAY3
// PORTA3(OUT) 	: FUNC RELAY4
// PORTA4(OUT) 	: FUNC RELAY5 	
// PORTA5(OUT) 	: FUNC RELAY6 	
// PORTA6(OUT) 	: FUNC RELAY7	
// PORTA7(OUT) 	: FUNC RELAY8 	

// PORTB0(OUT) 	: VCC1_ON
// PORTB1(OUT) 	: VCC2_ON
// PORTB2(OUT) 	: VCC3_ON
// PORTB3(OUT) 	: X
// PORTB4(OUT) 	: IO2_GPB_4(TBD)
// PORTB5(OUT) 	: IO2_GPB_5(TBD)
// PORTB6(OUT) 	: IO2_GPB_6(TBD)
// PORTB7(OUT) 	: IO2_GPB_7(TBD)

static void IO2_Write(unsigned char address, unsigned char value);
static unsigned char IO2_Read(unsigned char address);

void MCP23S17_Init2(void) 
{
	IO2_Write(IOCON,  0x38); 
	IO2_Write(IOCON+1,0x38); 
    IO2_Write(IODIRA, 0x00);   // 0 -> output, 1-> input
    IO2_Write(IODIRB, 0x00);   // 0 -> output, 1-> input
    IO2_Write(OLATA,  0x00);   // porta all low
    //IO2_Write(OLATB,  0x00);   // portb	
	IO2_Write(OLATB,  0x04);   // portb			
	
	VCC1_ON(true);
	VCC2_ON(true);
	VCC3_ON(true); //??
	FuncRYControl(255, false, 0);
}

static void IO2_Write(unsigned char address, unsigned char value) 
{    
    IO_EXPAND_CS(0);    
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};    SPI_I2S_SendData(SPI2,IO2_ADDRESS_WRITE); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};    SPI_I2S_SendData(SPI2,address); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};    SPI_I2S_SendData(SPI2,value); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    SPI_I2S_ReceiveData(SPI2);     
    IO_EXPAND_CS(1);
}

static unsigned char IO2_Read(unsigned char address)       
{    
    uint8_t received_data = 0;
    
    IO_EXPAND_CS(0);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};   SPI_I2S_SendData(SPI2,IO2_ADDRESS_READ);  
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};   SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};   SPI_I2S_SendData(SPI2,address); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};   SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};   SPI_I2S_SendData(SPI2,0xFF);          //dummy	
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};   received_data = SPI_I2S_ReceiveData(SPI2);
    IO_EXPAND_CS(1);

    return received_data;   
}

//active high
uint8_t VCC1_ON(uint8_t state)
{
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 1;
	
	CurValue = IO2_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO2_Write(OLATB, RstValue);
			read_value = IO2_Read(GPIOBBB);
						
			if((RstValue) == (read_value))	
			{
				RET = true;
				//dbgmsg("EVCC1   = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 3)	
				{
					dbgmsg("EVCC1   = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
					break;
				}
			}
		}
	}
	else
	{
		//dbgmsg("EVCC1   = %02d(OK) %02X/%02X\n",state,(RstValue),(CurValue));
	}

	return RET;
}

//active high
uint8_t VCC2_ON(uint8_t state)
{	
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 2;
	
	CurValue = IO2_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO2_Write(OLATB, RstValue);
			read_value = IO2_Read(GPIOBBB);
						
			if((RstValue) == (read_value))	
			{
				RET = true;
				//dbgmsg("EVCC2   = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 3)	
				{
					dbgmsg("EVCC2   = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
					break;
				}
			}
		}
	}
	else
	{
		//dbgmsg("EVCC2   = %02d(OK) %02X/%02X\n",state,(RstValue),(CurValue));
	}

	return RET;
}

//active low
uint8_t VCC3_ON(uint8_t state)
{	
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 3;
	
	CurValue = IO2_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO2_Write(OLATB, RstValue);
			read_value = IO2_Read(GPIOBBB);  
						
			if((RstValue) == (read_value))	
			{
				RET = true;
				//dbgmsg("EVCC3   = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 3)	
				{
					dbgmsg("EVCC3   = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
					break;
				}
			}
		}
	}
	else
	{
		//dbgmsg("EVCC3   = %02d(OK) %02X/%02X\n",state,(RstValue),(CurValue));
	}

	return RET;
}


//-----------------------------------------------------------------------------
uint8_t INT_ON(uint8_t state)
{	
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 5;
	
	CurValue = IO2_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO2_Write(OLATB, RstValue);
			read_value = IO2_Read(GPIOBBB);  
						
			if((RstValue) == (read_value))	
			{
				RET = true;
				//dbgmsg("EVCC3   = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 3)	
				{
					dbgmsg("INT_ON   = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
					break;
				}
			}
		}
	}
	else
	{
		//dbgmsg("EVCC3   = %02d(OK) %02X/%02X\n",state,(RstValue),(CurValue));
	}

	return RET;
}

uint8_t WP_ON(uint8_t state)
{	
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 6;
	
	CurValue = IO2_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO2_Write(OLATB, RstValue);
			read_value = IO2_Read(GPIOBBB);  
						
			if((RstValue) == (read_value))	
			{
				RET = true;
				//dbgmsg("EVCC3   = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 3)	
				{
					dbgmsg("WP_ON   = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
					break;
				}
			}
		}
	}
	else
	{
		//dbgmsg("EVCC3   = %02d(OK) %02X/%02X\n",state,(RstValue),(CurValue));
	}

	return RET;
}
//---------------------------------------------------------------------------------------------------
uint8_t FuncRYControl(uint8_t no, uint8_t state, uint16_t time)
{		
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	if( no == 255 )
	{		
		if(state == true)		
		{
			nRetry = 0;
			while(1)
			{
				IO2_Write(OLATA, 0xff);
				read_value = IO2_Read(GPIOAAA);  				
				if(0xff == read_value)	
				{
					RET = true;
					//dbgmsg("FN RY ALL = %02d, %04dms(OK)\n",state, time);
					break;
				}
				else
				{
					RET = false;
					if(nRetry++ > 3)	
					{
						dbgmsg("FN RY ALL = %02d, %04dms(NG)\n",state, time);
						break;
					}
				}
			}
		}
		else if(state == false)	
		{				
			nRetry = 0;
			while(1)
			{
				IO2_Write(OLATA, 0x00);
				read_value = IO2_Read(GPIOAAA);  
				
				if(0x00 == read_value)	
				{
					RET = true;
					//dbgmsg("FN RY ALL = %02d, %04dms(OK)\n",state, time);
					break;
				}
				else
				{
					RET = false;
					if(nRetry++ > 3)	
					{
						dbgmsg("FN RY ALL = %02d, %04dms(NG)\n",state, time);
						break;
					}
				}
			}
		}
	}
	else if( (1 <= no) && (no <= 8) )
	{
		uint8_t CurValue, RstValue;

		CurValue = IO2_Read(GPIOAAA);
		if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
		else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
		
		if (CurValue != RstValue)
		{
			nRetry = 0;
			while(1)
			{
				IO2_Write(OLATA, RstValue);
				read_value = IO2_Read(GPIOAAA);  
				
				if(RstValue == read_value)	
				{
					RET = true;
					//dbgmsg("FN RY %02d  = %02d, %04dms(OK)\n",no, state, time);
					break;
				}
				else
				{
					RET = false;
					if(nRetry++ > 3)	
					{
						dbgmsg("FN RY %02d  = %02d, %04dms(NG)\n",no, state, time);
						break;
					}
				}
			}
		}
		else
		{
			//dbgmsg("FN RY %02d  = %02d, %04dms(OK)\n",no, state, time);
		}
	}
	ntDelay_ms(time);
	
	return RET;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
