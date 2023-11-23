#include "GUI_Sysicon.h" 

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
    { FRAMEWIN_CreateIndirect, "", MENU_MAIN,  0,  28,   75, 242, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "", MENU_BT1,   4,   2,   68,  55, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "", MENU_BT2,   4,  62,   68,  55, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "", MENU_BT3,   4, 122,   68,  55, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "", MENU_BT4,   4, 182,   68,  55, 0, 0x0, 0 },
};

char menu_win_flag = 0;
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    int     NCode;
    int     Id;
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
            hItem = pMsg->hWin;
			FRAMEWIN_SetTitleVis(hItem, 0);  
			FRAMEWIN_SetBorderSize(hItem, 0);
            FRAMEWIN_SetClientColor(hItem, GUI_BLACK);	 
            
			hItem = WM_GetDialogItem(pMsg->hWin, MENU_BT1);	
			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);
			BUTTON_SetFont(hItem, GUI_FONT_16_ASCII );
			BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			BUTTON_SetText(hItem, "Menu\nSetting1");
			
			hItem = WM_GetDialogItem(pMsg->hWin, MENU_BT2);	
			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);
			BUTTON_SetFont(hItem, GUI_FONT_16_ASCII );
			BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			BUTTON_SetText(hItem, "Menu\nSetting2");
			
			hItem = WM_GetDialogItem(pMsg->hWin, MENU_BT3);	
			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);
			BUTTON_SetFont(hItem, GUI_FONT_16_ASCII );
			BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			BUTTON_SetText(hItem, "Spec\nView");
		
			hItem = WM_GetDialogItem(pMsg->hWin, MENU_BT4);	
			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);
			BUTTON_SetFont(hItem, GUI_FONT_16_ASCII );
			BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			BUTTON_SetText(hItem, "TBD");
		break;
        case WM_NOTIFY_PARENT:			
			Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch(Id) 
            {
                case MENU_BT1:
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_RELEASED:
							Beep(1);
							menu_win_flag = 0;
                        break;
                    }
                break;
                case MENU_BT2:
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_RELEASED:
							Beep(1);
							menu_win_flag = 1;
                        break;
                    }
                break;
                case MENU_BT3:
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_RELEASED:
							Beep(1);
							menu_win_flag = 2;
                        break;
                    }
                break;
                case MENU_BT4:
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_RELEASED:
							Beep(1);
							menu_win_flag = 3;
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
WM_HWIN CreateSysIcon(void) 
{
    WM_HWIN hWin;    
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}
/*************************** End of file ****************************/
