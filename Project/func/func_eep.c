#include "func_eep.h"

#define slA	0xA0

const uint8_t page_size = 32;

static uint8_t I2C_Read_Data(u8 *rData, u32 rSize, u16 Addr)
{ 
	s32 i;
	u8 TempData8, EEPROM_sID;
	u8 ID_TYPE;

	ID_TYPE = ((Addr >> 8) & 0xff);
	TempData8 = Addr & 0xff;

	if(ID_TYPE == 0)		EEPROM_sID = 0xA0;	//256 byte
	else if(ID_TYPE == 1)	EEPROM_sID = 0xA2;	//512
	else if(ID_TYPE == 2)	EEPROM_sID = 0xA4;	//768
	else if(ID_TYPE == 3)	EEPROM_sID = 0xA6;	//1024
	else if(ID_TYPE == 4)	EEPROM_sID = 0xA8;	//1280	
	else if(ID_TYPE == 5)	EEPROM_sID = 0xAA;	//1536
	else if(ID_TYPE == 6)	EEPROM_sID = 0xAC;	//1792
	else if(ID_TYPE == 7)	EEPROM_sID = 0xAE;	//2048
	
	i2c1_start(); 	Delay_us(100);
	
	if( i2c1_tx( EEPROM_sID & 0xFE ) != 0 )	{	i2c1_stop(); 	dbgmsg("eeprom_rd e1\n"); return false; }	Delay_us(100);
	if( i2c1_tx( TempData8 ) != 0 )			{	i2c1_stop(); 	dbgmsg("eeprom_rd e3\n"); return false; } 	Delay_us(100);
		
	i2c1_stop();	Delay_us(100);  
	i2c1_start();  	Delay_us(100);     

	if( i2c1_tx( (EEPROM_sID & 0xFE)|0x01 ) != 0 )	{	i2c1_stop(); dbgmsg("eeprom_rd e1\n"); return false; }	Delay_us(100);
	Delay_us(100);
	
	for(i=0; i<rSize; i++)
    {		
		if(i==(rSize-1)) TempData8 = i2c1_rx(0);		
		else			 TempData8 = i2c1_rx(1);			
		*rData=TempData8;
		rData++;
		
        //if(rSize < 50)   	
		//dbgmsg("0x%02X ",TempData8);
		dbgmsg("%02X, ",TempData8);
	}
	rData = rData - rSize;
	
	dbgmsg("\n");
    i2c1_stop();
	
	return true;
}

static uint8_t I2C_Write_Data(u8 *pData, u32 rSize, u16 Addr)
{
	s32 i;
	u8 TempData8,EEPROM_sID;
	u8 ID_TYPE;
	
	dbgmsg("Addr : %d\n",Addr);
	
	ID_TYPE = ((Addr >> 8) & 0xff);
	TempData8 = Addr & 0xff;

	if(ID_TYPE == 0)		EEPROM_sID = 0xA0;	//256 byte
	else if(ID_TYPE == 1)	EEPROM_sID = 0xA2;	//512
	else if(ID_TYPE == 2)	EEPROM_sID = 0xA4;	//768
	else if(ID_TYPE == 3)	EEPROM_sID = 0xA6;	//1024
	else if(ID_TYPE == 4)	EEPROM_sID = 0xA8;	//1280	
	else if(ID_TYPE == 5)	EEPROM_sID = 0xAA;	//1536
	else if(ID_TYPE == 6)	EEPROM_sID = 0xAC;	//1792
	else if(ID_TYPE == 7)	EEPROM_sID = 0xAE;	//2048
	
	dbgmsg("ID_TYPE : %d EEPROM_sID : %X Addr : %X\n",ID_TYPE, EEPROM_sID, Addr);
	
	i2c1_start(); 	
	Delay_us(100);
	
	if( i2c1_tx( EEPROM_sID & 0xFE ) != 0 ) {	i2c1_stop(); 	dbgmsg("eeprom_page wr error\n"); return false; }	Delay_us(100);
	if( i2c1_tx( TempData8 ) != 0 )			{	i2c1_stop(); 	dbgmsg("eeprom_address wr error\n");return false; } 	Delay_us(100);
	
	for(i=0; i<rSize; i++)
    {
		TempData8 = *pData++; 
		if(i2c1_tx( TempData8 ) != 0){	i2c1_stop();  dbgmsg("eeprom_data wr error 2\n"); return false; }	
		//dbgmsg("0x%02X ",TempData8);
		dbgmsg("%02X, ",TempData8);
	}    
	dbgmsg("\n");    
	
	i2c1_stop();
	
	return true;
}

static uint8_t M24Cxx_Write_Data(u8 *pData, u32 rSize, u16 Addr)
{
	// Applicable to M24C64, M24C32 IC
	
	s32 i;
	u8 TempData8,EEPROM_sID;
	u8 ID_TYPE;
	
	dbgmsg("Addr : %d\n",Addr);
	
	uint8_t ucAddrHigh = ((Addr & 0xFF00) >> 8);
	uint8_t ucAddrLow = ((Addr & 0x00FF) >> 0);
	
	ID_TYPE = ((Addr >> 8) & 0xff);
	TempData8 = Addr & 0xff;

	EEPROM_sID = 0xA0;
	
	dbgmsg("ID_TYPE : %d EEPROM_sID : %X Addr : %X\n",ID_TYPE, EEPROM_sID, Addr);
	
	i2c1_start(); 	
	Delay_us(100);
	
	if( i2c1_tx( EEPROM_sID & 0xFE ) != 0 ) {	i2c1_stop(); 	dbgmsg("eeprom device address wr error\n"); return false; }	Delay_us(100);
	if( i2c1_tx( ucAddrHigh ) != 0 )			{	i2c1_stop(); 	dbgmsg("eeprom High_address wr error\n");return false; } 	Delay_us(100);
	if( i2c1_tx( ucAddrLow ) != 0 )			{	i2c1_stop(); 	dbgmsg("eeprom Low_address wr error\n");return false; } 	Delay_us(100);
	
	for(i=0; i<rSize; i++)
    {
		TempData8 = *pData++; 
		if(i2c1_tx( TempData8 ) != 0){	i2c1_stop();  dbgmsg("eeprom_data wr error 2\n"); return false; }	
		dbgmsg("%02X, ",TempData8);
	}    
	dbgmsg("\n");    
	
	i2c1_stop();
	
	return true;
}

static uint8_t M24Cxx_Read_Data(u8 *rData, u32 rSize, u16 Addr)
{ 
	s32 i;
	u8 TempData8, EEPROM_sID;
	u8 ID_TYPE;

	ID_TYPE = ((Addr >> 8) & 0xff);
	
	EEPROM_sID = 0xA0;
	
	uint8_t ucAddrHigh = ((Addr & 0xFF00) >> 8);
	uint8_t ucAddrLow = ((Addr & 0x00FF) >> 0);
	
	i2c1_start(); 	Delay_us(100);
	
	if( i2c1_tx( EEPROM_sID & 0xFE ) != 0 )	{	i2c1_stop(); 	dbgmsg("eeprom_rd e1\n"); return false; }	Delay_us(100);
	if( i2c1_tx( ucAddrHigh ) != 0 )			{	i2c1_stop(); 	dbgmsg("eeprom High_address rd error\n"); return false; } 	Delay_us(100);
	if( i2c1_tx( ucAddrLow ) != 0 )			{	i2c1_stop(); 	dbgmsg("eeprom Low_address rd error\n"); return false; } 	Delay_us(100);
		
	i2c1_stop();	Delay_us(100);  
	i2c1_start();  	Delay_us(100);     

	if( i2c1_tx( (EEPROM_sID & 0xFE)|0x01 ) != 0 )	{	i2c1_stop(); dbgmsg("eeprom_rd e1\n"); return false; }	Delay_us(100);
	Delay_us(100);
	
	for(i=0; i<rSize; i++)
    {		
		if(i==(rSize-1)) TempData8 = i2c1_rx(0);		
		else			 TempData8 = i2c1_rx(1);			
		rData[i]=TempData8;		
		
        dbgmsg("%02X, ",TempData8);
	}	
	
	dbgmsg("\n");
    i2c1_stop();
	
	return true;
}

/*static uint8_t I2C_Read_Data(u8 *rData, u32 rSize, u16 Addr)
{ 
	u16 i;
	u8 TempData8;
	u8 ID_TYPE;
	u8 AddrHigh = (Addr>>8) & 0x00FF;
	u8 AddrLow = (Addr & 0x00FF);

	ID_TYPE = ((Addr >> 8) & 0xff);
	TempData8 = Addr & 0xff;

	dbgmsg("EEPROM_sID : %X AddrHigh : %02X  AddrLow: %02X  size: %d\n", slA, AddrHigh, AddrLow, rSize);
	
	i2c1_start(); 	Delay_us(100);
	
	if( i2c1_tx( slA & 0xFE ) != 0 )	{	i2c1_stop(); 	dbgmsg("eeprom_rd e1\n"); return false; }	Delay_us(100);
	if( i2c1_tx( AddrHigh ) != 0 )		{	i2c1_stop(); 	dbgmsg("eeprom_rd e2\n"); return false; } 	Delay_us(100);
	if( i2c1_tx( AddrLow ) != 0 )		{	i2c1_stop(); 	dbgmsg("eeprom_rd e3\n"); return false; } 	Delay_us(100);
		
	i2c1_stop();	Delay_us(100);  
	i2c1_start();  	Delay_us(100);     

	if( i2c1_tx( (slA & 0xFE)|0x01 ) != 0 )	{	i2c1_stop(); dbgmsg("eeprom_rd e4\n"); return false; }	Delay_us(100);
	Delay_us(100);
	
	for(i=0; i<rSize; i++)
    {		
		//if(i==(rSize-1)) TempData8 = i2c1_rx(0);		
		//else			 TempData8 = i2c1_rx(1);

		TempData8 = i2c1_rx(1);
		*rData=TempData8;
		rData++;
		
        //if(rSize < 50)   	
		dbgmsg("0x%02X ",TempData8);
	}
	//rData = rData - rSize;
	
	dbgmsg("\n\n");
    i2c1_stop();
	Delay_us(100);
	return true;
}*/

/*static uint8_t I2C_Write_Data(u8 *pData, u32 rSize, u16 Addr)
{
	s32 i;
	u8 TempData8 = 0;
	u8 AddrHigh = (Addr>>8) & 0x00FF;
	u8 AddrLow = (Addr & 0x00FF);
		
	dbgmsg("Addr : %d\t EEPROM_sID : %X AddrHigh : %02X  AddrLow: %02X\n", Addr, slA, AddrHigh, AddrLow);
	
	i2c1_start(); 	
	Delay_us(100);
	
	if( i2c1_tx( slA & 0xFE ) != 0 ) {	i2c1_stop(); 	dbgmsg("eeprom_page wr error\n"); return false; }	
	Delay_us(100);
	if( i2c1_tx( AddrHigh ) != 0 )			{	i2c1_stop(); 	dbgmsg("AddrHigh wr error\n");return false; } 	
	Delay_us(100);
	if( i2c1_tx( AddrLow ) != 0 )			{	i2c1_stop(); 	dbgmsg("AddrLow wr error\n");return false; } 	
	Delay_us(100);
	
	for(i=0; i<rSize; i++)
    {
		TempData8 = *pData++; 
		if(i2c1_tx( TempData8 ) != 0){	i2c1_stop();  dbgmsg("eeprom_data wr error, byte : \n", i); return false; }	
		//dbgmsg("0x%02X ",TempData8);
	}    
	dbgmsg("\n\n");    
	
	i2c1_stop();	
	//Delay_us(100);
	//WP_ON(true);
	//FuncRYControl(7,true,250);
	
	return true;
}*/

void EEP_Power_On(void)
{
	FuncRYControl(1,true,0);//GND Contact
    FuncRYControl(2,true,0);//GND - EEP WP Contact
	FuncRYControl(3,true,0);//SCL SDA
    FuncRYControl(4,true,0);//EEP_WP1, EEP_WP2 - GND Contact
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
void EEP_Power_Off(void)
{
	EvccControl(1,false); //
	EvccControl(2,false);
	Delay_ms(10);
	
	RYBD_6ON(false);	// external 12V off
	AVDD_ON(false); //VIN
	Delay_ms(50);
	DVDD_ON(false); //VTSP
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

uint8_t eep_init(uint16_t index)
{
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
	Delay_ms(100);
	//------------------------------------
	ListView_MEAS("OK");
	Delay_ms(200);
	
	return true;
}

uint8_t eep_write_flow(uint16_t index)
{
	uint8_t RET = 0;
	uint16_t page = 0;
	uint16_t etc = 0;
	uint16_t page_add = 0;
	char text[50];
	
	Address_Search_Function();
	
	int EEPROM_SIZE = In_eep.size;
	
	page = EEPROM_SIZE/16;
	etc = EEPROM_SIZE%16;
	
	dbgmsg("Page : %d, etc : %d\n",page,etc);
	for(uint16_t i=0; i < page; i++,page_add = page_add + 16)
	{
		sprintf(text, "Write %04d / %04d", i, page);
		ListView_MEAS(text);
		
		int retry = 0;
		RET = false;
		while (1)
		{
			retry++;
			RET = M24Cxx_Write_Data(&INPUT_EEP[page_add],16,page_add);
			
			if ( (retry >= 4) ||  (RET == true) )
				break;
		}
		
		if(RET == false)
		{
			ListView_MEAS("I2C Error");				
			return false;
		}
		//dbgmsg("i : %d \n", i );
		Delay_ms(70);			
	}
	if(etc)
	{
		sprintf(text, "Write %04d / %04d", page+1, page+1);
		ListView_MEAS(text);
		RET = I2C_Write_Data(&INPUT_EEP[page_add],etc,page_add);
		if(RET == false)
		{
			ListView_MEAS("I2C Error");				
			return false;
		}
		Delay_ms(70);	
	}
	return true;
}

/*
uint8_t eep_write_flow(uint16_t index)
{
	uint8_t RET = 0;
	uint16_t total_page = 0;
	uint16_t etc = 0;
	uint16_t page_add = 0;
	char text[50];
	uint8_t keypress = 0;
	
	int EEPROM_SIZE = In_eep.size;
	
	total_page = EEPROM_SIZE / page_size;
	etc = EEPROM_SIZE % page_size;
	
	dbgmsg("\n\nEEPROM Write Start\n");
	dbgmsg("total_page : %d, fragment data size : %d\n",total_page, etc);

	//{	
		//dbgmsg("Press enter to start EEPROM write\n");	
		//while(1)
		//{
			//keypress = KeyScan();
			//if (keypress == ST1_ON)	break;
		//}
	//}
	
	for(page_add = 0; page_add < EEPROM_SIZE; page_add = page_add + page_size)
	{
		sprintf(text, "Write %04d / %04d", page_add, total_page);
		ListView_MEAS(text);
		RET = I2C_Write_Data(&INPUT_EEP[page_add], page_size, page_add);
		Delay_ms(20);
		if(RET == false)
		{
			ListView_MEAS("I2C Error");				
			return false;
		}
		//dbgmsg("i : %d \n", i );		
	}
	dbgmsg("Write END page_add : %d\n\n", page_add);
	if(etc)
	{
		sprintf(text, "Write %04d / %04d", total_page+1, total_page+1);
		ListView_MEAS(text);
		RET = I2C_Write_Data(&INPUT_EEP[page_add],etc,page_add);
		if(RET == false)
		{
			ListView_MEAS("I2C Error");				
			return false;
		}
		Delay_ms(70);	
	}
	return true;
}*/


uint8_t eep_read_flow(uint16_t index)
{
	uint8_t isVerifyErr = false;
	int EEPROM_SIZE = In_eep.size;
	uint16_t page = 0;
	uint16_t etc = 0;
	uint16_t page_add = 0;
	uint8_t RET = 0;
	uint8_t Buff[16]={0};
	uint8_t ReadData[2048] = {0,};
	uint32_t spec = (func_spec_hex[index][0] << 24) | (func_spec_hex[index][1] << 16) | (func_spec_hex[index][2] << 8) | (func_spec_hex[index][3] << 0);
	
	char text[50];

	uint32_t CRC_TargetTemp = 0;
	InitCRC32();
	CRC_TargetTemp = Get_CRC32Target();
	
	page = EEPROM_SIZE/16;
	etc = EEPROM_SIZE%16;
	dbgmsg("\n");
	for(uint16_t i=0; i < page; i++,page_add = page_add + 16)
	{
		sprintf(text, "Read %04d / %04d", i+1, page);
		ListView_MEAS(text);		
		ListView_RESULT(BUSY_RESULT, AUTO_STATE);
		
		dbgmsg("page: %d=,",page_add);
		RET = M24Cxx_Read_Data(Buff, 16, page_add);
		if(RET == false)
		{
			ListView_MEAS("I2C Error");				
			return false;
		}
		for(uint16_t j=0;j<16;j++)
		{
			if (INPUT_EEP[page_add+j] != Buff[j])
			{
				dbgmsg("error addr: %d\n", j);
				isVerifyErr = true;
			}
			CRC_TargetTemp = Get_CRC32(Buff[j], CRC_TargetTemp);
		}
		Delay_ms(70);	
	}
	if(etc)
	{
		sprintf(text, "Read %04d / %04d", page, page);
		ListView_MEAS(text);
		RET = I2C_Read_Data(Buff, etc, page_add);
		if(RET == false)
		{
			ListView_MEAS("I2C Error");				
			return false;
		}
		for(uint16_t j=0;j<etc;j++)
		{
			if (INPUT_EEP[page_add+j] != Buff[j])
			{
				dbgmsg("error addr: %d\n", j);
				isVerifyErr = true;
			}
			CRC_TargetTemp = Get_CRC32(Buff[j], CRC_TargetTemp);
		}
	Delay_ms(70);	
	}
	dbgmsg("CRC : %08X / SPEC : %08X\n", CRC_TargetTemp, spec);
	/*
	for(uint16_t i=0; i<EEPROM_SIZE; ++i)
	{
		CRC_TargetTemp = Get_CRC32(ReadData[i], CRC_TargetTemp);
	}
	dbgmsg("CRC : %08X / SPEC : %08X\n", CRC_TargetTemp, spec);
	for(uint16_t i=0; i<EEPROM_SIZE; ++i)
	{
		if(INPUT_EEP[i] != ReadData[i])
		{
			dbgmsg("error addr: %d\n", i);
			isVerifyErr = true;
		}
	}*/
	if (isVerifyErr == true)
	{
		ListView_MEAS("Different Data");
		RET = false;
	}
	else if(CRC_TargetTemp == spec)
	{
		sprintf(text,"%08X / %08X",CRC_TargetTemp,spec);
		ListView_MEAS(text);
		dbgmsg("CRC : %08X / SPEC : %08X\n", CRC_TargetTemp, spec);
		RET = true;
	}
	else
	{
		ListView_MEAS("CRC NG");
		RET = false;
	}
	
	dbgmsg("EEPROM Verify End\n");
	return RET;
}

/*
uint8_t eep_read_flow(uint16_t index)
{
	uint8_t isVerifyErr = false;
	int EEPROM_SIZE = In_eep.size;
	uint16_t page = 0;
	uint16_t etc = 0;
	uint16_t page_add = 0;
	uint8_t RET = 0;
	uint8_t Buff[32]={0};
	uint8_t ReadData[8192] = {0,};
	uint32_t spec = (func_spec_hex[index][0] << 24) | (func_spec_hex[index][1] << 16) | (func_spec_hex[index][2] << 8) | (func_spec_hex[index][3] << 0);
	
	char text[50];
	u8 TempData8;
	u8 ID_TYPE;
	u8 AddrHigh = 0;
	u8 AddrLow = 0;
	u16 bytecount = 0;
	uint32_t CRC_TargetTemp = 0;
	InitCRC32();
	CRC_TargetTemp = Get_CRC32Target();
	
	page = EEPROM_SIZE / page_size;
	etc = EEPROM_SIZE % page_size;	
	page_add = 0;
	AddrHigh = 0;
	AddrLow = 0;
	
	dbgmsg("\n\nEEPROM Read Start\n");
	dbgmsg("total_page : %d, fragment data size : %d\n", page, etc);
	
	
	dbgmsg("EEPROM_sID : %X AddrHigh : %02X  AddrLow: %02X  size: %d\n", slA, AddrHigh, AddrLow, EEPROM_SIZE);
	
	i2c1_start(); 	Delay_us(100);
	
	if( i2c1_tx( slA & 0xFE ) != 0 )	{	i2c1_stop(); 	dbgmsg("eeprom_rd e1\n"); return false; }	Delay_us(100);
	if( i2c1_tx( AddrHigh ) != 0 )		{	i2c1_stop(); 	dbgmsg("eeprom_rd e2\n"); return false; } 	Delay_us(100);
	if( i2c1_tx( AddrLow ) != 0 )		{	i2c1_stop(); 	dbgmsg("eeprom_rd e3\n"); return false; } 	Delay_us(100);
		
	i2c1_stop();	Delay_us(100);  
	i2c1_start();  	Delay_us(100);     

	if( i2c1_tx( (slA & 0xFE)|0x01 ) != 0 )	{	i2c1_stop(); dbgmsg("eeprom_rd e4\n"); return false; }	Delay_us(100);
	Delay_us(100);
	
	for(bytecount = 0; bytecount < EEPROM_SIZE; bytecount++)
    {		
		//if(i==(rSize-1)) TempData8 = i2c1_rx(0);		
		//else			 TempData8 = i2c1_rx(1);

		TempData8 = i2c1_rx(1);
		ReadData[bytecount]=TempData8;		
		
		if (bytecount % 100 == 0)	dbgmsg("%d bytes read\n", bytecount);
		if ( ReadData[bytecount] != INPUT_EEP[bytecount] )
		{
			dbgmsg("error location: %d\t ReadData: 0x%02X\t INPUT_EEP: 0x%02X\n", bytecount, ReadData[bytecount], INPUT_EEP[bytecount]);
			break;
		}
		Delay_us(5);
	}
	dbgmsg("%d bytes read\n", bytecount);
	//rData = rData - rSize;
	
	dbgmsg("\n\n");
    i2c1_stop();
	Delay_us(100);
	
	for(uint16_t i=0; i<EEPROM_SIZE; ++i)
	{
		CRC_TargetTemp = Get_CRC32(ReadData[i], CRC_TargetTemp);
	}
	dbgmsg("CRC : %08X / SPEC : %08X\n", CRC_TargetTemp, spec);
	for(uint16_t i=0; i<EEPROM_SIZE; ++i)
	{
		if(INPUT_EEP[i] != ReadData[i])
		{
			isVerifyErr = true;
			dbgmsg("verify err memory location: %d\n", i);
		}
	}
	if(isVerifyErr == true)
	{
		ListView_MEAS("Different Data");
		RET = false;
	}
	else if(CRC_TargetTemp == spec)
	{
		sprintf(text,"%08X / %08X",CRC_TargetTemp,spec);
		ListView_MEAS(text);
		dbgmsg("CRC : %08X / SPEC : %08X\n", CRC_TargetTemp, spec);
		RET = true;
	}
	else
	{
		ListView_MEAS("CRC NG");
		RET = false;
	}
	return RET;
}*/

typedef enum
{
	EEP_INIT = 1,
	EEP_Write,
	EEP_Read
}EEP_TYPE;

uint8_t EEP_Function(uint16_t index)
{
	uint8_t ret = false;
	dbgmsg("   EEPROM ==> %02d\n", func_module[index]);
	dbgmsg("index No : %d\n", index);
	int kscan = 0;
		switch(func_module[index]) 
	{
			case EEP_INIT :		ret = eep_init(index);			break;	// ok
			case EEP_Write :	
				EEP_Power_On();									
			
				ListView_MEAS("~ing...");
				ListView_RESULT(BUSY_RESULT, AUTO_STATE);
			
				switch_Hold();
			//GPIO_ResetBits(GPIOB,GPIO_Pin_2);
				
				ret = eep_write_flow(index);
							
				//GPIO_SetBits(GPIOB,GPIO_Pin_2);
				EEP_Power_Off();
				if(ret == true)ListView_MEAS("OK");
				else ListView_MEAS("Fail");				
			
			break;	// ok
			case EEP_Read :
				ListView_MEAS("~ing...");
				ListView_RESULT(BUSY_RESULT, AUTO_STATE);
						
				EEP_Power_On();							
			
				switch_Hold();
			
				ret = eep_read_flow(index);		
				
				//EEP_Power_Off();		// For USB power continuity
				
				dbgmsg("EEPROM Verify end\n");
				break;	// ok
			default : ret = false;										break;	// ok
	}
	return ret;
}


