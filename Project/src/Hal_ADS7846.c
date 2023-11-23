#include "Hal_ADS7846.h"

#define TouchCalibrate_ON false
	
#define TOUCH_CS(x)		x ? GPIO_SetBits(GPIOD,GPIO_Pin_5): GPIO_ResetBits(GPIOD,GPIO_Pin_5)

#define THRESHOLD 2

Matrix matrix ;

Coordinate  display;

void TouchPanel_Calibrate(void);

void LCDTouch_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE);

    //ADS7846 CS
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //ADS7846 PENIRQ	
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN ;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);	    
    
    TOUCH_CS(1);
			

	#if TouchCalibrate_ON
		  TouchPanel_Calibrate();		
	#else		
		matrix.An 		  = (-724990.000000);
		matrix.Bn 		  = (12100.0000000);
		matrix.Cn 		  = (-1627039326.000000);
		matrix.Dn 		  = (18360.000000);
		matrix.En 		  = (-498080.000000);
		matrix.Fn 		  = (1594449320.000000);
		matrix.Divider 	= (5166512.000000);
	#endif  
} 

//matrix.An 		= (-589540.000000);
//matrix.Bn 		= (9495.000000);
//matrix.Cn 		= (1947267775.000000);
//matrix.Dn 		= (38250.000000);
//matrix.En 		= (-417715.000000);
//matrix.Fn 		= (1121805215.000000);
//matrix.Divider 	= (3520351.000000);

static void DelayUS(vu32 cnt)
{
	uint32_t i;
	i = cnt * 10;
	while(i--);
}

static void WR_CMD (uint8_t cmd)  
{ 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){};     SPI_I2S_SendData(SPI2,cmd); 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    SPI_I2S_ReceiveData(SPI2); 
} 

static int RD_ADS(void)  
{ 
	uint8_t buf[2];
	int value;
	 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){};     SPI_I2S_SendData(SPI2,0); //dummy	
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    buf[0]=SPI_I2S_ReceiveData(SPI2);  
        
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){};     SPI_I2S_SendData(SPI2,0); //dummy	
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};    buf[1]=SPI_I2S_ReceiveData(SPI2);
	value = (uint16_t)((buf[0] << 8) +buf[1]) >> 3;
		
	return value; 
} 

#define	CHX 	0x90    // 10010011
#define	CHY 	0xd0    // 11010011

int Read_X(void)  
{  
    int temp_x; 
    TOUCH_CS(0);	 	DelayUS(1); 
    WR_CMD(CHX);		DelayUS(1); 
    temp_x=RD_ADS();
    TOUCH_CS(1);
    return temp_x;    
} 

int Read_Y(void)  
{
    int temp_y;
    TOUCH_CS(0);	 	DelayUS(1);
    WR_CMD(CHY);	 	DelayUS(1);
    temp_y=RD_ADS();
    TOUCH_CS(1); 
    return temp_y;   //
}

void ADS_XY(int *getXX, int *getYY)  
{
//    int adx=0, ady=0; 
//	
//	for(int i = 0; i<16; i++)
//	{
//		adx += Read_X();		DelayUS(1);
//		ady += Read_Y();		DelayUS(1);
//	}
//	adx >>= 4;
//	ady >>= 4;
        
    *getXX = Read_Y();
	*getYY = Read_X();
}

Coordinate *Read_Screen(void)
{
    static Coordinate  screen;
    int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
    uint8_t count=0;
    int buffer[2][9]={{0},{0}};

    do
    {		   
        ADS_XY(TP_X,TP_Y);  
        buffer[0][count]=TP_X[0];  
        buffer[1][count]=TP_Y[0];
        count++;
    }
    while( !(TOUCH_PENIRQ) && count < 9);  

    if(count==9)   /* Average X Y  */ 
    {
        /* Average X  */
        temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
        temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
        temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;
        
        m0 = temp[0] - temp[1];
		    m1 = temp[1] - temp[2];
		    m2 = temp[2] - temp[0];
		
        m0 = m0 > 0 ? m0 : (-m0);   
		    m1 = m1 > 0 ? m1 : (-m1);   
		    m2 = m2 > 0 ? m2 : (-m2);
        
        if( m0 > THRESHOLD  &&  m1 > THRESHOLD  &&  m2 > THRESHOLD )	return 0;

        if(m0 < m1)
        {
            if(m2 < m0)		screen.x = (temp[0] + temp[2]) / 2;
            else     		screen.x = (temp[0] + temp[1]) / 2;	
        }
        else if(m2 < m1){	screen.x = (temp[0] + temp[2]) / 2;	}
	     	else  			{	screen.x = (temp[1] + temp[2]) / 2;	}

        /* Average Y  */
        temp[0] = (buffer[1][0] + buffer[1][1] + buffer[1][2]) / 3;
        temp[1] = (buffer[1][3] + buffer[1][4] + buffer[1][5]) / 3;
        temp[2] = (buffer[1][6] + buffer[1][7] + buffer[1][8]) / 3;
                
        m0 = temp[0] - temp[1];
		    m1 = temp[1] - temp[2]; 
		    m2 = temp[2] - temp[0];
		
        m0 = m0 > 0 ? m0 : (-m0); 
	    	m1 = m1 > 0 ? m1 : (-m1); 
	    	m2 = m2 > 0 ? m2 : (-m2);
        
        if( m0 > THRESHOLD  &&  m1 > THRESHOLD  &&  m2 > THRESHOLD )	return 0;

        if(m0 < m1)
        {
            if(m2 < m0)   	screen.y = (temp[0] + temp[2]) / 2;
            else          	screen.y = (temp[0] + temp[1]) / 2;	
        }
        else if(m2 < m1)  	screen.y = (temp[0] + temp[2]) / 2;
        else            	  screen.y = (temp[1] + temp[2]) / 2;
                    
        return &screen;
    }
    return 0;
}


FunctionalState getDisplayPoint(Coordinate * displayPtr, Coordinate * screenPtr )
{
	FunctionalState retTHRESHOLD =ENABLE ;
	/*
	An=168
	*/
	if( matrix.Divider != 0 )
	{
		 
		displayPtr -> x = (uint32_t)(( AX * ( screenPtr -> x ) + BX * ( screenPtr -> y) + CX ));
    		
		displayPtr -> y = (uint32_t)(( DX * ( screenPtr -> x ) + EX * ( screenPtr -> y) + FX )); 
		/* XD = AX+BY+C */        
	//	displayPtr->x = ( (matrix.An * screenPtr->x) + (matrix.Bn * screenPtr->y) + matrix.Cn ) / matrix.Divider ;
		/* YD = DX+EY+F */        
	//	displayPtr->y = ( (matrix.Dn * screenPtr->x) + (matrix.En * screenPtr->y) + matrix.Fn ) / matrix.Divider ;
	}
	else
	{
		
		screenPtr  -> x = 0;
    screenPtr  -> y = 0;
    displayPtr -> x = 0;
    displayPtr -> y = 0;
		
		retTHRESHOLD = DISABLE;
	}
	
	return(retTHRESHOLD);
	
} 


FunctionalState setCalibrationMatrix( Coordinate * displayPtr, Coordinate * screenPtr, Matrix * matrixPtr)
{

  FunctionalState retTHRESHOLD = ENABLE ;

	/* Ký(X0íX2) (Y1íY2)í(X1íX2) (Y0íY2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = DISABLE;
  }
	
  else
  {
    /* Aý((XD0íXD2) (Y1íY2)í(XD1íXD2) (Y0íY2))ïK	*/
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	/* Bý((X0íX2) (XD1íXD2)í(XD0íXD2) (X1íX2))ïK	*/
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* Cý(Y0(X2XD1íX1XD2)+Y1(X0XD2íX2XD0)+Y2(X1XD0íX0XD1))ïK */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* Dý((YD0íYD2) (Y1íY2)í(YD1íYD2) (Y0íY2))ïK	*/
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* Eý((X0íX2) (YD1íYD2)í(YD0íYD2) (X1íX2))ïK	*/
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* Fý(Y0(X2YD1íX1YD2)+Y1(X0YD2íX2YD0)+Y2(X1YD0íX0YD1))ïK */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
  }
	
  return( retTHRESHOLD );
}


Coordinate ScreenSample[3];
Coordinate DisplaySample[3] = {{  45, 45}, { 400, 60},{ 240,250}  };

void TouchPanel_Calibrate(void)
{
	uint8_t i=0;
	Coordinate * Ptr;	
	dbgmsg("TouchPanel Calibrate Start\n");
	for(i=0;i<3;i++)
	{
		GUI_Clear();
		GUI_SetColor(GUI_WHITE);
		GUI_SetFont(GUI_FONT_COMIC18B_ASCII);
		GUI_DispStringHCenterAt("TouchPanel Calibrate",480/2,1);  
		GUI_DispStringHCenterAt("Press the point displayed on the LCD",480/2,20);  
		GUI_AA_SetFactor(4);
		GUI_AA_FillCircle(DisplaySample[i].x, DisplaySample[i].y,5);
  		do
		{
			Ptr = Read_Screen();
		}
		while( Ptr == (void*)0 );
		
		ScreenSample[i].x= Ptr->x; ScreenSample[i].y= Ptr->y;
		
		dbgmsg("CAL Point%d : %d,%d\n",i+1,ScreenSample[i].x,ScreenSample[i].y);
		Delay_ms(500);
	}
	setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix );
	
    GUI_Clear();
	Delay_ms(1000);      

	dbgmsg("matrix.An = (%lf)\n",matrix.An);			  
	dbgmsg("matrix.Bn = (%lf)\n",matrix.Bn);			  
	dbgmsg("matrix.Cn = (%lf)\n",matrix.Cn);			  
	dbgmsg("matrix.Dn = (%lf)\n",matrix.Dn);			  
	dbgmsg("matrix.En = (%lf)\n",matrix.En);			  
	dbgmsg("matrix.Fn = (%lf)\n",matrix.Fn);		  
	dbgmsg("matrix.Divider = (%lf)\n",matrix.Divider);
	
	while(1)
	{
		if( TOUCH_PENIRQ == 0 )
		{   
			getDisplayPoint(&display, Read_Screen());
			if ((display.y < 272) && (display.y > 0) && (display.x > 0) && (display.x < 480))
			{	
				dbgmsg(">> touch sensing : %03d,%03d\n",display.x,display.y);
				Delay_ms(10);	
			}
		}
	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


