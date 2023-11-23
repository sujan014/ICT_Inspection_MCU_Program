#ifndef __HAL_SPI__H__
#define __HAL_SPI__H__

#include "main.h"

#define IO1_ADDRESS_WRITE   0x40 
#define IO1_ADDRESS_READ    0x41 

#define IO2_ADDRESS_WRITE   0x42 
#define IO2_ADDRESS_READ    0x43 

#define IO3_ADDRESS_WRITE   0x46 
#define IO3_ADDRESS_READ    0x47 

#define IO4_ADDRESS_WRITE   0x4E 
#define IO4_ADDRESS_READ    0x4F 


#define IODIRA        0x00      //Data Direction Register for PORTA    
#define IODIRB        0x01      //Data Direction Register for PORTB 
#define IPOLA         0x02      //Input Polarity Register for PORTA 
#define IPOLB         0x03      //Input Polarity Register for PORTB 
#define GPINTENA      0x04      //Interrupt-on-change enable Register for PORTA 
#define GPINTENB      0x05      //Interrupt-on-change enable Register for PORTB 
#define DEFVALA       0x06      //Default Value Register for PORTA    
#define DEFVALB       0x07      //Default Value Register for PORTB      
#define INTCONA       0x08      //Interrupt-on-change control Register for PORTA  
#define INTCONB       0x09      //Interrupt-on-change control Register for PORTB      
#define IOCON         0x0A      //Configuration register for device                      
#define GPPUA         0x0C      //100kOhm pullup resistor register for PORTA (sets pin to input when set)    
#define GPPUB         0x0D      //100kOhm pullup resistor register for PORTB (sets pin to input when set)      
#define INTFA         0x0E      //Interrupt flag Register for PORTA        
#define INTFB         0x0F      //Interrupt flag Register for PORTB    
#define INTCAPA       0x10      //Interrupt captured value Register for PORTA  
#define INTCAPB       0x11      //Interrupt captured value Register for PORTB    
#define GPIOAAA       0x12      //General purpose I/O Register for PORTA  
#define GPIOBBB       0x13      //General purpose I/O Register for PORTB 
#define OLATA         0x14      //Output latch Register for PORTA 
#define OLATB         0x15      //Output latch Register for PORTB  

#define IO_EXPAND_CS(x)	x ? GPIO_SetBits(GPIOB,GPIO_Pin_12) : GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define SPI1_CS(x)		x ? GPIO_SetBits(GPIOB,GPIO_Pin_2) : GPIO_ResetBits(GPIOB,GPIO_Pin_2)

void SPI1_Init(void);
void SPI2_Init(void);
	
#endif //__HAL_SPI__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
