#include "task_data_pc.h"

static char buff[10]={0};

uint8_t data_erase_pc(void)
{		
	FLASH_Unlock();
	dbgmsg("data_erase : FLASH_Sector_12\n");
	FLASH_EraseSector( FLASH_Sector_12, VoltageRange_3);	
	buff[0] = 1;buff[1] = 8;	tx_pc_data(INPUT_DATA_ERASE, 2, buff);
	ntDelay_ms(100);
	
	dbgmsg("data_erase : FLASH_Sector_13\n");
	FLASH_EraseSector( FLASH_Sector_13, VoltageRange_3);	
	buff[0] = 2;buff[1] = 8;	tx_pc_data(INPUT_DATA_ERASE, 2, buff);	
	ntDelay_ms(100);
	
	dbgmsg("data_erase : FLASH_Sector_14\n");
	FLASH_EraseSector( FLASH_Sector_14, VoltageRange_3);	
	buff[0] = 3;buff[1] = 8;	tx_pc_data(INPUT_DATA_ERASE, 2, buff);
	ntDelay_ms(100);
	
	dbgmsg("data_erase : FLASH_Sector_15\n");
	FLASH_EraseSector( FLASH_Sector_15, VoltageRange_3);	
	buff[0] = 4;buff[1] = 8;	tx_pc_data(INPUT_DATA_ERASE, 2, buff);
	ntDelay_ms(100);
	
	dbgmsg("data_erase : FLASH_Sector_16\n");
	FLASH_EraseSector( FLASH_Sector_16, VoltageRange_3);	
	buff[0] = 5;buff[1] = 8;	tx_pc_data(INPUT_DATA_ERASE, 2, buff);
	ntDelay_ms(100);
	
	dbgmsg("data_erase : FLASH_Sector_17\n");
	FLASH_EraseSector( FLASH_Sector_17, VoltageRange_3);	
	buff[0] = 6;buff[1] = 8;	tx_pc_data(INPUT_DATA_ERASE, 2, buff);
	ntDelay_ms(100);
	
	dbgmsg("data_erase : FLASH_Sector_18\n");	
	FLASH_EraseSector( FLASH_Sector_18, VoltageRange_3);	
	buff[0] = 7;buff[1] = 8;	tx_pc_data(INPUT_DATA_ERASE, 2, buff);
	ntDelay_ms(100);
	
	dbgmsg("data_erase : FLASH_Sector_19\n");
	FLASH_EraseSector( FLASH_Sector_19, VoltageRange_3);	
	buff[0] = 8;buff[1] = 8;	tx_pc_data(INPUT_DATA_ERASE, 2, buff);
	ntDelay_ms(100);
	
	FLASH_Lock();
	return true;
}

uint8_t data_write_pc(char *data)
{
	int cur_page =  	( ((uint16_t)data[0] << 8 ) + (uint16_t)data[1] ) ;
	int total_page =  	( ((uint16_t)data[2] << 8 ) + (uint16_t)data[3] ) ;
	int tx_size =  		( ((uint16_t)data[4] << 8 ) + (uint16_t)data[5] ) ;

	uint32_t data_add = INPUTDATA_ADDRESS + (cur_page * tx_size);
	
	FLASH_Unlock();
	
	for(int cnt = 0; cnt<tx_size; cnt++)
		FLASH_ProgramByte(data_add + cnt ,data[6+cnt]);
	
	FLASH_Lock();
	
	tx_pc_data(INPUT_DATA_WR, 4, data);
}
uint8_t DataMake_PC(char *data)
{
	FLASH_Unlock();
	
	char* ptr_year = strtok(data,",");
	char* ptr_mon = strtok(NULL,",");
	char* ptr_day = strtok(NULL,",");
	char* ptr_hour = strtok(NULL,",");
	char* ptr_min = strtok(NULL,",");
	char* ptr_size = strtok(NULL,",");
	char* ptr_name = strtok(NULL,",");
	char* ptr_crc32 = strtok(NULL,",");

	FLASH_ProgramWord(DATA_SIZE_ADD, atoi(ptr_size) );	// SPEC file down date 
	for(int i = 0; i < 180; i++)	
	{
		FLASH_ProgramByte(DATA_NAME_ADD+i,ptr_name[i]);
	}
	
	FLASH_ProgramWord(DATA_SAVE_YEAR_ADD, 	atoi(ptr_year) & 0xff );	// SPEC file down year 
	FLASH_ProgramWord(DATA_SAVE_MONTH_ADD, 	atoi(ptr_mon)  & 0xff );	// SPEC file down month
	FLASH_ProgramWord(DATA_SAVE_DAY_ADD, 	atoi(ptr_day)  & 0xff );	// SPEC file down date
	FLASH_ProgramWord(DATA_SAVE_HOUR_ADD, 	atoi(ptr_hour) & 0xff); 	// SPEC file down year 
	FLASH_ProgramWord(DATA_SAVE_MIN_ADD, 	atoi(ptr_min)  & 0xff);		// SPEC file down month
	FLASH_ProgramWord(DATA_SAVE_FLAG_ADD, DATA_CHECK_FLAG );			// SPEC file down date
	
	FLASH_ProgramWord(DATA_CRC32_ADD, strtoul(ptr_crc32, NULL, 16) );	// SPEC file down date 
	
	dbgmsg("20%02d-%02d-%02d, %02d:%02d[%X] Input Data Save OK![%X]\n",DATA_SAVE_YEAR,DATA_SAVE_MONTH,DATA_SAVE_DAY,DATA_SAVE_HOUR,DATA_SAVE_MIN,DATA_SAVE_FLAG);
		
	UpdateDatafile();
	DataUpdateGui();
	
	uint32_t crc_temp = set_crc32_init_value(CRC_INITIALVALUE);	
	for(int i=0;i<In_data.size;i++)		crc_temp = crc32_table[((crc_temp >> 24) ^ INPUT_DATA[i])] ^ (crc_temp << 8);
	
	dbgmsg("In_data.crc32: %08X \n", In_data.crc32);
	dbgmsg("crc_temp: %08X \n", crc_temp);
	
	if(In_data.crc32 == crc_temp)
	{
		buff[0] = true;		
	}
	else
	{
		buff[0] = false;
	}
	tx_pc_data(INPUT_DATA_FINSH, 5, buff);	
	
	FLASH_Lock();
	
	return true;
}

uint8_t data_send_pc(void)
{	
	buff[0] = (In_data.crc32 >>24) & 0xff;		
	buff[1] = (In_data.crc32 >>16) & 0xff;		
	buff[2] = (In_data.crc32 >> 8) & 0xff;		
	buff[3] = (In_data.crc32 >> 0) & 0xff;		
	
	tx_pc_data(INPUT_DATA_RD, 4, buff);
	
	return true;
}

uint8_t pmic_erase_pc(void)
{
	FLASH_Unlock();
	dbgmsg("data_erase : FLASH_Sector_9\n");
	FLASH_EraseSector( FLASH_Sector_9, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector9\nErase OK");	
	buff[0] = 1;buff[1] = 1;	tx_pc_data(PMIC_DATA_ERASE, 2, buff);
	ntDelay_ms(100);
	FLASH_Lock();	
	return true;
}
uint8_t eep_erase_pc(void)
{
	FLASH_Unlock();
	
	dbgmsg("data_erase : FLASH_Sector_10\n");
	FLASH_EraseSector( FLASH_Sector_10, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector10\nErase OK");
	buff[0] = 1;buff[1] = 1;	tx_pc_data(EEP_DATA_ERASE, 2, buff);
	ntDelay_ms(100);
	FLASH_Lock();	
	return true;
}

uint8_t tpic_erase_pc(void)
{
	FLASH_Unlock();
	dbgmsg("data_erase : FLASH_Sector_11\n");
	FLASH_EraseSector( FLASH_Sector_11, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector11\nErase OK");	
	buff[0] = 1;buff[1] = 1;	tx_pc_data(TPIC_DATA_ERASE, 2, buff);
	ntDelay_ms(100);
	FLASH_Lock();	
	return true;
}
uint8_t pmic_write_pc(char *data)
{
	int cur_page =  	( ((uint16_t)data[0] << 8 ) + (uint16_t)data[1] ) ;
	int total_page =  	( ((uint16_t)data[2] << 8 ) + (uint16_t)data[3] ) ;
	int tx_size =  		( ((uint16_t)data[4] << 8 ) + (uint16_t)data[5] ) ;

	uint32_t data_add = PMICDATA_ADDRESS + (cur_page * tx_size);

	FLASH_Unlock();
	
	for(int cnt = 0; cnt<tx_size; cnt++) //221108 MCH
	{
	FLASH_ProgramByte(data_add + cnt ,data[6+cnt]);
	dbgmsg("%02X\n",data[6+cnt]);
	}
	FLASH_Lock();
	
	tx_pc_data(PMIC_DATA_WR, 4, data);
}
uint8_t eep_write_pc(char *data)
{
	int cur_page =  	( ((uint16_t)data[0] << 8 ) + (uint16_t)data[1] ) ;
	int total_page =  	( ((uint16_t)data[2] << 8 ) + (uint16_t)data[3] ) ;
	int tx_size =  		( ((uint16_t)data[4] << 8 ) + (uint16_t)data[5] ) ;

	uint32_t data_add = EEPDATA_ADDRESS + (cur_page * tx_size);

	FLASH_Unlock();
	for(int cnt = 0; cnt<tx_size; cnt++)
	{
		FLASH_ProgramByte(data_add + cnt ,data[6+cnt]);
		dbgmsg("%02X, ",data[6+cnt]);
		if((cnt % 16 ==0)&&(cnt > 1))dbgmsg("\n");
	}
	FLASH_Lock();
	
	tx_pc_data(EEP_DATA_WR, 4, data);
}

uint8_t tpic_write_pc(char *data)
{
	int cur_page =  	( ((uint16_t)data[0] << 8 ) + (uint16_t)data[1] ) ;
	int total_page =  	( ((uint16_t)data[2] << 8 ) + (uint16_t)data[3] ) ;
	int tx_size =  		( ((uint16_t)data[4] << 8 ) + (uint16_t)data[5] ) ;

	uint32_t data_add = TPICDATA_ADDRESS + (cur_page * tx_size);

	FLASH_Unlock();
	for(int cnt = 0; cnt<tx_size; cnt++)
	{
		FLASH_ProgramByte(data_add + cnt ,data[6+cnt]);
		dbgmsg("%02X, ",data[6+cnt]);
		if((cnt % 16 ==0)&&(cnt !=0))dbgmsg("\n");
	}
	FLASH_Lock();
	
	tx_pc_data(TPIC_DATA_WR, 4, data);
}
uint8_t PmicMake_PC(char *data)
{
	FLASH_Unlock();
	
	char* ptr_year = strtok(data,",");
	char* ptr_mon = strtok(NULL,",");
	char* ptr_day = strtok(NULL,",");
	char* ptr_hour = strtok(NULL,",");
	char* ptr_min = strtok(NULL,",");
	char* ptr_size = strtok(NULL,",");
	char* ptr_name = strtok(NULL,",");
	char* ptr_crc32 = strtok(NULL,",");

	FLASH_ProgramWord(PMIC_SIZE_ADD, atoi(ptr_size) );	// SPEC file down date 
	for(int i = 0; i < 180; i++)	
	{
		FLASH_ProgramByte(PMIC_NAME_ADD+i,ptr_name[i]);
	}
	
	FLASH_ProgramWord(PMIC_SAVE_YEAR_ADD, 	atoi(ptr_year) & 0xff );	// SPEC file down year 
	FLASH_ProgramWord(PMIC_SAVE_MONTH_ADD, 	atoi(ptr_mon)  & 0xff );	// SPEC file down month
	FLASH_ProgramWord(PMIC_SAVE_DAY_ADD, 	atoi(ptr_day)  & 0xff );	// SPEC file down date
	FLASH_ProgramWord(PMIC_SAVE_HOUR_ADD, 	atoi(ptr_hour) & 0xff); 	// SPEC file down year 
	FLASH_ProgramWord(PMIC_SAVE_MIN_ADD, 	atoi(ptr_min)  & 0xff);		// SPEC file down month
	FLASH_ProgramWord(PMIC_SAVE_FLAG_ADD, 	PMIC_CHECK_FLAG );			// SPEC file down date
	
	FLASH_ProgramWord(PMIC_CRC32_ADD, strtoul(ptr_crc32, NULL, 16) );	// SPEC file down date 
	
	dbgmsg("20%02d-%02d-%02d, %02d:%02d[%X] PMIC Data Save OK![%X]\n",PMIC_SAVE_YEAR,PMIC_SAVE_MONTH,PMIC_SAVE_DAY,PMIC_SAVE_HOUR,PMIC_SAVE_MIN,PMIC_SAVE_FLAG);
		
	Updatepmicfile();
	pmicUpdateGui();
	
	uint32_t crc_temp = set_crc32_init_value(CRC_INITIALVALUE);	
	for(int i=0;i<In_pmic.size;i++)		crc_temp = crc32_table[((crc_temp >> 24) ^ INPUT_PMIC[i])] ^ (crc_temp << 8);
	
	dbgmsg("In_pmic.crc32: %08X \n", In_pmic.crc32);
	dbgmsg("crc_temp: %08X \n", crc_temp);
	if(In_pmic.crc32 == crc_temp)
	{
		buff[0] = true;		
	}
	else
	{
		buff[0] = false;
	}
	tx_pc_data(PMIC_DATA_FINSH, 5, buff);	
	
	FLASH_Lock();
	
	return true;
}

uint8_t EepMake_PC(char *data)
{
	FLASH_Unlock();
	
	char* ptr_year = strtok(data,",");
	char* ptr_mon = strtok(NULL,",");
	char* ptr_day = strtok(NULL,",");
	char* ptr_hour = strtok(NULL,",");
	char* ptr_min = strtok(NULL,",");
	char* ptr_size = strtok(NULL,",");
	char* ptr_name = strtok(NULL,",");
	char* ptr_crc32 = strtok(NULL,",");

	FLASH_ProgramWord(EEP_SIZE_ADD, atoi(ptr_size) );	// SPEC file down date 
	for(int i = 0; i < 180; i++)	
	{
		FLASH_ProgramByte(EEP_NAME_ADD+i,ptr_name[i]);
	}
	
	FLASH_ProgramWord(EEP_SAVE_YEAR_ADD, 	atoi(ptr_year) & 0xff );	// SPEC file down year 
	FLASH_ProgramWord(EEP_SAVE_MONTH_ADD, 	atoi(ptr_mon)  & 0xff );	// SPEC file down month
	FLASH_ProgramWord(EEP_SAVE_DAY_ADD, 	atoi(ptr_day)  & 0xff );	// SPEC file down date
	FLASH_ProgramWord(EEP_SAVE_HOUR_ADD, 	atoi(ptr_hour) & 0xff); 	// SPEC file down year 
	FLASH_ProgramWord(EEP_SAVE_MIN_ADD, 	atoi(ptr_min)  & 0xff);		// SPEC file down month
	FLASH_ProgramWord(EEP_SAVE_FLAG_ADD, 	EEP_CHECK_FLAG );			// SPEC file down date
	
	FLASH_ProgramWord(EEP_CRC32_ADD, strtoul(ptr_crc32, NULL, 16) );	// SPEC file down date 
	
	dbgmsg("20%02d-%02d-%02d, %02d:%02d[%X] EEP Data Save OK![%X]\n",EEP_SAVE_YEAR,EEP_SAVE_MONTH,EEP_SAVE_DAY,EEP_SAVE_HOUR,EEP_SAVE_MIN,EEP_SAVE_FLAG);
		
	Updateeepfile();
	eepUpdateGui();
	
	uint32_t crc_temp = set_crc32_init_value(CRC_INITIALVALUE);	
	for(int i=0;i<In_eep.size;i++)		crc_temp = crc32_table[((crc_temp >> 24) ^ INPUT_EEP[i])] ^ (crc_temp << 8);
	
	dbgmsg("In_eep.size: %d \n", In_eep.size);
	dbgmsg("In_eep.crc32: %08X \t\t calculated crc_temp: %08X \n", In_eep.crc32, crc_temp);
	
	if(In_eep.crc32 == crc_temp)
	{
		buff[0] = true;		
	}
	else
	{
		buff[0] = false;
	}
	tx_pc_data(EEP_DATA_FINSH, 5, buff);	
	
	FLASH_Lock();
	
	return true;
}

uint8_t TpicMake_PC(char *data)
{
	FLASH_Unlock();
	
	char* ptr_year = strtok(data,",");
	char* ptr_mon = strtok(NULL,",");
	char* ptr_day = strtok(NULL,",");
	char* ptr_hour = strtok(NULL,",");
	char* ptr_min = strtok(NULL,",");
	char* ptr_size = strtok(NULL,",");
	char* ptr_name = strtok(NULL,",");
	char* ptr_crc32 = strtok(NULL,",");

	FLASH_ProgramWord(TPIC_SIZE_ADD, atoi(ptr_size) );	// SPEC file down date 
	for(int i = 0; i < 180; i++)	
	{
		FLASH_ProgramByte(TPIC_NAME_ADD+i,ptr_name[i]);
	}
	
	FLASH_ProgramWord(TPIC_SAVE_YEAR_ADD, 	atoi(ptr_year) & 0xff );	// SPEC file down year 
	FLASH_ProgramWord(TPIC_SAVE_MONTH_ADD, 	atoi(ptr_mon)  & 0xff );	// SPEC file down month
	FLASH_ProgramWord(TPIC_SAVE_DAY_ADD, 	atoi(ptr_day)  & 0xff );	// SPEC file down date
	FLASH_ProgramWord(TPIC_SAVE_HOUR_ADD, 	atoi(ptr_hour) & 0xff); 	// SPEC file down year 
	FLASH_ProgramWord(TPIC_SAVE_MIN_ADD, 	atoi(ptr_min)  & 0xff);		// SPEC file down month
	FLASH_ProgramWord(TPIC_SAVE_FLAG_ADD, 	TPIC_CHECK_FLAG );			// SPEC file down date
	
	FLASH_ProgramWord(TPIC_CRC32_ADD, strtoul(ptr_crc32, NULL, 16) );	// SPEC file down date 
	
	dbgmsg("20%02d-%02d-%02d, %02d:%02d[%X] TPIC Data Save OK![%X]\n",TPIC_SAVE_YEAR,TPIC_SAVE_MONTH,TPIC_SAVE_DAY,TPIC_SAVE_HOUR,TPIC_SAVE_MIN,TPIC_SAVE_FLAG);
		
	Updatetpicfile();
	pmicDisp();
	
	uint32_t crc_temp = set_crc32_init_value(CRC_INITIALVALUE);	
	for(int i=0;i<In_tpic.size;i++)		crc_temp = crc32_table[((crc_temp >> 24) ^ INPUT_TPIC[i])] ^ (crc_temp << 8);
	
	dbgmsg("In_TPIC.size: %d \n", In_tpic.size);
	dbgmsg("In_TPIC.crc32: %08X \t\t calculated crc_temp: %08X \n", In_tpic.crc32, crc_temp);
	
	if(In_tpic.crc32 == crc_temp)
	{
		buff[0] = true;		
	}
	else
	{
		buff[0] = false;
	}
	tx_pc_data(TPIC_DATA_FINSH, 5, buff);	
	
	FLASH_Lock();
	
	return true;
}

uint8_t pmic_send_pc(void)
{	
	buff[0] = (In_pmic.crc32 >>24) & 0xff;		
	buff[1] = (In_pmic.crc32 >>16) & 0xff;		
	buff[2] = (In_pmic.crc32 >> 8) & 0xff;		
	buff[3] = (In_pmic.crc32 >> 0) & 0xff;		
	
	//dbgmsg("PMIC : %d,%d,%d,%d\n",buff[0],buff[1],buff[2],buff[3]);
	tx_pc_data(PMIC_DATA_RD, 4, buff);
	
	return true;
}
uint8_t eep_send_pc(void)
{	
	buff[0] = (In_eep.crc32 >>24) & 0xff;		
	buff[1] = (In_eep.crc32 >>16) & 0xff;		
	buff[2] = (In_eep.crc32 >> 8) & 0xff;		
	buff[3] = (In_eep.crc32 >> 0) & 0xff;		
	
	//dbgmsg("EEPROM : %d,%d,%d,%d\n",buff[0],buff[1],buff[2],buff[3]);
	tx_pc_data(EEP_DATA_RD, 4, buff);
	
	return true;
}
uint8_t tpic_send_pc(void)
{	
	buff[0] = (In_tpic.crc32 >>24) & 0xff;		
	buff[1] = (In_tpic.crc32 >>16) & 0xff;		
	buff[2] = (In_tpic.crc32 >> 8) & 0xff;		
	buff[3] = (In_tpic.crc32 >> 0) & 0xff;		
	
	//dbgmsg("TPIC :%d,%d,%d,%d\n",buff[0],buff[1],buff[2],buff[3]);
	
	tx_pc_data(TPIC_DATA_RD, 4, buff);
	
	return true;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
