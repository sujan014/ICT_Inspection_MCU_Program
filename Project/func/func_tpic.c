#include "func_eep.h"

#define Read_slA 0x55
#define Write_slA 0x54

char read_tpic[256] = {0,};

void TPIC_Power_On(void)
{
	FuncRYControl(1,true,0);//GND Contact
    FuncRYControl(2,true,0);//GND - EEP WP Contact
	FuncRYControl(3,true,0);//SCL SDA
    FuncRYControl(4,false,0);//SWCLK, SWDIO - GND Contact
    FuncRYControl(5,false,0);//USB_M,P
    FuncRYControl(6,false,250);//
	EvccControl(1,false);// 1.8V Off
	EvccControl(2,true);// 3.3V ON (Level Shift Voltage)
	
	DVDD_ON(true); //VTSP
	Delay_ms(10);
	AVDD_ON(true); //VIN
	Delay_ms(500); 
}
void TPIC_Power_Off(void)
{
	EvccControl(1,false); //
	EvccControl(2,false);
	Delay_ms(10);
	AVDD_ON(false); //VIN
	Delay_ms(50);
	DVDD_ON(false); //VTSP

    Delay_ms(50);
	Discharge1_ON(true);
	Discharge2_ON(true);
	Delay_ms(500);  
	Discharge1_ON(false);
	Discharge2_ON(false);

	//-----잔류전원 제거 Flow
	//------------------
}

uint8_t tpic_init(uint16_t index)
{
	dbgmsg("%s\n",__func__); 
	
	ListView_MEAS("OK");
	Delay_ms(200);
	return true;
}
uint8_t lgd_tpic_write(void)
{
	int TPIC_SIZE = In_tpic.size;
	
	dbgmsg("TPIC Write Start\n"); 
	
	dbgmsg("TPIC_SIZE : %d\n",TPIC_SIZE); 
	for(int cnt = 0; cnt < TPIC_SIZE; cnt++)
	{
		dbgmsg("data write : %d\n",cnt); 
		i2c1_start(); 
		Delay_us(100);
		if( i2c1_tx( Write_slA) != 0 ){	i2c1_stop(); dbgmsg("tpic_wr e1\n"); return false;}
		Delay_us(100);
		if( i2c1_tx(cnt+1) != 0 )		{i2c1_stop(); dbgmsg("tpic_wr e2\n");	return false; } 
		Delay_us(100);
		uint8_t TempData8 = INPUT_TPIC[cnt];  		
		if(i2c1_tx( TempData8 ) != 0){	i2c1_stop(); dbgmsg("tpic_wr e3\n");	return false; }	
		
		i2c1_stop();
		Delay_ms(50);
		dbgmsg("0x%02X ",TempData8);
	}	
	dbgmsg("\n");
	//i2c1_stop(); 	
	//Delay_ms(50);
	i2c1_start(); 
	Delay_us(100);
	if( i2c1_tx(Write_slA) != 0 ){	i2c1_stop(); dbgmsg("tpic_wr e4\n");	return false; }
	Delay_us(100);
	if( i2c1_tx(0xFF) != 0 ){	i2c1_stop(); dbgmsg("tpic_wr e5\n");	return false; }
	Delay_us(100);
	if( i2c1_tx(0x80) != 0 ){	i2c1_stop(); dbgmsg("tpic_wr e6\n");	return false; }
	Delay_us(100);
	i2c1_stop(); 	
	Delay_ms(500);

	return true; 
}


uint8_t lgd_tpic_read(void)
{
	
	uint8_t  TempData8;
	dbgmsg("TPIC Read Start\n"); 
	int TPIC_SIZE = In_tpic.size;
	
	i2c1_start(); 
	Delay_us(100);
	
	if( i2c1_tx(Write_slA) != 0 ){	i2c1_stop(); dbgmsg("tpic_wr e1\n"); 	 return false; }
	Delay_us(100);
	if( i2c1_tx( 0xFF ) != 0 )		{	i2c1_stop(); dbgmsg("tpic_wr e2\n"); 	return false; } 
	Delay_us(100);
	if( i2c1_tx( 0x01 ) != 0 )		{	i2c1_stop(); dbgmsg("tpic_wr e3\n");	return false; } 
	Delay_us(100);
	i2c1_stop(); 	
	Delay_ms(500);
	
	for(uint8_t cnt=0; cnt<TPIC_SIZE; cnt++)
	{
	dbgmsg("data read : %d\n",cnt); 
	i2c1_start(); 	Delay_us(100);
	if( i2c1_tx(Write_slA) != 0 )	{	i2c1_stop(); dbgmsg("tpic_rd e1\n");	return false; }
	Delay_us(100);
	if( i2c1_tx(cnt+1) != 0 )			{	i2c1_stop(); dbgmsg("tpic_rd e2\n");	return false; } 
	Delay_us(100);
	
	//i2c1_stop();
	//Delay_us(100);
	i2c1_start();  
	Delay_us(100);
	if(i2c1_tx( (Write_slA)|0x01) != 0 ){	i2c1_stop(); dbgmsg("tpic_rd e3\n");	return false; }
	Delay_us(100);
	TempData8 = i2c1_rx(0);
	//if( cnt == (TPIC_SIZE-1) ) 			TempData8 = i2c1_rx(0);
	//else     	           				TempData8 = i2c1_rx(1);
	read_tpic[cnt] = TempData8;  	
	
	//read_tpic[cnt] = i2c1_rx(0);
	dbgmsg("Data[%d] : 0x%02X\n",cnt, read_tpic[cnt]);
	i2c1_stop(); 
	Delay_ms(50);
	}
	//i2c1_stop(); 
	dbgmsg("\n");
	return true;
}

uint8_t tpic_write_flow(uint16_t index)
{
	dbgmsg("TPIC Write Flow\n"); 
	uint8_t ret = lgd_tpic_write();
	return ret;
}


uint8_t tpic_read_flow(uint16_t index)
{	
	dbgmsg("TPIC Read Flow\n"); 
	uint8_t isVerifyErr = false;
	uint8_t ucRet = false;
	uint32_t spec = (func_spec_hex[index][0] << 24) | (func_spec_hex[index][1] << 16) | (func_spec_hex[index][2] << 8) | (func_spec_hex[index][3] << 0);
	
	char text[50];
	
	int TPIC_SIZE = In_tpic.size;
	uint32_t CRC_TargetTemp = 0;
	
	for(int cnt = 0; cnt < TPIC_SIZE; cnt++)read_tpic[cnt] = NULL;
	
	InitCRC32();
	CRC_TargetTemp = Get_CRC32Target();
	
	uint8_t ret = lgd_tpic_read();
	
	if(ret == false)return false;
	else
	{
		for(uint8_t tt=0; tt< TPIC_SIZE; tt++)
		{
			CRC_TargetTemp = Get_CRC32(read_tpic[tt], CRC_TargetTemp);
			if(read_tpic[tt] != INPUT_TPIC[tt]){isVerifyErr = true;}
			dbgmsg("PCB[%02X],Data[%02X]\n",read_tpic[tt], INPUT_TPIC[tt]); 	
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
			sprintf(text, "%08X / %08X", CRC_TargetTemp,spec);
			ListView_MEAS(text);
			dbgmsg("Same data\n");
			ucRet = true;
		}
		else			
		{	
			sprintf(text, "0x%08X / 0x%08X", CRC_TargetTemp,spec);
			dbgmsg("Diff CRC\n");
			ListView_MEAS(text); 
			ucRet =  false;
		}
	}
	return ucRet;
}
typedef enum
{
	TPIC_INIT = 1,
	TPIC_Write,
	TPIC_Read
}TPIC_TYPE;

uint8_t TPIC_Function(uint16_t index)
{
	uint8_t ret = false;
	dbgmsg("   TPIC ==> %02d\n", func_module[index]);
	dbgmsg("index No : %d\n", index);
	
	switch(func_module[index]) 
	{
		case TPIC_INIT :		ret=tpic_init(index);			break;	// ok
			
		case TPIC_Write :		
				
				TPIC_Power_On();				
		
				switch_Hold();
		
				ListView_MEAS("~ing...");
				ListView_RESULT(BUSY_RESULT, AUTO_STATE);
				Delay_ms(5);
		
				//while(1)
				//{
				ret = tpic_write_flow(index);	
				//}
		
			TPIC_Power_Off();

		
		if(ret == true)ListView_MEAS("OK");
				else ListView_MEAS("Fail");			
		
		break;	// ok
		case TPIC_Read :	
				TPIC_Power_On();									
		
				switch_Hold();
		
				ListView_MEAS("~ing...");
				ListView_RESULT(BUSY_RESULT, AUTO_STATE);
				Delay_ms(5);
				//while(1)
				//{
				ret=tpic_read_flow(index);
				//}
				TPIC_Power_Off();								
		
		break;	// ok

			default : ret = false;										break;	// ok
	}
	return ret;
	
}











