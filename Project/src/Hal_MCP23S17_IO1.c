#include "Hal_MCP23S17_IO1.h"

// PORTA0(IN) 	: START1
// PORTA1(IN) 	: START2
// PORTA2(IN) 	: ESC
// PORTA3(IN) 	: MODE
// PORTA4(OUT) 	: LED OK
// PORTA5(OUT) 	: LED NG
// PORTA6(OUT) 	: BUZZ
// PORTA7(OUT) 	: EVCC CHECK RY ON

// PORTB0(OUT) 	: SOL1
// PORTB1(OUT) 	: SOL2
// PORTB2(OUT) 	: SOL3
// PORTB3(OUT) 	: SOL4
// PORTB4(OUT) 	: SOL5
// PORTB5(OUT) 	: SOL6
// PORTB6(OUT) 	: SOL7
// PORTB7(OUT) 	: SOL8

static void IO1_Write(unsigned char address, unsigned char value);
static unsigned char IO1_Read(unsigned char address);
	
void MCP23S17_Init1(void) 
{		
	IO1_Write(IOCON,  0x38); 
	IO1_Write(IOCON+1,0x38); 
    IO1_Write(IODIRA, 0x0F);   // 0 -> output, 1-> input
    IO1_Write(IODIRB, 0x00);   // 0 -> output, 1-> input
    IO1_Write(OLATA,  0x00);   // porta all low
    IO1_Write(OLATB,  0x00);   // portb	
	
	CylinderControl(255, false, 0);
	OKLED(true);
	NGLED(true);
	BUZZER(false);
	EVCC_RY(false);
}

static void IO1_Write(unsigned char address, unsigned char value) 
{    
    IO_EXPAND_CS(0);    
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};    SPI_I2S_SendData(SPI2,IO1_ADDRESS_WRITE); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};    SPI_I2S_SendData(SPI2,address); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};    SPI_I2S_SendData(SPI2,value); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    SPI_I2S_ReceiveData(SPI2);     
    IO_EXPAND_CS(1);
}

static unsigned char IO1_Read(unsigned char address)       
{    
    uint8_t received_data = 0;
    
    IO_EXPAND_CS(0);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};   SPI_I2S_SendData(SPI2,IO1_ADDRESS_READ);  
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};   SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};   SPI_I2S_SendData(SPI2,address); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};   SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};   SPI_I2S_SendData(SPI2,0xFF);          //dummy	
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};   received_data = SPI_I2S_ReceiveData(SPI2);
    IO_EXPAND_CS(1);     
		
    return received_data;   
}

uint8_t OKLED(uint8_t state)
{
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 5;
	
	CurValue = IO1_Read(GPIOAAA);
			
	if(state ==false )     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true ) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 

	CurValue = CurValue & 0xF0;
	RstValue = RstValue & 0xF0;
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO1_Write(OLATA, RstValue);
			read_value = IO1_Read(GPIOAAA);  
						
			if((RstValue>>4) == (read_value>>4))	
			{
				RET = true;
				//dbgmsg("OKLED   = %02d(OK) %02X/%02X\n",state,(RstValue>>4),(read_value>>4));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 3)	
				{
					dbgmsg("OKLED   = %02d(NG) %02X/%02X\n",state,(RstValue>>4),(read_value>>4));
					break;
				}
			}
		}
	}
	else
	{
		//dbgmsg("OKLED   = %02d(OK) %02X/%02X\n",state,(CurValue>>4),(RstValue>>4));
	}
	return RET;
}

uint8_t NGLED(uint8_t state)
{	
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 6;
	
	CurValue = IO1_Read(GPIOAAA);
			
	if(state ==false )     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true ) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 

	CurValue = CurValue & 0xF0;
	RstValue = RstValue & 0xF0;
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO1_Write(OLATA, RstValue);
			read_value = IO1_Read(GPIOAAA);  
						
			if((RstValue>>4) == (read_value>>4))	
			{
				RET = true;
				//dbgmsg("NGLED   = %02d(OK) %02X/%02X\n",state,(RstValue>>4),(read_value>>4));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 3)	
				{
					dbgmsg("NGLED   = %02d(NG) %02X/%02X\n",state,(RstValue>>4),(read_value>>4));
					break;
				}
			}
		}
	}
	else
	{
		//dbgmsg("NGLED   = %02d(OK) %02X/%02X\n",state,(CurValue>>4),(RstValue>>4));
	}

	return RET;
}

uint8_t BUZZER(uint8_t state)
{	
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 7;
	
	CurValue = IO1_Read(GPIOAAA);
			
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 

	CurValue = CurValue & 0xF0;
	RstValue = RstValue & 0xF0;
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO1_Write(OLATA, RstValue);
			read_value = IO1_Read(GPIOAAA);  
						
			if((RstValue>>4) == (read_value>>4))	
			{
				RET = true;
				//dbgmsg("BEEP    = %02d(OK) %02X/%02X\n",state,(RstValue>>4),(read_value>>4));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 3)	
				{
					dbgmsg("BEEP    = %02d(NG) %02X/%02X\n",state,(RstValue>>4),(read_value>>4));
					break;
				}
			}
		}
	}
	else
	{
		//dbgmsg("BEEP    = %02d(OK) %02X/%02X\n",state,(CurValue>>4),(RstValue>>4));
	}
	
	return RET;
}

void Beep_Delay(__IO u32 Count) 
{
	__IO u32 nCount = Count*8400;	
	for (; nCount != 0;nCount--); 
}

void Beep(int time)
{
	if(time == 0)
	{
		BUZZER(true); 
	}
	else
	{
		BUZZER(true);	
		ntDelay_ms(time); 
		
		BUZZER(false);	
		ntDelay_ms(time); 
	}	
}

uint8_t EVCC_RY(uint8_t state)
{	
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 8;
	
	CurValue = IO1_Read(GPIOAAA);
			
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 

	CurValue = CurValue & 0xF0;
	RstValue = RstValue & 0xF0;
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO1_Write(OLATA, RstValue);
			read_value = IO1_Read(GPIOAAA);  
						
			if((RstValue>>4) == (read_value>>4))	
			{
				RET = true;
				//dbgmsg("EVCC RY = %02d(OK) %02X/%02X\n",state,(RstValue>>4),(read_value>>4));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 3)	
				{
					dbgmsg("EVCC RY = %02d(NG) %02X/%02X\n",state,(RstValue>>4),(read_value>>4));
					break;
				}
			}
		}
	}
	else
	{
		//dbgmsg("EVCC RY = %02d(OK) %02X/%02X\n",state,(CurValue>>4),(RstValue>>4));
	}
	
	ntDelay_ms(100); 
	
	return RET;
}

uint8_t CylinderControl(uint8_t no, uint8_t state, uint16_t time)
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
				IO1_Write(OLATB, 0xff);
				read_value = IO1_Read(GPIOBBB);  				
				if(0xff == read_value)	
				{
					RET = true;
					//dbgmsg("Sol ALL = %02d, %04dms(OK)\n",state, time);
					break;
				}
				else
				{
					RET = false;
					if(nRetry++ > 3)	
					{
						dbgmsg("Sol ALL = %02d, %04dms(NG)\n",state, time);
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
				IO1_Write(OLATB, 0x00);
				read_value = IO1_Read(GPIOBBB);  
				
				if(0x00 == read_value)	
				{
					RET = true;
					//dbgmsg("Sol ALL = %02d, %04dms(OK)\n",state, time);
					break;
				}
				else
				{
					RET = false;
					if(nRetry++ > 3)	
					{
						dbgmsg("Sol ALL = %02d, %04dms(NG)\n",state, time);
						break;
					}
				}
			}
		}
	}
	else if( (1 <= no) && (no <= 8) )
	{
		uint8_t CurValue, RstValue;

		CurValue = IO1_Read(GPIOBBB);
		if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
		else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
		
		if (CurValue != RstValue)
		{
			nRetry = 0;
			while(1)
			{
				IO1_Write(OLATB, RstValue);
				read_value = IO1_Read(GPIOBBB);  
				
				if(RstValue == read_value)	
				{
					RET = true;
					//dbgmsg("Sol %02d  = %02d, %04dms(OK)\n",no, state, time);
					break;
				}
				else
				{
					RET = false;
					if(nRetry++ > 3)	
					{
						dbgmsg("Sol %02d  = %02d, %04dms(NG)\n",no, state, time);
						break;
					}
				}
			}
		}
		else
		{
			//dbgmsg("Sol %02d  = %02d, %04dms(OK)\n",no, state, time);
		}
	}
	
  //ki bar sensor 5V
	if( (safe_check_sol_number == no) && (state == true) && (RD_DBG_MD != 0) )
	{
		while(time--)
		{
			if(SAFE_SEN_IN == 0)
			{
				IO1_Write(OLATB, 0x00);
				read_value = IO1_Read(GPIOBBB);  				

				dbgmsg("Safe Sensor ON\n");
				MainPopup(true, GUI_DARKRED, "Safe Sensor ON");
				BUZZER(true);

				EscWait();

				BUZZER(false);
				MainPopup(false, GUI_DARKRED, "Safe Sensor ON");	

				RET = false;
				break;
			}
			ntDelay_ms(1);
		}
	}
	else
	{
		ntDelay_ms(time);
	}
  
    //autonics bar sensor
	/*if( (safe_check_sol_number == no) && (state == true) && (RD_DBG_MD != 0) )
	{
		while(time--)
		{
			if(SAFE_SEN_IN == 1)
			{
				IO1_Write(OLATB, 0x00);
				read_value = IO1_Read(GPIOBBB);  				

				dbgmsg("Safe Sensor ON\n");
				MainPopup(true, GUI_DARKRED, "Safe Sensor ON");
				BUZZER(true);

				EscWait();

				BUZZER(false);
				MainPopup(false, GUI_DARKRED, "Safe Sensor ON");	

				RET = false;
				break;
			}
			ntDelay_ms(1);
		}
	}
	else
	{
		ntDelay_ms(time);
	}*/
	return RET;
}



#define TACT_KEY_CNT  5
#define LONG_KEY_CNT  30
#define LIMIT_KEY_CNT 50
uint8_t ST1_KEY_CNT = 0, ST2_KEY_CNT = 0, ESC_KEY_CNT = 0, MODE_KEY_CNT = 0;
uint8_t ST12_KEY_CNT = 0, MODE_ST1_KEY_CNT = 0, MODE_ST2_KEY_CNT = 0, MODE_ESC_KEY_CNT = 0, ST1_ESC_KEY_CNT = 0;

uint8_t START1_IN =0;
uint8_t START2_IN =0;	
uint8_t MODE_IN   =0;
uint8_t ESC_IN	  =0;

uint8_t KeyRead(void)
{		
	return IO1_Read(GPIOAAA);
}
uint8_t KeyScan(void)
{	
	uint8_t read_value  = IO1_Read(GPIOAAA);  
	
	START1_IN 	= ( ( read_value >> 0 ) & 0x01 );	
	START2_IN 	= ( ( read_value >> 1 ) & 0x01 );
	ESC_IN  	= ( ( read_value >> 2 ) & 0x01 );
	MODE_IN		= ( ( read_value >> 3 ) & 0x01 );	
	
	uint8_t ret = 0;
	if ((MODE_IN == 0) && (START1_IN == 0))
	{
		ST2_KEY_CNT = ESC_KEY_CNT = 0;
		MODE_ST2_KEY_CNT = MODE_ESC_KEY_CNT = ST1_ESC_KEY_CNT = 0;
		MODE_ST1_KEY_CNT++;
		if (MODE_ST1_KEY_CNT == TACT_KEY_CNT)    ret = MOD_ON | ST1_ON;
		else if (MODE_ST1_KEY_CNT > TACT_KEY_CNT)MODE_ST1_KEY_CNT = LIMIT_KEY_CNT;
	}
	else if ((MODE_IN == 0) && (START2_IN == 0))
	{
		ST1_KEY_CNT = ESC_KEY_CNT = 0;
		MODE_ST1_KEY_CNT = MODE_ESC_KEY_CNT = ST1_ESC_KEY_CNT = 0;
		MODE_ST2_KEY_CNT++;
		if (MODE_ST2_KEY_CNT == TACT_KEY_CNT)    ret = MOD_ON | ST2_ON;
		else if (MODE_ST2_KEY_CNT > TACT_KEY_CNT)MODE_ST2_KEY_CNT = LIMIT_KEY_CNT;
	}
	else if ((MODE_IN == 0) && (ESC_IN == 0))
	{
		ST1_KEY_CNT = ST2_KEY_CNT = 0;
		MODE_ST1_KEY_CNT = MODE_ST2_KEY_CNT = ST1_ESC_KEY_CNT = 0;
		MODE_ESC_KEY_CNT++;
		if (MODE_ESC_KEY_CNT == TACT_KEY_CNT)    ret = MOD_ON | ESC_ON;
		else if (MODE_ESC_KEY_CNT > TACT_KEY_CNT)MODE_ESC_KEY_CNT = LIMIT_KEY_CNT;
	}
	else if ((START1_IN == 0) && (ESC_IN == 0))
	{
		ST1_KEY_CNT = ST2_KEY_CNT = 0;
		MODE_ST1_KEY_CNT = MODE_ST2_KEY_CNT = MODE_ESC_KEY_CNT = 0;
		ST1_ESC_KEY_CNT++;
		if (ST1_ESC_KEY_CNT == TACT_KEY_CNT)    ret = ST1_ON | ESC_ON;
		else if (ST1_ESC_KEY_CNT > TACT_KEY_CNT)ST1_ESC_KEY_CNT = LIMIT_KEY_CNT;
	}
	else if ((START1_IN == 0) && (START2_IN == 0))
	{
		ESC_KEY_CNT = 0;
		MODE_ST1_KEY_CNT = MODE_ST2_KEY_CNT = MODE_ESC_KEY_CNT = ST1_ESC_KEY_CNT = 0;
		ST12_KEY_CNT++;
		if (ST12_KEY_CNT == TACT_KEY_CNT)        ret = ST1_ON | ST2_ON;
		else if (ST12_KEY_CNT > TACT_KEY_CNT)    ST12_KEY_CNT = LIMIT_KEY_CNT;
	}
	else if(START1_IN == 0)
	{
		ESC_KEY_CNT = 0;
		MODE_ST1_KEY_CNT = MODE_ST2_KEY_CNT = MODE_ESC_KEY_CNT = ST1_ESC_KEY_CNT = 0;
		ST1_KEY_CNT++;
		if (ST1_KEY_CNT == TACT_KEY_CNT)     
		{
			ret = ST1_ON;
		}
		else if (ST1_KEY_CNT == LONG_KEY_CNT)  
		{
			ret = ST1_LON;
		}
		else if (ST1_KEY_CNT > LONG_KEY_CNT)  
		{
			ret = ST1_LON;
			ST1_KEY_CNT = LIMIT_KEY_CNT;
		}
	}
	else if(START2_IN == 0)
	{
		ESC_KEY_CNT = 0;
		MODE_ST1_KEY_CNT = MODE_ST2_KEY_CNT = MODE_ESC_KEY_CNT = ST1_ESC_KEY_CNT = 0;
		ST2_KEY_CNT++;
		if (ST2_KEY_CNT == TACT_KEY_CNT)     
		{
			ret = ST2_ON;
		}
		else if (ST2_KEY_CNT == LONG_KEY_CNT)   
		{
			ret = ST2_LON;
		}
		else if (ST2_KEY_CNT > LONG_KEY_CNT)   
		{
			ret = ST2_LON;
			ST2_KEY_CNT = LIMIT_KEY_CNT;
		}
	}
	else if(ESC_IN == 0)
	{
		ST1_KEY_CNT = ST2_KEY_CNT = MODE_KEY_CNT = 0;
		MODE_ST1_KEY_CNT = MODE_ST2_KEY_CNT = MODE_ESC_KEY_CNT = ST1_ESC_KEY_CNT = 0;
		ESC_KEY_CNT++;
		if (ESC_KEY_CNT == TACT_KEY_CNT)     
		{
			ret = ESC_ON;
		}
		else if (ESC_KEY_CNT == LONG_KEY_CNT)  
		{
			ret = ESC_LON;
		}
		else if (ESC_KEY_CNT > LONG_KEY_CNT)  
		{
			ret = ESC_LON;
			ESC_KEY_CNT = LIMIT_KEY_CNT;
		}
	}
	else if(MODE_IN == 0)
	{
		MODE_ST1_KEY_CNT = MODE_ST2_KEY_CNT = MODE_ESC_KEY_CNT = ST1_ESC_KEY_CNT = 0;
		MODE_KEY_CNT++;
		if (MODE_KEY_CNT == TACT_KEY_CNT)        
		{
			ret = MOD_ON;
		}
		else if (MODE_KEY_CNT == LONG_KEY_CNT)   
		{
			ret = MOD_LON;
		}
		else if (MODE_KEY_CNT > LONG_KEY_CNT)   
		{
			ret = MOD_LON;
			MODE_KEY_CNT = LIMIT_KEY_CNT;
		}
	}
	else
	{
		ST1_KEY_CNT = ST2_KEY_CNT = ESC_KEY_CNT = MODE_KEY_CNT = ST1_ESC_KEY_CNT = 0;
		ST12_KEY_CNT = MODE_ST1_KEY_CNT = MODE_ST2_KEY_CNT = MODE_ESC_KEY_CNT = 0;
		ret = 0;
	}	
	return ret;
}

uint8_t mfs_test(void)
{
	uint8_t read_value  = IO1_Read(GPIOAAA);  
	START1_IN 	= ( ( read_value >> 0 ) & 0x01 );

	if(START1_IN == 0) return 1;
	else return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
