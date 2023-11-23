#include "func_pmic.h"

#define Read_slA 0x41
#define Write_slA 0x40

char read_pmic[256] = {0,};

void PMIC_Power_On(void)
{	
    I2C1_Init();
	FuncRYControl(1,true,0);//GND Contact
    FuncRYControl(2,true,0);//GND - EEP WP Contact
	FuncRYControl(3,true,0);//SCL SDA
    FuncRYControl(4,false,0);//EEP_WP1, EEP_WP2 Low
    FuncRYControl(5,true,0);//USB_M,P
    FuncRYControl(6,false,250);//
	EvccControl(1,false);// 1.8V Off
	EvccControl(2,true);// 3.3V ON (Level Shift Voltage)

	DVDD_ON(true); //VTSP
	Delay_ms(10);
	AVDD_ON(true); //VIN
	RYBD_6ON(true);		// external 12V on
	Delay_ms(500); 
}

void PMIC_Power_Off(void)
{ 
	EvccControl(1,false); //
	EvccControl(2,false);
	Delay_ms(10);
	AVDD_ON(false);//VIN
	RYBD_6ON(false);	// external 12V off
	Delay_ms(50);
	DVDD_ON(false);//VTSP
	
    Delay_ms(50);
	Discharge1_ON(true);
	Discharge2_ON(true);
	Delay_ms(500);  
	Discharge1_ON(false);
	Discharge2_ON(false);
	//FuncRYControl(4,true,500);  //discharge
    //FuncRYControl(4,false,0); 
	Delay_ms(50);  //Delay_ms(250);  
	//-----잔류전원 제거 Flow
	//------------------
}
uint8_t LGD_pmic_init(uint16_t index)
{
	//전원 및 Port Init
	dbgmsg("%s\n",__func__); 
	//--------------------Relay Board ON
	RYBD_1ON(true);
	Delay_ms(100);
	RYBD_2ON(true);
	Delay_ms(100);
	RYBD_3ON(true);
	Delay_ms(100);
	RYBD_4ON(true);
	Delay_ms(100);
	RYBD_5ON(true);
	Delay_ms(100);
	//RYBD_6ON(true);
	Delay_ms(500);
	//------------------------------------
	ListView_MEAS("OK");
	return true;
}
/*uint8_t LGD_pmic_write(void)
{
	int PMIC_SIZE = In_pmic.size;
	
	dbgmsg("Write_slA : %X\n",Write_slA);
	dbgmsg("PMIC Size : %d\n",PMIC_SIZE);
  
	//for(int cnt = 0; cnt < PMIC_SIZE-1; cnt++)
	for(int cnt = 0; cnt < PMIC_SIZE; cnt++)
	{
		dbgmsg("%02X\t", INPUT_PMIC[cnt]);
		if (cnt % 2 == 1)	dbgmsg("\n");
	}
	
	//for(int cnt = 0; cnt < PMIC_SIZE-1; cnt++)
	for(int cnt = 0; cnt < PMIC_SIZE; cnt++)
	{
		i2c1_start();
		Delay_us(100);
		
		//device address write
		if( i2c1_tx( Write_slA) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e1 cnt: %d\n", cnt); return false;}
		Delay_us(100);
		//DAC address wrtie
		if( i2c1_tx(cnt) != 0 )		{i2c1_stop(); dbgmsg("pmic_wr e2   cnt\n", cnt);	return false; } 
		Delay_us(100);	
		//Data
		uint8_t TempData8 = INPUT_PMIC[cnt];  		dbgmsg("0x%02X ",TempData8);
		if(i2c1_tx( TempData8 ) != 0){	i2c1_stop(); dbgmsg("pmic_wr e3   cnt\n", cnt);	return false; }	
    
		i2c1_stop(); 	
		Delay_us(100);
		
	}	
	dbgmsg("\n");	

	i2c1_start(); 
	Delay_us(100);
	
  //device address
	if( i2c1_tx(Write_slA) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e4\n");	return false; }
	Delay_us(100);
  
  //control register address
	if( i2c1_tx(0xFF) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e5\n");	return false; }
	Delay_us(100);
  
  //control register data(0x40);
	if( i2c1_tx(0x40) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e6\n");	return false; }
	Delay_us(100);
	
	i2c1_stop(); 	
	Delay_ms(100);

	return true;  
}*/

uint8_t Address_Search_Function(void)
{
	uint8_t addr = 0, slaveAddress = 0;
	uint8_t found = 0;
	for(uint16_t i = 0; i <= 255; i++)
	{
		addr = i;
		
		i2c1_start();
		if(i2c1_tx( (addr & 0xFE) ) == 0)
		{
			i2c1_stop();
			slaveAddress = addr;			
			dbgmsg("HURRAY !! Slave Addr = 0x%2x\n", slaveAddress); 
			found = 1;
		}
		i2c1_stop();
	}
	if (found == 0)
	{
		dbgmsg("Warning : Slave Addr NOT MATCHED !!!\n");
	}
	Delay_ms(2);
	return 0;
}

uint8_t LGD_pmic_read(void)
{
  dbgmsg("%s\n",__func__); 
  uint8_t  TempData8;
	
	int PMIC_SIZE = In_pmic.size;
			
	for(int cnt = 0; cnt < PMIC_SIZE-1; cnt++)
    {
      //start
	  i2c1_start(); 
      Delay_us(100);
      
      //device address write
      if( i2c1_tx(Write_slA) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e1\n"); 	 return false; }
      Delay_us(100);
      
      //control register address
      if( i2c1_tx( 0xFF ) != 0 )		{	i2c1_stop(); dbgmsg("pmic_wr e2\n"); 	return false; } 
      Delay_us(100);
      
      //control register data(eeprom  = 0x01 ; DAC = 0x00)
      if( i2c1_tx( 0x01 ) != 0 )		{	i2c1_stop(); dbgmsg("pmic_wr e3\n");	return false; } 
      Delay_us(100);
      
      //stop
      i2c1_stop(); 	
      Delay_us(100);
      
      //start
      i2c1_start(); 
      Delay_us(100);
      
	  //device address
      if( i2c1_tx(Write_slA) != 0 )	{	i2c1_stop(); dbgmsg("pmic_rd e4\n");	return false; }
      Delay_us(100);
      
      //DAC Start address
      //if( i2c1_tx( 0x00 ) != 0 )			{	i2c1_stop(); dbgmsg("pmic_rd e5\n");	return false; }
	  if( i2c1_tx( cnt ) != 0 )			{	i2c1_stop(); dbgmsg("pmic_rd e5\n");	return false; } 
      Delay_us(100); 
      
      //repeated start
      i2c1_restart();  
      Delay_us(100);      

      //device address write
      if( i2c1_tx(Read_slA) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e6\n"); 	 return false; }
      Delay_us(100);
      
		//for(int cnt = 0; cnt < PMIC_SIZE-1; cnt++)
		{
		  //read Data NACK
		  //TempData8 = i2c1_rx(1);
			
			TempData8 = i2c1_rx(0);
		  Delay_us(100);
		  //save value
		  read_pmic[cnt] = TempData8;      
			dbgmsg(" 0x%02X: write: %02X,     read: 0x%02X     %s\n",cnt, INPUT_PMIC[cnt], read_pmic[cnt], (INPUT_PMIC[cnt] == read_pmic[cnt])? " ":"NG" );		  
		}
		
      i2c1_stop();
      Delay_us(100);
    }			
	return true;
}

uint8_t LGD_pmic_write(void)
{
	int PMIC_SIZE = In_pmic.size;
	uint8_t write_Address = 0x00;
	Address_Search_Function();
	
	i2c1_start();
	
	Delay_us(100);
	
	//device address write
	dbgmsg("Write_slA : %X\n",Write_slA);
	if( i2c1_tx( Write_slA) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e1\n"); return false;}
	Delay_us(100);

	// EEPROM / DAC  Start address wrtie
	if( i2c1_tx( write_Address ) != 0 )		{i2c1_stop(); dbgmsg("pmic_wr e2\n");	return false; } 
	Delay_us(100);

	for(int cnt = 0; cnt < PMIC_SIZE-1; cnt++)
	{
		//Data
		uint8_t TempData8 = INPUT_PMIC[cnt];  		dbgmsg("0x%02X ",TempData8);
		if(i2c1_tx( TempData8 )!= 0){	i2c1_stop(); dbgmsg("pmic_wr e3\n");	return false; }	
		Delay_ms(10);		
	}	
	dbgmsg("\n");
	i2c1_stop(); 		
	Delay_us(500);

	i2c1_start(); 
	Delay_us(100);
	
	//device address
	if( i2c1_tx(Write_slA) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e4\n");	return false; }
	Delay_us(100);
	
	//control register address
	if( i2c1_tx(0xFF) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e5\n");	return false; }
	Delay_us(100);
	
	//control register data(0x80);
	if( i2c1_tx(0x80) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e6\n");	return false; }
	Delay_us(100);
	
	i2c1_stop(); 	
	Delay_ms(400);

	return true;  
}

/*uint8_t LGD_pmic_read(uint8_t _type)
{
  dbgmsg("%s\n",__func__); 
  uint8_t  TempData8;
	
	int PMIC_SIZE = In_pmic.size;
	
	if (_type == type_EPRM)	dbgmsg("EEPROM Write\n");
	else if (_type == type_DAC)	dbgmsg("DAC Write\n");
	else return false;
	
	for(int cnt = 0; cnt < PMIC_SIZE-1; cnt++)
    {
      //start
	  i2c1_start(); 
      Delay_us(100);
      
      //device address write
      if( i2c1_tx(Write_slA) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e1\n"); 	 return false; }
      Delay_us(100);
      
      //control register address
      if( i2c1_tx( 0xFF ) != 0 )		{	i2c1_stop(); dbgmsg("pmic_wr e2\n"); 	return false; } 
      Delay_us(100);
      
      //control register data(eeprom  = 0x01 ; DAC = 0x00)
      if( i2c1_tx( _type ) != 0 )		{	i2c1_stop(); dbgmsg("pmic_wr e3\n");	return false; } 
      Delay_us(100);
      
      //stop
      i2c1_stop(); 	
      Delay_us(100);
      
      //start
      i2c1_start(); 
      Delay_us(100);
      
	  //device address
      if( i2c1_tx(Write_slA) != 0 )	{	i2c1_stop(); dbgmsg("pmic_rd e4\n");	return false; }
      Delay_us(100);
      
      //DAC Start address
      //if( i2c1_tx( 0x00 ) != 0 )			{	i2c1_stop(); dbgmsg("pmic_rd e5\n");	return false; }
	  if( i2c1_tx( cnt ) != 0 )			{	i2c1_stop(); dbgmsg("pmic_rd e5\n");	return false; } 
      Delay_us(100); 
      
      //repeated start
      i2c1_restart();  
      Delay_us(100);      

      //device address write
      if( i2c1_tx(Read_slA) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e6\n"); 	 return false; }
      Delay_us(100);
      
		//for(int cnt = 0; cnt < PMIC_SIZE-1; cnt++)
		{
		  //read Data NACK
		  //TempData8 = i2c1_rx(1);
			
			TempData8 = i2c1_rx(0);
		  Delay_us(100);
		  //save value
		  read_pmic[cnt] = TempData8;      
			dbgmsg(" 0x%02X: write: %02X,     read: 0x%02X     %s\n",cnt, INPUT_PMIC[cnt], read_pmic[cnt], (INPUT_PMIC[cnt] == read_pmic[cnt])? " ":"NG" );		  
		}
		
      i2c1_stop();
      Delay_us(100);
    }			
	return true;
}*/

/*uint8_t LGD_pmic_write(uint8_t _type)
{
	int PMIC_SIZE = In_pmic.size;
	
	dbgmsg("Write_slA : %X\n",Write_slA);
	dbgmsg("PMIC Size : %d\n",PMIC_SIZE);
	
	if (_type == type_EPRM)	dbgmsg("EEPROM Write\n");
	else if (_type == type_DAC)	dbgmsg("DAC Write\n");		
	
	i2c1_start();
	Delay_us(100);
	
	//device address write
	if( i2c1_tx( Write_slA) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e1 \n"); return false;}
	Delay_us(100);
	
	// EEPROM / DAC  Start address wrtie
	if( i2c1_tx(0x00) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e2 \n"); return false;}
	Delay_us(100);
	
	for (int cnt = 0; cnt < PMIC_SIZE-1; cnt++)
	{
		//Data
		uint8_t TempData8 = INPUT_PMIC[cnt];  		dbgmsg("write: 0x%02X \t",TempData8);
		if(i2c1_tx( TempData8 ) != 0){	i2c1_stop(); dbgmsg("pmic_wr e3   cnt\n", cnt+1);	return false; }	
    
		dbgmsg("\n");
		Delay_ms(10);
	}	
	
	i2c1_stop();
	
	if ( _type == type_EPRM )
	{
		// EEPROM write
		Delay_us(500);
		
		i2c1_start(); 
		Delay_us(100);
		
		//device address
		if( i2c1_tx(Write_slA) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e4\n");	return false; }
		Delay_us(100);
	  
		//control register address
		if( i2c1_tx(0xFF) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e5\n");	return false; }
		Delay_us(100);
	  
		//control register data(0x80);
		if( i2c1_tx(0x80) != 0 ){	i2c1_stop(); dbgmsg("pmic_wr e6\n");	return false; }
		Delay_us(100);
		
		i2c1_stop();
	}
	else{
		// DAC Write
	}
	Delay_ms(1000);

	return true;  
}*/

//uint8_t LGD_pmic_write_flow(uint16_t index, uint8_t _type)
uint8_t LGD_pmic_write_flow(uint16_t index)
{
	uint8_t ret = LGD_pmic_write();
	if(ret == true)ListView_MEAS("OK");
	else ListView_MEAS("Fail");
	return ret;
}

//uint8_t LGD_pmic_read_flow(uint16_t index, uint8_t _type)
uint8_t LGD_pmic_read_flow(uint16_t index)
{	
	
	
	uint8_t isVerifyErr = false;
	uint8_t ucRet = false;
	uint32_t spec = (func_spec_hex[index][0] << 24) | (func_spec_hex[index][1] << 16) | (func_spec_hex[index][2] << 8) | (func_spec_hex[index][3] << 0);
	
	char text[50];
	
	int PMIC_SIZE = In_pmic.size;
	uint32_t CRC_TargetTemp = 0;
	
	for(int cnt = 0; cnt < PMIC_SIZE-1; cnt++)read_pmic[cnt] = NULL;	
	
	InitCRC32();
	CRC_TargetTemp = Get_CRC32Target();
	
	uint8_t ret = LGD_pmic_read();
	
	if(ret == false)return false;
	else
	{
		//for(uint8_t tt=0; tt< PMIC_SIZE-1; tt++)
		for(uint8_t tt=0; tt< PMIC_SIZE-1; tt++)
		{
			CRC_TargetTemp = Get_CRC32(read_pmic[tt], CRC_TargetTemp);
			if(read_pmic[tt] != INPUT_PMIC[tt]){isVerifyErr = true;}
			dbgmsg("%d: PCB[%02X],Data[%02X]\n",tt+1, read_pmic[tt], INPUT_PMIC[tt]); 
		
		}	
		//sprintf(text, "0x%0X / 0x%0X", CRC_TargetTemp,);
		
		dbgmsg("CRC[%08X],SPEC[%08X]\n",CRC_TargetTemp, spec); 
		if(isVerifyErr == true)
		{
			ListView_MEAS("1:1 Verify Fail"); 
			dbgmsg("Diff data\n");
			ucRet = false;
		}
		else if(CRC_TargetTemp == spec)	
		{	
			//ListView_MEAS(text); 
			sprintf(text,"%08X / %08X",CRC_TargetTemp,spec);
			ListView_MEAS(text); 
			dbgmsg("Same data\n");
			ucRet = true;
		}
		else			
		{	
			dbgmsg("Diff CRC\n");
			ListView_MEAS("CRC NG"); 
			ucRet =  false;
		}
	}
	return ucRet;
}
typedef enum
{
	PMIC_INIT = 1,
	PMIC_Write,
	PMIC_Read
}PMIC_TYPE;

uint8_t PMIC_Function(uint16_t index)
{
	uint8_t ret = false;
	dbgmsg("   PMIC ==> %02d\n", func_module[index]);
	dbgmsg("index No : %d\n", index);
	
	switch(func_module[index]) 
	{
		case PMIC_INIT :		
			
			ret=LGD_pmic_init(index);			
			break;	// ok
			
		case PMIC_Write :		
			PMIC_Power_On();					
			
			switch_Hold();
		
			ListView_MEAS("~ing...");
			ListView_RESULT(BUSY_RESULT, AUTO_STATE);
			dbgmsg("PMIC WRITE START\n");			
			//ret = LGD_pmic_write_flow(index, type_EPRM);
			ret = LGD_pmic_write_flow(index);
			Delay_ms(200);
			PMIC_Power_Off();					
		
			break;	// ok
		
		case PMIC_Read :	

			PMIC_Power_On();					
		
			switch_Hold();
		
			ListView_MEAS("~ing...");
			ListView_RESULT(BUSY_RESULT, AUTO_STATE);
			Delay_ms(100);
			dbgmsg("PMIC READ START\n");			
			//ret = LGD_pmic_read_flow(index, type_EPRM);	
			ret = LGD_pmic_read_flow(index);
			PMIC_Power_Off();		
		
		break;	// ok
			default : ret = false;										break;	// ok
	}
	return ret;
	
}






