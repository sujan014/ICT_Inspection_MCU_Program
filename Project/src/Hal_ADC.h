#ifndef __HAL_ADC__H__
#define __HAL_ADC__H__

#include "main.h"

// measure adc port
// ADC1 ch10 ~ ch13
// sensor adc port
// ADC3 ch6,ch7,ch8,ch9,
void AdcInit(void);
	
uint16_t Measure_adc_value(uint8_t no,uint8_t cnt);
uint16_t Sensor_adc_value(uint8_t no,uint8_t cnt);

extern double Evcc1_Vlot;	
extern double Evcc2_Vlot;

void EVCC_Check(void);

#endif //__HAL_ADC__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
