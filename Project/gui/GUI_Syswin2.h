#ifndef __GUI_SYSWIN2__H__
#define __GUI_SYSWIN2__H__
   
#include "main.h"

#define MENU2_VIEW			(GUI_ID_USER + 0)    

#define MENU2_NG_CONTI		(GUI_ID_USER + 1)  
#define MENU2_FW_WRITE		(GUI_ID_USER + 2)
#define MENU2_LOG_SDC		(GUI_ID_USER + 3)   
#define MENU2_LOG_UNIT		(GUI_ID_USER + 4)  
#define MENU2_SUBPRO_DISP	(GUI_ID_USER + 5)  
#define MENU2_SHORT_ALL		(GUI_ID_USER + 6)    

#define MENU2_FULL_WRITE	(GUI_ID_USER + 7)    

#define MENU2_POPUP0		(GUI_ID_USER + 10)  

WM_HWIN CreateSyswin2(void);

#endif //__GUI_MAIN_WINDOW__H__
