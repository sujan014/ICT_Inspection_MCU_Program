#ifndef _TASK_ICT_SPEC_SAVE_H__
#define _TASK_ICT_SPEC_SAVE_H__

#include "main.h"

//*************************
// Operation mode
//*************************
#define OPEN_TEST		0
#define SHORT_TEST		1
#define PART_TEST		2
#define FUNC_TEST		3
#define SOL_RUN			4
#define MARKING_UNIT	5
#define RELAY			6
#define WAIT_KEY		7
#define TACT_TIME		8
#define VCC				9
#define LOG_SAVE		10
#define START_FLOW		11
#define END_FLOW		12
#define DISP_RESULT		13
#define CY_SEN_CHK		14
#define SAFE_SEN_CHK	15
#define STATE_LED		16
#define AUTO_ALIGN		17
//*************************
// Operation state
//*************************
#define ON_STATE		true
#define OFF_STATE		false
#define ST12_STATE		(ST1_ON|ST2_ON)
#define ST1_STATE		ST1_ON
#define ST2_STATE		ST2_ON
#define AUTO_STATE		0x41	//'A'
#define STEP_STATE		0x53	//'S'
#define USB_STATE		0x55	//'U'
#define PC_STATE		0x50	//'P'

//*************************
// Function Type
//*************************
#define LSI			1 	// touch
#define STM			2 	// touch
#define SYNA		3 	// touch
#define ATMEL		4 	// touch
#define SiW			5 	// touch
#define MELFAS		6 	// touch
#define MFS		  	7   // lgd auto
#define MFS_USB     8   // lgd auto

#define SPIFLASH	10	 // spi flash
#define PMIC 		11
#define EEPROM 		12
#define TPIC 		13
#define LXS			14	// LX Semicon

void UpdateIctSpecSdram(void);
void IctSpecSaveflow(void);
void IctSpecDisp(void);
uint16_t findTpName(uint16_t Tp);

char* find_func_str(uint16_t fn);
char* find_mode_str(uint16_t md);
char* find_state_str(uint16_t st);

/////
uint8_t ictspec_erase(void);
void UpdateGui(void);



#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

