#include "Hal_SDRAM.h"

#define SDRAM_TIMEOUT     ((uint32_t)0xFFFF)  

static void sram_delay(__IO uint32_t nCount);

void Sdram_Initial(void)
{
    FMC_SDRAMInitTypeDef  FMC_SDRAMInitStructure;
    FMC_SDRAMTimingInitTypeDef  FMC_SDRAMTimingInitStructure; 

    SDRAM_GPIOConfig();

    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);

    // FMC Configuration 
    // FMC SDRAM Bank configuration
    FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay    = 2;    // TMRD: 2 Clock cycles 
    FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay = 6;    // TXSR: min=70ns (7x11.11ns) 
    FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime      = 4;    // TRAS: min=42ns (4x11.11ns) max=120k (ns)
    FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay        = 6;    // TRC:  min=70 (7x11.11ns)       
    FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime    = 2;    // TWR:  min=1+ 7ns (1+1x11.11ns) 
    FMC_SDRAMTimingInitStructure.FMC_RPDelay              = 2;    // TRP:  20ns => 2x11.11ns 
    FMC_SDRAMTimingInitStructure.FMC_RCDDelay             = 2;    // TRCD: 20ns => 2x11.11ns 

    FMC_SDRAMInitStructure.FMC_Bank               = FMC_Bank2_SDRAM;            // FMC SDRAM control configuration 
    FMC_SDRAMInitStructure.FMC_ColumnBitsNumber   = FMC_ColumnBits_Number_8b;   // Column addressing: [8:0] 
    FMC_SDRAMInitStructure.FMC_RowBitsNumber      = FMC_RowBits_Number_12b;     // Row addressing: [11:0] 
    FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth  = SDRAM_MEMORY_WIDTH;
    FMC_SDRAMInitStructure.FMC_InternalBankNumber = FMC_InternalBank_Number_4;
    FMC_SDRAMInitStructure.FMC_CASLatency         = FMC_CAS_Latency_3; 
    FMC_SDRAMInitStructure.FMC_WriteProtection    = FMC_Write_Protection_Disable;
    FMC_SDRAMInitStructure.FMC_SDClockPeriod      = FMC_SDClock_Period_2;
    FMC_SDRAMInitStructure.FMC_ReadBurst          = FMC_Read_Burst_Disable;
    FMC_SDRAMInitStructure.FMC_ReadPipeDelay      = FMC_ReadPipe_Delay_1;
    FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct  = &FMC_SDRAMTimingInitStructure;

    FMC_SDRAMInit(&FMC_SDRAMInitStructure);     // FMC SDRAM bank initialization 

    SDRAM_InitSequence();
}

void SDRAM_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |
                           RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH, ENABLE);
                            
    //--------------------+--------------------+--------------------+--------------------+
    //+                       SDRAM pins assignment                                      +
    //+-------------------+--------------------+--------------------+--------------------+
    //| PD0  <-> FMC_D2   | PE0  <-> FMC_NBL0  | PF0  <-> FMC_A0    | PG0  <-> FMC_A10   |
    //| PD1  <-> FMC_D3   | PE1  <-> FMC_NBL1  | PF1  <-> FMC_A1    | PG1  <-> FMC_A11   |
    //| PD8  <-> FMC_D13  | PE7  <-> FMC_D4    | PF2  <-> FMC_A2    | PG8  <-> FMC_SDCLK |
    //| PD9  <-> FMC_D14  | PE8  <-> FMC_D5    | PF3  <-> FMC_A3    | PG15 <-> FMC_NCAS  |
    //| PD10 <-> FMC_D15  | PE9  <-> FMC_D6    | PF4  <-> FMC_A4    |--------------------+ 
    //| PD14 <-> FMC_D0   | PE10 <-> FMC_D7    | PF5  <-> FMC_A5    | PG4  <-> FMC_BA0   |
    //| PD15 <-> FMC_D1   | PE11 <-> FMC_D8    | PF11 <-> FMC_NRAS  | PG5  <-> FMC_BA1   | 
    //+-------------------| PE12 <-> FMC_D9    | PF12 <-> FMC_A6    |--------------------|
    //                    | PE13 <-> FMC_D10   | PF13 <-> FMC_A7    | PH7 <-> FMC_SDCKE1 |    
    //                    | PE14 <-> FMC_D11   | PF14 <-> FMC_A8    | PH6 <-> FMC_SDNE1  | 
    //                    | PE15 <-> FMC_D12   | PF15 <-> FMC_A9    | PH5 <-> FMC_SDNWE  |
    //+-------------------+--------------------+--------------------+--------------------+
    // Common GPIO configuration
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    // GPIOH configuration 
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource5, GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource6, GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource7, GPIO_AF_FMC);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6  | GPIO_Pin_7 ;
    GPIO_Init(GPIOH, &GPIO_InitStructure); 

    // GPIOD configuration 
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1  | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;

    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // GPIOE configuration 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource0 ,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource1 ,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 ,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 ,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 ,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_7 | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

    GPIO_Init(GPIOE, &GPIO_InitStructure);

    // GPIOF configuration 
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource0 ,  GPIO_AF_FMC);   
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1 ,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource2 ,  GPIO_AF_FMC);  
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource3 ,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource4 ,  GPIO_AF_FMC);   
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource5 ,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource11 , GPIO_AF_FMC);  
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource12 , GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource13 , GPIO_AF_FMC);  
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource14 , GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource15 , GPIO_AF_FMC);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3  | GPIO_Pin_4  | GPIO_Pin_5 |
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; 
                                
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    // GPIOG configuration 
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource0 ,  GPIO_AF_FMC); 
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1 ,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource4 ,  GPIO_AF_FMC); 
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource5 ,  GPIO_AF_FMC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource8 ,  GPIO_AF_FMC); 
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource15 , GPIO_AF_FMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_15;

    GPIO_Init(GPIOG, &GPIO_InitStructure);   
}

void SDRAM_InitSequence(void)
{
    FMC_SDRAMCommandTypeDef FMC_SDRAMCommandStructure;
    uint32_t tmpr = 0;
    uint32_t timeout = SDRAM_TIMEOUT; 

    // Step 3
    // Configure a clock configuration enable command
    FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_CLK_Enabled;
    FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank2;
    FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
    FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;
    // Wait until the SDRAM controller is ready
    while((FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
    {
        timeout--;
    }
    // Send the command
    FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);  

    // Step 4 
    // Insert 100 ms delay
    sram_delay(100);

    // Step 5 
    // Configure a PALL (precharge all) command 
    FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_PALL;
    FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank2;
    FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
    FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;

    // Wait until the SDRAM controller is ready 
    timeout = SDRAM_TIMEOUT; 
    while((FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
    {
        timeout--;
    }
    // Send the command
    FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

    // Step 6 
    // Configure a Auto-Refresh command 
    FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_AutoRefresh;
    FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank2;
    FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 4;
    FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;

    // Wait until the SDRAM controller is ready
    timeout = SDRAM_TIMEOUT; 
    while((FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
    {
        timeout--;
    }
    // Send the command 
    FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

    // Step 7 
    // Program the external memory mode register
    tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2          |
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                     SDRAM_MODEREG_CAS_LATENCY_3           |
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

    // Configure a load Mode register command
    FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_LoadMode;
    FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank2;
    FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
    FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = tmpr;

    // Wait until the SDRAM controller is ready
    timeout = SDRAM_TIMEOUT; 
    while((FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
    {
        timeout--;
    }
    // Send the command 
    FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

    // Step 8 
    //  Set the refresh rate counter
    //  (7.81 us x Freq) - 20 = (7.81 * 84MHz) - 20 = 683
    //  Set the device refresh counter
    
    // Set the refresh rate counter 
    // (15.62 us x Freq) - 20 
    // Set the device refresh counter 
    //FMC_SetRefreshCount(683);
    FMC_SetRefreshCount(1293);

    // Wait until the SDRAM controller is ready 
    timeout = SDRAM_TIMEOUT; 
    while((FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
    {
        timeout--;
    }
}

void SDRAM_SetRefresh(void)
{

}

void SDRAM_WriteBuffer(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize)
{
    __IO uint32_t write_pointer = (uint32_t)uwWriteAddress;

    FMC_SDRAMWriteProtectionConfig(FMC_Bank2_SDRAM, DISABLE);    // Disable write protection 

    // Wait until the SDRAM controller is ready
    while(FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET){}

    // While there is data to write 
    for (; uwBufferSize != 0; uwBufferSize--) 
    {
        *(uint32_t *) (SDRAM_BANK_ADDR + write_pointer) = *pBuffer++;        // Transfer data to the memory
        write_pointer += 4;        // Increment the address
    }    
}

void SDRAM_ReadBuffer(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize)
{
    __IO uint32_t write_pointer = (uint32_t)uwReadAddress;

    // Wait until the SDRAM controller is ready 
    while(FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET){}

    // Read data
    for(; uwBufferSize != 0x00; uwBufferSize--)
    {
        *pBuffer++ = *(__IO uint32_t *)(SDRAM_BANK_ADDR + write_pointer );
        write_pointer += 4;    // Increment the address
    } 
}

static void sram_delay(__IO uint32_t nCount)
{
    __IO uint32_t index = 0; 
    for(index = (100000 * nCount); index != 0; index--){}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
