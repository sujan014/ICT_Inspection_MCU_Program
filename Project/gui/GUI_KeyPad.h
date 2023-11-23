#ifndef __GUI_KEYPAD__H__
#define __GUI_KEYPAD__H__

#include "main.h"

//PASSWORD WINDOW
#define KEY_WINDOW    (GUI_ID_USER + 0)
#define KEY_TEXT_1    (GUI_ID_USER + 1) 
#define KEY_POPUP     (GUI_ID_USER + 2) 

#define KEY0_BUTTON   (GUI_ID_USER + 10)
#define KEY1_BUTTON   (GUI_ID_USER + 11)
#define KEY2_BUTTON   (GUI_ID_USER + 12)
#define KEY3_BUTTON   (GUI_ID_USER + 13)
#define KEY4_BUTTON   (GUI_ID_USER + 14)
#define KEY5_BUTTON   (GUI_ID_USER + 15)
#define KEY6_BUTTON   (GUI_ID_USER + 16)
#define KEY7_BUTTON   (GUI_ID_USER + 17)
#define KEY8_BUTTON   (GUI_ID_USER + 18)
#define KEY9_BUTTON   (GUI_ID_USER + 19)
#define KEYA_BUTTON   (GUI_ID_USER + 20)
#define KEYB_BUTTON   (GUI_ID_USER + 21)
#define KEYC_BUTTON   (GUI_ID_USER + 22)
#define KEYD_BUTTON   (GUI_ID_USER + 23)

#define PASSWORD_RD   255
#define PASSWORD_NG   0
#define PASSWORD_OK   1
#define DBUGMODE_OK   2

extern char InputKey[16];
extern int PASSWORD_FLAG;

void Keypad_POPUP(uint8_t flag, GUI_COLOR Color, char msg[100]);
WM_HWIN CreateKeyPad(void);

#endif //__GUI_MAIN_WINDOW__H__
