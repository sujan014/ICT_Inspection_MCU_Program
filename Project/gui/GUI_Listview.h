#ifndef __GUI_LISTVIEW__H__
#define __GUI_LISTVIEW__H__
   
#include "main.h"

#define MAIN_WINDOW   	(GUI_ID_USER + 0)
#define MAIN_LISTVIEW	(GUI_ID_USER + 1)
#define MAIN_POPUP    	(GUI_ID_USER + 2)

//Disp ListView
#define PASS_RESULT true
#define FAIL_RESULT false
#define SKIP_RESULT 0x53
#define BUSY_RESULT 0x55
#define USB_WRITE 	0x56 //210516 MCH
#define USB_VERIFY 	0x57 //210516 MCH


WM_HWIN CreateMainListview(void);
void ListView_ITEM(char *Item);
void ListView_MEAS(char *Meas);
void ListView_RESULT(uint8_t Flag, uint8_t mode);
void ListView_CLEAR(void);
void MainPopup(uint8_t flag, GUI_COLOR Color, char msg[100]);
void MainPopupTrans(uint8_t flag, GUI_COLOR Color, char msg[100]);
void MainPopupResult(uint8_t flag, uint8_t rst, char msg[100]);


#endif //__GUI_MAIN_WINDOW__H__
