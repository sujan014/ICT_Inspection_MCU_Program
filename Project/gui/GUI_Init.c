#include "GUI_Init.h" 

WM_HWIN hItem;
WM_HWIN hListview;
WM_HWIN hStatewin;
WM_HWIN hMenuBtwin;
WM_HWIN hKeyPadwin;
WM_HWIN hSysicon;
WM_HWIN hSyswin1;	
WM_HWIN hSyswin2;
WM_HWIN hSyswin3;
WM_HWIN hSyswin4;


void LogoDisplay(const GUI_BITMAP GUI_UNI_PTR *pBM, int disptime)    // disp center
{
	#define SCREENX 480
	#define SCREENY 272
		
    GUI_CONST_STORAGE GUI_BITMAP BTTemp = *pBM;
    const int ctx = (SCREENX-BTTemp.XSize)>>1, cty = (SCREENY-BTTemp.YSize)>>1;    
    
    GUI_SetBkColor(GUI_WHITE);		
	GUI_Clear();	
	GUI_DrawBitmap(pBM, ctx, cty );

    WM_DisableMemdev(WM_HBKWIN);	
	GUI_Delay(1);
	WM_EnableMemdev(WM_HBKWIN);

	Delay_ms(disptime);
  
	GUI_SetBkColor(GUI_BLACK);
	GUI_Clear();   
	WM_SelectWindow(WM_HBKWIN); 
}

void ProjectDisplay(void)
{    
	char text[100];    
	
	GUI_RECT Pro_Rect = {   6, 2, 200, 25};
	//GUI_RECT FWON_Rect ={ 190, 2, 295, 25};
	GUI_RECT FWON_Rect ={ 220, 2, 295, 25};
	GUI_RECT Fwv_Rect = { 300, 2, 474, 25};
	
	GUI_SetColor(GUI_BLACK); GUI_AA_SetFactor(0); GUI_AA_FillRoundedRect(2, 2, 478, 25, 0);
	GUI_SetColor(GUI_WHITE); GUI_AA_SetFactor(8); GUI_AA_DrawRoundedRect(2, 2, 478, 25, 8);
	
	GUI_SetFont(GUI_FONT_8X16);   
	GUI_DispStringInRectWrap(project_name_main, &Pro_Rect, GUI_TA_LEFT  | GUI_TA_VCENTER, GUI_WRAPMODE_WORD);	
	GUI_SetTextMode(GUI_TM_TRANS);	
				
	if(RD_SUB_DISP == 1)	sprintf(text,"%s",In_data.name);
	else					sprintf(text,"Serial:%08lX",serial_num);
	
	GUI_SetFont(GUI_FONT_6X9);
	GUI_DispStringInRectWrap(text, &Fwv_Rect, GUI_TA_RIGHT | GUI_TA_VCENTER, GUI_WRAPMODE_WORD);		
	GUI_SetTextMode(GUI_TM_TRANS);
	
	char tx1[50], tx2[50], /*tx3[50],*/ tx4[50];
	
	if(RD_FW_WRITE == true)		sprintf(tx1, "FW:O");
	else						sprintf(tx1, "FW:X");

	if(RD_NG_CONTINUE == true)	sprintf(tx2, "NG:O");
	else 						sprintf(tx2, "NG:X");

	/*if(RD_SDC_SAVE == true)		sprintf(tx3, "SDC:O");
	else						sprintf(tx3, "SDC:X");*/ //210514 MCH
	sprintf(tx4, "%s",project_name_sub);
	
	//sprintf(text, "%s/%s/%s\n%s",tx1,tx2,tx3,tx4);
	sprintf(text, "%s/%s\n%s",tx1,tx2,tx4);
	
	GUI_SetFont(GUI_FONT_6X8);//GUI_FONT_4X6 , GUI_FONT_6X9
	GUI_DispStringInRectWrap(text, &FWON_Rect, GUI_TA_LEFT | GUI_TA_VCENTER, GUI_WRAPMODE_WORD);	
	GUI_SetTextMode(GUI_TM_TRANS);
}

void LcdDisplayInit(void)
{
	hListview  	= CreateMainListview();
	hStatewin  	= CreateMainState();
	hMenuBtwin 	= CreateMenuButton();
	hKeyPadwin 	= CreateKeyPad();
	hSysicon 	= CreateSysIcon();
	hSyswin1 	= CreateSyswin1();
	hSyswin2 	= CreateSyswin2();
	hSyswin3 	= CreateSyswin3();
	hSyswin4	= CreateSyswin4();

	DispSelect(MAIN_PAGE);

    WM_DisableMemdev(WM_HBKWIN);
	GUI_Delay(1);
    WM_EnableMemdev(WM_HBKWIN);
    WM_SelectWindow(WM_HBKWIN);
}

void DispSelect(uint8_t page)
{	
	current_page = page;
	
	if(current_page == MAIN_PAGE)
	{
		WM_ShowWindow(hListview);	WM_SetStayOnTop(hListview,1);
		WM_ShowWindow(hStatewin);	WM_SetStayOnTop(hStatewin,1);
		WM_HideWindow(hMenuBtwin);	WM_SetStayOnTop(hMenuBtwin,0);
		WM_HideWindow(hKeyPadwin);	WM_SetStayOnTop(hKeyPadwin,0);
		WM_HideWindow(hSysicon);	WM_SetStayOnTop(hSysicon,0);
		WM_HideWindow(hSyswin1);	WM_SetStayOnTop(hSyswin1,0);
		WM_HideWindow(hSyswin2);	WM_SetStayOnTop(hSyswin2,0);
		WM_HideWindow(hSyswin3);	WM_SetStayOnTop(hSyswin3,0);
		WM_HideWindow(hSyswin4);	WM_SetStayOnTop(hSyswin4,0);
	}
	else if(current_page == MENUBT_PAGE)
	{
		WM_HideWindow(hListview);	WM_SetStayOnTop(hListview,0);
		WM_HideWindow(hStatewin);	WM_SetStayOnTop(hStatewin,0);
		WM_ShowWindow(hMenuBtwin);	WM_SetStayOnTop(hMenuBtwin,1);
		WM_HideWindow(hKeyPadwin);	WM_SetStayOnTop(hKeyPadwin,0);
		WM_HideWindow(hSysicon);	WM_SetStayOnTop(hSysicon,0);
		WM_HideWindow(hSyswin1);	WM_SetStayOnTop(hSyswin1,0);
		WM_HideWindow(hSyswin2);	WM_SetStayOnTop(hSyswin2,0);
		WM_HideWindow(hSyswin3);	WM_SetStayOnTop(hSyswin3,0);
		WM_HideWindow(hSyswin4);	WM_SetStayOnTop(hSyswin4,0);
	}
	else if(current_page == KEYPAD_PAGE)
	{
		WM_HideWindow(hListview);	WM_SetStayOnTop(hListview,0);
		WM_HideWindow(hStatewin);	WM_SetStayOnTop(hStatewin,0);
		WM_HideWindow(hMenuBtwin);	WM_SetStayOnTop(hMenuBtwin,0);
		WM_ShowWindow(hKeyPadwin);	WM_SetStayOnTop(hKeyPadwin,1);
		WM_HideWindow(hSysicon);	WM_SetStayOnTop(hSysicon,0);
		WM_HideWindow(hSyswin1);	WM_SetStayOnTop(hSyswin1,0);
		WM_HideWindow(hSyswin2);	WM_SetStayOnTop(hSyswin2,0);
		WM_HideWindow(hSyswin3);	WM_SetStayOnTop(hSyswin3,0);
		WM_HideWindow(hSyswin4);	WM_SetStayOnTop(hSyswin4,0);
	}
	else if(current_page == MENU_PAGE1)
	{		
		WM_HideWindow(hListview);	WM_SetStayOnTop(hListview,0);
		WM_HideWindow(hStatewin);	WM_SetStayOnTop(hStatewin,0);
		WM_HideWindow(hMenuBtwin);	WM_SetStayOnTop(hMenuBtwin,0);
		WM_HideWindow(hKeyPadwin);	WM_SetStayOnTop(hKeyPadwin,0);
		WM_ShowWindow(hSysicon);	WM_SetStayOnTop(hSysicon,1);
		WM_ShowWindow(hSyswin1);	WM_SetStayOnTop(hSyswin1,1);
		WM_HideWindow(hSyswin2);	WM_SetStayOnTop(hSyswin2,0);
		WM_HideWindow(hSyswin3);	WM_SetStayOnTop(hSyswin3,0);
		WM_HideWindow(hSyswin4);	WM_SetStayOnTop(hSyswin4,0);		
		menu_gettime_update();
		menu_settime_update(meun_year, meun_month, meun_day, meun_hour, meun_min);
	
	}
	else if(current_page == MENU_PAGE2)
	{
		WM_HideWindow(hListview);	WM_SetStayOnTop(hListview,0);
		WM_HideWindow(hStatewin);	WM_SetStayOnTop(hStatewin,0);
		WM_HideWindow(hMenuBtwin);	WM_SetStayOnTop(hMenuBtwin,0);
		WM_HideWindow(hKeyPadwin);	WM_SetStayOnTop(hKeyPadwin,0);
		WM_ShowWindow(hSysicon);	WM_SetStayOnTop(hSysicon,1);
		WM_HideWindow(hSyswin1);	WM_SetStayOnTop(hSyswin1,0);
		WM_ShowWindow(hSyswin2);	WM_SetStayOnTop(hSyswin2,1);
		WM_HideWindow(hSyswin3);	WM_SetStayOnTop(hSyswin3,0);
		WM_HideWindow(hSyswin4);	WM_SetStayOnTop(hSyswin4,0);
	}
	else if(current_page == MENU_PAGE3)
	{		
		WM_HideWindow(hListview);	WM_SetStayOnTop(hListview,0);
		WM_HideWindow(hStatewin);	WM_SetStayOnTop(hStatewin,0);
		WM_HideWindow(hMenuBtwin);	WM_SetStayOnTop(hMenuBtwin,0);
		WM_HideWindow(hKeyPadwin);	WM_SetStayOnTop(hKeyPadwin,0);
		WM_ShowWindow(hSysicon);	WM_SetStayOnTop(hSysicon,1);
		WM_HideWindow(hSyswin1);	WM_SetStayOnTop(hSyswin1,0);
		WM_HideWindow(hSyswin2);	WM_SetStayOnTop(hSyswin2,0);
		WM_ShowWindow(hSyswin3);	WM_SetStayOnTop(hSyswin3,1);
		WM_HideWindow(hSyswin4);	WM_SetStayOnTop(hSyswin4,0);
	}
	else if(current_page == MENU_PAGE4)
	{			
		WM_HideWindow(hListview);	WM_SetStayOnTop(hListview,0);
		WM_HideWindow(hStatewin);	WM_SetStayOnTop(hStatewin,0);
		WM_HideWindow(hMenuBtwin);	WM_SetStayOnTop(hMenuBtwin,0);
		WM_HideWindow(hKeyPadwin);	WM_SetStayOnTop(hKeyPadwin,0);
		WM_ShowWindow(hSysicon);	WM_SetStayOnTop(hSysicon,1);
		WM_HideWindow(hSyswin1);	WM_SetStayOnTop(hSyswin1,0);
		WM_HideWindow(hSyswin2);	WM_SetStayOnTop(hSyswin2,0);
		WM_HideWindow(hSyswin3);	WM_SetStayOnTop(hSyswin3,0);
		WM_ShowWindow(hSyswin4);	WM_SetStayOnTop(hSyswin4,1);		           
		EVCC_Check();			
	}
	else
	{		
		WM_ShowWindow(hListview);	WM_SetStayOnTop(hListview,1);
		WM_ShowWindow(hStatewin);	WM_SetStayOnTop(hStatewin,1);
		WM_HideWindow(hMenuBtwin);	WM_SetStayOnTop(hMenuBtwin,0);
		WM_HideWindow(hKeyPadwin);	WM_SetStayOnTop(hKeyPadwin,0);
		WM_HideWindow(hSysicon);	WM_SetStayOnTop(hSysicon,0);
		WM_HideWindow(hSyswin1);	WM_SetStayOnTop(hSyswin1,0);
		WM_HideWindow(hSyswin2);	WM_SetStayOnTop(hSyswin2,0);
		WM_HideWindow(hSyswin3);	WM_SetStayOnTop(hSyswin3,0);
		WM_HideWindow(hSyswin4);	WM_SetStayOnTop(hSyswin4,0);	
	}
	GUI_Exec();
}

uint8_t TouchSensing(void)
{
	static char first_touch = false;
	static GUI_PID_STATE State;  
	
	if( TOUCH_PENIRQ == 0 )
	{   		
		getDisplayPoint(&display, Read_Screen());	
		if ((display.y < 272) && (display.y > 0) && (display.x > 0) && (display.x < 480))
		{	
			//dbgmsg(">> touch sensing : %03d,%03d\n",display.x,display.y);
			State.x = display.x;
			State.y = display.y;
			State.Pressed = true;
			State.Layer = 0;
			first_touch = true;			
			GUI_TOUCH_StoreStateEx(&State);
			GUI_Exec();		
			return true;
		}
		else
		{
			
		}
	}
	else
	{
		if(first_touch == false)		return false;
		
		first_touch = false;		
		State.Pressed = false;		
		GUI_TOUCH_StoreStateEx(&State);
		GUI_Exec();
		
		return false;
	}    	
	return true;
}

int meun_year = 21, meun_month = 03, meun_day = 11,meun_week=1, meun_hour = 12,  meun_min = 00,  meun_sec = 00; 
uint8_t year_flag= 0, month_flag= 0, day_flag= 0, week_flag= 0, hour_flag= 0, min_flag= 0;
void sys_win(void)
{					
	char menu_win_flag_pre = 0;
	char menu_dispaly = true;
	menu_win_flag = 0;

	while(1)
	{
		Delay_ms(5);		
		if(menu_win_flag == 0)
		{
			if(usb_host_connected_flag  == false)	USBH_Process(&USB_OTG_Core, &USB_Host); 
		}

		if(KeyScan() == (MOD_ON|ST2_ON))	break;
		
		if( menu_dispaly == true)
		{
			if(menu_win_flag == 0)			DispSelect(MENU_PAGE1);
			else if(menu_win_flag == 1)		DispSelect(MENU_PAGE2);
			else if(menu_win_flag == 2)		DispSelect(MENU_PAGE3);
			else if(menu_win_flag == 3)		DispSelect(MENU_PAGE4);
			menu_win_flag_pre = menu_win_flag;
			menu_dispaly = false;
		}
		else
		{
			if(	menu_win_flag_pre != menu_win_flag )	menu_dispaly = true;
		}
		TouchSensing();
	}
}
void boot_win(void)
{
	DispSelect(KEYPAD_PAGE);
	
	for(int i = 0; i< 16; i++)    InputKey[i] = 0;
	PASSWORD_FLAG = PASSWORD_RD;
	hItem = WM_GetDialogItem(hKeyPadwin, KEY_TEXT_1);
	TEXT_SetText(hItem,"Please Enter The 4-Digit Passcode");
	GUI_Exec();
	while(1)
	{
		Delay_ms(5);		
		if(KeyScan() == (MOD_ON|ST2_ON))	break;
		
		if(PASSWORD_FLAG != PASSWORD_RD)
		{							
			if(PASSWORD_FLAG == PASSWORD_OK)
			{				
				Keypad_POPUP(true, GUI_DARKBLUE,"The passcode is correct\nRestart to the Bootloader");
				GUI_Delay(500);
				Keypad_POPUP(false, GUI_DARKBLUE,"");				
				Beep(500);
				WR_BOOT(BOOT_FLAG);
				NVIC_SystemReset();
				break;
			}
			else if(PASSWORD_FLAG == DBUGMODE_OK)
			{
				if(RD_DBG_MD == 0) 		{	WR_DBG_MD(1);	Keypad_POPUP(true, GUI_DARKBLUE,"The passcode is correct\nRestart to the release mode");	}
				else if(RD_DBG_MD == 1) {	WR_DBG_MD(0);	Keypad_POPUP(true, GUI_DARKBLUE,"The passcode is correct\nRestart to the debug mode");		}
				GUI_Delay(500);
				Keypad_POPUP(false, GUI_DARKBLUE,"");
				Beep(500);				
				WR_BOOT(APP_FLAG);
				NVIC_SystemReset();
				break;
			}
			else if(PASSWORD_FLAG == PASSWORD_NG)
			{								
				Keypad_POPUP(true, GUI_RED,"The passcode is incorrect\nReturn to the main display");
				GUI_Delay(500);
				Keypad_POPUP(false, GUI_RED,"");
				Beep(500);				
				break;
			}
		}		
		TouchSensing();
	}
}

void MenuChange(void)
{			
	Beep(1);
	OKLED(true);
	NGLED(true);

	menu_sel_flag = 0;
	DispSelect(MENUBT_PAGE);
	
	while(1)
	{			
		Delay_ms(5);

		if(KeyScan() == (MOD_ON|ST2_ON))	break;

		if(menu_sel_flag != 0)
		{
			if(menu_sel_flag == 1)
			{
				sys_win();
			}
			else if(menu_sel_flag == 2)
			{
				dbgmsg("TBD\n");
			}
			else if(menu_sel_flag == 3)
			{
				boot_win();
			}
			break;
		}
		TouchSensing();
	}
	
	DispSelect(MAIN_PAGE);
	
	OKLED(false);
	NGLED(false);
	Beep(100);	

	return;
}
/*************************** End of file ****************************/
