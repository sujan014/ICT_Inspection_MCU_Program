#ifndef _TASK_SPEC_PC_H__
#define _TASK_SPEC_PC_H__

#include "main.h"

uint8_t ictspec_erase_pc(void);
uint8_t Spec_Make_PC(char *data);
uint8_t Info_Save_PC(char *data);
uint8_t Tp_Save_PC(char *data);
uint8_t Open_Save_PC(char *data);
uint8_t Short_bs_Save_PC(char *data);
uint8_t Short_ex_save_PC(char *data);
uint8_t Part_Save_PC(char *data);
uint8_t Func_Save_PC(char *data);
uint8_t mode_Save1_PC(char *data);
uint8_t mode_Save2_PC(char *data);
uint8_t mode_Save3_PC(char *data);
uint8_t mode_Save4_PC(char *data);
uint8_t init_Save_PC(char *data);
///////////////////////////////////
uint8_t Info_Send_PC(void);
uint8_t Tp_Send_PC(void);
uint8_t Open_Send_PC(void);
uint8_t Short_bs_Send_PC(void);
uint8_t Short_ex_send_PC(void);
uint8_t Part_Send_PC(void);
uint8_t Func_Send_PC(void);
uint8_t mode_Send1_PC(void);
uint8_t mode_Send2_PC(void);
uint8_t mode_Send3_PC(void);
uint8_t mode_Send4_PC(void);
uint8_t init_Send_PC(void);
#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

