#include "Hal_I2C.h"

//#define I2C_CLOCK_STRETCH false
#define I2C_CLOCK_STRETCH true

uint16_t I2C_TIME = 2;
//uint16_t I2C_TIME = 100;	//1000;
	
#define SCL_LOW 		(GPIOB->BSRRH = GPIO_Pin_6)
#define SDA_LOW 		(GPIOB->BSRRH = GPIO_Pin_7)

#define SCL_INPUT 		(GPIOB->IDR & GPIO_Pin_6)
#define SDA_INPUT 		(GPIOB->IDR & GPIO_Pin_7)

#define SDA_IN_PORT 	{ GPIOB->MODER &= ~(3 << (7*2)); GPIOB->MODER |= 0 << (7*2); }
#define SDA_OUT_PORT	{ GPIOB->MODER &= ~(3 << (7*2)); GPIOB->MODER |= 1 << (7*2); }

#define SCL_IN_PORT 	{ GPIOB->MODER &= ~(3 << (6*2)); GPIOB->MODER |= 0 << (6*2); }
#define SCL_OUT_PORT	{ GPIOB->MODER &= ~(3 << (6*2)); GPIOB->MODER |= 1 << (6*2); }
//------------------------------------------------- LGD
#define SCL2_LOW 		(GPIOB->BSRRH = GPIO_Pin_3)
#define SDA2_LOW 		(GPIOB->BSRRH = GPIO_Pin_4)

#define SCL2_INPUT 		(GPIOB->IDR & GPIO_Pin_3)
#define SDA2_INPUT 		(GPIOB->IDR & GPIO_Pin_4)

#define SDA2_IN_PORT 	{ GPIOB->MODER &= ~(3 << (4*2)); GPIOB->MODER |= 0 << (4*2); }
#define SDA2_OUT_PORT	{ GPIOB->MODER &= ~(3 << (4*2)); GPIOB->MODER |= 1 << (4*2); }

#define SCL2_IN_PORT 	{ GPIOB->MODER &= ~(3 << (3*2)); GPIOB->MODER |= 0 << (3*2); }
#define SCL2_OUT_PORT	{ GPIOB->MODER &= ~(3 << (3*2)); GPIOB->MODER |= 1 << (3*2); }
//-------------------------------------------------

void I2C_Delay(__IO u32 nCount)
{
  //for (; nCount != 0;nCount--); 
  Delay_us(nCount);
}

void touch_i2c_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	touch_i2c_sda(1);
	touch_i2c_scl(1);  
}

void touch_i2c_sda(uint8_t i)  
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	
	if (i)
	{ 
		GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_7;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	else
	{
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_7;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
    
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	}
}

void touch_i2c_scl(uint8_t i)
{
	uint32_t timeout = 3000;
	GPIO_InitTypeDef    GPIO_InitStructure;

	if (i) 
	{ 

		GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		#if I2C_CLOCK_STRETCH
      while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
      {
        if(timeout-- == 1) break;
      }; 
		#endif
	}
	else
	{ 
    
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
    
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	}   
}

void touch_i2c_start(void)
{
   touch_i2c_sda(1);   Delay_us(I2C_TIME);
   touch_i2c_scl(1);   Delay_us(I2C_TIME);
   touch_i2c_sda(0);   Delay_us(I2C_TIME);
   touch_i2c_scl(0);   Delay_us(I2C_TIME);
}

void i2c1_restart(void)
{
   touch_i2c_sda(1);   Delay_us(I2C_TIME);
   touch_i2c_scl(1);   Delay_us(I2C_TIME);
   touch_i2c_sda(0);   Delay_us(I2C_TIME);
   touch_i2c_scl(0);   Delay_us(I2C_TIME);
}

void touch_i2c_stop(void)
{
   touch_i2c_sda(0);	Delay_us(I2C_TIME);
   touch_i2c_scl(1);	Delay_us(I2C_TIME);
   touch_i2c_sda(1);	Delay_us(I2C_TIME);
}

uint8_t touch_i2c_tx(unsigned char d)
{
	char x;
	static char b;
	
	for( x = 8; x; x--)
	{
	if( d & 0x80 ) 	touch_i2c_sda(1);  
	else 			touch_i2c_sda(0);
    
    Delay_us(I2C_TIME); touch_i2c_scl(1);
	Delay_us(I2C_TIME); touch_i2c_scl(0);
	d <<= 1;
	}
	Delay_us(I2C_TIME);  
	touch_i2c_sda(1); Delay_us(I2C_TIME); 
	
	touch_i2c_scl(1); Delay_us(I2C_TIME); 
	b = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	touch_i2c_scl(0);
  
	return b;	
}

uint8_t touch_i2c_rx(char ack)
{
	char x, d=0;
	touch_i2c_sda(1);  Delay_us(I2C_TIME); 
	for( x = 0; x < 8; x++) 
	{
		d <<= 1;       //MSB FIRST
		touch_i2c_scl(1); Delay_us(I2C_TIME); 
    
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) d |= 1;
		 touch_i2c_scl(0);Delay_us(I2C_TIME);
    
	} 
  Delay_us(I2C_TIME);
	if(ack)		touch_i2c_sda(0);
	else 		touch_i2c_sda(1);
  Delay_us(I2C_TIME);
  
	touch_i2c_scl(1);	Delay_us(I2C_TIME);
	touch_i2c_scl(0); 
	touch_i2c_sda(1);
	return d;
}

void I2C1_Init(void)
{	

}
///////
//// I2C1 Define
//////
void I2C1_SDA(uint8_t i)
{	
	GPIO_InitTypeDef    GPIO_InitStructure;
	
	if (i)
	{ 
		GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_7;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	else
	{
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_7;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
    
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	}
}

void I2C1_SCL(uint8_t i)
{
   uint32_t timeout = 3000;
	GPIO_InitTypeDef    GPIO_InitStructure;

	if (i) 
	{ 

		GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		#if I2C_CLOCK_STRETCH
      while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
      {
        if(timeout-- == 1) break;
      }; 
		#endif
	}
	else
	{ 
    
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
    
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	} 
}

void i2c1_start(void)
{
   touch_i2c_sda(1);   Delay_us(I2C_TIME);
   touch_i2c_scl(1);   Delay_us(I2C_TIME);
   touch_i2c_sda(0);   Delay_us(I2C_TIME);
   touch_i2c_scl(0);   Delay_us(I2C_TIME);
}

void i2c1_stop(void)
{
   touch_i2c_sda(0);	Delay_us(I2C_TIME);
   touch_i2c_scl(1);	Delay_us(I2C_TIME);
   touch_i2c_sda(1);	Delay_us(I2C_TIME);
}

uint8_t i2c1_tx(uint8_t d)
{
  char x;
	static char b;
	
	for( x = 8; x; x--)
	{
		if( d & 0x80 ) 	touch_i2c_sda(1);  
		else 			touch_i2c_sda(0);
    
		Delay_us(I2C_TIME/2); touch_i2c_scl(1);
		Delay_us(I2C_TIME); touch_i2c_scl(0);
		Delay_us(I2C_TIME/2);
		d <<= 1;
	}
	Delay_us(I2C_TIME);  
	touch_i2c_sda(1); Delay_us(I2C_TIME); 
	
	touch_i2c_scl(1); Delay_us(I2C_TIME); 
	b = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	touch_i2c_scl(0);
  
	return b;                            	
}

uint8_t i2c1_rx(uint8_t ack)
{
	char x, d=0;
	touch_i2c_sda(1);  Delay_us(I2C_TIME); 
	for( x = 0; x < 8; x++) 
	{
		d <<= 1;       //MSB FIRST
		touch_i2c_scl(1); Delay_us(I2C_TIME/2); 
    
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) d |= 1;
		Delay_us(I2C_TIME/2); 
		 touch_i2c_scl(0);Delay_us(I2C_TIME);
    
	} 
  Delay_us(I2C_TIME);
	if(ack)		touch_i2c_sda(0);
	else 		touch_i2c_sda(1);
  Delay_us(I2C_TIME);
  
	touch_i2c_scl(1);	Delay_us(I2C_TIME);
	touch_i2c_scl(0); 
	touch_i2c_sda(1);
	return d;
}

void led_i2c_sda(uint8_t i)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	
	if (i)
	{ 
		GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_0;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_0;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
    
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	}
}

void led_i2c_scl(uint8_t i)
{
	uint32_t timeout = 3000;
	GPIO_InitTypeDef    GPIO_InitStructure;

	if (i) 
	{ 

		GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_1;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		#if I2C_CLOCK_STRETCH
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
		{
			if(timeout-- == 1) break;
		}; 
		#endif
	}
	else
	{ 
    
		GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_1;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
    
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	}   
}

void led_i2c_start(void)
{
   led_i2c_sda(1);   I2C_Delay(I2C_TIME);
   led_i2c_scl(1);   I2C_Delay(I2C_TIME);
   led_i2c_sda(0);   I2C_Delay(I2C_TIME);
   led_i2c_scl(0);   I2C_Delay(I2C_TIME);
}

void led_i2c_stop(void)
{
   led_i2c_sda(0);	I2C_Delay(I2C_TIME);
   led_i2c_scl(1);	I2C_Delay(I2C_TIME);
   led_i2c_sda(1);	I2C_Delay(I2C_TIME);
}

uint8_t led_i2c_tx(unsigned char d)
{
	char x;
	static char b;
	
	for( x = 8; x; x--)
	{
		if( d & 0x80 ) 	led_i2c_sda(1);  
		else 			led_i2c_sda(0);
    
		I2C_Delay(I2C_TIME); led_i2c_scl(1);
		I2C_Delay(I2C_TIME); led_i2c_scl(0);
		d <<= 1;
	}
	I2C_Delay(I2C_TIME);  
	led_i2c_sda(1); I2C_Delay(I2C_TIME); 
	
	led_i2c_scl(1); I2C_Delay(I2C_TIME); 
	b = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
	led_i2c_scl(0);
  
	return b;	
}

uint8_t led_i2c_rx(uint8_t ack)
{
	char x, d=0;
	led_i2c_sda(1);  I2C_Delay(I2C_TIME); 
	for( x = 0; x < 8; x++) 
	{
		d <<= 1;       //MSB FIRST
		led_i2c_scl(1); I2C_Delay(I2C_TIME); 
    
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)) d |= 1;
		 led_i2c_scl(0);I2C_Delay(I2C_TIME);
    
	} 
	I2C_Delay(I2C_TIME);
	if(ack)		led_i2c_sda(0);
	else 		led_i2c_sda(1);
	I2C_Delay(I2C_TIME);
  
	led_i2c_scl(1);	I2C_Delay(I2C_TIME);
	led_i2c_scl(0); 
	led_i2c_sda(1);
	return d;
}


uint8_t olb_led_control(uint8_t ch, uint8_t mes)
{
	led_i2c_start();	I2C_Delay(5);
	
	if(led_i2c_tx( 0x9E & 0xFE ) != 0)
	{
		dbgmsg("Slave Addr Write Err\n"); 
		return false; 
	}	
		
	switch(ch)
	{
		case 1:
			if(led_i2c_tx(0x10) != 0)
			{
				dbgmsg("Addr Write Err\n"); 
				return false; 
			}	
		break;
		case 2: 
			if(led_i2c_tx(0x12) != 0)
			{
				dbgmsg("Addr Write Err\n"); 
				return false; 
			}	
			
		break;
		case 3: 
			led_i2c_tx(0x14);	
			if(led_i2c_tx(0x14) != 0)
			{
				dbgmsg("Addr Write Err\n"); 
				return false; 
			}	
		break;
		case 4: 
			led_i2c_tx(0x16);
			if(led_i2c_tx(0x16) != 0)
			{
				dbgmsg("Addr Write Err\n"); 
				return false; 
			}	
		
		break;
		default:
		break;	
	}	
	I2C_Delay(2);			
	
	if(led_i2c_tx(mes) != 0)
	{
		dbgmsg("data Write Err1\n"); 
		return false; 
	}				
	if(led_i2c_tx(0) != 0)
	{
		dbgmsg("data Write Err2\n"); 
		return false; 
	}	
	
	led_i2c_stop();	
	
	return true; 
}

void I2C2_Init(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	// scl out mode
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_100MHz;
	//GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// sda out mode
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_100MHz;
	//GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_4;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void I2C2_SDA(uint8_t level)
{	
	if(level)	
	{
		SDA2_IN_PORT;	
	}
	else		
	{	
		SDA2_OUT_PORT;
		SDA2_LOW;	
	}
}
void I2C2_SCL(uint8_t level)
{
	if(level)
	{
		SCL2_IN_PORT;
		#if I2C_CLOCK_STRETCH
			while(SCL2_INPUT == 0){};
		#endif
	}
	else
	{
		SCL2_OUT_PORT;
		SCL2_LOW;
	}   
}
void i2c2_start(void)
{
   I2C2_SDA(1);   
   I2C2_SCL(1);   I2C_Delay(I2C_TIME);
   I2C2_SDA(0);   I2C_Delay(I2C_TIME);
   I2C2_SCL(0);
}
void i2c2_stop(void)
{
   I2C2_SDA(0);	I2C_Delay(I2C_TIME);
   I2C2_SCL(1);	
   I2C2_SDA(1);	
}
uint8_t i2c2_tx(uint8_t data)
{
	uint8_t ack;
	for(int cnt = 8; cnt; cnt--)
	{
		if( data & 0x80 ) 	I2C2_SDA(1);  
		else 				I2C2_SDA(0);
		I2C2_SCL(1);	I2C_Delay(I2C_TIME);
		I2C2_SCL(0);	I2C_Delay(I2C_TIME);
		data <<= 1;		
	}
	I2C2_SDA(1);
	I2C2_SCL(1);	I2C_Delay(I2C_TIME);

	ack = SDA2_INPUT;
	
	I2C2_SCL(0);
	return ack;	
}
uint8_t i2c2_rx(uint8_t ack)
{
	uint8_t data=0;
	I2C2_SDA(1); 
	for( int cnt = 0; cnt < 8; cnt++) 
	{
		data <<= 1;       //MSB FIRST
		I2C2_SCL(1);	I2C_Delay(I2C_TIME);
		if(SDA2_INPUT) data |= 1;
		I2C2_SCL(0);	I2C_Delay(I2C_TIME);
	}
	
	if(ack)		I2C2_SDA(0);
	else 		I2C2_SDA(1);
	
	I2C2_SCL(1);	I2C_Delay(I2C_TIME);
	I2C2_SCL(0);
	I2C2_SDA(1);
	return data;
}

//-------------------------------------melfas
uint8_t melfas_i2c1_tx(unsigned char d)
{
	char x;
	static char b;
	for( x = 8; x; x--)
	{
		if( d & 0x80 ) 	I2C2_SDA(1);  
		else 			I2C2_SDA(0);
		I2C2_SCL(1); 	I2C_Delay(I2C_TIME);
		I2C2_SCL(0);	I2C_Delay(I2C_TIME);
		d <<= 1;
	}
	I2C2_SDA(1);
	I2C2_SCL(1); I2C_Delay(I2C_TIME);

	b = SDA2_INPUT;	
	I2C2_SCL(0);
	return b;	
}

uint8_t melfas_i2c1_rx(char ack)
{
	char x, d=0;
	I2C2_SDA(1); 
	for( x = 0; x < 8; x++) 
	{
		d <<= 1;       //MSB FIRST
		I2C2_SCL(1);   I2C_Delay(I2C_TIME);
		if(SDA2_INPUT) d |= 1;
		I2C2_SCL(0);   I2C_Delay(I2C_TIME);
	} 
	
	if(ack)		I2C2_SDA(0);
	else 		I2C2_SDA(1);
	
	I2C2_SCL(1);	I2C_Delay(I2C_TIME);
	I2C2_SCL(0);
	I2C2_SDA(1);
	return d;
}

//--------------------------------------------

// ----------------> Sujan
// This code is for testing only.
// blocks program until START1 is pressed
void switch_Hold(void)
{
	int kscan = 0;
	return;
	if(RD_DBG_MD == 1) return;
	
	dbgmsg("\nPRESS START1 SWITCH\n");
	
	while(1)
	{
		kscan = KeyScan();
		if (kscan == ST1_ON)	{	Delay_ms(300);	break;}
	}
	dbgmsg("\nSTART1 Press OK\n");
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
