#include "task_part.h"

uint8_t Part_test(uint8_t num, uint8_t st, uint8_t result)
{
	uint8_t ret = false;
	if(result == false)
	{
		if(RD_NG_CONTINUE == true)
		{
			Pt_str_idx = test_count;
			if(st == AUTO_STATE)
			{	
				pt_auto_result = ret = Part_Insp_Auto();	
				if(ret == ESC_KEY)
				{
					Pt_end_idx = test_count;
					result = ESC_KEY; 
					return ESC_KEY;
				}
			}
			else if(st == STEP_STATE)	
			{
				ret = Part_Insp_Step();	
			}
			Pt_end_idx = test_count;
			if(ret == false)	result = false;
		}
		else
		{
			return false;
		}
	}
	else if(result == true)
	{
		Pt_str_idx = test_count;					
		
		if(st == AUTO_STATE)
		{	
			pt_auto_result = ret = Part_Insp_Auto();	
			if(ret == ESC_KEY) 
			{
				Pt_end_idx = test_count;
				result = ESC_KEY; 
				return ESC_KEY;
			}
		}
		else if(st == STEP_STATE)
		{
			ret = Part_Insp_Step();	
		}
		
		Pt_end_idx = test_count;
		
		if(ret == false)	result = false;
	}	
   MuxInit();
  Delay_ms(100);
	return result;
}


uint8_t Part_Insp_Step(void) 
{
	dbgmsg(">> Part Test(Step)\n");
    double 	 value = 0;			// 검사결과값 받아올 변수
	uint8_t  key_ret = 0;
	
	for(int cnt = 0; cnt < part_count; cnt++)
    {
		MUXPinOn(part_tp1[cnt], part_tp2[cnt]);
		ModeSelect(part_range[cnt]); Delay_ms(10);
		
		ListView_ITEM(part_name[cnt]);
		while(1)
		{			
			value = Measure(part_range[cnt]);
			test_meas_value[test_count-1] = (value);
			ListView_MEAS(RESULT_SYB);
			
			if( (value <= part_spec_hi[cnt]) && (value >= part_spec_lo[cnt]) )
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
				
				if(key_ret == ST1_ON)		{	break;			}
				if(key_ret == ST1_LON)		{	break;			}
				else if(key_ret == ESC_ON)	{	return ESC_KEY;	}
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

				if(key_ret == ST1_ON)		{	break;			}
				else if(key_ret == ESC_ON)	{	return ESC_KEY;	}			
			}
			ntDelay_ms(100);
		}
	}
	return ESC_KEY;
}

uint8_t Part_Insp_Auto(void) 
{
	dbgmsg(">> Part Test(Auto)\n");	
	
	char text[256];  
    double 	 value = 0;			// 검사결과값 받아올 변수
	uint16_t ok_cnt = 0;
	uint16_t ng_cnt = 0;
	bool result = true;		
	
	for(int cnt = 0; cnt < part_count; cnt++)
	{
		MUXPinOn(part_tp1[cnt], part_tp2[cnt]);	//Delay_us(100);
		ModeSelect(part_range[cnt]);			//Delay_us(500);

		ListView_ITEM(part_name[cnt]);
		
		ok_cnt = ng_cnt = 0;
		while(1)
		{
			value = Measure(part_range[cnt]);
			test_meas_value[test_count-1] = (value);
			sprintf(test_meas_string[test_count-1],"%s",RESULT_SYB);
						
			if( (value <= part_spec_hi[cnt]) && (value >= part_spec_lo[cnt]) )
			{
				if(ok_cnt++ > part_ok_cnt[cnt])
				{
					ok_cnt = ng_cnt = 0;
					ListView_MEAS(RESULT_SYB);
					ListView_RESULT(PASS_RESULT,AUTO_STATE);
					break;
				}
			}
			else
			{
				if(ng_cnt++ > part_ng_cnt[cnt])
				{
					ok_cnt = ng_cnt = 0;					
					result = false;
										
					sprintf(text,"%.2f<(%s)<%.2f",part_spec_lo[cnt],RESULT_SYB,part_spec_hi[cnt]);		
					ListView_MEAS(text);
					
					ListView_RESULT(FAIL_RESULT,AUTO_STATE);	
					ntDelay_ms(100);
					if(RD_NG_CONTINUE == true)
					{	
						break;	
					}
					return result;
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
	return result;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
