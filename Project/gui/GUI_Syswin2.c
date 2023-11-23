#include "GUI_Syswin2.h" 

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
    { FRAMEWIN_CreateIndirect, "page2", MENU2_VIEW,			75,   28,	405,   242,	0, 	0x0, 	0 },  	
    { CHECKBOX_CreateIndirect,  "	",	MENU2_NG_CONTI,			10,   20,	200,    20,	0,	0x0,	0 },
    { CHECKBOX_CreateIndirect,  "	",	MENU2_FW_WRITE,			10,   50,	200,    20,	0,	0x0,	0 },
	{ CHECKBOX_CreateIndirect,  "	",	MENU2_FULL_WRITE,		200,   50,	200,    20,	0,	0x0,	0 },
   //{ CHECKBOX_CreateIndirect,  "	",	MENU2_LOG_SDC,			10,   80,	200,    20,	0,	0x0,	0 }, 210514 MCH
    /*{ CHECKBOX_CreateIndirect,  "	",	MENU2_LOG_UNIT,			10,   110,	200,    20,	0,	0x0,	0 },
    { CHECKBOX_CreateIndirect,  "	",	MENU2_SUBPRO_DISP,		10,   140,	200,    20,	0,	0x0,	0 },
    { CHECKBOX_CreateIndirect,  "	",	MENU2_SHORT_ALL,		10,   170,	200,    20,	0,	0x0,	0 },
	{ TEXT_CreateIndirect,      "	",	MENU2_POPUP0,			 1,     0,	405,   242,	0,	0x0,	0 },*/
	{ CHECKBOX_CreateIndirect,  "	",	MENU2_LOG_UNIT,			10,   80,	200,    20,	0,	0x0,	0 },
    { CHECKBOX_CreateIndirect,  "	",	MENU2_SUBPRO_DISP,		10,   110,	200,    20,	0,	0x0,	0 },
	{ TEXT_CreateIndirect,      "	",	MENU2_POPUP0,			 1,     0,	405,   242,	0,	0x0,	0 },
};

static void _cbDialog(WM_MESSAGE * pMsg) 
{
	char 	temp = 0;
    int     NCode;
    int     Id;
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
					
            hItem = pMsg->hWin;
            FRAMEWIN_SetTitleVis(hItem, 0);     
			FRAMEWIN_SetBorderSize(hItem, 0);    
            FRAMEWIN_SetClientColor(hItem, GUI_BLACK);	
            
			hItem = WM_GetDialogItem(pMsg->hWin, MENU2_NG_CONTI);	
			CHECKBOX_SetState(hItem, RD_NG_CONTINUE); 	
			CHECKBOX_SetText(hItem,"Continue inspection at NG");			
			CHECKBOX_SetBkColor(hItem, GUI_BLACK); 
			CHECKBOX_SetTextColor (hItem, GUI_WHITE);
		
			hItem = WM_GetDialogItem(pMsg->hWin, MENU2_FW_WRITE);	
			CHECKBOX_SetState(hItem, RD_FW_WRITE); 	
			CHECKBOX_SetText(hItem,"Enable F/W Write");			
			CHECKBOX_SetBkColor(hItem, GUI_BLACK);
			CHECKBOX_SetTextColor (hItem, GUI_WHITE);
		
			hItem = WM_GetDialogItem(pMsg->hWin, MENU2_FULL_WRITE);	
			CHECKBOX_SetState(hItem, RD_FULL_WRITE); 	
			CHECKBOX_SetText(hItem,"Boot+App Write");			
			CHECKBOX_SetBkColor(hItem, GUI_BLACK);
			CHECKBOX_SetTextColor (hItem, GUI_WHITE);
			
			hItem = WM_GetDialogItem(pMsg->hWin, MENU2_LOG_SDC);	
			CHECKBOX_SetState(hItem, RD_SDC_SAVE); 	
			CHECKBOX_SetText(hItem,"Save log for SDC");			
			CHECKBOX_SetBkColor(hItem, GUI_BLACK);
			CHECKBOX_SetTextColor (hItem, GUI_WHITE);
		
		
			hItem = WM_GetDialogItem(pMsg->hWin, MENU2_LOG_UNIT);	
			CHECKBOX_SetState(hItem, RD_LOG_UNIT); 	
			CHECKBOX_SetText(hItem,"Save log with Unit");			
			CHECKBOX_SetBkColor(hItem, GUI_BLACK);
			CHECKBOX_SetTextColor (hItem, GUI_WHITE);
		
		
			hItem = WM_GetDialogItem(pMsg->hWin, MENU2_SUBPRO_DISP);	
			CHECKBOX_SetState(hItem, RD_SUB_DISP); 	
			CHECKBOX_SetText(hItem,"Display data file name in subarea");			
			CHECKBOX_SetBkColor(hItem, GUI_BLACK);
			CHECKBOX_SetTextColor (hItem, GUI_WHITE);
			

			hItem = WM_GetDialogItem(pMsg->hWin, MENU2_SHORT_ALL);	
			CHECKBOX_SetState(hItem, RD_SHORT_ALL); 	
			CHECKBOX_SetText(hItem,"Short Test All Pin");			
			CHECKBOX_SetBkColor(hItem, GUI_BLACK);
			CHECKBOX_SetTextColor (hItem, GUI_WHITE);	
		
            hItem = WM_GetDialogItem(pMsg->hWin, MENU2_POPUP0); 
			TEXT_SetFont(hItem, GUI_FONT_32B_ASCII);      
            TEXT_SetBkColor(hItem, GUI_TRANSPARENT); 
			TEXT_SetTextColor(hItem, GUI_GREEN); 
			TEXT_SetText(hItem," ");   
			TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER|TEXT_CF_VCENTER);  
            WM_SetStayOnTop(hItem,0); 
			WM_HideWindow(hItem);
			
		break;
        case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch(Id) 
            {			
				case MENU2_NG_CONTI:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
							temp = !RD_NG_CONTINUE;
							WR_NG_CONTINUE(temp);
							Beep(1);
							CHECKBOX_SetState(hItem, RD_NG_CONTINUE); 	
							GUI_Exec();						
							ProjectDisplay();							
						break;
                    }
                break;	
				case MENU2_FW_WRITE:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
							temp = !RD_FW_WRITE;
							WR_FW_WRITE(temp);	
							Beep(1);
							CHECKBOX_SetState(hItem, RD_FW_WRITE); 
							GUI_Exec();							
							ProjectDisplay();	
						break;
                    }
                break;		
				case MENU2_LOG_SDC:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
							temp = !RD_SDC_SAVE;
							WR_SDC_SAVE(temp);	
							Beep(1);
							CHECKBOX_SetState(hItem, RD_SDC_SAVE); 
							GUI_Exec();							
							ProjectDisplay();	
						break;
                    }
                break;	
				case MENU2_LOG_UNIT:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:		
							temp = !RD_LOG_UNIT;
							WR_LOG_UNIT(temp);	
							Beep(1);
							CHECKBOX_SetState(hItem, RD_LOG_UNIT); 
							GUI_Exec();						
							ProjectDisplay();	
						break;
                    }
                break;
				case MENU2_SUBPRO_DISP:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
							temp = !RD_SUB_DISP;
							WR_SUB_DISP(temp);	
							Beep(1);
							CHECKBOX_SetState(hItem, RD_SUB_DISP); 	
							GUI_Exec();						
							ProjectDisplay();	
						break;
                    }
                break;
				case MENU2_SHORT_ALL:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
							temp = !RD_SHORT_ALL;
							WR_SHORT_ALL(temp);	
							Beep(1);
							CHECKBOX_SetState(hItem, RD_SHORT_ALL); 	
							GUI_Exec();						
							ProjectDisplay();	
						break;
                    }
                break;
				case MENU2_FULL_WRITE:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
							temp = !RD_FULL_WRITE;
							WR_FULL_WRITE(temp);	
							Beep(1);
							CHECKBOX_SetState(hItem, RD_FULL_WRITE); 	
							GUI_Exec();						
							ProjectDisplay();	
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
WM_HWIN CreateSyswin2(void) 
{
    WM_HWIN hWin;    
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}
/*************************** End of file ****************************/
