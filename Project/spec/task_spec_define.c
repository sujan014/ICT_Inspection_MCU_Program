#include "task_spec_define.h"

////////////////////////////////
//spec 폴더 memory_define.xlsx 참조
////////////////////////////////

qty QTY;
Input_data	In_data;
uint8_t *INPUT_DATA = (((uint8_t *)INPUTDATA_ADDRESS));
//------------------------------------PMIC&EEP
Input_data In_pmic;
uint8_t *INPUT_PMIC = (((uint8_t *)PMICDATA_ADDRESS));
Input_data In_eep;
uint8_t *INPUT_EEP = (((uint8_t *)EEPDATA_ADDRESS));
Input_data In_tpic;
uint8_t *INPUT_TPIC = (((uint8_t *)TPICDATA_ADDRESS));
//---------------------------------------------
// sdram addr
// 0xD0000000 	// LCD_FRAME_BUFFER
// 0xD0100000 	// GUI_ALLOC_AssignMemory
// 0xD0200000 	// PC TRX Buff
// 0xD0300000 	// 
// 0xD0400000 	// 
// 0xD0500000 	// spec
// 0xD0600000 	// spec
// 0xD0700000 	// 
// 0xD0800000 	//
// tp info                                                          
uint16_t tp_count 								__attribute__((at(0xD0500000)));
char 	 tp_name[720][32] 						__attribute__((at(0xD0500004)));
uint16_t tp_number[720] 						__attribute__((at(0xD0505A04)));
// open info                        			                    D0505FA4
uint16_t open_count								__attribute__((at(0xD0505FA4)));
uint16_t open_tp1[720]							__attribute__((at(0xD0505FA8)));
uint16_t open_tp2[720]							__attribute__((at(0xD0506548)));
uint16_t open_range[720]						__attribute__((at(0xD0506AE8)));
float 	 open_spec[720]							__attribute__((at(0xD0507088)));
uint16_t open_ok_cnt[720]						__attribute__((at(0xD0507BC8)));
uint16_t open_ng_cnt[720]						__attribute__((at(0xD0508168)));
// short base info                  			                    D0508708
uint16_t short_bs_count							__attribute__((at(0xD0508708)));
uint16_t short_bs_tp[720]						__attribute__((at(0xD050870C)));
uint16_t short_bs_group[720]					__attribute__((at(0xD0508CAC)));
uint16_t short_bs_range							__attribute__((at(0xD050924C)));
float 	 short_bs_spec							__attribute__((at(0xD0509250)));
uint16_t short_bs_ok_cnt						__attribute__((at(0xD0509254)));
uint16_t short_bs_ng_cnt						__attribute__((at(0xD0509258)));
// short extand info                			                    D050925C
uint16_t short_ex_count							__attribute__((at(0xD050925C)));
uint16_t short_ex_tp1[720]						__attribute__((at(0xD0509260)));
uint16_t short_ex_tp2[720]						__attribute__((at(0xD0509800)));
uint16_t short_ex_range[720]					__attribute__((at(0xD0509DA0)));
float 	 short_ex_spec[720]						__attribute__((at(0xD050A340)));
uint16_t short_ex_ok_cnt[720]					__attribute__((at(0xD050AE80)));
uint16_t short_ex_ng_cnt[720]					__attribute__((at(0xD050B420)));
// Part info                        			                    D050B9C0
uint16_t part_count								__attribute__((at(0xD050B9C0)));
char 	 part_name[720][32]						__attribute__((at(0xD050B9C4)));
uint16_t part_tp1[720]							__attribute__((at(0xD05113C4)));
uint16_t part_tp2[720]							__attribute__((at(0xD0511964)));
uint16_t part_range[720]						__attribute__((at(0xD0511F04)));
float 	 part_spec_hi[720]						__attribute__((at(0xD05124A4)));
float 	 part_spec_lo[720]						__attribute__((at(0xD0512FE4)));
uint16_t part_ok_cnt[720]						__attribute__((at(0xD0513B24)));
uint16_t part_ng_cnt[720]						__attribute__((at(0xD05140C4)));
// Func info                        			                    D0514664
uint16_t func_count								__attribute__((at(0xD0514664)));
char 	 func_name[180][32]						__attribute__((at(0xD0514668)));
uint16_t func_type[180]							__attribute__((at(0xD0515CE8)));
uint16_t func_module[180]						__attribute__((at(0xD0515E50)));
uint16_t func_spec_dec[180]						__attribute__((at(0xD0515FB8)));
uint8_t  func_spec_hex[180][4]					__attribute__((at(0xD0516120)));
float    func_spec_float[180]					__attribute__((at(0xD05163F0)));
char	 func_spec_ascii[180][16]				__attribute__((at(0xD05166C0)));
uint16_t func_log_flag[180]						__attribute__((at(0xD0517200)));
uint16_t func_panel_link[180]					__attribute__((at(0xD0517368)));
// name info                        			                    D05174D0
char 	 project_name_main[180]					__attribute__((at(0xD05174D0)));
char 	 project_name_sub[180]					__attribute__((at(0xD0517584)));
//                                  			                      5
char     mode1_name[32]							__attribute__((at(0xD0517638))); 
uint16_t mode1_sequence							__attribute__((at(0xD0517658))); 
uint16_t mode1_operation[100]					__attribute__((at(0xD051765C))); 
uint16_t mode1_number[100]						__attribute__((at(0xD0517724))); 
uint16_t mode1_status[100]						__attribute__((at(0xD05177EC))); 
uint16_t mode1_time[100]						__attribute__((at(0xD05178B4))); 
//                                  			                      5
char     mode2_name[32]							__attribute__((at(0xD051797C))); 
uint16_t mode2_sequence							__attribute__((at(0xD051799C))); 
uint16_t mode2_operation[100]					__attribute__((at(0xD05179A0))); 
uint16_t mode2_number[100]						__attribute__((at(0xD0517A68))); 
uint16_t mode2_status[100]						__attribute__((at(0xD0517B30))); 
uint16_t mode2_time[100]						__attribute__((at(0xD0517BF8))); 
//                                  			                      5
char     mode3_name[32]							__attribute__((at(0xD0517CC0))); 
uint16_t mode3_sequence							__attribute__((at(0xD0517CE0))); 
uint16_t mode3_operation[100]					__attribute__((at(0xD0517CE4))); 
uint16_t mode3_number[100]						__attribute__((at(0xD0517DAC))); 
uint16_t mode3_status[100]						__attribute__((at(0xD0517E74))); 
uint16_t mode3_time[100]						__attribute__((at(0xD0517F3C))); 
//                                  			                      5
char     mode4_name[32]							__attribute__((at(0xD0518004))); 
uint16_t mode4_sequence							__attribute__((at(0xD0518024))); 
uint16_t mode4_operation[100]					__attribute__((at(0xD0518028))); 
uint16_t mode4_number[100]						__attribute__((at(0xD05180F0))); 
uint16_t mode4_status[100]						__attribute__((at(0xD05181B8))); 
uint16_t mode4_time[100]						__attribute__((at(0xD0518280))); 
//                                  			                    D0518348
char     init_name[32]							__attribute__((at(0xD0518348))); 
uint16_t init_sequence							__attribute__((at(0xD0518368))); 
uint16_t init_operation[100]					__attribute__((at(0xD051836C))); 
uint16_t init_number[100]						__attribute__((at(0xD0518434))); 
uint16_t init_status[100]						__attribute__((at(0xD05184FC))); 
uint16_t init_time[100]							__attribute__((at(0xD05185C4)));   
//                                  			                    D051868C
uint16_t short_auto_pp 							__attribute__((at(0xD051868C)));   
uint16_t short_auto_mm 							__attribute__((at(0xD0518690)));
//Test Result_TOTAL                 			                    D0518694         
char	 test_item_name[2048][50]				__attribute__((at(0xD0518694)));       
char	 test_meas_string[2048][50]				__attribute__((at(0xD0531694)));  
char	 test_meas_string1[2048][50]			__attribute__((at(0xD054A694)));      
float	 test_meas_value[2048]					__attribute__((at(0xD0563694)));       
char	 test_result[2048]						__attribute__((at(0xD0565694)));       
uint16_t test_count								__attribute__((at(0xD0565E94)));
//                                                      D0265E98      5
char	 test_ng_item_name[2048][50]			__attribute__((at(0xD0565E98)));       
char	 test_ng_meas_string[2048][50]			__attribute__((at(0xD057EE98)));       
char	 test_ng_result[2048]					__attribute__((at(0xD0597E98)));       
uint16_t test_ng_count							__attribute__((at(0xD0598698)));   
//		                                                              5
int16_t  panel_min[50][1800]					__attribute__((at(0xD059869C)));
int16_t  panel_max[50][1800]					__attribute__((at(0xD05C45BC)));
//			                                                          5
uint16_t panel_count							__attribute__((at(0xD05F04DC)));	
uint16_t panel_tx_num[50]						__attribute__((at(0xD05F04E0)));	
uint16_t panel_rx_num[50]						__attribute__((at(0xD05F0544)));	
//				                                                      5
int16_t  panel_data[50][1800]					__attribute__((at(0xD05F05A8)));	
float    panel_dataf[50][1800]					__attribute__((at(0xD061C4C8)));
int16_t  ErrorNodeCount[50] 					__attribute__((at(0xD0674308)));
//																	  6
uint16_t short_tt_count							__attribute__((at(0xD067436C)));	
int16_t sdc_data[50][50] 						__attribute__((at(0xD0674370)));
float  sdc_dataf[50][50] 						__attribute__((at(0xD06756F8)));
		
/////0xD0200000 ~0xD02FFFFF
char pc_txbuff[128*1024]				__attribute__((at(0xD0200000)));// 131072byte 40000
char pc_rxbuff[128*1024]				__attribute__((at(0xD0220000)));// 131072byte 40000
char valid_rxbuff[128*1024]				__attribute__((at(0xD0240000)));// 131072byte 40000
char log_txbuff[128*1024]				__attribute__((at(0xD0260000)));// 131072byte
char uart_str1[128*1024] 				__attribute__((at(0xD0280000)));// 131072byte
char uart_str2[128*1024] 				__attribute__((at(0xD02A0000)));// 131072byte
char uart_str3[128*1024] 				__attribute__((at(0xD02C0000)));// 131072byte
char StrCat_buff[128*1024] 				__attribute__((at(0xD02E0000)));// 131072byte
		
unsigned long info_crc = 0;
unsigned long tp_crc = 0;
unsigned long open_crc = 0;
unsigned long short_crc = 0;
unsigned long shortex_crc = 0;
unsigned long part_crc = 0;
unsigned long func_crc = 0;
unsigned long mode1_crc = 0;
unsigned long mode2_crc = 0;
unsigned long mode3_crc = 0;
unsigned long mode4_crc = 0;
unsigned long init_crc = 0;
unsigned long panel_crc = 0;

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
