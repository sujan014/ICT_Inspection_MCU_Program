#ifndef _TASK_TEST__H__
#define _TASK_TEST__H__

#include "main.h"

#define ESC_KEY 3

extern char open_disp[100];
extern char short_disp[100];

extern uint16_t Op_str_idx;
extern uint16_t Op_end_idx;
extern uint16_t Sh_str_idx;
extern uint16_t Sh_end_idx;
extern uint16_t Pt_str_idx;
extern uint16_t Pt_end_idx;
extern uint16_t Fn_str_idx;
extern uint16_t Fn_end_idx;

extern uint8_t op_auto_result;
extern uint8_t sh_auto_result;
extern uint8_t pt_auto_result;
extern uint8_t fn_auto_result;

extern uint8_t test_result_flag;
extern uint8_t list_disp_flag;

void TestStart(void);
void TaskStartFlow(void);
uint8_t Check_Short_auto(void);

void TestResultDisp(uint8_t wheel,uint8_t mode);

#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

