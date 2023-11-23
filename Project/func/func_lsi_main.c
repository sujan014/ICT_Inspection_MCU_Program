#include "func_lsi_main.h"
#ifdef LSI_IC

#define SPIMODE	1
#define I2CMODE 0

char IC_Type[16];
static uint8_t Comm_Type = I2CMODE;
static const uint8_t SlaveAdd = 0x90;

static uint8_t DeviceID1 = 0x37;
static uint8_t DeviceID2 = 0x91;

uint32_t ChunkSignature = 0;
uint32_t ChunkAddr[10] = {0};
uint32_t ChunkSize[10] = {0};
uint8_t *ChunkData[10] = {0};

uint8_t rx_temp[5000];
uint8_t tx_temp[5000];

uint32_t TX_NUM = 0, RX_NUM = 0, FO_NUM = 0;

int16_t CM_OFFSET[TX_MAX * RX_MAX]	= {0};
int16_t CS_OFFSET_TX[TX_MAX]		= {0};
int16_t CS_OFFSET_RX[RX_MAX]		= {0};
int16_t CF_OFFSET[FO_MAX]			= {0};

int16_t TX_SHORT_GND[TX_MAX]		= {0};
int16_t RX_SHORT_GND[RX_MAX]		= {0};
int16_t TX_SHORT_TX[TX_MAX]			= {0};
int16_t RX_SHORT_RX[RX_MAX]			= {0};
int16_t TX_SHORT_RX[TX_MAX]			= {0};
int16_t RX_SHORT_TX[RX_MAX]			= {0};

int16_t TX_SHORT_GND_GAP[TX_MAX]	= {0};
int16_t RX_SHORT_GND_GAP[RX_MAX]	= {0};
int16_t TX_SHORT_TX_GAP[TX_MAX]		= {0};
int16_t RX_SHORT_RX_GAP[RX_MAX]		= {0};
int16_t TX_SHORT_RX_GAP[TX_MAX]		= {0};
int16_t RX_SHORT_TX_GAP[RX_MAX]		= {0};

int16_t JIT_CMR_AVR[TX_MAX*RX_MAX]	= {0};	//cmr avr
int16_t JIT_CMR_MM[TX_MAX*RX_MAX]	= {0};	//cmr mm
int16_t JIT_CMR_GAP[TX_MAX*RX_MAX]	= {0};	//cmr gap
int16_t JIT_CSR_TX_AVR[TX_MAX]		= {0};	//csr tx
int16_t JIT_CSR_RX_AVR[RX_MAX]		= {0};	//csr rx
int16_t JIT_CSR_TX_GAP[TX_MAX]		= {0};	//csr tx gap
int16_t JIT_CSR_RX_GAP[RX_MAX]		= {0};	//csr rx gap


void clear_data(void)
{
	memset(tx_temp, 0x00, 5000 * sizeof(uint8_t));	
	memset(rx_temp, 0x00, 5000 * sizeof(uint8_t));	
	memset(panel_data, 0x00, 30 * 900 * sizeof(int16_t));	
	memset(ErrorNodeCount, 0x00, 50 * sizeof(int));	
	memset(CM_OFFSET, 0x00, TX_MAX * RX_MAX * sizeof(int));	
	memset(CS_OFFSET_TX, 0x00, TX_MAX * sizeof(int));	
	memset(CS_OFFSET_RX, 0x00, RX_MAX * sizeof(int));	
	memset(CF_OFFSET, 0x00, FO_MAX * sizeof(int));	
	memset(TX_SHORT_GND, 0x00, TX_MAX * sizeof(int));	
	memset(RX_SHORT_GND, 0x00, RX_MAX * sizeof(int));	
	memset(TX_SHORT_TX, 0x00, TX_MAX * sizeof(int));	
	memset(RX_SHORT_RX, 0x00, RX_MAX * sizeof(int));	
	memset(TX_SHORT_RX, 0x00, TX_MAX * sizeof(int));	
	memset(RX_SHORT_TX, 0x00, RX_MAX * sizeof(int));	
	memset(TX_SHORT_GND_GAP, 0x00, TX_MAX * sizeof(int));	
	memset(RX_SHORT_GND_GAP, 0x00, RX_MAX * sizeof(int));	
	memset(TX_SHORT_TX_GAP, 0x00, TX_MAX * sizeof(int));	
	memset(RX_SHORT_RX_GAP, 0x00, RX_MAX * sizeof(int));	
	memset(TX_SHORT_RX_GAP, 0x00, TX_MAX * sizeof(int));	
	memset(RX_SHORT_TX_GAP, 0x00, RX_MAX * sizeof(int));	
	memset(JIT_CMR_AVR, 0x00, TX_MAX * RX_MAX * sizeof(int));	
	memset(JIT_CMR_MM, 0x00, TX_MAX * RX_MAX * sizeof(int));	
	memset(JIT_CMR_GAP, 0x00, TX_MAX * RX_MAX * sizeof(int));
	memset(JIT_CSR_TX_AVR, 0x00, TX_MAX * sizeof(int));	
	memset(JIT_CSR_RX_AVR, 0x00, RX_MAX * sizeof(int));	
	memset(JIT_CSR_TX_GAP, 0x00, TX_MAX * sizeof(int));	
	memset(JIT_CSR_RX_GAP, 0x00, RX_MAX * sizeof(int));	
}

///////////////////////////////////////////////////////
/////     		S6SY771_initial				 //////////
///////////////////////////////////////////////////////
#define TINTX_READ	GPIO_ReadInputDataBit(GPIOB,   GPIO_Pin_10)
#define TA_PIN(x)	x ? GPIO_SetBits(GPIOB,GPIO_Pin_11) : GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define RST_PIN(x)	x ? GPIO_SetBits(GPIOB,GPIO_Pin_11) : GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define INTR2_READ  GPIO_ReadInputDataBit(GPIOB,   GPIO_Pin_3)
static void gpio_init(uint16_t index)
{
	{	// TINTX Pin
		GPIO_InitTypeDef    GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
		//GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	
	{	// TA Pin
		GPIO_InitTypeDef    GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
		//GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_11;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}		
	{	// INTR2 Pin    200512 MCH
		GPIO_InitTypeDef    GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_3;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
}

static uint8_t LSI_Initial(uint16_t index)
{
	const char com_str[2][5]={"I2C","SPI"};
	char text[50];  
	dbgmsg("%s Start\n",__func__);
		
	gpio_init(index);

	TA_PIN(1);

	for(int i = 0; i <16; i++)	IC_Type[i]   =	func_spec_ascii[index][i];
	
	// I2C : SPEC_DEC=0
	// SPI : SPEC_DEC=1
	Comm_Type = func_spec_dec[index];
	if(Comm_Type == SPIMODE)		SPI1_Init();
	else if(Comm_Type == I2CMODE)	I2C1_Init();
	
	if(strcmp(IC_Type,"SY79A")==0 )			{	DeviceID1 =	 0x37;	DeviceID2 =  0x91;	}
	else if(strcmp(IC_Type,"SY792")==0 )	{	DeviceID1 =	 0x37;	DeviceID2 =  0x92;	}
	else if(strcmp(IC_Type,"SY771")==0 )	{	DeviceID1 =	 0x37;	DeviceID2 =  0x71;	}
	else if(strcmp(IC_Type,"SY761")==0)		{	DeviceID1 =	 0x37;	DeviceID2 =  0x61;	}
	else									{	sprintf(text, "NG(IC:%s / Com:%s)",IC_Type, com_str[Comm_Type]);	ListView_MEAS(text);return false;	}
	
	dbgmsg("IC_Type : %s\n",IC_Type);	
			
	clear_data();
	
	EvccControl(1, false);
	EvccControl(2, false);
	
	FuncRYControl(0xff,false,250);
	FuncRYControl(1,true,0);	// dvdd - gnd
	FuncRYControl(2,true,0);	// avdd - gnd
	FuncRYControl(3,true,0);	// scl - sda
	FuncRYControl(4,true,0);	// int - ta(rst)
	FuncRYControl(5,true,0); 	// sck - miso
	FuncRYControl(6,true,250);	// mosi - cs

	EvccControl(1, true);Delay_ms(100);
	EvccControl(2, true);Delay_ms(100);
	
	dbgmsg("%s Pass\n",__func__);	
	memset(ErrorNodeCount, 0x00, sizeof(int16_t)*50);
	memset(panel_data, 0x00, sizeof(int16_t)*50*1800);

	sprintf(text, "OK( IC:%s / Com:%s )",IC_Type,com_str[Comm_Type]);
	
	ListView_MEAS(text);
	
	return true;
}

static uint8_t LSI_Version(uint16_t index)
{
	uint8_t device_id[10];
	char text[100];  
	
	if(LSI_Read_Data(device_id,3,READ_DEVICE_ID) == false) 
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}

	dbgmsg("%02X%02X%02X\n", device_id[0], device_id[1], device_id[2]);
	
	if( (!(device_id[1] == DeviceID1 && device_id[2] == DeviceID2)) )
	{				
		sprintf(text, "%02X%02X/%02X%02X",device_id[1],device_id[2],DeviceID1,DeviceID2);
		ListView_MEAS(text);
		return false;
	}

	uint8_t image_ver[10];
	if(LSI_Read_Data(image_ver, 8, READ_IMAGE_VER) == false) 
	{
		ListView_MEAS("Comm. Fail2");
		return false;
	}		
	
	sprintf(text,"%02X.%02X.%02X.%02X/%02X.%02X.%02X.%02X",image_ver[0],image_ver[1],image_ver[2],image_ver[3],func_spec_hex[index][0],func_spec_hex[index][1],func_spec_hex[index][2],func_spec_hex[index][3]);
	ListView_MEAS(text);	
	dbgmsg("%s\n",text);
	
	if( !((image_ver[0] == func_spec_hex[index][0]) && (image_ver[1] == func_spec_hex[index][1]) && (image_ver[2] == func_spec_hex[index][2]) && (image_ver[3] == func_spec_hex[index][3])) )
	{			
		return false;
	}	
	return true;
}

static uint8_t LSI_Date(uint16_t index)
{
	uint8_t device_id[10];
	char text[100];  
	
	if(LSI_Read_Data(device_id,3,READ_DEVICE_ID) == false) 
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}
	
	if( (!(device_id[1] == DeviceID1 && device_id[2] == DeviceID2)) )
	{				
		sprintf(text, "%02X%02X/%02X%02X",device_id[1],device_id[2],DeviceID1,DeviceID2);
		ListView_MEAS(text);
		return false;
	}

	uint8_t image_date[10];
	if(LSI_Read_Data(image_date, 8, READ_IMAGE_VER) == false) 
	{
		ListView_MEAS("Comm. Fail2");
		return false;
	}		
	
	sprintf(text,"%02X%02X%02X%02X/%02X%02X%02X%02X",image_date[4],image_date[5],image_date[6],image_date[7],func_spec_hex[index][0],func_spec_hex[index][1],func_spec_hex[index][2],func_spec_hex[index][3]);
	ListView_MEAS(text);	
	dbgmsg("%s\n",text);
	
	if( !((image_date[4] == func_spec_hex[index][0]) && (image_date[5] == func_spec_hex[index][1]) && (image_date[6] == func_spec_hex[index][2]) && (image_date[7] == func_spec_hex[index][3])) )
	{			
		return false;
	}	
	return true;
}

ImgHeader	* header;
ImgChunk	* chunk;
static uint8_t Image_Parsing(void)
{
	uint8_t i=0;
	uint32_t chunk_offset = 0;

	header = (ImgHeader *)&INPUT_DATA[0];
	
	if( header->Signature == 0x53494654)
	{	
		if(1)
		{
			dbgmsg("Signature      : %08X\n",header->Signature);
			dbgmsg("Version        : %08X\n",header->Version);
			dbgmsg("Totalsize      : %08X\n",header->Totalsize);
			dbgmsg("Checksum       : %08X\n",header->Checksum);
			dbgmsg("ImgFileVer     : %08X\n",header->ImgFileVer);
			dbgmsg("ImgFileData    : %08X\n",header->ImgFileDate);
			dbgmsg("ImgFileDesc    : %08X\n",header->ImgFileDesc);
			dbgmsg("FirmwareVer    : %08X\n",header->FirmwareVer);
			dbgmsg("FirmwareDate   : %08X\n",header->FirmwareDate);
			dbgmsg("FirmwareDesc   : %08X\n",header->FirmwareDesc);
			dbgmsg("ParameterVer   : %08X\n",header->ParameterVer);
			dbgmsg("ParameterData  : %08X\n",header->ParameterData);
			dbgmsg("ParameterDesc  : %08X\n",header->ParameterDesc);
			dbgmsg("NumverOfChunk  : %08X\n",header->NumverOfChunk);
			dbgmsg("Flag           : %08X\n",header->Flag);
			dbgmsg("Reserved       : %08X\n",header->Reserved);
		}
		
		chunk_offset = 0x40;
		for(i=0; i < header->NumverOfChunk; i++)
		{	
			chunk = (ImgChunk *)&INPUT_DATA[chunk_offset];
			
			if(chunk->ChunkSignature == 0x53434654)
			{
				ChunkSignature = chunk->ChunkSignature;
				ChunkAddr[i]   = chunk->ChunkAddr;
				ChunkSize[i]   = chunk->ChunkSize;
				ChunkData[i]   = (uint8_t *)(INPUT_DATA + chunk_offset + 0x10);
				chunk_offset = chunk_offset + 0x10 + ChunkSize[i];
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}
static uint8_t Touch_mode_fix(void)
{
	uint8_t tx_buff[10];
	
	// touch mode fix
	if(LSI_Write_Command(SENSE_ON) == false)	return false;
	Delay_ms(20);
	
	//tx_buff[0] = 0x00;
	//if(LSI_Write_Data(tx_buff,1,0x8E) == false)	return false;
		
	tx_buff[0] = 0x02;	tx_buff[1] = 0x02;
	if(LSI_Write_Data(tx_buff,2,0xD7) == false)	return false;
	
	return true;
}
static uint8_t DownloadMode_Enter(void)
{
	uint8_t tx_buff[10];

	tx_buff[0] = 0x55;	tx_buff[1] = 0xAC;
	if(LSI_Write_Data(tx_buff,2,0x57) == false) return false;
	Delay_ms(50);
	
	return true;
}
static uint8_t CheckSum(uint8_t *data, int offset, int size)
{
	int i = 0;
	uint8_t Checksum = 0;
	
	for(i = 0; i<size; i++)
	{
		Checksum += data[i+offset];
	}

	return Checksum;
}
#define page_size 512
static uint8_t Chunk_Page_Erase(uint32_t address, uint32_t size)
{
	uint8_t tx_buff[10];
	
	uint32_t page_idx_start = address / page_size;
	uint32_t page_number    = (size-1)/(page_size)+1;
	
	tx_buff[0] = 0xD8;	
	tx_buff[1] = (uint8_t)( (page_idx_start >> 8) & 0xff );
	tx_buff[2] = (uint8_t)( (page_idx_start >> 0) & 0xff );
	tx_buff[3] = (uint8_t)( (page_number >> 8) & 0xff );
	tx_buff[4] = (uint8_t)( (page_number >> 0) & 0xff );
	tx_buff[5] = CheckSum(tx_buff,1,4);
			
	if(LSI_Write(tx_buff,6) == false)  return false;	
	Delay_ms(page_number + 20);
	
	return true;
}
static uint8_t Flash_Page_Write(uint32_t page_idx, uint8_t *Data)
{
	uint8_t tx_buff[1+2+page_size+1] = {0};
	uint16_t i;
	
	tx_buff[0] = 0xD9;	
	tx_buff[1] = (uint8_t)( (page_idx >> 8) & 0xff );
	tx_buff[2] = (uint8_t)( (page_idx >> 0) & 0xff );
	for(i=0; i < page_size; i++)	tx_buff[3+i] = Data[i];
	tx_buff[1+2+page_size] = CheckSum(tx_buff,1, 2+page_size);
		
	if(LSI_Write(tx_buff,1+2+page_size+1) == false)  return false;

	return true;
}
static uint8_t Chunk_Page_Write(uint32_t address, uint32_t size, u8 *Data,uint8_t chunk)
{
	uint8_t tx_buff[page_size];
	char text[100];
	uint16_t i;
	uint32_t j;
	int32_t page_idx;
	uint32_t page_idx_start = address / page_size;
	uint32_t page_number    = (size-1)/(page_size)+1;
	uint32_t size_copy = (size % page_size);
	if(size_copy == 0)	size_copy = page_size;
		
	j = 0;
	for(page_idx = (page_number-1); page_idx >= 0; page_idx--)
	{		
		if(++j%20 == 0)
		{
			sprintf(text,"Flash Write [%02d](%02d/%02d)",chunk,j,page_number);
			ListView_MEAS(text);
		}
		
		for(i=0; i < page_size; i++)
		{
			if(i < size_copy)	tx_buff[i] = Data[page_idx*page_size+i];
			else 				tx_buff[i] = 0;
		}
		
		if(Flash_Page_Write(page_idx+page_idx_start, tx_buff) == false) { 	return false;	}
		
		size_copy = page_size;		
		//if(Comm_Type == I2CMODE)	Delay_ms(5);		
	}
	
	return true;
}
static uint8_t LSI_ImgWrite(void)
{	
	uint8_t tx_buff[10];
	char text[100];  
	
	if(RD_FW_WRITE == false)
    {
        ListView_MEAS("SKIP");
        return SKIP_RESULT;
    }	
	
	if(Image_Parsing()== false)
	{
		ListView_MEAS("Data Parsing Fail");
		return false;
	}	
	
	if(Touch_mode_fix() == false)
	{
		ListView_MEAS("Touch mode fix Fail");
		return false;
	}

	if(DownloadMode_Enter() == false)
	{
		ListView_MEAS("DownMode Enter Fail");
		return false;
	}

	for(int i=0; i< header->NumverOfChunk; i++)
	{
		sprintf(text,"Flash Erase[%02d]",i);
		ListView_MEAS(text);
		if( Chunk_Page_Erase(ChunkAddr[i], ChunkSize[i]) == false) 
		{
			ListView_MEAS("Page Erase Fail");
			return false;
		}	
	}
	ListView_MEAS("Page Erase OK");
	
	sprintf(text,"Flash Write [%02d](%02d/%02d)",0,0,0);
	ListView_MEAS(text);
	//Write Seq
	for(int i=0; i< header->NumverOfChunk; i++)
	{
		if(Chunk_Page_Write(ChunkAddr[i], ChunkSize[i],ChunkData[i],i) == false)
		{
			ListView_MEAS("Page Write Fail");
			return false;
		}
	}
	ListView_MEAS("Page Write OK");
	
	Delay_ms(100);// S/W Reset
	TA_PIN(0);
	Delay_ms(100);// S/W Reset
	TA_PIN(1);
	Delay_ms(100);// S/W Reset
	
	if(LSI_Write_Command(0x42) == false)		{ ListView_MEAS("S/W Reset Fail1");	return false;}
	Delay_ms(50);

	tx_buff[0] = 0x00;
	if(LSI_Write_Data(tx_buff,1,0x8E) == false) {	ListView_MEAS("S/W Reset Fail2");	return false;}
	
	Delay_us(50);	
	tx_buff[0] = 0x02;	tx_buff[1] = 0x02;
	if(LSI_Write_Data(tx_buff,2,0xD7) == false)	{	ListView_MEAS("S/W Reset Fail3");	return false;}

	ListView_MEAS("OK");
	
	return true;
}
static uint8_t TINTX_Test(void)
{	
	uint8_t tx_buff[10];
	uint8_t rx_buff[10];

	if(LSI_Write_Command(FLUSH_ALL_EVENT) == false)  	
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}
	Delay_ms(100);

	tx_buff[0] = 0x12;	tx_buff[1] = 0x34;	tx_buff[2] = 0x56;	tx_buff[3] = 0x78;
	if(LSI_Write_Data(tx_buff,4,0x84) == false)  
	{
		ListView_MEAS("Comm. Fail2");
		return false;
	}
	Delay_ms(100);
	
	dbgmsg("check int low \n");
	{// check int low
		if(TINTX_READ != 0)
		{
			ListView_MEAS("TINTX Pin Low Fail");
			return false;
		}
		ListView_MEAS("TINTX Pin Low Pass");
	}
		
	if(LSI_Read_Data(rx_buff,6,READ_ONE_EVENT) == false)
	{
		ListView_MEAS("Comm. Fail3");
		return false;
	}
	else	
	{
		uint16_t cnt = 0;
		while(1)
		{
			dbgmsg("%02X %02X %02X %02X %02X %02X\n",rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3],rx_buff[4],rx_buff[5]);		
			if( (rx_buff[2] == 0x12 && rx_buff[3] == 0x34 && rx_buff[4] == 0x56 && rx_buff[5] == 0x78) )
			{
				dbgmsg("check int high\n");
				if(TINTX_READ != 1)		
				{
					ListView_MEAS("TINTX Pin High Fail");			
				}
				else
				{
					ListView_MEAS("TINTX Pin High Pass");	
					break;	
				}
				
				if(cnt++ > 10)	
				{
					ListView_MEAS("TINTX Pin High Fail");	
					return false;	
				}
				Delay_ms(10);
			}
			else
			{
				ListView_MEAS("TINTX Event Read Fail");
				return false;
			}
		}
	}
	
	if(LSI_Write_Command(0x62) == false)  
	{
		ListView_MEAS("Comm. Fail4");
		return false;
	}
	
	Delay_ms(50);
	ListView_MEAS("OK");
	
	return true;
}
static uint8_t TAPIN_Test(void)
{
	uint8_t rx_buff[10];
	
	//TA Pin High
	TA_PIN(1);	
	Delay_ms(10); 
	if(LSI_Read_Data(rx_buff,1,0x88) == false)
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}
	else
	{		
		if((rx_buff[0]&0x02) != 0x02)
		{	
			ListView_MEAS("TA Pin High Fail");
			return false;
		}
		ListView_MEAS("TA Pin High Pass");
	}

	//TA Pin Low
	TA_PIN(0);	
	Delay_ms(10); 	
	if(LSI_Read_Data(rx_buff,1,0x88) == false)
	{
		ListView_MEAS("Comm. Fail2");
		return false;
	}
	else
	{		
		if((rx_buff[0]&0x02) != 0x00)
		{			
			ListView_MEAS("TA Pin Low Fail");
			return false;
		}
		ListView_MEAS("TA Pin Low Pass");
	}
	
	ListView_MEAS("OK");
	return true;
}

gTSP_REPORT	Tsp_Report;
gTSP_RESULT	Tsp_Result;
///////////////////////////////////////////////////////////////
//////////				RawdataPasing					////////////
///////////////////////////////////////////////////////////////
static uint8_t RawdataPasing(uint8_t *data, uint16_t tx, uint16_t rx, uint16_t fo){	
	uint32_t offset = 80;
	uint32_t add = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = offset;
	uint32_t MUTUAL_CAP_ADD = add;
	dbgmsg("Mutual cap : %08d\n", MUTUAL_CAP_ADD);	
	uint32_t node = 0;
	for(int i=0; i < tx ; i++)
	{	
		for(int j=0; j < rx ; j++)
		{
			CM_OFFSET[node] = (int16_t)( ((int16_t)(data[MUTUAL_CAP_ADD+1]<<8)) | ((int16_t)(data[MUTUAL_CAP_ADD]<<0)));
			dbgmsg("%4d ",CM_OFFSET[node]);
			node++;
			MUTUAL_CAP_ADD = MUTUAL_CAP_ADD + 2;
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (tx*rx*2);
	uint32_t SELF_CAP_TX_ADD = add;
	dbgmsg("self cap tx : %08d\n", SELF_CAP_TX_ADD);	// self cap tx				(TX*2)
	for(int i=0; i < tx ; i++)
	{	
			CS_OFFSET_TX[i] =  (int16_t)( ((int16_t)(data[SELF_CAP_TX_ADD+1]<<8)) | ((int16_t)(data[SELF_CAP_TX_ADD]<<0)));
			dbgmsg("%4d ",CS_OFFSET_TX[i]); 
			SELF_CAP_TX_ADD = SELF_CAP_TX_ADD + 2;
	}
	dbgmsg("\n");
		
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (tx*2);	
	uint32_t SELF_CAP_RX_ADD = add;
	dbgmsg("self cap rx : %08d\n", SELF_CAP_RX_ADD);	// self cap rx				(RX*2)	
	for(int i=0; i < rx ; i++)
	{	
			CS_OFFSET_RX[i] =  (int16_t)( ((int16_t)(data[SELF_CAP_RX_ADD+1]<<8)) | ((int16_t)(data[SELF_CAP_RX_ADD]<<0)));
			dbgmsg("%4d ",CS_OFFSET_RX[i]); 
			SELF_CAP_RX_ADD = SELF_CAP_RX_ADD + 2;
	}
	dbgmsg("\n");

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (rx*2);	
	uint32_t FORCE_CAP_ADD = add;
	dbgmsg("force cap	 : %08d\n", FORCE_CAP_ADD);	// force cap				(FO*2)	
	for(int i=0; i < fo ; i++)
	{	
			CF_OFFSET[i] =  (int16_t)( ((int16_t)(data[FORCE_CAP_ADD+1]<<8)) | ((int16_t)(data[FORCE_CAP_ADD]<<0)));
			dbgmsg("%4d ",CF_OFFSET[i] ); 
			FORCE_CAP_ADD = FORCE_CAP_ADD + 2;
	}
	dbgmsg("\n");
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (fo*2);	
	uint32_t MUTUAL_CAP_TX_LINE_ADD = add;			//
	dbgmsg("mutual cap tx line : %08d\n", MUTUAL_CAP_TX_LINE_ADD);// mutual cap tx line		(TX*2)	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (tx*2);	
	uint32_t MUTUAL_CAP_RX_LINE_ADD = add;			//
	dbgmsg("mutual cap rx line : %08d\n", MUTUAL_CAP_RX_LINE_ADD);// mutual cap rx line		(RX*2)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (rx*2);	
	uint32_t MUTUAL_CAP_TX_AVR_ADD = add;			//
	dbgmsg("mutual cap tx avr	 : %08d\n", MUTUAL_CAP_TX_AVR_ADD);// mutual cap tx avr		(2)	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (2);			
	uint32_t MUTUAL_CAP_RX_AVR_ADD = add;				//
	dbgmsg("mutual cap rx avr	 : %08d\n", MUTUAL_CAP_RX_AVR_ADD);// mutual cap rx avr		(2)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (2);			
	uint32_t MUTUAL_CAP_FRAME_AVR_ADD = add;			//
	dbgmsg("mutual cap frame avr : %08d\n", MUTUAL_CAP_FRAME_AVR_ADD);// mutual cap frame avr		(2)	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	add = add + (2);			
	uint32_t TX_SHORT_GND_ADD = add;			//
	dbgmsg("tx short gnd : %08d\n", TX_SHORT_GND_ADD);// tx short gnd				(TX*2)	
	for(int i=0; i < tx ; i++)
	{	
		TX_SHORT_GND[i] =  (int16_t)( ((int16_t)(data[TX_SHORT_GND_ADD+1]<<8)) | ((int16_t)(data[TX_SHORT_GND_ADD]<<0)));
		dbgmsg("%4d ",TX_SHORT_GND[i]); 
		TX_SHORT_GND_ADD = TX_SHORT_GND_ADD + 2;
	}
	dbgmsg("\n");

	for(int i=0; i <tx  ; i++)
	{	
		if(i < (tx-1))
		{
			int AA = TX_SHORT_GND[i];
			int BB = TX_SHORT_GND[i+1];
			float max_t, abs_t;

			if(AA >= BB)	{	max_t = abs(AA); 	abs_t = abs(AA - BB);		}
			else			{	max_t = abs(BB); 	abs_t = abs(BB - AA);		}

			TX_SHORT_GND_GAP[i] =  (int16_t)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
		}
		else
		{
			int AA = TX_SHORT_GND[i];
			int BB = TX_SHORT_GND[i-1];
			float max_t, abs_t;

			if(AA >= BB)	{	max_t = abs(AA); 	abs_t = abs(AA - BB);		}
			else			{	max_t = abs(BB); 	abs_t = abs(BB - AA);		}

			TX_SHORT_GND_GAP[i] =  (int16_t)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
		}
		dbgmsg("%4d ",TX_SHORT_GND_GAP[i]); 
	}	
	dbgmsg("\n");
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (tx*2);		
	uint32_t RX_SHORT_GND_ADD = add;			//
	dbgmsg("rx short gnd : %08d\n", RX_SHORT_GND_ADD);// rx short gnd				(RX*2)	
	for(int i=0; i < rx ; i++)
	{	
		RX_SHORT_GND[i] =  (int16_t)( ((int16_t)(data[RX_SHORT_GND_ADD+1]<<8)) | ((int16_t)(data[RX_SHORT_GND_ADD]<<0)));
		dbgmsg("%4d ",RX_SHORT_GND[i]); 
		RX_SHORT_GND_ADD = RX_SHORT_GND_ADD + 2;
	}
	dbgmsg("\n");
	
	for(int i=0; i <rx  ; i++)
	{	
		if(i < (rx-1))
		{
			int AA = RX_SHORT_GND[i];
			int BB = RX_SHORT_GND[i+1];
			float max_t, abs_t;

			if(AA >= BB)	{	max_t = abs(AA); 	abs_t = abs(AA - BB);		}
			else			{	max_t = abs(BB); 	abs_t = abs(BB - AA);		}

			RX_SHORT_GND_GAP[i] =  (int16_t)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
		}
		else
		{
			int AA = RX_SHORT_GND[i];
			int BB = RX_SHORT_GND[i-1];
			float max_t, abs_t;

			if(AA >= BB)	{	max_t = abs(AA); 	abs_t = abs(AA - BB);		}
			else			{	max_t = abs(BB); 	abs_t = abs(BB - AA);		}

			RX_SHORT_GND_GAP[i] =  (int16_t)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
		}
		dbgmsg("%4d ",RX_SHORT_GND_GAP[i]); 
	}	
	dbgmsg("\n");
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (rx*2);		
	uint32_t TX_SHORT_TX_ADD = add;				//
	dbgmsg("tx short tx : %08d\n", TX_SHORT_TX_ADD);// tx short tx				(TX*2)	
	for(int i=0; i < tx ; i++)
	{	
			TX_SHORT_TX[i] =  (int16_t)( ((int16_t)(data[TX_SHORT_TX_ADD+1]<<8)) | ((int16_t)(data[TX_SHORT_TX_ADD]<<0)));
			dbgmsg("%4d ",TX_SHORT_TX[i]); 
			TX_SHORT_TX_ADD = TX_SHORT_TX_ADD + 2;
	}
	dbgmsg("\n");
	
	for(int i=0; i <tx  ; i++)
	{	
		if(i < (tx-1))
		{
			int AA = TX_SHORT_TX[i];
			int BB = TX_SHORT_TX[i+1];
			float max_t, abs_t;

			if(AA >= BB)	{	max_t = abs(AA); 	abs_t = abs(AA - BB);		}
			else			{	max_t = abs(BB); 	abs_t = abs(BB - AA);		}

			TX_SHORT_TX_GAP[i] =  (int16_t)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
		}
		else
		{
			int AA = TX_SHORT_TX[i];
			int BB = TX_SHORT_TX[i-1];
			float max_t, abs_t;

			if(AA >= BB)	{	max_t = abs(AA); 	abs_t = abs(AA - BB);		}
			else			{	max_t = abs(BB); 	abs_t = abs(BB - AA);		}

			TX_SHORT_TX_GAP[i] =  (int16_t)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
		}
		dbgmsg("%4d ",TX_SHORT_TX_GAP[i]); 
	}	
	dbgmsg("\n");
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (tx*2);		
	uint32_t RX_SHORT_RX_ADD = add;				//
	dbgmsg("rx short rx : %08d\n", RX_SHORT_RX_ADD);// rx short rx				(RX*2)	
	for(int i=0; i < rx ; i++)
	{	
			RX_SHORT_RX[i] =  (int16_t)( ((int16_t)(data[RX_SHORT_RX_ADD+1]<<8)) | ((int16_t)(data[RX_SHORT_RX_ADD]<<0)));
			dbgmsg("%4d ",RX_SHORT_RX[i]); 
			RX_SHORT_RX_ADD = RX_SHORT_RX_ADD + 2;
	}
	dbgmsg("\n");
	
	for(int i=0; i <rx  ; i++)
	{	
		if(i < (rx-1))
		{
			int AA = RX_SHORT_RX[i];
			int BB = RX_SHORT_RX[i+1];
			float max_t, abs_t;

			if(AA >= BB)	{	max_t = abs(AA); 	abs_t = abs(AA - BB);		}
			else			{	max_t = abs(BB); 	abs_t = abs(BB - AA);		}

			RX_SHORT_RX_GAP[i] =  (int16_t)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
		}
		else
		{
			int AA = RX_SHORT_RX[i];
			int BB = RX_SHORT_RX[i-1];
			float max_t, abs_t;

			if(AA >= BB)	{	max_t = abs(AA); 	abs_t = abs(AA - BB);		}
			else			{	max_t = abs(BB); 	abs_t = abs(BB - AA);		}

			RX_SHORT_RX_GAP[i] =  (int16_t)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
		}
		dbgmsg("%4d ",RX_SHORT_RX_GAP[i]); 
	}	
	dbgmsg("\n");
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (rx*2);		
	uint32_t TX_SHORT_RX_ADD = add;				//
	dbgmsg("tx short rx : %08d\n", TX_SHORT_RX_ADD);// tx short rx				(TX*2)	
	for(int i=0; i < tx ; i++)
	{	
			TX_SHORT_RX[i] =  (int16_t)( ((int16_t)(data[TX_SHORT_RX_ADD+1]<<8)) | ((int16_t)(data[TX_SHORT_RX_ADD]<<0)));
			dbgmsg("%4d ",TX_SHORT_RX[i]); 
			TX_SHORT_RX_ADD = TX_SHORT_RX_ADD + 2;
	}
	dbgmsg("\n");
	
	for(int i=0; i <tx  ; i++)
	{	
		if(i < (tx-1))
		{
			int AA = TX_SHORT_RX[i];
			int BB = TX_SHORT_RX[i+1];
			float max_t, abs_t;

			if(AA >= BB)	{	max_t = abs(AA); 	abs_t = abs(AA - BB);		}
			else			{	max_t = abs(BB); 	abs_t = abs(BB - AA);		}

			TX_SHORT_RX_GAP[i] =  (int16_t)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
		}
		else
		{
			int AA = TX_SHORT_RX[i];
			int BB = TX_SHORT_RX[i-1];
			float max_t, abs_t;

			if(AA >= BB)	{	max_t = abs(AA); 	abs_t = abs(AA - BB);		}
			else			{	max_t = abs(BB); 	abs_t = abs(BB - AA);		}

			TX_SHORT_RX_GAP[i] =  (int16_t)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
		}
		dbgmsg("%4d ",TX_SHORT_RX_GAP[i]); 
	}	
	dbgmsg("\n");
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (tx*2);		
	uint32_t RX_SHORT_TX_ADD = add;				//
	dbgmsg("rx short tx : %08d\n", RX_SHORT_TX_ADD);// rx short tx				(RX*2)	
	for(int i=0; i < rx ; i++)
	{	
			RX_SHORT_TX[i] =  (int16_t)( ((int16_t)(data[RX_SHORT_TX_ADD+1]<<8)) | ((int16_t)(data[RX_SHORT_TX_ADD]<<0)));
			dbgmsg("%4d ",RX_SHORT_TX[i]); 
			RX_SHORT_TX_ADD = RX_SHORT_TX_ADD + 2;
	}
	dbgmsg("\n");
	
	for(int i=0; i <rx  ; i++)
	{	
		if(i < (rx-1))
		{
			int AA = RX_SHORT_TX[i];
			int BB = RX_SHORT_TX[i+1];
			float max_t, abs_t;

			if(AA >= BB)	{	max_t = abs(AA); 	abs_t = abs(AA - BB);		}
			else			{	max_t = abs(BB); 	abs_t = abs(BB - AA);		}

			RX_SHORT_TX_GAP[i] =  (int16_t)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
		}
		else
		{
			int AA = RX_SHORT_TX[i];
			int BB = RX_SHORT_TX[i-1];
			float max_t, abs_t;

			if(AA >= BB)	{	max_t = abs(AA); 	abs_t = abs(AA - BB);		}
			else			{	max_t = abs(BB); 	abs_t = abs(BB - AA);		}

			RX_SHORT_TX_GAP[i] =  (int16_t)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
		}
		dbgmsg("%4d ",RX_SHORT_TX_GAP[i]); 
	}	
	dbgmsg("\n");
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (rx*2);		
	uint32_t TX_SELF_GAP_ADD = add;				//
	dbgmsg("tx self gap : %08d\n", TX_SELF_GAP_ADD);// tx self gap				(TX*2)	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (tx*2);		
	uint32_t RX_SELF_GAP_ADD = add;			//
	dbgmsg("rx self gap : %08d\n", RX_SELF_GAP_ADD);// rx self gap				(RX*2)	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (rx*2);			
	uint32_t FORCE_SHORT_GND_ADD = add;		//
	dbgmsg("force short gnd : %08d\n", FORCE_SHORT_GND_ADD);// rx self gap				(F0*2)	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	add = add + (fo*2);			
	uint32_t FORCE_SHORT_FORCE_ADD = add;			//
	dbgmsg("force short force : %08d\n", FORCE_SHORT_FORCE_ADD);// rx self gap				(F0*2)
	return true;
}

char TX_SHORT_GND_BUFF[64] = {NULL};
char RX_SHORT_GND_BUFF[64] = {NULL};
char TX_SHORT_TX_BUFF[64]  = {NULL};
char RX_SHORT_RX_BUFF[64]  = {NULL};
char TX_SHORT_RX_BUFF[64]  = {NULL};
char RX_SHORT_TX_BUFF[64]  = {NULL};
static uint8_t Short_Pasing(void)
{
	char Temp1[64] = {NULL};
	char Temp2[64] = {NULL};
	char Temp3[64] = {NULL};
		
	for(int i=0; i<32; i++)
	{
		if( ( ((Tsp_Report.TXRX_ShortToGndGap_l) >> (i)) & (0x01) ) == 0x01 )	Temp1[i +  0] = 1;
		else																	Temp1[i +  0] = 0;		
		if( ( ((Tsp_Report.TXRX_ShortToGndGap_m) >> (i)) & (0x01) ) == 0x01 )	Temp1[i + 32] = 1;
		else																	Temp1[i + 32] = 0;
		
		if( ( ((Tsp_Report.TXRX_ShortToTXRX_l)   >> (i)) & (0x01) ) == 0x01 )	Temp2[i +  0] = 1;
		else																	Temp2[i +  0] = 0;		
		if( ( ((Tsp_Report.TXRX_ShortToTXRX_m)   >> (i)) & (0x01) ) == 0x01 )	Temp2[i + 32] = 1;
		else																	Temp2[i + 32] = 0;
		
		if( ( ((Tsp_Report.TXRX_ShortToRXTX_l)   >> (i)) & (0x01) ) == 0x01 )	Temp3[i +  0] = 1;
		else																	Temp3[i +  0] = 0;		
		if( ( ((Tsp_Report.TXRX_ShortToRXTX_m)   >> (i)) & (0x01) ) == 0x01 )	Temp3[i + 32] = 1;
		else																	Temp3[i + 32] = 0;
	}
	
	for(int i=0; i<Tsp_Report.TX_Num; i++)
	{
		TX_SHORT_GND_BUFF[i] = Temp1[i];
		TX_SHORT_TX_BUFF[i]  = Temp2[i];
		TX_SHORT_RX_BUFF[i]  = Temp3[i];
	}

	for(int i=0; i<Tsp_Report.RX_Num; i++)
	{
		RX_SHORT_GND_BUFF[i] = Temp1[i + Tsp_Report.TX_Num];
		RX_SHORT_RX_BUFF[i]  = Temp2[i + Tsp_Report.TX_Num];
		RX_SHORT_TX_BUFF[i]  = Temp3[i + Tsp_Report.TX_Num];
	}		

	return true;
}

static uint8_t TSP_Test(void)
{
	uint8_t tx_buff[10];
	uint8_t rx_buff[2200]={0};
		
	Delay_ms(100);
	
	if(LSI_Write_Command(0x62) == false)  
	{
		ListView_MEAS("Comm. Fail1");	
		return false;
	}
	Delay_ms(100);
	
	tx_buff[0] = 0x37;	// node, open, short test run
	if(LSI_Write_Data(tx_buff,1,0xAE) == false)
	{
		ListView_MEAS("Comm. Fail2");
		return false;
	}
	ListView_MEAS("TSP Self-test.");	Delay_ms(200);
	ListView_MEAS("TSP Self-test..");	Delay_ms(200);
	ListView_MEAS("TSP Self-test.");	Delay_ms(200);
	ListView_MEAS("TSP Self-test..");	Delay_ms(200);
	ListView_MEAS("TSP Self-test.");	Delay_ms(200);	
	ListView_MEAS("TSP Self-test..");	Delay_ms(200);
	
	int rp_cnt = 0;
	char disp_flag = 0;
	while(1)
	{
		if(disp_flag == 0)		{ListView_MEAS("TSP Self-test..");		disp_flag = 1;}
		else if(disp_flag == 1)	{ListView_MEAS("TSP Self-test.");		disp_flag = 0;}
		
		if(LSI_Read_Data(rx_buff,6,0x60) == false)
		{
			ListView_MEAS("Comm. Fail3");
		}
		Delay_ms(50);

		dbgmsg("%02X %02X %02X %02X %02X %02X \n",rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3],rx_buff[4],rx_buff[5]);
		if(rx_buff[0]==0x1D && rx_buff[1]==0x41)
		{
			dbgmsg("Self Test OK\n");
			ListView_MEAS("Self Test OK");
			break;
		}
		else
		{
			if(rp_cnt++ > 30)	
			{
				ListView_MEAS("Time out");
				return false;
			}
		}
	}

	tx_buff[0] = 0x00;
	if(LSI_Write_Data(tx_buff,1,0xE4) == false)  
	{
		ListView_MEAS("Comm. Fail4");
		return false;
	}
	Delay_ms(50);
	
	if(LSI_Read_Data(rx_buff,2128,0x80) == false)
	{
		ListView_MEAS("Comm. Fail5");
		return false;
	}
	// Report / Result parsing
	{
		int cnt = 0;
		cnt = 0;		Tsp_Report.Signature			= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	0
		cnt = cnt + 4; 	Tsp_Report.Version				= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	4
		cnt = cnt + 4;	Tsp_Report.Totalsize			= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	8
		cnt = cnt + 4;	Tsp_Report.Checksum				= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	12
		cnt = cnt + 4;	Tsp_Report.TestResult			= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	16
		cnt = cnt + 4;	Tsp_Report.TRYcount				= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	20
		cnt = cnt + 4;	Tsp_Report.PASScount			= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	24
		cnt = cnt + 4;	Tsp_Report.FAILcount			= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	28
		cnt = cnt + 4;	Tsp_Report.RX_Num 				=   ((uint8_t)(rx_buff[cnt])); 																											//	32
		cnt = cnt + 1;	Tsp_Report.TX_Num 		 		=   ((uint8_t)(rx_buff[cnt])); 																											//	
		cnt = cnt + 1;	Tsp_Report.FORCE_CH0_Num 		=   ((uint8_t)(rx_buff[cnt])); 																											//	
		cnt = cnt + 1;	Tsp_Report.FORCE_CH1_Num		=   ((uint8_t)(rx_buff[cnt])); 																											//	
		cnt = cnt + 1;	Tsp_Report.Mutual_Cap_Min		= (int16_t)(((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));																//	36
		cnt = cnt + 2;	Tsp_Report.Mutual_Cap_Max		= (int16_t)(((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));																//	
		cnt = cnt + 2;	Tsp_Report.TXRX_Slope_l			= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	40
		cnt = cnt + 4;	Tsp_Report.TXRX_Slope_m			= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	44
		cnt = cnt + 4;	Tsp_Report.TXRX_Open_l			= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	48
		cnt = cnt + 4;	Tsp_Report.TXRX_Open_m			= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	52
		cnt = cnt + 4;	Tsp_Report.TXRX_ShortToGndGap_l	= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	56
		cnt = cnt + 4;	Tsp_Report.TXRX_ShortToGndGap_m	= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	60
		cnt = cnt + 4;	Tsp_Report.TXRX_ShortToTXRX_l	= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	64
		cnt = cnt + 4;	Tsp_Report.TXRX_ShortToTXRX_m	= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	68
		cnt = cnt + 4;	Tsp_Report.TXRX_ShortToRXTX_l	= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	72
		cnt = cnt + 4;	Tsp_Report.TXRX_ShortToRXTX_m	= (((uint32_t)(rx_buff[cnt+3]<<24))|((uint32_t)(rx_buff[cnt+2]<<16))|((uint32_t)(rx_buff[cnt+1]<<8))|((uint32_t)(rx_buff[cnt]<<0)));	//	76
		
		Tsp_Result.CM_Min 			= (Tsp_Report.TestResult >> 0) & 0x01;
		Tsp_Result.CM_Max 			= (Tsp_Report.TestResult >> 1) & 0x01;
		Tsp_Result.CM_RX_Slope 		= (Tsp_Report.TestResult >> 2) & 0x01;
		Tsp_Result.CM_TX_Slope 		= (Tsp_Report.TestResult >> 3) & 0x01;
		Tsp_Result.RX_PAD_Open 		= (Tsp_Report.TestResult >> 4) & 0x01;
		Tsp_Result.TX_PAD_Open 		= (Tsp_Report.TestResult >> 5) & 0x01;
		Tsp_Result.RX_GND_Short 	= (Tsp_Report.TestResult >> 6) & 0x01;
		Tsp_Result.TX_GND_Short 	= (Tsp_Report.TestResult >> 7) & 0x01;
		Tsp_Result.RX_RX_Short 		= (Tsp_Report.TestResult >> 8) & 0x01;
		Tsp_Result.TX_TX_Short 		= (Tsp_Report.TestResult >> 9) & 0x01;
		Tsp_Result.TX_RX_Short 		= (Tsp_Report.TestResult >>10) & 0x01;
		Tsp_Result.RX_GAP 			= (Tsp_Report.TestResult >>11) & 0x01;
		Tsp_Result.TX_GAP 			= (Tsp_Report.TestResult >>12) & 0x01;
		//Tsp_Result.FO_PAD_Open 	= (Tsp_Report.TestResult >>16) & 0x01;
		//Tsp_Result.FO_GND_Short 	= (Tsp_Report.TestResult >>17) & 0x01;
		//Tsp_Result.FO_FO_Short 	= (Tsp_Report.TestResult >>18) & 0x01;
				
		TX_NUM = Tsp_Report.TX_Num;
		RX_NUM = Tsp_Report.RX_Num;
		FO_NUM = Tsp_Report.FORCE_CH0_Num;
	}
	
	if(1)
	{
		dbgmsg("==================================\n");
		dbgmsg("=====     TSP REPORT VIEW    =====\n");
		dbgmsg("==================================\n");
		dbgmsg("-Signature        : %08X\n", Tsp_Report.Signature);
		dbgmsg("-Version          : %08X\n", Tsp_Report.Version);
		dbgmsg("-Totalsize        : %08X\n", Tsp_Report.Totalsize);
		dbgmsg("-Checksum         : %08X\n", Tsp_Report.Checksum);
		dbgmsg("-Test Result      : %08X\n", Tsp_Report.TestResult);
		dbgmsg("-TRY  count       : %8d\n",  Tsp_Report.TRYcount);
		dbgmsg("-PASS count       : %8d\n",  Tsp_Report.PASScount);
		dbgmsg("-FAIL count       : %8d\n",  Tsp_Report.FAILcount);
		dbgmsg("-Tx Number        : %8d\n",  Tsp_Report.TX_Num);
		dbgmsg("-Rx Number        : %8d\n",  Tsp_Report.RX_Num);
		dbgmsg("-Force ch0 Num    : %8d\n",  Tsp_Report.FORCE_CH0_Num);
		dbgmsg("-Force ch1 Num    : %8d\n",  Tsp_Report.FORCE_CH1_Num);
		dbgmsg("-Mutual Cap Min   : %8d\n",  Tsp_Report.Mutual_Cap_Min);
		dbgmsg("-Mutual Cap Max   : %8d\n",  Tsp_Report.Mutual_Cap_Max);	
		dbgmsg("-TXRX_Slope       : %08X %08X\n", Tsp_Report.TXRX_Slope_m,		 	 Tsp_Report.TXRX_Slope_l);
		dbgmsg("-TXRX_Open        : %08X %08X\n", Tsp_Report.TXRX_Open_m,			 Tsp_Report.TXRX_Open_l);
		dbgmsg("-TXRX_ShToGndGap  : %08X %08X\n", Tsp_Report.TXRX_ShortToGndGap_m,  Tsp_Report.TXRX_ShortToGndGap_l);
		dbgmsg("-TXRX_ShortToTXRX : %08X %08X\n", Tsp_Report.TXRX_ShortToTXRX_m,	 Tsp_Report.TXRX_ShortToTXRX_l);
		dbgmsg("-TXRX_ShortToRXTX : %08X %08X\n", Tsp_Report.TXRX_ShortToRXTX_m,	 Tsp_Report.TXRX_ShortToRXTX_l);
		
		dbgmsg("==================================\n");
		dbgmsg("=====      Test Result       =====\n");
		dbgmsg("==================================\n");
		dbgmsg("-CM_Min       : %d\n", Tsp_Result.CM_Min);
		dbgmsg("-CM_Max       : %d\n", Tsp_Result.CM_Max);
		dbgmsg("-CM_RX_Slope  : %d\n", Tsp_Result.CM_RX_Slope);
		dbgmsg("-CM_TX_Slope  : %d\n", Tsp_Result.CM_TX_Slope);
		dbgmsg("-RX_PAD_Open  : %d\n", Tsp_Result.RX_PAD_Open);
		dbgmsg("-TX_PAD_Open  : %d\n",  Tsp_Result.TX_PAD_Open);
		dbgmsg("-RX_GND_Short : %d\n",  Tsp_Result.RX_GND_Short);
		dbgmsg("-TX_GND_Short : %d\n",  Tsp_Result.TX_GND_Short);
		dbgmsg("-RX_RX_Short  : %d\n",  Tsp_Result.RX_RX_Short);
		dbgmsg("-TX_TX_Short  : %d\n",  Tsp_Result.TX_TX_Short);
		dbgmsg("-TX_RX_Short  : %d\n",  Tsp_Result.TX_RX_Short);
		dbgmsg("-RX_GAP       : %d\n",  Tsp_Result.RX_GAP);
		dbgmsg("-TX_GAP       : %d\n",  Tsp_Result.TX_GAP);
		dbgmsg("-FO_PAD_Open  : %d\n",  Tsp_Result.FO_PAD_Open);	
		dbgmsg("-FO_GND_Short : %d\n",  Tsp_Result.FO_GND_Short);	
		dbgmsg("-FO_FO_Short  : %d\n",  Tsp_Result.FO_FO_Short);
	}	
	
	if(TX_NUM > 50 || RX_NUM > 50)
	{
		TX_NUM = RX_NUM =0;
		ListView_MEAS("Result Read NG");
		return false;
	}
	
	RawdataPasing(rx_buff, TX_NUM, RX_NUM, FO_NUM);
	Short_Pasing();	
	ListView_MEAS("OK");
	return true;
}

uint8_t pre_ng_cont_flag = false;
static uint8_t TSP_RESULT(void)
{	
	char text[100];  
	WR_NG_CONTINUE(pre_ng_cont_flag);

	sprintf(text,"0x%08X",Tsp_Report.TestResult);
	ListView_MEAS(text);
	
	if(Tsp_Report.TestResult != 0)
	{
		return false;
	}
	return true;
}
static uint8_t TRX_Slope(void)
{
	char text[100];  
	pre_ng_cont_flag = RD_NG_CONTINUE;
	WR_NG_CONTINUE(true);

	uint16_t temp1 = ((Tsp_Report.TXRX_Slope_l >>  0) & 0xffff);
	uint16_t temp2 = ((Tsp_Report.TXRX_Slope_l >> 16) & 0xffff);
	uint16_t temp3 = ((Tsp_Report.TXRX_Slope_m >>  0) & 0xffff);
	uint16_t temp4 = ((Tsp_Report.TXRX_Slope_m >> 16) & 0xffff);
	sprintf(text,"%04X%04X %04X%04X",temp4,temp3,temp2,temp1);
	ListView_MEAS(text);
	
	if( !((Tsp_Report.TXRX_Slope_m == 0) && (Tsp_Report.TXRX_Slope_l == 0)) )
	{
		return false;
	}
	return true;
};

static uint8_t TRX_Open(void)
{
	char text[100];  
	WR_NG_CONTINUE(true);
	uint16_t temp1 = ((Tsp_Report.TXRX_Open_l >>  0) & 0xffff);
	uint16_t temp2 = ((Tsp_Report.TXRX_Open_l >> 16) & 0xffff);
	uint16_t temp3 = ((Tsp_Report.TXRX_Open_m >>  0) & 0xffff);
	uint16_t temp4 = ((Tsp_Report.TXRX_Open_m >> 16) & 0xffff);
	sprintf(text,"%04X%04X %04X%04X",temp4,temp3,temp2,temp1);
	ListView_MEAS(text);
	
	if( !((Tsp_Report.TXRX_Open_m == 0) && (Tsp_Report.TXRX_Open_l == 0)) )
	{
		return false;
	}
	
	return true;
};

static uint8_t TRX_ShToGndGap(void)
{
	char text[100];  
	WR_NG_CONTINUE(true);

	uint16_t temp1 = ((Tsp_Report.TXRX_ShortToGndGap_l >>  0) & 0xffff);
	uint16_t temp2 = ((Tsp_Report.TXRX_ShortToGndGap_l >> 16) & 0xffff);
	uint16_t temp3 = ((Tsp_Report.TXRX_ShortToGndGap_m >>  0) & 0xffff);
	uint16_t temp4 = ((Tsp_Report.TXRX_ShortToGndGap_m >> 16) & 0xffff);
	sprintf(text,"%04X%04X %04X%04X",temp4,temp3,temp2,temp1);
	ListView_MEAS(text);
	
	if( !((Tsp_Report.TXRX_ShortToGndGap_m == 0) && (Tsp_Report.TXRX_ShortToGndGap_l == 0)) )
	{
		return false;
	}
	return true;
};

static uint8_t TRX_ShortToTXRX(void)
{	
	char text[100];  
	WR_NG_CONTINUE(true);

	uint16_t temp1 = ((Tsp_Report.TXRX_ShortToTXRX_l >>  0) & 0xffff);
	uint16_t temp2 = ((Tsp_Report.TXRX_ShortToTXRX_l >> 16) & 0xffff);
	uint16_t temp3 = ((Tsp_Report.TXRX_ShortToTXRX_m >>  0) & 0xffff);
	uint16_t temp4 = ((Tsp_Report.TXRX_ShortToTXRX_m >> 16) & 0xffff);
	sprintf(text,"%04X%04X %04X%04X",temp4,temp3,temp2,temp1);
	ListView_MEAS(text);
	
	if( !((Tsp_Report.TXRX_ShortToTXRX_m == 0) && (Tsp_Report.TXRX_ShortToTXRX_l == 0)) )
	{
		return false;
	}
	return true;
};

static uint8_t TRX_ShortToRXTX(void)
{
	char text[100];  
	WR_NG_CONTINUE(true);

	uint16_t temp1 = ((Tsp_Report.TXRX_ShortToRXTX_l >>  0) & 0xffff);
	uint16_t temp2 = ((Tsp_Report.TXRX_ShortToRXTX_l >> 16) & 0xffff);
	uint16_t temp3 = ((Tsp_Report.TXRX_ShortToRXTX_m >>  0) & 0xffff);
	uint16_t temp4 = ((Tsp_Report.TXRX_ShortToRXTX_m >> 16) & 0xffff);
	sprintf(text,"%04X%04X %04X%04X",temp4,temp3,temp2,temp1);
	ListView_MEAS(text);
	
	if( !((Tsp_Report.TXRX_ShortToRXTX_m == 0) && (Tsp_Report.TXRX_ShortToRXTX_l == 0)) )
	{
		return false;
	}
	return true;
};


static uint8_t TA_HSYNC_Test(void)
{
	uint8_t tx_buff[10];
	uint8_t rx_buff[10];
	
	//TA Pin High
	TA_PIN(1);	Delay_ms(10);

	tx_buff[0] = 0x40;	tx_buff[1] = 0x00;	tx_buff[2] = 0x10;	tx_buff[3] = 0x04;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)	
	{
		ListView_MEAS("Comm. Fail1");	
		return false;
	}		
	Delay_ms(30);

	tx_buff[0] = 0x00;	tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)	
	{	
		ListView_MEAS("Comm. Fail2");	
		return false;
	}	
	Delay_ms(30);

	if(LSI_Read_Data(rx_buff,1,0xD2) == false)
	{
		//TA Pin Low
		TA_PIN(0);	Delay_ms(10);
		ListView_MEAS("Comm. Fail3");	
		return false;
	}
	else
	{
		//TA Pin Low
		TA_PIN(0);	Delay_ms(10);
		uint8_t temp = (rx_buff[0] & 0x10);

		if(temp == 0x01)
		{
			ListView_MEAS("TA-HSYNC Pin Short");
			return false;
			
		}
		else if(temp == 0x00)
		{
			ListView_MEAS("TA-HSYNC Pin Open");
		}
	}
	//TA Pin Low
	TA_PIN(0);	Delay_ms(10);
	
	//TA Pin High
	TA_PIN(1);	Delay_ms(10);

	tx_buff[0] = 0x40;	tx_buff[1] = 0x00;	tx_buff[2] = 0x10;	tx_buff[3] = 0x04;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{		
		ListView_MEAS("Comm. Fail4");	
		return false;	
	}
	Delay_ms(30);

	tx_buff[0] = 0x00;	tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{		
		ListView_MEAS("Comm. Fail5");	
		return false;	
	}
	Delay_ms(30);
	
	if(LSI_Read_Data(rx_buff,1,0xD2) == false)
	{
		//TA Pin Low
		TA_PIN(0);	Delay_ms(10);
		ListView_MEAS("Comm. Fail6");
		return false;	
	}
	else
	{
		//TA Pin Low
		TA_PIN(0);	Delay_ms(10);
		uint8_t temp = (rx_buff[0] & 0x10);

		if(temp == 0x01)
		{
			ListView_MEAS("TA-HSYNC Pin Short");
			return false;
			
		}
		else if(temp == 0x00)
		{
			ListView_MEAS("TA-HSYNC Pin Open");
		}
	}
	//TA Pin Low
	TA_PIN(0);	Delay_ms(10);
	
	ListView_MEAS("OK");
	return true;
}
static uint8_t TAPIN_Test_656VQ(void)
{
	uint8_t tx_buff[10];
	uint8_t rx_buff[10];
	
	tx_buff[0]=0x40; tx_buff[1]=0x00; tx_buff[2]=0x10; tx_buff[3]=0x00;
	
	if(LSI_Write_Data(tx_buff,4,0xD0) == false) 
	{
		if(Comm_Type == 0)	ListView_MEAS("I2C Fail1");			
		else 				ListView_MEAS("SPI Fail1");
		return false;
	}
	Delay_ms(3);
	
	tx_buff[0]=0x00; tx_buff[1]=0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false) 
	{
		if(Comm_Type == 0)	ListView_MEAS("I2C Fail2");			
		else 				ListView_MEAS("SPI Fail2");
		return false;
	}
	Delay_ms(3);
	
	tx_buff[0]=0xA0; tx_buff[1]=0xC0; tx_buff[2]=0x0A; tx_buff[3]=0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false) 
	{
		if(Comm_Type == 0)	ListView_MEAS("I2C Fail3");			
		else 				ListView_MEAS("SPI Fail3");
		return false;
	}
	
	Delay_ms(3);
	
	TA_PIN(1);	
	
	Delay_ms(10); 
	
	tx_buff[0]=0x40; tx_buff[1]=0x00; tx_buff[2]=0x10; tx_buff[3]=0x04;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false) 
	{
		if(Comm_Type == 0)	ListView_MEAS("I2C Fail4");			
		else 				ListView_MEAS("SPI Fail4");
		return false;
	}
	Delay_ms(3);
	tx_buff[0]=0x00; tx_buff[1]=0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false) 
	{
		if(Comm_Type == 0)	ListView_MEAS("I2C Fail5");			
		else 				ListView_MEAS("SPI Fail5");
		return false;
	}
	Delay_ms(10);
	if(LSI_Read_Data(rx_buff,4,0xD2) == false) 
	{
		if(Comm_Type == 0)	ListView_MEAS("I2C Fail6");			
		else 				ListView_MEAS("SPI Fail6");
		return false;
	}
	else 
	{		
		dbgmsg("TA Pin High Check : %02X%02X%02X%02X\n",rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3]);
		if((rx_buff[1]&0x02) != 0x02) 
		{
			ListView_MEAS("TA Pin High Fail");
			return false;
		}
		ListView_MEAS("TA Pin High Pass");
		
	}
	
	Delay_ms(5);
	
	TA_PIN(0);	
	
	Delay_ms(10);	
	
	tx_buff[0]=0x40; tx_buff[1]=0x00; tx_buff[2]=0x10; tx_buff[3]=0x04;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false) 
	{
		if(Comm_Type == 0)	ListView_MEAS("I2C Fail7");			
		else 				ListView_MEAS("SPI Fail7");
		return false;
	}
	Delay_ms(3);
	tx_buff[0]=0x00; tx_buff[1]=0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false) 
	{
		if(Comm_Type == 0)	ListView_MEAS("I2C Fail8");			
		else 				ListView_MEAS("SPI Fail8");
		return false;
	}
	Delay_ms(10);
		
	if(LSI_Read_Data(rx_buff,4,0xD2) == false) 
	{
		if(Comm_Type == 0)	ListView_MEAS("I2C Fail9");			
		else 				ListView_MEAS("SPI Fail9");
		return false;
	}
	else 
	{
		dbgmsg("TA Pin Low Check : %02X%02X%02X%02X\n",rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3]);
		if((rx_buff[1]|0x00) != 0x0D) 
		{
			ListView_MEAS("TA Pin Low Fail");
			return false;
		}
		ListView_MEAS("TA Pin Low Pass");		
	}
	
	ListView_MEAS("OK");
	return true;
}
static uint8_t INTR2_Test(void)
{	
	uint8_t tx_buff[10];

	tx_buff[0] = 0x40;	tx_buff[1] = 0x00;	tx_buff[2] = 0x10;	tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)  
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}
	Delay_ms(100);		
	
	tx_buff[0]=0x00; tx_buff[1]=0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false) 
	{
		ListView_MEAS("Comm. Fail2");
		return false;
	}
	Delay_ms(100);		
	
	tx_buff[0]=0xA0; tx_buff[1]=0xC0; tx_buff[2]=0x0E; tx_buff[3]=0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false) 
	{
		ListView_MEAS("Comm. Fail3");
		return false;
	}
	Delay_ms(100);		
	
	tx_buff[0] = 0x40;	tx_buff[1] = 0x00;	tx_buff[2] = 0x10;	tx_buff[3] = 0x04;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)  
	{
		ListView_MEAS("Comm. Fail4");
		return false;
	}
	Delay_ms(100);		
	
	tx_buff[0]=0x00; tx_buff[1]=0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false) 
	{
		ListView_MEAS("Comm. Fail5");
		return false;
	}
	Delay_ms(100);		
	
	tx_buff[0]=0x0E; tx_buff[1]=0x0F; tx_buff[2]=0x00; tx_buff[3]=0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false) 
	{
		ListView_MEAS("Comm. Fail6");
		return false;
	}
	Delay_ms(100);		
	
	{// check intr2 High
		if(INTR2_READ != 1)
		{
			ListView_MEAS("INTR2 Pin High Fail");
			return false;
		}
		ListView_MEAS("INTR2 Pin High Pass");
	}		
	Delay_ms(100);		
	
	tx_buff[0] = 0x40;	tx_buff[1] = 0x00;	tx_buff[2] = 0x10;	tx_buff[3] = 0x04;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)  
	{
		ListView_MEAS("Comm. Fail7");
		return false;
	}
	Delay_ms(100);		
	
	tx_buff[0]=0x00; tx_buff[1]=0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false) 
	{
		ListView_MEAS("Comm. Fail8");
		return false;
	}
	Delay_ms(100);		
	
	tx_buff[0]=0x0E; tx_buff[1]=0x0D; tx_buff[2]=0x00; tx_buff[3]=0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false) 
	{
		ListView_MEAS("Comm. Fail9");
		return false;
	}
	Delay_ms(100);
	
	{// check intr2 Low
		if(INTR2_READ != 0)
		{
			ListView_MEAS("INTR2 Pin Low Fail");
			return false;
		}
		ListView_MEAS("INTR2 Pin Low Pass");
	}
	ListView_MEAS("OK");
	
	return true;
}
static uint8_t SRAM_Test(void)
{
	uint8_t tx_buff[10];
	uint8_t rx_buff[10];
	
	if(LSI_Write_Command(0x62) == false)  
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}
	
	tx_buff[0]=0x0F;
	if(LSI_Write_Data(tx_buff,1,0x9D) == false) 
	{
		ListView_MEAS("Comm. Fail2");
		return false;
	}
	Delay_ms(155);
	
	if(LSI_Read_Data(rx_buff,1,0x9D) == false) 
	{
		ListView_MEAS("Comm. Fail3");
		return false;
	}
	else
	{			
		dbgmsg("SRAM Check : %02X\n",rx_buff[0]);
		if(rx_buff[0] != 0x00) 
		{
			ListView_MEAS("SRAM Test Fail");
			return false;
		}
	}
	
	ListView_MEAS("OK");
	return true;
}


static uint8_t Idle_Reset_Test(void)
{
	uint8_t tx_buff[10];
	uint8_t rx_buff[10];
	
	if(LSI_Write_Command(0x62) == false)  
	{
		if(Comm_Type == 0)	ListView_MEAS("I2C Fail1");			
		else 				ListView_MEAS("SPI Fail1");
		return false;
	}
	Delay_ms(35);
	
	tx_buff[0]=0x00;
	if(LSI_Write_Data(tx_buff,1,0x8E) == false) 
	{
		if(Comm_Type == 0)	ListView_MEAS("I2C Fail2");			
		else 				ListView_MEAS("SPI Fail2");
		return false;
	}
	tx_buff[0]=0x02; tx_buff[1]=0x00;
	if(LSI_Write_Data(tx_buff,2,0xD7) == false) 
	{
		if(Comm_Type == 0)	ListView_MEAS("I2C Fail3");			
		else 				ListView_MEAS("SPI Fail3");
		return false;
	}
	Delay_ms(205);
	if(LSI_Read_Data(rx_buff,6,0x60) == false) 
	{
		if(Comm_Type == 0)	ListView_MEAS("I2C Fail4");			
		else 				ListView_MEAS("SPI Fail4");
		return false;
	}
	else 
	{
		dbgmsg("Idle Reset Check : %02X%02X%02X%02X%02X%02X\n",rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3],rx_buff[4],rx_buff[5]);
		if((rx_buff[0] == 0x09) && (rx_buff[1] == 0x00) && (rx_buff[2] == 0x10) && (rx_buff[3] == 0x00) && (rx_buff[4] == 0x00) && (rx_buff[5] == 0x00))
		{
			dbgmsg("Idle Reset Fail");
			ListView_MEAS("Reset Fail");
			return false;
		}
	}
	
	ListView_MEAS("OK");
	return true;
}
static uint8_t INTR2_Test_SY792(void)
{	
	uint8_t tx_buff[10];

	tx_buff[0] = 0x40;	tx_buff[1] = 0x00;	tx_buff[2] = 0x10;	tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)  
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}
	Delay_ms(100);		
	
	tx_buff[0]=0x00; tx_buff[1]=0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false) 
	{
		ListView_MEAS("Comm. Fail2");
		return false;
	}
	Delay_ms(100);		
	tx_buff[0]=0xA0; tx_buff[1]=0x80; tx_buff[2]=0x2B; tx_buff[3]=0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false) 
	{
		ListView_MEAS("Comm. Fail3");
		return false;
	}
	Delay_ms(100);		
	
	tx_buff[0] = 0x40;	tx_buff[1] = 0x00;	tx_buff[2] = 0x10;	tx_buff[3] = 0x04;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)  
	{
		ListView_MEAS("Comm. Fail4");
		return false;
	}
	Delay_ms(100);		
	
	tx_buff[0]=0x00; tx_buff[1]=0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false) 
	{
		ListView_MEAS("Comm. Fail5");
		return false;
	}
	Delay_ms(100);		
	
	tx_buff[0]=0x8E; tx_buff[1]=0x1F; tx_buff[2]=0x00; tx_buff[3]=0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false) 
	{
		ListView_MEAS("Comm. Fail6");
		return false;
	}
	Delay_ms(100);		
	{// check intr2 High
		if(INTR2_READ != 1)
		{
			ListView_MEAS("INTR2 Pin High Fail");
			return false;
		}
		ListView_MEAS("INTR2 Pin High Pass");
	}		
	Delay_ms(100);		
	
	tx_buff[0] = 0x40;	tx_buff[1] = 0x00;	tx_buff[2] = 0x10;	tx_buff[3] = 0x04;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)  
	{
		ListView_MEAS("Comm. Fail7");
		return false;
	}
	Delay_ms(100);		
	
	tx_buff[0]=0x00; tx_buff[1]=0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false) 
	{
		ListView_MEAS("Comm. Fail8");
		return false;
	}
	Delay_ms(100);		
	
	tx_buff[0]=0x8E; tx_buff[1]=0x1E; tx_buff[2]=0x00; tx_buff[3]=0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false) 
	{
		ListView_MEAS("Comm. Fail9");
		return false;
	}	
	Delay_ms(100);		
	tx_buff[0]=0x00; tx_buff[1]=0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false) 
	{
		ListView_MEAS("Comm. Fail10");
		return false;
	}
	Delay_ms(100);		
	
	{// check intr2 Low
		if(INTR2_READ != 0)
		{
			ListView_MEAS("INTR2 Pin Low Fail");
			return false;
		}
		ListView_MEAS("INTR2 Pin Low Pass");
	}
	ListView_MEAS("OK");
	return true;
}
static uint8_t RESET_Test(void)
{
	uint8_t rx_buff[10];
	char text[100];  
		
	RST_PIN(1);	Delay_ms(100);
	RST_PIN(0);	Delay_ms(100);
	RST_PIN(1);	Delay_ms(100);
	
	if(LSI_Read_Data(rx_buff,6,READ_ONE_EVENT) == false)
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}
	else
	{
		dbgmsg("%02X %02X %02X %02X %02X %02X\n",rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3],rx_buff[4],rx_buff[5]);		
		if(rx_buff[0] == 0x09 && rx_buff[1] == 0x00 && rx_buff[2] == 0x10)
		{
			sprintf(text,"OK(%02X %02X %02X %02X %02X %02X)",rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3],rx_buff[4],rx_buff[5]);
			ListView_MEAS(text);
		}
		else
		{
			sprintf(text,"NG(%02X %02X %02X %02X %02X %02X)",rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3],rx_buff[4],rx_buff[5]);
			ListView_MEAS(text);
			return false;
		}
	}
	ListView_MEAS(text);
		
	return true;
}
static uint8_t CHIP_ID_Check(void)
{
	uint8_t device_id[10];	
	char text[100];  

	if(LSI_Read_Data(device_id,3,READ_DEVICE_ID) == false) 
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}

	dbgmsg("%02X%02X%02X\n", device_id[0], device_id[1], device_id[2]);

	sprintf(text,"%02X%02X",device_id[1],device_id[2]);
	ListView_MEAS(text);
	
	if( (!(device_id[1] == DeviceID1 && device_id[2] == DeviceID2)) )
	{				
		return false;
	}	
	
	return true;
}

static uint8_t TRIM_Check(uint16_t index)
{
	uint8_t tx_buff[10];
	uint8_t rx_buff[10];
	char text[100];  
	
	tx_buff[0] = 0x00;	tx_buff[1] = 0x04;	tx_buff[2] = 0xFA;	tx_buff[3] = 0x20;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)  
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}
	Delay_ms(10);
	
	tx_buff[0]=0x00; tx_buff[1]=0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false) 
	{
		ListView_MEAS("Comm. Fail2");
		return false;			
	}	
	Delay_ms(10);	
	
	if(LSI_Read_Data(rx_buff,4,0xD2) == false)
	{
		ListView_MEAS("Comm. Fail3");
		return false;			
	}
	Delay_ms(10);
	
	
	if( ((rx_buff[0] == 0x0D) && (rx_buff[1] == 0x90) && (rx_buff[2] == 0x00) && (rx_buff[3] == 0x00)) )
	{	
		sprintf(text,"75MHz(%02X.%02X.%02X.%02X)",rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3]);
		ListView_MEAS(text);	
	}	
	else if( ((rx_buff[0] == 0x0D) && (rx_buff[1] == 0x90) && (rx_buff[2] == 0x09) && (rx_buff[3] == 0x90)) )
	{
		sprintf(text,"70MHz(%02X.%02X.%02X.%02X)",rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3]);
		ListView_MEAS(text);	
	}
	else
	{
		sprintf(text,"%02X.%02X.%02X.%02X",rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3]);
		ListView_MEAS(text);	
	}
	
	if( !((rx_buff[0] == func_spec_hex[index][0]) && (rx_buff[1] == func_spec_hex[index][1]) && (rx_buff[2] == func_spec_hex[index][2]) && (rx_buff[3] == func_spec_hex[index][3])) )
	{			
		return false;
	}	
	
	return true;
}
///////////////////////////////////////////////////////
/////     		I2C&SPI Comm. fnc			 //////////
///////////////////////////////////////////////////////

#define SPI_DELAY_CS	10

uint8_t LSI_Read_Data(uint8_t *rData, uint32_t rSize, uint8_t Addr)
{
	if(Comm_Type == I2CMODE)	//i2c
	{
		uint8_t count = 0;
		uint8_t REP_NUM = 5;

		RETRY_RD:
			
		i2c1_start();    // => start

		if(i2c1_tx( SlaveAdd & 0xFE ) != 0)
		{
			if( count++ < REP_NUM) 	goto	RETRY_RD;  
			i2c1_stop();	
			dbgmsg("Slave Addr Write Err\n "); 
			return false; 
		}		

		if(i2c1_tx((uint8_t)Addr) != 0)
		{
			if( count++ < REP_NUM)	goto	RETRY_RD;  
			i2c1_stop();
			dbgmsg("add Err\n"); 
			return false; 
		}	
			
		i2c1_start();	
				
		if(i2c1_tx( (SlaveAdd & 0xFE) | 0x01 ) != 0)
		{
			if( count++ < REP_NUM) goto 	RETRY_RD;  
			i2c1_stop();
			dbgmsg("Slave Addr Read Err\n"); 
			return false; 
		}	
		
		for(uint32_t i=0; i<rSize; i++)
		{
			if( i == (rSize-1) ) 	*rData = i2c1_rx(0);
			else     	           	*rData = i2c1_rx(1);
			rData++;
		}
		rData = rData - rSize;

		i2c1_stop();		
	}
	else if(Comm_Type == SPIMODE)	//spi
	{		
		//dbgmsg("====[SPI_Read_Data START]====\n");					
					
		memset(rx_temp, 0, sizeof(rx_temp));
		memset(tx_temp, 0, sizeof(tx_temp));
		
		tx_temp[0] = 0xAA;				// Sync code
		tx_temp[1] = 0x00;				// tx size high byte
		tx_temp[2] = 0x01;				// tx size low byte
		tx_temp[3] = (rSize>>8) & 0xff;	// rx size high byte
		tx_temp[4] = (rSize>>0) & 0xff;	// rx size low byte
		tx_temp[5] = Addr;				// Command ID
		uint8_t checksum_tx = 0;		
		for(int i=0; i<6; i++)	checksum_tx += tx_temp[i];		
		tx_temp[6] = checksum_tx;		// check sum
		tx_temp[7] = 0x00;				// Dummy data	

		//if(rSize < 30)
		//{	
		//	dbgmsg("TX : ");
		//	for(int i=0; i<8; i++)	dbgmsg("%02X ", tx_temp[i]);
		//	dbgmsg("\n");
		//}

		SPI1_CS(0);
		Delay_us(SPI_DELAY_CS);

		for (int i = 0; i < 8; i++)
		{
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)  == RESET){};	SPI_I2S_SendData(SPI1,tx_temp[i]);  
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){};	SPI_I2S_ReceiveData(SPI1); 
		}
		SPI1_CS(1);
			
		     if( Addr == 0x72 ) Delay_us(1000);
		else if( Addr == 0xD2 ) Delay_us(4000);
		else if( Addr == 0x21 ) Delay_us(40000);
		else if( Addr == 0x80 ) Delay_us(10000);
		else if( Addr == 0x60 ) Delay_us(1000);
		else if( Addr == 0xAE ) Delay_us(1000);
		else 					Delay_us(1000);
				
		int size = 7 + rSize+2;
		size = (size + 3) & ~3;	

		SPI1_CS(0);		Delay_us(SPI_DELAY_CS);	
		for (int i = 0; i < size; i++)
		{
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)  == RESET){};	SPI_I2S_SendData(SPI1,0x80);  
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){};	rx_temp[i] = SPI_I2S_ReceiveData(SPI1); 
		}
		SPI1_CS(1);
		
		uint16_t rx_rength    = ((uint16_t)rx_temp[1]<<8) + (uint16_t)rx_temp[2];
		uint8_t  cal_checksum = 0;
		char text[100];  

		for(int i = 0; i < 5+rx_rength; i++)	cal_checksum += rx_temp[i];	

		if(cal_checksum != rx_temp[5+rx_rength])
		{
			sprintf(text,"RX_CHECKSUM_ERR(%02X/%02X)",cal_checksum,rx_temp[5+rx_rength]);
			ListView_MEAS(text);							
			return false;
		}
		
		if( (rx_temp[5] == Addr) && (rx_temp[6] == 0x00) )	
		{	
			//dbgmsg(" => TIF_STATUS_CMD_OK\n");				
		}
		else
		{
				 if( rx_temp[6]  == TIF_STATUS_ERROR ) 				{ 	dbgmsg(" => TIF_STATUS_ERROR\n");          sprintf(text,"STATUS_ERROR");	}
			else if( rx_temp[6]  == TIF_STATUS_UNKNOWN_CMD ) 		{ 	dbgmsg(" => TIF_STATUS_UNKNOWN_CMD\n");    sprintf(text,"UNKNOWN_CMD");	}
			else if( rx_temp[6]  == TIF_STATUS_CMD_FAIL ) 			{ 	dbgmsg(" => TIF_STATUS_CMD_FAIL\n");       sprintf(text,"CMD_FAIL");		}
			else if( rx_temp[6]  == TIF_STATUS_BAD_PARAMETER ) 		{ 	dbgmsg(" => TIF_STATUS_BAD_PARAMETER\n");  sprintf(text,"BAD_PARAMETER");	}
			else if( rx_temp[6]  == TIF_STATUS_CHECKSUM_FAIL ) 		{ 	dbgmsg(" => TIF_STATUS_CHECKSUM_FAIL\n");  sprintf(text,"CHECKSUM_FAIL");	}	
			else 													{ 	dbgmsg(" => TIF_UNKNOWN_ERROR\n");         sprintf(text,"UNKNOWN_ERROR");	}
			
			ListView_MEAS(text);									
			//return false;
		}
		
		for (int i = 0; i < rSize; i++)
		{
			rData[i]=rx_temp[i+7];
		}
	}
    
    return true;
}

uint8_t LSI_Write_Command(uint8_t command)
{	
	if(Comm_Type == I2CMODE)	//i2c
	{
		uint8_t count = 0;
		uint8_t REP_NUM = 5;

		RETRY_WC:
		
		i2c1_start();
		
		if(i2c1_tx( (SlaveAdd & 0xFE) ) != 0)
		{
			if( count++ < REP_NUM) goto 	RETRY_WC; 
			i2c1_stop();
			dbgmsg("Slave Addr Write Err\n"); 
			return false; 
		}	
		if(i2c1_tx( (uint8_t)command ) != 0)
		{
			if( count++ < REP_NUM) goto 	RETRY_WC; 
			i2c1_stop();
			dbgmsg("Command Write Err\n"); 
			return false; 
		}	
		i2c1_stop();
	}
	else if(Comm_Type == SPIMODE)	//spi
	{
		//dbgmsg("====[SPI_WRITE_CMD START]====\n");
						
		memset(rx_temp, 0, sizeof(rx_temp));
		memset(tx_temp, 0, sizeof(tx_temp));
		
		tx_temp[0] = 0xAA;				// Sync code
		tx_temp[1] = 0x00;				// tx size high byte
		tx_temp[2] = 0x01;				// tx size low byte
		tx_temp[3] = 0x00;	// rx size high byte
		tx_temp[4] = 0x00;	// rx size low byte
		tx_temp[5] = command;				// Command ID
		uint8_t checksum_tx = 0;		
		for(int i=0; i<6; i++)	checksum_tx += tx_temp[i];		
		tx_temp[6] = checksum_tx;		// check sum
		tx_temp[7] = 0x00;				// Dummy data
		
		SPI1_CS(0);
		Delay_us(SPI_DELAY_CS);

		for (int i = 0; i < 12; i++)
		{
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)  == RESET){};	SPI_I2S_SendData(SPI1,tx_temp[i]);  
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){};	SPI_I2S_ReceiveData(SPI1); 
		}
		SPI1_CS(1);
			
   		     if( command == 0x72 ) 	Delay_us(1000);
		else if( command == 0xD2 ) 	Delay_us(4000);
		else if( command == 0x21 ) 	Delay_us(40000);
		else if( command == 0x80 ) 	Delay_us(10000);
		else if( command == 0xD8 ) 	Delay_us(1000);
		else if( command == 0xD9 ) 	Delay_us(1000);
		else if( command == 0x60 ) 	Delay_us(1000);
		else if( command == 0xAE ) 	Delay_us(1000);
		else						Delay_us(1000);
	}
	return true; 
}

uint8_t LSI_Write(uint8_t *pData, uint32_t rSize)
{
	if(Comm_Type == I2CMODE)	//i2c
	{	
		//uint8_t TempData8;
		uint8_t count = 0;
		uint8_t REP_NUM = 5;

		RETRY_W:

		i2c1_start();
		if(i2c1_tx( (SlaveAdd & 0xFE) ) != 0)
		{
			if( count++ < REP_NUM) goto 	RETRY_W; 
			i2c1_stop();
			dbgmsg("Slave Addr Write Err\n"); 
			return false; 
		}			
		for(uint32_t i=0; i < rSize; i++)
		{
			//TempData8 = *pData++; 
			
			if(i2c1_tx( (uint8_t)(*pData++) ) != 0)
			{
				if( count++ < REP_NUM) goto 	RETRY_W; 
				i2c1_stop();
				dbgmsg("Data wr err(%03d/%03d)\n",i+1,rSize);
				return false; 
			}
		}    
		i2c1_stop();
	}
	else if(Comm_Type == SPIMODE)	//spi
	{
		//dbgmsg("====[SPI_WRITE START]====\n");	
				
		memset(rx_temp, 0, sizeof(rx_temp));
		memset(tx_temp, 0, sizeof(tx_temp));
		
		tx_temp[0] = 0xAA;					// Sync code
		tx_temp[1] = ((rSize)>>8)&0xff;		// tx size high byte
		tx_temp[2] = ((rSize)>>0)&0xff;		// tx size low byte
		tx_temp[3] = 0x00;					// rx size high byte
		tx_temp[4] = 0x00;					// rx size low byte
		for(int i=0; i<rSize; i++)	tx_temp[5+i] = pData[i];
		uint8_t checksum_tx = 0;
		for(int i=0; i<(5+rSize); i++)	checksum_tx += tx_temp[i];
		tx_temp[5+rSize] = checksum_tx;		// check sum
		
		int tsize = 5+rSize+1;
		tsize = (tsize + 3) & ~3;
						
		SPI1_CS(0);
		Delay_us(SPI_DELAY_CS);

		for (int i = 0; i < tsize; i++)
		{
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)  == RESET){};	SPI_I2S_SendData(SPI1,tx_temp[i]);  
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){};	SPI_I2S_ReceiveData(SPI1); 
		}
		SPI1_CS(1);
			
       		 if( pData[0] == 0x72 ) 	Delay_us(1000);
		else if( pData[0] == 0xD2 ) 	Delay_us(4000);
		else if( pData[0] == 0x21 ) 	Delay_us(40000);
		else if( pData[0] == 0x80 ) 	Delay_us(1000);
		else if( pData[0] == 0xD8 ) 	Delay_us(1000);
		else if( pData[0] == 0xD9 ) 	Delay_us(1000);
		else if( pData[0] == 0x60 ) 	Delay_us(1000);
		else if( pData[0] == 0xAE ) 	Delay_us(1000);
		else							Delay_us(1000);
				
		return true;
	}
	return true;
}


uint8_t LSI_Write_Data(uint8_t *pData, uint32_t rSize, u8 Addr)
{
	if(Comm_Type == I2CMODE)	//i2c
	{	
		//uint8_t TempData8;
		uint8_t count = 0;
		uint32_t REP_NUM = 5;
		RETRY_WD:

		i2c1_start();
		if(i2c1_tx( (SlaveAdd & 0xFE) ) != 0)
		{
			if( count++ < REP_NUM) goto 	RETRY_WD;
			i2c1_stop();
			dbgmsg("Slave Addr Write Err\n"); 
			return false; 
		}
		if(i2c1_tx((uint8_t)Addr) != 0)
		{
			if( count++ < REP_NUM) goto 	RETRY_WD;
			i2c1_stop();
			dbgmsg("add Err\n"); 
			return false; 
		}
		for(uint32_t i=0; i < rSize; i++)
		{
			//TempData8 = *pData++;			
			if(i2c1_tx( (uint8_t)(*pData++) ) != 0)
			{
				if( count++ < REP_NUM) goto 	RETRY_WD;
				i2c1_stop();
				dbgmsg("Data wr err(%03d/%03d)\n",i+1,rSize);
				return false; 
			}
		}
		i2c1_stop();	
	}
	else if(Comm_Type == SPIMODE)	//spi
	{
		//dbgmsg("====[SPI_WRITE_DATA START]====\n");
						
		memset(rx_temp, 0, sizeof(rx_temp));
		memset(tx_temp, 0, sizeof(tx_temp));
		
		tx_temp[0] = 0xAA;					// Sync code
		tx_temp[1] = ((rSize+1)>>8)&0xff;	// tx size high byte
		tx_temp[2] = ((rSize+1)>>0)&0xff;	// tx size low byte
		tx_temp[3] = 0x00;					// rx size high byte
		tx_temp[4] = 0x00;					// rx size low byte
		tx_temp[5] = Addr;					// Command ID
		for(int i=0; i<rSize; i++)	tx_temp[6+i] = pData[i];		
		uint8_t checksum_tx = 0;
		for(int i=0; i<(6+rSize); i++)	checksum_tx += tx_temp[i];		
		tx_temp[6+rSize] = checksum_tx;		// check sum
		
		int tsize = 6+rSize+1;
		tsize = (tsize + 3) & ~3;	
		
		SPI1_CS(0);
		Delay_us(SPI_DELAY_CS);

		for (int i = 0; i < tsize; i++)
		{
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)  == RESET){};	SPI_I2S_SendData(SPI1,tx_temp[i]);  
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){};	SPI_I2S_ReceiveData(SPI1); 
		}
		SPI1_CS(1);
		 
			 if( Addr == 0x72 ) 	Delay_us(1000);
		else if( Addr == 0xD2 ) 	Delay_us(4000);
		else if( Addr == 0x21 ) 	Delay_us(40000);
		else if( Addr == 0x80 ) 	Delay_us(1000);
		else if( Addr == 0xD8 ) 	Delay_us(1000);
		else if( Addr == 0xD9 ) 	Delay_us(1000);
		else if( Addr == 0x60 ) 	Delay_us(1000);
		else if( Addr == 0xAE ) 	Delay_us(1000);
		else						Delay_us(1000);
	}
	return true;
}

///////////////////////////////////////////////////////
/////     		LsiModule				 	 //////////
///////////////////////////////////////////////////////
#define LSI_INIT 				1
#define LSI_VER					2
#define LSI_DATE				3
#define LSI_WRITE				4
#define LSI_INTX_PIN			5
#define LSI_TA_PIN				6
#define LSI_TSP_TEST			7		
#define LSI_TSP_RESULT			8		
#define LSI_TRX_SLOPE			9		
#define LSI_TRX_OPEN	 		10	//	
#define LSI_TRX_SH2GND			11	
#define LSI_TRX_SH2TRX			12
#define LSI_TRX_SH2RTX			13	
#define LSI_TA_HSYNC_SHORT  	14	
//#define						15
#define LSI_TA_PIN_656VQ    	16	// 200406 MSM
#define LSI_INTR2_CHECK     	17 	// 200512 MCH
#define LSI_SRAM_TEST       	18 	// 200629 MCH
#define LSI_IDLE_RESET_TEST		19 	// 200629 MCH
#define LSI_CM_OFFSET 			20//
//#define						21
//#define						22
//#define						23
#define LSI_CM_OFFSET_GAP		24
#define LSI_INTR2_SY792			25 	//AMB624XT01 INTR2 CMD Change
#define LSI_RESET_TEST			26 	//
#define	LSI_CHIP_ID 			27 	//201222 MCH
#define	LSI_TRIM_READ 			28 	//201222 MCH
//#define						29
#define LSI_CS_OFFSET_X			30//
//#define						31
//#define						32
//#define						33
//#define						34
#define LSI_CS_OFFSET_Y			35
//#define 						36
//#define						37
//#define						38
//#define						39
#define	LSI_SDC_CALIB			40//
//#define						41
//#define						42
//#define						43
//#define						44
#define LSI_CMS					45
#define LSI_CS_SET_X			46
#define LSI_CS_SET_Y			47
#define LSI_CMS8				48
//#define 						49
#define LSI_CMR_Noise			50//
#define LSI_CSR_Noise			51
//#define						52
//#define						53
//#define						54
//#define						55
//#define						56
#define LSI_CMR_P2P				57
#define LSI_CMR_P2P_MM			58
//#define						59
//#define						60//
//#define						61
#define LSI_CSR_X_P2P			62
#define LSI_CSR_X_P2P_MM		63
//#define						64
//#define						65
//#define						66
#define LSI_CSR_Y_P2P			67
#define LSI_CSR_Y_P2P_MM		68
//#define						69
#define	LSI_CM2_OFFSET			70//
#define	LSI_CM2_OFFSET_GAP		71
//#define						72
//#define						73
//#define						74
#define	LSI_CM3_OFFSET			75
#define	LSI_CM3_OFFSET_GAP		76
//#define						77
//#define						78
//#define						79
#define LSI_TX_SH_GND			80//
#define LSI_RX_SH_GND			81
#define LSI_TX_SH_TX			82
#define LSI_RX_SH_RX			83
#define LSI_TX_SH_RX			84
#define LSI_RX_SH_TX			85
//#define 						86
//#define 						87
//#define						88
//#define						89
#define LSI_TX_SH_GND_GAP 		90//
#define LSI_RX_SH_GND_GAP		91
#define LSI_TX_SH_TX_GAP		92
#define LSI_RX_SH_RX_GAP		93
#define LSI_TX_SH_RX_GAP		94
#define LSI_RX_SH_TX_GAP		95
//#define 						96
//#define						97
//#define						98
//#define						99
#define	LSI_nJITTER_CHECK		100//
#define	LSI_nCMR_P2P_AVR			101
#define	LSI_nCMR_P2P_MM			102 
#define	LSI_nCMR_P2P_GAP			103
#define	LSI_nCSR_X_P2P_AVR_TX	104
#define	LSI_nCSR_X_P2P_AVR_RX	105	
#define	LSI_nCSR_X_P2P_GAP_TX	106
#define	LSI_nCSR_X_P2P_GAP_RX	107
//#define						108
//#define						109
#define	LSI_SHO_CHECK			110//
//#define 						111
//#define						112
//#define						113
//#define						114
//#define 						115
//#define						116
//#define						117
//#define						118
//#define						119
#define	LSI_DIFF_TEST1			120	// only Y792 amb623xt01
#define	LSI_DIFF_TEST2			121	// only Y792 amb655xt09/16
#define	LSI_DIFF_TEST3			122	// only Y792 amb655xt08/15
#define	LSI_DIFF_TEST4			123	// only Y792 amb671XZ01
//#define						124	
#define	LSI_DIFF_MUTUAL_BASE 	125 // only xt01
#define	LSI_DIFF_MUTUAL_BASE_PP	126 // only xt01 	
#define	LSI_DIFF_SELF_BASE_TX	127	// only xt01 
#define	LSI_DIFF_SELF_BASE_RX	128	// only xt01 
//#define						129	// only xt01 
#define	LSI_DIFF_OFFSET1_TX 	130//
#define	LSI_DIFF_OFFSET1_RX		131
#define	LSI_DIFF_OFFSET2_TX		132
#define	LSI_DIFF_OFFSET2_RX		133
//#define						134
#define	LSI_DIFF_OFFSET_ABS_TX 	135
#define	LSI_DIFF_OFFSET_ABS_RX	136
#define	LSI_DIFF_GAP_TX			137
#define	LSI_DIFF_GAP_RX			138
//#define						139
//#define						140
	
uint8_t LsiModule(uint16_t index)
{	
	uint8_t ret = false;	
	dbgmsg("LSI Module %d\n", func_module[index]);
	switch(func_module[index])
	{						
		case LSI_INIT:					ret = LSI_Initial(index);			break;	// 	1
		case LSI_VER:					ret = LSI_Version(index);			break;	// 	2
		case LSI_DATE:					ret = LSI_Date(index);				break;	// 	3
		case LSI_WRITE:					ret = LSI_ImgWrite();				break;	// 	4
		case LSI_INTX_PIN:				ret = TINTX_Test();					break;	// 	5
		case LSI_TA_PIN:				ret = TAPIN_Test();					break;	// 	6
		case LSI_TSP_TEST:				ret = TSP_Test();					break;	// 	7
		case LSI_TSP_RESULT:						
			for(int Retry=0; Retry <2; Retry++)
			{
				ret = TSP_RESULT();			
				if(ret == true)	break;				
			}	
																			break;	//  8	
		case LSI_TRX_SLOPE:				ret = TRX_Slope();					break;	// 	9
		case LSI_TRX_OPEN:				ret = TRX_Open();					break;	// 	10
		case LSI_TRX_SH2GND:			ret = TRX_ShToGndGap();				break;	// 	11
		case LSI_TRX_SH2TRX:			ret = TRX_ShortToTXRX();			break;	// 	12
		case LSI_TRX_SH2RTX:			ret = TRX_ShortToRXTX();			break;	// 	13
		case LSI_TA_HSYNC_SHORT:		ret = TA_HSYNC_Test();				break;	// 	14	
		//																			//	15
		case LSI_TA_PIN_656VQ:			ret = TAPIN_Test_656VQ();			break;	//	16	
		case LSI_INTR2_CHECK:			ret = INTR2_Test();					break;	//	17
		case LSI_SRAM_TEST:				ret = SRAM_Test();					break;	//	18
		case LSI_IDLE_RESET_TEST:		ret = Idle_Reset_Test();			break;	//	19	
		case LSI_CM_OFFSET:				ret = Cm_Offset(index);				break;	//	20
		//																			//	21
		//																			//	22
		//																			//	23
		case LSI_CM_OFFSET_GAP:			ret = Cm_Offset_Gap(index);			break;	//	24
		case LSI_INTR2_SY792:			ret = INTR2_Test_SY792();			break;	//	25
		case LSI_RESET_TEST:			ret = RESET_Test();					break;	//	26
		case LSI_CHIP_ID:				ret = CHIP_ID_Check();				break;	//	27
		case LSI_TRIM_READ:				ret = TRIM_Check(index);			break;	//	28
		//																			//	29
		case LSI_CS_OFFSET_X:			ret = Cs_Offset_X(index);			break;	//	30
		//																			//	31
		//																			//	32
		//																			//	33
		//																			//	34
		case LSI_CS_OFFSET_Y:			ret = Cs_Offset_Y(index);			break;	//	35
		//																			//	36
		//																			//	37
		//																			//	38
		//																			//	39
		case LSI_SDC_CALIB:								
			for(int Retry=0; Retry <2; Retry++)
			{
				ret = SDC_Calibration();			
				if(ret == true)	break;				
			}
																			break;	//	40
		//																			//	41
		//																			//	42
		//																			//	43
		//																			//	44
		case LSI_CMS:					ret = Cm_Set(index);				break;	//	45
		case LSI_CS_SET_X:				ret = Cs_Set_X(index);				break;	//	46
		case LSI_CS_SET_Y: 				ret = Cs_Set_Y(index);				break;	//	47
		case LSI_CMS8: 					ret = Cm_Set8(index);				break;	//	48
		//																			//	49		
		case LSI_CMR_Noise:				ret = Noise_Check();				break;	//	50
		case LSI_CSR_Noise:				ListView_MEAS("OK");ret = true;		break;	//	51 LSI_CMR_Noise Result share
		//																			//	52
		//																			//	53
		//																			//	54
		//																			//	55
		//																			//	56
		case LSI_CMR_P2P:				ret = CMR_P2P(index);				break;	//	57
		case LSI_CMR_P2P_MM:			ret = CMR_P2P_MM(index);			break;	//	58
		//																			//	59
		//																			//	60
		//																			//	61
		case LSI_CSR_X_P2P:				ret = CSR_X_P2P(index);				break;	//	62
		case LSI_CSR_X_P2P_MM:			ret = CSR_X_P2P_MM(index);			break;	//	63
		//																			//	64
		//																			//	65
		//																			//	66
		case LSI_CSR_Y_P2P:				ret = CSR_Y_P2P(index);				break;	//	67
		case LSI_CSR_Y_P2P_MM:			ret = CSR_Y_P2P_MM(index);			break;	//	68
		//																			//	69			
		case LSI_CM2_OFFSET:			ret = CM2_Offset(index);			break;	//	70
		case LSI_CM2_OFFSET_GAP:		ret = CM2_Offset_Gap(index);		break;	//	71
		//																			//	72
		//																			//	73
		//																			//	74
		case LSI_CM3_OFFSET:			ret = CM3_Offset(index);			break;	//	75
		case LSI_CM3_OFFSET_GAP:		ret = CM3_Offset_Gap(index);		break;	//	76
		//																			//	77
		//																			//	78
		//																			//	79		
		case LSI_TX_SH_GND:				ret = TX_SH_GND(index);				break; 	//	80
		case LSI_RX_SH_GND:				ret = RX_SH_GND(index);				break; 	//	81
		case LSI_TX_SH_TX:				ret = TX_SH_TX(index);				break; 	//	82
		case LSI_RX_SH_RX:				ret = RX_SH_RX(index);				break; 	//	83
		case LSI_TX_SH_RX:				ret = TX_SH_RX(index);				break; 	//	84
		case LSI_RX_SH_TX:				ret = RX_SH_TX(index);				break; 	//	85
		//																			//	86
		//																			//	87
		//																			//	88
		//																			//	89
		case LSI_TX_SH_GND_GAP:			ret = TX_SH_GND_GAP(index);			break;	//	90
		case LSI_RX_SH_GND_GAP:			ret = RX_SH_GND_GAP (index);		break;	//	91
		case LSI_TX_SH_TX_GAP:			ret = TX_SH_TX_GAP(index);			break;	//	92
		case LSI_RX_SH_RX_GAP:			ret = RX_SH_RX_GAP(index);			break;	//	93
		case LSI_TX_SH_RX_GAP:			ret = TX_SH_RX_GAP(index);			break;	//	94
		case LSI_RX_SH_TX_GAP:			ret = RX_SH_TX_GAP(index);			break;	//	95
		//																			//	96
		//																			//	97
		//																			//	98
		//																			//	99
		case LSI_nJITTER_CHECK:			ret = nNoise_Check_New();			break;	//	100
		case LSI_nCMR_P2P_AVR: 			ret = nCMR_P2P_AVR(index);			break;	//	101
		case LSI_nCMR_P2P_MM: 			ret = nCMR_P2P_MM(index);			break;	//	102
		case LSI_nCMR_P2P_GAP: 			ret = nCMR_P2P_GAP(index);			break;	//	103
		case LSI_nCSR_X_P2P_AVR_TX:		ret = nCSR_X_P2P_AVR(index);		break;	//	104
		case LSI_nCSR_X_P2P_AVR_RX:		ret = nCSR_Y_P2P_AVR(index);		break;	//	105	
		case LSI_nCSR_X_P2P_GAP_TX:		ret = nCSR_X_P2P_GAP(index);		break;	//	106
		case LSI_nCSR_X_P2P_GAP_RX:		ret = nCSR_Y_P2P_GAP(index);		break;	//	107
		//																			//	108
		//																			//	109
		case LSI_SHO_CHECK: 			ret = SHO_TEST(index);				break;	//	110
		//																			//	111
		//																			//	112
		//																			//	113
		//																			//	114
		//																			//	115
		//																			//	116
		//																			//	117
		//																			//	118
		//																			//	119
		case LSI_DIFF_TEST1: 			ret = Diff_Test1(index);			break;	//	120
		case LSI_DIFF_TEST2: 			ret = Diff_Test2(index);			break;	//	121
		case LSI_DIFF_TEST3: 			ret = Diff_Test3(index);			break;	//	122
		case LSI_DIFF_TEST4: 			ret = Diff_Test4(index);			break;	//	123
		//																			//	124				
		case LSI_DIFF_MUTUAL_BASE:		ret = Diff_Cm_Base(index);			break;	//	125
		case LSI_DIFF_MUTUAL_BASE_PP:	ret = Diff_Cm_PP(index);			break;	//	126
		case LSI_DIFF_SELF_BASE_TX:		ret = Diff_Cs_Base_Tx(index);		break;	//	127
		case LSI_DIFF_SELF_BASE_RX:		ret = Diff_Cs_Base_Rx(index);		break;	//	128	
		//																			//	129	
		case LSI_DIFF_OFFSET1_TX:		ret = Diff_Offset1_Tx(index);		break;	//	130 
		case LSI_DIFF_OFFSET1_RX:		ret = Diff_Offset1_Rx(index);		break;	//	131
		case LSI_DIFF_OFFSET2_TX:		ret = Diff_Offset2_Tx(index);		break;	//	132		
		case LSI_DIFF_OFFSET2_RX:		ret = Diff_Offset2_Rx(index);		break;	//	133
		//									 										//	134						
		case LSI_DIFF_OFFSET_ABS_TX:	ret = Diff_Offset_Abs_Tx(index);	break;	//	135
		case LSI_DIFF_OFFSET_ABS_RX:	ret = Diff_Offset_Abs_Rx(index);	break;	//	136
		case LSI_DIFF_GAP_TX: 			ret = Diff_Offset_Gap_Tx(index);	break;	//	137
		case LSI_DIFF_GAP_RX: 			ret = Diff_Offset_Gap_Rx(index);	break;	//	138	
	}
	return ret;
}

#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
