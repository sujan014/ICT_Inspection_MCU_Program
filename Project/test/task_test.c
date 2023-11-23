#include "task_test.h"

char open_disp[100];
char short_disp[100];

uint16_t Op_str_idx;
uint16_t Op_end_idx;
uint16_t Sh_str_idx;
uint16_t Sh_end_idx;
uint16_t Pt_str_idx;
uint16_t Pt_end_idx;
uint16_t Fn_str_idx;
uint16_t Fn_end_idx;

uint8_t op_auto_result = true;
uint8_t sh_auto_result = true;
uint8_t pt_auto_result = true;
uint8_t fn_auto_result = true;

uint8_t test_result_flag = false;
uint8_t test_finsh_flag = false;
uint8_t list_disp_flag = false;
int16_t result_total_Page = false;
int16_t result_disp_Page = false;

int16_t Result_Total_Page = 0, Result_Display_Page = 0;
int16_t NG_Result_Total_Page = 0, NG_Result_Display_Page = 0;
int16_t OK_Result_Total_Page = 0, OK_Result_Display_Page = 0;

uint8_t ListViewMode = 0;

uint8_t AutoRunMode = false;

uint8_t Check_Short_auto(void)
{	
	static int check_cnt = 0;

	uint16_t p_pin = short_auto_pp;
	uint16_t n_pin = short_auto_mm;

	if( (p_pin == 0) && (n_pin == 0))
	{
		return 0x00;
	}
	
	if (AutoRunMode == false)
	{
		MUXPinOn(p_pin,n_pin);		Delay_us(100);  
		ModeSelect(0);				Delay_us(100);   
		AutoRunMode = true;
	}

	uint32_t R_dat = 0;
	double res_value =0;
		
	R_dat = Measure_adc_value(10,255);   
	res_value = (((double)R_dat/16.384));  
	res_value = res_value - mux_res;

	if(res_value >= 200) 	res_value = 200;
	if(res_value <= 0)   	res_value = 0;
	
	if(res_value <= 150)	check_cnt++;
	else					check_cnt = 0;

	if(check_cnt > 2)	return ATS_ON;
	else				return 0x00;
}

void TestStart(void)
{   
  test_count = 0;	
  AutoRunMode = false;
  //-------------------------------230128 Sujan
	
	Delay_ms(500);
	AVDD_ON(false);
	DVDD_ON(false);
	RYBD_6ON(false);
	EvccControl(1,false);
	EvccControl(2,false);
	FuncRYControl(255,false,100);
	RYBD_1ON(false);
	RYBD_2ON(false);
	RYBD_3ON(false);
	RYBD_4ON(false);
	RYBD_5ON(false);
   
  //---------------------------------
	OKLED(false);NGLED(false);
	if(RD_MODE == 0)		operation(mode1_sequence, mode1_operation, mode1_number, mode1_status, mode1_time);
	else if(RD_MODE == 1) 	operation(mode2_sequence, mode2_operation, mode2_number, mode2_status, mode2_time);
	else if(RD_MODE == 2) 	operation(mode3_sequence, mode3_operation, mode3_number, mode3_status, mode3_time);
	else if(RD_MODE == 3) 	operation(mode4_sequence, mode4_operation, mode4_number, mode4_status, mode4_time);

  dbgmsg(">> End Test Count :  %d\n", test_count);
	
  EvccControl(1,false);
  EvccControl(2,false);  
//FuncRYControl(255,false,100); // Relay All Off
  FuncRYControl(1,true,0);
  FuncRYControl(2,true,0);
  FuncRYControl(3,true,0);  
  FuncRYControl(4,true,800);  //discharge
  
  FuncRYControl(1,false,0);
  FuncRYControl(2,false,0);
  FuncRYControl(3,false,0);
  FuncRYControl(5,false,0);
  FuncRYControl(4,false,0); 
  FuncRYControl(6,false,0);
  
	AVDD_ON(false);
	Delay_ms(100);
	DVDD_ON(false);
	Delay_ms(100);
	RYBD_6ON(false);
	Delay_ms(100);
	RYBD_1ON(false);
	Delay_ms(100);
	RYBD_2ON(false);
	Delay_ms(100);
	RYBD_3ON(false);
	Delay_ms(100);
	RYBD_4ON(false);
	Delay_ms(100);
	RYBD_5ON(false);
	Delay_ms(100);
	
	
  //Extend_VDD_RY(false);//VUSB_5V,VIN 12V relay_OFF
  Delay_ms(300);   
  //-----잔류전원 제거 Flow
  //------------------
  FuncRYControl(255,0,250);
      
  end_flow_pc(test_count, test_result_flag);
	
	if(test_count == 0)
	{
		Beep(100);
		list_disp_flag = false; 
	}
	else
	{
		Beep(50); Beep(50); Beep(50);
		list_disp_flag = test_finsh_flag  = true;
	}
}

void TaskStartFlow(void)
{
	VCC1_ON(false);	VCC2_ON(false);	//VCC3_ON(false);
	VCC3_ON(true); // VCC3 반대
	OKLED(false);	NGLED(false);
	FuncRYControl(0xff,false,0);

	// O/S 로그 초기화
	sprintf(open_disp, "-");
	sprintf(short_disp, "-");

	Op_str_idx = Op_end_idx = 0;
	Sh_str_idx = Sh_end_idx = 0;
	Pt_str_idx = Pt_end_idx = 0;
	Fn_str_idx = Fn_end_idx = 0;

	op_auto_result = sh_auto_result = ESC_KEY;
	pt_auto_result = fn_auto_result = ESC_KEY;
		
	memset(panel_data, 0x00, 50 * 1800 * sizeof(int16_t));	
	memset(panel_dataf, 0x00, 50 * 1800 * sizeof(float));		
	memset(ErrorNodeCount, 0x00, 50 * sizeof(int16_t));	
		
	memset(test_item_name, 		0x00, 2048 * 50 * sizeof(char));		
	memset(test_meas_string,	0x00, 2048 * 50 * sizeof(char));		
	memset(test_meas_string1, 	0x00, 2048 * 50 * sizeof(char));		
	memset(test_meas_value, 	0x00, 2048 * sizeof(float));		
	memset(test_result, 		0x00, 2048 * sizeof(char));
	test_count = 0;	
		
	memset(test_ng_item_name, 	0x00, 2048 * 50 * sizeof(char));		
	memset(test_ng_meas_string, 0x00, 2048 * 50 * sizeof(char));		
	memset(test_ng_result, 		0x00, 2048 * sizeof(char));
	test_ng_count = 0;	
	
	MainPopup(false, GUI_LIGHTRED," ");	

	test_result_flag = true;
	test_finsh_flag  = false; 
	list_disp_flag = false; 
	ListViewMode = 0;
  //----------------------------- 221108 MCH	
  /*EvccControl(1,false);
  EvccControl(2,false);
  FuncRYControl(255,false,100);
  RYBD_1ON(false);
  RYBD_2ON(false);
  RYBD_3ON(false);
  RYBD_4ON(false);
  Extend_VDD(true);//active low
  Extend_VDD_RY(false);*/
  //------------------------------
}

void TestResultDisp(uint8_t wheel,uint8_t mode)
{	
	if(is_pc_connected_flag == true)	// PC 검사기

	
	if(list_disp_flag == true)
	{
		wheel = 2;
		mode = false;	
		list_disp_flag = false;		
	}
	
	if(mode == true)
	{
		ListViewMode++;
		if(ListViewMode == 2) ListViewMode = 0;
	}
		
	if(test_finsh_flag == true)
	{
		char *pDispText[4] = {0};
		char  aDispText[4][100] = {0};
		char text[50];  
	
		MainPopup(false, GUI_LIGHTRED," ");         
		uint8_t  part_i=0;
		uint16_t idx = 0;  
			
		ListView_CLEAR();
		if(wheel == 0)    		
		{
			if(ListViewMode == 0 )			{	Result_Display_Page--; }
			else if(ListViewMode == 1 )	{	NG_Result_Display_Page--; }
		}
		else if(wheel == 1)    
		{
			if(ListViewMode == 0 )			{	Result_Display_Page++; }
			else if(ListViewMode == 1 )	{	NG_Result_Display_Page++; }
		}
		else if(wheel == 2) 
		{		
			if(ListViewMode == 0 )			
			{
				Result_Total_Page = (test_count-1)/15;  Result_Display_Page = 0; 
			}
			else if(ListViewMode == 1 )	
			{
				test_ng_count = 0;
				for(int cnt = 0; cnt < test_count; cnt++)	
				{
					if( test_result[cnt] == 'F' )
					{
						test_ng_result[test_ng_count] = 'F';							
						sprintf(test_ng_item_name[test_ng_count],"%s",test_item_name[cnt]);
						sprintf(test_ng_meas_string[test_ng_count],"%s",test_meas_string[cnt]);
						test_ng_count++;
					}
				}
				NG_Result_Total_Page = (test_ng_count-1)/15;   
				NG_Result_Display_Page = 0;
			}
		}
		
		if(ListViewMode == 0 )			
		{
			if(Result_Display_Page > Result_Total_Page)  Result_Display_Page = 0;
			else if(Result_Display_Page < 0)  Result_Display_Page = Result_Total_Page;
			
			for(idx = 0; idx < 15; idx++)
			{           
				if(idx + Result_Display_Page * 15 < test_count)
				{
					sprintf(aDispText[0], "%2d",	idx + Result_Display_Page * 15 + 1);
					sprintf(aDispText[1], "%s",		test_item_name[idx + Result_Display_Page * 15]);
					sprintf(aDispText[2], " ");
					sprintf(aDispText[3], " ");
					for (part_i = 0; part_i < 4; part_i++) pDispText[part_i] = aDispText[part_i];    

					LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pDispText);   
					
					if( test_result[idx + Result_Display_Page * 15] == 'P' )
					{                				
						LISTVIEW_SetItemText(hItem, 2, idx, test_meas_string[idx + Result_Display_Page * 15]); 
						LISTVIEW_SetItemText(hItem, 3, idx, "PASS");                 
						LISTVIEW_SetItemTextColor(hItem, 0, idx, LISTVIEW_CI_UNSEL , GUI_BLUE);
						LISTVIEW_SetItemTextColor(hItem, 1, idx, LISTVIEW_CI_UNSEL , GUI_BLUE);
						LISTVIEW_SetItemTextColor(hItem, 3, idx, LISTVIEW_CI_UNSEL , GUI_BLUE);

						LISTVIEW_SetItemTextColor(hItem, 0, idx, LISTVIEW_CI_UNSEL , GUI_WHITE);
						LISTVIEW_SetItemBkColor(hItem, 0, idx, LISTVIEW_CI_UNSEL , GUI_BLUE);
					}
					else if(test_result[idx + Result_Display_Page * 15] == 'F')
					{            				              
						LISTVIEW_SetItemText(hItem, 2, idx, test_meas_string[idx + Result_Display_Page * 15]);  
						LISTVIEW_SetItemText(hItem, 3, idx, "FAIL");         
						LISTVIEW_SetItemTextColor(hItem, 0, idx, LISTVIEW_CI_UNSEL , GUI_RED);
						LISTVIEW_SetItemTextColor(hItem, 1, idx, LISTVIEW_CI_UNSEL , GUI_RED);
						LISTVIEW_SetItemTextColor(hItem, 3, idx, LISTVIEW_CI_UNSEL , GUI_RED);
						
						LISTVIEW_SetItemTextColor(hItem, 0, idx, LISTVIEW_CI_UNSEL , GUI_WHITE);
						LISTVIEW_SetItemBkColor(hItem, 0, idx, LISTVIEW_CI_UNSEL , GUI_RED);
						
					}
					else if(test_result[idx + Result_Display_Page * 15] == 'S')
					{            				              
						LISTVIEW_SetItemText(hItem, 2, idx, test_meas_string[idx + Result_Display_Page * 15]);  
						LISTVIEW_SetItemText(hItem, 3, idx, "SKIP");         
						LISTVIEW_SetItemTextColor(hItem, 0, idx, LISTVIEW_CI_UNSEL , GUI_GREEN);
						LISTVIEW_SetItemTextColor(hItem, 1, idx, LISTVIEW_CI_UNSEL , GUI_GREEN);
						LISTVIEW_SetItemTextColor(hItem, 3, idx, LISTVIEW_CI_UNSEL , GUI_GREEN);
						
						LISTVIEW_SetItemTextColor(hItem, 0, idx, LISTVIEW_CI_UNSEL , GUI_WHITE);
						LISTVIEW_SetItemBkColor(hItem, 0, idx, LISTVIEW_CI_UNSEL , GUI_GREEN);						
					}
				}
				else
				{
					sprintf(aDispText[0], " ");
					sprintf(aDispText[1], " ");
					sprintf(aDispText[2], " ");
					sprintf(aDispText[3], " ");
					for (part_i = 0; part_i < 4; part_i++) pDispText[part_i] = aDispText[part_i];                
					LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pDispText);   
					LISTVIEW_SetItemText(hItem, 2, idx," ");
					LISTVIEW_SetItemText(hItem, 3, idx," ");         
				}
			}
			sprintf(aDispText[0], " ");
			sprintf(aDispText[1], " ");
			sprintf(aDispText[2], " ");
			sprintf(aDispText[3], " ");
			for (part_i = 0; part_i < 4; part_i++) pDispText[part_i] = aDispText[part_i];                
			LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pDispText);   
			
			sprintf(text, "ALL: %3d / %3d",Result_Display_Page+1, Result_Total_Page+1);
			LISTVIEW_SetItemText(hItem, 2, 15,text);
			LISTVIEW_SetItemTextColor(hItem, 2, 15, LISTVIEW_CI_UNSEL , GUI_WHITE);
			LISTVIEW_SetItemBkColor(hItem, 2, 15, LISTVIEW_CI_UNSEL , GUI_GRAY);
			LISTVIEW_SetItemText(hItem, 3, 15,"");         
		}
		else if(ListViewMode == 1 )	
		{	  
			if(NG_Result_Display_Page > NG_Result_Total_Page)  NG_Result_Display_Page = 0;
			else if(NG_Result_Display_Page < 0)  NG_Result_Display_Page = NG_Result_Total_Page;
				
			for(idx = 0; idx < 15; idx++)
			{           
				if(idx + NG_Result_Display_Page * 15 < test_ng_count)
				{
					sprintf(aDispText[0], "%2d",	idx + NG_Result_Display_Page * 15 + 1);
					sprintf(aDispText[1], "%s",		test_ng_item_name[idx + NG_Result_Display_Page * 15]);
					sprintf(aDispText[2], " ");
					sprintf(aDispText[3], " ");
					for (part_i = 0; part_i < 4; part_i++) pDispText[part_i] = aDispText[part_i];    

					LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pDispText);   
				
					LISTVIEW_SetItemText(hItem, 2, idx, test_ng_meas_string[idx + NG_Result_Display_Page * 15]);  
					LISTVIEW_SetItemText(hItem, 3, idx, "FAIL");         
					LISTVIEW_SetItemTextColor(hItem, 0, idx, LISTVIEW_CI_UNSEL , GUI_RED);
					LISTVIEW_SetItemTextColor(hItem, 1, idx, LISTVIEW_CI_UNSEL , GUI_RED);
					LISTVIEW_SetItemTextColor(hItem, 3, idx, LISTVIEW_CI_UNSEL , GUI_RED);
							
					LISTVIEW_SetItemTextColor(hItem, 0, idx, LISTVIEW_CI_UNSEL , GUI_WHITE);
					LISTVIEW_SetItemBkColor(hItem, 0, idx, LISTVIEW_CI_UNSEL , GUI_RED);
				}
				else
				{
					sprintf(aDispText[0], " ");
					sprintf(aDispText[1], " ");
					sprintf(aDispText[2], " ");
					sprintf(aDispText[3], " ");
					for (part_i = 0; part_i < 4; part_i++) pDispText[part_i] = aDispText[part_i];                
					LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pDispText);   
					LISTVIEW_SetItemText(hItem, 2, idx," ");
					LISTVIEW_SetItemText(hItem, 3, idx," ");         
				}
			}
			sprintf(aDispText[0], " ");
			sprintf(aDispText[1], " ");
			sprintf(aDispText[2], " ");
			sprintf(aDispText[3], " ");
			for (part_i = 0; part_i < 4; part_i++) pDispText[part_i] = aDispText[part_i];                
			LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pDispText);   
			
			sprintf(text, "FAIL: %3d / %3d",NG_Result_Display_Page+1, NG_Result_Total_Page+1);
			LISTVIEW_SetItemText(hItem, 2, 15,text);
			LISTVIEW_SetItemTextColor(hItem, 2, 15, LISTVIEW_CI_UNSEL , GUI_WHITE);
			LISTVIEW_SetItemBkColor(hItem, 2, 15, LISTVIEW_CI_UNSEL , GUI_GRAY);
			LISTVIEW_SetItemText(hItem, 3, 15,"");				
		}
		GUI_Exec();
	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
