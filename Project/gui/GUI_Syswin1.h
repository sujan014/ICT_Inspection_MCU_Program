#ifndef __GUI_SYSWIN1__H__
#define __GUI_SYSWIN1__H__
   
#include "main.h"

// Menu Menu window setting
#define MENU1_VIEW     		(GUI_ID_USER + 0x00)    
////////////
#define MENU1_TIME_EGDE		(GUI_ID_USER + 0x01) 
#define MENU1_TIME_TEXT		(GUI_ID_USER + 0x02)     
#define MENU1_RTC_TEXT		(GUI_ID_USER + 0x03)     
////////////
#define MENU1_YEAR_SEL		(GUI_ID_USER + 0x04)     
#define MENU1_MON_SEL		(GUI_ID_USER + 0x05)     
#define MENU1_DAY_SEL		(GUI_ID_USER + 0x06)     
#define MENU1_WEEK_SEL		(GUI_ID_USER + 0x07)     
#define MENU1_HOUR_SEL		(GUI_ID_USER + 0x08)     
#define MENU1_MIN_SEL		(GUI_ID_USER + 0x09)     
#define MENU1_RTC_UP_BT		(GUI_ID_USER + 0x0A)     
#define MENU1_RTC_DN_BT		(GUI_ID_USER + 0x0B)     
#define MENU1_RTC_SET_BT	(GUI_ID_USER + 0x0C)     
#define MENU1_RTC_RD_BT  	(GUI_ID_USER + 0x0D)    	
////////////
#define MENU1_ICT_DN_BT		(GUI_ID_USER + 0x10)    
#define MENU1_TSP_DN_BT		(GUI_ID_USER + 0x11)    
#define MENU1_DATA_DN_BT	(GUI_ID_USER + 0x12)    
////////////
#define MENU1_ICT_ST_TEXT	(GUI_ID_USER + 0x13)    
#define MENU1_TSP_ST_TEXT	(GUI_ID_USER + 0x14)    
#define MENU1_DATA_ST_TEXT	(GUI_ID_USER + 0x15)  
//--------------------------------------------LGD
#define MENU1_PMIC_DN_BT	(GUI_ID_USER + 0x16) 
#define MENU1_EEP_DN_BT		(GUI_ID_USER + 0x17) 
#define MENU1_PMIC_ST_TEXT	(GUI_ID_USER + 0x18)    
#define MENU1_EEP_ST_TEXT	(GUI_ID_USER + 0x19)   
//----------------------------------------------
////////////////
#define MENU1_POPUP0		(GUI_ID_USER + 0x20)  



void Sys1Popup(uint8_t flag, GUI_COLOR Color, char msg[100]);
void menu_gettime_update(void);
void menu_settime_update(int meun_year, int meun_month, int meun_day, int meun_hour, int meun_min);
WM_HWIN CreateSyswin1(void);

#endif //__GUI_MAIN_WINDOW__H__
