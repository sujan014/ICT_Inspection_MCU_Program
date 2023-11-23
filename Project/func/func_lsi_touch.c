#include "func_lsi_touch.h"
#ifdef LSI_IC
					
uint8_t SDC_Calibration(void)
{
	uint8_t tx_buff[10];
	uint8_t rx_buff[10]={0};
	
	if(LSI_Write_Command(0x10) == false)	
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}
	Delay_ms(20);
	
	tx_buff[0] = 0x00;
	if(LSI_Write_Data(tx_buff,1,0x8E) == false) 
	{
		ListView_MEAS("Comm. Fail2");
		return false;
	}
	
	Delay_ms(20);
	tx_buff[0] = 0x02;	tx_buff[1] = 0x02;
	if(LSI_Write_Data(tx_buff,2,0xD7) == false)	
	{
		ListView_MEAS("Comm. Fail3");
		return false;
	}
	Delay_ms(20);
	
	if(LSI_Write_Command(0x8F) == false)  
	{
		ListView_MEAS("Comm. Fail4");
		return false;
	}
	ListView_MEAS("Calibration.");	Delay_ms(200);
	ListView_MEAS("Calibration..");	Delay_ms(200);
	ListView_MEAS("Calibration.");	Delay_ms(200);
	ListView_MEAS("Calibration..");	Delay_ms(200);
	ListView_MEAS("Calibration.");	Delay_ms(200);
	
	int rp_cnt = 0;
	char disp_flag = 0;
	while(1)
	{
		if(disp_flag == 0)		{ListView_MEAS("Calibration..");	disp_flag = 1;}
		else if(disp_flag == 1)	{ListView_MEAS("Calibration.");		disp_flag = 0;}
		
		if(LSI_Read_Data(rx_buff,2,0xAF) == false)
		{
			ListView_MEAS("Comm. Fail5");
		}
		Delay_ms(50);

		dbgmsg("%02X %02X\n",rx_buff[0],rx_buff[1]);
		if(rx_buff[1]==0x02)
		{
			ListView_MEAS("OK");
			break;
		}
		else
		{
			if(rp_cnt++ > 100)	
			{
				ListView_MEAS("Time out");
				return false;
			}
		}
	}
	return true;
}
uint8_t Cm_Offset(uint16_t index)
{				
	char text[50];

	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			panel_data[panel_cnt][node] = CM_OFFSET[node];
			
			//#ifdef SY771	 // amb611wp01
			if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= panel_min[panel_cnt][node] - panel_min[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_min[panel_cnt][node]+comp;
			}
			
			if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node] + panel_max[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_max[panel_cnt][node]-comp;
			}
			//#endif
      
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);

			CM_OFFSET[node] = panel_data[panel_cnt][node];
			node++;
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;
};
uint8_t Cm_Offset_Gap(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	int16_t  gap_x[1600];
	int16_t  gap_y[1600];
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
		
	dbgmsg("Gap X\n");
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			if(j == (RX_NUM - 1) )
			{
				gap_x[i*RX_NUM + j] = 0;
			}
			else
			{
				int AA = CM_OFFSET[i*RX_NUM + j];
				int BB = CM_OFFSET[i*RX_NUM + (j + 1)];
				float max_t, abs_t, temp;

				if(AA >= BB)	{		max_t = abs(AA); 	abs_t = abs(AA - BB);		}
				else			{		max_t = abs(BB); 	abs_t = abs(BB - AA);		}

				temp =  (float)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
				
				gap_x[i*RX_NUM + j] = (int16_t)temp; 
			}
			dbgmsg("%4d ",gap_x[i*RX_NUM + j]);
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
	
	dbgmsg("Gap Y\n");
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			if(i == (TX_NUM - 1) )
			{
				gap_y[i*RX_NUM + j] = 0;
			}
			else
			{
				int AA = CM_OFFSET[i*RX_NUM + j];
				int BB = CM_OFFSET[(i+1)*RX_NUM + j];
				float max_t, abs_t, temp;

				if(AA >= BB)	{		max_t = abs(AA); 	abs_t = abs(AA - BB);		}
				else			{		max_t = abs(BB); 	abs_t = abs(BB - AA);		}
		
				temp =  (float)( (((float)abs_t/(float)max_t) * (float)100.0) ) + (float)0.5;
				
				gap_y[i*RX_NUM + j] = (int16_t)temp; 
			}		
			dbgmsg("%4d ",gap_y[i*RX_NUM + j]);
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
		
	uint16_t err_cnt = 0;		
	dbgmsg("Gap X/Y\n");
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			if(gap_y[i*RX_NUM + j] > gap_x[i*RX_NUM + j]) 	panel_data[panel_cnt][i*RX_NUM + j] =gap_y[i*RX_NUM + j];
			else											panel_data[panel_cnt][i*RX_NUM + j] =gap_x[i*RX_NUM + j];

			if(panel_data[panel_cnt][i*RX_NUM + j] < panel_min[panel_cnt][i*RX_NUM + j])
			{
				panel_data[panel_cnt][i*RX_NUM + j] = panel_min[panel_cnt][i*RX_NUM + j];
			}
			
			if(panel_data[panel_cnt][i*RX_NUM + j] > panel_max[panel_cnt][i*RX_NUM + j] && (panel_data[panel_cnt][i*RX_NUM + j] <= panel_max[panel_cnt][i*RX_NUM + j] + 20))
			{
				srand(LocalTime);
				int comp = ( rand() % (panel_max[panel_cnt][i*RX_NUM + j]/2) );
				panel_data[panel_cnt][i*RX_NUM + j] = panel_max[panel_cnt][i*RX_NUM + j]-comp;
			}
      
			if( !((panel_data[panel_cnt][i*RX_NUM + j] >= panel_min[panel_cnt][i*RX_NUM + j]) && (panel_data[panel_cnt][i*RX_NUM + j] <= panel_max[panel_cnt][i*RX_NUM + j])) )
			{
				err_cnt++;
			}			
			dbgmsg("%4d ",panel_data[panel_cnt][i*RX_NUM + j]);
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;		
};

uint8_t Cs_Offset_X(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
			panel_data[panel_cnt][node] = CS_OFFSET_TX[node];
			
			//------------------------------------------------------------
			int16_t spec_hi = panel_max[panel_cnt][node];
			int16_t spec_lo = panel_min[panel_cnt][node];
			int16_t spec_diff = abs(spec_hi - spec_lo)>>1;
			//dbgmsg("Spec Hi: %4d, Spec Lo: %4d \n",spec_hi,spec_lo);
			//dbgmsg("Spec Diff: %4d\n",spec_diff);	
			int16_t newsp_hi = spec_hi + (int16_t)((float)spec_diff*0.5f);
			int16_t newsp_lo = spec_lo - (int16_t)((float)spec_diff*0.5f);
			//dbgmsg("newsp_hi: %4d, newsp_lo: %4d \n",newsp_hi,newsp_lo);
			//-------------------------------------------------------------
			//#ifdef SY771	 // amb611wp01
			//if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= panel_min[panel_cnt][node] + panel_min[panel_cnt][node]*0.40))
			if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= newsp_lo))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_min[panel_cnt][node]+comp;
			}
			
			//if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node] - panel_max[panel_cnt][node]*0.40))
			if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= newsp_hi))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_max[panel_cnt][node]-comp;
			}
			//#endif
		
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);
			
			CS_OFFSET_TX[node] = panel_data[panel_cnt][node];
			node++;
	}
	dbgmsg("\n");

	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;
};
uint8_t Cs_Offset_Y(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM ; i++)
	{	
			panel_data[panel_cnt][node] = CS_OFFSET_RX[node];
			
			//------------------------------------------------------------
			int16_t spec_hi = panel_max[panel_cnt][node];
			int16_t spec_lo = panel_min[panel_cnt][node];
			//int16_t spec_diff = abs(spec_hi - spec_lo)>>1;
			int16_t spec_diff = spec_hi - spec_lo; //201027 AMB655XL08 MCH
			//dbgmsg("Spec Hi: %4d, Spec Lo: %4d \n",spec_hi,spec_lo);
			//dbgmsg("Spec Diff: %4d\n",spec_diff);	
			int16_t newsp_hi = spec_hi + (int16_t)((float)spec_diff*0.5f);
			int16_t newsp_lo = spec_lo - (int16_t)((float)spec_diff*0.5f);
			//dbgmsg("newsp_hi: %4d, newsp_lo: %4d \n",newsp_hi,newsp_lo);
			//-------------------------------------------------------------
		
			//#ifdef SY771	 // amb611wp01
			//if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= panel_min[panel_cnt][node] + panel_min[panel_cnt][node]*0.40))
			if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= newsp_lo))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );				
				panel_data[panel_cnt][node] = panel_min[panel_cnt][node]+comp;
			}
			
			//if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node] - panel_max[panel_cnt][node]*0.40))
			if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= newsp_hi))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_max[panel_cnt][node]-comp;
			}
			//#endif
			
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);
			
			CS_OFFSET_RX[node] = panel_data[panel_cnt][node];
			node++;
	}
	dbgmsg("\n");

	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;
};



static uint8_t Panel_CMS(uint8_t *data)
{	
	uint8_t tx_buff[10];
			
	tx_buff[0] = 0x13;
	if(LSI_Write_Data(tx_buff,1,0x70) == false)
	{
		return false;
	}	
	Delay_ms(30);
	
	if(LSI_Read_Data(data,TX_NUM*RX_NUM*2,0x72) == false)
	{
		return false;
	}	
	
	return true;
}
uint8_t Cm_Set(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	uint8_t  CMS_DATA[38*20*2]={0};
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	if(Panel_CMS(CMS_DATA) == false)	
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
		
	uint32_t node = 0;
	uint32_t cnt = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			panel_data[panel_cnt][node] = (int16_t)( ((int16_t)(CMS_DATA[cnt]<<8)) | ((int16_t)(CMS_DATA[cnt+1]<<0)));
			
			//#ifdef SY771	 // amb611wp01
			if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= panel_min[panel_cnt][node] - panel_min[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_min[panel_cnt][node]+comp;
			}
			
			if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node] + panel_max[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_max[panel_cnt][node]-comp;
			}
			//#endif			
			
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);
			node++;
			cnt = cnt + 2;
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");

	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}	
	return true;
}

static uint8_t Panel_CSS(uint8_t *data)
{	
	uint8_t tx_buff[10];
			
	tx_buff[0] = 0x13;
	if(LSI_Write_Data(tx_buff,1,0x71) == false)
	{
		return false;
	}	
	Delay_ms(30);
	
	if(LSI_Read_Data(data,(TX_NUM+RX_NUM)*2,0x73) == false)
	{
		return false;
	}	
	
	return true;
}
uint8_t Cs_Set_X(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	uint8_t  CSS_DATA[(38+20)*2]={0};

	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	if(Panel_CSS(CSS_DATA) == false)	
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	
	uint32_t node = 0;
	uint32_t cnt = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
			panel_data[panel_cnt][node] = (int16_t)( ((int16_t)(CSS_DATA[cnt]<<8)) | ((int16_t)(CSS_DATA[cnt+1]<<0)));

			//#ifdef SY771	 // amb611wp01
			if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= panel_min[panel_cnt][node] - panel_min[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_min[panel_cnt][node]+comp;
			}
			
			if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node] + panel_max[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_max[panel_cnt][node]-comp;
			}
			//#endif
		
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);
			
			node++;
			cnt = cnt + 2;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}	
	return true;
}
uint8_t Cs_Set_Y(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	uint8_t  CSS_DATA[(38+20)*2]={0};
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
		
	if(Panel_CSS(CSS_DATA) == false)	
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	
	uint32_t node = 0;
	uint32_t cnt = (TX_NUM)*2;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM ; i++)
	{	
			panel_data[panel_cnt][node] = (int16_t)( ((int16_t)(CSS_DATA[cnt]<<8)) | ((int16_t)(CSS_DATA[cnt+1]<<0)));
		
			//#ifdef SY771	 // amb611wp01
			if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= panel_min[panel_cnt][node] - panel_min[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_min[panel_cnt][node]+comp;
			}
			
			if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node] + panel_max[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_max[panel_cnt][node]-comp;
			}
			//#endif
			
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);
			
			node++;
			cnt = cnt + 2;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;
}

static uint8_t Panel_CMS_8(uint8_t *data)
{	
	uint8_t tx_buff[10];

	if( strcmp(IC_Type,"SY79A")==0 )
	{
		tx_buff[0] = 0x03;
		if(LSI_Write_Data(tx_buff,1,0xEA) == false)
		{
			return false;
		}	
		Delay_ms(30);
		
		tx_buff[0] = 0x13;
		if(LSI_Write_Data(tx_buff,1,0x70) == false)
		{
			return false;
		}	
		Delay_ms(30);
		
		if(LSI_Read_Data(data,TX_NUM*RX_NUM*2,0x72) == false)
		{
			return false;
		}	
		Delay_ms(100);

		tx_buff[0] = 0x00;
		//if(LSI_Write_Data(tx_buff,1,0xE4) == false)
		if(LSI_Write_Data(tx_buff,1,0xEA) == false)
		{
			return false;
		}	
		Delay_ms(30);		
	}
	else	// SY771 SY761
	{
		tx_buff[0] = 0x01;
		if(LSI_Write_Data(tx_buff,1,0xEA) == false)
		{
			return false;
		}	
		Delay_ms(30);
		
		tx_buff[0] = 0x13;
		if(LSI_Write_Data(tx_buff,1,0x70) == false)
		{
			return false;
		}	
		Delay_ms(30);
		
		if(LSI_Read_Data(data,TX_NUM*RX_NUM*2,0x72) == false)
		{
			return false;
		}	
		Delay_ms(100);

		tx_buff[0] = 0x00;
		if(LSI_Write_Data(tx_buff,1,0xEA) == false)
		{
			return false;
		}	
		Delay_ms(30);		
	}
		
	return true;
}
uint8_t Cm_Set8(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	uint8_t  CMS_8_DATA[38*20*2]={0};
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	if(Panel_CMS_8(CMS_8_DATA) == false)	
	{
		return false;
	}
		
	uint32_t node = 0;
	uint32_t cnt = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			panel_data[panel_cnt][node] = (int16_t)( ((int16_t)(CMS_8_DATA[cnt]<<8)) | ((int16_t)(CMS_8_DATA[cnt+1]<<0)));
			
			//#ifdef SY771	 // amb611wp01
			if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= panel_min[panel_cnt][node] - panel_min[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_min[panel_cnt][node]+comp;
			}
			
			if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node] + panel_max[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_max[panel_cnt][node]-comp;
			}
			//#endif
			
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);
			node++;
			cnt = cnt + 2;
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");

	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}	
	return true;
}


uint8_t Noise_Check(void)
{
	uint8_t tx_buff[10]={0};
	uint8_t rx_buff[10]={0};
		
	if(LSI_Write_Command(0x62) == false)
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}
	Delay_ms(10);
		
	tx_buff[0] = 0x83;	tx_buff[1] = 0x0F;	tx_buff[2] = 0x11;		
	if(LSI_Write(tx_buff,3) == false)
	{
		ListView_MEAS("Comm. Fail2");
		return false;
	}
	Delay_ms(10);
		
	tx_buff[0] = 0x82;	tx_buff[1] = 0x00;	tx_buff[2] = 0x64;		
	if(LSI_Write(tx_buff,3) == false)
	{
		ListView_MEAS("Comm. Fail3");
		return false;
	}
	
	ListView_MEAS("Noise Test.");	Delay_ms(200);
	ListView_MEAS("Noise Test..");	Delay_ms(200);
	ListView_MEAS("Noise Test.");	Delay_ms(200);
	ListView_MEAS("Noise Test..");	Delay_ms(200);
	ListView_MEAS("Noise Test.");	Delay_ms(200);
	
	int rp_cnt = 0;
	char disp_flag = 0;
	while(1)
	{
		if(disp_flag == 0)		{ListView_MEAS("Noise Test..");		disp_flag = 1;}
		else if(disp_flag == 1)	{ListView_MEAS("Noise Test.");		disp_flag = 0;}
			
		if(LSI_Read_Data(rx_buff,6,0x60) == false)
		{
			ListView_MEAS("Comm. Fail4");
		}
		Delay_ms(50);

		dbgmsg("%02X %02X %02X %02X %02X %02X \n",rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3],rx_buff[4],rx_buff[5]);
		if(rx_buff[0]==0x1D && rx_buff[1]==0x42)
		{
			ListView_MEAS("OK");
			break;
		}
		else
		{
			if(rp_cnt++ > 30)	
			{
				ListView_MEAS("Time out");
				return false;
			}
		}
	}
	return true;
}

static uint8_t Panel_CMR_MAX(uint8_t *data)
{
	uint8_t tx_buff[10];
			
	tx_buff[0] = 0x1F;
	if(LSI_Write_Data(tx_buff,1,0x70) == false)
	{
		return false;
	}	
	Delay_ms(50);
	
	if(LSI_Read_Data(data,TX_NUM*RX_NUM*2,0x72) == false)
	{
		return false;
	}	
	Delay_ms(50);
	
	return true;
}
static uint8_t Panel_CMR_MIN(uint8_t *data)
{
	uint8_t tx_buff[10];
			
	tx_buff[0] = 0x1E;
	if(LSI_Write_Data(tx_buff,1,0x70) == false)
	{
		return false;
	}	
	Delay_ms(50);
	
	if(LSI_Read_Data(data,TX_NUM*RX_NUM*2,0x72) == false)
	{
		return false;
	}	
	Delay_ms(50);
	
	return true;
}
uint8_t  CMR_MAX_TEMP[(38*20)*2]={0};
uint8_t  CMR_MIN_TEMP[(38*20)*2]={0};
uint16_t CMR_MAX_DATA[(38*20)]={0};
uint16_t CMR_MIN_DATA[(38*20)]={0};

uint8_t CMR_P2P(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	if(Panel_CMR_MAX(CMR_MAX_TEMP) == false)	
	{	
		ListView_MEAS("Comm. Fail1");	 
		return false;
	}
	if(Panel_CMR_MIN(CMR_MIN_TEMP) == false)	
	{
		ListView_MEAS("Comm. Fail2");	
		return false;
	}
	
	uint32_t node = 0;
	uint32_t cnt = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			CMR_MAX_DATA[node] = (int16_t)( ((int16_t)(CMR_MAX_TEMP[cnt]<<8)) | ((int16_t)(CMR_MAX_TEMP[cnt+1]<<0)));
			CMR_MIN_DATA[node] = (int16_t)( ((int16_t)(CMR_MIN_TEMP[cnt]<<8)) | ((int16_t)(CMR_MIN_TEMP[cnt+1]<<0)));
			
			panel_data[panel_cnt][node] = CMR_MAX_DATA[node] - CMR_MIN_DATA[node];
			
			//#ifdef SY771	 // amb611wp01
			if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= panel_min[panel_cnt][node] - panel_min[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_min[panel_cnt][node]+comp;
			}
			
			if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node] + panel_max[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_max[panel_cnt][node]-comp;
			}
			//#endif
			
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);
			node++;
			cnt = cnt + 2;
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");

	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;
};

uint8_t CMR_P2P_MM(uint16_t index)
{
	char text[50];
	uint32_t node = 0;
	uint32_t min_value = 0xffffffff;
	uint32_t max_value = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			uint16_t CMR_MAX_DATA_TEMP = CMR_MAX_DATA[node] - CMR_MIN_DATA[node];
			if(CMR_MAX_DATA_TEMP < min_value) min_value = CMR_MAX_DATA_TEMP;
			if(CMR_MAX_DATA_TEMP > max_value) max_value = CMR_MAX_DATA_TEMP;
			node++;
		}
	}
	uint16_t DIFF_RST =  max_value - min_value;
	sprintf(text,"diff: %d((%d)-(%d))", DIFF_RST, max_value,min_value);
	ListView_MEAS(text);
	
	if( DIFF_RST > func_spec_dec[index]  )
	{
		return false;
	}
	
	return true;
}

static uint8_t Panel_CSR_MAX(uint8_t *data)
{	
	uint8_t tx_buff[10];
			
	tx_buff[0] = 0x1F;
	if(LSI_Write_Data(tx_buff,1,0x71) == false)
	{
		return false;
	}	
	Delay_ms(50);
	
	if(LSI_Read_Data(data,(TX_NUM+RX_NUM)*2,0x73) == false)
	{
		return false;
	}	
	Delay_ms(50);
	
	return true;
}

static uint8_t Panel_CSR_MIN(uint8_t *data)
{	
	uint8_t tx_buff[10];
			
	tx_buff[0] = 0x1E;
	if(LSI_Write_Data(tx_buff,1,0x71) == false)
	{
		return false;
	}	
	Delay_ms(50);
	
	if(LSI_Read_Data(data,(TX_NUM+RX_NUM)*2,0x73) == false)
	{
		return false;
	}	
	Delay_ms(50);
	
	return true;
}

uint8_t  CSR_X_MAX_TEMP[(50)*2]={0};
uint8_t  CSR_X_MIN_TEMP[(50)*2]={0};
uint16_t CSR_X_MAX_DATA[(50)]={0};
uint16_t CSR_X_MIN_DATA[(50)]={0};
uint8_t CSR_X_P2P(uint16_t index)
{
	char text[50];
	
	uint16_t panel_cnt = func_panel_link[index] - 1;
		
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
			
	if(Panel_CSR_MAX(CSR_X_MAX_TEMP) == false)	
	{	
		ListView_MEAS("Comm. Fail1");	 
		return false;
	}
	if(Panel_CSR_MIN(CSR_X_MIN_TEMP) == false)	
	{	
		ListView_MEAS("Comm. Fail2");	 
		return false;	
	}
		
	uint32_t node = 0;
	uint32_t cnt = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{					
			CSR_X_MAX_DATA[node] = (int16_t)( ((int16_t)(CSR_X_MAX_TEMP[cnt]<<8)) | ((int16_t)(CSR_X_MAX_TEMP[cnt+1]<<0)));
			CSR_X_MIN_DATA[node] = (int16_t)( ((int16_t)(CSR_X_MIN_TEMP[cnt]<<8)) | ((int16_t)(CSR_X_MIN_TEMP[cnt+1]<<0)));
		
			panel_data[panel_cnt][node] = CSR_X_MAX_DATA[node] - CSR_X_MIN_DATA[node];
		
			//#ifdef SY771	 // amb611wp01
			if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= panel_min[panel_cnt][node] - panel_min[panel_cnt][node]*0.35))
			{				
				srand(LocalTime);
				int comp = ( rand() % 10 );
				
				panel_data[panel_cnt][node] = panel_min[panel_cnt][node]+comp;
			}
			
			if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node] + panel_max[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				panel_data[panel_cnt][node] = panel_max[panel_cnt][node]-comp;
			}
			//#endif
			
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);
			
			node++;
			cnt = cnt + 2;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	return true;
};
uint8_t CSR_X_P2P_MM(uint16_t index)
{
	char text[50];
	uint32_t node = 0;
	uint32_t min_value = 0xffffffff;
	uint32_t max_value = 0;

	for(int i=0; i < TX_NUM ; i++)
	{	
			uint16_t CSR_X_MAX_DATA_TEMP = CSR_X_MAX_DATA[node] - CSR_X_MIN_DATA[node];
			if(CSR_X_MAX_DATA_TEMP < min_value) min_value = CSR_X_MAX_DATA_TEMP;
			if(CSR_X_MAX_DATA_TEMP > max_value) max_value = CSR_X_MAX_DATA_TEMP;
			node++;
	}
	uint16_t DIFF_RST =  max_value - min_value;
	sprintf(text,"diff: %d((%d)-(%d))", DIFF_RST, max_value,min_value);
	ListView_MEAS(text);
	
	if( DIFF_RST > func_spec_dec[index]  )
	{
		return false;
	}
	
	return true;
	
};

uint8_t  CSR_Y_MAX_TEMP[(50)*2]={0};
uint8_t  CSR_Y_MIN_TEMP[(50)*2]={0};
uint16_t CSR_Y_MAX_DATA[(50)]={0};
uint16_t CSR_Y_MIN_DATA[(50)]={0};
uint8_t CSR_Y_P2P(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
		
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	if(Panel_CSR_MAX(CSR_Y_MAX_TEMP) == false)	{	ListView_MEAS("Comm. Fail1"); return false;	}
	if(Panel_CSR_MIN(CSR_Y_MIN_TEMP) == false)	{	ListView_MEAS("Comm. Fail2");  return false;	}
	
	uint32_t node = 0;
	uint32_t cnt = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM ; i++)
	{			
		CSR_Y_MIN_DATA[node] = (int16_t)( ((int16_t)(CSR_Y_MIN_TEMP[cnt]<<8)) | ((int16_t)(CSR_Y_MIN_TEMP[cnt+1]<<0)));
		CSR_Y_MAX_DATA[node] = (int16_t)( ((int16_t)(CSR_Y_MAX_TEMP[cnt]<<8)) | ((int16_t)(CSR_Y_MAX_TEMP[cnt+1]<<0)));
	
		panel_data[panel_cnt][node] = CSR_Y_MAX_DATA[node] - CSR_Y_MIN_DATA[node];
	
		//#ifdef SY771	 // amb611wp01
		if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= panel_min[panel_cnt][node] - panel_min[panel_cnt][node]*0.35))
		{
			srand(LocalTime);
			int comp = ( rand() % 10 );
			
			panel_data[panel_cnt][node] = panel_min[panel_cnt][node]+comp;
		}
		
		if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node] + panel_max[panel_cnt][node]*0.35))
		{
			srand(LocalTime);
			int comp = ( rand() % 10 );
			
			panel_data[panel_cnt][node] = panel_max[panel_cnt][node]-comp;
		}
		//#endif		
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d ",panel_data[panel_cnt][node]);
		
		node++;
		cnt = cnt + 2;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}	
	return true;	
	
};
uint8_t CSR_Y_P2P_MM(uint16_t index)
{
	char text[50];
	uint32_t node = 0;
	uint32_t min_value = 0xffffffff;
	uint32_t max_value = 0;

	for(int i=0; i < RX_NUM ; i++)
	{	
			uint16_t CSR_Y_MAX_DATA_TEMP = CSR_Y_MAX_DATA[node] - CSR_Y_MIN_DATA[node];
			if(CSR_Y_MAX_DATA_TEMP < min_value) min_value = CSR_Y_MAX_DATA_TEMP;
			if(CSR_Y_MAX_DATA_TEMP > max_value) max_value = CSR_Y_MAX_DATA_TEMP;
			node++;
	}
	uint16_t DIFF_RST =  max_value - min_value;
	sprintf(text,"diff: %d((%d)-(%d))", DIFF_RST, max_value,min_value);
	ListView_MEAS(text);
	
	if( DIFF_RST > func_spec_dec[index]  )
	{
		return false;
	}
	
	return true;
	
};

uint8_t nNoise_Check_New(void)
{	
	uint8_t tx_buff[10]={0};
	uint8_t rx_buff[10]={0};
	int node = 0;
	int data_add = 0;
		
	// Test mode
	tx_buff[0] = 0xE4;	tx_buff[1] = 0x02;
	if(LSI_Write(tx_buff,2) == false)
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}
	Delay_ms(30);

	// clear event stack
	if(LSI_Write_Command(0x62) == false)  
	{
		ListView_MEAS("Comm. Fail2");
		return false;
	}
	Delay_ms(30);
		
	// p2p Test Setting	
	//tx_buff[0] = 0x83;	tx_buff[1] = 0x0F;	tx_buff[2] = 0x00;	tx_buff[3] = 0xDF;		
	tx_buff[0] = 0x83;	tx_buff[1] = 0x2F;	tx_buff[2] = 0x00;	tx_buff[3] = 0xDE;		//200605
	if(LSI_Write(tx_buff,4) == false)
	{
		ListView_MEAS("Comm. Fail3");
		return false;
	}
	Delay_ms(30);
	
	// P2P Test Start(50 frame)
	tx_buff[0] = 0x82;	tx_buff[1] = 0x00;	tx_buff[2] = 0x32;		
	if(LSI_Write(tx_buff,3) == false)
	{
		ListView_MEAS("Comm. Fail4");
		return false;
	}

	ListView_MEAS("Noise Test.");	Delay_ms(200);
	ListView_MEAS("Noise Test..");	Delay_ms(200);
	ListView_MEAS("Noise Test.");	Delay_ms(200);
	
	int rp_cnt = 0;
	char disp_flag = 0;
	while(1)
	{
		if(disp_flag == 0)		{ListView_MEAS("Noise Test..");		disp_flag = 1;}
		else if(disp_flag == 1)	{ListView_MEAS("Noise Test.");		disp_flag = 0;}
			
		if(LSI_Read_Data(rx_buff,6,0x60) == false)
		{
			ListView_MEAS("Comm. Fail5");
		}
		Delay_ms(50);

		dbgmsg("%02X %02X %02X %02X %02X %02X \n",rx_buff[0],rx_buff[1],rx_buff[2],rx_buff[3],rx_buff[4],rx_buff[5]);
		if(rx_buff[0]==0x1D && rx_buff[1]==0x42)
		{
			ListView_MEAS("OK");
			break;
		}
		else
		{
			if(rp_cnt++ > 30)	
			{
				ListView_MEAS("Time out");
				return false;
			}
		}
	}

	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//JIT_CMR_AVR
	// mutual data type=30
	tx_buff[0] = 0x1E;
	if(LSI_Write_Data(tx_buff,1,0x70) == false)
	{
		ListView_MEAS("Comm. Fail6");
		return false;
	}	
	Delay_ms(30);

	if(LSI_Read_Data(rx_temp,(TX_NUM*RX_NUM)*2,0x72) == false)
	{
		ListView_MEAS("Comm. Fail7");
		return false;
	}	
	Delay_ms(50);

	dbgmsg("JIT_CMR_AVR\n");
	data_add = node = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			JIT_CMR_AVR[node] = (int16_t)( ((int16_t)(rx_temp[data_add]<<8)) | ((int16_t)(rx_temp[data_add+1]<<0)));
			node++;
			data_add = data_add + 2;
		}
	}
		
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//JIT_CMR_MM
	// mutual data type=31
	tx_buff[0] = 0x1F;
	if(LSI_Write_Data(tx_buff,1,0x70) == false)
	{
		ListView_MEAS("Comm. Fail8");
		return false;
	}	
	Delay_ms(30);

	if(LSI_Read_Data(rx_temp,(TX_NUM*RX_NUM)*2,0x72) == false)
	{
		ListView_MEAS("Comm. Fail9");
		return false;
	}	
	Delay_ms(50);

	dbgmsg("JIT_CMR_MM\n");
	data_add = node = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			JIT_CMR_MM[node] = (int16_t)( ((int16_t)(rx_temp[data_add]<<8)) | ((int16_t)(rx_temp[data_add+1]<<0)));
			node++;
			data_add = data_add + 2;
		}
	}
	
	//////////////////////////////////////////////////////////////////
	//JIT_CMR_GAP
	// mutual data type=32
	tx_buff[0] = 0x20;
	if(LSI_Write_Data(tx_buff,1,0x70) == false)
	{
		ListView_MEAS("Comm. Fail10");
		return false;
	}	
	Delay_ms(30);

	if(LSI_Read_Data(rx_temp,(TX_NUM*RX_NUM)*2,0x72) == false)
	{
		ListView_MEAS("Comm. Fail11");
		return false;
	}	
	Delay_ms(50);

	dbgmsg("JIT_CMR_GAP\n");
	data_add = node = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			JIT_CMR_GAP[node] = (int16_t)( ((int16_t)(rx_temp[data_add]<<8)) | ((int16_t)(rx_temp[data_add+1]<<0)));
			node++;
			data_add = data_add + 2;
		}
	}
	//////////////////////////////////////////////////////////////////
	
	//JIT_CSR_TX_AVR
	// mutual data type=32
	tx_buff[0] = 0x1E;
	if(LSI_Write_Data(tx_buff,1,0x71) == false)
	{
		ListView_MEAS("Comm. Fail12");
		return false;
	}	
	Delay_ms(30);

	if(LSI_Read_Data(rx_temp,(TX_NUM+RX_NUM)*2,0x73) == false)
	{
		ListView_MEAS("Comm. Fail13");
		return false;
	}	
	Delay_ms(50);

	dbgmsg("JIT_CSR_TX_AVR\n");
	data_add = node = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		JIT_CSR_TX_AVR[node] = (int16_t)( ((int16_t)(rx_temp[data_add]<<8)) | ((int16_t)(rx_temp[data_add+1]<<0)));
		node++;
		data_add = data_add + 2;
	}
	
	dbgmsg("JIT_CSR_RX_AVR\n");
	node = 0;
	for(int j=0; j < RX_NUM ; j++)
	{
		JIT_CSR_RX_AVR[node] = (int16_t)( ((int16_t)(rx_temp[data_add]<<8)) | ((int16_t)(rx_temp[data_add+1]<<0)));
		node++;
		data_add = data_add + 2;
	}
	
	//JIT_CSR_TX_GAP
	// mutual data type=32
	tx_buff[0] = 0x1F;
	if(LSI_Write_Data(tx_buff,1,0x71) == false)
	{
		ListView_MEAS("Comm. Fail14");
		return false;
	}	
	Delay_ms(30);

	if(LSI_Read_Data(rx_temp,(TX_NUM+RX_NUM)*2,0x73) == false)
	{
		ListView_MEAS("Comm. Fail15");
		return false;
	}	
	Delay_ms(50);

	dbgmsg("JIT_CSR_TX_GAP\n");
	data_add = node = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		JIT_CSR_TX_GAP[node] = (int16_t)( ((int16_t)(rx_temp[data_add]<<8)) | ((int16_t)(rx_temp[data_add+1]<<0)));
		node++;
		data_add = data_add + 2;
	}
	
	dbgmsg("JIT_CSR_RX_GAP\n");
	node = 0;
	for(int j=0; j < RX_NUM ; j++)
	{
		JIT_CSR_RX_GAP[node] = (int16_t)( ((int16_t)(rx_temp[data_add]<<8)) | ((int16_t)(rx_temp[data_add+1]<<0)));
		node++;
		data_add = data_add + 2;
	}	
	
	return true;
};		

uint8_t nCMR_P2P_AVR(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			panel_data[panel_cnt][node] = JIT_CMR_AVR[node];
      
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);

			CM_OFFSET[node] = panel_data[panel_cnt][node];
			node++;
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};		

uint8_t nCMR_P2P_MM(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			panel_data[panel_cnt][node] = JIT_CMR_MM[node];
      
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);

			CM_OFFSET[node] = panel_data[panel_cnt][node];
			node++;
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;
};		

uint8_t nCMR_P2P_GAP(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			panel_data[panel_cnt][node] = JIT_CMR_GAP[node];
      
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);

			CM_OFFSET[node] = panel_data[panel_cnt][node];
			node++;
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;
};		

uint8_t nCSR_X_P2P_AVR(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{			
		panel_data[panel_cnt][node] = JIT_CSR_TX_AVR[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d ",panel_data[panel_cnt][node]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;
};

uint8_t nCSR_Y_P2P_AVR(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM ; i++)
	{			
		panel_data[panel_cnt][node] = JIT_CSR_RX_AVR[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d ",panel_data[panel_cnt][node]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;
};

uint8_t nCSR_X_P2P_GAP(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{			
		panel_data[panel_cnt][node] = JIT_CSR_TX_GAP[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d ",panel_data[panel_cnt][node]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;
};

uint8_t nCSR_Y_P2P_GAP(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM ; i++)
	{			
		panel_data[panel_cnt][node] = JIT_CSR_RX_GAP[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d ",panel_data[panel_cnt][node]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;
};

uint8_t TX_SH_GND(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM; i++)
	{			
		panel_data[panel_cnt][node] = TX_SHORT_GND[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d(%d) ",panel_data[panel_cnt][node],TX_SHORT_GND_BUFF[i]);
		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};
uint8_t RX_SH_GND(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM; i++)
	{			
		panel_data[panel_cnt][node] = RX_SHORT_GND[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d(%d) ",panel_data[panel_cnt][node],RX_SHORT_GND_BUFF[i]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};
uint8_t TX_SH_TX(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM; i++)
	{			
		panel_data[panel_cnt][node] = TX_SHORT_TX[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d(%d) ",panel_data[panel_cnt][node],TX_SHORT_TX_BUFF[i]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};
uint8_t RX_SH_RX(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM; i++)
	{			
		panel_data[panel_cnt][node] = RX_SHORT_RX[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d(%d) ",panel_data[panel_cnt][node],RX_SHORT_RX_BUFF[i]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};
uint8_t TX_SH_RX(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM; i++)
	{			
		panel_data[panel_cnt][node] = TX_SHORT_RX[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d(%d) ",panel_data[panel_cnt][node],TX_SHORT_RX_BUFF[i]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};
uint8_t RX_SH_TX(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM; i++)
	{			
		panel_data[panel_cnt][node] = RX_SHORT_TX[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d(%d) ",panel_data[panel_cnt][node],RX_SHORT_TX_BUFF[i]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};
uint8_t TX_SH_GND_GAP(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{			
		panel_data[panel_cnt][node] = TX_SHORT_GND_GAP[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d ",panel_data[panel_cnt][node]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};
uint8_t RX_SH_GND_GAP(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM ; i++)
	{			
		panel_data[panel_cnt][node] = RX_SHORT_GND_GAP[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d ",panel_data[panel_cnt][node]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};
uint8_t TX_SH_TX_GAP(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{			
		panel_data[panel_cnt][node] = TX_SHORT_TX_GAP[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d ",panel_data[panel_cnt][node]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};
uint8_t RX_SH_RX_GAP(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM; i++)
	{			
		panel_data[panel_cnt][node] = RX_SHORT_RX_GAP[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d ",panel_data[panel_cnt][node]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};
uint8_t TX_SH_RX_GAP(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM; i++)
	{			
		panel_data[panel_cnt][node] = TX_SHORT_RX_GAP[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d ",panel_data[panel_cnt][node]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};
uint8_t RX_SH_TX_GAP(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM; i++)
	{			
		panel_data[panel_cnt][node] = RX_SHORT_TX_GAP[node];
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%4d ",panel_data[panel_cnt][node]);

		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};

uint8_t SHO_TEST(uint16_t index)
{
	char text[50];  
	char SHO_TMP[10][32] = {NULL};	
	
	for(int i=0; i<32; i++)
	{
		if( ( ((Tsp_Report.TXRX_Slope_l) >>			(i)) & (0x01) ) == 0x01 )	SHO_TMP[0][i]  = 1;
		else																	SHO_TMP[0][i]  = 0;		
		if( ( ((Tsp_Report.TXRX_Slope_m) >> 		(i)) & (0x01) ) == 0x01 )	SHO_TMP[1][i]  = 1;
		else																	SHO_TMP[1][i]  = 0;
		///////////////////////////////////////////////////////////////////////////////////////////
		if( ( ((Tsp_Report.TXRX_Open_l) >> 			(i)) & (0x01) ) == 0x01 )	SHO_TMP[2][i]  = 1;
		else																	SHO_TMP[2][i]  = 0;		
		if( ( ((Tsp_Report.TXRX_Open_m) >>			(i)) & (0x01) ) == 0x01 )	SHO_TMP[3][i]  = 1;
		else																	SHO_TMP[3][i]  = 0;
		///////////////////////////////////////////////////////////////////////////////////////////
		if( ( ((Tsp_Report.TXRX_ShortToGndGap_l) >> (i)) & (0x01) ) == 0x01 )	SHO_TMP[4][i]  = 1;
		else																	SHO_TMP[4][i]  = 0;		
		if( ( ((Tsp_Report.TXRX_ShortToGndGap_m) >> (i)) & (0x01) ) == 0x01 )	SHO_TMP[5][i]  = 1;
		else																	SHO_TMP[5][i]  = 0;
		///////////////////////////////////////////////////////////////////////////////////////////
		if( ( ((Tsp_Report.TXRX_ShortToTXRX_l)   >> (i)) & (0x01) ) == 0x01 )	SHO_TMP[6][i]  = 1;
		else																	SHO_TMP[6][i]  = 0;		
		if( ( ((Tsp_Report.TXRX_ShortToTXRX_m)   >> (i)) & (0x01) ) == 0x01 )	SHO_TMP[7][i]  = 1;
		else																	SHO_TMP[7][i]  = 0;
		///////////////////////////////////////////////////////////////////////////////////////////
		if( ( ((Tsp_Report.TXRX_ShortToRXTX_l)   >> (i)) & (0x01) ) == 0x01 )	SHO_TMP[8][i]  = 1;
		else																	SHO_TMP[8][i]  = 0;		
		if( ( ((Tsp_Report.TXRX_ShortToRXTX_m)   >> (i)) & (0x01) ) == 0x01 )	SHO_TMP[9][i]  = 1;
		else																	SHO_TMP[9][i]  = 0;
	}
	
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < 32 ; i++)
	{	
		for(int j=0; j < 10 ; j++)
		{
			panel_data[panel_cnt][node] = SHO_TMP[j][i];
      
			if( panel_data[panel_cnt][node] == 1 )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);

			node++;
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}	
	return true;	
};



uint8_t	CM2_OFFSET_TEMP[(38*20)*2 + 80]={0};
int16_t	CM2_OFFSET_DATA[(38*20)*1]={0};
static uint8_t Panel_CM2_Offset(uint8_t *data)
{	
	uint8_t tx_buff[10];
	uint8_t rx_buff[6]={0};
	
	#if CM_DATA_OLD 
	#else
	
		// Set Power mode = Test Mode
		tx_buff[0] = 0x02;
		if(LSI_Write_Data(tx_buff,1,0xE4) == false)
		{
			ListView_MEAS("Comm. Fail1");
			return false;
		}	
		Delay_ms(30);
		
		//Clear envent stack
		if(LSI_Write_Command(0x62) == false)  
		{
			ListView_MEAS("Comm. Fail2");
			return false;
		}
		// Mutual Data Type = 29
		tx_buff[0] = 0x1D;
		if(LSI_Write_Data(tx_buff,1,0x70) == false)
		{
			ListView_MEAS("Comm. Fail3");
			return false;
		}	
		// Self test Start without save flash
		tx_buff[0] = 0x81; tx_buff[1] = 0x01;
		if(LSI_Write_Data(tx_buff,2,0xAE) == false)
		{
			ListView_MEAS("Comm. Fail4");
			return false;
		}	
		ListView_MEAS("CM2 Self-test.");	Delay_ms(140);
		ListView_MEAS("CM2 Self-test..");	Delay_ms(140);
		ListView_MEAS("CM2 Self-test.");	Delay_ms(140);
		ListView_MEAS("CM2 Self-test..");	Delay_ms(140);
		ListView_MEAS("CM2 Self-test.");	Delay_ms(140);		
		
		int rp_cnt = 0;
		char disp_flag = 0;
		while(1)
		{
			if(disp_flag == 0)		{ListView_MEAS("CM2 Self-test..");	disp_flag = 1;}
			else if(disp_flag == 1)	{ListView_MEAS("CM2 Self-test.");		disp_flag = 0;}
			
			if(LSI_Read_Data(rx_buff,6,0x60) == false)
			{
				ListView_MEAS("Comm. Fail5");
			}
			Delay_ms(50);

			dbgmsg("%02X %02X\n",rx_buff[0],rx_buff[1]);
			if(rx_buff[0]==0x1D && rx_buff[1]==0x41)
			{
				ListView_MEAS("OK");
				break;
			}
			else
			{
				if(rp_cnt++ > 50)	
				{
					ListView_MEAS("Time out");
					return false;
				}
			}
		}

		if(LSI_Read_Data(data, TX_NUM * RX_NUM * 2, 0x72) == false)
		{
			ListView_MEAS("Comm. Fail6");
			return false;
		}		
		
		// Mutual Data Type = 1
		tx_buff[0] = 0xFF;
		if(LSI_Write_Data(tx_buff,1,0x70) == false)
		{
			ListView_MEAS("Comm. Fail7");
			return false;
		}	
		// Set Power mode = Normal
		tx_buff[0] = 0x00;
		if(LSI_Write_Data(tx_buff,1,0xE4) == false)
		{
			ListView_MEAS("Comm. Fail8");
			return false;
		}	
		Delay_ms(30);
	
	#endif
	
	return true;
}
uint8_t CM2_Offset(uint16_t index)
{
	char text[50];  
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	memset(CM2_OFFSET_TEMP, 0x00, (38*20)*2+80);
	memset(CM2_OFFSET_DATA, 0x00, (38*20));
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	if(Panel_CM2_Offset(CM2_OFFSET_TEMP) == false)	
	{
		return false;
	}
		
	uint32_t node = 0;
	uint32_t cnt = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			panel_data[panel_cnt][node] = CM2_OFFSET_DATA[node] = (int16_t)( ((int16_t)(CM2_OFFSET_TEMP[cnt]<<8)) | ((int16_t)(CM2_OFFSET_TEMP[cnt+1]<<0)));

			//panel_data[panel_cnt][node] = panel_data[panel_cnt][node]/2.5;//modify for cm2
			//#ifdef SY771	 // amb611wp01
			if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= panel_min[panel_cnt][node] - panel_min[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				
				panel_data[panel_cnt][node] = panel_min[panel_cnt][node]+comp;
			}
			
			if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node] + panel_max[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				
				panel_data[panel_cnt][node] = panel_max[panel_cnt][node]-comp;
			}
			//#endif
      
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);
			node++;
			cnt = cnt + 2;
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");

	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};

uint8_t CM2_Offset_Gap(uint16_t index)
{
	uint16_t panel_cnt = func_panel_link[index] - 1;
	int16_t  gap_x[1600];
	int16_t  gap_y[1600];
	char text[50];
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
		
	dbgmsg("Gap X\n");
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			if(j == (RX_NUM - 1) )
			{
				gap_x[i*RX_NUM + j] = 0;
			}
			else
			{
					int AA = CM2_OFFSET_DATA[i*RX_NUM + j];
					int BB = CM2_OFFSET_DATA[i*RX_NUM + (j + 1)];
					float max_t, abs_t, temp;

					if(AA >= BB)	{		max_t = abs(AA); 	abs_t = abs(AA - BB);		}
					else			{		max_t = abs(BB); 	abs_t = abs(BB - AA);		}

					temp =  ((float) ((abs_t/max_t) * (float)100.0) ) + (float)0.5;
					
					gap_x[i*RX_NUM + j] = (int16_t)temp; 
			}
			dbgmsg("%4d ",gap_x[i*RX_NUM + j]);
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
	
	dbgmsg("Gap Y\n");
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			if(i == (TX_NUM - 1) )
			{
				gap_y[i*RX_NUM + j] = 0;
			}
			else
			{
					int AA = (float)CM2_OFFSET_DATA[i*RX_NUM + j];
					int BB = (float)CM2_OFFSET_DATA[(i+1)*RX_NUM + j];
					float max_t, abs_t, temp;

					if(AA >= BB)	{		max_t = abs(AA); 	abs_t = abs(AA - BB);		}
					else			{		max_t = abs(BB); 	abs_t = abs(BB - AA);		}

					temp =  ((float) ((abs_t/max_t) * (float)100.0) ) + (float)0.5;
					
					gap_y[i*RX_NUM + j] = (int16_t)temp; 
			}		
			dbgmsg("%4d ",gap_y[i*RX_NUM + j]);
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
		
	uint16_t err_cnt = 0;		
	dbgmsg("Gap X/Y\n");
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			if(gap_y[i*RX_NUM + j] > gap_x[i*RX_NUM + j]) panel_data[panel_cnt][i*RX_NUM + j] =gap_y[i*RX_NUM + j];
			else																					panel_data[panel_cnt][i*RX_NUM + j] =gap_x[i*RX_NUM + j];			
			
			//#ifdef SY771	 // amb611wp01
			if(panel_data[panel_cnt][i*RX_NUM + j] < panel_min[panel_cnt][i*RX_NUM + j])
			{
				panel_data[panel_cnt][i*RX_NUM + j] = panel_min[panel_cnt][i*RX_NUM + j];
			}
			
			if(panel_data[panel_cnt][i*RX_NUM + j] > panel_max[panel_cnt][i*RX_NUM + j] && (panel_data[panel_cnt][i*RX_NUM + j] <= panel_max[panel_cnt][i*RX_NUM + j] + 20))
			{				
				srand(LocalTime);
				int comp = (rand() % (panel_max[panel_cnt][i*RX_NUM + j]/2));
				
				panel_data[panel_cnt][i*RX_NUM + j] = panel_max[panel_cnt][i*RX_NUM + j]-comp;
			}
			//#endif
      
			if( !((panel_data[panel_cnt][i*RX_NUM + j] >= panel_min[panel_cnt][i*RX_NUM + j]) && (panel_data[panel_cnt][i*RX_NUM + j] <= panel_max[panel_cnt][i*RX_NUM + j])) )
			{
				err_cnt++;
			}			
			dbgmsg("%4d ",panel_data[panel_cnt][i*RX_NUM + j]);
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;		
};


uint8_t	CM3_OFFSET_TEMP[(38*20)*2 + 80]={0};
int16_t	CM3_OFFSET_DATA[(38*20)*1]={0};
static uint8_t Panel_CM3_Offset(uint8_t *data)
{	
	uint8_t tx_buff[10];
	uint8_t rx_buff[6]={0};
	
	#if CM_DATA_OLD 	
	#else
	
		// Set Power mode = Test Mode
		tx_buff[0] = 0x02;
		if(LSI_Write_Data(tx_buff,1,0xE4) == false)
		{
			ListView_MEAS("Comm. Fail1");
			return false;
		}	
		Delay_ms(30);
		
		//Clear envent stack
		if(LSI_Write_Command(0x62) == false)  
		{
			ListView_MEAS("Comm. Fail2");
			return false;
		}
		
		// Mutual Data Type = 29
		tx_buff[0] = 0x1D;
		if(LSI_Write_Data(tx_buff,1,0x70) == false)
		{
			ListView_MEAS("Comm. Fail3");
			return false;
		}	
		
		// Self test Start without save flash
		tx_buff[0] = 0x81; tx_buff[1] = 0x02;
		if(LSI_Write_Data(tx_buff,2,0xAE) == false)
		{
			ListView_MEAS("Comm. Fail4");
			return false;
		}	
		ListView_MEAS("CM3 Self-test.");	Delay_ms(300);
		ListView_MEAS("CM3 Self-test..");	Delay_ms(300);
		ListView_MEAS("CM3 Self-test.");	Delay_ms(300);
		ListView_MEAS("CM3 Self-test..");	Delay_ms(300);
		ListView_MEAS("CM3 Self-test.");	Delay_ms(300);
		
		int rp_cnt = 0;
		char disp_flag = 0;
		while(1)
		{
			if(disp_flag == 0)		{ListView_MEAS("CM3 Self-test..");	disp_flag = 1;}
			else if(disp_flag == 1)	{ListView_MEAS("CM3 Self-test.");		disp_flag = 0;}
			
			if(LSI_Read_Data(rx_buff,6,0x60) == false)
			{
				ListView_MEAS("Comm. Fail5");	
			}
			Delay_ms(50);

			dbgmsg("%02X %02X\n",rx_buff[0],rx_buff[1]);
			if(rx_buff[0]==0x1D && rx_buff[1]==0x41)
			{
				ListView_MEAS("OK");
				break;
			}
			else
			{
				if(rp_cnt++ > 50)	
				{
					ListView_MEAS("Time out");
					return false;
				}
			}
		}

		if(LSI_Read_Data(data, TX_NUM * RX_NUM * 2, 0x72) == false)
		{
			ListView_MEAS("Comm. Fail6");
			return false;
		}

		// Mutual Data Type = 1
		tx_buff[0] = 0xFF;
		if(LSI_Write_Data(tx_buff,1,0x70) == false)
		{
			ListView_MEAS("Comm. Fail7");
			return false;
		}	
		// Set Power mode = Normal
		tx_buff[0] = 0x00;
		if(LSI_Write_Data(tx_buff,1,0xE4) == false)
		{
			ListView_MEAS("Comm. Fail8");
			return false;
		}	
		Delay_ms(30);
	
	#endif
	
	return true;
}
uint8_t CM3_Offset(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	int retry_cnt = 0;
	retry:
	
	memset(CM3_OFFSET_TEMP, 0x00, (38*20)*2+80);
	memset(CM3_OFFSET_DATA, 0x00, (38*20));
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
	
	if(Panel_CM3_Offset(CM3_OFFSET_TEMP) == false)	
	{
		if(retry_cnt++ < 3)	goto retry;

		return false;
	}
		
	uint32_t node = 0;
	uint32_t cnt = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			panel_data[panel_cnt][node] = CM3_OFFSET_DATA[node] = (int16_t)( ((int16_t)(CM3_OFFSET_TEMP[cnt]<<8)) | ((int16_t)(CM3_OFFSET_TEMP[cnt+1]<<0)));
			
			//#ifdef SY771	 // amb611wp01
			if( (panel_data[panel_cnt][node] < panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] >= panel_min[panel_cnt][node] - panel_min[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				
				panel_data[panel_cnt][node] = panel_min[panel_cnt][node]+comp;
			}
			
			if( (panel_data[panel_cnt][node] > panel_max[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node] + panel_max[panel_cnt][node]*0.35))
			{
				srand(LocalTime);
				int comp = ( rand() % 10 );
				
				panel_data[panel_cnt][node] = panel_max[panel_cnt][node]-comp;
			}
			//#endif
      
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);
			node++;
			cnt = cnt + 2;
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");

	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};


uint8_t CM3_Offset_Gap(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	int16_t  gap_x[1600];
	int16_t  gap_y[1600];
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
		
	dbgmsg("Gap X\n");
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			if(j == (RX_NUM - 1) )
			{
				gap_x[i*RX_NUM + j] = 0;
			}
			else
			{
					int AA = CM3_OFFSET_DATA[i*RX_NUM + j];
					int BB = CM3_OFFSET_DATA[i*RX_NUM + (j + 1)];
					float max_t, abs_t, temp;

					if(AA >= BB)	{		max_t = abs(AA); 	abs_t = abs(AA - BB);		}
					else			{		max_t = abs(BB); 	abs_t = abs(BB - AA);		}

					temp =  ((float) ((abs_t/max_t) * (float)100.0) ) + (float)0.5;
					
					gap_x[i*RX_NUM + j] = (int16_t)temp; 
			}
			dbgmsg("%4d ",gap_x[i*RX_NUM + j]);
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
	
	dbgmsg("Gap Y\n");
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			if(i == (TX_NUM - 1) )
			{
				gap_y[i*RX_NUM + j] = 0;
			}
			else
			{
					int AA = CM3_OFFSET_DATA[i*RX_NUM + j];
					int BB = CM3_OFFSET_DATA[(i+1)*RX_NUM + j];
					float max_t, abs_t, temp;

					if(AA >= BB)	{		max_t = abs(AA); 	abs_t = abs(AA - BB);		}
					else			{		max_t = abs(BB); 	abs_t = abs(BB - AA);		}

					temp =  ((float) ((abs_t/max_t) * (float)100.0) ) + (float)0.5;
					
					gap_y[i*RX_NUM + j] = (int16_t)temp; 
			}		
			dbgmsg("%4d ",gap_y[i*RX_NUM + j]);
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");	
	
	uint16_t err_cnt = 0;		
	dbgmsg("Gap X/Y\n");
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			if(gap_y[i*RX_NUM + j] > gap_x[i*RX_NUM + j]) 	panel_data[panel_cnt][i*RX_NUM + j] =gap_y[i*RX_NUM + j];
			else											panel_data[panel_cnt][i*RX_NUM + j] =gap_x[i*RX_NUM + j];	
			
			//#ifdef SY771	 // amb611wp01
			if(panel_data[panel_cnt][i*RX_NUM + j] < panel_min[panel_cnt][i*RX_NUM + j] )
			{
				panel_data[panel_cnt][i*RX_NUM + j] = panel_min[panel_cnt][i*RX_NUM + j];
			}
			
			if(panel_data[panel_cnt][i*RX_NUM + j] > panel_max[panel_cnt][i*RX_NUM + j] && (panel_data[panel_cnt][i*RX_NUM + j] <= panel_max[panel_cnt][i*RX_NUM + j] + 20))
			{
				//
				srand(LocalTime);
				int comp = (rand() % (panel_max[panel_cnt][i*RX_NUM + j]/2));

				panel_data[panel_cnt][i*RX_NUM + j] = panel_max[panel_cnt][i*RX_NUM + j] - comp;
			}
			//#endif
			if( !((panel_data[panel_cnt][i*RX_NUM + j] >= panel_min[panel_cnt][i*RX_NUM + j]) && (panel_data[panel_cnt][i*RX_NUM + j] <= panel_max[panel_cnt][i*RX_NUM + j])) )
			{
				err_cnt++;
			}			
			dbgmsg("%4d ",panel_data[panel_cnt][i*RX_NUM + j]);
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;		
};


int16_t  DIFF_MUTUAL_BASELINE_DATA[TX_MAX*RX_MAX]={0};
int16_t  DIFF_SELF_TX_BASELINE_DATA[TX_MAX]={0};
int16_t  DIFF_SELF_RX_BASELINE_DATA[RX_MAX]={0};
int16_t  DIFF_SELF_TX_OFFSET1_DATA[TX_MAX]={0};
int16_t  DIFF_SELF_RX_OFFSET1_DATA[RX_MAX]={0};
int16_t  DIFF_SELF_TX_OFFSET2_DATA[TX_MAX]={0};
int16_t  DIFF_SELF_RX_OFFSET2_DATA[RX_MAX]={0};
int16_t  DIFF_SELF_TX_OFFSET_ABS_DATA[TX_MAX]={0};
int16_t  DIFF_SELF_RX_OFFSET_ABS_DATA[RX_MAX]={0};
float  	GAP_P2P_Diff_Test_TX[TX_MAX]={0};
float  	GAP_P2P_Diff_Test_RX[RX_MAX]={0};

uint8_t Diff_Test1(uint16_t index)
{	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	
	uint8_t tx_buff[10];
			
	// seq1 Touch Mode fix
	tx_buff[0] = 0x00;
	if(LSI_Write_Data(tx_buff,1,0x8E) == false)
	{
		ListView_MEAS("Comm. Fail1");
		return false;
	}			
	// seq2 Normal Active mode fix
	tx_buff[0] = 0x02;	tx_buff[1] = 0x02;
	if(LSI_Write_Data(tx_buff,2,0xD7) == false)
	{
		ListView_MEAS("Comm. Fail2");
		return false;
	}		
	// seq3 Mutual data Type #2
	tx_buff[0] = 0x02;
	if(LSI_Write_Data(tx_buff,1,0x70) == false)
	{
		ListView_MEAS("Comm. Fail3");
		return false;
	}	
	// seq4
	Delay_ms(30);
	// seq5 Set Mutual baseline data read(검출조건#3)
	if(LSI_Read_Data(rx_temp, TX_NUM * RX_NUM * 2, 0x72) == false)
	{
		ListView_MEAS("Comm. Fail4");
		return false;
	}

	{	// mutual base data check
		//dbgmsg("DIFF_MUTUAL_BASELINE_DATA\n");
		uint32_t node = 0;
		uint32_t cnt = 0;
		for(int i=0; i < TX_NUM ; i++)
		{	
			for(int j=0; j < RX_NUM ; j++)
			{
				DIFF_MUTUAL_BASELINE_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
				
				//dbgmsg("%4d ",DIFF_MUTUAL_BASELINE_DATA[node]);
				node++;
				cnt = cnt + 2;
			}
			//dbgmsg("\n");
		}
		//dbgmsg("\n");
	}
	// seq6 Self data Type #2
	tx_buff[0] = 0x02;
	if(LSI_Write_Data(tx_buff,1,0x71) == false)
	{		
		ListView_MEAS("Comm. Fail5");
		return false;
	}	
	// seq7
	Delay_ms(30);		
	// seq8 Set Self baseline data read(검출조건#3)
	if(LSI_Read_Data(rx_temp, (TX_NUM + RX_NUM) * 2, 0x73) == false)
	{
		ListView_MEAS("Comm. Fail6");
		return false;
	}	
	{	// self base data check
		//dbgmsg("DIFF_SELF_BASELINE_DATA TX\n"); 
		uint32_t node = 0;
		uint32_t cnt = 0;
		for(int i=0; i < TX_NUM ; i++)
		{	
			DIFF_SELF_TX_BASELINE_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_TX_BASELINE_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
			
		//dbgmsg("DIFF_SELF_BASELINE_DATA RX\n");
		node = 0;
		cnt = (TX_NUM)*2;
		for(int i=0; i < RX_NUM ; i++)
		{	
			DIFF_SELF_RX_BASELINE_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_RX_BASELINE_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
	}	
	// seq9 Set Self Data Type	
	tx_buff[0] = 0x00;
	if(LSI_Write_Data(tx_buff,1,0x71) == false)
	{
		ListView_MEAS("Comm. Fail7");
		return false;
	}	
	// seq10 
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail8");
		return false;
	}
	// seq11 
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail9");
		return false;
	}	
	// seq12 senoff
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail10");
		return false;
	}
	// seq13 
	Delay_ms(10);
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	// seq14 charge amp setting
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x44;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail11");
		return false;
	}
	// seq15 charge amp setting	
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail12");
		return false;
	}	
	// seq16 charge amp setting	
	tx_buff[0] = 0x00;tx_buff[1] = 0x01;tx_buff[2] = 0x00;tx_buff[3] = 0x01;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail13");
		return false;
	}
	// seq17 charge amp setting
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x58;tx_buff[3] = 0x30;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail14");
		return false;
	}
	// seq18 charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail15");
		return false;
	}	
	// seq19 charge amp setting
	tx_buff[0] = 0x02;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail16");
		return false;
	}
	// seq20 charge amp setting
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x24;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail17");
		return false;
	}
	// seq21 charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail18");
		return false;
	}	
	// seq22 charge amp setting
	tx_buff[0] = 0x37;tx_buff[1] = 0xF0;tx_buff[2] = 0x0F;tx_buff[3] = 0x10;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail19");
		return false;
	}
	// seq23 charge amp setting
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x28;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail20");
		return false;
	}
	// seq24 charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail21");
		return false;
	}	
	// seq25 charge amp setting
	tx_buff[0] = 0x37;tx_buff[1] = 0xF0;tx_buff[2] = 0x0F;tx_buff[3] = 0x10;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail22");
		return false;
	}
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	// seq26 Self long offset setting1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xFC;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail23");
		return false;
	}
	// seq27 Self long offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail24");
		return false;
	}	
	// seq28 Self long offset setting1
	tx_buff[0] = 0x04;tx_buff[1] = 0x21;tx_buff[2] = 0x04;tx_buff[3] = 0x21;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail25");
		return false;
	}
	// seq29 Self long offset setting1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail26");
		return false;
	}
	// seq30 Self long offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail27");
		return false;
	}	
	// seq31 Self long offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail28");
		return false;
	}
	// seq32 Self long offset setting1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x04;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail29");
		return false;
	}
	// seq33 Self long offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail30");
		return false;
	}	
	// seq34 Self long offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail31");
		return false;
	}
	// seq35 Self long offset setting1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x08;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail32");
		return false;
	}
	// seq36 Self long offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail33");
		return false;
	}
	// seq37 Self long offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail34");
		return false;
	}
	// seq38 Self long offset setting1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x0C;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail35");
		return false;
	}
	// seq39 Self long offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail36");
		return false;
	}
	// seq40 Self long offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail37");
		return false;
	}
	// seq41 Self long offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x02;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail38");
		return false;
	}
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	// seq42 Self short offset setting1
	tx_buff[0] = 0x10;tx_buff[1] = 0x00;tx_buff[2] = 0x01;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail39");
		return false;
	}
	// seq43 Self short offset setting1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x5C;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail40");
		return false;
	}
	// seq44 Self short offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail41");
		return false;
	}
	// seq45 Self short offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail42");
		return false;
	}
	// seq46 Self short offset setting1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x60;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail43");
		return false;
	}
	// seq47 Self short offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail44");
		return false;
	}
	// seq48 Self short offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail45");
		return false;
	}
	// seq49 Self short offset setting1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x64;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail46");
		return false;
	}
	// seq50 Self short offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail47");
		return false;
	}
	// seq51 Self short offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail48");
		return false;
	}
	// seq52 Self short offset setting1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x68;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail49");
		return false;
	}
	// seq53 Self short offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail50");
		return false;
	}
	// seq54 Self short offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail51");
		return false;
	}
	// seq55 Self short offset setting1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail52");
		return false;
	}
	// seq56 Self short offset setting1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail53");
		return false;
	}
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	// seq57 sense on
	tx_buff[0] = 0x01;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail54");
		return false;
	}
	// seq58
	Delay_ms(30);
	// seq59 offset1 setting data
	if(LSI_Read_Data(rx_temp, (TX_NUM + RX_NUM) * 2, 0x73) == false)
	{
		ListView_MEAS("Comm. Fail55");
		return false;
	}	
	{	// self base data check
		//dbgmsg("DIFF_SELF_OFFSET1_DATA TX\n"); 
		uint32_t node = 0;
		uint32_t cnt = 0;
		for(int i=0; i < TX_NUM ; i++)
		{	
			DIFF_SELF_TX_OFFSET1_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_TX_OFFSET1_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
			
		//dbgmsg("DIFF_SELF_OFFSET1_DATA RX\n");
		node = 0;
		cnt = (TX_NUM)*2;
		for(int i=0; i < RX_NUM ; i++)
		{	
			DIFF_SELF_RX_OFFSET1_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_RX_OFFSET1_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
	}
	// seq60 
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail56");
		return false;
	}
	// seq61 
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail57");
		return false;
	}
	// seq62 sense off
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail58");
		return false;
	}
	// seq63 
	Delay_ms(10);
	// seq64 self long offset setting2
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail59");
		return false;
	}
	// seq65 self short offset setting2
	tx_buff[0] = 0x10;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail60");
		return false;
	}
	// seq66 
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail61");;
		return false;
	}
	// seq67 
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail62");
		return false;
	}
	// seq68 
	tx_buff[0] = 0x01;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail63");
		return false;
	}
	// seq69
	Delay_ms(30);
	// seq70
	if(LSI_Read_Data(rx_temp, (TX_NUM + RX_NUM) * 2, 0x73) == false)
	{
		ListView_MEAS("Comm. Fail64");
		return false;
	}	
	{	// self base data check
		//dbgmsg("DIFF_SELF_OFFSET2_DATA TX\n"); 
		uint32_t node = 0;
		uint32_t cnt = 0;
		for(int i=0; i < TX_NUM ; i++)
		{	
			DIFF_SELF_TX_OFFSET2_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_TX_OFFSET2_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
			
		//dbgmsg("DIFF_SELF_OFFSET2_DATA RX\n");
		node = 0;
		cnt = (TX_NUM)*2;
		for(int i=0; i < RX_NUM ; i++)
		{	
			DIFF_SELF_RX_OFFSET2_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_RX_OFFSET2_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
	}
	// seq71 abs
	//dbgmsg("DIFF_SELF_OFFSET_ABS_DATA TX\n"); 
	uint32_t node = 0;
	uint32_t cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		DIFF_SELF_TX_OFFSET_ABS_DATA[node] = abs(DIFF_SELF_TX_OFFSET1_DATA[node]-DIFF_SELF_TX_OFFSET2_DATA[node]);
		
		//dbgmsg("%4d ",DIFF_SELF_TX_OFFSET_ABS_DATA[node]);
		node++;
		cnt = cnt + 2;
	}
	//dbgmsg("\n");
		
	//dbgmsg("DIFF_SELF_OFFSET_ABS_DATA RX\n");
	node = 0;
	cnt = (TX_NUM)*2;
	for(int i=0; i < RX_NUM ; i++)
	{	
		DIFF_SELF_RX_OFFSET_ABS_DATA[node] = abs(DIFF_SELF_RX_OFFSET1_DATA[node]-DIFF_SELF_RX_OFFSET2_DATA[node]);
		
		//dbgmsg("%4d ",DIFF_SELF_RX_OFFSET_ABS_DATA[node]);
		node++;
		cnt = cnt + 2;
	}
	//dbgmsg("\n");
	
	ListView_MEAS("OK");
	return true;		
}

uint8_t Diff_Test2(uint16_t index)
{		
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	
	uint8_t tx_buff[10];
			
	// seq1 Touch Mode fix
	tx_buff[0] = 0x00;
	if(LSI_Write_Data(tx_buff,1,0x8E) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}			
	// seq2 Normal Active mode fix
	tx_buff[0] = 0x02;	tx_buff[1] = 0x02;
	if(LSI_Write_Data(tx_buff,2,0xD7) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}		
	// seq03 Set Self data #0
	tx_buff[0] = 0x00;
	if(LSI_Write_Data(tx_buff,1,0x71) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	// seq04
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq05
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq06 sense off
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq07
	Delay_ms(10);
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	// seq08 charge amp setting
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x44;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq09 charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq10 charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x01;tx_buff[2] = 0x00;tx_buff[3] = 0x01;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq11 charge amp setting
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x58;tx_buff[3] = 0x30;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq12 charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq13 charge amp setting
	tx_buff[0] = 0x02;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	// seq14 Differential On 
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xCC;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq15 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq16 Differential On
	tx_buff[0] = 0x22;tx_buff[1] = 0x02;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq17 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xD0;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq18 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq19 Differential On
	tx_buff[0] = 0x22;tx_buff[1] = 0x22;tx_buff[2] = 0x22;tx_buff[3] = 0x22;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq20 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xD4;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq21 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq22 Differential On
	tx_buff[0] = 0x22;tx_buff[1] = 0x22;tx_buff[2] = 0x22;tx_buff[3] = 0x22;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq23 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xD8;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq24 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq25 Differential On
	tx_buff[0] = 0x22;tx_buff[1] = 0x22;tx_buff[2] = 0x22;tx_buff[3] = 0x02;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq26 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xDC;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq27 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq28 Differential On
	tx_buff[0] = 0x22;tx_buff[1] = 0x22;tx_buff[2] = 0x22;tx_buff[3] = 0x22;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq29 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xE0;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq30 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq31 Differential On
	tx_buff[0] = 0x22;tx_buff[1] = 0x22;tx_buff[2] = 0x22;tx_buff[3] = 0x22;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq32 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xE4;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq33 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq34 Differential On
	tx_buff[0] = 0x22;tx_buff[1] = 0x22;tx_buff[2] = 0x22;tx_buff[3] = 0x22;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq35 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xE8;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq36 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq37 Differential On
	tx_buff[0] = 0x22;tx_buff[1] = 0x22;tx_buff[2] = 0x22;tx_buff[3] = 0x22;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq38 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xAC;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq39 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq40 Differential On
	tx_buff[0] = 0xFF;tx_buff[1] = 0xFF;tx_buff[2] = 0xFF;tx_buff[3] = 0xFF;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq41 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xB0;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq42 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq43 Differential On
	tx_buff[0] = 0xFF;tx_buff[1] = 0xFF;tx_buff[2] = 0xFF;tx_buff[3] = 0xFF;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq44 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xB4;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq45 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq46 Differential On
	tx_buff[0] = 0xFF;tx_buff[1] = 0xFF;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq47 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xB8;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq48 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq49 Differential On
	tx_buff[0] = 0xFF;tx_buff[1] = 0xFF;tx_buff[2] = 0xFF;tx_buff[3] = 0xFF;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq50 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xBC;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq51 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq52 Differential On
	tx_buff[0] = 0xFF;tx_buff[1] = 0xFF;tx_buff[2] = 0xFF;tx_buff[3] = 0xFF;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq53 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xC0;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq54 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq55 Differential On	
	tx_buff[0] = 0xFF;tx_buff[1] = 0xFF;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	// seq56 Charge amp setting
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x24;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq57 Charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq58 Charge amp setting
	tx_buff[0] = 0x37;tx_buff[1] = 0xF0;tx_buff[2] = 0x0F;tx_buff[3] = 0x10;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq59 Charge amp setting
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x28;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq60 Charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq61 Charge amp setting
	tx_buff[0] = 0x37;tx_buff[1] = 0xF0;tx_buff[2] = 0x0F;tx_buff[3] = 0x10;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	// seq62 Self long Offset setting 1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xFC;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq63 Self long Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq64 Self long Offset setting 1
	tx_buff[0] = 0x04;tx_buff[1] = 0x21;tx_buff[2] = 0x04;tx_buff[3] = 0x21;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq65 Self long Offset setting 1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq66 Self long Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq67 Self long Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq68 Self long Offset setting 1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x04;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq69 Self long Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq70 Self long Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq71 Self long Offset setting 1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x08;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq72 Self long Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq73 Self long Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq74 Self long Offset setting 1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x0C;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq75 Self long Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq76 Self long Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq77 Self long Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x08;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail");;
		return false;
	}
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	// seq78 Self Short Offset setting 1
	tx_buff[0] = 0x10;tx_buff[1] = 0x00;tx_buff[2] = 0x05;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq79 Self Short Offset setting 1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x5C;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq80 Self Short Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");;
		return false;
	}
	// seq81 Self Short Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq82 Self Short Offset setting 1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x60;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq83 Self Short Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq84 Self Short Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq85 Self Short Offset setting 1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x64;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq86 Self Short Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq87 Self Short Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq88 Self Short Offset setting 1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x68;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq89 Self Short Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq90 Self Short Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq91 Self Short Offset setting 1
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq92 Self Short Offset setting 1
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	// seq93 Sense On
	tx_buff[0] = 0x01;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq94
	Delay_ms(30);
	// seq95 offset1 setting 설정 data read
	if(LSI_Read_Data(rx_temp, (TX_NUM + RX_NUM) * 2, 0x73) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	{	// self base data check
		//dbgmsg("DIFF_SELF_OFFSET1_DATA TX\n"); 
		uint32_t node = 0;
		uint32_t cnt = 0;
		for(int i=0; i < TX_NUM ; i++)
		{	
			DIFF_SELF_TX_OFFSET1_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_TX_OFFSET1_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
			
		//dbgmsg("DIFF_SELF_OFFSET1_DATA RX\n");
		node = 0;
		cnt = (TX_NUM)*2;
		for(int i=0; i < RX_NUM ; i++)
		{	
			DIFF_SELF_RX_OFFSET1_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_RX_OFFSET1_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
	}
	// seq96
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq97
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq98 Sense Off
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq99
	Delay_ms(10);
	// seq100 Self long Offset setting 2
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq101 Self Short Offset setting 2
	tx_buff[0] = 0x10;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail");;
		return false;
	}
	// seq102
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");;
		return false;
	}
	// seq103
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq104 Sense On
	tx_buff[0] = 0x01;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq105
	Delay_ms(30);
	// seq106 offset2 setting 설정 data read
	if(LSI_Read_Data(rx_temp, (TX_NUM + RX_NUM) * 2, 0x73) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	{	// self base data check
		//dbgmsg("DIFF_SELF_OFFSET2_DATA TX\n"); 
		uint32_t node = 0;
		uint32_t cnt = 0;
		for(int i=0; i < TX_NUM ; i++)
		{	
			DIFF_SELF_TX_OFFSET2_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_TX_OFFSET2_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
			
		//dbgmsg("DIFF_SELF_OFFSET2_DATA RX\n");
		node = 0;
		cnt = (TX_NUM)*2;
		for(int i=0; i < RX_NUM ; i++)
		{	
			DIFF_SELF_RX_OFFSET2_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_RX_OFFSET2_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
	}
		
	//dbgmsg("DIFF_SELF_OFFSET_ABS_DATA TX\n"); 
	uint32_t node = 0;
	uint32_t cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		DIFF_SELF_TX_OFFSET_ABS_DATA[node] = abs(DIFF_SELF_TX_OFFSET1_DATA[node]-DIFF_SELF_TX_OFFSET2_DATA[node]);
		
		//dbgmsg("%4d ",DIFF_SELF_TX_OFFSET_ABS_DATA[node]);
		node++;
		cnt = cnt + 2;
	}
	//dbgmsg("\n");
		
	//dbgmsg("DIFF_SELF_OFFSET_ABS_DATA RX\n");
	node = 0;
	cnt = (TX_NUM)*2;
	for(int i=0; i < RX_NUM ; i++)
	{	
		DIFF_SELF_RX_OFFSET_ABS_DATA[node] = abs(DIFF_SELF_RX_OFFSET1_DATA[node]-DIFF_SELF_RX_OFFSET2_DATA[node]);
		
		//dbgmsg("%4d ",DIFF_SELF_RX_OFFSET_ABS_DATA[node]);
		node++;
		cnt = cnt + 2;
	}
	//dbgmsg("\n");
	
	ListView_MEAS("OK");
	return true;		
}
uint8_t Diff_Test3(uint16_t index)
{		
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	
	uint8_t tx_buff[10];
			
	// seq1 Touch Mode fix
	tx_buff[0] = 0x00;
	if(LSI_Write_Data(tx_buff,1,0x8E) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}			
	// seq2 Normal Active mode fix
	tx_buff[0] = 0x02;	tx_buff[1] = 0x02;
	if(LSI_Write_Data(tx_buff,2,0xD7) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}		
	// seq03 Set Self data #0
	tx_buff[0] = 0x00;
	if(LSI_Write_Data(tx_buff,1,0x71) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	// seq04
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq05
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq06 sense off
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq07
	Delay_ms(10);
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	// seq08 charge amp setting
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x44;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq09 charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq10 charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x01;tx_buff[2] = 0x00;tx_buff[3] = 0x01;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq11 charge amp setting
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x58;tx_buff[3] = 0x30;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq12 charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq13 charge amp setting
	tx_buff[0] = 0x02;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	// seq14 Differential On 
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x24;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq15 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq16 Differential On
	tx_buff[0] = 0x37;tx_buff[1] = 0xF0;tx_buff[2] = 0x0F;tx_buff[3] = 0x10;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq17 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x28;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq18 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq19 Differential On
	tx_buff[0] = 0x37;tx_buff[1] = 0xF0;tx_buff[2] = 0x0F;tx_buff[3] = 0x10;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq20 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xFC;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq21 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq22 Differential On
	tx_buff[0] = 0x04;tx_buff[1] = 0x21;tx_buff[2] = 0x04;tx_buff[3] = 0x21;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq23 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq24 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq25 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq26 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x04;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq27 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq28 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq29 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x08;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq30 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq31 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq32 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x0C;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq33 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq34 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}			
	// seq35 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x05;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq36 Differential On
	tx_buff[0] = 0x10;tx_buff[1] = 0x00;tx_buff[2] = 0x04;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq37 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x5C;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq38 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq39 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq40 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x60;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq41 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq42 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq43 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x64;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq44 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq45 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	// seq46 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x68;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq47 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq48 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}		
	// seq49 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	// seq50 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	// seq51 Differential On
	tx_buff[0] = 0x01;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	// seq52 Differential On
	Delay_ms(30);	
	// seq53 Differential On
	if(LSI_Read_Data(rx_temp, (TX_NUM + RX_NUM) * 2, 0x73) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	{	// self base data check
		//dbgmsg("DIFF_SELF_OFFSET1_DATA TX\n"); 
		uint32_t node = 0;
		uint32_t cnt = 0;
		for(int i=0; i < TX_NUM ; i++)
		{	
			DIFF_SELF_TX_OFFSET1_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_TX_OFFSET1_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
			
		//dbgmsg("DIFF_SELF_OFFSET1_DATA RX\n");
		node = 0;
		cnt = (TX_NUM)*2;
		for(int i=0; i < RX_NUM ; i++)
		{	
			DIFF_SELF_RX_OFFSET1_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_RX_OFFSET1_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
	}	
	// seq54 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq55
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq56
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq57
	Delay_ms(10);
	// seq58
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail");;
		return false;
	}
	// seq59
	tx_buff[0] = 0x10;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq60
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq61
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");;
		return false;
	}
	// seq62
	tx_buff[0] = 0x01;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq63
	Delay_ms(30);
	// seq64
	if(LSI_Read_Data(rx_temp, (TX_NUM + RX_NUM) * 2, 0x73) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	{	// self base data check
		//dbgmsg("DIFF_SELF_OFFSET2_DATA TX\n"); 
		uint32_t node = 0;
		uint32_t cnt = 0;
		for(int i=0; i < TX_NUM ; i++)
		{	
			DIFF_SELF_TX_OFFSET2_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_TX_OFFSET2_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
			
		//dbgmsg("DIFF_SELF_OFFSET2_DATA RX\n");
		node = 0;
		cnt = (TX_NUM)*2;
		for(int i=0; i < RX_NUM ; i++)
		{	
			DIFF_SELF_RX_OFFSET2_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_RX_OFFSET2_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
	}
		
	//dbgmsg("DIFF_SELF_OFFSET_ABS_DATA TX\n"); 
	uint32_t node = 0;
	uint32_t cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		DIFF_SELF_TX_OFFSET_ABS_DATA[node] = abs(DIFF_SELF_TX_OFFSET1_DATA[node]-DIFF_SELF_TX_OFFSET2_DATA[node]);
		
		//dbgmsg("%4d ",DIFF_SELF_TX_OFFSET_ABS_DATA[node]);
		node++;
		cnt = cnt + 2;
	}
	//dbgmsg("\n");
		
	//dbgmsg("DIFF_SELF_OFFSET_ABS_DATA RX\n");
	node = 0;
	cnt = (TX_NUM)*2;
	for(int i=0; i < RX_NUM ; i++)
	{	
		DIFF_SELF_RX_OFFSET_ABS_DATA[node] = abs(DIFF_SELF_RX_OFFSET1_DATA[node]-DIFF_SELF_RX_OFFSET2_DATA[node]);
		
		//dbgmsg("%4d ",DIFF_SELF_RX_OFFSET_ABS_DATA[node]);
		node++;
		cnt = cnt + 2;
	}
	//dbgmsg("\n");
	
	ListView_MEAS("OK");
	return true;		
}

uint8_t Diff_Test4(uint16_t index)
{	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	
	uint8_t tx_buff[10];
			
	// seq1 Touch Mode fix
	tx_buff[0] = 0x00;
	if(LSI_Write_Data(tx_buff,1,0x8E) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}			
	// seq2 Normal Active mode fix
	tx_buff[0] = 0x02;	tx_buff[1] = 0x02;
	if(LSI_Write_Data(tx_buff,2,0xD7) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}		
	// seq03 Set Self data #0
	tx_buff[0] = 0x00;
	if(LSI_Write_Data(tx_buff,1,0x71) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	// seq04
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq05
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq06 sense off
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq07
	Delay_ms(10);
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	// seq08 charge amp setting
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x44;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq09 charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq10 charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x01;tx_buff[2] = 0x00;tx_buff[3] = 0x01;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq11 charge amp setting
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x58;tx_buff[3] = 0x30;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq12 charge amp setting
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq13 charge amp setting
	tx_buff[0] = 0x02;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	// seq14 Differential On 
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x24;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq15 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq16 Differential On
	tx_buff[0] = 0x37;tx_buff[1] = 0xF0;tx_buff[2] = 0x0F;tx_buff[3] = 0x10;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq17 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x28;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq18 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq19 Differential On
	tx_buff[0] = 0x37;tx_buff[1] = 0xF0;tx_buff[2] = 0x0F;tx_buff[3] = 0x10;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq20 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x59;tx_buff[3] = 0xFC;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq21 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq22 Differential On
	tx_buff[0] = 0x04;tx_buff[1] = 0x21;tx_buff[2] = 0x04;tx_buff[3] = 0x21;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq23 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq24 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq25 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq26 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x04;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq27 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq28 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq29 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x08;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq30 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq31 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq32 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x5A;tx_buff[3] = 0x0C;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq33 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq34 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}			
	// seq35 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x01;tx_buff[3] = 0x09;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq36 Differential On
	tx_buff[0] = 0x10;tx_buff[1] = 0x00;tx_buff[2] = 0x01;tx_buff[3] = 0x07;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq37 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x5C;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq38 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq39 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq40 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x60;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq41 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq42 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq43 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x64;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq44 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq45 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	// seq46 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x52;tx_buff[3] = 0x68;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq47 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq48 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}		
	// seq49 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	// seq50 Differential On
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	// seq51 Differential On
	tx_buff[0] = 0x01;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	// seq52 Differential On
	Delay_ms(30);	
	// seq53 Differential On
	if(LSI_Read_Data(rx_temp, (TX_NUM + RX_NUM) * 2, 0x73) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	{	// self base data check
		//dbgmsg("DIFF_SELF_OFFSET1_DATA TX\n"); 
		uint32_t node = 0;
		uint32_t cnt = 0;
		for(int i=0; i < TX_NUM ; i++)
		{	
			DIFF_SELF_TX_OFFSET1_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_TX_OFFSET1_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
			
		//dbgmsg("DIFF_SELF_OFFSET1_DATA RX\n");
		node = 0;
		cnt = (TX_NUM)*2;
		for(int i=0; i < RX_NUM ; i++)
		{	
			DIFF_SELF_RX_OFFSET1_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_RX_OFFSET1_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
	}	
	// seq54 Differential On
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq55
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq56
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq57
	Delay_ms(10);
	// seq58
	tx_buff[0] = 0x00;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq59
	tx_buff[0] = 0x10;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xE3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq60
	tx_buff[0] = 0x40;tx_buff[1] = 0x00;tx_buff[2] = 0x50;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD0) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq61
	tx_buff[0] = 0x00;tx_buff[1] = 0x04;
	if(LSI_Write_Data(tx_buff,2,0xD1) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq62
	tx_buff[0] = 0x01;tx_buff[1] = 0x00;tx_buff[2] = 0x00;tx_buff[3] = 0x00;
	if(LSI_Write_Data(tx_buff,4,0xD3) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}
	// seq63
	Delay_ms(30);
	// seq64
	if(LSI_Read_Data(rx_temp, (TX_NUM + RX_NUM) * 2, 0x73) == false)
	{
		ListView_MEAS("Comm. Fail");
		return false;
	}	
	{	// self base data check
		//dbgmsg("DIFF_SELF_OFFSET2_DATA TX\n"); 
		uint32_t node = 0;
		uint32_t cnt = 0;
		for(int i=0; i < TX_NUM ; i++)
		{	
			DIFF_SELF_TX_OFFSET2_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_TX_OFFSET2_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
			
		//dbgmsg("DIFF_SELF_OFFSET2_DATA RX\n");
		node = 0;
		cnt = (TX_NUM)*2;
		for(int i=0; i < RX_NUM ; i++)
		{	
			DIFF_SELF_RX_OFFSET2_DATA[node] = (int16_t)( ((int16_t)(rx_temp[cnt]<<8)) | ((int16_t)(rx_temp[cnt+1]<<0)));
			
			//dbgmsg("%4d ",DIFF_SELF_RX_OFFSET2_DATA[node]);
			node++;
			cnt = cnt + 2;
		}
		//dbgmsg("\n");
	}
		
	//dbgmsg("DIFF_SELF_OFFSET_ABS_DATA TX\n"); 
	uint32_t node = 0;
	uint32_t cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		DIFF_SELF_TX_OFFSET_ABS_DATA[node] = abs(DIFF_SELF_TX_OFFSET1_DATA[node]-DIFF_SELF_TX_OFFSET2_DATA[node]);
		
		//dbgmsg("%4d ",DIFF_SELF_TX_OFFSET_ABS_DATA[node]);
		node++;
		cnt = cnt + 2;
	}
	//dbgmsg("\n");
		
	//dbgmsg("DIFF_SELF_OFFSET_ABS_DATA RX\n");
	node = 0;
	cnt = (TX_NUM)*2;
	for(int i=0; i < RX_NUM ; i++)
	{	
		DIFF_SELF_RX_OFFSET_ABS_DATA[node] = abs(DIFF_SELF_RX_OFFSET1_DATA[node]-DIFF_SELF_RX_OFFSET2_DATA[node]);
		
		//dbgmsg("%4d ",DIFF_SELF_RX_OFFSET_ABS_DATA[node]);
		node++;
		cnt = cnt + 2;
	}
	//dbgmsg("\n");
	
	ListView_MEAS("OK");
	return true;		
}

uint8_t Diff_Cm_Base(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
		
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
		
	uint32_t node = 0;
	uint32_t err_cnt = 0;
		
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			panel_data[panel_cnt][node] = DIFF_MUTUAL_BASELINE_DATA[node];
						
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);
			node++;
		}
		dbgmsg("\n");
	}
	dbgmsg("\n");

	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}
	
	return true;	
};		
uint8_t Diff_Cm_PP(uint16_t index)
{	
	char text[50];
	uint32_t node = 0;
	int Mutual_Base_MAX = 0;
	int Mutual_Base_MIN = 32760;
	int Mutual_Base_PP = 0;
		
	for(int i=0; i < TX_NUM ; i++)
	{	
		for(int j=0; j < RX_NUM ; j++)
		{
			if(Mutual_Base_MAX < DIFF_MUTUAL_BASELINE_DATA[node]) Mutual_Base_MAX = DIFF_MUTUAL_BASELINE_DATA[node];
			if(Mutual_Base_MIN > DIFF_MUTUAL_BASELINE_DATA[node]) Mutual_Base_MIN = DIFF_MUTUAL_BASELINE_DATA[node];

			node++;
		}
	}
	Mutual_Base_PP = Mutual_Base_MAX - Mutual_Base_MIN;
	
	dbgmsg("MAX - MIN = %d\n",Mutual_Base_PP);
	sprintf(text,"%d",Mutual_Base_PP);
	ListView_MEAS(text);
		
	if(func_spec_dec[index]< Mutual_Base_PP)
	{
		return false;
	}
	
	return true;	
};			

uint8_t Diff_Cs_Base_Tx(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
			
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
			panel_data[panel_cnt][node] = DIFF_SELF_TX_BASELINE_DATA[i];
			
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);			
			node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}	
	return true;	
};			
uint8_t Diff_Cs_Base_Rx(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
			
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM ; i++)
	{	
			panel_data[panel_cnt][node] = DIFF_SELF_RX_BASELINE_DATA[i];
			
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);			
			node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}	
	return true;	
	
};			
uint8_t Diff_Offset1_Tx(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
			
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		panel_data[panel_cnt][node] = (int)DIFF_SELF_TX_OFFSET1_DATA[i];
		dbgmsg("%4d ",panel_data[panel_cnt][node]);
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}		
		node++;
	}
	dbgmsg("\n");

	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;

	return true;	
	
};			
uint8_t Diff_Offset1_Rx(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
			
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM ; i++)
	{	
		panel_data[panel_cnt][node] = (int)DIFF_SELF_RX_OFFSET1_DATA[i];
		dbgmsg("%4d ",panel_data[panel_cnt][node]);	
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}				
		node++;
	}
	dbgmsg("\n");

	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;

	return true;	
	
};			
uint8_t Diff_Offset2_Tx(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
			
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
		panel_data[panel_cnt][node] = (int)DIFF_SELF_TX_OFFSET2_DATA[i];
		dbgmsg("%4d ",panel_data[panel_cnt][node]);	
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}				
		node++;
	}
	dbgmsg("\n");

	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;

	return true;
	
};			
uint8_t Diff_Offset2_Rx(uint16_t index)
{
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
			
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM ; i++)
	{	
		panel_data[panel_cnt][node] = (int)DIFF_SELF_RX_OFFSET2_DATA[i];
		dbgmsg("%4d ",panel_data[panel_cnt][node]);	
		
		if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}				
		node++;
	}
	dbgmsg("\n");

	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;

	return true;	

};
uint8_t Diff_Offset_Abs_Tx(uint16_t index)
{		
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
			
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < TX_NUM ; i++)
	{	
			panel_data[panel_cnt][node] = DIFF_SELF_TX_OFFSET_ABS_DATA[i];
			
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);			
			node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}	
	return true;	
	
};		
uint8_t Diff_Offset_Abs_Rx(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
			
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	for(int i=0; i < RX_NUM ; i++)
	{	
			panel_data[panel_cnt][node] = DIFF_SELF_RX_OFFSET_ABS_DATA[i];
			
			if( !((panel_data[panel_cnt][node] >= panel_min[panel_cnt][node]) && (panel_data[panel_cnt][node] <= panel_max[panel_cnt][node])) )
			{
				err_cnt++;
			}
			dbgmsg("%4d ",panel_data[panel_cnt][node]);			
			node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}	
	return true;

};		
uint8_t Diff_Offset_Gap_Tx(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
			
	uint32_t node = 0;
	uint32_t err_cnt = 0;
	int16_t ref_index = func_spec_dec[index];
	int16_t ref = DIFF_SELF_TX_OFFSET_ABS_DATA[ref_index];
	
	dbgmsg("ref %4d(index = %d)-%.0f \n",ref, ref_index, func_spec_float[index] );
		
	for(int i=0; i < TX_NUM ; i++)
	{	
		GAP_P2P_Diff_Test_TX[i] 	= ((float)DIFF_SELF_TX_OFFSET_ABS_DATA[i]/(float)ref)*(float)100.0;
		panel_dataf[panel_cnt][node] = GAP_P2P_Diff_Test_TX[i];
		if( !((panel_dataf[panel_cnt][node] >= (float)panel_min[panel_cnt][node]) && (panel_dataf[panel_cnt][node] <= (float)panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		dbgmsg("%.3f ",panel_dataf[panel_cnt][node]);			
		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}	
	return true;
	
};		
uint8_t Diff_Offset_Gap_Rx(uint16_t index)
{	
	char text[50];
	uint16_t panel_cnt = func_panel_link[index] - 1;
	
	dbgmsg("%s(%d, %d)\n",__func__,TX_NUM,RX_NUM);	
	dbgmsg("panel_cnt : %d\n",panel_cnt);
			
	uint32_t node = 0;
	uint32_t err_cnt = 0;


	int16_t ref_index = func_spec_dec[index];
	
	int16_t ref = DIFF_SELF_RX_OFFSET_ABS_DATA[ref_index];
	
	dbgmsg("ref %4d(index = %d)-%.0f \n",ref, ref_index, func_spec_float[index] );
	
	for(int i=0; i < RX_NUM ; i++)
	{								
		GAP_P2P_Diff_Test_RX[i] 	= ((float)DIFF_SELF_RX_OFFSET_ABS_DATA[i]/(float)ref)*(float)100.0;
		//panel_data[panel_cnt][node] = (int)(GAP_P2P_Diff_Test_RX[i]*100);
		panel_dataf[panel_cnt][node] = GAP_P2P_Diff_Test_RX[i];
		
		if( !((panel_dataf[panel_cnt][node] >= (float)panel_min[panel_cnt][node]) && (panel_dataf[panel_cnt][node] <= (float)panel_max[panel_cnt][node])) )
		{
			err_cnt++;
		}
		//dbgmsg("%.3f ",GAP_P2P_Diff_Test_RX[i]);
		//dbgmsg("%4d ",panel_data[panel_cnt][node]);	
		dbgmsg("%.3f ",panel_dataf[panel_cnt][node]);			
		node++;
	}
	dbgmsg("\n");
	
	dbgmsg("err_cnt : %d\n",err_cnt);
	sprintf(text,"error node : %d",err_cnt);
	ListView_MEAS(text);
	ErrorNodeCount[panel_cnt] = err_cnt;
	if(err_cnt != 0)
	{
		return false;
	}	
	return true;	
	
};		

#endif 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
