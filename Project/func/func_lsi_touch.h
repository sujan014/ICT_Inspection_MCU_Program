#ifndef _FUNC_LSI_TOUCH_H__
#define _FUNC_LSI_TOUCH_H__

#include "main.h"

uint8_t SDC_Calibration(void);
uint8_t Cm_Offset(uint16_t index);
uint8_t Cm_Offset_Gap(uint16_t index);
uint8_t Cs_Offset_X(uint16_t index);
uint8_t Cs_Offset_Y(uint16_t index);

uint8_t Cm_Set(uint16_t index);
uint8_t Cs_Set_X(uint16_t index);
uint8_t Cs_Set_Y(uint16_t index);
uint8_t Cm_Set8(uint16_t index);

uint8_t Noise_Check(void);
uint8_t CMR_P2P(uint16_t index);
uint8_t CMR_P2P_MM(uint16_t index);
uint8_t CSR_X_P2P(uint16_t index);
uint8_t CSR_X_P2P_MM(uint16_t index);
uint8_t CSR_Y_P2P(uint16_t index);
uint8_t CSR_Y_P2P_MM(uint16_t index);

uint8_t nNoise_Check_New(void);		
uint8_t nCMR_P2P_AVR(uint16_t index);		
uint8_t nCMR_P2P_MM(uint16_t index);		
uint8_t nCMR_P2P_GAP(uint16_t index);		
uint8_t nCSR_X_P2P_AVR(uint16_t index);
uint8_t nCSR_Y_P2P_AVR(uint16_t index);
uint8_t nCSR_X_P2P_GAP(uint16_t index);
uint8_t nCSR_Y_P2P_GAP(uint16_t index);

uint8_t TX_SH_GND(uint16_t index);
uint8_t RX_SH_GND(uint16_t index);
uint8_t TX_SH_TX(uint16_t index);
uint8_t RX_SH_RX(uint16_t index);
uint8_t TX_SH_RX(uint16_t index);
uint8_t RX_SH_TX(uint16_t index);
uint8_t TX_SH_GND_GAP(uint16_t index);
uint8_t RX_SH_GND_GAP(uint16_t index);
uint8_t TX_SH_TX_GAP(uint16_t index);
uint8_t RX_SH_RX_GAP(uint16_t index);
uint8_t TX_SH_RX_GAP(uint16_t index);
uint8_t RX_SH_TX_GAP(uint16_t index);
uint8_t SHO_TEST(uint16_t index);

uint8_t CM2_Offset(uint16_t index);
uint8_t CM2_Offset_Gap(uint16_t index);
uint8_t CM3_Offset(uint16_t index);
uint8_t CM3_Offset_Gap(uint16_t index);

uint8_t Diff_Test1(uint16_t index);
uint8_t Diff_Test2(uint16_t index);
uint8_t Diff_Test3(uint16_t index);
uint8_t Diff_Test4(uint16_t index);

uint8_t Diff_Cm_Base(uint16_t index);		
uint8_t Diff_Cm_PP(uint16_t index);		

uint8_t Diff_Cs_Base_Tx(uint16_t index);	
uint8_t Diff_Cs_Base_Rx(uint16_t index);	
uint8_t Diff_Offset1_Tx(uint16_t index);	
uint8_t Diff_Offset1_Rx(uint16_t index);	
uint8_t Diff_Offset2_Tx(uint16_t index);	
uint8_t Diff_Offset2_Rx(uint16_t index);	

uint8_t Diff_Offset_Abs_Tx(uint16_t index);
uint8_t Diff_Offset_Abs_Rx(uint16_t index);
uint8_t Diff_Offset_Gap_Tx(uint16_t index);
uint8_t Diff_Offset_Gap_Rx(uint16_t index);


#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

