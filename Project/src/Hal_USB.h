#ifndef __USB_BSP__H__
#define __USB_BSP__H__

#include "main.h"
#include "usbh_core.h"

extern __ALIGN_BEGIN USB_OTG_CORE_HANDLE      USB_OTG_Core __ALIGN_END;
extern __ALIGN_BEGIN USBH_HOST                USB_Host __ALIGN_END;

extern volatile uint8_t usb_host_connected_flag;

void USB_OTG_BSP_Init (USB_OTG_CORE_HANDLE *pdev);
void USB_OTG_BSP_EnableInterrupt (USB_OTG_CORE_HANDLE *pdev);
void USB_OTG_BSP_uDelay (const uint32_t usec);
void USB_OTG_BSP_mDelay (const uint32_t msec);

#endif //__USB_BSP__H__
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
