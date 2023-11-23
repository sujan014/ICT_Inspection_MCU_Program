#include "stm32fxxx_it.h"

extern USB_OTG_CORE_HANDLE USB_OTG_Core;
extern USBH_HOST USB_Host; 
extern void USB_OTG_BSP_TimerIRQ (void);
extern void LTDC_ISR_Handler(void);
extern void DMA2D_ISR_Handler(void);
extern volatile GUI_TIMER_TIME GuiTimer;

int systic_cnt_1usec=0;
int systic_cnt_1msec=0;
void SysTick_Handler(void)
{
    systic_cnt_1usec++;	// 1usec
    if(systic_cnt_1usec == 1000)
    {
        systic_cnt_1usec = 0;
		systic_cnt_1msec++;
        GuiTimer++;
		if(systic_cnt_1msec == 1000)
		{
			systic_cnt_1msec = 0;
			STATE_LED_TG;
		}
    }
    Time_Update();
}

void NMI_Handler(void)
{
	dbgmsg("\nNMI_Handler");
    while(1){};
}

void HardFault_Handler(void)
{
	dbgmsg("\nHardFault_Handler");
    while(1){};
}

void MemManage_Handler(void)
{
	dbgmsg("\nMemManage_Handler");
    while(1){};
}

void BusFault_Handler(void)
{
	dbgmsg("\nBusFault_Handler");
    while(1){}; 
}

void UsageFault_Handler(void)
{
	dbgmsg("\nUsageFault_Handler");
    while (1) {}    //NVIC_SystemReset();  
}

void SVC_Handler(void)
{
	dbgmsg("\nSVC_Handler");
    while(1){};
}

void DebugMon_Handler(void)
{
	dbgmsg("\nDebugMon_Handler");
    while(1){};
}

void PendSV_Handler(void)
{
	dbgmsg("\nPendSV_Handler");
    while(1){};
}

void EXTI1_IRQHandler(void)
{	
}

void TIM2_IRQHandler(void)
{        	
	Time_Update_Timer();
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

void TIM4_IRQHandler(void)
{        	
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}

void OTG_FS_IRQHandler(void)
{
    USBH_OTG_ISR_Handler(&USB_OTG_Core);
}

void LTDC_IRQHandler(void)
{
    LTDC_ISR_Handler();    
}

void DMA2D_IRQHandler(void)
{
    DMA2D_ISR_Handler();    
}

void USART1_IRQHandler(void)
{
	rx_pc_data();
}

void USART2_IRQHandler(void)
{
	//Uart2_Receive_Check();
}

void USART3_IRQHandler(void)
{
	//Uart3_Receive_Check();
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
