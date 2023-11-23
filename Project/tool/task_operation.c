#include "task_operation.h"
float tact_time = 0;
void StopWatch(uint8_t flag)
{	
	static uint64_t time_cnt_pre; 
	static uint64_t time_cnt_post; 
	static uint8_t time_check_start = false; 
	char text[20];  
    
    if(flag == STOPWATCH_START)
    {
		//dbgmsg(">> Time Check Start\n");
		time_check_start = true;
        time_cnt_pre = LocalTime;
        time_cnt_post = time_cnt_pre;    
        tact_time = (((double)(time_cnt_post - time_cnt_pre))/1000000.0);   		
        sprintf(text,"%5.2f sec", tact_time); 
		//dbgmsg(">> Tact Time Start : %s\n",text);			
		hItem = WM_GetDialogItem(hStatewin, STATE_TACT);
        TEXT_SetText(hItem,text); 
		GUI_Exec();
    }
    else if(flag == STOPWATCH_CHECK)
    {              
        time_cnt_post = LocalTime;    
        tact_time = (((double)(time_cnt_post - time_cnt_pre))/1000000.0);
        sprintf(text,"%5.2f sec", tact_time);   		
		//dbgmsg(">> Tact Time Check : %s\n",text);		
		hItem = WM_GetDialogItem(hStatewin, STATE_TACT);
        TEXT_SetText(hItem,text); 
		GUI_Exec();
    }
    else if(flag == STOPWATCH_END)
    {        
		if(time_check_start == false) return;      
		time_check_start = false;
        time_cnt_post = LocalTime;    
        tact_time = (((double)(time_cnt_post - time_cnt_pre))/1000000.0);          
        sprintf(text,"%5.2f sec", tact_time);   		
		//dbgmsg(">> Tact Time End: %s\n",text);		
		hItem = WM_GetDialogItem(hStatewin, STATE_TACT);
        TEXT_SetText(hItem,text); 
		GUI_Exec();		
    }
	
	if( is_pc_connected_flag == true )
	{	
		CustomStrCat(log_txbuff, "%d,",flag);
		tx_pc_data(STOP_WATCH, strlen(log_txbuff), log_txbuff);
	}
	else
	{
		
	}
}
char SafeSensor_Flag = 0;
uint8_t WaitButton(uint16_t no, uint16_t state, uint16_t time)
{	
	char text[50];  
	
	if(state == 1)	    {sprintf(text,"Press start1\nGo to next flow");				}
	else if(state == 2)	{sprintf(text,"Press start2\nGo to next flow");				}
	else if(state == 3)	{sprintf(text,"Press start1 + start2\nGo to next flow");	}
	else				return false;
	
	MainPopup(true, GUI_LIGHTRED, text);
	Delay_ms(50);
	
	while(1)
	{
		uint8_t key = KeyScan();
				
		if( (key == 0x00) && (is_pc_connected_flag == true) )
		{
			if(test_state_flag != 0)
			{
				key = test_state_flag;
				test_state_flag = 0;
			}
		}
		
		if( key == state)
		{				
			if(SafeSensor_Flag == true)
			{

			}
			
			MainPopup(false, GUI_LIGHTRED, "");   
			return true;
		}
		else if(key == ESC_ON)
		{			
			MainPopup(false, GUI_LIGHTRED, "");  
			return false;
		}
	}
}

char MainResult_PID[50];
char MainResult_VID[50];
char MainResult_Boot[50];
char MainResult_Core[50];
char MainResult_App[50];
char MainResult_Para[50];
char MainResult_CRC[50];
char MainResult_Full[100];

extern uint8_t firmwareversion_error;
extern char str_err_event_code[50];
void MainResult(uint8_t final_result)            
{	  
	if(final_result == true )		
	{		
		//MainPopupResult(true,final_result,"PASS"); 			
		//LJB
		sprintf(MainResult_Full,"[PASS]     PID : %s\n                VID : %s\n              Boot : %s\n              Core : %s\n               App : %s\n              Para : %s\n              CRC : %s\n",MainResult_PID,MainResult_VID,MainResult_Boot,MainResult_Core,MainResult_App,MainResult_Para,MainResult_CRC);
		MainPopupResult(true,final_result,MainResult_Full); 
		//LJB	
		
		OKLED(true);
	}	
	else if(final_result == false )	
	{			
		char text[100];
		char optmp[20];
		char shtmp[20];
		char pttmp[20];
		char fntmp[20];
		
		if( op_auto_result == false)    	{	sprintf(optmp,"Open  : FAIL"); 	}
		else if( op_auto_result == ESC_KEY)	{	sprintf(optmp,"Open  : SKIP"); 	}
		else                            	{	sprintf(optmp,"Open  : PASS");	}
		
		if( sh_auto_result == false)    	{	sprintf(shtmp,"Short : FAIL");	}
		else if( sh_auto_result == ESC_KEY)	{	sprintf(shtmp,"Short : SKIP");	}
		else                            	{	sprintf(shtmp,"Short : PASS");	}
		
		if( pt_auto_result == false)    	{	sprintf(pttmp,"Part  : FAIL");	}
		else if( pt_auto_result == ESC_KEY)	{	sprintf(pttmp,"Part  : SKIP");	}
		else                            	{	sprintf(pttmp,"Part  : PASS");	}
		
		if( fn_auto_result == false)   	 	{	sprintf(fntmp,"Func  : FAIL");	}
		else if( fn_auto_result == ESC_KEY)	{	sprintf(fntmp,"Func  : SKIP");	}
		else					        	{	sprintf(fntmp,"Func  : PASS");	}
		
		//if(firmwareversion_error == true) sprintf(text,"FAIL\n %s ",str_err_event_code);
//			else sprintf(text,"FAIL\n %s / %s\n%s / %s",optmp,shtmp,pttmp,fntmp);		
		//sprintf(str_err_event_code," ");
		
		MainPopupResult(true,final_result,text); 
		
		//MainPopupResult(true,final_result,"FAIL"); 
		
		NGLED(true);	
		BUZZER(true);
		EscWait();
	}
	return;
}
void MakingUnit(uint8_t result, uint8_t no, uint8_t state, uint16_t time)
{
	if(result == true)
	{		
		if(no == 0)// GPIO 제어시
		{
			if(state == true)		dbgmsg("Port high\n");
			else if(state == false)	dbgmsg("Port low\n");
			Delay_ms(time);
		}
		else if( (1 <= no) && (no <= 8) )	// pen making
		{
			CylinderControl(no, state, time);
		}
	}
	else
	{
		
	}
}
		
void EvccControl(uint8_t num,uint8_t st)
{			
	if( num == 1 )		{	VCC1_ON(st); }
	else if( num == 2 )	{	VCC2_ON(st); }
	else if( num == 3 )	{	VCC3_ON(st); }
	//else				{	VCC1_ON(false);	VCC2_ON(false);	VCC3_ON(false);	}
	else				{	VCC1_ON(false);	VCC2_ON(false);	VCC3_ON(true);	} //VCC 반대
}

int safe_check_sol_number = 0;
void safe_sen_seq_update(uint8_t no, uint8_t state, uint16_t time)
{
	safe_check_sol_number = no;
}
uint8_t WaitAutoAlign(uint16_t no, uint16_t state, uint16_t time)
{	
	char text[50];  
	
	sprintf(text,"Check Auto Align");	
	MainPopup(true, GUI_LIGHTRED, text);
	Delay_ms(100);
	
	char data[1];
	data[0] = state;
	tx_pc_data(VISON_STATE, 1, data);
	
	state = 3;
	while(1)
	{
		uint8_t key = KeyScan();
				
		if( (key == 0x00) && (is_pc_connected_flag == true) )
		{
			if(test_state_flag != 0)
			{
				key = test_state_flag;
				test_state_flag = 0;
			}
		}
		
		if( key == state)
		{				
			//MainPopup(false, GUI_LIGHTRED, "");
			dbgmsg("Align Match OK\n");
			return true;
		}
		else if(key == ESC_ON)
		{			
			//MainPopup(false, GUI_LIGHTRED, "");  
			dbgmsg("Align Match NG\n");
			return false;
		}
	}
}
void TaskEndFlow(void)
{
	
}

void seq_flow_send(uint8_t flag)
{
	char data[1];
	data[0] = flag;
	tx_pc_data(SEQ_STATE, 1, data);
}

uint8_t is_insp_flag	 = false;
void operation(uint16_t seq, uint16_t *run, uint16_t *num, uint16_t *state, uint16_t *time)
{
	uint8_t goto_end_flow 	 = false;
	uint8_t is_end_flow		 = false;
	is_insp_flag	 = false;
  
	if (seq > 100) return;
	
	seq_flow_send(1);
	
	for(int cnt = 0; cnt < seq; cnt++)
	{
		if (goto_end_flow == true)
		{
			if (is_end_flow == false)
			{
				if (run[cnt] != END_FLOW) continue;  
				else  is_end_flow = true;
			}
		}
		switch(run[cnt])
		{
			case OPEN_TEST:
				dbgmsg("OP:OP_TEST(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				test_result_flag = Open_test(num[cnt], state[cnt] ,test_result_flag);
				test_index_pc(OPEN_STATE, Op_str_idx, Op_end_idx, op_auto_result,test_result_flag);
				if( test_result_flag  == false ) { 	if(RD_NG_CONTINUE == false)	goto_end_flow = true; 	}		
			break;
			case SHORT_TEST:
				dbgmsg("OP:SH_TEST(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				test_result_flag = Short_test(num[cnt], state[cnt] ,test_result_flag);
				test_index_pc(SHORT_STATE, Sh_str_idx, Sh_end_idx, sh_auto_result,test_result_flag);
				if( test_result_flag  == false ) { 	if(RD_NG_CONTINUE == false)	goto_end_flow = true; 	}		
			break;					
			case PART_TEST:
				dbgmsg("OP:PT_TEST(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				test_result_flag = Part_test(num[cnt], state[cnt] ,test_result_flag);
				test_index_pc(PART_STATE, Pt_str_idx, Pt_end_idx, pt_auto_result,test_result_flag);
				if( test_result_flag  == false ) { 	if(RD_NG_CONTINUE == false)	goto_end_flow = true; 	}			
			break;				
			case FUNC_TEST:   
				dbgmsg("OP:FN_TEST(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				test_result_flag = Func_test(num[cnt], state[cnt] ,test_result_flag);
				test_index_pc(FUNC_STATE, Fn_str_idx, Fn_end_idx, fn_auto_result,test_result_flag);
				if( test_result_flag  == false ) { 	if(RD_NG_CONTINUE == false)	goto_end_flow = true; 	}		
			break;
			case SOL_RUN:	
				dbgmsg("OP:SOL_RUN(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				if(CylinderControl(num[cnt],state[cnt],time[cnt])==false)	goto_end_flow = true;
			break;
			case CY_SEN_CHK:
				dbgmsg("OP:CYL_SEN(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				if(CylinderSensorCheck(num[cnt],state[cnt],time[cnt])==false)	goto_end_flow = true;
			break;		
			case SAFE_SEN_CHK:
				dbgmsg("OP:SAFE_SEN_CHK(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				safe_sen_seq_update(num[cnt],state[cnt],time[cnt]);
			break;
			case MARKING_UNIT: 	
				dbgmsg("OP:MARKING(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				if (is_insp_flag == false) 			break;
				if (test_result_flag == ESC_KEY) 	break;
				MakingUnit(test_result_flag, num[cnt], state[cnt], time[cnt]);
			break;	
			case RELAY	: 				
				dbgmsg("OP:FN_RELY(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				if(FuncRYControl(num[cnt],state[cnt],time[cnt])==false)		goto_end_flow = true;
			break;
			case WAIT_KEY:
				dbgmsg("OP:KETWAIT(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				if(WaitButton(num[cnt],state[cnt],time[cnt]) == false)		goto_end_flow = true;			
			break;		
			case TACT_TIME:
				dbgmsg("OP:TT_CHEK(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				StopWatch(state[cnt]);
			break;  
			case VCC:
				dbgmsg("OP:VCC_CON(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				EvccControl(num[cnt], state[cnt]);
			break;
			case LOG_SAVE:	
				dbgmsg("OP:LOGSAVE(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				if (is_insp_flag == false) 			break;  
				if (test_result_flag == ESC_KEY) 	break;
				Log_Save(state[cnt],test_result_flag);	
			break;
			case START_FLOW:
				dbgmsg("OP:ST_FLOW(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				is_insp_flag = true; 
				TaskStartFlow();
				start_flow_pc();
			break;
			case END_FLOW:
				dbgmsg("OP:ENDFLOW(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				if (is_insp_flag == false) 			break;  
				if (test_result_flag == ESC_KEY) 	break;
				if(state[cnt]== true)	
				{						
						EvccControl(1,false); //
						EvccControl(2,false);
						Delay_ms(10);
						DVDD_ON(false);
						Delay_ms(50);
						RYBD_6ON(false);
						Delay_ms(250);	
						AVDD_ON(false);
						RYBD_1ON(false);
						RYBD_2ON(false);
						RYBD_3ON(false);
						RYBD_4ON(false);
						RYBD_5ON(false);						
						Delay_ms(250);		
					InspCountWrite(test_result_flag);
				}
			break;
			case DISP_RESULT:
				dbgmsg("OP:DISPRST(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				if (is_insp_flag == false) 			break;  
				if (test_result_flag == ESC_KEY) 	break;
				MainResult(test_result_flag);	
			break;
			case AUTO_ALIGN:
				dbgmsg("OP:AUTO_ALIGN(%d,%d,%d)\n",num[cnt],state[cnt],time[cnt]);
				if(WaitAutoAlign(num[cnt],state[cnt],time[cnt]) == false)		goto_end_flow = true;
			break;
		}
	}
	is_insp_flag = false;
	
	seq_flow_send(0);
}

uint8_t ESC_LIM = 1;
uint8_t ESC_IN_TEST = false;
uint32_t ESC_TIM_CNT = 0;
void OperEscKeyCheck(void)	//50ms
{
	if( ( is_insp_flag == true) && (ESC_IN_TEST == false) )
	{					
		uint8_t ESC_IN = ( ( KeyRead() >> 2 ) & 0x01 );
		if ( ESC_IN == 0)
		{						
			ESC_IN_TEST = true;
		}
		else
		{
			ESC_TIM_CNT = 0;
		}
	}
	else
	{
		ESC_IN_TEST = false;
		ESC_TIM_CNT = 0;
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
