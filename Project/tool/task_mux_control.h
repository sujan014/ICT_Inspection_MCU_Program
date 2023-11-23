#ifndef _TASK_MUX_CONTROL__H__
#define _TASK_MUX_CONTROL__H__

#include "main.h"

#define VHC4051
//#define DG408LE

extern float mux_res;

void MuxOnRes(void);
void MuxInit(void);
void MUXPinOn(uint16_t HI, uint16_t LO);
void MUXPinOnP64N64(uint16_t HI, uint16_t LO); // PIN 64PIN MAP


#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

