#ifndef _TASK_DATA_PC_H__
#define _TASK_DATA_PC_H__

#include "main.h"

uint8_t data_erase_pc(void);
uint8_t data_write_pc(char *data);
uint8_t DataMake_PC(char *data);
uint8_t data_send_pc(void);
//----------------------------------LGD
uint8_t pmic_erase_pc(void);
uint8_t pmic_write_pc(char *data);
uint8_t PmicMake_PC(char *data);
uint8_t pmic_send_pc(void);

uint8_t eep_erase_pc(void);
uint8_t eep_write_pc(char *data);
uint8_t EepMake_PC(char *data);
uint8_t eep_send_pc(void);

uint8_t tpic_write_pc(char *data);
uint8_t TpicMake_PC(char *data);
uint8_t tpic_send_pc(void);
//-------------------------------------
#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

