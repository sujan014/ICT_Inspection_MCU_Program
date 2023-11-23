#ifndef _MAIN__H__
#define _MAIN__H__

// System library include
#include <stm32f4xx.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>

// STemWin library include
#include "GUI.h"
#include "GUIDRV_Lin.h"
#include "GUIConf.h"
#include "BUTTON.h"
#include "WM.h"
#include "DIALOG.h"
#include "LISTBOX.h"
#include "CHECKBOX.h"
#include "FRAMEWIN.h"
#include "PROGBAR.h"
#include "TEXT.h"
#include "BUTTON.h"
#include "SLIDER.h"
#include "HEADER.h"
#include "GRAPH.h"
#include "ICONVIEW.h"
#include "LISTVIEW.h"
#include "TREEVIEW.h"

#include "sys_util.h"

// App include
#include "GUI_Init.h" 
#include "GUI_Listview.h" 
#include "GUI_State.h" 
#include "GUI_MenuButton.h" 
#include "GUI_KeyPad.h" 
#include "GUI_Sysicon.h" 
#include "GUI_Syswin1.h" 
#include "GUI_Syswin2.h" 
#include "GUI_Syswin3.h" 
#include "GUI_Syswin4.h" 

#include "GUI_Resource.h"

// H/W architecture Layer include
#include "Hal_GPIO.h"
#include "Hal_UART.h"
#include "Hal_RTC.h"
#include "Hal_SPI.h"
#include "Hal_MCP23S17_IO1.h"
#include "Hal_MCP23S17_IO2.h"
#include "Hal_MCP23S17_IO3.h"
#include "Hal_MCP23S17_IO4.h"
#include "Hal_ADC.h"
#include "Hal_DAC.h"
#include "Hal_I2C.h"

#include "Hal_ADS7846.h"
#include "Hal_USB.h"
#include "Hal_Sdram.h"
#include "Hal_Timer.h"

// FS USB library include
#include "usbh_core.h"
#include "usbh_msc_core.h"
#include "usb_core.h"
#include "usb_conf.h"
#include "usb_hcd_int.h"

// App include
#include "Task_USBh.h"
#include "task_spec_define.h"
#include "task_ict_spec_save.h"
#include "task_panel_spec_save.h"
#include "task_data_save.h"
#include "task_spec.h"
#include "task_spec_pc.h"
#include "task_panel_pc.h"
#include "task_data_pc.h"
//-------------------- LGD
#include "task_pmic.h"
#include "task_eep.h"
//--------------------

#include "task_operation.h"
#include "task_Measure.h"
#include "task_mux_control.h"
#include "task_pc_conn.h"

#include "task_test.h"
#include "task_open.h"
#include "task_short.h"
#include "task_part.h"
#include "task_func.h"
#include "task_log.h"


//#define MFF_ISC //210510 MCH

// App fucn
#include "func_lsi_main.h"
#include "func_lsi_touch.h"
#include "func_mfs_touch.h" //210510 MCH
#include "func_mfs_touch.h"
#include "CRC_Control.h"
#include "func_pmic.h"
#include "func_eep.h"
#include "func_tpic.h"

// T-IC (LX Semicon)
#include "LX_Semicon.h"

extern float sys_ver;

#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

