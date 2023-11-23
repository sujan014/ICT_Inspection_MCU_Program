#include "task_eep.h"

void Updateeepfile(void)
{
		if( EEP_SAVE_FLAG == EEP_CHECK_FLAG )
		{
			In_eep.size = (*( (uint32_t *)EEP_SIZE_ADD));
			In_eep.name = (((char*)EEP_NAME_ADD));
			In_eep.crc32 = (*( (uint32_t *)EEP_CRC32_ADD));
		}
		else
		{
			In_eep.size  = 0;
			In_eep.name = "No EEP File";
			In_eep.crc32 = 0;
		}
}

void eepUpdateGui(void)
{
	eepDisp();
}

void eepDisp(void)
{
	char text[1024];    
	hItem = WM_GetDialogItem(hSyswin1,MENU1_EEP_ST_TEXT);
	TEXT_SetTextColor(hItem, GUI_WHITE);	
	TEXT_SetBkColor(hItem, GUI_DARKBLUE);	
	sprintf(text, " - %s - size : %.2f kB",In_eep.name,(float)((float)In_eep.size/1024) );
	TEXT_SetText(hItem,text);
	GUI_Exec();
}

FATFS eepfatfs;
FIL eepfile;
DIR eepdir;
char eep_file_path[255+21] = {NULL};
FILINFO eepfileinfo;
uint16_t eep_number = 0;
int  eep_data_size = 0;
char eep_name[180] = {NULL};

uint8_t check_eep_file(void)
{
	char *fn;	
	if ( f_mount( 0, &eepfatfs ) == FR_OK )
	{
		static char lfn[_MAX_LFN + 1];
		eepfileinfo.lfname = lfn; 
		eepfileinfo.lfsize = sizeof lfn; 
		
		if(f_opendir(&eepdir, "0:/Spec/05.EEP_FILE") == FR_OK)
		{
			eep_number = 0;
			while(1)
			{
				if(f_readdir(&eepdir, &eepfileinfo) == FR_OK)
				{
					if (eepfileinfo.fname[0] == 0)		break; 
					if (eepfileinfo.fname[0] == '.')	continue; 	
					fn = *eepfileinfo.lfname ? eepfileinfo.lfname : eepfileinfo.fname;
					eep_data_size = eepfileinfo.fsize;
					dbgmsg("EEPROM file name:[%s], size:[%d]\r\n", fn, eep_data_size);
					sprintf(eep_name, "%s",fn);
					sprintf(eep_file_path, "0:/Spec/05.EEP_FILE/%s",fn);
					eep_number++;
				}
			}		
		}
		f_mount(0, NULL);
		if(eep_number == 1)	return true;
		else					return false;		
	}
	else
	{
		return false;
	}	
}

uint8_t eep_erase(void)
{
	FLASH_Unlock();
	dbgmsg("data_erase : FLASH_Sector_10\n");
	FLASH_EraseSector( FLASH_Sector_10, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector10\nErase OK");	
	FLASH_Lock();	
	return true;
}

uint8_t eep_write(void)
{
	if ( f_mount( 0, &eepfatfs ) == FR_OK )
	{
		if(f_open(&eepfile, eep_file_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			int cnt1=0;
			//char ReadTextBuff[100]={0};
			char ReadTextBuff[256]={0};
			//------------
			char* ptr;
			char value;
			//------------
			char text[50];  
		
			dbgmsg("Data Write  : %s\n",eep_file_path);
			//--------------------------------------------
			while( f_gets(ReadTextBuff, 256, &eepfile) )
			{
				ptr   = strtok(ReadTextBuff,",");
				while(ptr)
				{
					if(ptr[0] == '\n')	break;
					value = strtol(ptr, NULL, 16);
					FLASH_ProgramByte(EEPDATA_ADDRESS + cnt1 ,value);
					cnt1++;
					ptr   = strtok(NULL,",");

					dbgmsg("%02X\n",value);
				
					if(cnt1 % 1024 == 0)
					{
						//sprintf(text,"POCB Download\n%d kB/ %ld kB\n", cnt1/1024, odfile.fsize/1024);
						sprintf(text,"EEPROM Download\n%d kB", cnt1/1024);
						Sys1Popup(true, GUI_DARKBLUE, text);
					}
				
				}
			}
			eep_data_size = cnt1;

			FLASH_Lock();
		}
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&eepfile);	
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}

uint8_t eep_verify(void)
{
	if ( f_mount( 0, &eepfatfs ) == FR_OK )
	{
		if(f_open(&eepfile, eep_file_path, FA_READ) == FR_OK)
		{
			int cnt1, err_verify;
			//char ReadTextBuff[100]={0};
			//------------
			char ReadTextBuff[256]={0};
			char* ptr;
			char value;
			//------------
			//char text[50];  
		
			dbgmsg("EEPROM_Data Verify : %s\n",eep_file_path);
			err_verify = cnt1 = 0;
			
			while( f_gets(ReadTextBuff, 256, &eepfile) )
			{
				ptr   = strtok(ReadTextBuff,",");
				while(ptr)
				{
					if(ptr[0] == '\n')	break;
					value = strtol(ptr, NULL, 16);
					
					if(INPUT_EEP[cnt1] !=value)
					{
							err_verify = 1;
							break;
					}
					cnt1++;
					ptr   = strtok(NULL,",");

					//dbgmsg("%02X, ",value);
					//if((cnt1+1) %16==0)dbgmsg("\n");	
				}
			}
			if(err_verify !=0)
			{
				return false;
				
			}	
		}
		else 
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&eepfile);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;		
}

uint8_t eepMake(void)
{
	FLASH_Unlock();	
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);		
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);	
	
	FLASH_ProgramWord(EEP_SIZE_ADD, eep_data_size );	// SPEC file down date 
	for(int i = 0; i < 180; i++)	
	{
		FLASH_ProgramByte(EEP_NAME_ADD+i,eep_name[i]);
	}
	FLASH_ProgramWord(EEP_SAVE_YEAR_ADD,  RTC_DateStr.RTC_Year  & 0xff );  // SPEC file down year 
	FLASH_ProgramWord(EEP_SAVE_MONTH_ADD, RTC_DateStr.RTC_Month & 0xff );	// SPEC file down month
	FLASH_ProgramWord(EEP_SAVE_DAY_ADD,   RTC_DateStr.RTC_Date  & 0xff );	// SPEC file down date
		
	FLASH_ProgramWord(EEP_SAVE_HOUR_ADD, RTC_TimeStr.RTC_Hours  & 0xff );  // SPEC file down year 
	FLASH_ProgramWord(EEP_SAVE_MIN_ADD, RTC_TimeStr.RTC_Minutes & 0xff );	// SPEC file down month
	
	FLASH_ProgramWord(EEP_SAVE_FLAG_ADD, EEP_CHECK_FLAG );	// SPEC file down date
	
	dbgmsg("20%02d-%02d-%02d, %02d:%02d[%X] OD Data Save OK!\n",EEP_SAVE_YEAR,EEP_SAVE_MONTH,EEP_SAVE_DAY,EEP_SAVE_HOUR,EEP_SAVE_MIN,EEP_SAVE_FLAG);
	FLASH_Lock();
	
	return true;
}

void eepSaveflow(void)
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
		
		if(check_eep_file() == true)
		{
			if(eep_erase()==false) {goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "OD Data Area\nErase  OK");		
			if(eep_write()==false)	{goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "OD Data File\nWrite  OK");		
			if(eep_verify()==false){goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "OD Data File\nVerify OK");	
		
			eepMake();
			
			Sys1Popup(true, GUI_DARKBLUE, "EEP Data File\nUpdate finsh");
			Beep(250);	
		}
		else
		{
			errflow:	
			eep_erase();
			
			Sys1Popup(true, GUI_DARKRED, "File Error\n(Press ESC KEY)");
			BUZZER(true); 
			EscWait();
			BUZZER(false);		
		}	
	}
	Updateeepfile();
	eepUpdateGui();
	Delay_ms(100);
	Sys1Popup(false, GUI_DARKRED, "");
}





