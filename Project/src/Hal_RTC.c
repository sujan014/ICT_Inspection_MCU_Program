#include "Hal_RTC.h"

__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

void RTC_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
	PWR_BackupRegulatorCmd(ENABLE);

    RCC_LSEConfig(RCC_LSE_ON);
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    SynchPrediv  = 0xFF;  AsynchPrediv = 0x7F;
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();
    RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, DISABLE);    
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);	
}

//BKPSRAM_BASE
void RTC_Initial(void)
{
    if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
    {     
        RTC_Config();
		RTC_DateStr.RTC_Year 	= 21;
		RTC_DateStr.RTC_Month 	= 1;   
		RTC_DateStr.RTC_Date 	= 31;      
        RTC_DateStr.RTC_WeekDay = 1;   
        RTC_TimeStr.RTC_Hours   = 24;  
		RTC_TimeStr.RTC_Minutes = 30;  
		RTC_TimeStr.RTC_Seconds = 30;    
		
        if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr)!= ERROR && RTC_SetDate(RTC_Format_BIN, &RTC_DateStr) != ERROR) 	
			RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
    }
    else
    {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		PWR_BackupAccessCmd(ENABLE);
		PWR_BackupRegulatorCmd(ENABLE);
		
        RTC_WaitForSynchro();
        RTC_ClearFlag(RTC_FLAG_ALRAF);
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);		
    } 
}

//4kb sram
uint8_t bk_sram_read(uint32_t ReadAddress)
{
    __IO uint32_t read_pointer = (uint32_t)ReadAddress;
	return *(__IO uint8_t *)(BKPSRAM_BASE+read_pointer);
}

void bk_sram_write(uint32_t WriteAddress, uint8_t data)
{
	__IO uint32_t write_pointer = (uint32_t)WriteAddress;
	*(uint8_t *) (BKPSRAM_BASE + write_pointer) = data;        // Transfer data to the memory
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

