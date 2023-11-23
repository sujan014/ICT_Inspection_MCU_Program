#ifndef __HAL_GPIO__H__
#define __HAL_GPIO__H__

#include "main.h"

////////////////////////////
#define STATE_LED_TG	GPIO_ToggleBits(GPIOE,GPIO_Pin_2)

#define PA_0(x)			x ? GPIO_SetBits(GPIOA,GPIO_Pin_0):GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define PA_1(x)			x ? GPIO_SetBits(GPIOA,GPIO_Pin_1):GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define PC_13(x)		x ? GPIO_SetBits(GPIOC,GPIO_Pin_13):GPIO_ResetBits(GPIOC,GPIO_Pin_13)

#define SAFE_SEN_IN		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)
//74vhc238(3-8de)
#define I_SEL_A1(x)		x ? GPIO_SetBits(GPIOA,GPIO_Pin_5):GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define I_SEL_A2(x)		x ? GPIO_SetBits(GPIOA,GPIO_Pin_6):GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define I_SEL_A3(x)		x ? GPIO_SetBits(GPIOA,GPIO_Pin_7):GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define I_SEL_EN(x)		x ? GPIO_SetBits(GPIOA,GPIO_Pin_8):GPIO_ResetBits(GPIOA,GPIO_Pin_8)
//74vhc238(3-8de) -> 74vhc273(dff)
#define PCS_A1(x)		x ? GPIO_SetBits(GPIOD,GPIO_Pin_11):GPIO_ResetBits(GPIOD,GPIO_Pin_11)
#define PCS_A2(x)		x ? GPIO_SetBits(GPIOD,GPIO_Pin_12):GPIO_ResetBits(GPIOD,GPIO_Pin_12)
#define PCS_A3(x)		x ? GPIO_SetBits(GPIOD,GPIO_Pin_13):GPIO_ResetBits(GPIOD,GPIO_Pin_13)
#define PCS_A_EN(x)		x ? GPIO_SetBits(GPIOD,GPIO_Pin_7): GPIO_ResetBits(GPIOD,GPIO_Pin_7)
//74vhc238(3-8de) -> 74vhc273(dff)
#define NCS_A1(x)		x ? GPIO_SetBits(GPIOD,GPIO_Pin_2):GPIO_ResetBits(GPIOD,GPIO_Pin_2)
#define NCS_A2(x)		x ? GPIO_SetBits(GPIOD,GPIO_Pin_3):GPIO_ResetBits(GPIOD,GPIO_Pin_3)
#define NCS_A3(x)		x ? GPIO_SetBits(GPIOD,GPIO_Pin_4):GPIO_ResetBits(GPIOD,GPIO_Pin_4)
#define NCS_A_EN(x)		x ? GPIO_SetBits(GPIOD,GPIO_Pin_6):GPIO_ResetBits(GPIOD,GPIO_Pin_6)

#define RECHARGE_EN(x)	x ? GPIO_SetBits(GPIOG,GPIO_Pin_9):GPIO_ResetBits(GPIOG,GPIO_Pin_9)
#define I_GAIN10_EN(x)	x ? GPIO_SetBits(GPIOH,GPIO_Pin_4):GPIO_ResetBits(GPIOH,GPIO_Pin_4)

#define CAP_CHK_IN		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)

#define MUX_PA0(x)		x ? GPIO_SetBits(GPIOC,GPIO_Pin_4):GPIO_ResetBits(GPIOC,GPIO_Pin_4)
#define MUX_PA1(x)		x ? GPIO_SetBits(GPIOC,GPIO_Pin_5):GPIO_ResetBits(GPIOC,GPIO_Pin_5)
#define MUX_PA2(x)		x ? GPIO_SetBits(GPIOC,GPIO_Pin_6):GPIO_ResetBits(GPIOC,GPIO_Pin_6)

#define MUX_NA0(x)		x ? GPIO_SetBits(GPIOC,GPIO_Pin_7):GPIO_ResetBits(GPIOC,GPIO_Pin_7)
#define MUX_NA1(x)		x ? GPIO_SetBits(GPIOC,GPIO_Pin_8):GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define MUX_NA2(x)		x ? GPIO_SetBits(GPIOC,GPIO_Pin_9):GPIO_ResetBits(GPIOC,GPIO_Pin_9)

#define MUX_D1(x)		x ? GPIO_SetBits(GPIOG,GPIO_Pin_2): GPIO_ResetBits(GPIOG,GPIO_Pin_2)
#define MUX_D2(x)		x ? GPIO_SetBits(GPIOG,GPIO_Pin_3): GPIO_ResetBits(GPIOG,GPIO_Pin_3)
#define MUX_D3(x)		x ? GPIO_SetBits(GPIOG,GPIO_Pin_13):GPIO_ResetBits(GPIOG,GPIO_Pin_13)
#define MUX_D4(x)		x ? GPIO_SetBits(GPIOG,GPIO_Pin_14):GPIO_ResetBits(GPIOG,GPIO_Pin_14)
#define MUX_D5(x)		x ? GPIO_SetBits(GPIOI,GPIO_Pin_3): GPIO_ResetBits(GPIOI,GPIO_Pin_3)
#define MUX_D6(x)		x ? GPIO_SetBits(GPIOI,GPIO_Pin_6): GPIO_ResetBits(GPIOI,GPIO_Pin_6)
#define MUX_D7(x)		x ? GPIO_SetBits(GPIOI,GPIO_Pin_7): GPIO_ResetBits(GPIOI,GPIO_Pin_7)
#define MUX_D8(x)		x ? GPIO_SetBits(GPIOI,GPIO_Pin_8): GPIO_ResetBits(GPIOI,GPIO_Pin_8)
#define CLR_273(x)		x ? GPIO_SetBits(GPIOI,GPIO_Pin_11):GPIO_ResetBits(GPIOI,GPIO_Pin_11)


void GPIO_Initial(void);
#endif //__HAL_GPIO__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

