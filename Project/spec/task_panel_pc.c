#include "task_panel_pc.h"

static char buff[10]={0};

uint8_t PanelErase_Pc(void)
{
	FLASH_Unlock();
	dbgmsg("panel_erase : FLASH_Sector_21\n");
	FLASH_EraseSector( FLASH_Sector_21,  VoltageRange_3);

	buff[0] = 1;buff[1] = 3;	tx_pc_data(PANEL_DATA_ERASE, 2, buff);
	ntDelay_ms(100);

	dbgmsg("panel_erase : FLASH_Sector_22\n");
	FLASH_EraseSector( FLASH_Sector_22, VoltageRange_3);
	
	buff[0] = 2;buff[1] = 3;	tx_pc_data(PANEL_DATA_ERASE, 2, buff);
	ntDelay_ms(100);
	
	dbgmsg("panel_erase : FLASH_Sector_23\n");
	FLASH_EraseSector( FLASH_Sector_23, VoltageRange_3);
	FLASH_Lock();	

	buff[0] = 3;buff[1] = 3;	tx_pc_data(PANEL_DATA_ERASE, 2, buff);	
	ntDelay_ms(100);
	
	return true;
}

#define PanelSize (2 * 1800)

uint8_t Panel_Save_PC(char *data)
{	
	FLASH_Unlock();	
		
	char* ptr_cnt = strtok(data,",");
	if(atoi(ptr_cnt) <= 50)	FLASH_ProgramHalfWord(TSP_TT_NUM_ADD, atoi(ptr_cnt));
	else					FLASH_ProgramHalfWord(TSP_TT_NUM_ADD, 50);
	for( int cnt = 0; cnt <  atoi(ptr_cnt); cnt++ )
	{
		int node_cnt = 0;
		char* ptr_tx  = strtok(NULL,","); 
		int tx_temp = atoi(ptr_tx);

		char* ptr_rx  = strtok(NULL,","); 
		int rx_temp = atoi(ptr_rx);

		uint32_t tx_number_add = TSP_TX_NUM_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( tx_number_add, tx_temp);		

		uint32_t rx_number_add = TSP_RX_NUM_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( rx_number_add, rx_temp);
			
		panel_tx_num[cnt] = (*( (uint16_t *)( TSP_TX_NUM_ADD + cnt * 2 ) ));
		panel_rx_num[cnt] = (*( (uint16_t *)( TSP_RX_NUM_ADD + cnt * 2 ) ));
		
		uint32_t min_value_add = TSP01_MIN_ADD+((cnt) * PanelSize);
		node_cnt = 0;
		for( int tx = 0; tx <  tx_temp; tx++ )
		{
			for( int rx = 0; rx <  rx_temp; rx++ )
			{
				int16_t min_temp = atoi(strtok(NULL,","));
				uint32_t save_add = min_value_add + (node_cnt) * 2;
				FLASH_ProgramHalfWord( save_add, min_temp );
				node_cnt++;
			}
		}
		
		uint32_t max_value_add = TSP01_MAX_ADD+((cnt) * PanelSize);
		node_cnt = 0;
		for( int tx = 0; tx <  tx_temp; tx++ )
		{
			for( int rx = 0; rx <  rx_temp; rx++ )
			{
				int16_t max_temp = atoi(strtok(NULL,","));
				uint32_t save_add = max_value_add + (node_cnt) * 2;
				FLASH_ProgramHalfWord( save_add, max_temp );
				node_cnt++;
			}
		}		
		buff[0] = cnt; buff[1] = atoi(ptr_cnt);	tx_pc_data(PANEL_DATA_WR, 2, buff);	
		ntDelay_ms(100);
	}
	
	panel_count = (*( (uint16_t *)TSP_TT_NUM_ADD));
	if(panel_count > 50) panel_count = 0;
	
	for( int cnt = 0; cnt <  panel_count; cnt++ )
	{			
		panel_tx_num[cnt] = (*( (uint16_t *)( TSP_TX_NUM_ADD + cnt * 2 ) ));
		panel_rx_num[cnt] = (*( (uint16_t *)( TSP_RX_NUM_ADD + cnt * 2 ) ));
		
		uint32_t min_value_add = TSP01_MIN_ADD+((cnt) * PanelSize);
		int node_cnt = 0;
		for( int tx = 0; tx <  panel_tx_num[cnt]; tx++ )
		{
			for( int rx = 0; rx <  panel_rx_num[cnt]; rx++ )
			{
				int16_t min_value_temp = (*( (int16_t *)( min_value_add    + node_cnt * 2 ) ));
				panel_min[cnt][node_cnt] = min_value_temp;				
				node_cnt++;
			}
		}
		
		uint32_t max_value_add = TSP01_MAX_ADD+((cnt) * PanelSize);
		node_cnt = 0;
		for( int tx = 0; tx <  panel_tx_num[cnt]; tx++ )
		{
			for( int rx = 0; rx <  panel_rx_num[cnt]; rx++ )
			{				
				int16_t max_value_temp = (*( (int16_t *)( max_value_add    + node_cnt * 2 ) ));
				panel_max[cnt][node_cnt] = max_value_temp;				
				node_cnt++;
			}
		}
	}	
	
	FLASH_Lock();	
	
	buff[0] = panel_count; buff[1] = panel_count;	tx_pc_data(PANEL_DATA_WR, 2, buff);	
}

uint8_t PanelMake_Pc(char *data)
{
	FLASH_Unlock();	
	
	char* ptr_year = strtok(data,",");
	char* ptr_mon = strtok(NULL,",");
	char* ptr_day = strtok(NULL,",");
	char* ptr_hour = strtok(NULL,",");
	char* ptr_min = strtok(NULL,",");
	

	FLASH_ProgramWord(TSP_SAVE_YEAR_ADD, 	atoi(ptr_year) & 0xff );	// SPEC file down year 
	FLASH_ProgramWord(TSP_SAVE_MONTH_ADD, 	atoi(ptr_mon)  & 0xff );	// SPEC file down month
	FLASH_ProgramWord(TSP_SAVE_DAY_ADD, 	atoi(ptr_day)  & 0xff );	// SPEC file down date
	FLASH_ProgramWord(TSP_SAVE_HOUR_ADD, 	atoi(ptr_hour) & 0xff); 	// SPEC file down year 
	FLASH_ProgramWord(TSP_SAVE_MIN_ADD, 	atoi(ptr_min)  & 0xff);		// SPEC file down month
	FLASH_ProgramWord(TSP_SAVE_FLAG_ADD, TSP_CHECK_FLAG );	// SPEC file down date
	
	dbgmsg("20%02d-%02d-%02d, %02d:%02d[%X] Panel Spec Save OK!\n",TSP_SAVE_YEAR, TSP_SAVE_MONTH, TSP_SAVE_DAY, TSP_SAVE_HOUR, TSP_SAVE_MIN, TSP_SAVE_FLAG);
	
	FLASH_Lock();	
		
	UpdatePanelSpecSdram();
	PanelUpdateGui();
		
	buff[0] = true;	tx_pc_data(PANEL_DATA_FINSH, 1, buff);	
	
	dbgmsg("PANEL_DATA_FINSH\n");
}

uint8_t Panel_Send_PC(void)
{	
	buff[0] = ( panel_crc >>24) & 0xff;		
	buff[1] = ( panel_crc >>16) & 0xff;		
	buff[2] = ( panel_crc >> 8) & 0xff;		
	buff[3] = ( panel_crc >> 0) & 0xff;		
	
	tx_pc_data(PANEL_DATA_RD, 4, buff);
	
	return true;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
