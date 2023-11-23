#include "GUI_Syswin3.h" 

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
    { FRAMEWIN_CreateIndirect, "", MENU_SPEC,    75,     28, 	 405, 	 242,	0, 	0x0,	0 },
														
    { BUTTON_CreateIndirect,   "", TP_BT,		  2,	  2,	  65,	  30,	0,	0x0,	0 },
    { BUTTON_CreateIndirect,   "", OPEN_BT,		 69,	  2,	  65,	  30,	0,	0x0,	0 },
    { BUTTON_CreateIndirect,   "", SH_BS_BT,	136,	  2,	  65,	  30,	0,	0x0,	0 },
    { BUTTON_CreateIndirect,   "", SH_EX_BT,	203,	  2,	  65,	  30,	0,	0x0,	0 },
    { BUTTON_CreateIndirect,   "", PART_BT,		270,	  2,	  65,	  30,	0,	0x0,	0 },
    { BUTTON_CreateIndirect,   "", FUNC_BT,		337,	  2,	  65,	  30,	0,	0x0,	0 },
											
    { BUTTON_CreateIndirect,   "", MODE1_BT,	 69,	 35,	  65,	  30,	0,	0x0,	0 },
    { BUTTON_CreateIndirect,   "", MODE2_BT,	136,	 35,	  65,	  30,	0,	0x0,	0 },
    { BUTTON_CreateIndirect,   "", MODE3_BT,	203,	 35,	  65,	  30,	0,	0x0,	0 },
    { BUTTON_CreateIndirect,   "", MODE4_BT,	270,	 35,	  65,	  30,	0,	0x0,	0 },
	
    { BUTTON_CreateIndirect,   "", PREV_BT,		  2,	 35,	  65,	  30,	0,	0x0,	0 },
    { BUTTON_CreateIndirect,   "", NEXT_BT,		337,	 35,	  65,	  30,	0,	0x0,	0 },
		
    { LISTVIEW_CreateIndirect, "", LIST_TP  , 	  0,     67,	 405,	 175,	0,	0x0,	0 }, 
    { LISTVIEW_CreateIndirect, "", LIST_OPen, 	  0,     67,	 405,	 175,	0,	0x0,	0 }, 
    { LISTVIEW_CreateIndirect, "", LIST_SHbs, 	  0,     67,	 405,	 175,	0,	0x0,	0 }, 
    { LISTVIEW_CreateIndirect, "", LIST_SHex, 	  0,     67,	 405,	 175,	0,	0x0,	0 }, 
    { LISTVIEW_CreateIndirect, "", LIST_Part, 	  0,     67,	 405,	 175,	0,	0x0,	0 }, 
    { LISTVIEW_CreateIndirect, "", LIST_Func, 	  0,     67,	 405,	 175,	0,	0x0,	0 }, 
	
    { LISTVIEW_CreateIndirect, "", LIST_SEQ, 	  0,     67,	 405,	 175,	0,	0x0,	0 }, 
	
    { TEXT_CreateIndirect,     "", SPEC_POPUP,	  0,	  0,	 405,	 242,	0,	0x0,	0 }, 
};

#define HI_EDIT	0
#define LO_EDIT	1

#define FIRST	0
#define NEXT	1	
#define PREV	2	
#define STAY	3	
									
#define NONE_SEL		0			
#define TESTPOINT_SEL	1
#define OPEN_SEL		2	
#define SHORT_SEL		3
#define PART_SEL		4	
#define SKIP_SEL		5	
#define FUNC_SEL		6

#define MODE1_SEL		7
#define MODE2_SEL		8
#define MODE3_SEL		9
#define MODE4_SEL	   10
void View_Open_Spec(int wheel);
void View_Short_bs_Spec(int wheel);
void View_Part_Spec(int wheel);
void View_TP_Spec(int wheel);
void View_Short_ex_Spec(int wheel);
void View_Func_Spec(int wheel);

void View_mode1_Spec(int wheel);
void View_mode2_Spec(int wheel);
void View_mode3_Spec(int wheel);
void View_mode4_Spec(int wheel);

int8_t VIEW_SELECT = 0;
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	HEADER_Handle hHeader;
    int     NCode;
    int     Id;
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
            hItem = pMsg->hWin;
            FRAMEWIN_SetTitleVis(hItem, 0);  
            FRAMEWIN_SetBorderSize(hItem, 0);
            FRAMEWIN_SetClientColor(hItem, GUI_BLACK);	
            
            hItem = WM_GetDialogItem(pMsg->hWin, LIST_TP);
            LISTVIEW_SetAutoScrollV(hItem, 1);
            LISTVIEW_AddColumn(hItem,  45, "No", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem, 100, "Name", 		GUI_TA_LEFT    | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  90, "Number", 	GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_SetGridVis(hItem, 1);				LISTVIEW_EnableSort(hItem);				
			hHeader = LISTVIEW_GetHeader(hItem);  HEADER_SetBkColor(hHeader,GUI_BLACK);	HEADER_SetTextColor(hHeader,GUI_WHITE);	HEADER_SetFont(hHeader,GUI_FONT_13B_ASCII);			
			WM_SetStayOnTop(hItem,1);   WM_ShowWindow(hItem);  

            hItem = WM_GetDialogItem(pMsg->hWin, LIST_OPen);
            LISTVIEW_SetAutoScrollV(hItem, 1);
            LISTVIEW_AddColumn(hItem,  45, "No",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  100, "TP", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  55, "Range", 	GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  55, "Spec", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  45, "OK", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  45, "NG",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_SetGridVis(hItem, 1);	LISTVIEW_EnableSort(hItem);				
			hHeader = LISTVIEW_GetHeader(hItem);  HEADER_SetBkColor(hHeader,GUI_BLACK);	HEADER_SetTextColor(hHeader,GUI_WHITE);	HEADER_SetFont(hHeader,GUI_FONT_13B_ASCII);
			WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
			
			hItem = WM_GetDialogItem(pMsg->hWin, LIST_SHbs);
            LISTVIEW_SetAutoScrollV(hItem, 1);
            LISTVIEW_AddColumn(hItem,  45, "No",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  45, "TP", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  45, "Con",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  55, "Range", 	GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  55, "Spec", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  45, "OK", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  45, "NG", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_SetGridVis(hItem, 1);	LISTVIEW_EnableSort(hItem);				
			hHeader = LISTVIEW_GetHeader(hItem);  HEADER_SetBkColor(hHeader,GUI_BLACK);	HEADER_SetTextColor(hHeader,GUI_WHITE);	HEADER_SetFont(hHeader,GUI_FONT_13B_ASCII);
			WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
			
            hItem = WM_GetDialogItem(pMsg->hWin, LIST_SHex);
            LISTVIEW_SetAutoScrollV(hItem, 1);
            LISTVIEW_AddColumn(hItem,  45, "No",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  100, "TP", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  55, "Range", 	GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  55, "Spec", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  45, "OK", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  45, "NG", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_SetGridVis(hItem, 1);	LISTVIEW_EnableSort(hItem);				
			hHeader = LISTVIEW_GetHeader(hItem);  HEADER_SetBkColor(hHeader,GUI_BLACK);	HEADER_SetTextColor(hHeader,GUI_WHITE);	HEADER_SetFont(hHeader,GUI_FONT_13B_ASCII);
			WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
			
            hItem = WM_GetDialogItem(pMsg->hWin, LIST_Part);
            LISTVIEW_SetAutoScrollV(hItem, 1);
            LISTVIEW_AddColumn(hItem,  45, "No",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  100, "Name", 	GUI_TA_LEFT	   | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  60, "TP", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  55, "Range",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  40, "Hi", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  40, "Lo", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  30, "OK",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  30, "NG",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_SetGridVis(hItem, 1);	LISTVIEW_EnableSort(hItem);				
			hHeader = LISTVIEW_GetHeader(hItem);  HEADER_SetBkColor(hHeader,GUI_BLACK);	HEADER_SetTextColor(hHeader,GUI_WHITE);	HEADER_SetFont(hHeader,GUI_FONT_13B_ASCII);
			WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
			
            hItem = WM_GetDialogItem(pMsg->hWin, LIST_Func);
            LISTVIEW_SetAutoScrollV(hItem, 1);
            LISTVIEW_AddColumn(hItem,  20, "No",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  80, "Name", 		GUI_TA_LEFT	   | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  50, "Typ", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  20, "Num",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  30, "Sp1", 	GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  55, "Sp2", 	GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  40, "Sp3",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  55, "Sp4",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  20, "lg",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  20, "pl",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_SetGridVis(hItem, 1);	LISTVIEW_EnableSort(hItem);				
			hHeader = LISTVIEW_GetHeader(hItem);  HEADER_SetBkColor(hHeader,GUI_BLACK);	HEADER_SetTextColor(hHeader,GUI_WHITE);	HEADER_SetFont(hHeader,GUI_FONT_13B_ASCII);
			WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
			
			
            hItem = WM_GetDialogItem(pMsg->hWin, LIST_SEQ);
            LISTVIEW_SetAutoScrollV(hItem, 1);
            LISTVIEW_AddColumn(hItem,  30, "Seq",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem, 100, "Run", 		GUI_TA_LEFT	   | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  80, "State", 		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  40, "Num",		GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,  40, "Time", 	GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_SetGridVis(hItem, 1);	LISTVIEW_EnableSort(hItem);				
			hHeader = LISTVIEW_GetHeader(hItem);  HEADER_SetBkColor(hHeader,GUI_BLACK);	HEADER_SetTextColor(hHeader,GUI_WHITE);	HEADER_SetFont(hHeader,GUI_FONT_13B_ASCII);
			WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
					
            hItem = WM_GetDialogItem(pMsg->hWin, TP_BT);	 BUTTON_SetText(hItem, "Test\nPoint");	BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
            hItem = WM_GetDialogItem(pMsg->hWin, OPEN_BT);	 BUTTON_SetText(hItem, "Open");			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
            hItem = WM_GetDialogItem(pMsg->hWin, SH_BS_BT);	 BUTTON_SetText(hItem, "Short\nbase");	BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
            hItem = WM_GetDialogItem(pMsg->hWin, SH_EX_BT);	 BUTTON_SetText(hItem, "Short\nextand");BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			hItem = WM_GetDialogItem(pMsg->hWin, PART_BT);	 BUTTON_SetText(hItem, "Part");			BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			hItem = WM_GetDialogItem(pMsg->hWin, FUNC_BT);	 BUTTON_SetText(hItem, "Function");		BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);

			hItem = WM_GetDialogItem(pMsg->hWin, MODE1_BT);	 BUTTON_SetText(hItem, "Mode1");		BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_GRAY);		BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			hItem = WM_GetDialogItem(pMsg->hWin, MODE2_BT);	 BUTTON_SetText(hItem, "Mode2");		BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_GRAY);		BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
   			hItem = WM_GetDialogItem(pMsg->hWin, MODE3_BT);	 BUTTON_SetText(hItem, "Mode3");		BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_GRAY);		BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			hItem = WM_GetDialogItem(pMsg->hWin, MODE4_BT);	 BUTTON_SetText(hItem, "Mode4");		BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_GRAY);		BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);

            hItem = WM_GetDialogItem(pMsg->hWin, NEXT_BT);	 BUTTON_SetText(hItem, "Next\nPage");	BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_LIGHTBLUE);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
			hItem = WM_GetDialogItem(pMsg->hWin, PREV_BT);	 BUTTON_SetText(hItem, "Prev\nPage");	BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_LIGHTBLUE);	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED, GUI_WHITE);
            
            hItem = WM_GetDialogItem(pMsg->hWin,SPEC_POPUP);  TEXT_SetText(hItem,"");   TEXT_SetTextAlign(hItem,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
			TEXT_SetFont(hItem, GUI_FONT_32B_ASCII);         
            TEXT_SetTextColor(hItem, GUI_RED);
			TEXT_SetBkColor(hItem, GUI_TRANSPARENT);
			
			WM_HideWindow(hItem);   WM_SetStayOnTop(hItem,0);
		break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
			switch(Id) 
			{						
				case NEXT_BT:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED: 
							
							if(VIEW_SELECT == TESTPOINT_SEL)	View_TP_Spec(NEXT);				
							else if(VIEW_SELECT == OPEN_SEL)	View_Open_Spec(NEXT);   
							else if(VIEW_SELECT == SHORT_SEL)	View_Short_bs_Spec(NEXT);
							else if(VIEW_SELECT == PART_SEL)	View_Part_Spec(NEXT);
							else if(VIEW_SELECT == SKIP_SEL)	View_Short_ex_Spec(NEXT);    
							else if(VIEW_SELECT == FUNC_SEL)	View_Func_Spec(NEXT); 
							else if(VIEW_SELECT == MODE1_SEL)	View_mode1_Spec(NEXT);
							else if(VIEW_SELECT == MODE2_SEL)	View_mode2_Spec(NEXT);
							else if(VIEW_SELECT == MODE3_SEL)	View_mode3_Spec(NEXT);
							else if(VIEW_SELECT == MODE4_SEL)	View_mode4_Spec(NEXT);   
							
							Beep(1);   
						break;
							
					}
				break;
				case PREV_BT:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:
							
							if(VIEW_SELECT == TESTPOINT_SEL)	View_TP_Spec(PREV);				
							else if(VIEW_SELECT == OPEN_SEL)	View_Open_Spec(PREV);   
							else if(VIEW_SELECT == SHORT_SEL)	View_Short_bs_Spec(PREV);
							else if(VIEW_SELECT == PART_SEL)	View_Part_Spec(PREV);
							else if(VIEW_SELECT == SKIP_SEL)	View_Short_ex_Spec(PREV);    
							else if(VIEW_SELECT == FUNC_SEL)	View_Func_Spec(PREV);
							else if(VIEW_SELECT == MODE1_SEL)	View_mode1_Spec(PREV);
							else if(VIEW_SELECT == MODE2_SEL)	View_mode2_Spec(PREV);
							else if(VIEW_SELECT == MODE3_SEL)	View_mode3_Spec(PREV);
							else if(VIEW_SELECT == MODE4_SEL)	View_mode4_Spec(PREV);
							
							Beep(1);
						break;
					}
				break;     
										
				case MODE1_BT:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:  
							
							VIEW_SELECT = MODE1_SEL;
							View_mode1_Spec(FIRST);
							Beep(1);							
						break;
					}
				break;
				case MODE2_BT:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:  
							
							VIEW_SELECT = MODE2_SEL;
							View_mode2_Spec(FIRST);
							Beep(1);							
						break;
					}
				break;
				case MODE3_BT:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:  
							
							VIEW_SELECT = MODE3_SEL;
							View_mode3_Spec(FIRST);
							Beep(1);							
						break;
					}
				break;
				case MODE4_BT:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:  
							
							VIEW_SELECT = MODE4_SEL;
							View_mode4_Spec(FIRST);
							Beep(1);							
						break;
					}
				break;
					
					
				case TP_BT:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:  
							
							VIEW_SELECT = TESTPOINT_SEL;
							View_TP_Spec(FIRST);
							Beep(1);							
						break;
					}
				break;
				case OPEN_BT:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:
							
							VIEW_SELECT = OPEN_SEL;
							View_Open_Spec(FIRST);
							Beep(1);							
						break;
					}
				break;
				case SH_BS_BT:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:
							VIEW_SELECT = SHORT_SEL;
							View_Short_bs_Spec(FIRST);
							Beep(1);     
						break;
					}
				break;
				case SH_EX_BT:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:
							VIEW_SELECT = SKIP_SEL;
							View_Short_ex_Spec(FIRST);
							Beep(1);     
						break;
					}
				break;               
				case PART_BT:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:
							VIEW_SELECT = PART_SEL;
							View_Part_Spec(FIRST);
							Beep(1);   							
						break;
					}
				break;       
				case FUNC_BT:
					switch(NCode)
					{
						case WM_NOTIFICATION_RELEASED:
							VIEW_SELECT = FUNC_SEL;
							View_Func_Spec(FIRST);
							Beep(1);   							
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

int8_t SP_ResultPage = 0, SP_Dsp_Page = 0;
const uint16_t list_row = 10;


void View_TP_Spec(int wheel)
{		
	char *pSPT[10];
	char acSPT[10][100] = {{0}};
	char text[30];

	int cnt,cnt1;

	if(wheel == FIRST)
	{	
		hItem = WM_GetDialogItem(hSyswin3, LIST_Func);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Part);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHex);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHbs);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_OPen);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_TP);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,1);   WM_ShowWindow(hItem);
	
		SP_Dsp_Page = 0;
		SP_ResultPage = (tp_count-1)/list_row;
	}
	else if(wheel == NEXT)
	{
		SP_Dsp_Page++;
		if(SP_Dsp_Page > SP_ResultPage)  SP_Dsp_Page = 0;
	}
	else if(wheel == PREV)
	{
		SP_Dsp_Page--;
		if(SP_Dsp_Page < 0)  SP_Dsp_Page = SP_ResultPage;
	}
			
	hItem = WM_GetDialogItem(hSyswin3, LIST_TP);
	LISTVIEW_DeleteAllRows(hItem);
	GUI_Delay(10);

	for(cnt = 0; cnt <list_row; cnt++)
	{   
		if(cnt + SP_Dsp_Page * list_row < tp_count)
		{
			sprintf(acSPT[0],"%02d",cnt + SP_Dsp_Page * list_row+1);                
			sprintf(acSPT[1],"%s",tp_name[cnt + SP_Dsp_Page * list_row]);    
			sprintf(acSPT[2],"%d",tp_number[cnt + SP_Dsp_Page * list_row]);                                            
		}
		else
		{
			sprintf(acSPT[0],"");
			sprintf(acSPT[1],"");
			sprintf(acSPT[2],"");                              
		}
		for (cnt1 = 0; cnt1 < 3; cnt1++) pSPT[cnt1] = acSPT[cnt1];
		LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	}
	sprintf(acSPT[0],"");
	sprintf(acSPT[1],"");
	sprintf(acSPT[2],"");      
	for (cnt1 = 0; cnt1 < 3; cnt1++) pSPT[cnt1] = acSPT[cnt1];
	LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	
	sprintf(text, "%3d/%3d",SP_Dsp_Page+1, SP_ResultPage+1);
	LISTVIEW_SetItemText(hItem, 2, 10, text);
	LISTVIEW_SetItemTextColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_WHITE);
	LISTVIEW_SetItemBkColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_GRAY);	
}

void View_Open_Spec(int wheel)
{
	char *pSPT[10];
	char acSPT[10][100] = {{0}};
	char text[30];

	int cnt,cnt1;

	if(wheel == FIRST)
	{	
		hItem = WM_GetDialogItem(hSyswin3, LIST_TP);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Func);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Part);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHex);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHbs);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_OPen);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,1);   WM_ShowWindow(hItem);
	
		SP_Dsp_Page = 0;
		SP_ResultPage = (open_count-1)/list_row;
	}
	else if(wheel == NEXT)
	{
		SP_Dsp_Page++;
		if(SP_Dsp_Page > SP_ResultPage)  SP_Dsp_Page = 0;
	}
	else if(wheel == PREV)
	{
		SP_Dsp_Page--;
		if(SP_Dsp_Page < 0)  SP_Dsp_Page = SP_ResultPage;
	}
			
	hItem = WM_GetDialogItem(hSyswin3, LIST_OPen);
	LISTVIEW_DeleteAllRows(hItem);
	GUI_Delay(10);
	
	for(cnt = 0; cnt <list_row; cnt++)
	{   
		if(cnt + SP_Dsp_Page * list_row < open_count)
		{
			sprintf(acSPT[0],"%02d",	cnt + SP_Dsp_Page * list_row+1);                
			sprintf(acSPT[1],"%d-%d",	open_tp1[cnt + SP_Dsp_Page * list_row],open_tp2[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[2],"%d",		open_range[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[3],"%.2f",	open_spec[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[4],"%d",		open_ok_cnt[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[5],"%d",		open_ng_cnt[cnt + SP_Dsp_Page * list_row]);		
		}
		else
		{
			sprintf(acSPT[0],"");
			sprintf(acSPT[1],"");
			sprintf(acSPT[2],"");
			sprintf(acSPT[3],"");
			sprintf(acSPT[4],"");
			sprintf(acSPT[5],"");			
		}	
		for (cnt1 = 0; cnt1 < 6; cnt1++) pSPT[cnt1] = acSPT[cnt1];
		LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	}
	sprintf(acSPT[0],"");
	sprintf(acSPT[1],"");
	sprintf(acSPT[2],"");
	sprintf(acSPT[3],"");
	sprintf(acSPT[4],"");
	sprintf(acSPT[5],"");			
	for (cnt1 = 0; cnt1 < 6; cnt1++) pSPT[cnt1] = acSPT[cnt1];
	LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	
	sprintf(text, "%3d/%3d",SP_Dsp_Page+1, SP_ResultPage+1);
	LISTVIEW_SetItemText(hItem, 2, 10, text);
	LISTVIEW_SetItemTextColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_WHITE);
	LISTVIEW_SetItemBkColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_GRAY);
}

void View_Short_ex_Spec(int wheel)
{
	char *pSPT[10];
	char acSPT[10][100] = {{0}};
	char text[30];

	int cnt,cnt1;

	if(wheel == FIRST)
	{	
		hItem = WM_GetDialogItem(hSyswin3, LIST_TP);	LISTVIEW_DeleteAllRows(hItem);	WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Func);	LISTVIEW_DeleteAllRows(hItem);	WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Part);	LISTVIEW_DeleteAllRows(hItem);	WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHbs);	LISTVIEW_DeleteAllRows(hItem);	WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_OPen);	LISTVIEW_DeleteAllRows(hItem);	WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);	LISTVIEW_DeleteAllRows(hItem);	WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHex);	LISTVIEW_DeleteAllRows(hItem);	WM_SetStayOnTop(hItem,1);   WM_ShowWindow(hItem);
	
		SP_Dsp_Page = 0;
		SP_ResultPage = (short_ex_count-1)/list_row;
	}
	else if(wheel == NEXT)
	{
		SP_Dsp_Page++;
		if(SP_Dsp_Page > SP_ResultPage)  SP_Dsp_Page = 0;
	}
	else if(wheel == PREV)
	{
		SP_Dsp_Page--;
		if(SP_Dsp_Page < 0)  SP_Dsp_Page = SP_ResultPage;
	}
			
	hItem = WM_GetDialogItem(hSyswin3, LIST_SHex);
	LISTVIEW_DeleteAllRows(hItem);
	GUI_Delay(10);
	
	for(cnt = 0; cnt <list_row; cnt++)
	{   
		if(cnt + SP_Dsp_Page * list_row < short_ex_count)
		{
			sprintf(acSPT[0],"%02d",	cnt + SP_Dsp_Page * list_row+1);                
			sprintf(acSPT[1],"%d-%d",	short_ex_tp1[cnt + SP_Dsp_Page * list_row],short_ex_tp2[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[2],"%d",		short_ex_range[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[3],"%.2f",	short_ex_spec[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[4],"%d",		short_ex_ok_cnt[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[5],"%d",		short_ex_ng_cnt[cnt + SP_Dsp_Page * list_row]);			
		}
		else
		{		
			sprintf(acSPT[0],"");
			sprintf(acSPT[1],"");
			sprintf(acSPT[2],"");
			sprintf(acSPT[3],"");
			sprintf(acSPT[4],"");
			sprintf(acSPT[5],"");
		}
		for (cnt1 = 0; cnt1 < 6; cnt1++) pSPT[cnt1] = acSPT[cnt1];
		LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	}
	sprintf(acSPT[0],"");
	sprintf(acSPT[1],"");
	sprintf(acSPT[2],"");
	sprintf(acSPT[3],"");
	sprintf(acSPT[4],"");
	sprintf(acSPT[5],"");
	for (cnt1 = 0; cnt1 < 6; cnt1++) pSPT[cnt1] = acSPT[cnt1];
	LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	
	sprintf(text, "%3d/%3d",SP_Dsp_Page+1, SP_ResultPage+1);
	LISTVIEW_SetItemText(hItem, 2, 10, text);
	LISTVIEW_SetItemTextColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_WHITE);
	LISTVIEW_SetItemBkColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_GRAY);	
}
void View_Short_bs_Spec(int wheel)
{
	char *pSPT[10];
	char acSPT[10][100] = {{0}};
	char text[30];

	int cnt,cnt1;

	if(wheel == FIRST)
	{	
		hItem = WM_GetDialogItem(hSyswin3, LIST_TP);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Func);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Part);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHex);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_OPen);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHbs);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,1);   WM_ShowWindow(hItem);
	
		SP_Dsp_Page = 0;
		SP_ResultPage = (short_bs_count-1)/list_row;
	}
	else if(wheel == NEXT)
	{
		SP_Dsp_Page++;
		if(SP_Dsp_Page > SP_ResultPage)  SP_Dsp_Page = 0;
	}
	else if(wheel == PREV)
	{
		SP_Dsp_Page--;
		if(SP_Dsp_Page < 0)  SP_Dsp_Page = SP_ResultPage;
	}
			
	hItem = WM_GetDialogItem(hSyswin3, LIST_SHbs);
	LISTVIEW_DeleteAllRows(hItem);
	GUI_Delay(10);
	
	for(cnt = 0; cnt <list_row; cnt++)
	{   
		if(cnt + SP_Dsp_Page * list_row < short_bs_count)
		{
			if(cnt == 0)
			{
				sprintf(acSPT[0],"%02d",	cnt + SP_Dsp_Page * list_row + 1);
				sprintf(acSPT[1],"%d",		short_bs_tp[cnt + SP_Dsp_Page * list_row]);                    
				sprintf(acSPT[2],"%d",		short_bs_group[cnt + SP_Dsp_Page * list_row]);
				sprintf(acSPT[3],"%d",		short_bs_range);
				sprintf(acSPT[4],"%.2f",	short_bs_spec);
				sprintf(acSPT[5],"%d",		short_bs_ok_cnt);
				sprintf(acSPT[6],"%d",		short_bs_ng_cnt);
			}
			else
			{
				sprintf(acSPT[0],"%02d",	cnt + SP_Dsp_Page * list_row + 1);
				sprintf(acSPT[1],"%d",		short_bs_tp[cnt + SP_Dsp_Page * list_row]);                    
				sprintf(acSPT[2],"%d",		short_bs_group[cnt + SP_Dsp_Page * list_row]);
				sprintf(acSPT[3],"");
				sprintf(acSPT[4],"");
				sprintf(acSPT[5],"");
				sprintf(acSPT[6],"");
			}
		}
		else
		{
			sprintf(acSPT[0],"");
			sprintf(acSPT[1],"");           
			sprintf(acSPT[2],"");
			sprintf(acSPT[3],"");
			sprintf(acSPT[4],"");
			sprintf(acSPT[5],"");
			sprintf(acSPT[6],"");
		}
		for (cnt1 = 0; cnt1 < 7; cnt1++) pSPT[cnt1] = acSPT[cnt1];
		LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	}
	sprintf(acSPT[0],"");
	sprintf(acSPT[1],"");           
	sprintf(acSPT[2],"");
	sprintf(acSPT[3],"");
	sprintf(acSPT[4],"");
	sprintf(acSPT[5],"");
	sprintf(acSPT[6],"");
	for (cnt1 = 0; cnt1 < 7; cnt1++) pSPT[cnt1] = acSPT[cnt1];
	LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	
	sprintf(text, "%3d/%3d",SP_Dsp_Page+1, SP_ResultPage+1);
	LISTVIEW_SetItemText(hItem, 3, 10, text);
	LISTVIEW_SetItemTextColor(hItem, 3, 10, LISTVIEW_CI_UNSEL , GUI_WHITE);
	LISTVIEW_SetItemBkColor(hItem, 3, 10, LISTVIEW_CI_UNSEL , GUI_GRAY);	
}

void View_Part_Spec(int wheel)
{
	char *pSPT[10];
	char acSPT[10][100] = {{0}};
	char text[30];

	int cnt,cnt1;

	if(wheel == FIRST)
	{	
		hItem = WM_GetDialogItem(hSyswin3, LIST_TP);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Func);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHex);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHbs);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_OPen);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Part);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,1);   WM_ShowWindow(hItem);
	
		SP_Dsp_Page = 0;
		SP_ResultPage = (part_count-1)/list_row;
	}
	else if(wheel == NEXT)
	{
		SP_Dsp_Page++;
		if(SP_Dsp_Page > SP_ResultPage)  SP_Dsp_Page = 0;
	}
	else if(wheel == PREV)
	{
		SP_Dsp_Page--;
		if(SP_Dsp_Page < 0)  SP_Dsp_Page = SP_ResultPage;
	}
			
	hItem = WM_GetDialogItem(hSyswin3, LIST_Part);
	LISTVIEW_DeleteAllRows(hItem);
	GUI_Delay(10);
	
	for(cnt = 0; cnt <list_row; cnt++)
	{   
		if(cnt + SP_Dsp_Page * list_row < part_count)
		{
			sprintf(acSPT[0],"%02d",	cnt + SP_Dsp_Page * list_row+1);                
			sprintf(acSPT[1],"%s",		part_name[cnt + SP_Dsp_Page * list_row]);                    
			sprintf(acSPT[2],"%d-%d",	part_tp1[cnt + SP_Dsp_Page * list_row],part_tp2[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[3],"%d",		part_range[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[4],"%.2f",	part_spec_hi[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[5],"%.2f",	part_spec_lo[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[6],"%d",		part_ok_cnt[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[7],"%d",		part_ng_cnt[cnt + SP_Dsp_Page * list_row]);
		}
		else
		{
			sprintf(acSPT[0],"");
			sprintf(acSPT[1],"");
			sprintf(acSPT[2],"");
			sprintf(acSPT[3],"");
			sprintf(acSPT[4],"");
			sprintf(acSPT[5],"");
			sprintf(acSPT[6],"");
			sprintf(acSPT[7],"");
		}
		for (cnt1 = 0; cnt1 < 8; cnt1++) pSPT[cnt1] = acSPT[cnt1];
		LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	}
	sprintf(acSPT[0],"");
	sprintf(acSPT[1],"");
	sprintf(acSPT[2],"");
	sprintf(acSPT[3],"");
	sprintf(acSPT[4],"");
	sprintf(acSPT[5],"");
	sprintf(acSPT[6],"");
	sprintf(acSPT[7],"");
	for (cnt1 = 0; cnt1 < 8; cnt1++) pSPT[cnt1] = acSPT[cnt1];
	LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	
	sprintf(text, "%3d/%3d",SP_Dsp_Page+1, SP_ResultPage+1);
	LISTVIEW_SetItemText(hItem, 2, 10, text);
	LISTVIEW_SetItemTextColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_WHITE);
	LISTVIEW_SetItemBkColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_GRAY);	
}

void View_Func_Spec(int wheel)
{
	char *pSPT[10];
	char acSPT[10][100] = {{0}};
	char text[30];

	int cnt,cnt1;

	if(wheel == FIRST)
	{	
		hItem = WM_GetDialogItem(hSyswin3, LIST_TP);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHex);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHbs);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_OPen);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Part);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Func);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,1);   WM_ShowWindow(hItem);
	
		SP_Dsp_Page = 0;
		SP_ResultPage = (func_count-1)/list_row;
	}
	else if(wheel == NEXT)
	{
		SP_Dsp_Page++;
		if(SP_Dsp_Page > SP_ResultPage)  SP_Dsp_Page = 0;
	}
	else if(wheel == PREV)
	{
		SP_Dsp_Page--;
		if(SP_Dsp_Page < 0)  SP_Dsp_Page = SP_ResultPage;
	}
			
	hItem = WM_GetDialogItem(hSyswin3, LIST_Func);
	LISTVIEW_DeleteAllRows(hItem);
	GUI_Delay(10);
	
	for(cnt = 0; cnt <list_row; cnt++)
	{   
		if(cnt + SP_Dsp_Page * list_row < func_count)
			
		{
			sprintf(acSPT[0],"%02d",	cnt + SP_Dsp_Page * list_row+1);                
			sprintf(acSPT[1],"%s",		func_name[cnt + SP_Dsp_Page * list_row]);  
			sprintf(acSPT[2],"%s",find_func_str(func_type[cnt + SP_Dsp_Page * list_row]));
			sprintf(acSPT[3],"%d",		func_module[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[4],"%d",		func_spec_dec[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[5],"%02X.%02X.%02X.%02X",	func_spec_hex[cnt + SP_Dsp_Page * list_row][0],func_spec_hex[cnt + SP_Dsp_Page * list_row][1],func_spec_hex[cnt + SP_Dsp_Page * list_row][2],func_spec_hex[cnt + SP_Dsp_Page * list_row][3]);
			sprintf(acSPT[6],"%.2f",	func_spec_float[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[7],"%s",		func_spec_ascii[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[8],"%02d",	func_log_flag[cnt + SP_Dsp_Page * list_row]);
			sprintf(acSPT[9],"%02d",	func_panel_link[cnt + SP_Dsp_Page * list_row]);
		}
		else
		{
			sprintf(acSPT[0],"");
			sprintf(acSPT[1],"");
			sprintf(acSPT[2],"");
			sprintf(acSPT[3],"");
			sprintf(acSPT[4],"");
			sprintf(acSPT[5],"");
			sprintf(acSPT[6],"");
			sprintf(acSPT[7],""); 
			sprintf(acSPT[8],""); 
			sprintf(acSPT[9],""); 
		}
		for (cnt1 = 0; cnt1 < 10; cnt1++) pSPT[cnt1] = acSPT[cnt1];
		LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);    
	}
	
	sprintf(acSPT[0],"");
	sprintf(acSPT[1],"");
	sprintf(acSPT[2],"");
	sprintf(acSPT[3],"");
	sprintf(acSPT[4],"");
	sprintf(acSPT[5],"");
	sprintf(acSPT[6],"");
	sprintf(acSPT[7],"");
	sprintf(acSPT[8],"");
	sprintf(acSPT[9],"");
	for (cnt1 = 0; cnt1 < 10; cnt1++) pSPT[cnt1] = acSPT[cnt1];
	LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	
	sprintf(text, "%3d/%3d",SP_Dsp_Page+1, SP_ResultPage+1);
	LISTVIEW_SetItemText(hItem, 2, 10, text);
	LISTVIEW_SetItemTextColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_WHITE);
	LISTVIEW_SetItemBkColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_GRAY);
}



void View_mode1_Spec(int wheel)
{
	char *pSPT[10];
	char acSPT[10][100] = {{0}};
	char text[30];

	int cnt,cnt1;
		
	if(wheel == FIRST)
	{	
		hItem = WM_GetDialogItem(hSyswin3, LIST_TP);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHex);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHbs);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_OPen);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Part);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Func);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,1);   WM_ShowWindow(hItem);
		SP_Dsp_Page = 0;
		SP_ResultPage = (mode1_sequence-1)/list_row;
	}
	else if(wheel == NEXT)
	{
		SP_Dsp_Page++;
		if(SP_Dsp_Page > SP_ResultPage)  SP_Dsp_Page = 0;
	}
	else if(wheel == PREV)
	{
		SP_Dsp_Page--;
		if(SP_Dsp_Page < 0)  SP_Dsp_Page = SP_ResultPage;
	}
	
	hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);
	LISTVIEW_DeleteAllRows(hItem);
	GUI_Delay(10);

	for(cnt = 0; cnt <list_row; cnt++)
	{   
		if(cnt + SP_Dsp_Page * list_row < mode1_sequence)
		{
			sprintf(acSPT[0],"%02d", cnt + SP_Dsp_Page * list_row+1);      
			sprintf(acSPT[1],"%s", 	 find_mode_str( mode1_operation[(cnt + SP_Dsp_Page * list_row)] ) );  
			sprintf(acSPT[2],"%s", 	 find_state_str( mode1_status[(cnt + SP_Dsp_Page * list_row)] ) );      
			sprintf(acSPT[3],"%d", 	 mode1_number[(cnt + SP_Dsp_Page * list_row)] );  
			sprintf(acSPT[4],"%d", 	 mode1_time[(cnt + SP_Dsp_Page * list_row)] );  
		}
		else
		{
			sprintf(acSPT[0],"");
			sprintf(acSPT[1],"");
			sprintf(acSPT[2],"");
			sprintf(acSPT[3],"");
			sprintf(acSPT[4],"");
		}
		for (cnt1 = 0; cnt1 < 5; cnt1++) pSPT[cnt1] = acSPT[cnt1];
		LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);    
	}
	
	sprintf(acSPT[0],"");
	sprintf(acSPT[1],"");
	sprintf(acSPT[2],"");
	sprintf(acSPT[3],"");
	sprintf(acSPT[4],"");
	for (cnt1 = 0; cnt1 < 5; cnt1++) pSPT[cnt1] = acSPT[cnt1];
	LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	
	sprintf(text, "%3d/%3d",SP_Dsp_Page+1, SP_ResultPage+1);
	LISTVIEW_SetItemText(hItem, 2, 10, text);
	LISTVIEW_SetItemTextColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_WHITE);
	LISTVIEW_SetItemBkColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_GRAY);
}
void View_mode2_Spec(int wheel)
{
	char *pSPT[10];
	char acSPT[10][100] = {{0}};
	char text[30];

	int cnt,cnt1;
		
	if(wheel == FIRST)
	{	
		hItem = WM_GetDialogItem(hSyswin3, LIST_TP);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHex);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHbs);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_OPen);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Part);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Func);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,1);   WM_ShowWindow(hItem);
		SP_Dsp_Page = 0;
		SP_ResultPage = (mode2_sequence-1)/list_row;
	}
	else if(wheel == NEXT)
	{
		SP_Dsp_Page++;
		if(SP_Dsp_Page > SP_ResultPage)  SP_Dsp_Page = 0;
	}
	else if(wheel == PREV)
	{
		SP_Dsp_Page--;
		if(SP_Dsp_Page < 0)  SP_Dsp_Page = SP_ResultPage;
	}
	
	hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);
	LISTVIEW_DeleteAllRows(hItem);
	GUI_Delay(10);

	for(cnt = 0; cnt <list_row; cnt++)
	{   
		if(cnt + SP_Dsp_Page * list_row < mode2_sequence)
		{
			sprintf(acSPT[0],"%02d", cnt + SP_Dsp_Page * list_row+1);      
			sprintf(acSPT[1],"%s", 	 find_mode_str( mode2_operation[(cnt + SP_Dsp_Page * list_row)] ) );  
			sprintf(acSPT[2],"%s", 	 find_state_str( mode2_status[(cnt + SP_Dsp_Page * list_row)] ) );     
			sprintf(acSPT[3],"%d", 	 mode2_number[(cnt + SP_Dsp_Page * list_row)] );  
			sprintf(acSPT[4],"%d", 	 mode2_time[(cnt + SP_Dsp_Page * list_row)] );  
		}
		else
		{
			sprintf(acSPT[0],"");
			sprintf(acSPT[1],"");
			sprintf(acSPT[2],"");
			sprintf(acSPT[3],"");
			sprintf(acSPT[4],"");
		}
		for (cnt1 = 0; cnt1 < 5; cnt1++) pSPT[cnt1] = acSPT[cnt1];
		LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);    
	}
	
	sprintf(acSPT[0],"");
	sprintf(acSPT[1],"");
	sprintf(acSPT[2],"");
	sprintf(acSPT[3],"");
	sprintf(acSPT[4],"");
	for (cnt1 = 0; cnt1 < 5; cnt1++) pSPT[cnt1] = acSPT[cnt1];
	LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	
	sprintf(text, "%3d/%3d",SP_Dsp_Page+1, SP_ResultPage+1);
	LISTVIEW_SetItemText(hItem, 2, 10, text);
	LISTVIEW_SetItemTextColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_WHITE);
	LISTVIEW_SetItemBkColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_GRAY);
}
void View_mode3_Spec(int wheel)
{
	char *pSPT[10];
	char acSPT[10][100] = {{0}};
	char text[30];

	int cnt,cnt1;
		
	if(wheel == FIRST)
	{	
		hItem = WM_GetDialogItem(hSyswin3, LIST_TP);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHex);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHbs);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_OPen);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Part);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Func);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,1);   WM_ShowWindow(hItem);
		SP_Dsp_Page = 0;
		SP_ResultPage = (mode3_sequence-1)/list_row;
	}
	else if(wheel == NEXT)
	{
		SP_Dsp_Page++;
		if(SP_Dsp_Page > SP_ResultPage)  SP_Dsp_Page = 0;
	}
	else if(wheel == PREV)
	{
		SP_Dsp_Page--;
		if(SP_Dsp_Page < 0)  SP_Dsp_Page = SP_ResultPage;
	}
	
	hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);
	LISTVIEW_DeleteAllRows(hItem);
	GUI_Delay(10);

	for(cnt = 0; cnt <list_row; cnt++)
	{   
		if(cnt + SP_Dsp_Page * list_row < mode3_sequence)
		{
			sprintf(acSPT[0],"%02d", cnt + SP_Dsp_Page * list_row+1);      
			sprintf(acSPT[1],"%s", 	 find_mode_str( mode3_operation[(cnt + SP_Dsp_Page * list_row)] ) );  
			sprintf(acSPT[2],"%s", 	 find_state_str( mode3_status[(cnt + SP_Dsp_Page * list_row)] ) );  
			sprintf(acSPT[3],"%d", 	 mode3_number[(cnt + SP_Dsp_Page * list_row)] );  
			sprintf(acSPT[4],"%d", 	 mode3_time[(cnt + SP_Dsp_Page * list_row)] );  
		}
		else
		{
			sprintf(acSPT[0],"");
			sprintf(acSPT[1],"");
			sprintf(acSPT[2],"");
			sprintf(acSPT[3],"");
			sprintf(acSPT[4],"");
		}
		for (cnt1 = 0; cnt1 < 5; cnt1++) pSPT[cnt1] = acSPT[cnt1];
		LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);    
	}
	
	sprintf(acSPT[0],"");
	sprintf(acSPT[1],"");
	sprintf(acSPT[2],"");
	sprintf(acSPT[3],"");
	sprintf(acSPT[4],"");
	for (cnt1 = 0; cnt1 < 5; cnt1++) pSPT[cnt1] = acSPT[cnt1];
	LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	
	sprintf(text, "%3d/%3d",SP_Dsp_Page+1, SP_ResultPage+1);
	LISTVIEW_SetItemText(hItem, 2, 10, text);
	LISTVIEW_SetItemTextColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_WHITE);
	LISTVIEW_SetItemBkColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_GRAY);
}
void View_mode4_Spec(int wheel)
{
	char *pSPT[10];
	char acSPT[10][100] = {{0}};
	char text[30];

	int cnt,cnt1;
		
	if(wheel == FIRST)
	{	
		hItem = WM_GetDialogItem(hSyswin3, LIST_TP);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHex);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SHbs);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_OPen);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Part);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_Func);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,0);   WM_HideWindow(hItem);
		hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);	LISTVIEW_DeleteAllRows(hItem);		WM_SetStayOnTop(hItem,1);   WM_ShowWindow(hItem);
		SP_Dsp_Page = 0;
		SP_ResultPage = (mode4_sequence-1)/list_row;
	}
	else if(wheel == NEXT)
	{
		SP_Dsp_Page++;
		if(SP_Dsp_Page > SP_ResultPage)  SP_Dsp_Page = 0;
	}
	else if(wheel == PREV)
	{
		SP_Dsp_Page--;
		if(SP_Dsp_Page < 0)  SP_Dsp_Page = SP_ResultPage;
	}
	
	hItem = WM_GetDialogItem(hSyswin3, LIST_SEQ);
	LISTVIEW_DeleteAllRows(hItem);
	GUI_Delay(10);

	for(cnt = 0; cnt <list_row; cnt++)
	{   
		if(cnt + SP_Dsp_Page * list_row < mode4_sequence)
		{
			sprintf(acSPT[0],"%02d", cnt + SP_Dsp_Page * list_row+1);      
			sprintf(acSPT[1],"%s", 	 find_mode_str( mode4_operation[(cnt + SP_Dsp_Page * list_row)] ) );  
			sprintf(acSPT[2],"%s", 	 find_state_str( mode4_status[(cnt + SP_Dsp_Page * list_row)] ) );    
			sprintf(acSPT[3],"%d", 	 mode4_number[(cnt + SP_Dsp_Page * list_row)] );  
			sprintf(acSPT[4],"%d", 	 mode4_time[(cnt + SP_Dsp_Page * list_row)] );  
		}
		else
		{
			sprintf(acSPT[0],"");
			sprintf(acSPT[1],"");
			sprintf(acSPT[2],"");
			sprintf(acSPT[3],"");
			sprintf(acSPT[4],"");
		}
		for (cnt1 = 0; cnt1 < 5; cnt1++) pSPT[cnt1] = acSPT[cnt1];
		LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);    
	}
	
	sprintf(acSPT[0],"");
	sprintf(acSPT[1],"");
	sprintf(acSPT[2],"");
	sprintf(acSPT[3],"");
	sprintf(acSPT[4],"");
	for (cnt1 = 0; cnt1 < 5; cnt1++) pSPT[cnt1] = acSPT[cnt1];
	LISTVIEW_AddRow(hItem, (const GUI_ConstString *)pSPT);
	
	sprintf(text, "%3d/%3d",SP_Dsp_Page+1, SP_ResultPage+1);
	LISTVIEW_SetItemText(hItem, 2, 10, text);
	LISTVIEW_SetItemTextColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_WHITE);
	LISTVIEW_SetItemBkColor(hItem, 2, 10, LISTVIEW_CI_UNSEL , GUI_GRAY);
}


WM_HWIN CreateSyswin3(void) 
{
    WM_HWIN hWin;    
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}
/*************************** End of file ****************************/
