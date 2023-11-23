#include <stdint.h>
#include "LX_Semicon.h"

#define FLITFCTRL_COMMAND_BOOST_MODE		0x08

#define LXS_INIT 					1
#define LXS_FW_WRITE				2
#define LXS_FW_VER_READ				3	
#define LXS_PID_CHECK				4
#define LXS_VID_CHECK				5
#define LXS_BOOT_VER_CHECK			6
#define LXS_CORE_VER_CHECK			7
#define LXS_APP_VER_CHECK			8
#define LXS_PARA_VER_CHECK			9
#define LXS_CRC_CHECK				10
#define LXS_END						11

uint16_t pid_val;
uint16_t vid_val;
uint16_t boot_ver_val;
uint16_t core_ver_val;
uint16_t app_ver_val;
uint16_t para_ver_val;
uint16_t crc_val;

extern char MainResult_PID[50];
extern char MainResult_VID[50];
extern char MainResult_Boot[50];
extern char MainResult_Core[50];
extern char MainResult_App[50];
extern char MainResult_Para[50];
extern char MainResult_CRC[50];

typedef struct
{
	uint16_t bootV;
	uint16_t coreV;
	uint16_t appV;
	uint16_t paraV;
} t_Version;

typedef struct
{
	uint32_t addr;
	uint16_t size;
	uint8_t status;
	uint8_t cmd;
} t_FAPControl;

uint8_t slaveAddress = 0x10<<1;					// old 0x34

uint8_t MSB(uint16_t value)
{
	return value>>8 & 0xFF;
}

uint8_t LSB(uint16_t value)
{
	return value & 0x00FF;
}

uint8_t LXS_Module(uint16_t index)
{	
	uint8_t ret = false;
	dbgmsg("	LX_SEMICON ==> %d\n", func_module[index]);
	
	switch(func_module[index])
	{		
		case LXS_INIT:					ret = LXS_Initialize(index);			break;			// 1				
		case LXS_FW_WRITE: 				ret = LXS_FirmwareWrite(index);			break;			// 2
		case LXS_FW_VER_READ: 			ret = LXS_FirmwareVerRead(index);		break;			// 3
		case LXS_PID_CHECK: 			ret = LXS_PID_Check(index);		 		break;			// 4
		case LXS_VID_CHECK: 			ret = LXS_VID_Check(index);		 		break;			// 5
		case LXS_BOOT_VER_CHECK: 		ret = LXS_Boot_Ver_Check(index);	 	break;			// 6
		case LXS_CORE_VER_CHECK: 		ret = LXS_Core_Ver_Check(index);	 	break;			// 7
		case LXS_APP_VER_CHECK: 		ret = LXS_App_Ver_Check(index);			break;			// 8
		case LXS_PARA_VER_CHECK: 		ret = LXS_Para_Ver_Check(index);	 	break;			// 9
		case LXS_CRC_CHECK: 			ret = LXS_CRC_Check(index);				break;			// 10
		case LXS_END: 					ret = LXS_End_Flow(index);				break;			// 11
		default :				ret = false;						break;
	}
	return ret;
}


int LXS_Initialize(uint16_t index)
{		
	//EX_PORT INIT
	
//	EX_PORT(1,true);		// ~TAVCC	OFF
//	EX_PORT(2,false);		// EX_FN_RY	OFF
	dbgmsg("\n%s\n",__func__); 
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
	RYBD_6ON(true);
	Delay_ms(100);
	//--------------------Relay Board ON
	FuncRYControl(1,true,0);//GND Contact
    FuncRYControl(2,false,0);//GND - EEP WP Contact
	FuncRYControl(3,false,0);//SCL SDA
    FuncRYControl(4,true,0);//SWCLK, SWDIO - GND Contact
    FuncRYControl(5,false,0);//USB_M,P	//FuncRYControl(5,true,0);//USB_M,P
    FuncRYControl(6,true,0);// TP_SCL, TP_SDA
	FuncRYControl(7,true,200);// TP_INT, TP_EN
	EvccControl(1,false);// 1.8V Off
	EvccControl(2,true);// 3.3V ON (Level Shift Voltage)
	Delay_ms(100);
	DVDD_ON(true);
	Delay_ms(10);
	AVDD_ON(true);
	Delay_ms(250); 	//Delay_ms(500); );
	
	Delay_ms(500);
	ListView_MEAS("OK");
		
	switch_Hold();
	return true;
}

int LXS_FirmwareWrite(uint16_t index)
{
	dbgmsg("\n%s\n",__func__);
	
	if (RD_FW_WRITE == false)
    {
        ListView_MEAS("SKIP");
        return SKIP_RESULT;
    }
	
	ListView_MEAS("~ing...");
	ListView_RESULT(BUSY_RESULT, AUTO_STATE);
	//LJB		
	slaveAddress_Change();
	//LJB
	
	
	uint8_t bBoostMode = 0x00;
	if (!CheckDFUP())
	{
		dbgmsg("Cannot change download mode\n");
	}
	CheckDFUP();
	bBoostMode = CheckBoostMode();	
	if(DownloadProgram(bBoostMode))
	{
		ListView_MEAS("OK");		
		return true;
	}
	else
	{
		ListView_MEAS("NG");		
		return false;
	}
	
}

int LXS_FirmwareVerRead(uint16_t index)
{
	dbgmsg("\n%s\n",__func__);	
	//LJB	
//	slaveAddress = 0x10 <<1;		// Slave Addr will be change, After Write
	//LJB
	
	slaveAddress_Change();
	ListView_MEAS("~ing...");
	ListView_RESULT(BUSY_RESULT, AUTO_STATE);
	
	PostDownload();					// need reset time optimization
	ListView_MEAS("OK");	
	return true;
}

int LXS_PID_Check(uint16_t index)
{		
	dbgmsg("PID = %04X\n",pid_val);
	
	sprintf(MainResult_PID,"%02X.%02X / %02X.%02X",MSB(pid_val),LSB(pid_val),func_spec_hex[index][2],func_spec_hex[index][3]);
	ListView_MEAS(MainResult_PID);
	
	
	if(MSB(pid_val)==func_spec_hex[index][2] && LSB(pid_val)==func_spec_hex[index][3])
	{
		return true;
	}
	else
	{
		return false;
	}
}

int LXS_VID_Check(uint16_t index)
{	
	dbgmsg("VID = %04X\n",vid_val);
	
	sprintf(MainResult_VID,"%02X.%02X / %02X.%02X",MSB(vid_val),LSB(vid_val),func_spec_hex[index][2],func_spec_hex[index][3]);
	ListView_MEAS(MainResult_VID);
	
	if(MSB(vid_val)==func_spec_hex[index][2] && LSB(vid_val)==func_spec_hex[index][3])
	{
		return true;
	}
	else
	{
		return false;
	}
}


int LXS_Boot_Ver_Check(uint16_t index)
{		
	dbgmsg("bootV = %04X\n",boot_ver_val);
	
	sprintf(MainResult_Boot,"%02X.%02X / %02X.%02X",MSB(boot_ver_val),LSB(boot_ver_val),func_spec_hex[index][2],func_spec_hex[index][3]);
	ListView_MEAS(MainResult_Boot);
	
	if(MSB(boot_ver_val)==func_spec_hex[index][2] && LSB(boot_ver_val)==func_spec_hex[index][3])
	{
		return true;
	}
	else
	{
		return false;
	}
}

int LXS_Core_Ver_Check(uint16_t index)
{
	dbgmsg("coreV = %04X\n",core_ver_val);		
	
	sprintf(MainResult_Core,"%02X.%02X / %02X.%02X",MSB(core_ver_val),LSB(core_ver_val),func_spec_hex[index][2],func_spec_hex[index][3]);
	ListView_MEAS(MainResult_Core);
	
	if(MSB(core_ver_val)==func_spec_hex[index][2] && LSB(core_ver_val)==func_spec_hex[index][3])
	{
		return true;
	}
	else
	{
		return false;
	}
}

int LXS_App_Ver_Check(uint16_t index)
{
	dbgmsg("appV = %04X\n",app_ver_val);	
	
	sprintf(MainResult_App,"%02X.%02X / %02X.%02X",MSB(app_ver_val),LSB(app_ver_val),func_spec_hex[index][2],func_spec_hex[index][3]);
	ListView_MEAS(MainResult_App);
	
	if(MSB(app_ver_val)==func_spec_hex[index][2] && LSB(app_ver_val)==func_spec_hex[index][3])
	{
		return true;
	}
	else
	{
		return false;
	}	
}

int LXS_Para_Ver_Check(uint16_t index)
{
	dbgmsg("paraV = %04X\n",para_ver_val);	
	
	sprintf(MainResult_Para,"%02X.%02X / %02X.%02X",MSB(para_ver_val),LSB(para_ver_val),func_spec_hex[index][2],func_spec_hex[index][3]);
	ListView_MEAS(MainResult_Para);
	
	if(MSB(para_ver_val)==func_spec_hex[index][2] && LSB(para_ver_val)==func_spec_hex[index][3])
	{
		return true;
	}
	else
	{
		return false;
	}		
}

int LXS_CRC_Check(uint16_t index)
{	
	dbgmsg("crc = %04X\n",crc_val);	
	
	sprintf(MainResult_CRC,"%02X.%02X / %02X.%02X",MSB(crc_val),LSB(crc_val),func_spec_hex[index][2],func_spec_hex[index][3]);
	ListView_MEAS(MainResult_CRC);
	
	if(MSB(crc_val)==func_spec_hex[index][2] && LSB(crc_val)==func_spec_hex[index][3])
	{
		return true;
	}
	else
	{
		return false;
	}	
}

int LXS_End_Flow(uint16_t index)
{
	
	AVDD_ON(false);
	Delay_ms(50);
	DVDD_ON(false);
	Delay_ms(100);
	
	Discharge1_ON(true);
	Discharge2_ON(true);
	Delay_ms(500);  
	Discharge1_ON(false);
	Discharge2_ON(false);
	Delay_ms(50);
	
	ListView_MEAS("OK");	
	return true;
}



int I2C_Read(uint16_t address, uint8_t *read_buf, unsigned int read_len)
{
	s32 i;
	u8 TempData8;
	u8 count = 0;
	uint32_t REP_NUM = 5;

	RETRY_RD:		
	
	i2c1_start();
	
	if(i2c1_tx( (slaveAddress & 0xFE) ) != 0)
	{
		if( count++ < REP_NUM) goto 	RETRY_RD;
		i2c1_stop();
		dbgmsg("Slave Addr Write Err\n"); 
		return false; 
	}	
			
	TempData8 = address>>8; 		
	if(i2c1_tx( (u8)TempData8 ) != 0)
	{
		if( count++ < REP_NUM) goto 	RETRY_RD;
		i2c1_stop();
		dbgmsg("High Address write error: %0x0X\n", TempData8);
		return false; 
	}
	TempData8 = address & 0xFF; 		
	if(i2c1_tx( (u8)TempData8 ) != 0)
	{
		if( count++ < REP_NUM) goto 	RETRY_RD;
		i2c1_stop();
		dbgmsg("Low Address write error: %0x0X\n", TempData8);
		return false; 
	}	
	
	i2c1_stop(); 	
	
	i2c1_start();	
	
	if(i2c1_tx( (slaveAddress & 0xFE) | 0x01 ) != 0)
	{
		if( count++ < REP_NUM) goto 	RETRY_RD;  
		i2c1_stop();
		//dbgmsg("Slave Addr Read Err\n"); 
		return false; 
	}	
				
	for(i=0; i<read_len; i++)
	{
		if( i == (read_len-1) ) 	TempData8 = i2c1_rx(0);
		else     	           		TempData8 = i2c1_rx(1);
		*read_buf=TempData8;
		read_buf++;
	}
	read_buf = read_buf - read_len;			
	
	i2c1_stop();
	
	return true;
}

int I2C_Write(uint16_t address, uint8_t *write_buf, unsigned int write_len)
{
	s32 i;
	u8 TempData8;
	u8 count = 0;
	uint32_t REP_NUM = 5;

	RETRY_WD:	
	
	i2c1_start();	
	
	if(i2c1_tx( (slaveAddress & 0xFE) ) != 0)
	{
		if( count++ < REP_NUM) goto	RETRY_WD;
		i2c1_stop();
		dbgmsg("Slave Addr Write Err: %0x0X\n", slaveAddress & 0xFE); 
		return false; 
	}	
	
	TempData8 = address>>8; 		
	if(i2c1_tx( (u8)TempData8 ) != 0)
	{
		if( count++ < REP_NUM) goto 	RETRY_WD;
		i2c1_stop();
		dbgmsg("High Address write error: %0x0X\n", TempData8);
		return false; 
	}
	TempData8 = address & 0xFF; 		
	if(i2c1_tx( (u8)TempData8 ) != 0)
	{
		if( count++ < REP_NUM) goto 	RETRY_WD;
		i2c1_stop();
		dbgmsg("Low Address write error: %0x0X\n", TempData8);
		return false; 
	}
	
	for(i=0; i < write_len; i++)
	{
		TempData8 = *write_buf++; 
		
		if(i2c1_tx( (u8)TempData8 ) != 0)
		{
			if( count++ < REP_NUM) goto	RETRY_WD;
			i2c1_stop();
			//dbgmsg("Data wr err(%03d/%03d)\n",i+1,write_len);
			return false; 
		}			
	}	
	i2c1_stop();
	
	return true;
}


uint8_t slaveAddress_Change(void)
{	
	u8 count = 0;
	uint32_t REP_NUM = 10;
	uint8_t addr = 0;
	uint8_t found = 0;
	slaveAddress = 0x34 <<1;		// Slave Addr will be change, After Write
	
	RETRY:
	
	i2c1_start();	
	
	if(i2c1_tx( (slaveAddress & 0xFE) ) != 0)
	{
		if( count++ < REP_NUM) goto	RETRY;
		i2c1_stop();
		
		slaveAddress = 0x10 <<1;
	}	
	else		
	{
		i2c1_stop();
	}
	
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
	
	return 0;
}


uint8_t WaitForReadyStatus(void)
{
	uint16_t timeout = 0;
	uint8_t chVal = 0;
	while(chVal != 0xA0)
	{
		I2C_Read(0x0600, &chVal, sizeof(char));
		// time out exception code here
		if (timeout++ > 1000)
		{
			dbgmsg("WaitForReadyStatus Timeout\n");			
			return 0;
		}
	}
	
	return  1;
}

uint8_t CheckDFUP(void)
{
	t_Version version;
	
	uint8_t protocolName[14] = {0x00,};
	uint8_t write_buf[8] = {0x00,};
	I2C1_Init();

	//[0:7] : Protocol Name (SWIP or DFUP)
	//[8] : protocol minor ver
	//[9] : protocol major ver
	//[10:11] : VID
	//[12:13] : PID
	I2C_Read(0x0150, protocolName, 14);

	//dbgmsg("Protocol Name = %02x %02x %02x %02x \n",protocolName[0],protocolName[1],protocolName[2],protocolName[3]);
	dbgmsg("Protocol Name = %c %c %c %c \n",protocolName[0],protocolName[1],protocolName[2],protocolName[3]);		// Display in character
	dbgmsg("protocol minor ver = 0x%02x \n",protocolName[8]);
	dbgmsg("protocol major ver = 0x%02x \n",protocolName[9]);
	dbgmsg("VID = 0x%02x%02x \n",protocolName[10],protocolName[11]);
	dbgmsg("PID = 0x%02x%02x \n",protocolName[12],protocolName[13]);
	
	if (protocolName[0] == 'S' && protocolName[1] == 'W' && protocolName[2] == 'I' && protocolName[3] == 'P')
	{
		// read version
		//t_Version version;
		char chMode = 2;	 //Mode Set 2 (DFUP Mode)
		
		I2C_Read(0x0120, (uint8_t*)&version, sizeof(t_Version));
		dbgmsg("Current version :: Boot[%02x%02x] App[%02x%02x] Core[%02x%02x] para[%02x%02x]\n",\
			MSB(version.bootV), LSB(version.bootV), MSB(version.appV), LSB(version.appV), MSB(version.coreV), LSB(version.coreV), MSB(version.paraV), LSB(version.paraV));
		
		write_buf[0] = 2;
		I2C_Write(0x0610, write_buf, 1);
		
		return WaitForReadyStatus();
	}
	
//	I2C_Read(0x0120, (uint8_t*)&version, sizeof(t_Version));
//	dbgmsg("Current version :: Boot[%02x%02x] App[%02x%02x] Core[%02x%02x] para[%02x%02x]\n",\
//	MSB(version.bootV), LSB(version.bootV), MSB(version.appV), LSB(version.appV), MSB(version.coreV), LSB(version.coreV), MSB(version.paraV), LSB(version.paraV));
		
	return true;
}


//#define FW_APP_SIZE			112*1024
//#define APP_OFFSET			0x4000
//#define FW_APP_SIZE			128*1024
//#define APP_OFFSET			0x0000

#define	ADD_BUFFER_SIZE		128
#define ADD_BUFFER_SIZE_BOOST	4096
#define	SEND_BUFFER_SIZE		48
#define FLITFCTRL_COMMAND_FLASH_WRITE		0x03

uint8_t CheckBoostMode(void)
{
	dbgmsg("\n%s\n",__func__);
	
	t_FAPControl FAPCtrl;
	FAPCtrl.addr = 0x00;
	FAPCtrl.size = 0x00;
	FAPCtrl.status = 0x00;
	FAPCtrl.cmd = FLITFCTRL_COMMAND_BOOST_MODE;
	
	I2C_Write(0x1400, (uint8_t*)&FAPCtrl, sizeof(FAPCtrl)); // Write SRAM Buffer to FLASH
	
	if (!WaitForReadyStatus())
	{
		dbgmsg("Ready status failed !! FAPCtrl.addr = %x\n", FAPCtrl.addr);
		return 0;
	}
	I2C_Read(0x1400, (uint8_t*)&FAPCtrl, sizeof(FAPCtrl));
	return FAPCtrl.status;
}

int DownloadProgram(unsigned char bBoostMode)	//bool : True, False 
{	
	dbgmsg("\n%s\n",__func__);
	
	t_FAPControl FAPCtrl;
	uint8_t TEMP_BUF[1024] = {0x00,};
	
	uint8_t* fwBinary = (((uint8_t *)INPUTDATA_ADDRESS));
	
	int nSize = In_data.size;
	int nBufferOffset = 0;	
	
	long FW_APP_SIZE = (128*1024);
	uint16_t APP_OFFSET = 0x0000;

	dbgmsg("F/W size : %d bytes\n",  nSize);
	
	if(RD_FULL_WRITE)			// do something with this
	{
		dbgmsg("128kb Full Write Set, \tOffset = 0x%04X\n", APP_OFFSET);
	}
	else
	{
		APP_OFFSET = 0x4000;
		dbgmsg("112kb Write Set, \tOffset = 0x%04X\n", APP_OFFSET);		
		fwBinary = fwBinary + APP_OFFSET;
	}
	
	
	dbgmsg("FW Download Process Start\n");	
	dbgmsg("nSize = %d\n",nSize);
	dbgmsg("FW_APP_SIZE = %d\n",FW_APP_SIZE);
	
	if (nSize > FW_APP_SIZE)
	{
		dbgmsg("FW Size Error\n");
		dbgmsg("nSize = %x\n",nSize);
		dbgmsg("FW_APP_SIZE = %x\n",FW_APP_SIZE);
		return false;
	}
	
	int nIndex = 0;
	
	dbgmsg("Process #1 Boost mode = 0x%x\n",bBoostMode);	
	
	/*
	
	if (bBoostMode)
	{		
		////////////////////          Download Boost Mode          //////////////////////
		
		int nLastSize = ADD_BUFFER_SIZE_BOOST % SEND_BUFFER_SIZE;
		int nIndex2 = 0;
	dbgmsg("--------------------------------\n");
	dbgmsg("nLastSize: %d\n",  nLastSize);
	dbgmsg("ADD_BUFFER_SIZE_BOOST: %d\n",  ADD_BUFFER_SIZE_BOOST);
	dbgmsg("SEND_BUFFER_SIZE: %d\n",  SEND_BUFFER_SIZE);
	dbgmsg("APP_OFFSET: 0x%08X     %d\n\n",  APP_OFFSET, APP_OFFSET);
	uint32_t downloaded_size = 0;
	
		//for(int i = APP_OFFSET; i < nSize; i += ADD_BUFFER_SIZE_BOOST)		// 112 Kbyte Write
		for(int i = 0; i < nSize; i += ADD_BUFFER_SIZE_BOOST)		// 112 Kbyte Write
		{
			for(nIndex2 = 0; nIndex2 < ADD_BUFFER_SIZE_BOOST - nLastSize; nIndex2 += SEND_BUFFER_SIZE)		// Divide 4096 bytes into 48 bytes
			{				
				memcpy(TEMP_BUF, fwBinary + nIndex, SEND_BUFFER_SIZE);		// Add Data to SRAM buffer
				I2C_Write(0x6000 + nIndex2, TEMP_BUF, SEND_BUFFER_SIZE);
				nIndex += SEND_BUFFER_SIZE;
				
				downloaded_size += SEND_BUFFER_SIZE;
				dbgmsg("ndex2: %d\t dwnld_size: 0x%08X     %d\n",  nIndex2, downloaded_size, downloaded_size);
			}
				
			if (nLastSize != 0)
			{
				//memcpy(TEMP_BUF, fwBinary + nIndex, nLastSize);			// Add data to SRAM buffer
				//I2C_Write(0x6000 + nIndex2, TEMP_BUF, SEND_BUFFER_SIZE);
				//nIndex += SEND_BUFFER_SIZE;
				memcpy(TEMP_BUF, fwBinary + nIndex, nLastSize);			// Add data to SRAM buffer
				I2C_Write(0x6000 + nIndex2, TEMP_BUF, nLastSize);
				nIndex += nLastSize;
				
				downloaded_size += nLastSize;
				dbgmsg("ndex2: %d\t dwnld_Last size: 0x%08X     %d\n",  nIndex2, downloaded_size, downloaded_size);
			}
			dbgmsg("---- FAPCtrl.addr: 0x%08X -----\n", i + APP_OFFSET);
			FAPCtrl.addr = i + APP_OFFSET;
			FAPCtrl.size = ADD_BUFFER_SIZE_BOOST;
			FAPCtrl.status = 0;
			FAPCtrl.cmd = FLITFCTRL_COMMAND_FLASH_WRITE;
			I2C_Write(0x1400, (uint8_t*)&FAPCtrl, sizeof(FAPCtrl));		// Write SRAM Buffer to FLASH
			if ( !WaitForReadyStatus() )
			{
				dbgmsg("Ready Status Failed !! FAPCtrl.addr = %x", i);
				return false;
			}
		}
		dbgmsg("\n--------------------------------\n\n");
	/*}
	else
	{
		////////////////          Download Normal Mode          ///////////////
		
		for(int i = APP_OFFSET; i < nSize + APP_OFFSET; i += ADD_BUFFER_SIZE)		// 116 Kbyte Write
		{
			int nIdxIncrement = 16;
			for(int j = 0; j < 128; j += nIdxIncrement, nBufferOffset += nIdxIncrement)		// Divide 128 bytes into 16 bytes and send it 8 times
			{
				memcpy(TEMP_BUF, fwBinary + nBufferOffset, nIdxIncrement);			// Add Data to SRAM Buffer
				I2C_Write(0x6000 + j, TEMP_BUF, nIdxIncrement);
			}
			FAPCtrl.addr = i;
			FAPCtrl.size = 128;
			FAPCtrl.status = 0;
			FAPCtrl.cmd = FLITFCTRL_COMMAND_FLASH_WRITE;
			I2C_Write(0x1400, (uint8_t*)&FAPCtrl, sizeof(FAPCtrl));		// Write SRAM Buffer to FLASH
			if ( !WaitForReadyStatus() )
			{
				dbgmsg("Ready Status Failed !! FAPCtrl.addr = %x", i);
				return false;
			}
		}
	}
		*/
		
	//for(int i = APP_OFFSET; i < nSize + APP_OFFSET; i += ADD_BUFFER_SIZE)		// 116 Kbyte Write
	for(int i = APP_OFFSET; i < nSize ; i += ADD_BUFFER_SIZE)					// only for 128kb F/W
		{			
			int nIdxIncrement = 16;
			for(int j = 0; j < 128; j += nIdxIncrement, nBufferOffset += nIdxIncrement)		// Divide 128 bytes into 16 bytes and send it 8 times
			{
				memcpy(TEMP_BUF, fwBinary + nBufferOffset, nIdxIncrement);			// Add Data to SRAM Buffer
				I2C_Write(0x6000 + j, TEMP_BUF, nIdxIncrement);
			}
			FAPCtrl.addr = i;
			FAPCtrl.size = 128;
			FAPCtrl.status = 0;
			FAPCtrl.cmd = FLITFCTRL_COMMAND_FLASH_WRITE;
			
			
			I2C_Write(0x1400, (uint8_t*)&FAPCtrl, sizeof(FAPCtrl));		// Write SRAM Buffer to FLASH
			
			if ( !WaitForReadyStatus() )
			{
				dbgmsg("Ready Status Failed !! FAPCtrl.addr = %x", i);
				return false;
			}		
			
			dbgmsg("FAPCtrl.addr = %x\n", i);
			
		}		
	
	//return PostDownload();
	return true;
}

int PostDownload(void)
{
	char chMode = 0x11;	
	I2C_Write(0x0610, (uint8_t*)&chMode, sizeof(chMode));		// Request Watchdog Reset
	
	// Try about 3~4 times with delay
	// time out exception code here
		
//	EX_PORT(1,true);		// ~TAVCC	OFF
	// VIN, VTSP Power OFF
	AVDD_ON(false);
	Delay_ms(50);
	DVDD_ON(false);
	Delay_ms(100);
	
	Discharge1_ON(true);
	Discharge2_ON(true);
	Delay_ms(500);  
	Discharge1_ON(false);
	Discharge2_ON(false);
	Delay_ms(50);
	
	switch_Hold();
	
	Delay_ms(1000);
	
//	EX_PORT(1,false);		// ~TAVCC	ON
	// VIN, VTSP Power ON	
	DVDD_ON(true);
	Delay_ms(10);
	AVDD_ON(true);
	
	Delay_ms(250);	
	
		
	//get vid pid//
	uint8_t vid_pid_check[14] = {0x00,};	
	I2C_Read(0x0150, vid_pid_check, 14);
	vid_val = vid_pid_check[11]<<8 | vid_pid_check[10];
	pid_val = vid_pid_check[13]<<8 | vid_pid_check[12];
	//get vid pid//
	
	
	//struct initialize//
	t_Version LX_version;
	LX_version.bootV = 0xFFFF;
	LX_version.coreV = 0xFFFF;
	LX_version.appV = 0xFFFF;
	LX_version.paraV = 0xFFFF;	
	//struct initialize//	
	
	I2C_Read(0x0120, (uint8_t*)&LX_version, sizeof(LX_version));
	dbgmsg("After version :: Boot[%02x%02x] App[%02x%02x] Core[%02x%02x] para[%02x%02x]\n",\
			MSB(LX_version.bootV), LSB(LX_version.bootV), MSB(LX_version.appV), LSB(LX_version.appV), MSB(LX_version.coreV), LSB(LX_version.coreV), MSB(LX_version.paraV), LSB(LX_version.paraV));

	uint16_t nCRC = 0;
	I2C_Read(0x0140, (uint8_t*)&nCRC, sizeof(uint16_t));
	dbgmsg("After CRC :: %x %x\n", MSB(nCRC), LSB(nCRC));
		
	boot_ver_val=LX_version.bootV;
	core_ver_val=LX_version.coreV;
	app_ver_val=LX_version.appV;
	para_ver_val=LX_version.paraV;
	crc_val=LSB(nCRC)<<8 | MSB(nCRC);	

	return true;
}


/*******************************  Code Example
	
int main(void)
{
	if (!CheckDFUP()){
		dbgmsg("Cannot change download mode\n");
	}
	
	bool bBoostMode = CheckBoostMode();
	return DownloadProgram(bBoostMode);
}

*******************************/

