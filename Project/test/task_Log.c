#include "task_log.h"

uint8_t  Save_sec = 0,  Save_min = 0,   Save_hour = 0;
uint8_t  Save_year = 0, Save_month = 0, Save_day = 0;

char SUM_DIR[256]={0}, SUM_FIL[256]={0};
char FUN_DIR[256]={0}, FUN_FIL[256]={0};
char PN1_FIL[256]={0};
char PN2_FIL[256]={0};
char SDC_DIR[256]={0}, SDC_FIL[256]={0};
	
void DIR_FILE_UPDATE(uint8_t pf_flag)
{
	char modetext[30];
	
	if(RD_MODE == 0) 		sprintf(modetext,"%s",mode1_name);
	else if(RD_MODE == 1)	sprintf(modetext,"%s",mode2_name);
	else if(RD_MODE == 2)	sprintf(modetext,"%s",mode3_name);
	else if(RD_MODE == 3)	sprintf(modetext,"%s",mode4_name);
	
	//////////////////////////////////////
	////Summary
	//////////////////////////////////////
	sprintf(SUM_DIR, "0:/LOG(%s)",project_name_main); 																		f_mkdir(SUM_DIR); 
	sprintf(SUM_DIR, "0:/LOG(%s)/%s",project_name_main,modetext); 															f_mkdir(SUM_DIR); 
	sprintf(SUM_DIR, "0:/LOG(%s)/%s/20%02d-%02d-%02d",project_name_main,modetext,Save_year,Save_month,Save_day); 			f_mkdir(SUM_DIR); 
	sprintf(SUM_FIL, "0:/LOG(%s)/%s/20%02d-%02d-%02d/summary_%02d%02d%02d(%08lX).csv",project_name_main,modetext,Save_year,Save_month,Save_day,Save_year,Save_month,Save_day,serial_num);
	
	//////////////////////////////////////
	////Func
	//////////////////////////////////////
	sprintf(FUN_DIR, "0:/LOG(%s)",project_name_main); 																		f_mkdir(FUN_DIR); 
	sprintf(FUN_DIR, "0:/LOG(%s)/%s",project_name_main,modetext); 															f_mkdir(FUN_DIR); 
	sprintf(FUN_DIR, "0:/LOG(%s)/%s/20%02d-%02d-%02d",project_name_main,modetext,Save_year,Save_month,Save_day);  			f_mkdir(FUN_DIR); 
	sprintf(FUN_DIR, "0:/LOG(%s)/%s/20%02d-%02d-%02d/func",project_name_main,modetext,Save_year,Save_month,Save_day); 	 	f_mkdir(FUN_DIR); 
	sprintf(FUN_FIL, "0:/LOG(%s)/%s/20%02d-%02d-%02d/func/func_%02d%02d%02d(%08lX).csv",project_name_main,modetext,Save_year,Save_month,Save_day,Save_year,Save_month,Save_day,serial_num);
	//////////////////////////////////////
	////1D Panel
	//////////////////////////////////////
	sprintf(PN1_FIL, "0:/LOG(%s)/%s/20%02d-%02d-%02d/func/1D_Panel_%02d%02d%02d(%08lX).csv",project_name_main,modetext,Save_year,Save_month,Save_day,Save_year,Save_month,Save_day,serial_num);
	//////////////////////////////////////
	////2D Panel
	//////////////////////////////////////
	sprintf(PN2_FIL, "0:/LOG(%s)/%s/20%02d-%02d-%02d/func/2D_Panel_%02d%02d%02d(%08lX).csv",project_name_main,modetext,Save_year,Save_month,Save_day,Save_year,Save_month,Save_day,serial_num);

	//////////////////////////////////////
	////SDC
	//////////////////////////////////////
	sprintf(SDC_DIR, "0:/LOG(%s)",project_name_main); 																		f_mkdir(SDC_DIR); 
	sprintf(SDC_DIR, "0:/LOG(%s)/%s",project_name_main,modetext); 															f_mkdir(SDC_DIR); 
	sprintf(SDC_DIR, "0:/LOG(%s)/%s/20%02d-%02d-%02d",project_name_main,modetext,Save_year,Save_month,Save_day);  			f_mkdir(SDC_DIR); 
	sprintf(SDC_DIR, "0:/LOG(%s)/%s/20%02d-%02d-%02d/SDC_Log",project_name_main,modetext,Save_year,Save_month,Save_day);	f_mkdir(SDC_DIR); 
	if(pf_flag == true)
	{
		sprintf(SDC_FIL, "0:/LOG(%s)/%s/20%02d-%02d-%02d/SDC_Log/%s_TSP_LOG_SPL%d_P.csv",project_name_main,modetext,Save_year,Save_month,Save_day,project_name_main,QTY.TT);
	}
	else if(pf_flag == false)	
	{
		sprintf(SDC_FIL,"0:/LOG(%s)/%s/20%02d-%02d-%02d/SDC_Log/%s_TSP_LOG_SPL%d_F.csv",project_name_main,modetext,Save_year,Save_month,Save_day,project_name_main,QTY.TT);
	}
	
}

char logwritetBuff[1024];
void LogSaveUsb(uint8_t pf_flag)
{					
	if(usb_host_connected_flag == 0)	return;
					
	MainPopup(true, GUI_LIGHTBLUE, "LOG SAVE TO USB");	
	
	DWORD size = 0;  
	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		DIR_FILE_UPDATE(pf_flag);
		
		MainPopup(true, GUI_LIGHTBLUE, "LOG SAVE TO USB\nSummary");		
		dbgmsg(">> Summary save  : %s\n",SUM_FIL);
		if(f_open(&file, SUM_FIL, FA_OPEN_ALWAYS |FA_WRITE) == FR_OK)
		{					   
			size = (&file)->fsize;
			f_lseek(&file,size); 
			
			if(size == 0)
			{				
				//// label
				sprintf(logwritetBuff,",,,,,,,,,,,,,,upper,");			f_puts(logwritetBuff,&file);

				// add part test index
				for(int cnt = 0; cnt < part_count; cnt++)
				{
					sprintf(logwritetBuff,"%.2f,",part_spec_hi[cnt]);	f_puts(logwritetBuff,&file); 
				}
				
				sprintf(logwritetBuff,"\n,,,,,,,,,,,,,,lower,");			f_puts(logwritetBuff,&file);
				// add part test index
				for(int cnt = 0; cnt < part_count; cnt++)
				{
					sprintf(logwritetBuff,"%.2f,",part_spec_lo[cnt]);		f_puts(logwritetBuff,&file); 
				}
				sprintf(logwritetBuff,"\nProject,");			f_puts(logwritetBuff,&file); 
				sprintf(logwritetBuff,"Serial,");				f_puts(logwritetBuff,&file); 
				sprintf(logwritetBuff,"Version,");				f_puts(logwritetBuff,&file);
				sprintf(logwritetBuff,"VCC1,");				f_puts(logwritetBuff,&file);
				sprintf(logwritetBuff,"VCC2,");				f_puts(logwritetBuff,&file);
				sprintf(logwritetBuff,"NG Cont.,");				f_puts(logwritetBuff,&file);
				sprintf(logwritetBuff,"Date,Time,Result,");		f_puts(logwritetBuff,&file); 
				sprintf(logwritetBuff,"QTY,PASS,FAIL,");		f_puts(logwritetBuff,&file); 
				sprintf(logwritetBuff,"Tack Time,");			f_puts(logwritetBuff,&file); 
				sprintf(logwritetBuff,"Open,Short,");			f_puts(logwritetBuff,&file); 
				
				//EVCC_Check();
				// add part test index
				for(int cnt = 0; cnt < part_count; cnt++)
				{
					sprintf(logwritetBuff,"%s,",part_name[cnt]);		f_puts(logwritetBuff,&file); 
				}

				// add func test index
				for(int cnt = 0; cnt < func_count; cnt++)
				{
					sprintf(logwritetBuff,"%s,",func_name[cnt]);
					if(func_log_flag[cnt] == true)	f_puts(logwritetBuff,&file);
				}
			}					
			//// data	
			//Time Write
			sprintf(logwritetBuff,"\n%s,",project_name_main);					f_puts(logwritetBuff,&file);
			
			sprintf(logwritetBuff,"0x%08lX,",serial_num);						f_puts(logwritetBuff,&file); 			
		
			if(RD_DBG_MD == 0)	{	sprintf(logwritetBuff,"KM-V%1.2f(D),",sys_ver);	}
			else				{	sprintf(logwritetBuff,"KM-V%1.2f(R),",sys_ver);	}
            
			f_puts(logwritetBuff,&file); 
						
			sprintf(logwritetBuff,"%.2lfV,",Evcc1_Vlot);	f_puts(logwritetBuff,&file); 	
			sprintf(logwritetBuff,"%.2lfV,",Evcc2_Vlot);	f_puts(logwritetBuff,&file); 	
			
			if(RD_NG_CONTINUE == true)	sprintf(logwritetBuff,"ON,");					
			else 						sprintf(logwritetBuff,"OFF,");		
			f_puts(logwritetBuff,&file); 		

			
			sprintf(logwritetBuff,"%02d-%02d-%02d,",Save_year,Save_month,Save_day);	f_puts(logwritetBuff,&file); 
			sprintf(logwritetBuff,"%02d:%02d:%02d,",Save_hour,Save_min,Save_sec);	f_puts(logwritetBuff,&file); 
			
			//Result Write
			if(pf_flag == true)			{	sprintf(logwritetBuff, "PASS,");		f_puts(logwritetBuff,&file); 	}
			else if(pf_flag == false)	{	sprintf(logwritetBuff, "FAIL,");		f_puts(logwritetBuff,&file); 	}
			
			//Count Write
			sprintf(logwritetBuff,"%d,", QTY.TT); 		f_puts(logwritetBuff,&file); 
			sprintf(logwritetBuff,"%d,", QTY.OK); 		f_puts(logwritetBuff,&file); 
			sprintf(logwritetBuff,"%d,", QTY.NG); 		f_puts(logwritetBuff,&file); 
			
			//Tack time Write
			sprintf(logwritetBuff,"%.2f,",tact_time);	f_puts(logwritetBuff,&file); 
			sprintf(logwritetBuff,"%s,",open_disp);	 	f_puts(logwritetBuff,&file); 
			sprintf(logwritetBuff,"%s,",short_disp);	f_puts(logwritetBuff,&file); 		
		
			//Part Result Write
			for(int cnt = 0; cnt < part_count; cnt++)
			{
				if(cnt < Pt_end_idx - Pt_str_idx)
				{
					if(RD_LOG_UNIT == true)			{	sprintf(logwritetBuff,"%s,",    test_meas_string[cnt+Pt_str_idx]);	f_puts(logwritetBuff,&file); 	}
					else if(RD_LOG_UNIT == false)	{	sprintf(logwritetBuff,"%.02f,", test_meas_value[cnt+Pt_str_idx]);	f_puts(logwritetBuff,&file); 	}
				}
				else
				{
					sprintf(logwritetBuff,"-,");	f_puts(logwritetBuff,&file); 
				}
			}

			//Func Result Write
			for(int cnt = 0; cnt < func_count; cnt++)
			{
				if(cnt < Fn_end_idx - Fn_str_idx)
				{
					if(func_log_flag[cnt] == true)	
					{
						sprintf(logwritetBuff,"%s,",    test_meas_string[cnt+Fn_str_idx]);	f_puts(logwritetBuff,&file);
					}
				}
				else
				{
					if(func_log_flag[cnt] == true)	
					{
						sprintf(logwritetBuff,"-,");	f_puts(logwritetBuff,&file); 
					}
				}
			}
			f_close(&file);  
		}
	
		if(func_count == 0)	goto endflow;
		MainPopup(true, GUI_LIGHTBLUE, "LOG SAVE TO USB\nFunction");	
		dbgmsg(">> Function save : %s\n",FUN_FIL);
		if(f_open(&file, FUN_FIL, FA_OPEN_ALWAYS |FA_WRITE) == FR_OK)
		{		   
			size = (&file)->fsize;
			f_lseek(&file,size); 
			
			if(size == 0)
			{					
				//// label
				sprintf(logwritetBuff,"\nProject,");			f_puts(logwritetBuff,&file); 
				sprintf(logwritetBuff,"Serial,");				f_puts(logwritetBuff,&file); 
				sprintf(logwritetBuff,"Version,");				f_puts(logwritetBuff,&file);
				sprintf(logwritetBuff,"VCC1,");				f_puts(logwritetBuff,&file);
				sprintf(logwritetBuff,"VCC2,");				f_puts(logwritetBuff,&file);
				sprintf(logwritetBuff,"NG Cont.,");				f_puts(logwritetBuff,&file);
				
				sprintf(logwritetBuff,"Date,Time,Result,");		f_puts(logwritetBuff,&file); 
				sprintf(logwritetBuff,"QTY,PASS,FAIL,");		f_puts(logwritetBuff,&file); 

				// add func test index
				for(int cnt = 0; cnt < func_count; cnt++)
				{
					sprintf(logwritetBuff,"%s,",func_name[cnt]);
					if(func_log_flag[cnt] == true)	f_puts(logwritetBuff,&file);
				}
			}

			//// data	
			//Time Write
			sprintf(logwritetBuff,"\n%s,",project_name_main);	f_puts(logwritetBuff,&file); 

			sprintf(logwritetBuff,"0x%08lX,",serial_num);		f_puts(logwritetBuff,&file); 
			
			if(RD_DBG_MD == 0)	{	sprintf(logwritetBuff,"KM-V%1.2f(D)),",sys_ver);	}
			else				{	sprintf(logwritetBuff,"KM-V%1.2f(R),",sys_ver);	}
		
			f_puts(logwritetBuff,&file); 

			sprintf(logwritetBuff,"%.2lfV,",Evcc1_Vlot);	f_puts(logwritetBuff,&file); 	
			sprintf(logwritetBuff,"%.2lfV,",Evcc2_Vlot);	f_puts(logwritetBuff,&file); 	
			
			if(RD_NG_CONTINUE == true)	sprintf(logwritetBuff,"ON,");
			else 						sprintf(logwritetBuff,"OFF,");
			f_puts(logwritetBuff,&file);

			sprintf(logwritetBuff,"%02d-%02d-%02d,",Save_year,Save_month,Save_day);	f_puts(logwritetBuff,&file); 
			sprintf(logwritetBuff,"%02d:%02d:%02d,",Save_hour,Save_min,Save_sec);	f_puts(logwritetBuff,&file); 

			//Result Write
			if(pf_flag == true)			{	sprintf(logwritetBuff, "PASS,");		f_puts(logwritetBuff,&file); 	}
			else if(pf_flag == false)	{	sprintf(logwritetBuff, "FAIL,");		f_puts(logwritetBuff,&file); 	}

			//Count Write
			sprintf(logwritetBuff,"%d,", QTY.TT); 		f_puts(logwritetBuff,&file); 
			sprintf(logwritetBuff,"%d,", QTY.OK); 		f_puts(logwritetBuff,&file); 
			sprintf(logwritetBuff,"%d,", QTY.NG); 		f_puts(logwritetBuff,&file); 
			
			//Func Result Write
			for(int cnt = 0; cnt < func_count; cnt++)
			{
				if(cnt < Fn_end_idx - Fn_str_idx)
				{
					if(func_log_flag[cnt] == true)	
					{
						sprintf(logwritetBuff,"%s,",    test_meas_string[cnt+Fn_str_idx]);	f_puts(logwritetBuff,&file);
					}
				}
				else
				{
					if(func_log_flag[cnt] == true)	
					{
						sprintf(logwritetBuff,"-,");	f_puts(logwritetBuff,&file); 
					}
				}
			}
			f_close(&file);  
		}

		MainPopup(true, GUI_LIGHTBLUE, "LOG SAVE TO USB\n1D Panel");	
		dbgmsg(">> 1D Panel save : %s\n",PN1_FIL);                                          
		if(f_open(&file, PN1_FIL, FA_OPEN_ALWAYS |FA_WRITE) == FR_OK) 
		{								
			size = (&file)->fsize;
			f_lseek(&file,size);
			
			if(size == 0)
			{
				//// label
				sprintf(logwritetBuff,"ITEM,Count,Tx,Rx,Error,Value\n");			f_puts(logwritetBuff,&file); 
			}
			
			sprintf(logwritetBuff,"Result,");			f_puts(logwritetBuff,&file);
			sprintf(logwritetBuff,"%d,",QTY.TT);		f_puts(logwritetBuff,&file);
			if(pf_flag == true)						{	sprintf(logwritetBuff, "PASS\n");		f_puts(logwritetBuff,&file); 	}
			else if(pf_flag == false)				{	sprintf(logwritetBuff, "FAIL\n");		f_puts(logwritetBuff,&file); 	}
			
			////////////////
			sprintf(logwritetBuff,"Time,");					f_puts(logwritetBuff,&file);
			sprintf(logwritetBuff,"%d,",QTY.TT);										f_puts(logwritetBuff,&file);
			sprintf(logwritetBuff,"%02d-%02d-%02d ",Save_year,Save_month,Save_day);	f_puts(logwritetBuff,&file); 
			sprintf(logwritetBuff,"%02d:%02d:%02d\n",Save_hour,Save_min,Save_sec);	f_puts(logwritetBuff,&file); 
			
			for(int cnt = 0; cnt < func_count; cnt++)
			{				
				if(func_panel_link[cnt] != false)	
				{
					uint16_t panel_cnt = func_panel_link[cnt] - 1;
					uint16_t panel_float = (uint16_t)func_spec_float[cnt];
					uint16_t tx_num = panel_tx_num[panel_cnt];
					uint16_t rx_num = panel_rx_num[panel_cnt];								
					sprintf(logwritetBuff,"%s,",func_name[cnt]);			f_puts(logwritetBuff,&file); 
					sprintf(logwritetBuff,"%d,",QTY.TT);					f_puts(logwritetBuff,&file);
					sprintf(logwritetBuff,"%d,%d,",tx_num,rx_num);			f_puts(logwritetBuff,&file); 
					sprintf(logwritetBuff,"%d,",ErrorNodeCount[panel_cnt]);	f_puts(logwritetBuff,&file); 
					
					uint16_t node = 0;
					for(int tx = 0; tx < tx_num; tx++)
					{
						for(int rx = 0; rx < rx_num; rx++)
						{
							if(panel_float == 0)	sprintf(logwritetBuff,"%d,",panel_data[panel_cnt][node]);		
							else 					sprintf(logwritetBuff,"%.3f,",panel_dataf[panel_cnt][node]);		
							
							f_puts(logwritetBuff,&file); 
							node++;
						}
					}
					sprintf(logwritetBuff,",\n");		f_puts(logwritetBuff,&file); 
				}
			}				
			f_close(&file);
		}
					
		MainPopup(true, GUI_LIGHTBLUE, "LOG SAVE TO USB\n2D Panel");	
		dbgmsg(">> 2D Panel save : %s\n",PN2_FIL);
		if(f_open(&file, PN2_FIL, FA_OPEN_ALWAYS |FA_WRITE) == FR_OK)
		{								
			size = (&file)->fsize;
			f_lseek(&file,size); 
			
			if(size == 0)
			{
				//// label
				sprintf(logwritetBuff,"Project Name,");			f_puts(logwritetBuff,&file); 
				sprintf(logwritetBuff,"Date,Time,Result,");		f_puts(logwritetBuff,&file); 
				sprintf(logwritetBuff,"QTY,PASS,FAIL,");		f_puts(logwritetBuff,&file); 
				sprintf(logwritetBuff,"Item,Nodes,\n");		f_puts(logwritetBuff,&file); 
			}
			
			//// data	
			//Time Write
			sprintf(logwritetBuff,"%s,",project_name_main);						f_puts(logwritetBuff,&file); 
			sprintf(logwritetBuff,"%02d-%02d-%02d,",Save_year,Save_month,Save_day);	f_puts(logwritetBuff,&file); 
			sprintf(logwritetBuff,"%02d:%02d:%02d,",Save_hour,Save_min,Save_sec);	f_puts(logwritetBuff,&file); 
			
			//Result Write
			if(pf_flag == true)			{	sprintf(logwritetBuff, "PASS,");		f_puts(logwritetBuff,&file); 	}
			else if(pf_flag == false)	{	sprintf(logwritetBuff, "FAIL,");		f_puts(logwritetBuff,&file); 	}
		
			//Count Write
			sprintf(logwritetBuff,"%d,", QTY.TT); 		f_puts(logwritetBuff,&file); 
			sprintf(logwritetBuff,"%d,", QTY.OK); 		f_puts(logwritetBuff,&file); 
			sprintf(logwritetBuff,"%d,", QTY.NG); 		f_puts(logwritetBuff,&file); 
		
			uint8_t display_count = 0;
			for(int cnt = 0; cnt < func_count; cnt++)
			{				
				if(func_panel_link[cnt] != false)	
				{
					if (++display_count >= 2){ sprintf(logwritetBuff,",,,,,,,"); f_puts(logwritetBuff,&file); }
					uint16_t panel_cnt = func_panel_link[cnt] - 1;
					uint16_t tx_num = panel_tx_num[panel_cnt];
					uint16_t rx_num = panel_rx_num[panel_cnt];
					uint16_t panel_float = (uint16_t)func_spec_float[cnt];
					
					sprintf(logwritetBuff,"%s(err:%d),",func_name[cnt],ErrorNodeCount[panel_cnt]);		f_puts(logwritetBuff,&file); 
					
					uint16_t node = 0;
					for(int tx = 0; tx < tx_num; tx++)
					{
						if (tx > 0)
						{
							sprintf(logwritetBuff,",\n,,,,,,,,");		f_puts(logwritetBuff,&file); 
						}
						for(int rx = 0; rx < rx_num; rx++)
						{
							if(panel_float == 0)	sprintf(logwritetBuff,"%d,",panel_data[panel_cnt][node]);		
							else 					sprintf(logwritetBuff,"%.3f,",panel_dataf[panel_cnt][node]);	
							f_puts(logwritetBuff,&file); 
							node++;
						}
					}
					sprintf(logwritetBuff,",\n\n");		f_puts(logwritetBuff,&file); 
				}
			}
			f_close(&file);
		}
				
		if(RD_SDC_SAVE == 0)	goto endflow;
		
		MainPopup(true, GUI_LIGHTBLUE, "LOG SAVE TO USB\nSDC Panel");	
		dbgmsg(">> SDC Log  save : %s\n",SDC_FIL);
		if(f_open(&file, SDC_FIL, FA_OPEN_ALWAYS |FA_WRITE) == FR_OK)				
		{					
			size = (&file)->fsize;
			f_lseek(&file,size); 
							
			for(int cnt = 0; cnt < func_count; cnt++)
			{
				if(func_panel_link[cnt] != false)	
				{					
					uint16_t panel_cnt = func_panel_link[cnt] - 1;
					uint16_t panel_float = (uint16_t)func_spec_float[cnt];					
					uint16_t tx_num = panel_tx_num[panel_cnt];
					uint16_t rx_num = panel_rx_num[panel_cnt];					
					uint16_t node = 0;										
			
					node = 0;
					for(int tx = 0; tx < tx_num; tx++)
					{
						for(int rx = 0; rx < rx_num; rx++)
						{
							if(panel_float == 0) sdc_data[rx][tx] 	= panel_data[panel_cnt][node];
							else 				 sdc_dataf[rx][tx] 	= panel_dataf[panel_cnt][node];
							node++;
						}
					}								
					
					sprintf(logwritetBuff,"Date,");		f_puts(logwritetBuff,&file); 
					sprintf(logwritetBuff,"Cell ID,");	f_puts(logwritetBuff,&file); 
					sprintf(logwritetBuff,"CH ID,");	f_puts(logwritetBuff,&file); 
					sprintf(logwritetBuff,"ZONE,");		f_puts(logwritetBuff,&file); 
					sprintf(logwritetBuff,"PG CH,");	f_puts(logwritetBuff,&file); 
					sprintf(logwritetBuff,"TITLE,");	f_puts(logwritetBuff,&file); 
					sprintf(logwritetBuff,"TYPE,");		f_puts(logwritetBuff,&file); 
					sprintf(logwritetBuff,"X,");		f_puts(logwritetBuff,&file); 
					sprintf(logwritetBuff,"Y,");		f_puts(logwritetBuff,&file); 
					sprintf(logwritetBuff,"COUNT,");	f_puts(logwritetBuff,&file); 
					
					if(tx_num == 1)
					{
						for(int rx = 0; rx < rx_num; rx++)
						{
							sprintf(logwritetBuff,"X%02d,",rx);	f_puts(logwritetBuff,&file); 
						}
					}
					else
					{
						for(int tx = 0; tx < tx_num; tx++)
						{
							sprintf(logwritetBuff,"X%02d,",tx);	f_puts(logwritetBuff,&file); 
						}
					}
					sprintf(logwritetBuff,",\n");		f_puts(logwritetBuff,&file); 
					//////////////////////////////////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////////					
					
					if(tx_num == 1)
					{
						float avr 	= 0.0;
						int sum   	= 0;
						float sumf	= 0;
						int max   	= -65535;
						int min   	=  65535;
						
						for(int tx = 0; tx < tx_num; tx++)
						{
							sprintf(logwritetBuff,"%02d-%02d-%02d %02d:%02d:%02d,",Save_year,Save_month,Save_day,Save_hour,Save_min,Save_sec);	
							f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"SPL%d,",QTY.TT);			f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%d,",QTY.TT);			f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%d,",QTY.TT);			f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%d,",QTY.TT);			f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%s,",func_name[cnt]);	f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%d,",0);					f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%d,",rx_num);			f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%d,",tx_num);			f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%02d,",tx);				f_puts(logwritetBuff,&file); 
							
							for(int rx = 0; rx < rx_num; rx++)
							{										
								if(panel_float == 0) 
								{
									sprintf(logwritetBuff,"%d,",sdc_data[rx][tx]);	f_puts(logwritetBuff,&file); 
									sum += sdc_data[rx][tx];
									if(sdc_data[rx][tx] < min) min = sdc_data[rx][tx];
									if(sdc_data[rx][tx] > max) max = sdc_data[rx][tx];
								}
								else 	
								{
									sprintf(logwritetBuff,"%.3f,",sdc_dataf[rx][tx]);	f_puts(logwritetBuff,&file); 
									sumf += sdc_dataf[rx][tx];
									if(sdc_dataf[rx][tx] < (float)min) min = (int)sdc_dataf[rx][tx];
									if(sdc_dataf[rx][tx] > (float)max) max = (int)sdc_dataf[rx][tx];
								}
							}
							sprintf(logwritetBuff,",\n");		f_puts(logwritetBuff,&file); 
						}
						sprintf(logwritetBuff,"%02d-%02d-%02d %02d:%02d:%02d,",Save_year,Save_month,Save_day,Save_hour,Save_min,Save_sec);	f_puts(logwritetBuff,&file); 
						sprintf(logwritetBuff,"SPL%d,",QTY.TT);			f_puts(logwritetBuff,&file); 
						sprintf(logwritetBuff,"%d,",QTY.TT);	f_puts(logwritetBuff,&file); 
						sprintf(logwritetBuff,"%d,",QTY.TT);	f_puts(logwritetBuff,&file); 
						sprintf(logwritetBuff,"%d,",QTY.TT);	f_puts(logwritetBuff,&file); 
						sprintf(logwritetBuff,"%s,",func_name[cnt]);	f_puts(logwritetBuff,&file); 
						
						if(ErrorNodeCount[panel_cnt] == 0)		sprintf(logwritetBuff,"GOOD,");														
						else									sprintf(logwritetBuff,"FAIL,");	
						f_puts(logwritetBuff,&file); 
									
						if(panel_float == 0) 	avr = (( (float)sum) / ((float)(rx_num*tx_num)) );
						else					avr = (( (float)sumf) / ((float)(rx_num*tx_num)) );
						
						sprintf(logwritetBuff,"%f,",avr);	f_puts(logwritetBuff,&file); 
						sprintf(logwritetBuff,"%d,",max);	f_puts(logwritetBuff,&file); 
						sprintf(logwritetBuff,"%d,",min);	f_puts(logwritetBuff,&file); 							
						sprintf(logwritetBuff,",\n");		f_puts(logwritetBuff,&file); 
					}
					else
					{
						float avr 	= 0.0;
						int sum   	= 0;
						float sumf	= 0;
						int max   	= -65535;
						int min   	=  65535;
						for(int rx = 0; rx < rx_num; rx++)
						{
							sprintf(logwritetBuff,"%02d-%02d-%02d %02d:%02d:%02d,",Save_year,Save_month,Save_day,Save_hour,Save_min,Save_sec);	f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"SPL%d,",QTY.TT);	f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%d,",QTY.TT);	f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%d,",QTY.TT);	f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%d,",QTY.TT);	f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%s,",func_name[cnt]);	f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%d,",0);	f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%d,",tx_num);	f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%d,",rx_num);	f_puts(logwritetBuff,&file); 
							sprintf(logwritetBuff,"%02d,",rx);	f_puts(logwritetBuff,&file); 
							for(int tx = 0; tx < tx_num; tx++)
							{
								if(panel_float == 0) 
								{
									sprintf(logwritetBuff,"%d,",sdc_data[rx][tx]);	f_puts(logwritetBuff,&file); 
									sum += sdc_data[rx][tx];
									if(sdc_data[rx][tx] < min) min = sdc_data[rx][tx];
									if(sdc_data[rx][tx] > max) max = sdc_data[rx][tx];
								}
								else 	
								{
									sprintf(logwritetBuff,"%.3f,",sdc_dataf[rx][tx]);	f_puts(logwritetBuff,&file); 
									sumf += sdc_dataf[rx][tx];
									if(sdc_dataf[rx][tx] < (float)min) min = (int)sdc_dataf[rx][tx];
									if(sdc_dataf[rx][tx] > (float)max) max = (int)sdc_dataf[rx][tx];
								}
								
							}
							sprintf(logwritetBuff,",\n");		f_puts(logwritetBuff,&file); 
						}
						sprintf(logwritetBuff,"%02d-%02d-%02d %02d:%02d:%02d,",Save_year,Save_month,Save_day,Save_hour,Save_min,Save_sec);	f_puts(logwritetBuff,&file); 
						sprintf(logwritetBuff,"SPL%d,",QTY.TT);			f_puts(logwritetBuff,&file);
						sprintf(logwritetBuff,"%d,",QTY.TT);	f_puts(logwritetBuff,&file); 
						sprintf(logwritetBuff,"%d,",QTY.TT);	f_puts(logwritetBuff,&file); 
						sprintf(logwritetBuff,"%d,",QTY.TT);	f_puts(logwritetBuff,&file);  
						sprintf(logwritetBuff,"%s,",func_name[cnt]);	f_puts(logwritetBuff,&file); 
						
						if(ErrorNodeCount[panel_cnt] == 0)		sprintf(logwritetBuff,"GOOD,");								
						else									sprintf(logwritetBuff,"FAIL,");	
						f_puts(logwritetBuff,&file); 
						
						if(panel_float == 0) 	avr = (( (float)sum) / ((float)(rx_num*tx_num)) );
						else					avr = (( (float)sumf) / ((float)(rx_num*tx_num)) );
						sprintf(logwritetBuff,"%f,",avr);	f_puts(logwritetBuff,&file); 
						sprintf(logwritetBuff,"%d,",max);	f_puts(logwritetBuff,&file); 
						sprintf(logwritetBuff,"%d,",min);	f_puts(logwritetBuff,&file); 							
						sprintf(logwritetBuff,",\n");		f_puts(logwritetBuff,&file); 
					}			
				}
			}
			f_close(&file);
		}		
		endflow:
		
		f_mount(0, NULL);
	}
}

void LogSavePc(uint8_t pf_flag)
{
	if(is_pc_connected_flag == 0)	return;	
		
	MainPopup(true, GUI_LIGHTBLUE, "LOG SAVE TO PC");	
	
	log_save_pc(pf_flag);
	
}

void Log_Save(uint8_t num, uint8_t result)
{	
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);		RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
	
	Save_hour = RTC_TimeStr.RTC_Hours; Save_min = RTC_TimeStr.RTC_Minutes; Save_sec = RTC_TimeStr.RTC_Seconds;
	Save_year = RTC_DateStr.RTC_Year;  Save_month = RTC_DateStr.RTC_Month; Save_day = RTC_DateStr.RTC_Date; 
	
	EVCC_Check();
	
	if(num == 0x55)			{	LogSaveUsb(result);	}
	else if(num == 0x50)	{ 	LogSavePc(result);	}
	
	MainPopup(false,GUI_LIGHTBLUE,"");
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
