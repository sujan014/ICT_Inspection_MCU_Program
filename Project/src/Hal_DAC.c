#include "Hal_DAC.h"

void CapDacInit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    DAC_InitStructure.DAC_Trigger         = DAC_Trigger_Software;
    DAC_InitStructure.DAC_WaveGeneration  = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer    = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);    
    DAC_Cmd(DAC_Channel_1, ENABLE);
	
    DacOut(0);	
}

void DacOut(float voltage)
{
    uint16_t DAC_CODE = 0;
	float DAC_REF_VOLT = 2.5;    
	DAC_CODE = (uint16_t)(((voltage * 4096) / (float)(DAC_REF_VOLT))-1);
    DAC_SetChannel1Data(DAC_Align_12b_R, DAC_CODE);	
	
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

