#ifndef _TASK_MEASURE_H__
#define _TASK_MEASURE_H__

#include "main.h"

#define OFF			0
#define I_ON1		1
#define I_ON2		2
#define I_ON3		3
#define I_ON4		4
#define I_ON5		5
#define I_LOW		6
#define MEAS_OUT	7
#define EVCC_CUR	8

extern char RESULT_SYB[50];
extern float Measure_Result;

void MeasureCurSel(uint8_t range);
void ModeSelect(uint8_t range);
float Measure(uint8_t range);
float MeasureCap(void);
int cap_count(void);

#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

