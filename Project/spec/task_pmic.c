#include "task_pmic.h"

//--------------------TPIC¶û °°ÀÌ »ç¿ë 210911
void Updatepmicfile(void)
{
	if(PMIC_SAVE_FLAG == PMIC_CHECK_FLAG)
	{	
		In_pmic.size = (*( (uint32_t *)PMIC_SIZE_ADD));
		In_pmic.name = (((char*)PMIC_NAME_ADD));
		In_pmic.crc32 = (*( (uint32_t *)PMIC_CRC32_ADD));
		
		dbgmsg("PMIC data\n");
		for(int cnt = 0; cnt < In_pmic.size; cnt++)
		{
			dbgmsg("[%d]=0x%02X\n", cnt, INPUT_PMIC[cnt]);				
		}
	}
	else 
	{
		In_pmic.size = 0;
		In_pmic.name = "No PMIC File";
		In_pmic.crc32 = 0;	
	}
}

void pmicUpdateGui(void)
{
pmicDisp();
}
void pmicDisp(void)
{
	char text[1024];    
	hItem = WM_GetDialogItem(hSyswin1,MENU1_PMIC_ST_TEXT);
	TEXT_SetTextColor(hItem, GUI_WHITE);	
	TEXT_SetBkColor(hItem, GUI_DARKBLUE);
	//sprintf(text, " - %s\n - size : %.2f kB", In_pmic.name, (float)((float)In_pmic.size/1024));
	sprintf(text, "-%s\n-%s", In_pmic.name,In_tpic.name);
	TEXT_SetText(hItem,text);
	GUI_Exec();
}

FATFS pmicfatfs;
FIL pmicfile;
DIR pmicdir;
char pmic_file_path[255+21] = {NULL};
FILINFO pmicfileinfo;
uint16_t pmic_number = 0;
int  pmic_data_size = 0;
char pmic_name[180] = {NULL};

uint8_t check_pmic_file(void)
{
	char *fn;	
	if ( f_mount( 0, &pmicfatfs ) == FR_OK )
	{
		static char lfn[_MAX_LFN + 1];
		pmicfileinfo.lfname = lfn; 
		pmicfileinfo.lfsize = sizeof lfn; 
		
		if(f_opendir(&pmicdir, "0:/Spec/04.PMIC_FILE") == FR_OK)
		{
			pmic_number = 0;
			while(1)
			{
				if(f_readdir(&pmicdir, &pmicfileinfo) == FR_OK)
				{
					if (pmicfileinfo.fname[0] == 0)		break; 
					if (pmicfileinfo.fname[0] == '.')	continue; 	
					fn = *pmicfileinfo.lfname ? pmicfileinfo.lfname : pmicfileinfo.fname;
					pmic_data_size = pmicfileinfo.fsize;
					dbgmsg("PMIC file name:[%s], size:[%d]\r\n", fn, pmic_data_size);
					sprintf(pmic_name, "%s",fn);
					sprintf(pmic_file_path, "0:/Spec/04.PMIC_FILE/%s",fn);
					pmic_number++;
				}
			}		
		}
		f_mount(0, NULL);
		if(pmic_number == 1)	return true;
		else					return false;		
	}
	else
	{
		return false;
	}	

}

uint8_t pmic_erase(void)
{
	FLASH_Unlock();
	dbgmsg("data_erase : FLASH_Sector_9\n");
	FLASH_EraseSector( FLASH_Sector_9, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector9\nErase OK");	
	FLASH_Lock();	
	return true;
}

uint8_t pmic_write(void)
{
	if ( f_mount( 0, &pmicfatfs ) == FR_OK )
	{
		if(f_open(&pmicfile, pmic_file_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			int cnt1=0;
			//char ReadTextBuff[100]={0};
			//------------
			char* ptr;
			char value;
			//------------
			char ReadTextBuff[256]={0};
			char text[50]; 
			
			dbgmsg("PMIC Write  : %s\n",pmic_file_path);	
			//--------------------------------------------
			while( f_gets(ReadTextBuff, 256, &pmicfile) )
			{
				ptr   = strtok(ReadTextBuff,",");
				while(ptr)
				{
					if(ptr[0] == '\n')	break;
					value = strtol(ptr, NULL, 16);
					FLASH_ProgramByte(PMICDATA_ADDRESS + cnt1 ,value);
					cnt1++;
					ptr   = strtok(NULL,",");
					/*dbgmsg("%02X, ",value);
					if((cnt1+1) %16==0)dbgmsg("\n");*/
				
					if(cnt1 % 1024 == 0)
					{
						//sprintf(text,"POCB Download\n%d kB/ %ld kB\n", cnt1/1024, pocbfile.fsize/1024);
						sprintf(text,"PMIC Download\n%d kB", cnt1/1024);
						Sys1Popup(true, GUI_DARKBLUE, text);
					}
				}
			}
			pmic_data_size = cnt1;
			//--------------------------------------------
			/*while( f_gets_FW(ReadTextBuff, 17, &pocbfile) )
			{
				for(cnt = 0; cnt <16;cnt++) 
				{
					if(cnt1 < pocbfile.fsize)
					{
						dbgmsg("%02X, ",ReadTextBuff[cnt]);
						if((cnt1+1) %16==0)dbgmsg("\n");
						FLASH_ProgramByte(POCBDATA_ADDRESS + cnt1 ,ReadTextBuff[cnt]);
						cnt1++;
					}
					if(cnt1 % 1024 == 0)
					{
						sprintf(text,"POCB Download\n%d kB/ %ld kB\n", cnt1/1024, pocbfile.fsize/1024);
						Sys1Popup(true, GUI_DARKBLUE, text);
					}
				}
			}*/
			FLASH_Lock();
		}
		else 
		{	
			f_mount(0, NULL);
			return false;
		}
		f_close(&pmicfile);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;	
}

uint8_t pmic_verify(void)
{
	if ( f_mount( 0, &pmicfatfs ) == FR_OK )
	{
		if(f_open(&pmicfile, pmic_file_path, FA_READ) == FR_OK)
		{
			int cnt1, err_verify;
			//char ReadTextBuff[100]={0};
			//------------
			char ReadTextBuff[256]={0};
			char* ptr;
			char value;
			//------------
			//char text[50]; 

			dbgmsg("PMIC Data Verify : %s\n",pmic_file_path);
			
			err_verify = cnt1 = 0;
			//-------------------------------------------
			while( f_gets(ReadTextBuff, 256, &pmicfile) )
			{
				ptr   = strtok(ReadTextBuff,",");
				while(ptr)
				{
					if(ptr[0] == '\n')	break;
					value = strtol(ptr, NULL, 16);
					
					if(INPUT_PMIC[cnt1] !=value)
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
		f_close(&pmicfile);
	}
	else 
	{
		return false;
	}
	f_mount(0, NULL);
	return true;
}

uint8_t pmicMake(void)
{
	FLASH_Unlock();	
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);		
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);	
	
	FLASH_ProgramWord(PMIC_SIZE_ADD, pmic_data_size );
	for(int i = 0; i < 180; i++)	
	{
		FLASH_ProgramByte(PMIC_NAME_ADD+i,pmic_name[i]);
	}
	FLASH_ProgramWord(PMIC_SAVE_YEAR_ADD,  RTC_DateStr.RTC_Year  & 0xff );  // SPEC file down year 
	FLASH_ProgramWord(PMIC_SAVE_MONTH_ADD, RTC_DateStr.RTC_Month & 0xff );	// SPEC file down month
	FLASH_ProgramWord(PMIC_SAVE_DAY_ADD,   RTC_DateStr.RTC_Date  & 0xff );	// SPEC file down date
	FLASH_ProgramWord(PMIC_SAVE_HOUR_ADD, RTC_TimeStr.RTC_Hours  & 0xff );  // SPEC file down year 
	FLASH_ProgramWord(PMIC_SAVE_MIN_ADD, RTC_TimeStr.RTC_Minutes & 0xff );	// SPEC file down month
	
	FLASH_ProgramWord(PMIC_SAVE_FLAG_ADD, PMIC_CHECK_FLAG );	// SPEC file down date
	
	dbgmsg("20%02d-%02d-%02d, %02d:%02d[%X] PMIC Data Save OK!\n",PMIC_SAVE_YEAR,PMIC_SAVE_MONTH,PMIC_SAVE_DAY,PMIC_SAVE_HOUR,PMIC_SAVE_MIN,PMIC_SAVE_FLAG);
	FLASH_Lock();
	
	return true;
}

//--------------------------------------------TPIC
void Updatetpicfile(void)
{
	if(TPIC_SAVE_FLAG == TPIC_CHECK_FLAG)
	{	
		In_tpic.size = (*( (uint32_t *)TPIC_SIZE_ADD));
		In_tpic.name = (((char*)TPIC_NAME_ADD));
		In_tpic.crc32 = (*( (uint32_t *)TPIC_CRC32_ADD));
	}
	else 
	{
		In_tpic.size = 0;
		In_tpic.name = "No TPIC File";
		In_tpic.crc32 = 0;	
	}
}

FATFS tpicfatfs;
FIL tpicfile;
DIR tpicdir;
char tpic_file_path[255+21] = {NULL};
FILINFO tpicfileinfo;
uint16_t tpic_number = 0;
int  tpic_data_size = 0;
char tpic_name[180] = {NULL};

uint8_t check_tpic_file(void)
{
	char *fn;	
	if ( f_mount( 0, &tpicfatfs ) == FR_OK )
	{
		static char lfn[_MAX_LFN + 1];
		tpicfileinfo.lfname = lfn; 
		tpicfileinfo.lfsize = sizeof lfn; 
		
		if(f_opendir(&tpicdir, "0:/Spec/06.TPIC_FILE") == FR_OK)
		{
			tpic_number = 0;
			while(1)
			{
				if(f_readdir(&tpicdir, &tpicfileinfo) == FR_OK)
				{
					if (tpicfileinfo.fname[0] == 0)		break; 
					if (tpicfileinfo.fname[0] == '.')	continue; 	
					fn = *tpicfileinfo.lfname ? tpicfileinfo.lfname : tpicfileinfo.fname;
					tpic_data_size = tpicfileinfo.fsize;
					dbgmsg("TPIC file name:[%s], size:[%d]\r\n", fn, tpic_data_size);
					sprintf(tpic_name, "%s",fn);
					sprintf(tpic_file_path, "0:/Spec/06.TPIC_FILE/%s",fn);
					tpic_number++;
				}
			}		
		}
		f_mount(0, NULL);
		if(tpic_number == 1)	return true;
		else					return false;		
	}
	else
	{
		return false;
	}	
}
//----------- 
uint8_t tpic_write(void)
{
	if ( f_mount( 0, &tpicfatfs ) == FR_OK )
	{
		if(f_open(&tpicfile, tpic_file_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			int cnt1=0;
			//------------
			char* ptr;
			char value;
			//------------
			char ReadTextBuff[256]={0};
			char text[50]; 
			
			dbgmsg("TPIC Write  : %s\n",tpic_file_path);	
			//--------------------------------------------
			while( f_gets(ReadTextBuff, 256, &tpicfile) )
			{
				ptr   = strtok(ReadTextBuff,",");
				while(ptr)
				{
					if(ptr[0] == '\n')	break;
					value = strtol(ptr, NULL, 16);
					FLASH_ProgramByte(TPICDATA_ADDRESS + cnt1 ,value);
					dbgmsg("TPIC Write : %02X\n",value);	
					cnt1++;
					ptr   = strtok(NULL,",");			
					if(cnt1 % 1024 == 0)
					{
						sprintf(text,"TPIC Download\n%d kB", cnt1/1024);
						Sys1Popup(true, GUI_DARKBLUE, text);
					}
				
				}
			}
			tpic_data_size = cnt1;
			FLASH_Lock();
		}
		else 
		{	
			f_mount(0, NULL);
			return false;
		}
		f_close(&tpicfile);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;	
}

uint8_t tpic_verify(void)
{
	if ( f_mount( 0, &tpicfatfs ) == FR_OK )
	{
		if(f_open(&tpicfile, tpic_file_path, FA_READ) == FR_OK)
		{
			int cnt1, err_verify;
			char ReadTextBuff[256]={0};
			char* ptr;
			char value;

			dbgmsg("TPIC Data Verify : %s\n",tpic_file_path);
			
			err_verify = cnt1 = 0;
			while( f_gets(ReadTextBuff, 256, &tpicfile) )
			{
				ptr   = strtok(ReadTextBuff,",");
				while(ptr)
				{
					if(ptr[0] == '\n')	break;
					value = strtol(ptr, NULL, 16);
					
					if(INPUT_TPIC[cnt1] !=value)
					{
							err_verify = 1;
							break;
					}
					cnt1++;
					ptr   = strtok(NULL,",");
					dbgmsg("INPUT_TPIC[%d] : Value : %02X\n",INPUT_TPIC[cnt1], value);	

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
		f_close(&tpicfile);
	}
	else 
	{
		return false;
	}
	f_mount(0, NULL);
	return true;
}

uint8_t tpic_erase(void)
{
	FLASH_Unlock();
	dbgmsg("data_erase : FLASH_Sector_11\n");
	FLASH_EraseSector( FLASH_Sector_11, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector11\nErase OK");	
	FLASH_Lock();	
	return true;
}


uint8_t tpicMake(void)
{
	FLASH_Unlock();	
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);		
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);	
	
	FLASH_ProgramWord(TPIC_SIZE_ADD, tpic_data_size );
	for(int i = 0; i < 180; i++)	
	{
		FLASH_ProgramByte(TPIC_NAME_ADD+i,tpic_name[i]);
	}
	FLASH_ProgramWord(TPIC_SAVE_YEAR_ADD,  RTC_DateStr.RTC_Year  & 0xff );  // SPEC file down year 
	FLASH_ProgramWord(TPIC_SAVE_MONTH_ADD, RTC_DateStr.RTC_Month & 0xff );	// SPEC file down month
	FLASH_ProgramWord(TPIC_SAVE_DAY_ADD,   RTC_DateStr.RTC_Date  & 0xff );	// SPEC file down date
	FLASH_ProgramWord(TPIC_SAVE_HOUR_ADD, RTC_TimeStr.RTC_Hours  & 0xff );  // SPEC file down year 
	FLASH_ProgramWord(TPIC_SAVE_MIN_ADD, RTC_TimeStr.RTC_Minutes & 0xff );	// SPEC file down month
	
	FLASH_ProgramWord(TPIC_SAVE_FLAG_ADD, TPIC_CHECK_FLAG );	// SPEC file down date
	
	dbgmsg("20%02d-%02d-%02d, %02d:%02d[%X] TPIC Data Save OK!\n",TPIC_SAVE_YEAR,TPIC_SAVE_MONTH,TPIC_SAVE_DAY,TPIC_SAVE_HOUR,TPIC_SAVE_MIN,TPIC_SAVE_FLAG);
	FLASH_Lock();
	
	return true;
}
//--------------------------------------------------

void pmicSaveflow(void) // TPIC¶û °°ÀÌ »ç¿ë
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
		if(check_pmic_file() == true && check_tpic_file() == true)
		{
			if(pmic_erase()==false) {goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "PMIC Area\nErase  OK");		
			if(pmic_write()==false)	{goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "PMIC File\nWrite  OK");		
			if(pmic_verify()==false){goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "PMIC File\nVerify OK");			
			if(tpic_erase()==false) {goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "TPIC Area\nErase  OK");	
			if(tpic_write()==false)	{goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "TPIC File\nWrite  OK");		
			if(tpic_verify()==false){goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "TPIC File\nVerify OK");	
		
			pmicMake();
			tpicMake();

			Sys1Popup(true, GUI_DARKBLUE, "PMIC File\nUpdate finsh");
			Beep(250);
		}
		else 
		{
			errflow:	
			pmic_erase();
			tpic_erase();
			Sys1Popup(true, GUI_DARKRED, "File Error\n(Press ESC KEY)");
			BUZZER(true); 
			EscWait();
			BUZZER(false);	
		}
	}
	Updatepmicfile();
	Updatetpicfile();
	pmicUpdateGui(); 

	Delay_ms(100);
	Sys1Popup(false, GUI_DARKRED, "");
}
