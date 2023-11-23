#ifndef __GUI_MENUBUTTON__H__
#define __GUI_MENUBUTTON__H__

#include "main.h"

#define MENUSEL_VIEW     				(GUI_ID_USER + 0x00)   
////////////////
#define MENUSEL_BT1					(GUI_ID_USER + 0x10)   
#define MENUSEL_BT2     				(GUI_ID_USER + 0x11)   
#define MENUSEL_BT3					(GUI_ID_USER + 0x12)   

extern uint8_t menu_sel_flag;
WM_HWIN CreateMenuButton(void);

#endif //__GUI_MAIN_WINDOW__H__
