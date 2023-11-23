#ifndef __HAL_ADS7846__H__
#define __HAL_ADS7846__H__

#include "main.h"

#define TOUCH_PENIRQ	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)

typedef struct Matrix 
{						
long double An,  
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
} Matrix ;


#define AX  (-0.127017)
#define BX  (-0.000027)
#define CX  (492.357377)
#define DX  (0.002377)
#define EX  (-0.076110)
#define FX  (278.436386)


typedef	struct POINT 
{
    uint16_t x;
    uint16_t y;
}Coordinate;

extern Coordinate  display;

void LCDTouch_Init(void);
void ADS_XY(int *getXX, int *getYY);
Coordinate *Read_Screen(void);
FunctionalState getDisplayPoint(Coordinate * displayPtr, Coordinate * screenPtr);

void Touch_Cal_First(void);
void Touch_Cal_Menu(void);

#endif //__HAL_AD7846__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

