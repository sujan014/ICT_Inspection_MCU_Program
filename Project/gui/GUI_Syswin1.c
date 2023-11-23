#include "GUI_Syswin1.h" 

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
    { FRAMEWIN_CreateIndirect, "", MENU1_VIEW,		 75,	28,		 405, 	242, 0, 0x0, 0 },   
		
	{ TEXT_CreateIndirect,     "", MENU1_TIME_EGDE,		  5,	 5,		 295, 	 40, 0, 0x0, 0 },     
    { TEXT_CreateIndirect,     "", MENU1_TIME_TEXT,		  8,	 8,		 289, 	 17, 0, 0x0, 0 },   
    { TEXT_CreateIndirect,     "", MENU1_RTC_TEXT,		  8,	25,		 289, 	 17, 0, 0x0, 0 },  
    // RTC State    						
    { BUTTON_CreateIndirect,   "", MENU1_YEAR_SEL,		  5,	50,		  36, 	 42, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "", MENU1_MON_SEL,		 44,	50,		  36, 	 42, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "", MENU1_DAY_SEL,		 83,	50,		  36, 	 42, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "", MENU1_HOUR_SEL,		122,	50,		  36, 	 42, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "", MENU1_MIN_SEL,		161,	50,		  36, 	 42, 0, 0x0, 0 },
							
    { BUTTON_CreateIndirect,   "", MENU1_RTC_UP_BT,		205,	50,		  45, 	 42, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "", MENU1_RTC_DN_BT,		255,	50,		  45, 	 42, 0, 0x0, 0 },  
	
    { BUTTON_CreateIndirect,   "", MENU1_RTC_SET_BT,	305,	5,		  95, 	 90, 0, 0x0, 0 },  
					
    { BUTTON_CreateIndirect,   "", MENU1_ICT_DN_BT,		  5,	100,	 100, 	 44, 0, 0x0, 0 },    
	{ BUTTON_CreateIndirect,   "", MENU1_DATA_DN_BT,	  5,	147,	 100, 	 30, 0, 0x0, 0 },  
	{ BUTTON_CreateIndirect,   "", MENU1_PMIC_DN_BT,	  5,	179,	 100, 	 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,   "", MENU1_EEP_DN_BT,	  	  5,	211,	 100, 	 30, 0, 0x0, 0 },
	
    { TEXT_CreateIndirect,     "", MENU1_ICT_ST_TEXT,	110,	100,	 290, 	 44, 0, 0x0, 0 },   
	{ TEXT_CreateIndirect,     "", MENU1_DATA_ST_TEXT,	110,	147,	 290, 	 30, 0, 0x0, 0 },  
	{ TEXT_CreateIndirect,     "", MENU1_PMIC_ST_TEXT,	110,	179,	 290, 	 30, 0, 0x0, 0 }, 
	{ TEXT_CreateIndirect,     "", MENU1_EEP_ST_TEXT,	110,	211,	 290, 	 30, 0, 0x0, 0 },  	

    // State Pop up
    { TEXT_CreateIndirect,     "", MENU1_POPUP0,       0,     0,   405,   242, 0, 0x0, 0 },
};

static void menu_save_update(void);
static void menu_setdisp_update(char year_fg, char month_fg, char day_fg, char hour_fg, char min_fg);
static void updn_button_update(int upddn,char year_fg, char month_fg, char day_fg, char hour_fg, char min_fg);
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    int     NCode;
    int     Id;
	char text[100];
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
            FRAMEWIN_SetTitleVis(hItem, 0);     
			FRAMEWIN_SetBorderSize(hItem, 0);    
            FRAMEWIN_SetClientColor(hItem, GUI_BLACK);	 
                        
		    hItem = WM_GetDialogItem(pMsg->hWin, MENU1_RTC_SET_BT);   	 
			BUTTON_SetText(hItem, "SAVE");
			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKRED);	
			BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
            
            hItem = WM_GetDialogItem(pMsg->hWin,MENU1_TIME_EGDE); 
			TEXT_SetBkColor(hItem, GUI_LIGHTBLUE);
			
            hItem = WM_GetDialogItem(pMsg->hWin,MENU1_TIME_TEXT);  
			TEXT_SetText(hItem,"");      
			TEXT_SetTextAlign(hItem,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
			TEXT_SetBkColor(hItem, GUI_BLACK);
			
            hItem = WM_GetDialogItem(pMsg->hWin,MENU1_RTC_TEXT); 
			TEXT_SetText(hItem,"");   
			TEXT_SetTextAlign(hItem,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
            TEXT_SetTextColor(hItem, GUI_LIGHTRED);
			TEXT_SetBkColor(hItem, GUI_BLACK);
						
			menu_gettime_update();
			menu_settime_update(meun_year,meun_month,meun_day,meun_hour,meun_min);			

            hItem = WM_GetDialogItem(pMsg->hWin,MENU1_YEAR_SEL);  BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKRED);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);BUTTON_SetText(hItem,"year");
            hItem = WM_GetDialogItem(pMsg->hWin,MENU1_MON_SEL);   BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKRED);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);BUTTON_SetText(hItem,"mon");
            hItem = WM_GetDialogItem(pMsg->hWin,MENU1_DAY_SEL);   BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKRED);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);BUTTON_SetText(hItem,"day");
            hItem = WM_GetDialogItem(pMsg->hWin,MENU1_HOUR_SEL);  BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKRED);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);BUTTON_SetText(hItem,"hour");
            hItem = WM_GetDialogItem(pMsg->hWin,MENU1_MIN_SEL);   BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKRED);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);BUTTON_SetText(hItem,"min");
            hItem = WM_GetDialogItem(pMsg->hWin,MENU1_RTC_UP_BT); BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_LIGHTBLUE);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);BUTTON_SetText(hItem,"up");		
            hItem = WM_GetDialogItem(pMsg->hWin,MENU1_RTC_DN_BT); BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_LIGHTBLUE);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);BUTTON_SetText(hItem,"down");	
			
			///////////////////////////////////////
            hItem = WM_GetDialogItem(pMsg->hWin,MENU1_ICT_DN_BT);
			BUTTON_SetText(hItem, "ICT Spec Update\n(USB)");
			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);
			BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			///////////////////////////////////////
            hItem = WM_GetDialogItem(pMsg->hWin,MENU1_TSP_DN_BT);			
			BUTTON_SetText(hItem, "Panel Spec Update\n(USB)");
			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);
			BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			///////////////////////////////////////
            hItem = WM_GetDialogItem(pMsg->hWin,MENU1_DATA_DN_BT);			
			BUTTON_SetText(hItem, "Input Data Update\n(USB)");
			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);
			BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			///////////////////////////////////////
			hItem = WM_GetDialogItem(pMsg->hWin,MENU1_PMIC_DN_BT);			
			//BUTTON_SetText(hItem, "PMIC Data Update\n(USB)");
			BUTTON_SetText(hItem, "PMIC&TPIC Data\nUpdate(USB)");
			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);
			BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			hItem = WM_GetDialogItem(pMsg->hWin,MENU1_EEP_DN_BT);			
			BUTTON_SetText(hItem, "EEP Data Update\n(USB)");
			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);
			BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);	
			///////////////////////////////////////////////////////////
			hItem = WM_GetDialogItem(pMsg->hWin,MENU1_ICT_ST_TEXT);
			TEXT_SetTextAlign(hItem,TEXT_CF_LEFT|TEXT_CF_VCENTER);	
			TEXT_SetTextColor(hItem, GUI_WHITE);	
			TEXT_SetBkColor(hItem, GUI_DARKBLUE);	
			sprintf(text, " - TP(%d)\n - Open(%d)/Short bs(%d)/Short ex(%d)\n - Part(%d)/Func(%d)",tp_count,open_count,short_bs_count, short_ex_count, part_count, func_count);
			TEXT_SetText(hItem,text);

			hItem = WM_GetDialogItem(pMsg->hWin,MENU1_DATA_ST_TEXT);
			TEXT_SetTextAlign(hItem,TEXT_CF_LEFT|TEXT_CF_VCENTER);	
			TEXT_SetTextColor(hItem, GUI_WHITE);	
			TEXT_SetBkColor(hItem, GUI_DARKBLUE);		
			sprintf(text, " - %s\n - size : %.2f kB", In_data.name, (float)((float)In_data.size/1024));
			TEXT_SetText(hItem,text);
			//------------------------------------------------------------------------------------------ PMIC TPIC 같이 사용
			hItem = WM_GetDialogItem(pMsg->hWin,MENU1_PMIC_ST_TEXT);
			TEXT_SetTextAlign(hItem,TEXT_CF_LEFT|TEXT_CF_VCENTER);	
			TEXT_SetTextColor(hItem, GUI_WHITE);	
			TEXT_SetBkColor(hItem, GUI_DARKBLUE);		
			//sprintf(text, " - %s\n - size : %.2f kB", In_pmic.name, (float)((float)In_pmic.size/1024));
			sprintf(text, "-%s\n-%s", In_pmic.name,In_tpic.name);
			TEXT_SetText(hItem,text);
			//--------------------------------------------------------------------------------------------
			hItem = WM_GetDialogItem(pMsg->hWin,MENU1_EEP_ST_TEXT);
			TEXT_SetTextAlign(hItem,TEXT_CF_LEFT|TEXT_CF_VCENTER);	
			TEXT_SetTextColor(hItem, GUI_WHITE);	
			TEXT_SetBkColor(hItem, GUI_DARKBLUE);		
			sprintf(text, " - %s\n - size : %.2f kB", In_eep.name, (float)((float)In_eep.size/1024));
			TEXT_SetText(hItem,text);
			//-------------------------------------------------------------------------------------------
			hItem = WM_GetDialogItem(pMsg->hWin,MENU1_TSP_ST_TEXT);		
			TEXT_SetTextAlign(hItem,TEXT_CF_LEFT|TEXT_CF_VCENTER);				
			TEXT_SetTextColor(hItem, GUI_WHITE);
			TEXT_SetBkColor(hItem, GUI_DARKBLUE);	
			sprintf(text, " - TSP Panel Spec(%d)",panel_count);	
			TEXT_SetText(hItem, text);
			///////////////////////////////////////
			
            hItem = WM_GetDialogItem(pMsg->hWin, MENU1_POPUP0); 		TEXT_SetFont(hItem, GUI_FONT_32B_ASCII);      

            TEXT_SetBkColor(hItem, GUI_BLUE);
			TEXT_SetTextColor(hItem, GUI_RED); 
			TEXT_SetText(hItem,"");    
			TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER|TEXT_CF_VCENTER);  
            WM_SetStayOnTop(hItem,0);   
			WM_HideWindow(hItem);
			
		break;
        case WM_NOTIFY_PARENT:		
					
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;		
            switch(Id) 
            {										
				case MENU1_YEAR_SEL:
					switch(NCode)
					{		
						case WM_NOTIFICATION_RELEASED:		
							Beep(1);							
							year_flag =  month_flag = day_flag = hour_flag = min_flag = false;
							year_flag = true;	
							menu_setdisp_update(year_flag, month_flag, day_flag, hour_flag, min_flag);	
						break;
					}
				break;
				case MENU1_MON_SEL:
					switch(NCode)
					{		
						case WM_NOTIFICATION_RELEASED:				
							Beep(1);				
							year_flag =  month_flag = day_flag = hour_flag = min_flag = false;
							month_flag = true;	
							menu_setdisp_update(year_flag, month_flag, day_flag, hour_flag, min_flag);	
						break;
					}
				break;
				case MENU1_DAY_SEL:
					switch(NCode)
					{		
						case WM_NOTIFICATION_RELEASED:						
							Beep(1);			
							year_flag =  month_flag = day_flag = hour_flag = min_flag = false;
							day_flag = true;	
							menu_setdisp_update(year_flag, month_flag, day_flag, hour_flag, min_flag);	
						break;
					}
				break;
				case MENU1_HOUR_SEL:
					switch(NCode)
					{		
						case WM_NOTIFICATION_RELEASED:	
							Beep(1);								
							year_flag =  month_flag = day_flag = hour_flag = min_flag = false;
							hour_flag = true;	
							menu_setdisp_update(year_flag, month_flag, day_flag, hour_flag, min_flag);		
						break;
					}
				break;
				case MENU1_MIN_SEL:
					switch(NCode)
					{		
						case WM_NOTIFICATION_RELEASED:	
							Beep(1);								
							year_flag =  month_flag = day_flag = hour_flag = min_flag = false;
							min_flag = true;	
							menu_setdisp_update(year_flag, month_flag, day_flag, hour_flag, min_flag);	
						break;
					}
				break;				   	
				case MENU1_RTC_UP_BT:
					switch(NCode)
					{		
						case WM_NOTIFICATION_RELEASED:	
							Beep(1);							
							updn_button_update(MENU1_RTC_UP_BT,year_flag, month_flag, day_flag, hour_flag, min_flag);	
						break;
					}
				break;			
				case MENU1_RTC_DN_BT:
					switch(NCode)
					{		
						case WM_NOTIFICATION_RELEASED:	
							Beep(1);								
							updn_button_update(MENU1_RTC_DN_BT,year_flag, month_flag, day_flag, hour_flag, min_flag);	
						break;
					}
				break;				
				case MENU1_RTC_SET_BT:
					switch(NCode)
					{		
						case WM_NOTIFICATION_RELEASED:
							Beep(1);					
							menu_save_update();								
							menu_gettime_update();						
						break;
					}
				break;	  
				case MENU1_ICT_DN_BT:
					switch(NCode)
					{		
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							IctSpecSaveflow();					
						break;
					}
				break;	
		
				case MENU1_TSP_DN_BT:
					switch(NCode)
					{		
						case WM_NOTIFICATION_RELEASED:
							Beep(1);	
							PanelSaveFlow();						
						break;
					}
				break;			
				case MENU1_DATA_DN_BT:
					switch(NCode)
					{		
						case WM_NOTIFICATION_RELEASED:	
							Beep(1);
							DataSaveflow();					
						break;
					}
				break;						
				//------------------------------------
				case MENU1_PMIC_DN_BT:
					switch(NCode)
					{		
						case WM_NOTIFICATION_RELEASED:	
							Beep(1);
							pmicSaveflow();
						break;
					}
				break;	
				case MENU1_EEP_DN_BT:
					switch(NCode)
					{		
						case WM_NOTIFICATION_RELEASED:	
							Beep(1);
							eepSaveflow();			
						break;
					}
				break;	
			}			
        break;
        
        default:
            WM_DefaultProc(pMsg);
        break;
    }
}

void Sys1Popup(uint8_t flag, GUI_COLOR Color, char msg[100])
{
	hItem = WM_GetDialogItem(hSyswin1, MENU1_POPUP0);
    if( flag == true ) 
    {
        TEXT_SetBkColor(hItem, GUI_DARKBLUE); 
        TEXT_SetTextColor(hItem, GUI_WHITE); 
        TEXT_SetText(hItem,msg);
        WM_ShowWindow(hItem);        
    }
    else if( flag == false ) 
    {
        WM_HideWindow(hItem);
    }
	
    GUI_Exec();
}

static void menu_save_update(void)
{
	char text[100];
	year_flag =  month_flag = day_flag = week_flag = hour_flag = min_flag = 0;
	
	sprintf(text,"setting time : 20%02d-%02d-%02d %02d:%02d:00",meun_year,meun_month,meun_day,meun_hour,meun_min);
	hItem = WM_GetDialogItem(hSyswin1, MENU1_RTC_TEXT);
	TEXT_SetText(hItem,text); 

	Sys1Popup(true, GUI_DARKRED, "Current Time Save");	
	RTC_DateStr.RTC_Year    = meun_year;    RTC_DateStr.RTC_Month   = meun_month;
	RTC_DateStr.RTC_Date    = meun_day;     RTC_DateStr.RTC_WeekDay = meun_week;
	RTC_TimeStr.RTC_Hours   = meun_hour;    RTC_TimeStr.RTC_Minutes = meun_min;
	
	Delay_ms(100);
	
	Beep(100);
	
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr)!= ERROR && RTC_SetDate(RTC_Format_BIN, &RTC_DateStr) != ERROR) 
	{        
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);          
	}
	Sys1Popup(false, GUI_DARKRED, "");
}	

void menu_gettime_update(void)
{
	char text[100];
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);    RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
	
	meun_year = RTC_DateStr.RTC_Year;   
	meun_month = RTC_DateStr.RTC_Month;
	meun_day  = RTC_DateStr.RTC_Date;  	
	
	meun_hour = RTC_TimeStr.RTC_Hours;
	meun_min   = RTC_TimeStr.RTC_Minutes;
	meun_sec   = RTC_TimeStr.RTC_Seconds;
	
	sprintf(text,"current time : 20%02d-%02d-%02d %02d:%02d:%02d",meun_year,meun_month,meun_day,meun_hour,meun_min,meun_sec);  
    hItem = WM_GetDialogItem(hSyswin1, MENU1_TIME_TEXT);
	TEXT_SetText(hItem, text);
	GUI_Exec();
}

void menu_settime_update(int meun_year, int meun_month, int meun_day, int meun_hour, int meun_min)
{
	char text[100];
	RTC_DateStr.RTC_Year    = meun_year;    RTC_DateStr.RTC_Month   = meun_month;                           
	RTC_DateStr.RTC_Date    = meun_day;     RTC_DateStr.RTC_WeekDay = meun_week;                            
	RTC_TimeStr.RTC_Hours   = meun_hour;    RTC_TimeStr.RTC_Minutes = meun_min;                        

	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr)!= ERROR && RTC_SetDate(RTC_Format_BIN, &RTC_DateStr) != ERROR)                            
	{        
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);   
	}							
	
	sprintf(text,"setting time : 20%02d-%02d-%02d %02d:%02d:00",meun_year,meun_month,meun_day,meun_hour,meun_min);
    hItem = WM_GetDialogItem(hSyswin1, MENU1_RTC_TEXT);
	TEXT_SetText(hItem,text);
	GUI_Exec();
}

static void menu_setdisp_update(char year_fg, char month_fg, char day_fg, char hour_fg, char min_fg)
{
	char text[100];
	if( year_fg == true)
	{
		sprintf(text,"setting year : 20%02d-%02d-%02d %02d:%02d:00",meun_year,meun_month,meun_day,meun_hour,meun_min);
	}
	else if( month_fg == true)
	{
		sprintf(text,"setting mon  : 20%02d-%02d-%02d %02d:%02d:00",meun_year,meun_month,meun_day,meun_hour,meun_min);
	}
	else if( day_fg == true)
	{
		sprintf(text,"setting day  : 20%02d-%02d-%02d %02d:%02d:00",meun_year,meun_month,meun_day,meun_hour,meun_min);
	}
	else if( hour_fg == true)
	{
		sprintf(text,"setting hour : 20%02d-%02d-%02d %02d:%02d:00",meun_year,meun_month,meun_day,meun_hour,meun_min);
	}
	else if( min_fg == true)
	{
		sprintf(text,"setting min  : 20%02d-%02d-%02d %02d:%02d:00",meun_year,meun_month,meun_day,meun_hour,meun_min);
	}
	else
	{
		sprintf(text,"setting time : 20%02d-%02d-%02d %02d:%02d:00",meun_year,meun_month,meun_day,meun_hour,meun_min);
	}
	hItem = WM_GetDialogItem(hSyswin1,MENU1_RTC_TEXT);  
	TEXT_SetText(hItem,text);  
	GUI_Exec();
}
static void updn_button_update(int upddn, char year_fg, char month_fg, char day_fg, char hour_fg, char min_fg)
{
	if(year_fg == 1)
	{
		if(upddn == MENU1_RTC_UP_BT)		meun_year++;
		else if(upddn == MENU1_RTC_DN_BT)	meun_year--;		
		if(meun_year > 30) meun_year = 20;
		else if(meun_year < 20) meun_year = 30;
	}
	else if(month_fg == 1)
	{
		if(upddn == MENU1_RTC_UP_BT)		meun_month++;
		else if(upddn == MENU1_RTC_DN_BT)	meun_month--;
		if(meun_month > 12) meun_month = 1;
		else if(meun_month < 1)  meun_month = 12;
	}
	else if(day_fg == 1)
	{
		if(upddn == MENU1_RTC_UP_BT)		meun_day++;
		else if(upddn == MENU1_RTC_DN_BT)	meun_day--;
		if(meun_day > 31) meun_day = 1;
		else if(meun_day < 1)  meun_day = 31;
	}
	else if(hour_fg == 1)
	{
		if(upddn == MENU1_RTC_UP_BT)		meun_hour++;
		else if(upddn == MENU1_RTC_DN_BT)	meun_hour--;
		if(meun_hour > 23) meun_hour = 0;
		else if(meun_hour < 0)  meun_hour = 23;
	}
	else if(min_fg == 1)
	{
		if(upddn == MENU1_RTC_UP_BT)		meun_min++;
		else if(upddn == MENU1_RTC_DN_BT)	meun_min--;
		if(meun_min > 59) meun_min = 0;
		else if(meun_min < 0) meun_min = 59;
	}
	menu_setdisp_update(year_fg, month_fg, day_fg, hour_fg, min_fg);		 
}

WM_HWIN CreateSyswin1(void) 
{
    WM_HWIN hWin;    
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}
/*************************** End of file ****************************/
