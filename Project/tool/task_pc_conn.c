#include "task_pc_conn.h"

char test_state_flag = false;
char is_pc_connected_flag = false;
uint8_t MFS_STATE = 0;

void IsPcCheck(void)
{		
	static int cnt = 1000;
	if(is_pc_connected_flag == true)	// PC 검사기
	{
		if(cnt++ > 1000) // main 5ms * 1000 = 5000
		{
			CustomStrCat(log_txbuff, "NG,");		
			if(RD_DBG_MD == 0)	{	CustomStrCat(NULL, "KM-V%1.2f(D),",sys_ver);	}
			else				{	CustomStrCat(NULL, "KM-V%1.2f(R),",sys_ver);	}
			CustomStrCat(NULL, "%08X,",serial_num);
			CustomStrCat(NULL, "%08X,",RD_BOOT);
			CustomStrCat(NULL, "%d,",RD_DBG_MD);

			tx_pc_data(CONN_CHECK, strlen(log_txbuff), log_txbuff);

			cnt = 0;
		}
	}	
}
void log_save_pc(uint8_t flag)
{
	if(is_pc_connected_flag == false) return;
	
	char data[1];	
	data[0] = flag;	
	tx_pc_data(SUMMARY_LOG, 1, data);
}

void info_state_pc(uint8_t *buf)
{
	if(is_pc_connected_flag == false) return;
	
	char data[1];	
	data[0] = 0;	
	tx_pc_data(INFO_STATE, 0, data);
}
void start_flow_pc(void)
{	
	if(is_pc_connected_flag == false) return;
	
	char data[1];	
	data[0] = 0;	
	tx_pc_data(START_STATE, 0, data);
}
void end_flow_pc(uint16_t cnt, uint8_t flag)
{
	if(is_pc_connected_flag == false) return;
	
	CustomStrCat(log_txbuff, "%d,",cnt);		//2
	CustomStrCat(NULL, "%d,",flag);		//2
	tx_pc_data(END_STATE, strlen(log_txbuff), log_txbuff);
}

void test_index_pc(uint8_t state, uint16_t str_idx, uint16_t end_idx, uint8_t result,uint8_t allresult)
{
	if(is_pc_connected_flag == false) return;
	
	CustomStrCat(log_txbuff, "%d,",str_idx);		//2
	CustomStrCat(NULL, "%d,",end_idx);		//2
	CustomStrCat(NULL, "%d,",result);		//2
	CustomStrCat(NULL, "%d,",allresult);		//2
	tx_pc_data(state, strlen(log_txbuff), log_txbuff);
}

void conn_check_pc(char *data)
{
	if(data[0] == 1)
	{
		hItem = WM_GetDialogItem(hStatewin, STATE_PC);  
		TEXT_SetText(hItem,"PC(Connect)"); 
		TEXT_SetBkColor(hItem, GUI_GREEN);
		TEXT_SetTextColor(hItem, GUI_BLACK);
		GUI_Exec(); 
		
		CustomStrCat(log_txbuff, "OK,");		//2
		if(RD_DBG_MD == 0)	{	CustomStrCat(NULL, "KM-V%1.2f(D),",sys_ver);	}
		else				{	CustomStrCat(NULL, "KM-V%1.2f(R),",sys_ver);	}
		CustomStrCat(NULL, "%08X,",serial_num);		//2
		CustomStrCat(NULL, "%08X,",RD_BOOT);
		CustomStrCat(NULL, "%d,",RD_DBG_MD);
		tx_pc_data(CONN_CHECK, strlen(log_txbuff), log_txbuff);
		
		is_pc_connected_flag = true;
	}
	else if(data[0] == 0)
	{
		is_pc_connected_flag = false;
		
		hItem = WM_GetDialogItem(hStatewin, STATE_PC);  
		TEXT_SetText(hItem,"PC(Disconnect)"); 
		TEXT_SetBkColor(hItem, GUI_BLACK);
		TEXT_SetTextColor(hItem, GUI_WHITE);
		GUI_Exec();
	}		
	Beep(10);
}
void mode_change_pc(uint8_t mode)
{
	InspModeChange(mode);	

	char data[1];
	
	data[0] = RD_MODE;
	
	tx_pc_data(MODE_CHANGE, 1, data);
}
void key_state_pc(char *key)
{
	if(key[0] == (ST1_ON|ST2_ON) )	
	{
		test_state_flag = (ST1_ON|ST2_ON);
		
		WR_NG_CONTINUE(key[1]);
		WR_FW_WRITE(key[2]);
		WR_SDC_SAVE(key[3]);
		WR_LOG_UNIT(key[4]);
		WR_SUB_DISP(key[5]);
		WR_SHORT_ALL(key[6]);
		WR_MODE(key[7]);
		WR_FULL_WRITE(key[8]);
	}
	else if( key[0] == (ESC_ON) )	
	{
		test_state_flag = (ESC_ON);
	}
	else if(key[0] == (ST1_ON) )	
	{
		test_state_flag = (ST1_ON);
	}
	else if(key[0] == (ST2_ON) )	
	{
		test_state_flag = (ST2_ON);
	}
	else if(key[0] == (MOD_ON) )	
	{
		test_state_flag = (MOD_ON);
	}
}
void key_value_pc(uint8_t value)
{
	char data[1];
	
	data[0] = value;
	
	tx_pc_data(KEY_VALUE, 1, data);
	
	ntDelay_ms(50);
}
void safe_value_pc(uint8_t value)
{
	char data[1];
	
	data[0] = value;
		
	tx_pc_data(SAFE_VALUE, 1, data);
	
	ntDelay_ms(50);
}
void sen_value_pc(uint8_t value)
{
	char data[1];
	
	data[0] = value;

	tx_pc_data(SEN_VALUE, 1, data);
	
	ntDelay_ms(50);
}
void qty_value_pc(uint8_t value)
{
	char data[1];
	data[0] = value;
	tx_pc_data(QTY_VALUE, 1, data);
}
void sol_cont_pc(char *buf)
{
	uint8_t  no    = 0;
	uint8_t  state = 0;
	uint16_t time  = 0;
	
	no 	  = buf[0];
	state = buf[1];
	time  = (buf[2]<<8) + buf[3];
	buf[0]=CylinderControl(no,state,time);
	tx_pc_data(SOL_RUN, 1, buf);
}
void okngled_cont_pc(char *buf)
{
	uint8_t  ret   = 0;
	uint8_t  no    = 0;
	uint8_t  state = 0;
	
	no 	  = buf[0];
	state = buf[1];
	if(no == 1)			{	ret = OKLED(state) + 1;				}
	else if(no == 2)	{	ret = NGLED(state) + 1;				}
	else if(no == 0xff)	{	ret = OKLED(state) + NGLED(state);	}
	
	if(ret == 2)	buf[0] = true;
	else 			buf[0] = false;
	tx_pc_data(STATE_LED, 1, buf);
}

void func_rly_cont_pc(char *buf)
{
	uint8_t  no    = 0;
	uint8_t  state = 0;
	uint16_t time  = 0;
	
	no 	  = buf[0];
	state = buf[1];
	time  = (buf[2]<<8) + buf[3];
	buf[0]=FuncRYControl(no,state,time);
	tx_pc_data(RELAY, 1, buf);
}
void run_mode_change(char value)
{
	if(value == 0)
	{
		Beep(500);
		WR_BOOT(APP_FLAG);
		NVIC_SystemReset();
		dbgmsg("go app\n");
	}
	else if(value == 1)
	{
		Beep(500);
		WR_BOOT(BOOT_FLAG);
		NVIC_SystemReset();
		dbgmsg("go boot\n");
	}
}

void debug_mode_change()
{
	if(RD_DBG_MD == 0) 		{	WR_DBG_MD(1);}
	else if(RD_DBG_MD == 1) {	WR_DBG_MD(0);}

	Beep(500);				
	WR_BOOT(APP_FLAG);
	NVIC_SystemReset();
}

void led_setting(char *buf)
{	
	WR_LED_CH1(buf[0]);
	WR_LED_CH2(buf[1]);
	WR_LED_CH3(buf[2]);
	WR_LED_CH4(buf[3]);	
		
	uint8_t ret = 0;
	ret += olb_led_control(1,RD_LED_CH1);		ntDelay_ms(50);
	ret += olb_led_control(2,RD_LED_CH2);		ntDelay_ms(50);
	ret += olb_led_control(3,RD_LED_CH3);		ntDelay_ms(50);
	ret += olb_led_control(4,RD_LED_CH4);		ntDelay_ms(50);
		
	if(ret == 4)	buf[0] = true;
	else 			buf[0] = false;

	tx_pc_data(LED_VALUE, 1, buf);
	
	dbgmsg("led_setting(%d,%d,%d,%d)\n",RD_LED_CH1,RD_LED_CH2,RD_LED_CH3,RD_LED_CH4); 
}

void paring_data(uint8_t nType, int length, char *data)
{
		//	dbgmsg("rx nType(%d)\n",nType); 
	switch(nType)
	{
		////////////////////
		case CONN_CHECK:	conn_check_pc(data);		break;		
		case RUN_MODE:		run_mode_change(data[0]);	break;		
		case DEBUG_MODE:	debug_mode_change();		break;		
		
		////////////////////
		case KEY_STATE:		key_state_pc(data);			break;
		case KEY_VALUE:									break;
		case MODE_CHANGE:	mode_change_pc(data[0]);	break;
		case SAFE_VALUE:								break;
		case QTY_VALUE:									break;
		case POPUP_DISP:								break;	
		case POPUP_RESULT:								break;	
		case STOP_WATCH:								break;	
		case INFO_STATE:								break;		
		
		case LED_VALUE:		led_setting(data);			break;		
		
		////////////////////			
		case SOL_RUN:		sol_cont_pc(data);						break;		
		case STATE_LED:		okngled_cont_pc(data);					break;			
		case RELAY:			func_rly_cont_pc(data);					break;			
		////////////////////	
		////////////////////	
		////////////////////	
		case ICT_SPEC_ERASE:	ictspec_erase_pc();					break;	//	100
		case ICT_PROJECT_WR:	Info_Save_PC(data);					break;  //  101
		case ICT_TESTPOINT_WR:	Tp_Save_PC(data);					break;  //  102
		case ICT_OPEN_WR:		Open_Save_PC(data);					break;  //  103
		case ICT_SHORTBS_WR:	Short_bs_Save_PC(data);				break;  //  104
		case ICT_SHORTEX_WR:	Short_ex_save_PC(data);				break;  //  105
		case ICT_PART_WR:		Part_Save_PC(data);					break;  //  106
		case ICT_FUNC_WR:		Func_Save_PC(data);					break;  //  107
		case OP_MODE1_WR:		mode_Save1_PC(data);				break;  //  108
		case OP_MODE2_WR:		mode_Save2_PC(data);				break;  //  109
		case OP_MODE3_WR:		mode_Save3_PC(data);				break;  //  110
		case OP_MODE4_WR:		mode_Save4_PC(data);				break;  //  111
		case OP_INIT_WR:		init_Save_PC(data);					break;	//  112
		case ICT_SPEC_FINSH:	Spec_Make_PC(data);		Beep(5);	break;	//	113
		case ICT_PROJECT_RD:	Info_Send_PC();						break;	//	115
		////////////////////	

		case PANEL_DATA_ERASE:	PanelErase_Pc();					break;	//  126
		case PANEL_DATA_WR:		Panel_Save_PC(data);				break;	//  126
		case PANEL_DATA_FINSH:	PanelMake_Pc(data);		Beep(5);	break;	//  126
		case PANEL_DATA_RD:		Panel_Send_PC();		Beep(5);	break;	//  126
										
		case INPUT_DATA_ERASE:	data_erase_pc();					break;	//  126
		case INPUT_DATA_WR:		data_write_pc(data);				break;	//  126
		case INPUT_DATA_FINSH:	DataMake_PC(data);		Beep(5);	break;	//  126
		case INPUT_DATA_RD:		data_send_pc();			Beep(5);	break;	//  126
		//------------------------------------------------------------------------
		case PMIC_DATA_ERASE : 	pmic_erase_pc();					break;  //  145
		case PMIC_DATA_WR:		pmic_write_pc(data);				break;	//  146
		case PMIC_DATA_FINSH:	PmicMake_PC(data);		Beep(5);	break;	//  147
		case PMIC_DATA_RD:		pmic_send_pc();			Beep(5);	break;	//  148
		
		case EEP_DATA_ERASE : 	eep_erase_pc();						break;  //  185
		case EEP_DATA_WR:		eep_write_pc(data);					break;	//  186
		case EEP_DATA_FINSH:	EepMake_PC(data);		Beep(5);	break;	//  187
		case EEP_DATA_RD:		eep_send_pc();			Beep(5);	break;	//  188

		case TPIC_DATA_ERASE : 	tpic_erase_pc();					break;  //  143
		case TPIC_DATA_WR:		tpic_write_pc(data);				break;	//  127
		case TPIC_DATA_FINSH:	TpicMake_PC(data);		Beep(5);	break;	//  128
		case TPIC_DATA_RD:		tpic_send_pc();			Beep(5);	break;	//  129

		//-------------------------------------------------------------------------
		case MFS_ING :		MFS_STATE = 	1;				break;	//  220
		case MFS_PASS :		MFS_STATE =  2;		break;	//  221
		case MFS_FAIL:		MFS_STATE =	3;		break;	//  223		
	}                                                                    
}

void MFS_STATE_CLEAR()
{
	MFS_STATE =0;
}
uint32_t 	rx_pc_cnt = 0;
uint8_t 	rx_nType = 0;
uint16_t 	rx_rength = 0;
uint16_t 	checksum_cal = 0;
uint16_t 	checksum_rx  = 0;
void rx_pc_data(void)
{	
	//if(pc_rx_enable == false)	return;		
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		pc_rxbuff[rx_pc_cnt++] = (uint8_t)USART_ReceiveData(USART1);		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	
	if(pc_rxbuff[0] != 'S') rx_pc_cnt = rx_rength = 0;
	
	if(rx_pc_cnt >= 6)
	{
		if(((pc_rxbuff[0]) == 'S') && (pc_rxbuff[1] == 'T') && (pc_rxbuff[2] == 'X') && (pc_rxbuff[3] == ',') )	//STX,
		{
			rx_nType  = pc_rxbuff[4];
			rx_rength = ( ((uint16_t)pc_rxbuff[5] << 8 ) + (uint16_t)pc_rxbuff[6] ) ;
		}
		else		// 정상데이터 수신이 아닐시 버퍼 클리어		
		{	
			//dbgmsg("%c%c%c%c\n",pc_rxbuff[0],pc_rxbuff[1],pc_rxbuff[2],pc_rxbuff[3]);
			rx_pc_cnt = rx_rength = 0;			
			/*	USART_Cmd(USART1, DISABLE);	// DISABLE USART
	      USART_ITConfig(USART1, USART_IT_RXNE, DISABLE );	// DISABLE Receive Interrupt	
				rx_pc_cnt = rx_rength = 0;
				USART_Cmd(USART1, ENABLE);	// ENABLE USART
	      USART_ITConfig(USART1, USART_IT_RXNE, ENABLE );	// ENABLE Receive Interrupt	
			*/
		}
	}
	
	if( rx_pc_cnt >= 13 + rx_rength )	
	{	
		checksum_cal = checksum_rx = 0;
		for (int cnt = 0; cnt < rx_rength; cnt++)
		{
			valid_rxbuff[cnt] = pc_rxbuff[cnt + 7];
			checksum_cal += (uint16_t)valid_rxbuff[cnt];
		}
		
		checksum_cal &= 0xFFFF;
		checksum_rx = ( ((uint16_t)pc_rxbuff[7 + rx_rength] << 8 ) + (uint16_t)pc_rxbuff[7 + rx_rength+1] ) &0xFFFF;
		
		if(checksum_cal == checksum_rx)
		{
			paring_data(rx_nType, rx_rength, valid_rxbuff);
		}
		else
		{
			dbgmsg("checksum ng\n");
		}
		rx_pc_cnt = 0;
	}
	if( rx_pc_cnt >= (128*1024) )   rx_pc_cnt = 0;
}

void tx_pc_data(uint8_t nType, int length, char *data)
{
	uint32_t Checksum_cal = 0;
	
	pc_txbuff[0] = (uint8_t)'S'; 		// stx1
	pc_txbuff[1] = (uint8_t)'T'; 		// stx2
	pc_txbuff[2] = (uint8_t)'X'; 		// stx3
	pc_txbuff[3] = (uint8_t)',';  	// ,
	pc_txbuff[4] = (uint8_t)nType;	// nType
	pc_txbuff[5] = (uint8_t)( (length >> 8) & 0xff );  // length high
	pc_txbuff[6] = (uint8_t)( (length >> 0) & 0xff );  // length low
	for (int cnt = 0; cnt < length; cnt++)
	{
		pc_txbuff[7 + cnt] = (uint8_t)data[cnt]; 	//data
		Checksum_cal += (uint8_t)data[cnt];
	}
	Checksum_cal = Checksum_cal & 0xFFFF;
	pc_txbuff[7 + length + 0] = (uint8_t)( (Checksum_cal >> 8) & 0xff );  // length high
	pc_txbuff[7 + length + 1] = (uint8_t)( (Checksum_cal >> 0) & 0xff );  // length low
	pc_txbuff[7 + length + 2] = (uint8_t)',';   // ,
	pc_txbuff[7 + length + 3] = (uint8_t)'E';	// etx 1
	pc_txbuff[7 + length + 4] = (uint8_t)'T';	// etx 2
	pc_txbuff[7 + length + 5] = (uint8_t)'X';	// etx 3
	
	for (int cnt = 0; cnt < length + 13; cnt++)
	{	
		if(cnt <= 3)					txpc("%c",pc_txbuff[cnt]);
		else if(cnt >= 7 + length + 2)	txpc("%c",pc_txbuff[cnt]);
		else							txpc("%02X",pc_txbuff[cnt]);
	}
	
	//dbgmsg(" - nType: %02X\n - length:%d\n",nType,length);
	//for (int cnt = 0; cnt < length + 13; cnt++)
	//{	
	//	if(cnt <= 3)					dbgmsg("%c",pc_txbuff[cnt]);
	//	else if(cnt >= 7 + length + 2)	dbgmsg("%c",pc_txbuff[cnt]);
	//	else							dbgmsg("%02X",pc_txbuff[cnt]);
	//}
	//dbgmsg("\n");
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
