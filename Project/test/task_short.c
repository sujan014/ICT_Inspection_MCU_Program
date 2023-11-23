#include "task_short.h"

#define DISPLAY_MULTI_LINE false

uint8_t Short_test(uint8_t num, uint8_t st, uint8_t result)
{
	uint8_t ret = false;
	if(result == false)
	{
		if(RD_NG_CONTINUE == true)
		{
			Sh_str_idx = test_count;
			if(st == AUTO_STATE)
			{					
				sh_auto_result = ret = Short_Insp_Auto();	
				if(ret == ESC_KEY)
				{
					Sh_end_idx = test_count;
					result = ESC_KEY; 
					return ESC_KEY;
				}
			}
			else if(st == STEP_STATE)	
			{
				ret = Short_Insp_Step();	
			}
			Sh_end_idx = test_count;
			if(ret == false)	result = false;
		}
		else
		{
			return false;
		}
	}
	else if(result == true)
	{
		Sh_str_idx = test_count;					
		
		if(st == AUTO_STATE)
		{	
			sh_auto_result = ret = Short_Insp_Auto();	
			if(ret == ESC_KEY) 
			{
				Sh_end_idx = test_count;
				result = ESC_KEY; 
				return ESC_KEY;
			}
		}
		else if(st == STEP_STATE)
		{
			ret = Short_Insp_Step();	
		}
		
		Sh_end_idx = test_count;
		
		if(ret == false)	result = false;
	}	
	return result;
}


uint8_t Short_Insp_Step(void)
{
	float 	 value = 0;			// 검사결과값 받아올 변수
    char  	 text[100];			// Display 용 배열
    bool 	 add_flag  = true;	// listview 사용시 라인추가하기 위해 선언한 변수
	
	uint8_t  key_ret = 0;
	uint16_t sh_range = 0;
	float 	 sh_spec  = 0;	

		dbgmsg(">> Short Test(Step)_all\n");	

		for(int cnt = 0; cnt < (short_bs_count-1); cnt++)
		{
			for(int cnt1 = cnt+1; cnt1 < short_bs_count; cnt1++)
			{
				//int cnt1 = cnt+1;
				
				if(short_bs_group[cnt] != short_bs_group[cnt1])	continue;   
		
				sh_range = short_bs_range;
				sh_spec  = short_bs_spec;
				
				uint8_t skip_flag = false;
				for(int cnt2 = 0; cnt2 < short_ex_count; cnt2++)
				{
					if((short_bs_tp[cnt] == short_ex_tp1[cnt2]) && (short_bs_tp[cnt1] == short_ex_tp2[cnt2]) && (short_ex_range[cnt2] == 0xFF) )
					{
						if(add_flag == true)
						{
							sprintf(text,"Short Check(%d)",short_bs_group[cnt]);
							ListView_ITEM(text);
							add_flag = false;
						}
						sprintf(text, "SKIP(%s-%s)",tp_name[findTpName(short_bs_tp[cnt])], tp_name[findTpName(short_bs_tp[cnt1])]);
						ListView_MEAS(text);
						ListView_RESULT(SKIP_RESULT,STEP_STATE);
						add_flag = DISPLAY_MULTI_LINE;
						skip_flag = true;
						break;
					}
					else if((short_bs_tp[cnt] == short_ex_tp2[cnt2]) && (short_bs_tp[cnt1] == short_ex_tp1[cnt2]) && (short_ex_range[cnt2] == 0xFF) )
					{
						if(add_flag == true)
						{
							sprintf(text,"Short Check(%d)",short_bs_group[cnt]);
							ListView_ITEM(text);
							add_flag = false;
						}
						sprintf(text, "SKIP(%s-%s)",tp_name[findTpName(short_bs_tp[cnt])], tp_name[findTpName(short_bs_tp[cnt1])]);
						ListView_MEAS(text);
						ListView_RESULT(SKIP_RESULT,STEP_STATE);
						add_flag = DISPLAY_MULTI_LINE;
						skip_flag = true;
						break;
					}			
					else if((short_bs_tp[cnt] == short_ex_tp1[cnt2]) && (short_bs_tp[cnt1] == short_ex_tp2[cnt2]) && (short_ex_range[cnt2] != 0xFF) )
					{
						sh_range = short_ex_range[cnt2];
						sh_spec  = short_ex_spec[cnt2];
						break;
					}
					else if((short_bs_tp[cnt] == short_ex_tp2[cnt2]) && (short_bs_tp[cnt1] == short_ex_tp1[cnt2]) && (short_ex_range[cnt2] != 0xFF) )
					{
						sh_range = short_ex_range[cnt2];
						sh_spec  = short_ex_spec[cnt2];
						break;
					}		
				}
				if (skip_flag == true) continue;
				
				MUXPinOn(short_bs_tp[cnt], short_bs_tp[cnt1]);    Delay_us(100);		
				ModeSelect(sh_range);	                            Delay_us(100);		
				
				if(add_flag == true)
				{
					sprintf(text,"Short Check(%d)",short_bs_group[cnt]);
					ListView_ITEM(text);
					add_flag = false;
				}
			
				while(1)
				{
					value = Measure(sh_range);		
					test_meas_value[test_count-1] = value;	
				
					sprintf(text, "%s(%s-%s)",RESULT_SYB, tp_name[findTpName(short_bs_tp[cnt])], tp_name[findTpName(short_bs_tp[cnt1])]);
					ListView_MEAS(text);
		
					if(value >= sh_spec)		
					{
						ListView_RESULT(PASS_RESULT,STEP_STATE);
						key_ret = KeyScan();			
		
						if( (key_ret == 0x00) && (is_pc_connected_flag == true) )
						{
							if(test_state_flag != 0)
							{
								key_ret = test_state_flag;
								test_state_flag = 0;
							}
						}
						
						if(key_ret == ST1_ON)	     	{	add_flag = DISPLAY_MULTI_LINE;	break;			}
						if(key_ret == ST1_LON)		  {	add_flag = DISPLAY_MULTI_LINE;	break;			}
						else if(key_ret == ESC_ON)	{	add_flag = DISPLAY_MULTI_LINE;	return ESC_KEY;	}
					}
					else
					{
						ListView_RESULT(FAIL_RESULT,STEP_STATE);
						key_ret = KeyScan();
						
						if( (key_ret == 0x00) && (is_pc_connected_flag == true) )
						{
							if(test_state_flag != 0)
							{
								key_ret = test_state_flag;
								test_state_flag = 0;
							}
						}
						
						if(key_ret == ST1_ON)	    	{	add_flag = true;	break;			}
						else if(key_ret == ESC_ON)	{	add_flag = true;	return ESC_KEY;	}
					}
					ntDelay_ms(100);
				}
			}
		}	

	
	return ESC_KEY;
};

uint8_t Short_Insp_Auto(void)
{
	char text[256];  
	float 	 value = 0;			// 검사결과값 받아올 변수
	bool 	 add_flag  = true;	// listview 사용시 라인추가하기 위해 선언한 변수
	
	uint16_t ok_cnt = 0;
	uint16_t ng_cnt = 0;
	bool result = true;
	
	uint16_t sh_range = 0;
	float 	 sh_spec  = 0;	
	uint16_t sh_okcnt = 0;
	uint16_t sh_ngcnt = 0;
	
	
		dbgmsg(">> Short Test(Auto)_all\n");	

		int cur_cnt = 0;
		for(int cnt = 0; cnt < (short_bs_count-1); cnt++)
		{
			for(int cnt1 = cnt+1; cnt1 < short_bs_count; cnt1++)	//전수검사
			{
				//int cnt1 = cnt+1;				
				if(short_bs_group[cnt] != short_bs_group[cnt1])	continue;   
					
				sh_range = short_bs_range;
				sh_spec  = short_bs_spec;
				
				uint8_t skip_flag = false;
				for(int cnt2 = 0; cnt2 < short_ex_count; cnt2++)
				{
					if((short_bs_tp[cnt] == short_ex_tp1[cnt2]) && (short_bs_tp[cnt1] == short_ex_tp2[cnt2]) && (short_ex_range[cnt2] == 0xFF) )
					{
						skip_flag = true;
						break;
					}
					else if((short_bs_tp[cnt] == short_ex_tp2[cnt2]) && (short_bs_tp[cnt1] == short_ex_tp1[cnt2]) && (short_ex_range[cnt2] == 0xFF) )
					{
						skip_flag = true;
						break;
					}
					else if((short_bs_tp[cnt] == short_ex_tp1[cnt2]) && (short_bs_tp[cnt1] == short_ex_tp2[cnt2]) && (short_ex_range[cnt2] != 0xFF) )
					{
						sh_range = short_ex_range[cnt2];
						sh_spec  = short_ex_spec[cnt2];
						sh_okcnt = short_ex_ok_cnt[cnt2];
						sh_ngcnt = short_ex_ng_cnt[cnt2];
						break;
					}
					else if((short_bs_tp[cnt] == short_ex_tp2[cnt2]) && (short_bs_tp[cnt1] == short_ex_tp1[cnt2]) && (short_ex_range[cnt2] != 0xFF) )
					{
						sh_range = short_ex_range[cnt2];
						sh_spec  = short_ex_spec[cnt2];
						sh_okcnt = short_ex_ok_cnt[cnt2];
						sh_ngcnt = short_ex_ng_cnt[cnt2];
						break;
					}	
				}			
				if (skip_flag == true) continue;
	
				MUXPinOn(short_bs_tp[cnt], short_bs_tp[cnt1]);		//Delay_us(100);			
				ModeSelect(sh_range);							              	//Delay_us(100);	
						
				cur_cnt++;
	     		
				//if(Check_Range==0) {Delay_ms(80);}	
        //else               {}
					
				if(add_flag == true)
				{			
					ListView_ITEM("Short Check");
					ListView_RESULT(BUSY_RESULT,AUTO_STATE);
					//sprintf(text, "%d/%d",cur_cnt, open_count);
					//ListView_MEAS(text);
					add_flag = false;
					//ListView_RESULT(BUSY_RESULT);	
				}
				
				//if( cur_cnt % 25 == 0 )
				{				
					sprintf(text, "%d/%d",cnt, short_bs_count);
					//sprintf(text, "%d/%d",cur_cnt, short_tt_count);
					ListView_MEAS(text);
				}
				
				ok_cnt = ng_cnt = 0;
				//Delay_ms(80);
				
				while(1)
				{
					value = Measure(sh_range);
					
					test_meas_value[test_count-1] = value;
					
					if(value >= sh_spec)		
					{
						if(ok_cnt++ > sh_okcnt)
						{				
							ok_cnt = ng_cnt = 0;			
							//ListView_RESULT(PASS_RESULT);
							break;
						}					
					}
					else
					{
						if(ng_cnt++ > sh_ngcnt)
						{
							ok_cnt = ng_cnt = 0;
							result = false;
							sprintf(text, "%s(%s-%s)",RESULT_SYB, tp_name[findTpName(short_bs_tp[cnt])], tp_name[findTpName(short_bs_tp[cnt1])]);						
							ListView_MEAS(text);
							sprintf(short_disp, "%s", text);
							ListView_RESULT(FAIL_RESULT,AUTO_STATE);
							ntDelay_ms(100);
							if(RD_NG_CONTINUE == true)
							{	
								add_flag = true;	
								break;	
							}
							goto end_flow;
						}
						ntDelay_ms(100);
					}
					if(( ( KeyRead() >> 2 ) & 0x01 ) == 0)	
					{
						return ESC_KEY;		
					}
					else if( (is_pc_connected_flag == true) && test_state_flag == (ESC_ON) )
					{
						return ESC_KEY;	
					}
				}
			}
		}
		
	end_flow:
	if(result == true)
	{
		sprintf(short_disp, "OK");
		ListView_MEAS("OK");
		ListView_RESULT(PASS_RESULT,AUTO_STATE);
    }
	else 
	{
		ListView_RESULT(FAIL_RESULT,AUTO_STATE);
	}
	
	return result;
};


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
