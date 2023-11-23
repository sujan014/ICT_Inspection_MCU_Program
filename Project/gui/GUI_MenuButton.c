#include "GUI_MenuButton.h" 

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
    { FRAMEWIN_CreateIndirect, "", MENUSEL_VIEW,    0,   28, 480,  242, 0, 0x0, 0 }, 	
    { BUTTON_CreateIndirect,   "", MENUSEL_BT1, 5,   15, 150,  200, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "", MENUSEL_BT2,  165, 15, 150,  200, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,   "", MENUSEL_BT3, 325, 15, 150,  200, 0, 0x0, 0 },	
};

uint8_t menu_sel_flag = 0;
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    int  NCode, Id;
   
    switch (pMsg->MsgId) 
    {
		case WM_INIT_DIALOG:      		
			hItem = pMsg->hWin;
			FRAMEWIN_SetTitleVis(hItem, 0);     
			FRAMEWIN_SetBorderSize(hItem, 0);   
			FRAMEWIN_SetClientColor(hItem, GUI_BLACK);		

			hItem = WM_GetDialogItem(pMsg->hWin, MENUSEL_BT1);	
			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);
			BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII );
			BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			BUTTON_SetText(hItem, "System Menu");
				
			hItem = WM_GetDialogItem(pMsg->hWin, MENUSEL_BT2);	
			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);
			BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII );
			BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			BUTTON_SetText(hItem, "TBD");
				
			hItem = WM_GetDialogItem(pMsg->hWin, MENUSEL_BT3);	
			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);
			BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII );
			BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			BUTTON_SetText(hItem, "Bootloader\nMode Change");
		break;

		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{		
				case MENUSEL_BT1:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							menu_sel_flag = 1;													
						break;
					}
				break;
				case MENUSEL_BT2:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							//menu_sel_flag = 2;												
						break;
					}
				break;
				case MENUSEL_BT3:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:
							Beep(1);
							menu_sel_flag = 3;												
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
	
WM_HWIN CreateMenuButton(void) 
{
    WM_HWIN hWin;    
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}
/*************************** End of file ****************************/
