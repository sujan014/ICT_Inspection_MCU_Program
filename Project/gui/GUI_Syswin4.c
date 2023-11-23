#include "GUI_Syswin4.h" 

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
    { FRAMEWIN_CreateIndirect, "page4", MENU4_VIEW,     		 75,	28,		 405, 	242, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect, 		"", MENU4_EVCC1_L,	 	 5,	 5,		 95,	20,		0,	 0x0,	 0 },  
    { TEXT_CreateIndirect, 		"", MENU4_EVCC1,		105,	 5,		 95,	20,		0,	 0x0,	 0 },   
    { TEXT_CreateIndirect, 		"", MENU4_EVCC2_L,			205,	 5,		 95,	20,		0,	 0x0,	 0 },   
    { TEXT_CreateIndirect, 		"", MENU4_EVCC2,			305,	 5,		 95,	20,		0,	 0x0,	 0 },   
};
#define MENU4_EVCC1_L	(GUI_ID_USER + 1)    
#define MENU4_EVCC1		(GUI_ID_USER + 2)
#define MENU4_EVCC2_L	(GUI_ID_USER + 3)    
#define MENU4_EVCC2		(GUI_ID_USER + 4) 

static void _cbDialog(WM_MESSAGE * pMsg) 
{
//    int     NCode;
//    int     Id;
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
            hItem = pMsg->hWin;
            FRAMEWIN_SetTitleVis(hItem, 0);     
			FRAMEWIN_SetBorderSize(hItem, 0);    
            FRAMEWIN_SetClientColor(hItem, GUI_BLACK);	 
            
			hItem = WM_GetDialogItem(pMsg->hWin,MENU4_EVCC1_L);		
			TEXT_SetText(hItem,"VCC1");			
			TEXT_SetTextAlign(hItem,TEXT_CF_HCENTER|TEXT_CF_VCENTER);	
			TEXT_SetTextColor(hItem, GUI_WHITE);	
			TEXT_SetBkColor(hItem, GUI_DARKBLUE);
            
			hItem = WM_GetDialogItem(pMsg->hWin,MENU4_EVCC2_L);			
			TEXT_SetText(hItem,"VCC2");			
			TEXT_SetTextAlign(hItem,TEXT_CF_HCENTER|TEXT_CF_VCENTER);	
			TEXT_SetTextColor(hItem, GUI_WHITE);	
			TEXT_SetBkColor(hItem, GUI_DARKBLUE);
		
			hItem = WM_GetDialogItem(pMsg->hWin,MENU4_EVCC1);	
			TEXT_SetText(hItem,"");	
			TEXT_SetTextAlign(hItem,TEXT_CF_HCENTER|TEXT_CF_VCENTER);	
			TEXT_SetTextColor(hItem, GUI_BLACK);	
			TEXT_SetBkColor(hItem, GUI_WHITE);
			
            hItem = WM_GetDialogItem(pMsg->hWin,MENU4_EVCC2);	
			TEXT_SetText(hItem,"");	TEXT_SetTextAlign(hItem,TEXT_CF_HCENTER|TEXT_CF_VCENTER);	
			TEXT_SetTextColor(hItem, GUI_BLACK);	
			TEXT_SetBkColor(hItem, GUI_WHITE);
		
		break;
        case WM_NOTIFY_PARENT:
			
        break;
        
        default:
            WM_DefaultProc(pMsg);
        break;
    }
}
WM_HWIN CreateSyswin4(void) 
{
    WM_HWIN hWin;    
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}
/*************************** End of file ****************************/
