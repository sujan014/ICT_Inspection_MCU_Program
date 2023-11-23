#ifndef __GUI_SYSWIN3__H__
#define __GUI_SYSWIN3__H__
   
#include "main.h"

#define MENU_SPEC       (GUI_ID_USER + 0x00)    // Mene window    

#define SPEC_LIST       (GUI_ID_USER + 0x01)    // SPEC ListView
#define LIST_TP   	    (GUI_ID_USER + 0x02)    // SPEC ListView
#define LIST_OPen       (GUI_ID_USER + 0x03)    // SPEC ListView
#define LIST_SHbs       (GUI_ID_USER + 0x04)    // SPEC ListView
#define LIST_SHex       (GUI_ID_USER + 0x05)    // SPEC ListView
#define LIST_Part       (GUI_ID_USER + 0x06)    // SPEC ListView
#define LIST_Func       (GUI_ID_USER + 0x07)    // SPEC ListView

#define LIST_SEQ       	(GUI_ID_USER + 0x08)    // SPEC ListView

#define NEXT_BT         (GUI_ID_USER + 0x10)    // NEXT BT
#define PREV_BT         (GUI_ID_USER + 0x11)    // PREV BT
#define PART_BT         (GUI_ID_USER + 0x12)    // PART View
#define TP_BT           (GUI_ID_USER + 0x13)    // TP View
#define OPEN_BT			(GUI_ID_USER + 0x14)    // OPEN View
#define SH_BS_BT		(GUI_ID_USER + 0x15)    // Short View
#define SH_EX_BT		(GUI_ID_USER + 0x16)    // Short skip View
#define FUNC_BT			(GUI_ID_USER + 0x17)    // Short skip View
#define MODE1_BT		(GUI_ID_USER + 0x18)    // Short skip View
#define MODE2_BT		(GUI_ID_USER + 0x19)    // Short skip View
#define MODE3_BT		(GUI_ID_USER + 0x1A)    // Short skip View
#define MODE4_BT		(GUI_ID_USER + 0x1B)    // Short skip View

#define SPEC_POPUP		(GUI_ID_USER + 0xAA)    // Pop-up

WM_HWIN CreateSyswin3(void);

#endif //__GUI_MAIN_WINDOW__H__
