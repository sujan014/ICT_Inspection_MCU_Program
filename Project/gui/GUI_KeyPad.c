#include "GUI_KeyPad.h" 

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
    { FRAMEWIN_CreateIndirect, "" ,    KEY_WINDOW,      0,   28,  480, 242, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "1",    KEY1_BUTTON,    30,   25,  140,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "2",    KEY2_BUTTON,   170,   25,  140,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "3",    KEY3_BUTTON,   310,   25,  140,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "4",    KEY4_BUTTON,    30,   75,  140,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "5",    KEY5_BUTTON,   170,   75,  140,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "6",    KEY6_BUTTON,   310,   75,  140,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "7",    KEY7_BUTTON,    30,  125,  140,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "8",    KEY8_BUTTON,   170,  125,  140,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "9",    KEY9_BUTTON,   310,  125,  140,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "0",    KEY0_BUTTON,   170,  175,  140,  50, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,   "-",    KEYA_BUTTON,    30,  175,   70,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   ".",    KEYB_BUTTON,   100,  175,   70,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "DEL",  KEYC_BUTTON,   310,  175,   70,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "OK",   KEYD_BUTTON,   380,  175,   70,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,     "",     KEY_TEXT_1,      0,    0,  480,  25, 0, 0x0, 0 },
    { TEXT_CreateIndirect,     "",     KEY_POPUP,     50,   20,  380, 174, 0, 0x0, 0 },
};

int pw_length = 4;
char InputKey[16]	= {NULL};
uint8_t Keypad_flag = 0;

int PASSWORD_FLAG 	= PASSWORD_NG;
const char PASSWORD_MST[4] = {'6','2','6','5'};
const char PASSWORD_DBG[4] = {'6','1','7','5'};


static void _cbDialog(WM_MESSAGE * pMsg) 
{
    int     NCode;
    int     Id;
	char text[10];

    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
            hItem = pMsg->hWin;
			FRAMEWIN_SetTitleVis(hItem, 0);  
			FRAMEWIN_SetBorderSize(hItem, 0);
            FRAMEWIN_SetClientColor(hItem, GUI_BLACK);	    
			
			hItem = WM_GetDialogItem(pMsg->hWin, KEY_TEXT_1); 
			TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
			TEXT_SetTextAlign(hItem,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
			TEXT_SetText(hItem,"Please Enter The 4-Digit Passcode");
			TEXT_SetBkColor(hItem, GUI_BLACK);
			TEXT_SetTextColor(hItem, GUI_WHITE);

			hItem = WM_GetDialogItem(pMsg->hWin, KEY_POPUP);  
			TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
			TEXT_SetTextColor(hItem, GUI_RED);
			TEXT_SetBkColor(hItem, GUI_LIGHTYELLOW);
			TEXT_SetTextAlign(hItem,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
			TEXT_SetText(hItem," "); 				
			WM_SetStayOnTop(hItem,0);  
			WM_HideWindow(hItem);
			
			for(int i = 0; i<14;i++)
			{
				hItem = WM_GetDialogItem(pMsg->hWin, KEY0_BUTTON + i); 	
				BUTTON_SetFont(hItem, GUI_FONT_24B_ASCII);	
				BUTTON_SetTextAlign(hItem,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
				
				if(i==10)		
				{
					sprintf(text,"-");
					BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_GRAY);	
					BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
				}
				else if(i==11)	
				{
					sprintf(text,".");
					BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_GRAY);	
					BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
				}
				else if(i==12)	
				{
					sprintf(text,"DEL");
					BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_RED);	
					BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
				}
				else if(i==13)	
				{
					sprintf(text,"O K");
					BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLUE);	
					BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);		
				}
				else			
				{
					BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);	
					BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
					sprintf(text,"%d",i);
				}				
				BUTTON_SetText(hItem, text);
			}			
        break;
        case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
			int key_err;
			int dbg_err;
		
			switch(Id)
			{
				case KEYD_BUTTON: // 	OK
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:	
							Beep(1);
							key_err = dbg_err = pw_length;
							Keypad_flag = 0;
							for(int i = 0; i < pw_length; i++)
							{
								if(InputKey[i] == PASSWORD_MST[i])	key_err--;	
								if(InputKey[i] == PASSWORD_DBG[i])	dbg_err--;	
								InputKey[i] = 0;
							}
															
							if(key_err == 0)		PASSWORD_FLAG = PASSWORD_OK;
							else if(dbg_err == 0)	PASSWORD_FLAG = DBUGMODE_OK;
							else					PASSWORD_FLAG = PASSWORD_NG;
						break;
					}
				break;
				case KEYC_BUTTON: // DEL
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED: 
							Beep(1);
							Keypad_flag = 0;
							for(int i = 0; i< 16; i++)    InputKey[i] = 0;
							hItem = WM_GetDialogItem(hKeyPadwin, KEY_TEXT_1);	
							TEXT_SetText(hItem,"Please Enter Value");
							GUI_Exec(); 								
						break;
					}
				break;
				case KEY0_BUTTON: // 0
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							InputKey[Keypad_flag] = '0';
							if(Keypad_flag++ == pw_length-1)    Keypad_flag = pw_length-1;
							hItem = WM_GetDialogItem(hKeyPadwin, KEY_TEXT_1);
							TEXT_SetText(hItem,InputKey);
							GUI_Exec(); 							
						break;
					}
				break;
				case KEY1_BUTTON: // 1
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							InputKey[Keypad_flag] = '1';
							if(Keypad_flag++ == pw_length-1)    Keypad_flag = pw_length-1;
							hItem = WM_GetDialogItem(hKeyPadwin, KEY_TEXT_1);
							TEXT_SetText(hItem,InputKey);
							GUI_Exec(); 							
						break;
					}
				break;
				case KEY2_BUTTON: // 2
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							InputKey[Keypad_flag] = '2';
							if(Keypad_flag++ == pw_length-1)    Keypad_flag = pw_length-1;
							hItem = WM_GetDialogItem(hKeyPadwin, KEY_TEXT_1);
							TEXT_SetText(hItem,InputKey);
							GUI_Exec(); 							
						break;
					}
				break;
				case KEY3_BUTTON: // 3
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							InputKey[Keypad_flag] = '3';
							if(Keypad_flag++ == pw_length-1)    Keypad_flag = pw_length-1;
							hItem = WM_GetDialogItem(hKeyPadwin, KEY_TEXT_1);
							TEXT_SetText(hItem,InputKey);
							GUI_Exec(); 							
						break;
					}
				break;    
				 case KEY4_BUTTON: // 4
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							InputKey[Keypad_flag] = '4';
							if(Keypad_flag++ == pw_length-1)    Keypad_flag = pw_length-1;
							hItem = WM_GetDialogItem(hKeyPadwin, KEY_TEXT_1);
							TEXT_SetText(hItem,InputKey);
							GUI_Exec(); 							
						break;
					}
				break;                   
				 case KEY5_BUTTON: // 5
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							InputKey[Keypad_flag] = '5';
							if(Keypad_flag++ == pw_length-1)    Keypad_flag = pw_length-1;
							hItem = WM_GetDialogItem(hKeyPadwin, KEY_TEXT_1);
							TEXT_SetText(hItem,InputKey);
							GUI_Exec(); 							
						break;
					}
				break;                   
				case KEY6_BUTTON: // 6
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							InputKey[Keypad_flag] = '6';
							if(Keypad_flag++ == pw_length-1)    Keypad_flag = pw_length-1;
							hItem = WM_GetDialogItem(hKeyPadwin, KEY_TEXT_1);
							TEXT_SetText(hItem,InputKey);
							GUI_Exec(); 							
						break;
					}
				break;                    
				case KEY7_BUTTON: // 7
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							InputKey[Keypad_flag] = '7';
							if(Keypad_flag++ == pw_length-1)    Keypad_flag = pw_length-1;
							hItem = WM_GetDialogItem(hKeyPadwin, KEY_TEXT_1);
							TEXT_SetText(hItem,InputKey);
							GUI_Exec(); 							
						break;
					}
				break;                    
				case KEY8_BUTTON: // 8
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							InputKey[Keypad_flag] = '8';
							if(Keypad_flag++ == pw_length-1)    Keypad_flag = pw_length-1;
							hItem = WM_GetDialogItem(hKeyPadwin, KEY_TEXT_1);
							TEXT_SetText(hItem,InputKey);
							GUI_Exec(); 							
						break;
					}
				break;
				case KEY9_BUTTON: // 9
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							InputKey[Keypad_flag] = '9';
							if(Keypad_flag++ == pw_length-1)    Keypad_flag = pw_length-1;
							hItem = WM_GetDialogItem(hKeyPadwin, KEY_TEXT_1);
							TEXT_SetText(hItem,InputKey);
							GUI_Exec(); 					
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

void Keypad_POPUP(uint8_t flag, GUI_COLOR Color, char msg[100])
{
    hItem = WM_GetDialogItem(hKeyPadwin, KEY_POPUP);
    if( flag == true ) 
    {
        TEXT_SetBkColor(hItem, GUI_BLACK); 
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

WM_HWIN CreateKeyPad(void) 
{
    WM_HWIN hWin;    
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}
/*************************** End of file ****************************/
