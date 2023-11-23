#include "Hal_ADC.h"

// 해당 파일 수정하지 말것!!
/**
  * @brief  검사 관련 ADC 채널 세팅
  * @param  None
  * @retval 수정하지 말것!!
  */
void AdcInit(void)
{   
	ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    GPIO_InitTypeDef      GPIO_InitStructure;

    ADC_DeInit();
	
	////////////////////////////
	// measure adc port	
	// ADC1 ch10 ~ ch13
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2| GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    ADC_CommonInitStructure.ADC_Mode             = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler        = ADC_Prescaler_Div4;
    ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;    
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_Resolution 			 = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode 			 = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode 	 = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge 	 = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv 		 = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign 			 = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion 		 = 1;
 
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE); 
	////////////////////////////
	// sensor adc port
	// ADC3 ch6,ch7,ch8,ch9,
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
  	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    ADC_CommonInitStructure.ADC_Mode             = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler        = ADC_Prescaler_Div4;
    ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    
	ADC_Init(ADC3, &ADC_InitStructure);
	ADC_Cmd(ADC3, ENABLE); 
	
}

static void Measure_ADC_CH_Init(uint8_t CH)
{
	ADC_RegularChannelConfig(ADC1, CH, 1, ADC_SampleTime_3Cycles); 
}

uint16_t Measure_adc_read(uint8_t no)
{       
    uint32_t time_out = 0xffffff;

	ADC_SoftwareStartConv(ADC1); 
	
    while(1)
    {
		if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET)	
			return ADC_GetConversionValue(ADC1);
        
		if(time_out-- == 0)	
			return 0;
    }
}

uint16_t Measure_adc_value(uint8_t no, uint8_t cnt)
{
	uint32_t SUM_ADC  = 0;
	uint32_t SUM_TEMP = 0;
	uint8_t adc_sum_cnt;
	
	Measure_ADC_CH_Init(no);
	
	for(adc_sum_cnt = 0; adc_sum_cnt < cnt ; adc_sum_cnt++)
	{
		SUM_TEMP = Measure_adc_read(no);
		SUM_ADC += SUM_TEMP;
	}
	SUM_ADC = SUM_ADC / cnt;
	
	return (uint16_t)SUM_ADC;
}

void Sensor_ADC_CH_Init(uint8_t CH)
{
	ADC_RegularChannelConfig(ADC3, CH, 1, ADC_SampleTime_3Cycles); 
}

uint16_t Sensor_adc_read(uint8_t no)
{       
    uint32_t time_out = 1000;

	ADC_SoftwareStartConv(ADC3); 
	
    while(1)
    {		
		if(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == SET)	
			return ADC_GetConversionValue(ADC3);
        
		if(time_out-- == 0)	
			return 0;
    }
}

uint16_t Sensor_adc_value(uint8_t no, uint8_t cnt)
{
	uint32_t SUM_ADC  = 0;
	uint32_t SUM_TEMP = 0;
	uint8_t adc_sum_cnt;
	
	Sensor_ADC_CH_Init(no);
	
	for(adc_sum_cnt = 0; adc_sum_cnt < cnt ; adc_sum_cnt++)
	{
		SUM_TEMP =Sensor_adc_read(no);
		SUM_ADC += SUM_TEMP;
	}
	SUM_ADC = SUM_ADC / cnt;
	
	return (uint16_t)SUM_ADC;
}

#define EVCC_R1	43
#define EVCC_R2	4.99
double Evcc1_Vlot =0;	
double Evcc2_Vlot =0;
void EVCC_Check(void)
{
	char text[50];
	EVCC_RY(true);

	Delay_ms(50);
	EvccControl(1, true);	
	EvccControl(2, true);	
	Delay_ms(50);

	Evcc1_Vlot = ( ( (((double)Sensor_adc_value(4,200)/4096.0)*2.5)*((EVCC_R1+EVCC_R2)) ) / (double)(EVCC_R2) );	
	Evcc2_Vlot = ( ( (((double)Sensor_adc_value(5,200)/4096.0)*2.5)*((EVCC_R1+EVCC_R2)) ) / (double)(EVCC_R2) );

	sprintf(text,"%.2lf V", Evcc1_Vlot);
	hItem = WM_GetDialogItem(hSyswin4,MENU4_EVCC1);	
	TEXT_SetText(hItem,text);

	sprintf(text,"%.2lf V", Evcc2_Vlot);
	hItem = WM_GetDialogItem(hSyswin4,MENU4_EVCC2);	
	TEXT_SetText(hItem,text);
	
	EvccControl(1, false);
	EvccControl(2, false);
	EVCC_RY(false);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

