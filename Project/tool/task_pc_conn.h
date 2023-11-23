#ifndef _TASK_PC_CONN_H__
#define _TASK_PC_CONN_H__

#include "main.h"

#define ICT_SPEC_ERASE  	100
#define ICT_PROJECT_WR  	101
#define ICT_TESTPOINT_WR  	102
#define ICT_OPEN_WR  		103
#define ICT_SHORTBS_WR  	104
#define ICT_SHORTEX_WR 	 	105
#define ICT_PART_WR  		106
#define ICT_FUNC_WR  		107
#define OP_MODE1_WR			108
#define OP_MODE2_WR			109
#define OP_MODE3_WR			110
#define OP_MODE4_WR			111
#define OP_INIT_WR			112
#define ICT_SPEC_FINSH  	113
////////////////////////////////
#define ICT_PROJECT_RD  	115
#define ICT_TESTPOINT_RD  	116
#define ICT_OPEN_RD  		117
#define ICT_SHORTBS_RD  	118
#define ICT_SHORTEX_RD 	 	119
#define ICT_PART_RD  		120
#define ICT_FUNC_RD  		121
#define OP_MODE1_RD			122
#define OP_MODE2_RD			123
#define OP_MODE3_RD			124
#define OP_MODE4_RD			125
#define OP_INIT_RD			126

#define PANEL_DATA_ERASE  	130
#define PANEL_DATA_WR  		131
#define PANEL_DATA_FINSH  	132
#define PANEL_DATA_RD  		133

#define INPUT_DATA_ERASE 	135
#define INPUT_DATA_WR 		136
#define INPUT_DATA_FINSH 	137
#define INPUT_DATA_RD  		138
		
#define FW_DATA_ERASE  		140
#define FW_DATA_WR  		141
#define FW_DATA_FINSH  		142
//----------------------------- LGD
#define PMIC_DATA_ERASE  	145
#define PMIC_DATA_WR  		146
#define PMIC_DATA_FINSH  	147
#define PMIC_DATA_RD  		148

#define EEP_DATA_ERASE  	185
#define EEP_DATA_WR  		186
#define EEP_DATA_FINSH  	187
#define EEP_DATA_RD  		188

#define TPIC_DATA_ERASE  	143
#define TPIC_DATA_WR  		127
#define TPIC_DATA_FINSH  	128
#define TPIC_DATA_RD  		129


//-----------------------------		
#define LIST_ITEM   		150
#define LIST_MEAS   		151
#define LIST_RESULT 		152

#define START_STATE			155
#define OPEN_STATE 			160
#define SHORT_STATE  		161
#define PART_STATE 			162
#define FUNC_STATE			163
#define END_STATE			165

#define PANLE_DATA	 		166

#define STAGE_STATE			168
#define VISON_STATE			169
#define INFO_STATE			170
#define SUMMARY_LOG			180
#define PANEL_LOG			181

#define KEY_STATE  			190
#define KEY_VALUE   		200
#define MODE_CHANGE 		201
#define SAFE_VALUE  		202
#define QTY_VALUE   		203
#define POPUP_DISP   		204
#define POPUP_RESULT 		205
#define STOP_WATCH   		206
#define SEN_VALUE  			207
#define LED_VALUE  			208
#define SEQ_STATE 			210

#define MFS_ING  			220
#define MFS_PASS 			221
#define MFS_FAIL 			222

#define MFS_WRITE  			223
#define MFS_VERIFY			224

#define DEBUG_MODE  		253
#define RUN_MODE  			254
#define CONN_CHECK  		255

extern char is_pc_connected_flag;
extern char test_state_flag;

void IsPcCheck(void);
void tx_pc_data(uint8_t nType, int length, char *data);
void rx_pc_data(void);

void mode_change_pc(uint8_t mode);
void key_value_pc(uint8_t value);
void safe_value_pc(uint8_t value);
void sen_value_pc(uint8_t value);
void test_index_pc(uint8_t state, uint16_t str_idx, uint16_t end_idx, uint8_t result,uint8_t allresult);

void qty_value_pc(uint8_t value);
void start_flow_pc(void);
void end_flow_pc(uint16_t cnt, uint8_t flag);
void log_save_pc(uint8_t flag);


#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

