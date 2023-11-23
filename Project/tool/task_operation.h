#ifndef _TASK_OPERATION_H__
#define _TASK_OPERATION_H__

#include "main.h"


#define STOPWATCH_START true
#define STOPWATCH_CHECK 0xaa
#define STOPWATCH_END   false

extern uint8_t ESC_IN_TEST;
extern float tact_time;
extern int safe_check_sol_number;

void operation(uint16_t seq, uint16_t *run, uint16_t *num, uint16_t *state, uint16_t *time);
void EvccControl(uint8_t num,uint8_t st);
void OperEscKeyCheck(void);
void StopWatch(uint8_t flag);

#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

