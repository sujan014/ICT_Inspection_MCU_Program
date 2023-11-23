#ifndef __GUI_SYSICON__H__
#define __GUI_SYSICON__H__
   
#include "main.h"

#define MENU_MAIN		 (GUI_ID_USER + 0x00)		
#define MENU_BT1		 (GUI_ID_USER + 0x01)
#define MENU_BT2		 (GUI_ID_USER + 0x02)
#define MENU_BT3		 (GUI_ID_USER + 0x03)
#define MENU_BT4		 (GUI_ID_USER + 0x04)

extern char menu_win_flag;
WM_HWIN CreateSysIcon(void);

#endif //__GUI_MAIN_WINDOW__H__
