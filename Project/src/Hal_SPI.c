#include "Hal_SPI.h"

void SPI2_Init(void) 
{ 
    SPI_InitTypeDef 	SPI_InitStruct;	
    GPIO_InitTypeDef	GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);     // APB1CLK = 42Mhz, APB1Timer =  84Mhz

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    SPI_I2S_DeInit(SPI2);
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master; 
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; 
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;     
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
//    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; 
//    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; 
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; 
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		// clk = 42 /  16  =   2.625 Mhz
    // SPI_BaudRatePrescaler_2    // clk = 42 /   2  =  21.0 Mhz
    // SPI_BaudRatePrescaler_4    // clk = 42 /   4  =  10.5 Mhz
    // SPI_BaudRatePrescaler_8    // clk = 42 /   8  =  5.25 Mhz
    // SPI_BaudRatePrescaler_16   // clk = 42 /  16  =   2.625 Mhz
    // SPI_BaudRatePrescaler_32   // clk = 42 /  32  =   1.312 Mhz
    // SPI_BaudRatePrescaler_64   // clk = 42 /  64  =   0.656 Mhz
    // SPI_BaudRatePrescaler_128  // clk = 42 / 128  =   0.328 Mhz
    // SPI_BaudRatePrescaler_256  // clk = 42 / 256  =   0.164 Mhz  
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; 
    SPI_InitStruct.SPI_CRCPolynomial = 7; 
    SPI_Init(SPI2, &SPI_InitStruct);
    SPI_Cmd(SPI2, ENABLE); 
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
		GPIO_Init(GPIOB, &GPIO_InitStructure);           
    IO_EXPAND_CS(1);
} 

void SPI1_Init(void) 
{ 
    SPI_InitTypeDef SPI_InitStruct;	
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);    // APB2CLK = 84Mhz, APB2Timer = 168Mhz

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    SPI_I2S_DeInit(SPI1);
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master; 
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;     
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;     
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; 
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    // SPI_BaudRatePrescaler_2    // clk = 84 /   2  =  42.0 Mhz
    // SPI_BaudRatePrescaler_4    // clk = 84 /   4  =  21.0 Mhz
    // SPI_BaudRatePrescaler_8    // clk = 84 /   8  =  10.5 Mhz
    // SPI_BaudRatePrescaler_16   // clk = 84 /  16  =   5.2 Mhz
    // SPI_BaudRatePrescaler_32   // clk = 84 /  32  =   2.6 Mhz
    // SPI_BaudRatePrescaler_64   // clk = 84 /  64  =   1.3 Mhz
    // SPI_BaudRatePrescaler_128  // clk = 84 / 128  =   0.6 Mhz
    // SPI_BaudRatePrescaler_256  // clk = 84 / 256  =   0.3 Mhz  
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; 
    SPI_InitStruct.SPI_CRCPolynomial = 7; 
    SPI_Init(SPI1, &SPI_InitStruct);
    SPI_Cmd(SPI1, ENABLE); 
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	SPI1_CS(1);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
