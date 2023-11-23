#ifndef _TASK_SPEC_DEFINE_H__
#define _TASK_SPEC_DEFINE_H__

#include "main.h"
/*
[FLASH MEMORY MAP]
 Sector00	0x08000000	 0x08003FFF 	16	BOOTLOADER #1
 Sector01	0x08004000	 0x08007FFF 	16	BOOTLOADER #2
 Sector02	0x08008000	 0x0800BFFF 	16	BOOTLOADER #3
 Sector03	0x0800C000	 0x0800FFFF 	16	BOOTLOADER #4
 Sector04	0x08010000	 0x0801FFFF 	64	BOOTLOADER #5
 Sector05	0x08020000	 0x0803FFFF 	128	APPLICATION #1
 Sector06	0x08040000	 0x0805FFFF 	128	APPLICATION	#2
 Sector07	0x08060000	 0x0807FFFF 	128	APPLICATION #3
 Sector08	0x08080000	 0x0809FFFF 	128	APPLICATION #4
 Sector09	0x080A0000	 0x080BFFFF 	128	Reserve #1
 Sector10	0x080C0000	 0x080DFFFF 	128	Reserve #2
 Sector11	0x080E0000	 0x080FFFFF 	128 Reserve #3
 
 Sector12	0x08100000	 0x08103FFF 	16	Input Data Spec
 Sector13	0x08104000	 0x08107FFF 	16	Input Data #1
 Sector14	0x08108000	 0x0810BFFF 	16	Input Data #2
 Sector15	0x0810C000	 0x0810FFFF 	16	Input Data #3
 Sector16	0x08110000	 0x0811FFFF 	64	Input Data #4
 Sector17	0x08120000	 0x0813FFFF 	128	Input Data #5
 Sector18	0x08140000	 0x0815FFFF 	128	Input Data #6
 Sector19	0x08160000	 0x0817FFFF 	128	Input Data #7
 Sector20	0x08180000	 0x0819FFFF 	128	ICT SPEC
 Sector21	0x081A0000	 0x081BFFFF 	128	TSP Panel SPEC
 Sector22	0x081C0000	 0x081DFFFF 	128	TSP Panel SPEC
 Sector23	0x081E0000	 0x081FFFFF 	128	TSP Panel SPEC
*/

//////////////////////////////////////////////////////////////
// Sector20	0x08180000	 0x0819FFFF 	128	ICT SPEC
//////////////////////////////////////////////////////////////
#define TEST_POINT_TOTAL_ADD		0x8180000
#define TEST_POINT_NAME_ADD			0x8180004
#define TEST_POINT_NUMBER_ADD		0x8185A04
//                                    
#define OPEN_TOTAL_ADD				0x8185FA4
#define OPEN_TESTPOINT1_ADD 		0x8185FA8
#define OPEN_TESTPOINT2_ADD 		0x8186548
#define OPEN_TEST_RANGE_ADD 		0x8186AE8
#define OPEN_TEST_SPEC_ADD 			0x8187088
#define OPEN_TEST_OK_COUNT_ADD 		0x8187BC8
#define OPEN_TEST_NG_COUNT_ADD 		0x8188168
//                                    
#define SHORT_TOTAL_ADD				0x8188708
#define SHORT_TEST_POINT_ADD		0x818870C
#define SHORT_TEST_GROUP_ADD		0x8188CAC
#define SHORT_TEST_RANGE_ADD		0x818924C
#define SHORT_TEST_SPEC_ADD			0x8189250
#define SHORT_TEST_OK_COUNT_ADD		0x8189254
#define SHORT_TEST_NG_COUNT_ADD		0x8189258
//                                    
#define SHEX_TOTAL_ADD				0x818925C
#define SHEX_TESTPOINT1_ADD 		0x8189260
#define SHEX_TESTPOINT2_ADD 		0x8189800
#define SHEX_TEST_RANGE_ADD 		0x8189DA0
#define SHEX_TEST_SPEC_ADD 			0x818A340
#define SHEX_TEST_OK_COUNT_ADD 		0x818AE80
#define SHEX_TEST_NG_COUNT_ADD 		0x818B420
//                                    
#define PART_TOTAL_ADD				0x818B9C0
#define PART_TEST_NAME_ADD			0x818B9C4
#define PART_TESTPOINT1_ADD 		0x81913C4
#define PART_TESTPOINT2_ADD 		0x8191964
#define PART_TEST_RANGE_ADD			0x8191F04
#define PART_TEST_UPPER_ADD			0x81924A4
#define PART_TEST_LOWER_ADD			0x8192FE4
#define PART_TEST_OK_COUNT_ADD		0x8193B24
#define PART_TEST_NG_COUNT_ADD 		0x81940C4
//	                                  
#define FUNC_TOTAL_ADD				0x8194664
#define FUNC_TEST_NAME_ADD			0x8194668
#define FUNC_TEST_TYPE_ADD 			0x8195CE8
#define FUNC_TEST_MODE_ADD 			0x8195E50
#define FUNC_TEST_SPEC1_ADD			0x8195FB8
#define FUNC_TEST_SPEC2_ADD			0x8196120
#define FUNC_TEST_SPEC3_ADD			0x81963F0
#define FUNC_TEST_SPEC4_ADD			0x81966C0
#define FUNC_TEST_LOG_ADD			0x8197200
#define FUNC_TEST_PANELLINK_ADD		0x8197368
//                                    
#define PJ_MAIN_NAME_ADD			0x81974D0
#define PJ_SUB_NAME_ADD				0x8197584
//                                    
#define MODE1_NAME_ADD				0x8197638
#define MODE1_SEQ_ADD				0x8197658
#define MODE1_RUN_ADD				0x819765C
#define MODE1_NUBMER_ADD    		0x8197724
#define MODE1_STATE_ADD				0x81977EC
#define MODE1_TIME_ADD				0x81978B4
//                                    
#define MODE2_NAME_ADD				0x819797C
#define MODE2_SEQ_ADD				0x819799C
#define MODE2_RUN_ADD				0x81979A0
#define MODE2_NUBMER_ADD			0x8197A68
#define MODE2_STATE_ADD				0x8197B30
#define MODE2_TIME_ADD				0x8197BF8
//                                    
#define MODE3_NAME_ADD				0x8197CC0
#define MODE3_SEQ_ADD				0x8197CE0
#define MODE3_RUN_ADD				0x8197CE4
#define MODE3_NUBMER_ADD    		0x8197DAC
#define MODE3_STATE_ADD				0x8197E74
#define MODE3_TIME_ADD				0x8197F3C
//                                    
#define MODE4_NAME_ADD				0x8198004
#define MODE4_SEQ_ADD				0x8198024
#define MODE4_RUN_ADD				0x8198028
#define MODE4_NUBMER_ADD			0x81980F0
#define MODE4_STATE_ADD				0x81981B8
#define MODE4_TIME_ADD				0x8198280
//                                    
#define INIT_NAME_ADD				0x8198348
#define INIT_SEQ_ADD				0x8198368
#define INIT_RUN_ADD				0x819836C
#define INIT_NUBMER_ADD				0x8198434
#define INIT_STATE_ADD				0x81984FC
#define INIT_TIME_ADD				0x81985C4
//                                    
#define SH_AUTO_P_ADD				0x819868C
#define SH_AUTO_N_ADD				0x8198690
//                                    
#define SPEC_SAVE_FLAG_ADD			0x8198694
#define SPEC_SAVE_YEAR_ADD			0x8198698
#define SPEC_SAVE_MONTH_ADD			0x819869C
#define SPEC_SAVE_DAY_ADD 			0x81986A0
#define SPEC_SAVE_HOUR_ADD			0x81986A4
#define SPEC_SAVE_MIN_ADD 			0x81986A8
//                                      
#define SPEC_SAVE_FLAG				(*((uint32_t *)SPEC_SAVE_FLAG_ADD))
#define SPEC_SAVE_YEAR				(*((uint32_t *)SPEC_SAVE_YEAR_ADD))
#define SPEC_SAVE_MONTH				(*((uint32_t *)SPEC_SAVE_MONTH_ADD))
#define SPEC_SAVE_DAY				(*((uint32_t *)SPEC_SAVE_DAY_ADD))
#define SPEC_SAVE_HOUR				(*((uint32_t *)SPEC_SAVE_HOUR_ADD))
#define SPEC_SAVE_MIN				(*((uint32_t *)SPEC_SAVE_MIN_ADD))

#define SPEC_CHECK_FLAG				0x53504543	//SPEC

//////////////////////////////////////////////////////////////
// Sector21	0x081A0000	 0x081BFFFF 	128	TSP Panel SPEC
// Sector22	0x081C0000	 0x081DFFFF 	128	TSP Panel SPEC
// Sector23	0x081E0000	 0x081FFFFF 	128 TSP Panel SPEC
//////////////////////////////////////////////////////////////
#define TSP01_MIN_ADD				0x81A0000	//sector21
#define TSP02_MIN_ADD				0x81A0E10	
#define TSP03_MIN_ADD				0x81A1C20	
#define TSP04_MIN_ADD				0x81A2A30	
#define TSP05_MIN_ADD				0x81A3840	
#define TSP06_MIN_ADD				0x81A4650	
#define TSP07_MIN_ADD				0x81A5460	
#define TSP08_MIN_ADD				0x81A6270	
#define TSP09_MIN_ADD				0x81A7080	
#define TSP10_MIN_ADD				0x81A7E90	
#define TSP11_MIN_ADD				0x81A8CA0	
#define TSP12_MIN_ADD				0x81A9AB0	
#define TSP13_MIN_ADD				0x81AA8C0	
#define TSP14_MIN_ADD				0x81AB6D0	
#define TSP15_MIN_ADD				0x81AC4E0	
#define TSP16_MIN_ADD				0x81AD2F0	
#define TSP17_MIN_ADD				0x81AE100	
#define TSP18_MIN_ADD				0x81AEF10	
#define TSP19_MIN_ADD				0x81AFD20	
#define TSP20_MIN_ADD				0x81B0B30	
#define TSP21_MIN_ADD				0x81B1940	
#define TSP22_MIN_ADD				0x81B2750	
#define TSP23_MIN_ADD				0x81B3560	
#define TSP24_MIN_ADD				0x81B4370	
#define TSP25_MIN_ADD				0x81B5180	
#define TSP26_MIN_ADD				0x81B5F90	
#define TSP27_MIN_ADD				0x81B6DA0	
#define TSP28_MIN_ADD				0x81B7BB0	
#define TSP29_MIN_ADD				0x81B89C0	
#define TSP30_MIN_ADD				0x81B97D0	
#define TSP31_MIN_ADD				0x81BA5E0	
#define TSP32_MIN_ADD				0x81BB3F0	
#define TSP33_MIN_ADD				0x81BC200	
#define TSP34_MIN_ADD				0x81BD010	
#define TSP35_MIN_ADD				0x81BDE20	
#define TSP36_MIN_ADD				0x81BEC30	
#define TSP37_MIN_ADD				0x81BFA40	
#define TSP38_MIN_ADD				0x81C0850	//sector22
#define TSP39_MIN_ADD				0x81C1660	
#define TSP40_MIN_ADD				0x81C2470	
#define TSP41_MIN_ADD				0x81C3280	
#define TSP42_MIN_ADD				0x81C4090	
#define TSP43_MIN_ADD				0x81C4EA0	
#define TSP44_MIN_ADD				0x81C5CB0	
#define TSP45_MIN_ADD				0x81C6AC0	
#define TSP46_MIN_ADD				0x81C78D0	
#define TSP47_MIN_ADD				0x81C86E0	
#define TSP48_MIN_ADD				0x81C94F0	
#define TSP49_MIN_ADD				0x81CA300	
#define TSP50_MIN_ADD				0x81CB110	
#define TSP01_MAX_ADD				0x81CBF20	
#define TSP02_MAX_ADD				0x81CCD30	
#define TSP03_MAX_ADD				0x81CDB40	
#define TSP04_MAX_ADD				0x81CE950	
#define TSP05_MAX_ADD				0x81CF760	
#define TSP06_MAX_ADD				0x81D0570	
#define TSP07_MAX_ADD				0x81D1380	
#define TSP08_MAX_ADD				0x81D2190	
#define TSP09_MAX_ADD				0x81D2FA0	
#define TSP10_MAX_ADD				0x81D3DB0	
#define TSP11_MAX_ADD				0x81D4BC0	
#define TSP12_MAX_ADD				0x81D59D0	
#define TSP13_MAX_ADD				0x81D67E0	
#define TSP14_MAX_ADD				0x81D75F0	
#define TSP15_MAX_ADD				0x81D8400	
#define TSP16_MAX_ADD				0x81D9210	
#define TSP17_MAX_ADD				0x81DA020	
#define TSP18_MAX_ADD				0x81DAE30	
#define TSP19_MAX_ADD				0x81DBC40	
#define TSP20_MAX_ADD				0x81DCA50	
#define TSP21_MAX_ADD				0x81DD860	
#define TSP22_MAX_ADD				0x81DE670	
#define TSP23_MAX_ADD				0x81DF480	
#define TSP24_MAX_ADD				0x81E0290	//sector23
#define TSP25_MAX_ADD				0x81E10A0	
#define TSP26_MAX_ADD				0x81E1EB0	
#define TSP27_MAX_ADD				0x81E2CC0	
#define TSP28_MAX_ADD				0x81E3AD0	
#define TSP29_MAX_ADD				0x81E48E0	
#define TSP30_MAX_ADD				0x81E56F0	
#define TSP31_MAX_ADD				0x81E6500	
#define TSP32_MAX_ADD				0x81E7310	
#define TSP33_MAX_ADD				0x81E8120	
#define TSP34_MAX_ADD				0x81E8F30	
#define TSP35_MAX_ADD				0x81E9D40	
#define TSP36_MAX_ADD				0x81EAB50	
#define TSP37_MAX_ADD				0x81EB960	
#define TSP38_MAX_ADD				0x81EC770	
#define TSP39_MAX_ADD				0x81ED580	
#define TSP40_MAX_ADD				0x81EE390	
#define TSP41_MAX_ADD   			0x81EF1A0	
#define TSP42_MAX_ADD   			0x81EFFB0	
#define TSP43_MAX_ADD   			0x81F0DC0	
#define TSP44_MAX_ADD   			0x81F1BD0	
#define TSP45_MAX_ADD   			0x81F29E0	
#define TSP46_MAX_ADD   			0x81F37F0	
#define TSP47_MAX_ADD   			0x81F4600	
#define TSP48_MAX_ADD   			0x81F5410	
#define TSP49_MAX_ADD   			0x81F6220	
#define TSP50_MAX_ADD   			0x81F7030	
#define TSP_TT_NUM_ADD				0x81F7E40	
#define TSP_TX_NUM_ADD				0x81F7E44	
#define TSP_RX_NUM_ADD				0x81F7EA8	
#define TSP_SAVE_FLAG_ADD			0x81F7F0C	
#define TSP_SAVE_YEAR_ADD			0x81F7F10	
#define TSP_SAVE_MONTH_ADD			0x81F7F14	
#define TSP_SAVE_DAY_ADD			0x81F7F18	
#define TSP_SAVE_HOUR_ADD			0x81F7F1C	
#define TSP_SAVE_MIN_ADD			0x81F7F20	//end 81F7F24
                                                	
#define TSP_SAVE_FLAG				(*((uint32_t *)TSP_SAVE_FLAG_ADD))
#define TSP_SAVE_YEAR				(*((uint32_t *)TSP_SAVE_YEAR_ADD))
#define TSP_SAVE_MONTH				(*((uint32_t *)TSP_SAVE_MONTH_ADD))
#define TSP_SAVE_DAY				(*((uint32_t *)TSP_SAVE_DAY_ADD))
#define TSP_SAVE_HOUR				(*((uint32_t *)TSP_SAVE_HOUR_ADD))
#define TSP_SAVE_MIN				(*((uint32_t *)TSP_SAVE_MIN_ADD))

#define TSP_CHECK_FLAG				0x54535053	//TSPS
	
//////////////////////////////////////////////////////////////
// Sector12	0x08100000	 0x08103FFF 	16	Input Data Spec
//////////////////////////////////////////////////////////////
#define DATA_SAVE_FLAG_ADD			0x8100000
#define DATA_SAVE_YEAR_ADD			0x8100004
#define DATA_SAVE_MONTH_ADD			0x8100008
#define DATA_SAVE_DAY_ADD 			0x810000C
#define DATA_SAVE_HOUR_ADD			0x8100010
#define DATA_SAVE_MIN_ADD 			0x8100014
#define DATA_SIZE_ADD 				0x8100018
#define DATA_CRC32_ADD 				0x810001C
#define DATA_NAME_ADD 				0x8100020
        
#define DATA_SAVE_FLAG				(*((uint32_t *) DATA_SAVE_FLAG_ADD))
#define DATA_SAVE_YEAR				(*((uint32_t *) DATA_SAVE_YEAR_ADD))
#define DATA_SAVE_MONTH				(*((uint32_t *) DATA_SAVE_MONTH_ADD))
#define DATA_SAVE_DAY				(*((uint32_t *) DATA_SAVE_DAY_ADD))
#define DATA_SAVE_HOUR				(*((uint32_t *) DATA_SAVE_HOUR_ADD))
#define DATA_SAVE_MIN				(*((uint32_t *) DATA_SAVE_MIN_ADD))
#define DATA_CRC32					(*((uint32_t *) DATA_CRC32_ADD))
        
#define INPUTDATA_ADDRESS   	(uint32_t)0x08104000
		
#define DATA_CHECK_FLAG				0x44415441 // DATA
//------------------------------------------------------LGD PMIC
//Sector09 0x080A0000               0x080BFFFF			128	Reserve #1
//--------------------------------------------------------------
#define PMIC_SAVE_FLAG_ADD			0x080A0000   
#define	PMIC_SAVE_YEAR_ADD			0x080A0004   
#define PMIC_SAVE_MONTH_ADD			0x080A0008
#define PMIC_SAVE_DAY_ADD			0x080A000C
#define PMIC_SAVE_HOUR_ADD			0x080A0010
#define PMIC_SAVE_MIN_ADD			0x080A0014
#define PMIC_SIZE_ADD 				0x080A0018
#define PMIC_CRC32_ADD				0x080A001C
#define PMIC_NAME_ADD				0x080A0020

#define PMIC_SAVE_FLAG				(*((uint32_t *) PMIC_SAVE_FLAG_ADD))
#define PMIC_SAVE_YEAR				(*((uint32_t *) PMIC_SAVE_YEAR_ADD))
#define PMIC_SAVE_MONTH				(*((uint32_t *) PMIC_SAVE_MONTH_ADD))
#define PMIC_SAVE_DAY				(*((uint32_t *) PMIC_SAVE_DAY_ADD))
#define PMIC_SAVE_HOUR				(*((uint32_t *) PMIC_SAVE_HOUR_ADD))
#define PMIC_SAVE_MIN				(*((uint32_t *) PMIC_SAVE_MIN_ADD))
#define PMIC_CRC32					(*((uint32_t *) PMIC_CRC32_ADD))

#define PMICDATA_ADDRESS 			(uint32_t)0x080A4000
#define PMIC_CHECK_FLAG				0x98127348 //

//------------------------------------------------------LGD TPIC
//Sector09-2 0x080E0000              0x080FFFFF			128	Reserve #3
//--------------------------------------------------------------
#define TPIC_SAVE_FLAG_ADD			0x080E0000   
#define	TPIC_SAVE_YEAR_ADD			0x080E0004   
#define TPIC_SAVE_MONTH_ADD			0x080E0008
#define TPIC_SAVE_DAY_ADD			0x080E000C
#define TPIC_SAVE_HOUR_ADD			0x080E0010
#define TPIC_SAVE_MIN_ADD			0x080E0014
#define TPIC_SIZE_ADD 				0x080E0018
#define TPIC_CRC32_ADD				0x080E001C
#define TPIC_NAME_ADD				0x080E0020

#define TPIC_SAVE_FLAG				(*((uint32_t *) TPIC_SAVE_FLAG_ADD))
#define TPIC_SAVE_YEAR				(*((uint32_t *) TPIC_SAVE_YEAR_ADD))
#define TPIC_SAVE_MONTH				(*((uint32_t *) TPIC_SAVE_MONTH_ADD))
#define TPIC_SAVE_DAY				(*((uint32_t *) TPIC_SAVE_DAY_ADD))
#define TPIC_SAVE_HOUR				(*((uint32_t *) TPIC_SAVE_HOUR_ADD))
#define TPIC_SAVE_MIN				(*((uint32_t *) TPIC_SAVE_MIN_ADD))
#define TPIC_CRC32					(*((uint32_t *) TPIC_CRC32_ADD))

#define TPICDATA_ADDRESS 			(uint32_t)0x080E4000
#define TPIC_CHECK_FLAG				0x59871457 //

//----------------------------------------------------------------------------LGD EEPROM
//Sector10 0x080C0000               0x080DFFFF			128	Reserve #2
//---------------------------------------------------------------------------
#define EEP_SAVE_FLAG_ADD			0x080C0000 
#define	EEP_SAVE_YEAR_ADD			0x080C0004   
#define EEP_SAVE_MONTH_ADD			0x080C0008
#define EEP_SAVE_DAY_ADD			0x080C000C
#define EEP_SAVE_HOUR_ADD			0x080C0010
#define EEP_SAVE_MIN_ADD			0x080C0014
#define EEP_SIZE_ADD 				0x080C0018
#define EEP_CRC32_ADD				0x080C001C
#define EEP_NAME_ADD				0x080C0020

#define EEP_SAVE_FLAG				(*((uint32_t *) EEP_SAVE_FLAG_ADD))
#define EEP_SAVE_YEAR				(*((uint32_t *) EEP_SAVE_YEAR_ADD))
#define EEP_SAVE_MONTH				(*((uint32_t *) EEP_SAVE_MONTH_ADD))
#define EEP_SAVE_DAY				(*((uint32_t *) EEP_SAVE_DAY_ADD))
#define EEP_SAVE_HOUR				(*((uint32_t *) EEP_SAVE_HOUR_ADD))
#define EEP_SAVE_MIN				(*((uint32_t *) EEP_SAVE_MIN_ADD))
#define EEP_CRC32					(*((uint32_t *) EEP_CRC32_ADD))

#define EEPDATA_ADDRESS 			(uint32_t)0x080C4000
#define EEP_CHECK_FLAG				0x32758412 //
//------------------------------------------------------
// tp info                                                          
extern uint16_t tp_count;
extern char 	 tp_name[720][32];
extern uint16_t tp_number[720];
// open info                        
extern uint16_t open_count;
extern uint16_t open_tp1[720];
extern uint16_t open_tp2[720];
extern uint16_t open_range[720];
extern float 	 open_spec[720];
extern uint16_t open_ok_cnt[720];
extern uint16_t open_ng_cnt[720];
// short base info                  
extern uint16_t short_bs_count;
extern uint16_t short_bs_tp[720];
extern uint16_t short_bs_group[720];
extern uint16_t short_bs_range;
extern float 	 short_bs_spec;
extern uint16_t short_bs_ok_cnt;
extern uint16_t short_bs_ng_cnt;
// short extand info                
extern uint16_t short_ex_count;
extern uint16_t short_ex_tp1[720];
extern uint16_t short_ex_tp2[720];
extern uint16_t short_ex_range[720];
extern float 	 short_ex_spec[720];
extern uint16_t short_ex_ok_cnt[720];
extern uint16_t short_ex_ng_cnt[720];
// Part info                        
extern uint16_t part_count;
extern char 	 part_name[720][32];
extern uint16_t part_tp1[720];
extern uint16_t part_tp2[720];
extern uint16_t part_range[720];
extern float 	 part_spec_hi[720];
extern float 	 part_spec_lo[720];
extern uint16_t part_ok_cnt[720];
extern uint16_t part_ng_cnt[720];
// Func info                        
extern uint16_t func_count;
extern char 	 func_name[180][32];
extern uint16_t func_type[180];
extern uint16_t func_module[180];
extern uint16_t func_spec_dec[180];
extern uint8_t  func_spec_hex[180][4];
extern float    func_spec_float[180];
extern char	 func_spec_ascii[180][16];
extern uint16_t func_log_flag[180];
extern uint16_t func_panel_link[180];
// name info                        
extern char 	 project_name_main[180];
extern char 	 project_name_sub[180];
//                                  
extern char     mode1_name[32]; 
extern uint16_t mode1_sequence; 
extern uint16_t mode1_operation[100]; 
extern uint16_t mode1_number[100]; 
extern uint16_t mode1_status[100]; 
extern uint16_t mode1_time[100]; 
//                                  
extern char     mode2_name[32]; 
extern uint16_t mode2_sequence; 
extern uint16_t mode2_operation[100]; 
extern uint16_t mode2_number[100]; 
extern uint16_t mode2_status[100]; 
extern uint16_t mode2_time[100]; 
//                                  
extern char     mode3_name[32]; 
extern uint16_t mode3_sequence; 
extern uint16_t mode3_operation[100]; 
extern uint16_t mode3_number[100]; 
extern uint16_t mode3_status[100]; 
extern uint16_t mode3_time[100]; 
//                                  
extern char     mode4_name[32]; 
extern uint16_t mode4_sequence; 
extern uint16_t mode4_operation[100]; 
extern uint16_t mode4_number[100]; 
extern uint16_t mode4_status[100]; 
extern uint16_t mode4_time[100]; 
//                                  
extern char     init_name[32]; 
extern uint16_t init_sequence; 
extern uint16_t init_operation[100]; 
extern uint16_t init_number[100]; 
extern uint16_t init_status[100]; 
extern uint16_t init_time[100];   
//                                  
extern uint16_t short_auto_pp;   
extern uint16_t short_auto_mm;
//Test Result_TOTAL                       
extern char	 test_item_name[2048][50];       
extern char	 test_meas_string[2048][50];  
extern char	 test_meas_string1[2048][50];      
extern float	 test_meas_value[2048];       
extern char	 test_result[2048];       
extern uint16_t test_count;
//                                  
extern char	 	test_ng_item_name[2048][50];       
extern char	 	test_ng_meas_string[2048][50];       
extern char	 	test_ng_result[2048];       
extern uint16_t test_ng_count;   
//
extern int16_t  panel_min[50][1800];
extern int16_t  panel_max[50][1800];
//
extern uint16_t panel_count;	
extern uint16_t panel_tx_num[50];	
extern uint16_t panel_rx_num[50];	
//	
extern int16_t  panel_data[50][1800];	
extern float    panel_dataf[50][1800];
extern int16_t  ErrorNodeCount[50];

extern uint16_t short_tt_count;

extern int16_t sdc_data[50][50];
extern float  sdc_dataf[50][50];

extern char pc_txbuff[128*1024];
extern char pc_rxbuff[128*1024];
extern char valid_rxbuff[128*1024];
extern char log_txbuff[128*1024];
extern char uart_str1[128*1024];
extern char uart_str2[128*1024];
extern char uart_str3[128*1024];
extern char StrCat_buff[128*1024]; 

extern unsigned long info_crc;
extern unsigned long tp_crc;
extern unsigned long open_crc;
extern unsigned long short_crc;
extern unsigned long shortex_crc;
extern unsigned long part_crc;
extern unsigned long func_crc;
extern unsigned long mode1_crc;
extern unsigned long mode2_crc;
extern unsigned long mode3_crc;
extern unsigned long mode4_crc;
extern unsigned long init_crc;
extern unsigned long panel_crc;

typedef struct 
{
    int32_t  TT;
    int32_t  OK;
    int32_t  NG;
}qty;
extern qty QTY;

typedef struct
{
	int 	 size;
	char 	 *name;
	uint32_t crc32;
}Input_data;
extern Input_data In_data;
extern uint8_t *INPUT_DATA;
//--------------------------
extern Input_data In_pmic;
extern uint8_t *INPUT_PMIC;
extern Input_data In_eep;
extern uint8_t *INPUT_EEP;
extern Input_data In_tpic;
extern uint8_t *INPUT_TPIC;
//---------------------------
#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

