#include "main.h"

volatile GUI_TIMER_TIME GuiTimer;

GUI_TIMER_TIME GUI_X_GetTime(void)
{ 
    return GuiTimer; 
}

void GUI_X_Delay(int ms) 
{ 
    int tEnd = GuiTimer + ms;
    while ((tEnd - GuiTimer) > 0);
}

void GUI_X_Init(void) {}
void GUI_X_ExecIdle(void) {}
void GUI_X_Log     (const char *s) { GUI_USE_PARA(s); }
void GUI_X_Warn    (const char *s) { GUI_USE_PARA(s); }
void GUI_X_ErrorOut(const char *s) { GUI_USE_PARA(s); }
/*************************** End of file ****************************/
