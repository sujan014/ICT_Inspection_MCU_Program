#include "task_Measure.h"

//mear sig
uint16_t MeasureICT(void)
{
	return Measure_adc_value(10,128); 
}

int cap_count(void)
{
	int temp = 0;
	int over = 0;

	RECHARGE_EN(1);	
	Delay_ms(1);	// cap 방전시간
	RECHARGE_EN(0);
	TIM_SetCounter(TIM3,0);  
	while(CAP_CHK_IN)
	{
		if(TIM_GetCounter(TIM3) >= 50000)	//1 -> 0.25 usec, 50000 -> 12.5msec
		{
			if(over++ > 1)
			{
				temp  = -1;
				TIM_SetCounter(TIM3,0);
				break;
			}
			TIM_SetCounter(TIM3,0);
		}
	}
	RECHARGE_EN(1);	
	if(temp != -1)  temp = TIM_GetCounter(TIM3)+(over*50000);
	
	return ((temp));
}

float MeasureCap(void)
{
	const int avr_cnt = 1;
	const float time = 0.25;	//us
	int temp = 0;
	int sum_cap= 0;
	
	sum_cap = 0;
	for(int i=0;i<avr_cnt;i++)
	{
		temp = cap_count();
		if(temp == -1)	return -1;
		sum_cap+=temp;
	}
	sum_cap = sum_cap / avr_cnt;	
	return  (float)sum_cap*(float)time;
}
//mear out sig
uint16_t MeasureOUT(void)
{
	return Measure_adc_value(12,128); 
}
//evcc current sig
uint16_t MeasureEVCC(void)
{
	return Measure_adc_value(13,128); 
}

void MeasureCurSel(uint8_t range)
{
	switch(range)
	{
		case OFF:		 I_SEL_EN(1); I_SEL_A3(0); I_SEL_A2(0); I_SEL_A1(0); I_SEL_EN(1);	break;	// add:0 0 0, EN:1	= ALL Low
		case I_ON1:		 I_SEL_EN(1); I_SEL_A3(0); I_SEL_A2(0); I_SEL_A1(0); I_SEL_EN(0);	break;	// add:0 0 0, EN:0	= Y0 High
		case I_ON2:		 I_SEL_EN(1); I_SEL_A3(0); I_SEL_A2(0); I_SEL_A1(1); I_SEL_EN(0);	break;	// add:0 0 1, EN:0	= Y1 High 
		case I_ON3:		 I_SEL_EN(1); I_SEL_A3(0); I_SEL_A2(1); I_SEL_A1(0); I_SEL_EN(0);	break;	// add:0 1 0, EN:0	= Y2 High
		case I_ON4:		 I_SEL_EN(1); I_SEL_A3(0); I_SEL_A2(1); I_SEL_A1(1); I_SEL_EN(0);	break;	// add:0 1 1, EN:0	= Y3 High
		case I_ON5:		 I_SEL_EN(1); I_SEL_A3(1); I_SEL_A2(0); I_SEL_A1(0); I_SEL_EN(0);	break;	// add:1 0 0, EN:0	= Y4 High
		case I_LOW:		 I_SEL_EN(1); I_SEL_A3(1); I_SEL_A2(0); I_SEL_A1(1); I_SEL_EN(0);	break;	// add:1 0 1, EN:0	= Y5 High
		case MEAS_OUT:	 I_SEL_EN(1); I_SEL_A3(1); I_SEL_A2(1); I_SEL_A1(0); I_SEL_EN(0);	break;	// add:1 1 0, EN:0	= Y6 High
		case EVCC_CUR:	 I_SEL_EN(1); I_SEL_A3(1); I_SEL_A2(1); I_SEL_A1(1); I_SEL_EN(0);	break;	// add:1 1 1, EN:0	= Y7 High
        default:    	 I_SEL_EN(1); I_SEL_A3(0); I_SEL_A2(0); I_SEL_A1(0); I_SEL_EN(1);	break;	// add:0 0 0, EN:1	= ALL Low
	}
}

void ModeSelect(uint8_t range)
{
    RECHARGE_EN(0);
    switch(range)
    {
        //  Res	
        case 0:     I_GAIN10_EN(0);	DacOut(0.0); MeasureCurSel(I_ON1);		break; 	// 200R~   		10mA			( I1 + GANE10 )
        case 1:     I_GAIN10_EN(1);	DacOut(0.0); MeasureCurSel(I_ON1);		break; 	// 2.5K~   		1.0mA			( I1 + GANE1  )
        case 2:     I_GAIN10_EN(1);	DacOut(0.0); MeasureCurSel(I_ON2);		break;	// 25K~    		0.1mA			( I2 + GANE1  )
        case 3:     I_GAIN10_EN(1);	DacOut(0.0); MeasureCurSel(I_ON3);		break; 	// 250K~   		0.01mA			( I3 + GANE1  )
        case 4:     I_GAIN10_EN(1);	DacOut(0.0); MeasureCurSel(I_ON4);		break;	// 2.5M~   		0.001mA			( I4 + GANE1  )
        case 5:     I_GAIN10_EN(1);	DacOut(0.0); MeasureCurSel(I_ON5);		break;	// 10.0M~  		0.00014mA		( I5 + GANE1  )	
        case 6:     I_GAIN10_EN(1);	DacOut(0.0); MeasureCurSel(I_LOW);		break;	//          
        //  Cap                                  				
        //case 10:								 									break;	// 100p			TESTING
        case 11:    I_GAIN10_EN(1);	DacOut(1.0); MeasureCurSel(I_ON2);		break;	// 1nF~			0.1mA	1.0V	( I2 + GANE1 )
        case 12:    I_GAIN10_EN(1);	DacOut(1.0); MeasureCurSel(I_ON2);		break;	// 10nF~		0.1mA	1.0V	( I2 + GANE1 )
        case 13:    I_GAIN10_EN(1);	DacOut(0.5); MeasureCurSel(I_ON2);		break;	// 100nF~		0.1mA	0.5V	( I2 + GANE1 )
        case 14:    I_GAIN10_EN(0);	DacOut(0.5); MeasureCurSel(I_ON2);		break;	// 1000nF~		0.1mA	0.5V	( I2 + GANE10)
        case 15:    I_GAIN10_EN(0);	DacOut(1.0); MeasureCurSel(I_ON1);		break;	// 4700nF~		1.0mA	1.0V	( I1 + GANE10)
        case 16:    I_GAIN10_EN(0);	DacOut(1.0); MeasureCurSel(I_ON1);		break;	// 10000nF~		1.0mA	1.0V	( I1 + GANE10)	
        case 17:    I_GAIN10_EN(0);	DacOut(0.7); MeasureCurSel(I_ON1);		break;	// 100000nF~	1.0mA	0.7V	( I1 + GANE10)	
        //  Diode		
        case 20:	I_GAIN10_EN(1);	DacOut(0.0); MeasureCurSel(I_ON1);		break; 	// Didoe 1mA    2.5V  ( I1 + GANE1  )
		// EVCC		
        case 30:	I_GAIN10_EN(1);	DacOut(0.0); MeasureCurSel(OFF);		break;	//  EVCC1 CUR
        case 31:	I_GAIN10_EN(1);	DacOut(0.0); MeasureCurSel(EVCC_CUR);	break;	//  EVCC2 CUR
        //   MEASURE OUT ON		
		case 50:	I_GAIN10_EN(1); DacOut(0.0); MeasureCurSel(MEAS_OUT);	break;	//
        case 255:	I_GAIN10_EN(1); DacOut(0.0); MeasureCurSel(OFF);		break;   
        default:	I_GAIN10_EN(1); DacOut(0.0); MeasureCurSel(OFF);		break;   
    }
	Delay_ms(4); 	//Delay_ms(5); 
}

/////
#define C11_ONTIME	(float)16		
#define C12_ONTIME	(float)100		
#define C13_ONTIME	(float)440	
#define C14_ONTIME	(float)440	
#define C15_ONTIME	(float)320	//295	
#define C16_ONTIME	(float)630	//580	
#define C17_ONTIME	(float)1030	

#define C11_OFFTIME	(float)10
#define C12_OFFTIME	(float)10
#define C13_OFFTIME	(float)10
#define C14_OFFTIME	(float)10
#define C15_OFFTIME	(float)10
#define C16_OFFTIME	(float)10
#define C17_OFFTIME	(float)10
	
#define C11_REF	(float)1
#define C12_REF	(float)10
#define C13_REF	(float)100
#define C14_REF	(float)1000
#define C15_REF	(float)4700
#define C16_REF	(float)10000
#define C17_REF	(float)100000

#define C11_Factor	(float)(C11_REF/C11_ONTIME)
#define C12_Factor	(float)(C12_REF/C12_ONTIME)
#define C13_Factor	(float)(C13_REF/C13_ONTIME)
#define C14_Factor	(float)(C14_REF/C14_ONTIME)
#define C15_Factor	(float)(C15_REF/C15_ONTIME)
#define C16_Factor	(float)(C16_REF/C16_ONTIME)
#define C17_Factor	(float)(C17_REF/C17_ONTIME)

#define adc_res  	(2.5/4096)		//	
#define range0_cur 	(0.01)			// 	200R
#define range1_cur 	(0.001)			//	2.5kR	
#define range2_cur 	(0.0001)		//	25kR
#define range3_cur 	(0.00001)		//	250kR
#define range4_cur 	(0.000001)		//	2.5MR
#define range5_cur 	(0.00000014)	//	15MR
#define range6_cur 	(0.0)	//low res
		
float Measure_Result = 0;
char RESULT_SYB[50]={NULL};
float Measure(uint8_t range)
{    
	float Measure_Volt = 0;
    switch(range)
    {
		/////////////////////////////      
		//  -----   RES -------------------
		/////////////////////////////      
        case 0:		// range0	~200R
			Measure_Volt 	= (float)MeasureICT() * (float)(adc_res);
            Measure_Result 	= (float)Measure_Volt/(float)range0_cur;
			Measure_Result 	= Measure_Result - mux_res;
			if(Measure_Result >= 200) Measure_Result = 200;
            if(Measure_Result <= 0)   Measure_Result = 0;
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
            sprintf(RESULT_SYB,"%.2fR",Measure_Result);
		
        break;		
        case 1:		// range1	~2.5kR
			Measure_Volt 	= (float)MeasureICT() * (float)(adc_res);
            Measure_Result 	= (float)Measure_Volt/(float)range1_cur;
			Measure_Result 	= Measure_Result - mux_res;
            if(Measure_Result >= 2450) Measure_Result = 2500;
            if(Measure_Result <= 100)    Measure_Result = 0;			
			Measure_Result = (Measure_Result/(float)1000.0);		
			//Measure_Result = floor(Measure_Result*1000)/(float)1000;		
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
            sprintf(RESULT_SYB,"%.2fkR",Measure_Result);
        break;		
        case 2:		// range2	~25kR
			Measure_Volt 	= (float)MeasureICT() * (float)(adc_res);
            Measure_Result 	= (float)Measure_Volt/(float)range2_cur;
            if(Measure_Result >= 24900) Measure_Result = 25000;
            if(Measure_Result <= 1000)    Measure_Result = 0;	
			Measure_Result = (Measure_Result/(float)1000.0);		
			//Measure_Result = floor(Measure_Result*1000)/(float)1000;		
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 10) ) )/10.0;
            sprintf(RESULT_SYB,"%.1fkR",Measure_Result);
        break;		
        case 3:		// range3	~250kR
			Measure_Volt 	= (float)MeasureICT() * (float)(adc_res);
            Measure_Result 	= (float)Measure_Volt/(float)range3_cur;
            if(Measure_Result >= 249000) Measure_Result = 250000;
            if(Measure_Result <= 4000)    Measure_Result = 0;	
			Measure_Result = (Measure_Result/(float)1000.0);		
			//Measure_Result = floor(Measure_Result*1000)/(float)1000;		
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 10) ) )/10.0;
            sprintf(RESULT_SYB,"%.1fkR",Measure_Result);
        break;
        case 4:		// range4	~2.5MR
			Measure_Volt 	= (float)MeasureICT() * (float)(adc_res);
            Measure_Result 	= (float)Measure_Volt/(float)range4_cur;
            if(Measure_Result >= 2490000) Measure_Result = 2500000;
            if(Measure_Result <= 100000)    	Measure_Result = 0;	
			Measure_Result = (Measure_Result/(float)1000000.0);		
			//Measure_Result = floor(Measure_Result*1000)/(float)1000;		
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 10) ) )/10.0;
            sprintf(RESULT_SYB,"%.1fMR",Measure_Result);
        break;			
        case 5:		// range5	~15MR
			Measure_Volt 	= (float)MeasureICT() * (float)(adc_res);
            Measure_Result 	= (float)Measure_Volt/(float)range5_cur;
            if(Measure_Result >= 14900000) Measure_Result = 15000000;
            if(Measure_Result <= 1000000)    Measure_Result = 0;
			Measure_Result = (Measure_Result/(float)1000000.0);		
			//Measure_Result = floor(Measure_Result*1000)/(float)1000;		
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 10) ) )/10.0;
            sprintf(RESULT_SYB,"%.1fMR",Measure_Result);
        break;		
        case 6:		// range6	row res
			//16bit adc code add
        break;
		/////////////////////////////      
        //  -----   Diode -------------------   
		/////////////////////////////               
        case 20: 
			Measure_Volt 	= (float)MeasureICT() * (float)(adc_res);
            Measure_Result = (float)(Measure_Volt - (float)0.05);    	 // MAX 2.45 V
            if(Measure_Result >= (float)2.45) Measure_Result = (float)2.45;
            if(Measure_Result <= 0)    Measure_Result = 0;
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
            sprintf(RESULT_SYB,"%.2fV",Measure_Result);  
		break;            
		/////////////////////////////        
        //  -----   CAP -------------------  
		/////////////////////////////              
        case 10:		// range10	100pF
            Measure_Result = MeasureCap();
			Measure_Result = (float)( floor(Measure_Result) );
			//Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
            if(Measure_Result >= 0)
            {
                sprintf(RESULT_SYB,"%.0f",Measure_Result);    
            }
            else
            {
                sprintf(RESULT_SYB,"Short"); 
            }
        break;

        case 11:		// range11	1nF~10nF
            Measure_Result = MeasureCap();
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
		    if(Measure_Result >= 0)
            {    
				Measure_Result = Measure_Result*(C11_Factor);
				if(Measure_Result <= (float)0.5)	Measure_Result = 0;
				
				Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
				Measure_Result = (float)( floor(Measure_Result) );
                sprintf(RESULT_SYB,"%.0fnF",Measure_Result);
            }
            else
            {
                sprintf(RESULT_SYB,"Short"); 
            }
        break; 
        case 12:		// range12	10nF~100nF
            Measure_Result = MeasureCap();   
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
            if(Measure_Result >= 0)
            {
				Measure_Result = Measure_Result*(C12_Factor);
				if(Measure_Result <= 5)	Measure_Result = 0;

				Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
				Measure_Result = (float)( floor(Measure_Result) );				
                sprintf(RESULT_SYB,"%.0fnF",Measure_Result);
            }
            else
            {
                sprintf(RESULT_SYB,"Short"); 
            }
        break;
        case 13:		// range13	100nF~1000nF
            Measure_Result = MeasureCap();   
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
            if(Measure_Result >= 0)
            {
				Measure_Result = Measure_Result*(C13_Factor);
				if(Measure_Result <= 10)	Measure_Result = 0;
					
				Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
				Measure_Result = (float)( floor(Measure_Result) );
                sprintf(RESULT_SYB,"%.0fnF",Measure_Result);
            }  
            else
            {
                sprintf(RESULT_SYB,"Short"); 
            }
        break; 
        case 14:		// range14	1000nF~4700nF
            Measure_Result = MeasureCap();
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
            if(Measure_Result >= 0)
            {
				Measure_Result = Measure_Result*(C14_Factor);
				if(Measure_Result <= 100)	Measure_Result = 0;

				Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
				Measure_Result = (float)( floor(Measure_Result) );
                sprintf(RESULT_SYB,"%.0fnF",Measure_Result);
            }
            else
            {
                sprintf(RESULT_SYB,"Short"); 
            }
        break;			
        case 15:		// range15	4700nF~10000nF
            Measure_Result = MeasureCap();   
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
            if(Measure_Result >= 0)
            {
				Measure_Result = Measure_Result*(C15_Factor);
				if(Measure_Result <= 500)	Measure_Result = 0;

				Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
				Measure_Result = (float)( floor(Measure_Result) );
                sprintf(RESULT_SYB,"%.0fnF",Measure_Result);
            }
            else
            {
                sprintf(RESULT_SYB,"Short"); 
            }
        break;			
        case 16:		// range16	10uF~100uF
            Measure_Result = MeasureCap();   
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
            if(Measure_Result >= 0)
            {  
				Measure_Result = Measure_Result*(C16_Factor)/(float)1000;
				if(Measure_Result <= 1)	Measure_Result = 0;

				Measure_Result = ( (float)( (uint32_t)(Measure_Result * 10) ) )/10.0;
				Measure_Result = (float)( floor(Measure_Result) );
                sprintf(RESULT_SYB,"%.1fuF",Measure_Result);
            }
            else
            {
                sprintf(RESULT_SYB,"Short"); 
            } 
        break;			
        case 17:		// range17	100uF~
            Measure_Result = MeasureCap();   
			Measure_Result = ( (float)( (uint32_t)(Measure_Result * 100) ) )/100.0;
            if(Measure_Result >= 0)
            { 
				Measure_Result = Measure_Result*(C17_Factor)/(float)1000;
				if(Measure_Result <= 10)	Measure_Result = 0;

				Measure_Result = ( (float)( (uint32_t)(Measure_Result * 10) ) )/10.0;
				Measure_Result = (float)( floor(Measure_Result) );
                sprintf(RESULT_SYB,"%.1fuF",Measure_Result);
            }
            else
            {
                sprintf(RESULT_SYB,"Short"); 
            }
        break;
		/////////////////////////////        
        //  -----   VCC1/VCC2 -------------------  
		/////////////////////////////  	
        case 30:		// range30	VCC1~
			Measure_Volt 	= (float)MeasureEVCC() * (float)(adc_res);
		
        break;   
        case 31:		// range31	VCC2~
			Measure_Volt 	= (float)MeasureEVCC() * (float)(adc_res);
		
        break;   
		/////////////////////////////        
        //  -----   MEAS OUT
		/////////////////////////////  	
        case 50:		// range50	MEAS OUT~			
			Measure_Result = MeasureOUT();
		
        break;   
			
	}	
    return Measure_Result;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
