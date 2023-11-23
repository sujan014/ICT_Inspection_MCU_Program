#include "Hal_TIMER.h"

void Sys_Timer2(void)	// 1ms
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    TIM_Cmd(TIM2, DISABLE);   
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);    // Enable TIM2 Update interrupt  	

    TIM_TimeBaseStructure.TIM_Period                = 100-1;   // 100 *  10usec = 1msec
    TIM_TimeBaseStructure.TIM_Prescaler             = 840-1;    // (1/84Mhz)*840  = 10usec	
    TIM_TimeBaseStructure.TIM_ClockDivision         = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode           = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);    
    TIM_SetCounter(TIM2,0); 
 	
    TIM_Cmd(TIM2, ENABLE);  
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);    // Enable TIM2 Update interrupt
}

void Sys_Timer3(char on)	// cap
{
	if(on == false )
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		TIM_Cmd(TIM3, DISABLE);  
		TIM_SetCounter(TIM3,0);  
	}
	else
	{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		
		TIM_Cmd(TIM3, DISABLE);   	
 
		TIM_TimeBaseStructure.TIM_Period                = 51000-1;  	// overflow	12.5ms short
		TIM_TimeBaseStructure.TIM_Prescaler             = 21-1;        	// (1/84Mhz)*21 = 0.25 usec
		TIM_TimeBaseStructure.TIM_ClockDivision         = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode           = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
			
		TIM_SetCounter(TIM3,0);
		TIM_Cmd(TIM3, ENABLE);  
	}
}

void Sys_Timer4(void)	//1ms
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    TIM_Cmd(TIM4, DISABLE);   
    TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);    // Enable TIM3 Update interrupt  	
    TIM_TimeBaseStructure.TIM_Period                = 50-1;   // 50 *  10usec = 500usec
    TIM_TimeBaseStructure.TIM_Prescaler             = 840-1;    // (1/84Mhz)*840  = 10usec	
    TIM_TimeBaseStructure.TIM_ClockDivision         = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode           = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);    
    TIM_SetCounter(TIM4,0); 
 	
    TIM_Cmd(TIM4, ENABLE);  
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);    // Enable TIM2 Update interrupt
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

