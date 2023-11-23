#include "GUI_State.h" 

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
    { FRAMEWIN_CreateIndirect, "",	STATE_WINDOW, 370,  28, 110, 242, 0, 0x64,   0 },

    { RADIO_CreateIndirect,    "",	STATE_RADIO,	2,   2, 106,  80, 0, 0x1504, 0 },  

    { TEXT_CreateIndirect,     "",	STATE_USB,		2,  85, 106,  15, 0, 0x0, 0 },
    { TEXT_CreateIndirect,     "",	STATE_PC,		2, 102, 106,  15, 0, 0x0, 0 },
    { TEXT_CreateIndirect,     "",	STATE_LABEL3,	2, 119, 106,  15, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,     "",	STATE_QTY,		2, 137, 106,  20, 0, 0x0, 0 },
    { TEXT_CreateIndirect,     "",	STATE_OK,		2, 159, 106,  20, 0, 0x0, 0 },
    { TEXT_CreateIndirect,     "",	STATE_NG,		2, 181, 106,  20, 0, 0x0, 0 },
    { TEXT_CreateIndirect,     "",	STATE_TACT,		2, 203, 106,  20, 0, 0x0, 0 },
    { TEXT_CreateIndirect,     "",	STATE_FW,		2, 226, 106,  14, 0, 0x0, 0 },
};

static void _cbDialog(WM_MESSAGE * pMsg) 
{
    int     NCode;
    int     Id;
	char text[50];
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
            
            hItem = pMsg->hWin;
            FRAMEWIN_SetTitleVis(hItem, 0);  
            FRAMEWIN_SetBorderSize  (hItem, 0);
            FRAMEWIN_SetClientColor (hItem, GUI_DARKGRAY);
            
            hItem = WM_GetDialogItem(pMsg->hWin, STATE_RADIO);
				
			RADIO_SetText(hItem, mode1_name, 0); 
			RADIO_SetText(hItem, mode2_name, 1);
			RADIO_SetText(hItem, mode3_name, 2);  
			RADIO_SetText(hItem, mode4_name, 3);
				
            RADIO_SetBkColor(hItem, GUI_BLACK); 
			RADIO_SetFocusColor(hItem, GUI_WHITE); 
			RADIO_SetTextColor(hItem, GUI_WHITE); 
            RADIO_SetFont(hItem, GUI_FONT_6X9 );
			
            if(RD_MODE ==      0)	{ 	RADIO_SetValue(hItem, 0);			  }
            else if(RD_MODE == 1) 	{ 	RADIO_SetValue(hItem, 1);			  }
            else if(RD_MODE == 2) 	{ 	RADIO_SetValue(hItem, 2);			  }
            else if(RD_MODE == 3) 	{	RADIO_SetValue(hItem, 3);			  }
			else					{ 	RADIO_SetValue(hItem, 0); WR_MODE(0); }
        		
            hItem = WM_GetDialogItem(pMsg->hWin, STATE_USB);    
			TEXT_SetBkColor(hItem, GUI_BLACK); 
			TEXT_SetTextColor(hItem, GUI_WHITE); 
            TEXT_SetFont(hItem, GUI_FONT_6X9 );
			TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER|TEXT_CF_VCENTER);
            TEXT_SetText(hItem,"USB( 00.00% )"); 
            
            hItem = WM_GetDialogItem(pMsg->hWin, STATE_PC);    
			TEXT_SetBkColor(hItem, GUI_BLACK);
			TEXT_SetTextColor(hItem, GUI_WHITE); 
            TEXT_SetFont(hItem, GUI_FONT_6X9 );
			TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER|TEXT_CF_VCENTER);
            TEXT_SetText(hItem,"PC(Disconnect)"); 
			
			hItem = WM_GetDialogItem(pMsg->hWin, STATE_LABEL3);    
			TEXT_SetBkColor(hItem, GUI_BLACK); 
			TEXT_SetTextColor(hItem, GUI_WHITE); 
            TEXT_SetFont(hItem, GUI_FONT_6X9 );
			TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER|TEXT_CF_VCENTER);
			TEXT_SetText(hItem,""); 
             
			QTY.TT = RD_QTY;
            hItem = WM_GetDialogItem(pMsg->hWin, STATE_QTY);    
			TEXT_SetBkColor(hItem, GUI_BLACK); 
			TEXT_SetTextColor(hItem, GUI_WHITE);            
            TEXT_SetFont(hItem, GUI_FONT_6X9 );
			TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER|TEXT_CF_VCENTER);
            sprintf(text,"QTY  :  %6d", RD_QTY);    
			TEXT_SetText(hItem,text);               
             
            QTY.OK = RD_OK;
			hItem = WM_GetDialogItem(pMsg->hWin, STATE_OK);     
			TEXT_SetBkColor(hItem, GUI_BLACK); 
			TEXT_SetTextColor(hItem, GUI_WHITE);
            TEXT_SetFont(hItem, GUI_FONT_6X9 ); 
			TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER|TEXT_CF_VCENTER);
            sprintf(text,"O K  :  %6d", RD_OK);    
			TEXT_SetText(hItem,text);
            
            QTY.NG = RD_NG;
            hItem = WM_GetDialogItem(pMsg->hWin, STATE_NG);   
			TEXT_SetBkColor(hItem, GUI_BLACK); 
			TEXT_SetTextColor(hItem, GUI_WHITE);
            TEXT_SetFont(hItem, GUI_FONT_6X9 );
			TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER|TEXT_CF_VCENTER);
            sprintf(text,"N G  :  %6d", RD_NG);    
			TEXT_SetText(hItem,text);
            
            hItem = WM_GetDialogItem(pMsg->hWin, STATE_TACT);   
			TEXT_SetBkColor(hItem, GUI_BLACK);
			TEXT_SetTextColor(hItem, GUI_WHITE);
            TEXT_SetFont(hItem, GUI_FONT_6X9 ); 
			TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER|TEXT_CF_VCENTER);
			TEXT_SetText(hItem,"0.00 sec");
                                         
			hItem = WM_GetDialogItem(pMsg->hWin, STATE_FW);  
			TEXT_SetBkColor(hItem, GUI_BLACK); 
			TEXT_SetTextColor(hItem, GUI_RED);
			TEXT_SetFont(hItem, GUI_FONT_6X9 );   
			TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER|TEXT_CF_VCENTER);

			if(RD_DBG_MD == 0)	{	sprintf(text,"KM-V%1.2f(D)",sys_ver);	}
			else				{	sprintf(text,"KM-V%1.2f(R)",sys_ver);	}

			TEXT_SetText(hItem,text);

		break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch(Id) 
            {
                case STATE_RADIO: // Notifications sent by 'Radio'
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_VALUE_CHANGED:
							hItem = (WM_GetDialogItem(pMsg->hWin, STATE_RADIO));
                            if(RADIO_GetValue(hItem) == 0)      RADIO_SetValue(hItem,0);
                            else if(RADIO_GetValue(hItem) == 1) RADIO_SetValue(hItem,1);
                            else if(RADIO_GetValue(hItem) == 2) RADIO_SetValue(hItem,2);
                            else if(RADIO_GetValue(hItem) == 3) RADIO_SetValue(hItem,3);
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
void ModeNameUpdate(void)
{
	hItem = WM_GetDialogItem(hStatewin, STATE_RADIO);		
	RADIO_SetText(hItem, mode1_name, 0); 
	RADIO_SetText(hItem, mode2_name, 1);
	RADIO_SetText(hItem, mode3_name, 2);  
	RADIO_SetText(hItem, mode4_name, 3);
}

void InspModeChange(uint8_t mode)
{
	//MainPopup(false, GUI_LIGHTBLUE," ");
	hItem = WM_GetDialogItem(hStatewin, STATE_RADIO);
	if(mode == 0) 		{ WR_MODE(1);	RADIO_SetValue(hItem, 1); } 
	else if(mode == 1) 	{ WR_MODE(2);	RADIO_SetValue(hItem, 2); } 
	else if(mode == 2) 	{ WR_MODE(3);	RADIO_SetValue(hItem, 3); } 
	else if(mode == 3) 	{ WR_MODE(0);	RADIO_SetValue(hItem, 0); } 
	else				{ WR_MODE(0);	RADIO_SetValue(hItem, 0); } 		
	GUI_Exec();
}

#define CLEAR 2
#define PASS  true
#define FAIL  false	
void InspCountWrite(uint8_t flag) 
{	
	if(is_pc_connected_flag == true)	// PC 검사기
	{
		qty_value_pc(flag);
	}
	else
	{	
		char text[50];
		if(flag == CLEAR) 		{	QTY.TT = QTY.OK = QTY.NG = 0;     		}
		else if(flag == PASS)   {   QTY.OK++;	QTY.TT = QTY.OK + QTY.NG;   }
		else if(flag == FAIL)   {   QTY.NG++;   QTY.TT = QTY.OK + QTY.NG;   }

		WR_QTY(QTY.TT);
		WR_OK(QTY.OK);
		WR_NG(QTY.NG);
		
		hItem = WM_GetDialogItem(hStatewin,STATE_QTY);  	
		sprintf(text,"QTY  :  %6d", QTY.TT);	
		TEXT_SetText(hItem,text); 
		
		hItem = WM_GetDialogItem(hStatewin,STATE_OK);   
		sprintf(text,"O K  :  %6d", QTY.OK);  	
		TEXT_SetText(hItem,text);
		
		hItem = WM_GetDialogItem(hStatewin,STATE_NG);  
		sprintf(text,"N G  :  %6d", QTY.NG);  
		TEXT_SetText(hItem,text);
		   
		GUI_Exec();	
	}
}
void InspCountReset(void)
{
	OKLED(true);		NGLED(true);
	MainPopup(true, GUI_LIGHTBLUE,"Counter Reset\nYES:Start1 / NO:ESC");
	Delay_ms(500);
	while(1)
	{
		uint8_t KEY_RET = KeyScan();                    
		if( KEY_RET == (ST1_ON) )		{	InspCountWrite(CLEAR);	Beep(250);	break;	}
		else if( KEY_RET == (ESC_ON) )	{										break;  }
	}
	OKLED(false);		NGLED(false);
	MainPopup(false, GUI_LIGHTBLUE," "); 
}
WM_HWIN CreateMainState(void) 
{
    WM_HWIN hWin;    
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}
/*************************** End of file ****************************/
