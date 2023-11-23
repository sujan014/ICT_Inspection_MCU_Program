#ifndef __GUI_STATE__H__
#define __GUI_STATE__H__
   
#include "main.h"

#define STATE_WINDOW    (GUI_ID_USER + 0x00)
#define STATE_RADIO     (GUI_ID_USER + 0x01)
#define STATE_USB		(GUI_ID_USER + 0x02)
#define STATE_PC		(GUI_ID_USER + 0x03)
#define STATE_LABEL3	(GUI_ID_USER + 0x04)

#define STATE_QTY       (GUI_ID_USER + 0x10)
#define STATE_OK        (GUI_ID_USER + 0x11)
#define STATE_NG        (GUI_ID_USER + 0x12)
#define STATE_TACT      (GUI_ID_USER + 0x13)
#define STATE_FW        (GUI_ID_USER + 0x14)


WM_HWIN CreateMainState(void);
void InspModeChange(uint8_t mode);
void InspCountWrite(uint8_t flag);
void InspCountReset(void);
void ModeNameUpdate(void);

#endif //__GUI_MAIN_WINDOW__H__
