#include "Task_USBh.h"

uint8_t USBH_USR_ApplicationState = USH_USR_FS_INIT;

FATFS fatfs;
FIL file;
DIR dir;
FATFS* fs;
DWORD fre_clust;   

USBH_Usr_cb_TypeDef USR_cb =
{
    USBH_USR_Init,
    USBH_USR_DeInit,
    USBH_USR_DeviceAttached,
    USBH_USR_ResetDevice,
    USBH_USR_DeviceDisconnected,
    USBH_USR_OverCurrentDetected,
    USBH_USR_DeviceSpeedDetected,
    USBH_USR_Device_DescAvailable,
    USBH_USR_DeviceAddressAssigned,
    USBH_USR_Configuration_DescAvailable,
    USBH_USR_Manufacturer_String,
    USBH_USR_Product_String,
    USBH_USR_SerialNum_String,
    USBH_USR_EnumerationDone,
    USBH_USR_UserInput,
    USBH_USR_MSC_Application,
    USBH_USR_DeviceNotSupported,
    USBH_USR_UnrecoveredError    
};

uint8_t Explore_Disk (char* path , uint8_t recu_level);

void USBH_USR_Init(void)
{
    USBH_USR_ApplicationState = USH_USR_FS_INIT;
}

void USBH_USR_DeviceAttached(void)
{
    USBH_USR_ApplicationState = USH_USR_FS_INIT;
}

void USBH_USR_UnrecoveredError (void)
{
    USBH_USR_ApplicationState = USH_USR_FS_INIT;
}

void USBH_USR_DeviceDisconnected (void)
{    
    USBH_USR_ApplicationState = USH_USR_FS_INIT;
    usb_host_connected_flag = 0;
    hItem = WM_GetDialogItem(hStatewin, STATE_USB);    
    TEXT_SetText(hItem,"USB( 00.00% )"); 
	TEXT_SetBkColor(hItem, GUI_BLACK); 
	TEXT_SetTextColor(hItem, GUI_WHITE);    
    GUI_Exec(); 
	Beep(5);
}

void USBH_USR_ResetDevice(void)
{
	USBH_USR_ApplicationState = USH_USR_FS_INIT;
}

void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
}

void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 	 
}

void USBH_USR_DeviceAddressAssigned(void)
{
}

void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc, USBH_InterfaceDesc_TypeDef *itfDesc, USBH_EpDesc_TypeDef *epDesc)
{
}

void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
}

void USBH_USR_Product_String(void *ProductString)
{
}

void USBH_USR_SerialNum_String(void *SerialNumString)
{
} 

void USBH_USR_EnumerationDone(void)
{  
} 

void USBH_USR_DeviceNotSupported(void)
{
}  

USBH_USR_Status USBH_USR_UserInput(void)
{
    USBH_USR_Status usbh_usr_status;  
    usbh_usr_status = USBH_USR_RESP_OK;
    return usbh_usr_status;	
}  

void USBH_USR_OverCurrentDetected (void)
{    
}
    
int display_flag=0;
int USBH_USR_MSC_Application(void)
{           	
    if (USBH_USR_ApplicationState == USH_USR_FS_INIT) 
    {        
        if ( f_mount( 0, &fatfs ) != FR_OK ) 
        {
            usb_host_connected_flag = 0;
            return(-1);            
        }
        f_mount(0, NULL); 
        USBH_USR_ApplicationState = USH_USR_FS_MSC_DONE; 
        display_flag = 1;
    }
    else if (USBH_USR_ApplicationState == USH_USR_FS_MSC_DONE) 
    {          		
       if(display_flag == 1)
       {
			char text[25];
			display_flag = 0;
            usb_host_connected_flag = 1;
            sprintf(text,"USB( %04.2lf%% )",USB_Estimation_Free()); 
            hItem = WM_GetDialogItem(hStatewin, STATE_USB);  
			TEXT_SetText(hItem,text); TEXT_SetBkColor(hItem, GUI_GREEN);
			TEXT_SetTextColor(hItem, GUI_BLACK);
			GUI_Exec(); 
			Beep(5);
		}
    }    
    return 0; 
}

double USB_Estimation_Free(void)
{
    double USB_Capa = 0 , USB_Free = 0, USB_disp = 0;
    
    if ( f_mount( 0, &fatfs ) != FR_OK ) 
    {
        usb_host_connected_flag = 0;
        return 0;   
    }
    else
    {
        if(f_getfree("0:/",&fre_clust, &fs) != FR_OK)
        {
            usb_host_connected_flag = 0;
            return 0;
        }
        else
        {
            USB_Capa = (double)((double)((double)USBH_MSC_Param.MSCapacity / 1048576.0) * (double)USBH_MSC_Param.MSPageLength);
            USB_Free = ((double)512*(double)fs->csize * (double)fs->free_clust) / 1048576.0;
            USB_disp = (USB_Free/USB_Capa)*100;    
        }
    }
    f_mount(0, NULL); 
    
    return USB_disp;
}

void USBH_USR_DeInit(void)
{
    USBH_USR_ApplicationState = USH_USR_FS_INIT;
}

char result_trim[512];
char*trimAll(char *input)
{
    int i=0,chk=0;
    
    int len=strlen(input);
    memset(result_trim,'\0',sizeof(result_trim));
    for(i=0;i<len;i++)
    {
        if( (*(input+i)==' ')||(*(input+i)=='\t')||(*(input+i)=='\n') )continue;
        result_trim[chk]=*(input+i);
        chk++;
    }
    return result_trim;
}

char* f_gets_FW(char* buff, int len, FIL* fil)
{
	int i = 0;
	char *p = buff;
	UINT rc;

	while (i < len - 1) 
    {
        /* Read bytes until buffer gets filled */
		f_read(fil, p, 1, &rc);
		if (rc != 1) break;			/* Break when no data to read */
        *p++;
		i++;
	}
	*p = 0;
	return i ? buff : NULL;			/* When no data read (eof or error), return with error. */
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
