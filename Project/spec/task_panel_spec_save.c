#include "task_panel_spec_save.h"

#define PanelSize (2 * 1800)

void panel_view(int index)
{
	dbgmsg("panel%02d=> tx: %02d - rx:%02d\n", index+1, panel_tx_num[index], panel_rx_num[index]  );
	dbgmsg(" - min \n");
	
	uint16_t min_node = 0;
	for(int tx = 0; tx < panel_tx_num[index]; tx++)
	{
		for(int rx = 0; rx < panel_rx_num[index]; rx++)	
		{
			dbgmsg("%04d ", panel_min[index][min_node]  );
			min_node++;
		}
		dbgmsg("\n");
	}
	
	dbgmsg(" - max \n");
	
	uint16_t max_node = 0;
	for(int tx = 0; tx < panel_tx_num[index]; tx++)
	{
		for(int rx = 0; rx < panel_rx_num[index]; rx++)	
		{	
			dbgmsg("%04d ", panel_max[index][max_node]  );	
			max_node++;	
		}	
		dbgmsg("\n");
	}
}

void UpdatePanelSpecSdram(void)
{	
	panel_count = 0;
	for(int cnt=0; cnt< 50; cnt++)
	{
		for(int tx = 0; tx < 1800; tx++)
		{
			panel_data[cnt][tx] = 0;
		}
	}	
	if(TSP_SAVE_FLAG == TSP_CHECK_FLAG)
	{
		panel_count = (*( (uint16_t *)TSP_TT_NUM_ADD));
		if(panel_count > 50) panel_count = 0;				

		panel_crc = set_crc32_init_value(CRC_INITIALVALUE);
		panel_crc = crc32_table[((panel_crc >> 24) ^ ( (panel_count>>8) & 0x00ff) ) ]  ^ (panel_crc << 8);
		panel_crc = crc32_table[((panel_crc >> 24) ^ ( (panel_count>>0) & 0x00ff) ) ]  ^ (panel_crc << 8);
		
		for(int cnt=0; cnt< panel_count; cnt++)
		{
			panel_tx_num[cnt] = (*( (uint16_t *)( TSP_TX_NUM_ADD + cnt * 2 ) ));
			panel_rx_num[cnt] = (*( (uint16_t *)( TSP_RX_NUM_ADD + cnt * 2 ) ));
			
			panel_crc = crc32_table[((panel_crc >> 24) ^ ( (panel_tx_num[cnt]>>8) & 0x00ff) ) ]  ^ (panel_crc << 8);
			panel_crc = crc32_table[((panel_crc >> 24) ^ ( (panel_tx_num[cnt]>>0) & 0x00ff) ) ]  ^ (panel_crc << 8);
				
			panel_crc = crc32_table[((panel_crc >> 24) ^ ( (panel_rx_num[cnt]>>8) & 0x00ff) ) ]  ^ (panel_crc << 8);
			panel_crc = crc32_table[((panel_crc >> 24) ^ ( (panel_rx_num[cnt]>>0) & 0x00ff) ) ]  ^ (panel_crc << 8);
			
			uint32_t min_value_add = TSP01_MIN_ADD+((cnt) * PanelSize);
			uint32_t max_value_add = TSP01_MAX_ADD+((cnt) * PanelSize);
				
			uint16_t node = 0;
			for(int tx = 0; tx < panel_tx_num[cnt]; tx++)
			{
				for(int rx = 0; rx < panel_rx_num[cnt]; rx++)
				{
					int16_t min_value_temp = (*( (int16_t *)( min_value_add    + node * 2 ) ));
					int16_t max_value_temp = (*( (int16_t *)( max_value_add    + node * 2 ) ));					
					panel_min[cnt][node] = min_value_temp;
					panel_max[cnt][node] = max_value_temp;
					
					panel_crc = crc32_table[((panel_crc >> 24) ^ ( (panel_min[cnt][node]>>8) & 0x00ff) ) ]  ^ (panel_crc << 8);
					panel_crc = crc32_table[((panel_crc >> 24) ^ ( (panel_min[cnt][node]>>0) & 0x00ff) ) ]  ^ (panel_crc << 8);
					
					panel_crc = crc32_table[((panel_crc >> 24) ^ ( (panel_max[cnt][node]>>8) & 0x00ff) ) ]  ^ (panel_crc << 8);
					panel_crc = crc32_table[((panel_crc >> 24) ^ ( (panel_max[cnt][node]>>0) & 0x00ff) ) ]  ^ (panel_crc << 8);
					
					node++;
				}
			}
		}
		
		dbgmsg("panel_crc %08lX\n",panel_crc);
	}
	else
	{	
		panel_crc = 0;
		panel_count = 0;
		for(int cnt=0; cnt< 50; cnt++)
		{
			for(int tx = 0; tx < 1800; tx++)
			{
				panel_data[cnt][tx] = 0;
			}
		}			
		//dbgmsg(" - panel_count    : %4d\n", panel_count);
	}
}
void PanelUpdateGui(void)
{
	//ProjectDisplay();
	//ModeNameUpdate();	
	PanelDisp();
}
void PanelDisp(void)
{
	char text[30];  
	hItem = WM_GetDialogItem(hSyswin1,MENU1_TSP_ST_TEXT);
	TEXT_SetTextColor(hItem, GUI_WHITE);	
	TEXT_SetBkColor(hItem, GUI_DARKBLUE);	
	sprintf(text, " - TSP Panel Spec(%d)",panel_count);	
	TEXT_SetText(hItem,text);
	GUI_Exec();
}

const char *info_panel_path = "0:/Spec/02.PANEL_SPEC/01.panel.csv";
uint8_t check_panel_file(void)
{	
	uint8_t er_flag = 0;
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_panel_path, FA_READ) != FR_OK)	{	er_flag++;	}	f_close(&file);
				
		f_mount(0, NULL);
	
		if(er_flag == 0)	return true;
		else				return false;
	}
	else
	{
		return false;
	}
}
uint8_t PanelErase(void)
{
	FLASH_Unlock();
	dbgmsg("panel_erase : FLASH_Sector_21\n");
	FLASH_EraseSector( FLASH_Sector_21,  VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector21\nErase OK");
	dbgmsg("panel_erase : FLASH_Sector_22\n");
	FLASH_EraseSector( FLASH_Sector_22, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector22\nErase OK");
	dbgmsg("panel_erase : FLASH_Sector_23\n");
	FLASH_EraseSector( FLASH_Sector_23, VoltageRange_3); Sys1Popup(true, GUI_DARKBLUE, "Sector23\nErase OK");
	FLASH_Lock();	
	return true;
}
uint8_t PanelSave(void)
{	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_panel_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			dbgmsg("Panel Save  : %s\n",info_panel_path);
				
			char Buff[1024]={0};
			int spec_cnt=0;
			int cnt=0;
			int tx_temp, rx_temp;
			int tx_cnt, node_cnt;
			
			while(f_gets(Buff, 1024, &file))
			{
				if(spec_cnt == 0) 	//label read
				{				
					cnt++;				
					spec_cnt++;				
					if(cnt > 50) break;
				}
				else if(spec_cnt == 1) 	//TX RX Number read
				{
					strtok(Buff,","); 	
					char* TX_NUM = strtok(NULL,","); tx_temp = atoi(TX_NUM);	
					
					strtok(NULL,","); 	
					char* RX_NUM = strtok(NULL,","); rx_temp = atoi(RX_NUM);
					
					uint32_t tx_number_add = TSP_TX_NUM_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( tx_number_add, tx_temp);

					uint32_t rx_number_add = TSP_RX_NUM_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( rx_number_add, rx_temp);

					spec_cnt++;
					//dbgmsg("TX RX Number read(%d,%d) \n", tx_temp, rx_temp);
				}
				else if(spec_cnt == 2)	//min value label
				{
					node_cnt = tx_cnt = 0;
					spec_cnt++;
					//dbgmsg("min value label\n");
				}
				else if(spec_cnt == 3)	// min value
				{			
					uint32_t min_value_add = TSP01_MIN_ADD+((cnt-1) * PanelSize);
					//dbgmsg("%s\n",Buff);
					
					tx_cnt++;				
					strtok(Buff,",");
					for(int cnt = 0; cnt < rx_temp; cnt++)				// rx data read
					{
						int16_t min_temp = atoi(strtok(NULL,","));
						uint32_t save_add = min_value_add + (node_cnt) * 2;
						FLASH_ProgramHalfWord( save_add, min_temp );

						node_cnt++;
					}
					if(tx_cnt == tx_temp)	spec_cnt++;
				}
				else if(spec_cnt == 4)	//max value label
				{
					node_cnt = tx_cnt = 0;
					spec_cnt++;
					//dbgmsg("max value label\n");
				}
				else if(spec_cnt == 5)	//max value
				{
					//dbgmsg("%s\n",Buff);
					uint32_t max_value_add = TSP01_MAX_ADD+((cnt-1) * PanelSize);
					tx_cnt++;
					strtok(Buff,",");
					for(int cnt = 0; cnt < rx_temp; cnt++)				// rx data read
					{
						int16_t max_temp = atoi(strtok(NULL,","));
						uint32_t save_add = max_value_add + (node_cnt) * 2;
						FLASH_ProgramHalfWord( save_add, max_temp );					
						node_cnt++;
					}
					
					if(tx_cnt == tx_temp)	spec_cnt = 0;	
				}
			}		
			if(cnt <= 50)	FLASH_ProgramHalfWord(TSP_TT_NUM_ADD, cnt);
			else			FLASH_ProgramHalfWord(TSP_TT_NUM_ADD, 50);
			
			FLASH_Lock();
		}
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&file);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}

uint8_t PanelMake(void)
{	
	FLASH_Unlock();
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);		
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);			
	
	FLASH_ProgramWord(TSP_SAVE_YEAR_ADD,  RTC_DateStr.RTC_Year   & 0xff );  // TSP file down year 
	FLASH_ProgramWord(TSP_SAVE_MONTH_ADD, RTC_DateStr.RTC_Month  & 0xff );	// TSP file down month
	FLASH_ProgramWord(TSP_SAVE_DAY_ADD,   RTC_DateStr.RTC_Date   & 0xff );  // TSP file down date	 
	FLASH_ProgramWord(TSP_SAVE_HOUR_ADD,  RTC_TimeStr.RTC_Hours   & 0xff ); // TSP file down year 
	FLASH_ProgramWord(TSP_SAVE_MIN_ADD,   RTC_TimeStr.RTC_Minutes & 0xff );	// TSP file down month
	
	FLASH_ProgramWord(TSP_SAVE_FLAG_ADD, TSP_CHECK_FLAG );	// SPEC file down date
	
	dbgmsg("20%02d-%02d-%02d, %02d:%02d[%X] Panel Spec Save OK!\n",TSP_SAVE_YEAR, TSP_SAVE_MONTH, TSP_SAVE_DAY, TSP_SAVE_HOUR, TSP_SAVE_MIN, TSP_SAVE_FLAG);
	
	FLASH_Lock();
	
	return true;
}
void PanelSaveFlow(void)
{
	if(usb_host_connected_flag == 0)
	{
		Sys1Popup(true, GUI_DARKRED,"USB Unmount\n(Press ESC KEY)"); 
		Beep(250);
	}
	else
	{
		Sys1Popup(true, GUI_DARKBLUE, "USB Mount");
		
		if(check_panel_file() == true)
		{			
			if(PanelErase()==false) {goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Panel Area\nErase  OK");		
			if(PanelSave()==false)	{goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Panel Data\nWrite  OK");		
						
			PanelMake();
			
			Sys1Popup(true, GUI_DARKBLUE, "Panle Data\nUpdate finsh");
			Beep(250);
		}
		else
		{
			errflow:			
			PanelErase();
			
			Sys1Popup(true, GUI_DARKRED, "File Error\n(Press ESC KEY)");
			BUZZER(true); 
			EscWait();
			BUZZER(false);			
		}		
	}
	UpdatePanelSpecSdram();
	PanelUpdateGui();
	
	Delay_ms(100);
	Sys1Popup(false, GUI_DARKRED, "");
	
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
