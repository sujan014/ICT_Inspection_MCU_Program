#ifndef __GUI_INIT__H__
#define __GUI_INIT__H__
   
#include "main.h" 

#define MAIN_PAGE  		0
#define MENUBT_PAGE  	1
#define KEYPAD_PAGE  	2
#define MENU_PAGE1		3
#define MENU_PAGE2		4
#define MENU_PAGE3		5
#define MENU_PAGE4		6

extern WM_HWIN hItem;
extern WM_HWIN hListview;
extern WM_HWIN hStatewin;
extern WM_HWIN hMenuBtwin;
extern WM_HWIN hKeyPadwin;
extern WM_HWIN hSysicon;
extern WM_HWIN hSyswin1;	
extern WM_HWIN hSyswin2;
extern WM_HWIN hSyswin3;
extern WM_HWIN hSyswin4;

extern int meun_year, meun_month, meun_day, meun_week, meun_hour, meun_min, meun_sec; 
extern uint8_t year_flag, month_flag, day_flag, week_flag, hour_flag, min_flag;

void LogoDisplay(const GUI_BITMAP GUI_UNI_PTR *pBM, int disptime);
void ProjectDisplay(void);
void LcdDisplayInit(void);
void MenuChange(void);
void DispSelect(uint8_t page);
uint8_t TouchSensing(void);

#endif //__GUI_MAIN_WINDOW__H__
