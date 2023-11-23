#include "task_data_save.h"

void UpdateDatafile(void)
{
	if( DATA_SAVE_FLAG == DATA_CHECK_FLAG )
	{
		In_data.size = (*( (uint32_t *)DATA_SIZE_ADD));
		In_data.name = (((char*)DATA_NAME_ADD));
		In_data.crc32 = (*( (uint32_t *)DATA_CRC32_ADD));
	}
	else
	{
		In_data.size = 0;
		In_data.name = "No File";
		In_data.crc32 = 0;
	}
}
	
void DataUpdateGui(void)
{
	ProjectDisplay();
	DataDisp();
}
void DataDisp(void)
{
	char text[1024];    
	hItem = WM_GetDialogItem(hSyswin1,MENU1_DATA_ST_TEXT);
	TEXT_SetTextColor(hItem, GUI_WHITE);	
	TEXT_SetBkColor(hItem, GUI_DARKBLUE);	
	sprintf(text, " - %s\n - size : %.2f kB", In_data.name, (float)((float)In_data.size/1024));
	TEXT_SetText(hItem,text);
	GUI_Exec();
}
FATFS dfatfs;
FIL dfile;
DIR ddir;
char data_file_path[255+21] = {NULL};
FILINFO datafileinfo;
uint16_t file_number = 0;
int  ipd_data_size = 0;
char ipd_name[180] = {NULL};
uint8_t check_data_file(void)
{
	char *fn;	
	if ( f_mount( 0, &dfatfs ) == FR_OK )
	{			
		static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */ 
		datafileinfo.lfname = lfn; 
		datafileinfo.lfsize = sizeof lfn; 
		
		if(f_opendir(&ddir, "0:/Spec/03.DATA_FILE") == FR_OK)
		{
			file_number = 0;
			
			while(1)
			{
				if(f_readdir(&ddir, &datafileinfo) == FR_OK)
				{
					if (datafileinfo.fname[0] == 0)		break; 
					if (datafileinfo.fname[0] == '.')	continue; 				
					fn = *datafileinfo.lfname ? datafileinfo.lfname : datafileinfo.fname; 
					ipd_data_size = datafileinfo.fsize;
					dbgmsg("file name:[%s], size:[%d]\r\n", fn, ipd_data_size);
					sprintf(ipd_name, "%s",fn);
					sprintf(data_file_path, "0:/Spec/03.DATA_FILE/%s",fn);					
					file_number++;
				}
			}
		}
		f_mount(0, NULL);
		
		if(file_number == 1)	return true;
		else					return false;		
	}
	else
	{
		return false;
	}
}

uint8_t data_erase(void)
{		
	FLASH_Unlock();
	dbgmsg("data_erase : FLASH_Sector_12\n");
	FLASH_EraseSector( FLASH_Sector_12, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector12\nErase OK");	
	dbgmsg("data_erase : FLASH_Sector_13\n");
	FLASH_EraseSector( FLASH_Sector_13, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector13\nErase OK");	
	dbgmsg("data_erase : FLASH_Sector_14\n");
	FLASH_EraseSector( FLASH_Sector_14, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector14\nErase OK");	
	dbgmsg("data_erase : FLASH_Sector_15\n");
	FLASH_EraseSector( FLASH_Sector_15, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector15\nErase OK");	
	dbgmsg("data_erase : FLASH_Sector_16\n");
	FLASH_EraseSector( FLASH_Sector_16, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector16\nErase OK");	
	dbgmsg("data_erase : FLASH_Sector_17\n");
	FLASH_EraseSector( FLASH_Sector_17, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector17\nErase OK");	
	dbgmsg("data_erase : FLASH_Sector_18\n");
	FLASH_EraseSector( FLASH_Sector_18, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector18\nErase OK");	
	dbgmsg("data_erase : FLASH_Sector_19\n");
	FLASH_EraseSector( FLASH_Sector_19, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector19\nErase OK");	
	FLASH_Lock();	
	return true;
}
uint8_t data_write(void)
{	
	if ( f_mount( 0, &dfatfs ) == FR_OK )
	{
		if(f_open(&dfile, data_file_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			
			int cnt=0, cnt1=0;
			char ReadTextBuff[100]={0};
			char text[50];    
			
			cnt = cnt1 = 0;
			dbgmsg("Data Write  : %s\n",data_file_path);		
			while( f_gets_FW(ReadTextBuff, 17, &dfile) )
			{
				for(cnt = 0; cnt <16;cnt++) 
				{                    
					if(cnt1 < dfile.fsize)
					{     
						FLASH_ProgramByte(INPUTDATA_ADDRESS + cnt1 ,ReadTextBuff[cnt]);
						cnt1++;
					}
					if(cnt1 % 1024 == 0)
					{
						sprintf(text,"Download\n%d kB/ %ld kB\n", cnt1/1024, dfile.fsize/1024);
						Sys1Popup(true, GUI_DARKBLUE, text);
					}
				}
			}
			FLASH_Lock();
		}
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&dfile);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}

uint8_t data_verify(void)
{	
	if ( f_mount( 0, &dfatfs ) == FR_OK )
	{
		if(f_open(&dfile, data_file_path, FA_READ) == FR_OK)
		{
			int cnt, cnt1, err_verify;
			char ReadTextBuff[100]={0};
			char text[50];    
			
			dbgmsg("Data Verify : %s\n",data_file_path);
		
			err_verify = cnt = cnt1 = 0;
			while( f_gets_FW(ReadTextBuff, 17, &dfile) )
			{
				for(cnt = 0; cnt <16;cnt++) 
				{
					if(cnt1 < dfile.fsize)
					{
						if(INPUT_DATA[cnt1] != ReadTextBuff[cnt])
						{
							err_verify = 1;
							break;
						}
						cnt1++;
					}
					if(cnt1 % 1024 == 0)
					{
						sprintf(text,"Verify\n%d kB/ %ld kB\n", cnt1/1024, dfile.fsize/1024);
						Sys1Popup(true, GUI_DARKBLUE, text);
					}
					if(err_verify == 1) break;
				}
			}
		}	
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&dfile);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}

uint8_t DataMake(void)
{
	FLASH_Unlock();	
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);		
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);			
			
	FLASH_ProgramWord(DATA_SIZE_ADD, ipd_data_size );	// SPEC file down date 
	for(int i = 0; i < 180; i++)	
	{
		FLASH_ProgramByte(DATA_NAME_ADD+i,ipd_name[i]);
	}

	FLASH_ProgramWord(DATA_SAVE_YEAR_ADD,  RTC_DateStr.RTC_Year  & 0xff );  // SPEC file down year 
	FLASH_ProgramWord(DATA_SAVE_MONTH_ADD, RTC_DateStr.RTC_Month & 0xff );	// SPEC file down month
	FLASH_ProgramWord(DATA_SAVE_DAY_ADD,   RTC_DateStr.RTC_Date  & 0xff );	// SPEC file down date
		
	FLASH_ProgramWord(DATA_SAVE_HOUR_ADD, RTC_TimeStr.RTC_Hours  & 0xff );  // SPEC file down year 
	FLASH_ProgramWord(DATA_SAVE_MIN_ADD, RTC_TimeStr.RTC_Minutes & 0xff );	// SPEC file down month
	
	FLASH_ProgramWord(DATA_SAVE_FLAG_ADD, DATA_CHECK_FLAG );	// SPEC file down date
	
	dbgmsg("20%02d-%02d-%02d, %02d:%02d[%X] Input Data Save OK!\n",DATA_SAVE_YEAR,DATA_SAVE_MONTH,DATA_SAVE_DAY,DATA_SAVE_HOUR,DATA_SAVE_MIN,DATA_SAVE_FLAG);
	
	FLASH_Lock();
	
	return true;
}
void DataSaveflow(void)
{
	if(usb_host_connected_flag == 0)
	{
		Sys1Popup(true, GUI_DARKRED,"USB Unmount\n(Press ESC KEY)");
		Beep(250);
	}
	else
	{
		Sys1Popup(true, GUI_DARKBLUE, "USB Mount");
		Beep(250);
		
		if(check_data_file() == true)
		{			
			if(data_erase()==false) {goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Data Area\nErase  OK");		
			if(data_write()==false)	{goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Data File\nWrite  OK");		
			if(data_verify()==false){goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Data File\nVerify OK");		
						
			DataMake();
			
			Sys1Popup(true, GUI_DARKBLUE, "Data File\nUpdate finsh");
			Beep(250);
		}
		else
		{
			errflow:			
			data_erase();
			
			Sys1Popup(true, GUI_DARKRED, "File Error\n(Press ESC KEY)");
			BUZZER(true); 
			EscWait();
			BUZZER(false);			
		}		
	}
	UpdateDatafile();
	DataUpdateGui();
	
	Delay_ms(100);
	Sys1Popup(false, GUI_DARKRED, "");
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
