#include "Hal_GPIO.h"

void GPIO_Initial(void)
{
	GPIO_InitTypeDef GPO;
	GPIO_InitTypeDef GPI;
		
	GPO.GPIO_Mode  = GPIO_Mode_OUT;
	GPO.GPIO_OType = GPIO_OType_PP;
	GPO.GPIO_Speed = GPIO_Speed_100MHz;
	GPO.GPIO_PuPd  = GPIO_PuPd_UP;
	
	GPI.GPIO_Mode  = GPIO_Mode_IN;
	GPI.GPIO_OType = GPIO_OType_PP;
	GPI.GPIO_Speed = GPIO_Speed_100MHz;
	GPI.GPIO_PuPd  = GPIO_PuPd_UP;
	
	//[PORTA]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPO.GPIO_Pin = GPIO_Pin_0;	    GPIO_Init(GPIOA, &GPO);	// PA00		PA.0
	GPO.GPIO_Pin = GPIO_Pin_1;	    GPIO_Init(GPIOA, &GPO);	// PA01		PA.1
	GPO.GPIO_Pin = GPIO_Pin_5;	    GPIO_Init(GPIOA, &GPO);	// PA05		I_SEL_A1
	GPO.GPIO_Pin = GPIO_Pin_6;	    GPIO_Init(GPIOA, &GPO);	// PA06		I_SEL_A2
	GPO.GPIO_Pin = GPIO_Pin_7;	    GPIO_Init(GPIOA, &GPO);	// PA07		I_SEL_A3
	GPO.GPIO_Pin = GPIO_Pin_8;		GPIO_Init(GPIOA, &GPO);	// PA08		/I_SEL_EN
	
	//[PORTB]
	GPO.GPIO_Pin = GPIO_Pin_2;		GPIO_Init(GPIOB, &GPO);	// PA08		/I_SEL_EN
	
	//[PORTC]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPI.GPIO_Pin = GPIO_Pin_1;	    GPIO_Init(GPIOC, &GPI);	// PC01		/CAP_CHK_IN
	GPO.GPIO_Pin = GPIO_Pin_4;	    GPIO_Init(GPIOC, &GPO);	// PC04		MUX_PA0
	GPO.GPIO_Pin = GPIO_Pin_5;	    GPIO_Init(GPIOC, &GPO);	// PC05		MUX_PA1
	GPO.GPIO_Pin = GPIO_Pin_6;	    GPIO_Init(GPIOC, &GPO);	// PC06		MUX_PA2
	GPO.GPIO_Pin = GPIO_Pin_7;	    GPIO_Init(GPIOC, &GPO);	// PC07		MUX_NA0
	GPO.GPIO_Pin = GPIO_Pin_8;	    GPIO_Init(GPIOC, &GPO);	// PC08		MUX_NA1
	GPO.GPIO_Pin = GPIO_Pin_9;	    GPIO_Init(GPIOC, &GPO);	// PC09		MUX_NA2
	GPI.GPIO_Pin = GPIO_Pin_12;		GPIO_Init(GPIOC, &GPI);	// PC12		/SAFE_SEN_IN
	GPO.GPIO_Pin = GPIO_Pin_13;		GPIO_Init(GPIOC, &GPO);	// PC13		PC.13
	
	//[PORTD]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPO.GPIO_Pin = GPIO_Pin_2;	    GPIO_Init(GPIOD, &GPO);	// PD02		NCS_A1
	GPO.GPIO_Pin = GPIO_Pin_3;	    GPIO_Init(GPIOD, &GPO);	// PD03		NCS_A2
	GPO.GPIO_Pin = GPIO_Pin_4;	    GPIO_Init(GPIOD, &GPO);	// PD04		NCS_A3
	GPO.GPIO_Pin = GPIO_Pin_6;	    GPIO_Init(GPIOD, &GPO);	// PD06		/NCS_A_EN
	GPO.GPIO_Pin = GPIO_Pin_7;	    GPIO_Init(GPIOD, &GPO);	// PD07		/PCS_A_EN
	GPO.GPIO_Pin = GPIO_Pin_11;		GPIO_Init(GPIOD, &GPO);	// PD11		PCS_A1
	GPO.GPIO_Pin = GPIO_Pin_12;		GPIO_Init(GPIOD, &GPO);	// PD12		PCS_A2
	GPO.GPIO_Pin = GPIO_Pin_13;		GPIO_Init(GPIOD, &GPO);	// PD13		PCS_A3
	
	//[PORTE]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPO.GPIO_Pin = GPIO_Pin_2;	    GPIO_Init(GPIOE, &GPO);	// PE02		STATE_LED	
	
	//[PORTG]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPO.GPIO_Pin = GPIO_Pin_2;	    GPIO_Init(GPIOG, &GPO);	// PG02		MUX_D1
	GPO.GPIO_Pin = GPIO_Pin_3;	    GPIO_Init(GPIOG, &GPO);	// PG03		MUX_D2
	GPO.GPIO_Pin = GPIO_Pin_9;	    GPIO_Init(GPIOG, &GPO);	// PG09		RECHARGE_EN
	GPO.GPIO_Pin = GPIO_Pin_13;		GPIO_Init(GPIOG, &GPO);	// PG13		MUX_D3
	GPO.GPIO_Pin = GPIO_Pin_14;		GPIO_Init(GPIOG, &GPO);	// PG14		MUX_D4
	
	//[PORTH]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	GPO.GPIO_Pin = GPIO_Pin_4;	    GPIO_Init(GPIOH, &GPO);	// PH04		/I_GAIN10_EN
	
	//[PORTI]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
	GPO.GPIO_Pin = GPIO_Pin_3;	    GPIO_Init(GPIOI, &GPO);	// PI03		MUX_D5
	GPO.GPIO_Pin = GPIO_Pin_6;	    GPIO_Init(GPIOI, &GPO);	// PI06		MUX_D6
	GPO.GPIO_Pin = GPIO_Pin_7;	    GPIO_Init(GPIOI, &GPO);	// PI07		MUX_D7
	GPO.GPIO_Pin = GPIO_Pin_8;	    GPIO_Init(GPIOI, &GPO);	// PI08		MUX_D8
	GPO.GPIO_Pin = GPIO_Pin_11;		GPIO_Init(GPIOI, &GPO);	// PI11		/273_CLR
	
	PA_0(0); 		
	PA_1(0);
	PC_13(0);		

	RECHARGE_EN(0);
	I_GAIN10_EN(1);
	//
	I_SEL_EN(1);
	I_SEL_A1(0);
	I_SEL_A2(0);
	I_SEL_A3(0);
	//PSC CONTROL
	PCS_A_EN(1);
	PCS_A1(0);
	PCS_A2(0);
	PCS_A3(0);
	//NSC CONTROL
	NCS_A_EN(1);
	NCS_A1(0);
	NCS_A2(0);
	NCS_A3(0);
	//PA CONTROL
	MUX_PA0(0);
	MUX_PA1(0);
	MUX_PA2(0);
	//NA CONTROL
	MUX_NA0(0);
	MUX_NA1(0); 
	MUX_NA2(0);
	
	#ifdef VHC4051
		MUX_D1(1);
		MUX_D2(1);
		MUX_D3(1);
		MUX_D4(1);
		MUX_D5(1);
		MUX_D6(1); 
		MUX_D7(1);
		MUX_D8(1);
	#endif
	
	#ifdef DG408LE
		MUX_D1(0);
		MUX_D2(0);
		MUX_D3(0);
		MUX_D4(0);
		MUX_D5(0);
		MUX_D6(0); 
		MUX_D7(0);
		MUX_D8(0);
	#endif
	
	CLR_273(1);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
