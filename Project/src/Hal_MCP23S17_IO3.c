#include "Hal_MCP23S17_IO3.h"

// PORTA0(IN) 	: SEN_ON1
// PORTA1(IN) 	: SEN_ON2
// PORTA2(IN) 	: SEN_ON3
// PORTA3(IN) 	: SEN_ON4
// PORTA4(IN) 	: SEN_ON5
// PORTA5(IN) 	: SEN_ON6
// PORTA6(IN) 	: SEN_ON7
// PORTA7(IN) 	: SEN_ON8

// PORTB0(OUT) 	: IO3_GPB_0(TBD)
// PORTB1(OUT) 	: IO3_GPB_1(TBD)
// PORTB2(OUT) 	: IO3_GPB_2(TBD)
// PORTB3(OUT) 	: IO3_GPB_3(TBD)
// PORTB4(OUT) 	: IO3_GPB_4(TBD)
// PORTB5(OUT) 	: IO3_GPB_5(TBD)
// PORTB6(OUT) 	: IO3_GPB_6(TBD)
// PORTB7(OUT) 	: IO3_GPB_7(TBD)

static void IO3_Write(unsigned char address, unsigned char value);
static unsigned char IO3_Read(unsigned char address);

void MCP23S17_Init3(void) 
{
    IO3_Write(IOCON,  0x38); 
    IO3_Write(IOCON+1,0x38); 
    IO3_Write(IODIRA, 0xFF);   // 0 -> output, 1-> input
    IO3_Write(IODIRB, 0x00);   // 0 -> output, 1-> input
    IO3_Write(OLATA,  0x00);   // porta all low
    //IO3_Write(OLATB,  0x00);   // portb			
    IO3_Write(OLATB,  0xC0); //AVDD DVDD OFF
}

static void IO3_Write(unsigned char address, unsigned char value) 
{    
    IO_EXPAND_CS(0);    
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};    SPI_I2S_SendData(SPI2,IO3_ADDRESS_WRITE); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};    SPI_I2S_SendData(SPI2,address); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};    SPI_I2S_SendData(SPI2,value); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    SPI_I2S_ReceiveData(SPI2);     
    IO_EXPAND_CS(1);
}

static unsigned char IO3_Read(unsigned char address)       
{    
    uint8_t received_data = 0;
    
    IO_EXPAND_CS(0);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};   SPI_I2S_SendData(SPI2,IO3_ADDRESS_READ);  
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};   SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};   SPI_I2S_SendData(SPI2,address); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};   SPI_I2S_ReceiveData(SPI2); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET){};   SPI_I2S_SendData(SPI2,0xFF);          //dummy	
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};   received_data = SPI_I2S_ReceiveData(SPI2);
    IO_EXPAND_CS(1);     
		
    return received_data;   
}

uint8_t Sen_Scan(void)
{	
    return ~(IO3_Read(GPIOAAA));   
}

uint8_t CylinderSensorCheck(uint8_t no, uint8_t state, uint16_t time)
{	
	char Sen_State[8] = {0};
	char text[50];
	int flag = false;

	sprintf(text, "Check Cylinder Sensor(%02X)",no);						
	MainPopup(true, GUI_DARKRED, text);	

	while(time--)
	{
		uint8_t read_value = Sen_Scan();

		Sen_State[(no-1)] 	= ( ( read_value >> (no-1) ) & 0x01 );

		if(Sen_State[no-1] == (state))
		{
			flag = true;
			break;
		}
		ntDelay_ms(1);		
	}
	
	if(flag == false) 
	{
		dbgmsg("NG\n");
		CylinderControl(255, false, 0);
		sprintf(text, "Cylinder Sensor(%02X)\nPosition Error",no);		
		MainPopup(true, GUI_DARKRED,text);	
	}
	else
	{
		dbgmsg("OK\n");
		MainPopup(false, GUI_DARKRED,"");	
	}	
		
	return flag; 
}


uint8_t RYBD_1ON(uint8_t state)
{
	for(int i = 0; i < 10; i++)
	{
		if (state == 0)	// Relay Board Relay 1,2,
		{
			PA_0(0);	
		}
		else
		{
			PA_0(1);
		}
		Delay_ms(1);
	}
	return true;
	
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 1;
	
	CurValue = IO3_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO3_Write(OLATB, RstValue);
			read_value = IO3_Read(GPIOBBB);						
			
			if((RstValue) == (read_value))	
			{
				RET = true;
				dbgmsg("RYBD 1ON   = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 100)	
				{
					dbgmsg("RYBD 1ON = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
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

uint8_t RYBD_2ON(uint8_t state)
{
	for(int i = 0; i < 10; i++)
	{
		if (state == 0)		// Relay Board Relay 1,2,3,4,5		
		{
			PA_1(0);
		}
		else{
			PA_1(1);
		}
		Delay_ms(1);
	}
	return true;
	
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 2;
	
	CurValue = IO3_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO3_Write(OLATB, RstValue);
			read_value = IO3_Read(GPIOBBB);						
			
			if((RstValue) == (read_value))	
			{
				RET = true;
				dbgmsg("RYBD 2ON   = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 100)	
				{
					dbgmsg("RYBD 2ON = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
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

uint8_t RYBD_3ON(uint8_t state)
{
	return 0;
	
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 3;
	
	CurValue = IO3_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO3_Write(OLATB, RstValue);
			read_value = IO3_Read(GPIOBBB);						
			
			if((RstValue) == (read_value))	
			{
				RET = true;
				dbgmsg("RYBD 3ON   = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 100)	
				{
					dbgmsg("RYBD 3ON = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
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
uint8_t RYBD_4ON(uint8_t state)
{
	return 0;
	
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 4;
	
	CurValue = IO3_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO3_Write(OLATB, RstValue);
			read_value = IO3_Read(GPIOBBB);						
			
			if((RstValue) == (read_value))	
			{
				RET = true;
				dbgmsg("RYBD 4ON   = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 100)	
				{
					dbgmsg("RYBD 4ON = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
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
uint8_t RYBD_5ON(uint8_t state)
{
	return 0;
	
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 5;
	
	CurValue = IO3_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO3_Write(OLATB, RstValue);
			read_value = IO3_Read(GPIOBBB);						
			
			if((RstValue) == (read_value))	
			{
				RET = true;
				dbgmsg("RYBD 5ON   = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 100)	
				{
					dbgmsg("RYBD 5ON = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
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
uint8_t RYBD_6ON(uint8_t state)
{
	for(int i = 0; i < 10; i++)
	{
		if (state == 0)		// Relay Board Relay 6 (VIN Control)
		{
			PC_13(0);
		}
		else
		{
			PC_13(1);
		}
		Delay_ms(1);
	}
	return true;
	
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 6;
	
	CurValue = IO3_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO3_Write(OLATB, RstValue);
			read_value = IO3_Read(GPIOBBB);						
			
			if((RstValue) == (read_value))	
			{
				RET = true;
				dbgmsg("RYBD 6ON   = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 100)	
				{
					dbgmsg("RYBD 6ON = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
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

//----------------------------------------------------------------------------------
uint8_t AVDD_ON(uint8_t state)
{
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 7;
	
	CurValue = IO3_Read(GPIOBBB);
	
	if(state == true)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == false) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO3_Write(OLATB, RstValue);
			read_value = IO3_Read(GPIOBBB);						
			
			if((RstValue) == (read_value))	
			{
				RET = true;
				dbgmsg("VTSP OFF   = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 100)	
				{
					dbgmsg("VTSP OFF = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
					break;
				}
			}
		}
	}
	else
	{
		dbgmsg("VTSP OFF = %02d(OK) %02X/%02X\n",state,(RstValue),(CurValue));
	}	
	return RET;
}
uint8_t DVDD_ON(uint8_t state)
{
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 8;
	
	CurValue = IO3_Read(GPIOBBB);
	
	if(state == true)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == false) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO3_Write(OLATB, RstValue);
			read_value = IO3_Read(GPIOBBB);						
			
			if((RstValue) == (read_value))	
			{
				RET = true;
				dbgmsg("VUSB_OFF = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 100)	
				{
					dbgmsg("VUSB_OFF = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
					break;
				}
			}
		}
	}
	else
	{
		dbgmsg("VUSB_OFF = %02d(OK) %02X/%02X\n",state,(RstValue),(CurValue));
	}	
	return RET;
}
//--------------------------------------------------------------------------------------------------
uint8_t Extend_VDD_RY(uint8_t state)
{
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
	uint8_t no = 7;
	
	CurValue = IO3_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << (no-1))) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << (no-1))  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO3_Write(OLATB, RstValue);
			read_value = IO3_Read(GPIOBBB);						
			
			if((RstValue) == (read_value))	
			{
				RET = true;
				dbgmsg("Extend_VDD_RY   = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 100)	
				{
					dbgmsg("Extend_VDD_RY = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
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

uint8_t Extend_VDD(uint8_t state)
{
	uint8_t RET = true;
	uint8_t nRetry = 0;
	uint8_t read_value = 0; 
	
	uint8_t CurValue, RstValue;
//	uint8_t no = 8;
	
	CurValue = IO3_Read(GPIOBBB);
	
	if(state == false)     RstValue = ((~(0x01 << 5)) & 0xff) & CurValue;
	else if(state == true) RstValue =   ((0x01 << 5)  & 0xff) | CurValue; 
	
	if (CurValue != RstValue)
	{
		nRetry = 0;
		while(1)
		{
			IO3_Write(OLATB, RstValue);
			read_value = IO3_Read(GPIOBBB);
			
			if((RstValue) == (read_value))	
			{
				RET = true;
				dbgmsg("EX VDD  = %02d(OK) %02X/%02X\n",state,(RstValue),(read_value));
				break;
			}
			else
			{
				RET = false;
				if(nRetry++ > 100)	
				{
					dbgmsg("EX VDD = %02d(NG) %02X/%02X\n",state,(RstValue),(read_value));
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
