#include "GUI_Listview.h" 

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
    { FRAMEWIN_CreateIndirect, "", MAIN_WINDOW,      0,  28, 370, 242, 0, 0x0, 0 },
    { LISTVIEW_CreateIndirect, "", MAIN_LISTVIEW,    0,   0, 370, 242, 0, 0x0, 0 },
    { TEXT_CreateIndirect,     "", MAIN_POPUP,      25,  35, 320, 172, 0, 0x0, 0 },
};


static void _cbDialog(WM_MESSAGE * pMsg) 
{
	HEADER_Handle hHeader;
	
    int     Id;
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
            hItem = pMsg->hWin;  
            FRAMEWIN_SetTitleVis(hItem, 0);
            FRAMEWIN_SetBorderSize  (hItem, 0);
            FRAMEWIN_SetClientColor (hItem, GUI_GRAY);

			hItem = WM_GetDialogItem(pMsg->hWin, MAIN_LISTVIEW);
		
			LISTVIEW_SetFont(hItem, GUI_FONT_13_ASCII);            

            LISTVIEW_AddColumn(hItem,  25, "No",         	GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem, 125, "Test Item",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem, 175, "Measure Value", GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  40, "Result",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_SetGridVis(hItem, 1);
            LISTVIEW_EnableSort(hItem);   

			hHeader = LISTVIEW_GetHeader(hItem);  
			HEADER_SetBkColor(hHeader,GUI_BLACK);
			HEADER_SetTextColor(hHeader,GUI_WHITE);
				
            hItem = WM_GetDialogItem(pMsg->hWin, MAIN_POPUP); 
			TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);            
            TEXT_SetBkColor(hItem, GUI_TRANSPARENT); 
			TEXT_SetTextColor(hItem, GUI_RED); 
			TEXT_SetText(hItem,"");
            TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER|TEXT_CF_VCENTER);
            WM_SetStayOnTop(hItem,1);
            WM_HideWindow(hItem);
			
        break;
        
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc);
            switch(Id) 
            {
                case MAIN_LISTVIEW: // Notifications sent by 'Listview'

                break;
            }
        break;
        
        default:
            WM_DefaultProc(pMsg);
        break;
    }
}

void ListView_ITEM(char *Item) 
{	   
	if(is_pc_connected_flag == true)	// PC 검사기
	{
		test_count++;
		sprintf(test_item_name[test_count-1],"%s",Item);
	}
	else
	{
		uint8_t cnt=0, row=0;
		char *pInspText[4];
		char  aInspText[4][50] = {{0}};
		char text[10];
		hItem = WM_GetDialogItem(hListview, MAIN_LISTVIEW);

		if(test_count == 0) 						LISTVIEW_DeleteAllRows(hItem);
		if((LISTVIEW_GetNumRows(hItem)-1) == 14)  	LISTVIEW_DeleteAllRows(hItem);

		for (cnt = 0; cnt < 4; cnt++)
		{
			sprintf(aInspText[cnt], " ");	
			pInspText[cnt] = aInspText[cnt];    
		}	
		LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pInspText); 	
		
		test_count++;
		row = LISTVIEW_GetNumRows(hItem)-1;	
		sprintf(text,"%3d",test_count);   						LISTVIEW_SetItemText(hItem, 0, row, text);
		sprintf(test_item_name[test_count-1],"%s",Item);		LISTVIEW_SetItemText(hItem, 1, row, test_item_name[test_count-1]);
		sprintf(test_meas_string[test_count-1],"Checking");   	LISTVIEW_SetItemText(hItem, 2, row, test_meas_string[test_count-1]);
		test_result[test_count-1] = NULL;						LISTVIEW_SetItemText(hItem, 3, row, "");
		GUI_Exec();	
	}
}

void ListView_MEAS(char *Meas)
{
	if(is_pc_connected_flag == true)	// PC 검사기
	{
		sprintf(test_meas_string[test_count-1],"%s",Meas);	
	}
	else
	{
		uint8_t row;
		hItem = WM_GetDialogItem(hListview, MAIN_LISTVIEW);
		row = LISTVIEW_GetNumRows(hItem)-1;   

		sprintf(test_meas_string[test_count-1],"%s",Meas);	
		LISTVIEW_SetItemText(hItem, 2, row, test_meas_string[test_count-1]);														
		LISTVIEW_SetItemText(hItem, 3, row, "");
		
		GUI_Exec();
	}
}

void ListView_RESULT(uint8_t Flag, uint8_t mode)
{   
	if(is_pc_connected_flag == true)	// PC 검사기
	{
		if(Flag ==PASS_RESULT)			test_result[test_count-1] = 'P';
		else if(Flag == FAIL_RESULT)	test_result[test_count-1] = 'F';
		else if(Flag == SKIP_RESULT)	test_result[test_count-1] = 'S';
		else if(Flag == BUSY_RESULT)	test_result[test_count-1] = 'B';
		else if(Flag == USB_WRITE)		test_result[test_count-1] = 'I'; //210516 MCH
		else if(Flag == USB_VERIFY)		test_result[test_count-1] = 'U'; //210516 MCH
		
		if(mode == AUTO_STATE)
		{					
			CustomStrCat(log_txbuff, "%d,",test_count);
			CustomStrCat(NULL, "%s,",test_item_name[test_count-1]);		//1
			CustomStrCat(NULL, "%s,",test_meas_string[test_count-1]);	//1
			CustomStrCat(NULL, "%c,",test_result[test_count-1]);		//1
			tx_pc_data(LIST_RESULT, strlen(log_txbuff), log_txbuff);
			ntDelay_ms(15);			
			
			/*
			if(Flag == FAIL_RESULT)
			{
				test_ng_count++;			
				sprintf(test_ng_item_name[test_ng_count-1],"%s",test_item_name[test_count-1]);
				sprintf(test_ng_meas_string[test_ng_count-1],"%s",test_meas_string[test_count-1]);	
				test_ng_result[test_ng_count-1] = 'F';
				
				CustomStrCat(log_txbuff, "%d,",test_ng_count);
				CustomStrCat(NULL, "%s,",test_ng_item_name[test_ng_count-1]);		//1
				CustomStrCat(NULL, "%s,",test_ng_meas_string[test_ng_count-1]);	//1
				CustomStrCat(NULL, "%c,",test_ng_result[test_ng_count-1]);		//1
				tx_pc_data(LIST_RESULT, strlen(log_txbuff), log_txbuff);
				ntDelay_ms(15);
			}
			*/
		}
		else if(mode == STEP_STATE)
		{
			CustomStrCat(log_txbuff, "%d,",test_count);
			CustomStrCat(NULL, "%s,",test_item_name[test_count-1]);		//1
			CustomStrCat(NULL, "%s,",test_meas_string[test_count-1]);	//1
			CustomStrCat(NULL, "%c,",test_result[test_count-1]);		//1
			tx_pc_data(LIST_RESULT, strlen(log_txbuff), log_txbuff);
			ntDelay_ms(15);
		}
	}
	else
	{
		uint8_t row;
		hItem = WM_GetDialogItem(hListview, MAIN_LISTVIEW);
		row = LISTVIEW_GetNumRows(hItem)-1;   
		if(Flag ==PASS_RESULT)
		{
			test_result[test_count-1] = 'P';
			LISTVIEW_SetItemText(hItem, 3, row, "PASS");  
			LISTVIEW_SetItemTextColor(hItem, 3, row, LISTVIEW_CI_UNSEL , GUI_BLUE);
		}
		else if(Flag == FAIL_RESULT)
		{
			test_result[test_count-1] = 'F';
			LISTVIEW_SetItemText(hItem, 3, row, "FAIL");  
			LISTVIEW_SetItemTextColor(hItem, 3, row, LISTVIEW_CI_UNSEL , GUI_RED);
		}
		else if(Flag == SKIP_RESULT)
		{
			test_result[test_count-1] = 'S';
			LISTVIEW_SetItemText(hItem, 3, row, "SKIP");  
			LISTVIEW_SetItemTextColor(hItem, 3, row, LISTVIEW_CI_UNSEL , GUI_DARKGREEN);
		}	
		else if(Flag == BUSY_RESULT)
		{
			test_result[test_count-1] = 'B';
			LISTVIEW_SetItemText(hItem, 3, row, "BUSY");  
			LISTVIEW_SetItemTextColor(hItem, 3, row, LISTVIEW_CI_UNSEL , GUI_DARKBLUE);
		}	
		GUI_Exec();
	}
}
void ListView_CLEAR(void)
{		
	hItem = WM_GetDialogItem(hListview, MAIN_LISTVIEW);
	LISTVIEW_DeleteAllRows(hItem);
}
void MainPopupTrans(uint8_t flag, GUI_COLOR Color, char msg[100])
{		
	if(is_pc_connected_flag == true)	// PC 검사기
	{
		CustomStrCat(log_txbuff, "%d,",flag);	//1
		CustomStrCat(NULL, "%s,",msg);			//2
		tx_pc_data(POPUP_DISP, strlen(log_txbuff), log_txbuff);
	}
	else
	{
		hItem = WM_GetDialogItem(hListview, MAIN_POPUP);
	
		if( flag == true ) 
		{
			TEXT_SetBkColor(hItem, GUI_TRANSPARENT); 
			TEXT_SetTextColor(hItem, Color); 
			TEXT_SetText(hItem,msg);
			WM_ShowWindow(hItem);        
		}
		else if( flag == false ) 
		{
			WM_HideWindow(hItem);
		}	
		GUI_Exec();
	}
}

void MainPopup(uint8_t flag, GUI_COLOR Color, char msg[100])
{	
	if(is_pc_connected_flag == true)	// PC 검사기
	{
		CustomStrCat(log_txbuff, "%d,",flag);	//1
		CustomStrCat(NULL, "%s,",msg);			//2
		tx_pc_data(POPUP_DISP, strlen(log_txbuff), log_txbuff);
	}
	else
	{
		hItem = WM_GetDialogItem(hListview, MAIN_POPUP);
	
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
}

void MainPopupResult(uint8_t flag, uint8_t rst, char msg[100])
{		
	if(is_pc_connected_flag == true)	// PC 검사기
	{
		CustomStrCat(log_txbuff, "%d,",flag);	//1
		CustomStrCat(NULL, "%d,",rst);			//2
		CustomStrCat(NULL, "%s,",msg);			//3
		tx_pc_data(POPUP_RESULT, strlen(log_txbuff), log_txbuff);
	}
	else
	{
		hItem = WM_GetDialogItem(hListview, MAIN_POPUP);
	
		if( flag == true ) 
		{		
			if(rst == true)
			{
				TEXT_SetBkColor(hItem, GUI_BLUE); 
				TEXT_SetTextColor(hItem, GUI_BLACK); 
				TEXT_SetText(hItem,msg);
				WM_ShowWindow(hItem);        
			}
			else if(rst == false)
			{
				TEXT_SetBkColor(hItem, GUI_RED); 
				TEXT_SetTextColor(hItem, GUI_BLACK); 
				TEXT_SetText(hItem,msg);
				WM_ShowWindow(hItem);
			}            
		}
		else if( flag == false ) 
		{
			WM_HideWindow(hItem);
		}
		GUI_Exec();		
	}		
}

WM_HWIN CreateMainListview(void) 
{
    WM_HWIN hWin;    
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}
/*************************** End of file ****************************/
