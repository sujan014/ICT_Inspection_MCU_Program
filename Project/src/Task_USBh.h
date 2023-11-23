#ifndef __USH_USR_H__
#define __USH_USR_H__

#include "main.h"
#include "ff.h"
#include "usbh_core.h"
#include "usbh_msc_core.h"
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"
#include "usb_conf.h"

extern  USBH_Usr_cb_TypeDef USR_cb;

/* State Machine for the USBH_USR_ApplicationState */
#define USH_USR_FS_INIT       0
#define USH_USR_FS_MSC_DONE   2
#define USH_USR_FS_READLIST   3
#define USH_USR_FS_WRITEFILE  4
#define USH_USR_FS_DRAW       5

extern  uint8_t USBH_USR_ApplicationState ;
extern FATFS fatfs;
extern FIL file;
extern DIR dir;
extern FATFS* fs;
extern DWORD fre_clust;  

void USBH_USR_ApplicationSelected(void);
void USBH_USR_Init(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceAttached(void);
void USBH_USR_ResetDevice(void);
void USBH_USR_DeviceDisconnected (void);
void USBH_USR_OverCurrentDetected (void);
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed); 
void USBH_USR_Device_DescAvailable(void *);
void USBH_USR_DeviceAddressAssigned(void);
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc);
void USBH_USR_Manufacturer_String(void *);
void USBH_USR_Product_String(void *);
void USBH_USR_SerialNum_String(void *);
void USBH_USR_EnumerationDone(void);
USBH_USR_Status USBH_USR_UserInput(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceNotSupported(void);
void USBH_USR_UnrecoveredError(void);
int USBH_USR_MSC_Application(void);

double USB_Estimation_Free(void);
char*trimAll(char *input);
uint8_t Explore_Disk (char* path , uint8_t recu_level);
char* f_gets_FW(char* buff, int len, FIL* fil);

#endif /*__USH_USR_H__*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

