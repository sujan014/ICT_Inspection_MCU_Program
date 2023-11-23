#include "task_func.h"

uint8_t Func_test(uint8_t num, uint8_t st, uint8_t result)
{

	uint8_t ret = false;
	if(result == false)
	{
		if(RD_NG_CONTINUE == true)
		{
			Fn_str_idx = test_count;
			if(st == AUTO_STATE)
			{	
				fn_auto_result = ret = Func_Insp_Auto();	
				if(ret == ESC_KEY)
				{
					Fn_end_idx = test_count;
					result = ESC_KEY; 
					return ESC_KEY;
				}
			}
			else if(st == STEP_STATE)	
			{
				ret = Func_Insp_Step();	
			}
			Fn_end_idx = test_count;
			if(ret == false)	result = false;
		}
		else
		{
			return false;
		}
	}
	else if(result == true)
	{
		Fn_str_idx = test_count;					
		
		if(st == AUTO_STATE)
		{	
			fn_auto_result = ret = Func_Insp_Auto();	
			if(ret == ESC_KEY) 
			{
				Fn_end_idx = test_count;
				result = ESC_KEY; 
				return ESC_KEY;
			}
		}
		else if(st == STEP_STATE)
		{
			ret = Func_Insp_Step();	
		}
		
		Fn_end_idx = test_count;
		
		if(ret == false)	result = false;
	}	
	return result;
}


uint8_t Func_Insp_Step(void)
{
	dbgmsg(">> Func_Insp(Step)\n");

	uint8_t key_ret = 0;
	uint8_t result = true;
	
	for(int cnt = 0; cnt < func_count; cnt++)
	{		
		ListView_ITEM(func_name[cnt]);
		
		retry_flow:
		
		result = Func_Test(cnt, func_type[cnt]);
		if(result == true)			ListView_RESULT(PASS_RESULT,STEP_STATE);
		else if(result == false)	ListView_RESULT(FAIL_RESULT,STEP_STATE);
		else 						ListView_RESULT(SKIP_RESULT,STEP_STATE);
		
		while(1)
		{
			Delay_ms(5);
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
			else if(key_ret == MOD_ON)	{	goto retry_flow;}
			else if(key_ret == ESC_ON)	{	return result;	}
		}
	}	
	return result;
}

uint8_t Func_Insp_Auto(void)
{
	dbgmsg(">> Func_Insp(Auto)\n");
	
	uint8_t result = true;
	uint8_t ret = true;
		
	for(int cnt = 0; cnt < func_count; cnt++)
	{
		ListView_ITEM(func_name[cnt]);
			
		ret = Func_Test(cnt,func_type[cnt]);
		
		if(is_pc_connected_flag == true)	
		{
			if(func_panel_link[cnt] != 0)
			{				
				uint16_t panel_cnt = func_panel_link[cnt] - 1;		
				uint16_t panel_float = (uint16_t)func_spec_float[cnt];
				//////////////////////////////////////////////
				uint16_t tx_num = panel_tx_num[panel_cnt];
				uint16_t rx_num = panel_rx_num[panel_cnt];
				uint16_t err_num = ErrorNodeCount[panel_cnt];

				CustomStrCat(log_txbuff,"%d,", panel_cnt);
				CustomStrCat(NULL,"%d,",tx_num);	
				CustomStrCat(NULL,"%d,",rx_num);	
				CustomStrCat(NULL,"%d,",err_num);	
				CustomStrCat(NULL,"%d,",panel_float);	
				
				dbgmsg("Panel Info:%s\n",log_txbuff);
								
				uint32_t node = 0;
				for(int i=0; i < tx_num ; i++)
				{
					for(int j=0; j < rx_num ; j++)
					{
						if(panel_float == 0)	CustomStrCat(NULL,"%d,", panel_data[panel_cnt][node]);
						else 					CustomStrCat(NULL,"%.3f,", panel_dataf[panel_cnt][node]);
						node++;
					}
				}
				tx_pc_data(PANLE_DATA, strlen(log_txbuff), log_txbuff);				
			}
		}
		
		if(ret == true)
		{
			ListView_RESULT(PASS_RESULT,AUTO_STATE);
		}
		else if(ret == false)
		{
			result = ret;
			ListView_RESULT(FAIL_RESULT,AUTO_STATE);
			if(RD_NG_CONTINUE == false)	return result;
		}
		else
		{
			ListView_RESULT(SKIP_RESULT,AUTO_STATE);
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
	return result;
}

uint8_t Func_Test(uint16_t cnt, uint16_t type)
{	
	uint8_t ret;
	switch(type)
	{
		case LSI:					
			#ifdef LSI_IC
				ret = LsiModule(cnt);	
			#else
				ListView_MEAS("Undefine Module");
				dbgmsg("Un Define\n");
				ret = FAIL_RESULT;
			#endif 
		break;
		case STM: 	
			#ifdef STM_IC	
			#else			
				ListView_MEAS("Undefine Module");
				dbgmsg("UnDefine\n");
				ret = FAIL_RESULT;
			#endif 
		break;
		case SYNA: 	
			#ifdef SYNA_IC	
			#else			
				ListView_MEAS("Undefine Module");
				dbgmsg("UnDefine\n");
				ret = FAIL_RESULT;
			#endif 
		break;
		case ATMEL: 	
			#ifdef ATMEL_IC	
			#else			
				ListView_MEAS("Undefine Module");	
				dbgmsg("UnDefine\n");
				ret = FAIL_RESULT;
			#endif 
		break;
		case SiW: 	
			#ifdef SiW_IC	
			#else
				ListView_MEAS("Undefine Module");
				dbgmsg("UnDefine\n");
				ret = FAIL_RESULT;
			#endif 
		break;
		case MELFAS: 	
			#ifdef MELFAS_IC	
			#else
				ListView_MEAS("UnDefine Module");	
				dbgmsg("UnDefine\n");
				ret = FAIL_RESULT;
			#endif 
		break;
		case SPIFLASH: 	
			#ifdef SPIFLASH_IC	
			#else
				ListView_MEAS("Undefine Module");
				dbgmsg("UnDefine\n");
				ret = FAIL_RESULT;
			#endif
		break;
		//-----------------------------------------------		
		case MFS:
			#ifdef MFF_ISC
				ret = MELFAS_ISC(cnt);	
			#else
				ListView_MEAS("Undefine Module");
				dbgmsg("Un Define\n");
				ret = FAIL_RESULT;
			#endif 
		break;
		//------------------------------------------------
		case PMIC:
			ret = PMIC_Function(cnt);	
			break;
		case EEPROM:
			ret = EEP_Function(cnt);	
			break;
		case TPIC:
			ret = TPIC_Function(cnt);	
			break;
		//------------------------------------------------
		case LXS:
			#ifdef LXS_IC
				ret = LXS_Module(cnt);
			#else
				ListView_MEAS("Undefine Module");
				dbgmsg("UnDefine LXS\n");
				ret = SKIP_RESULT;
			#endif
		break;		
		//------------------------------------------------
		default :		
			ListView_MEAS("Undefine Module");
			dbgmsg("UnDefine\n");
			ret = FAIL_RESULT;
		break;
	}
	return ret;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
