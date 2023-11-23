#ifndef __USB_CONF__H__
#define __USB_CONF__H__

#include "stm32f4xx.h"
 
#define USB_OTG_FS_CORE

#define RX_FIFO_FS_SIZE                          128
#define TXH_NP_FS_FIFOSIZ                         96
#define TXH_P_FS_FIFOSIZ                          96

#define USE_HOST_MODE

#define __ALIGN_BEGIN
#define __ALIGN_END   

#define __packed    __packed

#if defined (__CC_ARM)         /* ARM Compiler */
  #define __packed    __packed
#elif defined (__ICCARM__)     /* IAR Compiler */
  #define __packed    __packed
#elif defined   ( __GNUC__ )   /* GNU Compiler */                        
  #define __packed    __attribute__ ((__packed__))
#elif defined   (__TASKING__)  /* TASKING Compiler */
  #define __packed    __unaligned
#endif /* __CC_ARM */


#endif //__USB_CONF__H__
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

