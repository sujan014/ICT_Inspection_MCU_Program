#include "task_open.h"

#define DISPLAY_MULTI_LINE false

uint8_t Open_test(uint8_t num, uint8_t st, uint8_t result)
{		
	uint8_t ret = false;
	if(result == false)
	{
		if(RD_NG_CONTINUE == true)
		{
			Op_str_idx = test_count;
			if(st == AUTO_STATE)
			{	
				op_auto_result = ret = Open_Insp_Auto();	
				if(ret == ESC_KEY)
				{
					Op_end_idx = test_count;
					result = ESC_KEY; 
					return ESC_KEY;
				}
			}
			else if(st == STEP_STATE)	
			{
				ret = Open_Insp_Step();	
			}
			Op_end_idx = test_count;
			if(ret == false)	result = false;
		}
		else
		{
			return false;
		}
	}
	else if(result == true)
	{
		Op_str_idx = test_count;					
		
		if(st == AUTO_STATE)
		{	
			op_auto_result = ret = Open_Insp_Auto();	
			if(ret == ESC_KEY) 
			{
				Op_end_idx = test_count;
				result = ESC_KEY; 
				return ESC_KEY;
			}
		}
		else if(st == STEP_STATE)
		{
			ret = Open_Insp_Step();	
		}
		
		Op_end_idx = test_count;
		
		if(ret == false)	result = false;
	}	
	return result;
}




// Open 검사 함수
uint8_t Open_Insp_Step(void) 
{		
	dbgmsg(">> Open Test(Step)\n");	
	float 	 value = 0;			// 검사결과값 받아올 변수
    char  	 text[100];			// Display 용 배열
    bool 	 add_flag  = true;	// listview 사용시 라인추가하기 위해 선언한 변수

	uint8_t  key_ret = 0;
	
	for(int cnt = 0; cnt < open_count; cnt++)
    {
		MUXPinOn(open_tp1[cnt], open_tp2[cnt]);	Delay_us(100);
		ModeSelect(open_range[cnt]);	Delay_ms(10);
		
		if(add_flag == true)
		{
			ListView_ITEM("Open Check");
			add_flag = false;
		}
		
		while(1)
		{
			value = Measure(open_range[cnt]);
			test_meas_value[test_count-1] = value;
			
			sprintf(text, "%s(%s-%s)",RESULT_SYB, tp_name[findTpName(open_tp1[cnt])], tp_name[findTpName(open_tp2[cnt])]);
			ListView_MEAS(text);
						
			if(value <= open_spec[cnt])
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
				
				if(key_ret == ST1_ON)		{	add_flag = DISPLAY_MULTI_LINE;	break;			}
				else if(key_ret == ST1_LON)	{	add_flag = DISPLAY_MULTI_LINE;	break;			}
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
					
				if(key_ret == ST1_ON)		{	add_flag = true;	break;			}
				else if(key_ret == ESC_ON)	{	add_flag = true;	return ESC_KEY;	}
			}
			ntDelay_ms(100);
		}
	}	
	return ESC_KEY;
}

uint8_t Open_Insp_Auto(void) 
{		
	dbgmsg(">> Open Test(Auto)\n");
	
    float 	 value = 0;			// 검사결과값 받아올 변수
    char  	 text[100];			// Display 용 배열
    bool 	 add_flag  = true;	// listview 사용시 라인추가하기 위해 선언한 변수

	uint16_t ok_cnt = 0;
	uint16_t ng_cnt = 0;
	bool result = true;
		
	int cur_cnt = 0;
	for(int cnt = 0; cnt < open_count; cnt++)
	{    
		MUXPinOn(open_tp1[cnt], open_tp2[cnt]);	//Delay_us(100);		
		ModeSelect(open_range[cnt]);	//Delay_us(500);	

		cur_cnt++;
		if (add_flag == true)
		{
			ListView_ITEM("Open Check");
			ListView_RESULT(BUSY_RESULT,AUTO_STATE);
			add_flag = false;
		}		
		if( cur_cnt % 25 == 0 )
		{				
			sprintf(text, "%d/%d",cur_cnt, open_count);
			ListView_MEAS(text);
		}
    
		ok_cnt = ng_cnt = 0;
		while(true)
		{
			value = Measure(open_range[cnt]);
			
			test_meas_value[test_count-1] = value;
			if(value <= open_spec[cnt])		
			{
				if(ok_cnt++ > open_ok_cnt[cnt])
				{
					ok_cnt = ng_cnt = 0;
					break;
				}
			}
			else			
			{
				if(ng_cnt++ > open_ng_cnt[cnt])
				{
					ok_cnt = ng_cnt = 0;
					result = false;
					sprintf(text, "%s(%s-%s)",RESULT_SYB, tp_name[findTpName(open_tp1[cnt])], tp_name[findTpName(open_tp2[cnt])]);  
					ListView_MEAS(text);
					sprintf(open_disp, "%s", text);
					ListView_RESULT(FAIL_RESULT,AUTO_STATE);	
					ntDelay_ms(100);

					if(RD_NG_CONTINUE == true)
					{	
						add_flag = true;	
						break;	
					}
					goto end_flow;
				}
				ntDelay_ms(1);
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

	end_flow:
	if(result == true)
	{
		sprintf(open_disp, "OK");
		ListView_MEAS("OK");
		ListView_RESULT(PASS_RESULT,AUTO_STATE);
    }
	else 
	{
		ListView_RESULT(FAIL_RESULT,AUTO_STATE);
	}
	
	return result;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
