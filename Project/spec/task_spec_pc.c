#include "task_spec_pc.h"

static char buff[50];

uint8_t Spec_Make_PC(char *data)
{
	FLASH_Unlock();
	
	char* ptr_year = strtok(data,",");
	char* ptr_mon = strtok(NULL,",");
	char* ptr_day = strtok(NULL,",");
	char* ptr_hour = strtok(NULL,",");
	char* ptr_min = strtok(NULL,",");
		
	FLASH_ProgramWord(SPEC_SAVE_YEAR_ADD, 	atoi(ptr_year) & 0xff );	// SPEC file down year 
	FLASH_ProgramWord(SPEC_SAVE_MONTH_ADD, 	atoi(ptr_mon)  & 0xff );	// SPEC file down month
	FLASH_ProgramWord(SPEC_SAVE_DAY_ADD, 	atoi(ptr_day)  & 0xff );	// SPEC file down date
	FLASH_ProgramWord(SPEC_SAVE_HOUR_ADD, 	atoi(ptr_hour) & 0xff); 	// SPEC file down year 
	FLASH_ProgramWord(SPEC_SAVE_MIN_ADD, 	atoi(ptr_min)  & 0xff);		// SPEC file down month
	FLASH_ProgramWord(SPEC_SAVE_FLAG_ADD, SPEC_CHECK_FLAG );	// SPEC file down date
		
	FLASH_Lock();
	
	dbgmsg("20%02d-%02d-%02d, %02d:%02d[%X] ICT Spec Save OK!\n",SPEC_SAVE_YEAR,SPEC_SAVE_MONTH,SPEC_SAVE_DAY,SPEC_SAVE_HOUR,SPEC_SAVE_MIN,SPEC_SAVE_FLAG);

	UpdateIctSpecSdram();
	UpdateGui();
		
	buff[0] = true;	tx_pc_data(ICT_SPEC_FINSH, 1, buff);	
	return true;
}
uint8_t ictspec_erase_pc(void)
{
	dbgmsg("spec_erase : FLASH_Sector_20\n");
	FLASH_Unlock();
	FLASH_EraseSector(FLASH_Sector_20, VoltageRange_3);
	FLASH_Lock();	
	
	buff[0] = true;	tx_pc_data(ICT_SPEC_ERASE, 1, buff);
	
	return true;
}
uint8_t Info_Save_PC(char *data)
{			
	FLASH_Unlock();			
							
	char* ptr_name_main = strtok(data,",");
	char* ptr_name_sub = strtok(NULL,",");
	char* ptr_num1 =  strtok(NULL,",");
	char* ptr_num2 =  strtok(NULL,",");
	
	uint32_t pj_main_name_add = PJ_MAIN_NAME_ADD;
	uint32_t pj_main_name_cnt = strlen(ptr_name_main);
	uint32_t pj_main_name_max = 180;							
	for(int i = 0; i < pj_main_name_max; i++)
	{
		if( i == pj_main_name_max -1 )
		{
			FLASH_ProgramByte( pj_main_name_add + i,  NULL );
		}
		else if( i < pj_main_name_cnt )
		{
			if( (ptr_name_main[i] != '\n') && (ptr_name_main[i] != '\r') )	{	FLASH_ProgramByte( pj_main_name_add + i,  ptr_name_main[i] );  		}
			else															{	FLASH_ProgramByte( pj_main_name_add + i,  NULL );  			}
		}
		else
		{
			FLASH_ProgramByte( pj_main_name_add + i,  NULL );
		}
	}	
				
	uint32_t pj_sub_name_add = PJ_SUB_NAME_ADD;
	uint32_t pj_sub_name_cnt = strlen(ptr_name_sub);
	uint32_t pj_sub_name_max = 180;							
	for(int i = 0; i < pj_sub_name_max; i++)
	{
		if( i == pj_sub_name_max -1 )
		{
			FLASH_ProgramByte( pj_sub_name_add + i,  NULL );
		}
		else if( i < pj_sub_name_cnt )
		{
			if( (ptr_name_sub[i] != '\n') && (ptr_name_sub[i] != '\r') )	{	FLASH_ProgramByte( pj_sub_name_add + i,  ptr_name_sub[i] );  	}
			else															{	FLASH_ProgramByte( pj_sub_name_add + i,  NULL );  		}
		}
		else
		{
			FLASH_ProgramByte( pj_sub_name_add + i,  NULL );
		}
	}	

	FLASH_ProgramHalfWord( SH_AUTO_P_ADD,  atoi(ptr_num1));	
	FLASH_ProgramHalfWord( SH_AUTO_N_ADD,  atoi(ptr_num2));	
	
	FLASH_Lock();					
	
	sprintf(project_name_main, "%s", (char *)(PJ_MAIN_NAME_ADD) );	
	sprintf(project_name_sub,  "%s", (char *)(PJ_SUB_NAME_ADD)  );		
	//dbgmsg("- Promain Name : %s\n",project_name_main);
	//dbgmsg("- ProSub  Name : %s\n",project_name_sub);
	
	short_auto_pp 	 =  (*( (uint16_t *)( SH_AUTO_P_ADD ) ));
	short_auto_mm 	 =  (*( (uint16_t *)( SH_AUTO_N_ADD ) ));
	//dbgmsg("- Short Auto   : %03d-%03d\n",short_auto_pp,short_auto_mm);

	buff[0] = true; tx_pc_data(ICT_PROJECT_WR, 1, buff);	
	
	return true;
}

uint8_t Tp_Save_PC(char *data)
{		
	FLASH_Unlock();

	char* ptr_count = strtok(data,",");
	for( int cnt = 0; cnt <  atoi(ptr_count); cnt++ )
	{
		char* ptr_name   = strtok(NULL,","); 
		char* ptr_number = strtok(NULL,","); 
		
		// tp name 
		uint32_t tp_name_add = TEST_POINT_NAME_ADD + (cnt * 32);
		uint32_t tp_name_cnt = strlen(ptr_name);
		uint32_t tp_name_max = 32;
		
		for(int i = 0; i < tp_name_max; i++)
		{
			if( i == tp_name_max -1 )
			{
				FLASH_ProgramByte( tp_name_add + i,  NULL );
			}
			else if( i < tp_name_cnt )
			{
				if( (ptr_name[i] != '\n') && (ptr_name[i] != '\r') )	{		FLASH_ProgramByte( tp_name_add + i,  ptr_name[i] );  		}
				else													{		FLASH_ProgramByte( tp_name_add + i,  NULL );  				}
			}
			else
			{
				FLASH_ProgramByte( tp_name_add + i,  NULL );
			}
		}
		// tp number
		uint32_t tp_number_add = TEST_POINT_NUMBER_ADD + (cnt * 2);
		FLASH_ProgramHalfWord( tp_number_add,  atoi(ptr_number));				
	}
	
	if(atoi(ptr_count) <= 720)	FLASH_ProgramHalfWord(TEST_POINT_TOTAL_ADD, atoi(ptr_count));
	else						FLASH_ProgramHalfWord(TEST_POINT_TOTAL_ADD, 720);

	FLASH_Lock();

	tp_count =  (*( (uint16_t *)TEST_POINT_TOTAL_ADD));
	if(tp_count > 720) tp_count = 0;
	//dbgmsg(" - tp_count       : %4d\n", tp_count);
	for( int cnt = 0; cnt < tp_count; cnt++ )
	{
		sprintf(tp_name[cnt], "%s", (char *)(TEST_POINT_NAME_ADD + (cnt * 32)));	
		tp_number[cnt] = (*( (uint16_t *)( TEST_POINT_NUMBER_ADD + cnt * 2 ) ));
		//dbgmsg("- %s, %d\n",tp_name[cnt],tp_number[cnt]);
	}
		
	buff[0] = true; tx_pc_data(ICT_TESTPOINT_WR, 1, buff);	
	
	return true;
}

uint8_t Open_Save_PC(char *data)
{	
	FLASH_Unlock();
	
	char* ptr_count = strtok(data,",");
	for( int cnt = 0; cnt <  atoi(ptr_count); cnt++ )
	{
		char* ptr_tp1  = strtok(NULL,","); 
		char* ptr_tp2  = strtok(NULL,","); 
		char* ptr_rg   = strtok(NULL,","); 
		char* ptr_spec = strtok(NULL,","); 
		char* ptr_ok   = strtok(NULL,","); 
		char* ptr_ng   = strtok(NULL,","); 
				
		// open tp1
		uint32_t open_tp1_add = OPEN_TESTPOINT1_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( open_tp1_add,  atoi(ptr_tp1));
		
		// open tp2
		uint32_t open_tp2_add = OPEN_TESTPOINT2_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( open_tp2_add,  atoi(ptr_tp2));
		
		// open range
		uint32_t open_range_add = OPEN_TEST_RANGE_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( open_range_add,  atoi(ptr_rg));
		
		// open spec
		uint32_t open_spec_add = OPEN_TEST_SPEC_ADD + (cnt) * 4;		
		float spec_temp = atof(ptr_spec);
		FLASH_ProgramWord( open_spec_add,  *(uint32_t *)&(spec_temp) );
		
		// open ok
		uint32_t open_ok_add = OPEN_TEST_OK_COUNT_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( open_ok_add,  atoi(ptr_ok));
		
		// open ng
		uint32_t open_ng_add = OPEN_TEST_NG_COUNT_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( open_ng_add,  atoi(ptr_ng));				
	}
	if(atoi(ptr_count) <= 720)	FLASH_ProgramHalfWord(OPEN_TOTAL_ADD, atoi(ptr_count));
	else						FLASH_ProgramHalfWord(OPEN_TOTAL_ADD, 720);

	FLASH_Lock();
	
	open_count = (*( (uint16_t *)OPEN_TOTAL_ADD));
	if(open_count > 720) open_count = 0;	
	//dbgmsg(" - open_count     : %4d\n", open_count);	
	for(int cnt=0; cnt< open_count; cnt++)
	{
		open_tp1[cnt]		=	(*( (uint16_t *)( OPEN_TESTPOINT1_ADD    + cnt * 2 ) ));
		open_tp2[cnt] 		=	(*( (uint16_t *)( OPEN_TESTPOINT2_ADD    + cnt * 2 ) ));
		open_range[cnt] 	=	(*( (uint16_t *)( OPEN_TEST_RANGE_ADD    + cnt * 2 ) ));
		open_ok_cnt[cnt] 	=	(*( (uint16_t *)( OPEN_TEST_OK_COUNT_ADD + cnt * 2 ) ));
		open_ng_cnt[cnt] 	=	(*( (uint16_t *)( OPEN_TEST_NG_COUNT_ADD + cnt * 2 ) ));
		open_spec[cnt] 		=	(*(float *)&(*( (uint32_t *)( OPEN_TEST_SPEC_ADD + cnt * 4 ) )));
		//dbgmsg("- %d-%d, %d ,%d, %d, %.1f\n",open_tp1[cnt],open_tp2[cnt],open_range[cnt],open_ok_cnt[cnt],open_ng_cnt[cnt],open_spec[cnt]);
	}
		
	buff[0] = true;	tx_pc_data(ICT_OPEN_WR, 1, buff);	
	
	return true;
}


uint8_t Short_bs_Save_PC(char *data)
{	
	FLASH_Unlock();
		
	char* ptr_count = strtok(data,",");
	for( int cnt = 0; cnt <  atoi(ptr_count); cnt++ )
	{	
		if(cnt == 0) 	//first line(tp,gr,range,spec,okcnt,ngcnt)
		{
			char* ptr_tp   = strtok(NULL,","); 
			char* ptr_gr   = strtok(NULL,","); 
			char* ptr_rg   = strtok(NULL,","); 
			char* ptr_spec = strtok(NULL,","); 
			char* ptr_ok   = strtok(NULL,","); 
			char* ptr_ng   = strtok(NULL,","); 

			// short tp
			uint32_t short_tp_add = SHORT_TEST_POINT_ADD + (cnt) * 2;
			FLASH_ProgramHalfWord( short_tp_add,  atoi(ptr_tp));					
			// short group
			uint32_t short_group_add = SHORT_TEST_GROUP_ADD + (cnt) * 2;
			FLASH_ProgramHalfWord( short_group_add,  atoi(ptr_gr));					
			// short range
			uint32_t short_range_add = SHORT_TEST_RANGE_ADD;
			FLASH_ProgramHalfWord( short_range_add,  atoi(ptr_rg));					
			// short spec
			uint32_t short_spec_add = SHORT_TEST_SPEC_ADD;			
			float spec_temp = atof(ptr_spec);
			FLASH_ProgramWord( short_spec_add,  *(uint32_t *)&(spec_temp) );					
			// short ok
			uint32_t short_ok_add = SHORT_TEST_OK_COUNT_ADD;
			FLASH_ProgramHalfWord( short_ok_add,  atoi(ptr_ok));					
			// short ng
			uint32_t short_ng_add = SHORT_TEST_NG_COUNT_ADD;
			FLASH_ProgramHalfWord( short_ng_add,  atoi(ptr_ng));
		}
		else//other line(tp,gr)
		{
			char* ptr_tp  = strtok(NULL,","); 
			char* ptr_gr   = strtok(NULL,","); 
			// short tp
			uint32_t short_tp_add = SHORT_TEST_POINT_ADD + (cnt) * 2;
			FLASH_ProgramHalfWord( short_tp_add,  atoi(ptr_tp));					
			// short group
			uint32_t short_group_add = SHORT_TEST_GROUP_ADD + (cnt) * 2;
			FLASH_ProgramHalfWord( short_group_add,  atoi(ptr_gr));
		}
	}			
	if(atoi(ptr_count) <= 720)	FLASH_ProgramHalfWord(SHORT_TOTAL_ADD, atoi(ptr_count));
	else						FLASH_ProgramHalfWord(SHORT_TOTAL_ADD, 720);
	
	FLASH_Lock();
	
	short_bs_count  =	(*( (uint16_t *)SHORT_TOTAL_ADD));		
	if(short_bs_count > 720) short_bs_count = 0;
	//dbgmsg(" - short_count    : %4d\n", short_bs_count);
	short_bs_range  =	(*( (uint16_t *)SHORT_TEST_RANGE_ADD));
	short_bs_ok_cnt =	(*( (uint16_t *)SHORT_TEST_OK_COUNT_ADD	));
	short_bs_ng_cnt =	(*( (uint16_t *)SHORT_TEST_NG_COUNT_ADD	)); 
	short_bs_spec   =	(*(float *) & (*( (uint32_t *)SHORT_TEST_SPEC_ADD		)));
	for(int cnt=0; cnt< short_bs_count; cnt++)
	{
		short_bs_tp[cnt]	=	(*( (uint16_t *)( SHORT_TEST_POINT_ADD + cnt * 2 ) ));
		short_bs_group[cnt]	=	(*( (uint16_t *)( SHORT_TEST_GROUP_ADD + cnt * 2 ) ));
		//dbgmsg("- %d-%d, %d, %d, %d, %.1f\n",short_bs_tp[cnt],short_bs_group[cnt],short_bs_range,short_bs_ok_cnt,short_bs_ng_cnt,short_bs_spec);
	}		

	buff[0] = true;	tx_pc_data(ICT_SHORTBS_WR, 1, buff);	
	return true;
}
uint8_t Short_ex_save_PC(char *data)
{	
	FLASH_Unlock();

	char* ptr_count = strtok(data,",");
	for( int cnt = 0; cnt <  atoi(ptr_count); cnt++ )
	{	
			if(cnt > 720) break;
			char* ptr_tp1  = strtok(NULL,","); 
			char* ptr_tp2  = strtok(NULL,","); 
			char* ptr_rg   = strtok(NULL,","); 
			char* ptr_spec = strtok(NULL,","); 
			char* ptr_ok   = strtok(NULL,","); 
			char* ptr_ng   = strtok(NULL,","); 
				
			// short ex tp1
			uint32_t shex_tp1_add = SHEX_TESTPOINT1_ADD + (cnt) * 2;
			FLASH_ProgramHalfWord( shex_tp1_add,  atoi(ptr_tp1));					
			// short ex tp2
			uint32_t shex_tp2_add = SHEX_TESTPOINT2_ADD + (cnt) * 2;
			FLASH_ProgramHalfWord( shex_tp2_add,  atoi(ptr_tp2));					
			// short ex range
			uint32_t shex_range_add = SHEX_TEST_RANGE_ADD + (cnt) * 2;
			FLASH_ProgramHalfWord( shex_range_add,  atoi(ptr_rg));					
			// short ex spec
			uint32_t shex_spec_add = SHEX_TEST_SPEC_ADD + (cnt) * 4;		
			float spec_temp = atof(ptr_spec);
			FLASH_ProgramWord( shex_spec_add,  *(uint32_t *)&(spec_temp) );					
			// short ex ok
			uint32_t shex_ok_add = SHEX_TEST_OK_COUNT_ADD + (cnt) * 2;
			FLASH_ProgramHalfWord( shex_ok_add,  atoi(ptr_ok));					
			// short ex ng
			uint32_t shex_ng_add = SHEX_TEST_NG_COUNT_ADD + (cnt) * 2;
			FLASH_ProgramHalfWord( shex_ng_add,  atoi(ptr_ng));
	}			
	if(atoi(ptr_count) <= 720)	FLASH_ProgramHalfWord(SHEX_TOTAL_ADD, atoi(ptr_count));
	else						FLASH_ProgramHalfWord(SHEX_TOTAL_ADD, 720);			
	
	FLASH_Lock();
	
	short_ex_count = (*( (uint16_t *)SHEX_TOTAL_ADD));		
	if(short_ex_count > 720) short_ex_count = 0;
	//dbgmsg(" - short_ex_count : %4d\n", short_ex_count);
	for(int cnt=0; cnt< short_ex_count; cnt++)
	{
		short_ex_tp1[cnt]	=	(*( (uint16_t *)( SHEX_TESTPOINT1_ADD    + cnt * 2 ) ));
		short_ex_tp2[cnt]	=	(*( (uint16_t *)( SHEX_TESTPOINT2_ADD    + cnt * 2 ) ));
		short_ex_range[cnt]	=	(*( (uint16_t *)( SHEX_TEST_RANGE_ADD    + cnt * 2 ) ));
		short_ex_ok_cnt[cnt]=	(*( (uint16_t *)( SHEX_TEST_OK_COUNT_ADD + cnt * 2 ) ));
		short_ex_ng_cnt[cnt]=	(*( (uint16_t *)( SHEX_TEST_NG_COUNT_ADD + cnt * 2 ) ));
		short_ex_spec[cnt]	= (*(float *)&(*( (uint32_t *)( SHEX_TEST_SPEC_ADD + cnt * 4 ) )));
		//dbgmsg("- %d-%d, %d, %d, %d, %.1f\n",short_ex_tp1[cnt],short_ex_tp2[cnt],short_ex_range[cnt],short_ex_ok_cnt[cnt],short_ex_ng_cnt[cnt],short_ex_spec[cnt]);
	}
		
	buff[0] = true;	tx_pc_data(ICT_SHORTEX_WR, 1, buff);	
	return true;
}
uint8_t Part_Save_PC(char *data)
{	
	FLASH_Unlock();
	
	char* ptr_count = strtok(data,",");
	for( int cnt = 0; cnt <  atoi(ptr_count); cnt++ )
	{	
		char* ptr_name = strtok(NULL,",");
		char* ptr_tp1  = strtok(NULL,",");
		char* ptr_tp2  = strtok(NULL,",");
		char* ptr_rg   = strtok(NULL,",");
		char* ptr_hi   = strtok(NULL,",");
		char* ptr_lo   = strtok(NULL,",");
		char* ptr_ok   = strtok(NULL,",");
		char* ptr_ng   = strtok(NULL,",");
		//dbgmsg("[%s]%s, %s, %s, %s, %s, %s, %s, %s\n", __func__, ptr_name, ptr_tp1, ptr_tp2, ptr_rg, ptr_hi, ptr_lo,ptr_ok,ptr_ng);
		
		// part name
		uint32_t part_name_add = PART_TEST_NAME_ADD + (cnt) * 32;
		uint32_t part_name_cnt = strlen(ptr_name);
		uint32_t part_name_max = 32;
								
		for(int i = 0; i < part_name_max; i++)
		{
			if( i == part_name_max -1 )
			{
				FLASH_ProgramByte( part_name_add + i,  NULL );
			}
			else if( i < part_name_cnt )
			{
				if( (ptr_name[i] != '\n') && (ptr_name[i] != '\r') ) 	{		FLASH_ProgramByte( part_name_add + i,  ptr_name[i] );  			}
				else													{		FLASH_ProgramByte( part_name_add + i,  NULL );  				}
			}
			else
			{
				FLASH_ProgramByte( part_name_add + i,  NULL );
			}
		}
		// part tp1
		uint32_t part_tp1_add = PART_TESTPOINT1_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( part_tp1_add,  atoi(ptr_tp1));
		// part tp2
		uint32_t part_tp2_add = PART_TESTPOINT2_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( part_tp2_add,  atoi(ptr_tp2));
		// part range
		uint32_t part_range_add = PART_TEST_RANGE_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( part_range_add,  atoi(ptr_rg));
		// part upper
		uint32_t part_upper_add = PART_TEST_UPPER_ADD + (cnt) * 4;		
		float up_temp = atof(ptr_hi);
		FLASH_ProgramWord( part_upper_add,  *(uint32_t *)&(up_temp) );
		// part lower
		uint32_t part_lower_add = PART_TEST_LOWER_ADD + (cnt) * 4;
		float low_temp = atof(ptr_lo);
		FLASH_ProgramWord( part_lower_add,  *(uint32_t *)&(low_temp) );
		// part ok
		uint32_t part_ok_add = PART_TEST_OK_COUNT_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( part_ok_add,  atoi(ptr_ok));
		// part ng
		uint32_t part_ng_add = PART_TEST_NG_COUNT_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( part_ng_add,  atoi(ptr_ng));
	}
	if(atoi(ptr_count) <= 720)	FLASH_ProgramHalfWord(PART_TOTAL_ADD, atoi(ptr_count));
	else						FLASH_ProgramHalfWord(PART_TOTAL_ADD, 720);

	FLASH_Lock();

	part_count = (*( (uint16_t *)PART_TOTAL_ADD));
	if(part_count > 720) part_count = 0;
	dbgmsg(" - part_count     : %4d\n", part_count); 
	for(int cnt=0; cnt< part_count; cnt++)
	{
		sprintf(part_name[cnt], "%s", (char *)(PART_TEST_NAME_ADD + (cnt * 32)));
		part_tp1[cnt]		= 			  (*( (uint16_t *)( PART_TESTPOINT1_ADD    + cnt * 2 ) ));
		part_tp2[cnt] 		= 			  (*( (uint16_t *)( PART_TESTPOINT2_ADD    + cnt * 2 ) ));
		part_range[cnt] 	= 			  (*( (uint16_t *)( PART_TEST_RANGE_ADD    + cnt * 2 ) ));
		part_ok_cnt[cnt] 	= 			  (*( (uint16_t *)( PART_TEST_OK_COUNT_ADD + cnt * 2 ) ));
		part_ng_cnt[cnt] 	= 			  (*( (uint16_t *)( PART_TEST_NG_COUNT_ADD + cnt * 2 ) ));
		part_spec_hi[cnt]	= (*(float *)&(*( (uint32_t *)( PART_TEST_UPPER_ADD    + cnt * 4 ) )));
		part_spec_lo[cnt]	= (*(float *)&(*( (uint32_t *)( PART_TEST_LOWER_ADD    + cnt * 4 ) )));
		//dbgmsg("- %s, %d-%d, %d, %d, %d, %.1f, %.1f\n",part_name[cnt],part_tp1[cnt],part_tp2[cnt],part_range[cnt],part_ok_cnt[cnt],part_ng_cnt[cnt],part_spec_hi[cnt],part_spec_lo[cnt]);
	}
		
	buff[0] = true;	tx_pc_data(ICT_PART_WR, 1, buff);	
	return true;
}
uint8_t Func_Save_PC(char *data)
{	
	FLASH_Unlock();
	
	char* ptr_count = strtok(data,",");
	for( int cnt = 0; cnt <  atoi(ptr_count); cnt++ )
	{			
		char* ptr_name  		= strtok(NULL,","); 
		char* ptr_type  		= strtok(NULL,","); 
		char* ptr_mode  		= strtok(NULL,","); 
		char* ptr_log   		= strtok(NULL,","); 
		char* ptr_panel_link 	= strtok(NULL,","); 
		char* ptr_spec_dec   	= strtok(NULL,","); 
		char* ptr_spec_hex1   	= strtok(NULL,","); 
		char* ptr_spec_hex2   	= strtok(NULL,","); 
		char* ptr_spec_hex3   	= strtok(NULL,","); 
		char* ptr_spec_hex4   	= strtok(NULL,","); 
		char* ptr_spec_float 	= strtok(NULL,","); 
		char* ptr_spec_ascii 	= strtok(NULL,",");
		
		//dbgmsg("[%s]%s, %s, %s, %s, %s, %s, %s.%s.%s.%s, %s, %s\n", __func__, ptr_name, ptr_type, ptr_mode, ptr_log, ptr_panel_link, ptr_spec_dec, ptr_spec_hex1,ptr_spec_hex2,ptr_spec_hex3,ptr_spec_hex4,ptr_spec_float, ptr_spec_ascii);
		
		/////////////// func name	/////////////
		uint32_t func_name_add = FUNC_TEST_NAME_ADD + (cnt) * 32;
		uint32_t func_name_cnt = strlen(ptr_name);
		uint32_t func_name_max = 32;
								
		for(int i = 0; i < func_name_max; i++)
		{
			if( i == func_name_max -1 )
			{
				FLASH_ProgramByte( func_name_add + i,  NULL );
			}
			else if( i < func_name_cnt )
			{
				if( (ptr_name[i] != '\n') && (ptr_name[i] != '\r') )	{	FLASH_ProgramByte( func_name_add + i,  ptr_name[i] );  	}
				else													{	FLASH_ProgramByte( func_name_add + i,  NULL );  		}
			}
			else
			{
				FLASH_ProgramByte( func_name_add + i,  NULL );
			}
		}
		
		/////////////// func type	/////////////
		uint32_t func_type_add = FUNC_TEST_TYPE_ADD + (cnt) * 2;				
		uint16_t type = atoi(ptr_type);
		FLASH_ProgramHalfWord( func_type_add,  type);
		
		/////////////// func module	/////////////
		uint32_t func_module_add = FUNC_TEST_MODE_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( func_module_add,  atoi(ptr_mode));
															
		/////////////// func dec spec	/////////////
		uint32_t func_spec1_add = FUNC_TEST_SPEC1_ADD + (cnt) * 2;	
		FLASH_ProgramHalfWord( func_spec1_add,  atoi(ptr_spec_dec) );
										
		/////////////// func hex spec /////////////
		uint32_t func_spec2_add = FUNC_TEST_SPEC2_ADD + (cnt) * 4;					
		FLASH_ProgramByte(func_spec2_add + 0, atoi(ptr_spec_hex1)); 	//
		FLASH_ProgramByte(func_spec2_add + 1, atoi(ptr_spec_hex2)); 	//
		FLASH_ProgramByte(func_spec2_add + 2, atoi(ptr_spec_hex3)); 	//
		FLASH_ProgramByte(func_spec2_add + 3, atoi(ptr_spec_hex4)); 	//
						
		/////////////// func float spec	/////////////
		uint32_t func_spec3_add = FUNC_TEST_SPEC3_ADD + (cnt) * 4;		
		float spec3_temp = atof(ptr_spec_float);
		FLASH_ProgramWord( func_spec3_add,  *(uint32_t *)&(spec3_temp) );
		
		/////////////// func ascii spec	/////////////
		uint32_t func_spec4_add = FUNC_TEST_SPEC4_ADD + (cnt) * 16;
		uint32_t func_spec4_cnt = strlen(ptr_spec_ascii);
		uint32_t func_spec4_max = 16;
								
		for(int i = 0; i < func_spec4_max; i++)
		{
			if( i == func_spec4_max -1 )
			{
				FLASH_ProgramByte( func_spec4_add + i,  NULL );
			}
			else if( i < func_spec4_cnt )
			{
				if( (ptr_spec_ascii[i] != '\n') && (ptr_spec_ascii[i] != '\r') )	{	FLASH_ProgramByte( func_spec4_add + i,  ptr_spec_ascii[i] );  	}
				else																{	FLASH_ProgramByte( func_spec4_add + i,  NULL );  		}
			}
			else
			{
				FLASH_ProgramByte( func_spec4_add + i,  NULL );
			}
		}		
		/////////////// func log on/off	/////////////
		uint32_t func_log_add = FUNC_TEST_LOG_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( func_log_add, atoi(ptr_log));
		
		/////////////// func panel link	/////////////
		uint32_t func_panellink_add = FUNC_TEST_PANELLINK_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( func_panellink_add,  atoi(ptr_panel_link));
	}
	
	if(atoi(ptr_count) <= 180)	FLASH_ProgramHalfWord(FUNC_TOTAL_ADD, atoi(ptr_count));
	else						FLASH_ProgramHalfWord(FUNC_TOTAL_ADD, 180);

	FLASH_Lock();

	func_count = (*( (uint16_t *)FUNC_TOTAL_ADD));
	if(func_count > 720) func_count = 0;
	//dbgmsg(" - func_count     : %4d\n", func_count);	
	for(int cnt = 0; cnt < func_count; cnt++)
	{
		sprintf(func_name[cnt], "%s", (char *)(FUNC_TEST_NAME_ADD + (cnt * 32)));
		func_type[cnt]	    	=  ( *( (uint16_t *)( FUNC_TEST_TYPE_ADD    + cnt * 2 ) ));
		func_module[cnt]		=  ( *( (uint16_t *)( FUNC_TEST_MODE_ADD    + cnt * 2 ) ));
		func_spec_dec[cnt]		=  ( *( (uint16_t *)( FUNC_TEST_SPEC1_ADD    + cnt * 2 ) ));
		func_spec_hex[cnt][0]	=  ( *( (uint8_t *) ( FUNC_TEST_SPEC2_ADD + 0 + (cnt * 4) ) ));
		func_spec_hex[cnt][1]	=  ( *( (uint8_t *) ( FUNC_TEST_SPEC2_ADD + 1 + (cnt * 4) ) ));
		func_spec_hex[cnt][2]	=  ( *( (uint8_t *) ( FUNC_TEST_SPEC2_ADD + 2 + (cnt * 4) ) ));
		func_spec_hex[cnt][3]	=  ( *( (uint8_t *) ( FUNC_TEST_SPEC2_ADD + 3 + (cnt * 4) ) ));
		func_spec_float[cnt]	= (*(float *) & ( *( (uint32_t *)( FUNC_TEST_SPEC3_ADD    + cnt * 4 ) )));
		sprintf(func_spec_ascii[cnt], "%s", (char *)(FUNC_TEST_SPEC4_ADD + (cnt * 16)));
		
		func_log_flag[cnt]		= ( *( (uint16_t *)( FUNC_TEST_LOG_ADD    + cnt * 2 ) ));
		func_panel_link[cnt]	= ( *( (uint16_t *)( FUNC_TEST_PANELLINK_ADD    + cnt * 2 ) ));		
		
		//dbgmsg("- %s, %s, %d, %d, %d, %d, %02X.%02X.%02X.%02X, %.1f, %s\n",func_name[cnt],find_func_str(func_type[cnt]),func_module[cnt],func_log_flag[cnt],func_panel_link[cnt],func_spec_dec[cnt],
		//func_spec_hex[cnt][0],func_spec_hex[cnt][1],func_spec_hex[cnt][2],func_spec_hex[cnt][3],
		//func_spec_float[cnt],func_spec_ascii[cnt]);
	}
	buff[0] = true;	tx_pc_data(ICT_FUNC_WR, 1, buff);	
		
	return true;
}
uint8_t mode_Save1_PC(char *data)
{	
	FLASH_Unlock();

	char* ptr_count = strtok(data,",");
	char*  name 	= strtok(NULL,","); 
	
	uint32_t name_add = MODE1_NAME_ADD;
	uint32_t name_cnt = strlen(name);
	uint32_t name_max = 32;
							
	for(int i = 0; i < name_max; i++)
	{
		if( i == name_max -1 )
		{
			FLASH_ProgramByte( name_add + i,  NULL );
		}
		else if( i < name_cnt )
		{
			if( (name[i] != '\n') && (name[i] != '\r') )	{	FLASH_ProgramByte( name_add + i,  name[i] );  	}
			else											{	FLASH_ProgramByte( name_add + i,  NULL );  		}
		}
		else
		{
			FLASH_ProgramByte( name_add + i,  NULL );
		}
	}
	
	for( int cnt = 0; cnt <  atoi(ptr_count); cnt++ )
	{	
		char* op = strtok(NULL,",");
		char* nm = strtok(NULL,",");
		char* st = strtok(NULL,",");
		char* tm = strtok(NULL,",");
		//dbgmsg("[%s]%s, %s, %s, %s, %s\n", __func__, sq, op, st, nm, tm);
		
		uint16_t mode = atoi(op);					
		uint32_t add = MODE1_RUN_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  mode);
											
		add = MODE1_NUBMER_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  atoi(nm));

		uint16_t state = atoi(st);
		add = MODE1_STATE_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  state);
		
		add = MODE1_TIME_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  atoi(tm));			
	}
	if(atoi(ptr_count) <= 100)	FLASH_ProgramHalfWord(MODE1_SEQ_ADD, atoi(ptr_count));
	else						FLASH_ProgramHalfWord(MODE1_SEQ_ADD, 100);
	
	FLASH_Lock();

	sprintf(mode1_name, "%s", (char *)(MODE1_NAME_ADD ));
	mode1_sequence = (*( (uint16_t *)MODE1_SEQ_ADD));
	//dbgmsg(" - mode1_sequence     : %4d\n", mode1_sequence);	
	if(mode1_sequence > 100)  mode1_sequence = 0;
	for( int cnt = 0; cnt < mode1_sequence; cnt++ )
	{
		mode1_operation[cnt] = (*( (uint16_t *)( MODE1_RUN_ADD    + cnt * 2 ) ));	
		mode1_number[cnt]	 = (*( (uint16_t *)( MODE1_NUBMER_ADD + cnt * 2 ) ));
		mode1_status[cnt]	 = (*( (uint16_t *)( MODE1_STATE_ADD  + cnt * 2 ) ));	
		mode1_time[cnt]		 = (*( (uint16_t *)( MODE1_TIME_ADD   + cnt * 2 ) ));
		//dbgmsg("- %s, %s, %d, %s, %d\n",mode1_name,find_mode_str(mode1_operation[cnt]),mode1_number[cnt],find_state_str(mode1_status[cnt]),mode1_time[cnt]);
	}
	buff[0] = true;	tx_pc_data(OP_MODE1_WR, 1, buff);	
	return true;
}
uint8_t mode_Save2_PC(char *data)
{	
	FLASH_Unlock();

	char* ptr_count = strtok(data,",");
	char*  name 	= strtok(NULL,","); 
	
	uint32_t name_add = MODE2_NAME_ADD;
	uint32_t name_cnt = strlen(name);
	uint32_t name_max = 32;
							
	for(int i = 0; i < name_max; i++)
	{
		if( i == name_max -1 )
		{
			FLASH_ProgramByte( name_add + i,  NULL );
		}
		else if( i < name_cnt )
		{
			if( (name[i] != '\n') && (name[i] != '\r') )	{	FLASH_ProgramByte( name_add + i,  name[i] );  	}
			else											{	FLASH_ProgramByte( name_add + i,  NULL );  		}
		}
		else
		{
			FLASH_ProgramByte( name_add + i,  NULL );
		}
	}
	
	for( int cnt = 0; cnt <  atoi(ptr_count); cnt++ )
	{	
		char* op = strtok(NULL,",");
		char* nm = strtok(NULL,",");
		char* st = strtok(NULL,",");
		char* tm = strtok(NULL,",");
		//dbgmsg("[%s]%s, %s, %s, %s, %s\n", __func__, sq, op, st, nm, tm);
		
		uint16_t mode = atoi(op);					
		uint32_t add = MODE2_RUN_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  mode);
											
		add = MODE2_NUBMER_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  atoi(nm));

		uint16_t state = atoi(st);
		add = MODE2_STATE_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  state);
		
		add = MODE2_TIME_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  atoi(tm));			
	}
	if(atoi(ptr_count) <= 100)	FLASH_ProgramHalfWord(MODE2_SEQ_ADD, atoi(ptr_count));
	else						FLASH_ProgramHalfWord(MODE2_SEQ_ADD, 100);
	
	FLASH_Lock();

	sprintf(mode2_name, "%s", (char *)(MODE2_NAME_ADD ));
	mode2_sequence = (*( (uint16_t *)MODE2_SEQ_ADD));
	//dbgmsg(" - mode2_sequence     : %4d\n", mode2_sequence);	
	if(mode2_sequence > 100)  mode2_sequence = 0;
	for( int cnt = 0; cnt < mode2_sequence; cnt++ )
	{
		mode2_operation[cnt] = (*( (uint16_t *)( MODE2_RUN_ADD    + cnt * 2 ) ));	
		mode2_number[cnt]	 = (*( (uint16_t *)( MODE2_NUBMER_ADD + cnt * 2 ) ));
		mode2_status[cnt]	 = (*( (uint16_t *)( MODE2_STATE_ADD  + cnt * 2 ) ));	
		mode2_time[cnt]		 = (*( (uint16_t *)( MODE2_TIME_ADD   + cnt * 2 ) ));
		//dbgmsg("- %s, %s, %d, %s, %d\n",mode2_name,find_mode_str(mode2_operation[cnt]),mode2_number[cnt],find_state_str(mode2_status[cnt]),mode2_time[cnt]);
	}
	
	buff[0] = true;	tx_pc_data(OP_MODE2_WR, 1, buff);	
	return true;
}
uint8_t mode_Save3_PC(char *data)
{	
	FLASH_Unlock();

	char* ptr_count = strtok(data,",");
	char*  name 	= strtok(NULL,","); 
	
	uint32_t name_add = MODE3_NAME_ADD;
	uint32_t name_cnt = strlen(name);
	uint32_t name_max = 32;
							
	for(int i = 0; i < name_max; i++)
	{
		if( i == name_max -1 )
		{
			FLASH_ProgramByte( name_add + i,  NULL );
		}
		else if( i < name_cnt )
		{
			if( (name[i] != '\n') && (name[i] != '\r') )	{	FLASH_ProgramByte( name_add + i,  name[i] );  	}
			else											{	FLASH_ProgramByte( name_add + i,  NULL );  		}
		}
		else
		{
			FLASH_ProgramByte( name_add + i,  NULL );
		}
	}
	
	for( int cnt = 0; cnt <  atoi(ptr_count); cnt++ )
	{	
		char* op = strtok(NULL,",");
		char* nm = strtok(NULL,",");
		char* st = strtok(NULL,",");
		char* tm = strtok(NULL,",");
		//dbgmsg("[%s]%s, %s, %s, %s, %s\n", __func__, sq, op, st, nm, tm);
		
		uint16_t mode = atoi(op);					
		uint32_t add = MODE3_RUN_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  mode);
											
		add = MODE3_NUBMER_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  atoi(nm));

		uint16_t state = atoi(st);
		add = MODE3_STATE_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  state);
		
		add = MODE3_TIME_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  atoi(tm));			
	}
	if(atoi(ptr_count) <= 100)	FLASH_ProgramHalfWord(MODE3_SEQ_ADD, atoi(ptr_count));
	else						FLASH_ProgramHalfWord(MODE3_SEQ_ADD, 100);
	
	FLASH_Lock();

	sprintf(mode3_name, "%s", (char *)(MODE3_NAME_ADD ));
	mode3_sequence = (*( (uint16_t *)MODE3_SEQ_ADD));
	//dbgmsg(" - mode3_sequence     : %4d\n", mode3_sequence);	
	if(mode3_sequence > 100)  mode3_sequence = 0;
	for( int cnt = 0; cnt < mode3_sequence; cnt++ )
	{
		mode3_operation[cnt] = (*( (uint16_t *)( MODE3_RUN_ADD    + cnt * 2 ) ));	
		mode3_number[cnt]	 = (*( (uint16_t *)( MODE3_NUBMER_ADD + cnt * 2 ) ));
		mode3_status[cnt]	 = (*( (uint16_t *)( MODE3_STATE_ADD  + cnt * 2 ) ));	
		mode3_time[cnt]		 = (*( (uint16_t *)( MODE3_TIME_ADD   + cnt * 2 ) ));
		//dbgmsg("- %s, %s, %d, %s, %d\n",mode3_name,find_mode_str(mode3_operation[cnt]),mode3_number[cnt],find_state_str(mode3_status[cnt]),mode3_time[cnt]);
	}
	
	buff[0] = true;	tx_pc_data(OP_MODE3_WR, 1, buff);	
	return true;
}
uint8_t mode_Save4_PC(char *data)
{	
	FLASH_Unlock();

	char* ptr_count = strtok(data,",");
	char*  name 	= strtok(NULL,","); 
	
	uint32_t name_add = MODE4_NAME_ADD;
	uint32_t name_cnt = strlen(name);
	uint32_t name_max = 32;
							
	for(int i = 0; i < name_max; i++)
	{
		if( i == name_max -1 )
		{
			FLASH_ProgramByte( name_add + i,  NULL );
		}
		else if( i < name_cnt )
		{
			if( (name[i] != '\n') && (name[i] != '\r') )	{	FLASH_ProgramByte( name_add + i,  name[i] );  	}
			else											{	FLASH_ProgramByte( name_add + i,  NULL );  		}
		}
		else
		{
			FLASH_ProgramByte( name_add + i,  NULL );
		}
	}
	
	for( int cnt = 0; cnt <  atoi(ptr_count); cnt++ )
	{	
		char* op = strtok(NULL,",");
		char* nm = strtok(NULL,",");
		char* st = strtok(NULL,",");
		char* tm = strtok(NULL,",");
		//dbgmsg("[%s]%s, %s, %s, %s, %s\n", __func__, sq, op, st, nm, tm);
		
		uint16_t mode = atoi(op);					
		uint32_t add = MODE4_RUN_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  mode);
											
		add = MODE4_NUBMER_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  atoi(nm));

		uint16_t state = atoi(st);
		add = MODE4_STATE_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  state);
		
		add = MODE4_TIME_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  atoi(tm));			
	}
	if(atoi(ptr_count) <= 100)	FLASH_ProgramHalfWord(MODE4_SEQ_ADD, atoi(ptr_count));
	else						FLASH_ProgramHalfWord(MODE4_SEQ_ADD, 100);
	
	FLASH_Lock();

	sprintf(mode4_name, "%s", (char *)(MODE4_NAME_ADD ));
	mode4_sequence = (*( (uint16_t *)MODE4_SEQ_ADD));
	//dbgmsg(" - mode4_sequence     : %4d\n", mode4_sequence);	
	if(mode4_sequence > 100)  mode4_sequence = 0;
	for( int cnt = 0; cnt < mode4_sequence; cnt++ )
	{
		mode4_operation[cnt] = (*( (uint16_t *)( MODE4_RUN_ADD    + cnt * 2 ) ));	
		mode4_number[cnt]	 = (*( (uint16_t *)( MODE4_NUBMER_ADD + cnt * 2 ) ));
		mode4_status[cnt]	 = (*( (uint16_t *)( MODE4_STATE_ADD  + cnt * 2 ) ));	
		mode4_time[cnt]		 = (*( (uint16_t *)( MODE4_TIME_ADD   + cnt * 2 ) ));
		//dbgmsg("- %s, %s, %d, %s, %d\n",mode4_name,find_mode_str(mode4_operation[cnt]),mode4_number[cnt],find_state_str(mode4_status[cnt]),mode4_time[cnt]);
	}
	
	buff[0] = true;	tx_pc_data(OP_MODE4_WR, 1, buff);	
	return true;
}
uint8_t init_Save_PC(char *data)
{	
	FLASH_Unlock();

	char* ptr_count = strtok(data,",");
	char*  name 	= strtok(NULL,","); 
	
	uint32_t name_add = INIT_NAME_ADD;
	uint32_t name_cnt = strlen(name);
	uint32_t name_max = 32;
							
	for(int i = 0; i < name_max; i++)
	{
		if( i == name_max -1 )
		{
			FLASH_ProgramByte( name_add + i,  NULL );
		}
		else if( i < name_cnt )
		{
			if( (name[i] != '\n') && (name[i] != '\r') )	{	FLASH_ProgramByte( name_add + i,  name[i] );  	}
			else											{	FLASH_ProgramByte( name_add + i,  NULL );  		}
		}
		else
		{
			FLASH_ProgramByte( name_add + i,  NULL );
		}
	}
	
	for( int cnt = 0; cnt <  atoi(ptr_count); cnt++ )
	{	
		char* op = strtok(NULL,",");
		char* nm = strtok(NULL,",");
		char* st = strtok(NULL,",");
		char* tm = strtok(NULL,",");
		//dbgmsg("[%s]%s, %s, %s, %s, %s\n", __func__, sq, op, st, nm, tm);
		
		uint16_t mode = atoi(op);					
		uint32_t add = INIT_RUN_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  mode);
											
		add = INIT_NUBMER_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  atoi(nm));

		uint16_t state = atoi(st);
		add = INIT_STATE_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  state);
		
		add = INIT_TIME_ADD + (cnt) * 2;
		FLASH_ProgramHalfWord( add,  atoi(tm));			
	}
	if(atoi(ptr_count) <= 100)	FLASH_ProgramHalfWord(INIT_SEQ_ADD, atoi(ptr_count));
	else						FLASH_ProgramHalfWord(INIT_SEQ_ADD, 100);
	
	FLASH_Lock();

	sprintf(init_name, "%s", (char *)(INIT_NAME_ADD ));
	init_sequence = (*( (uint16_t *)INIT_SEQ_ADD));
	//dbgmsg(" - init_sequence     : %4d\n", init_sequence);	
	if(init_sequence > 100)  init_sequence = 0;
	for( int cnt = 0; cnt < init_sequence; cnt++ )
	{
		init_operation[cnt]  = (*( (uint16_t *)( INIT_RUN_ADD    + cnt * 2 ) ));	
		init_number[cnt]	 = (*( (uint16_t *)( INIT_NUBMER_ADD + cnt * 2 ) ));
		init_status[cnt]	 = (*( (uint16_t *)( INIT_STATE_ADD  + cnt * 2 ) ));	
		init_time[cnt]		 = (*( (uint16_t *)( INIT_TIME_ADD   + cnt * 2 ) ));
		//dbgmsg("- %s, %s, %d, %s, %d\n",init_name,find_mode_str(init_operation[cnt]),init_number[cnt],find_state_str(init_status[cnt]),init_time[cnt]);
	}
	
	buff[0] = true;	tx_pc_data(OP_INIT_WR, 1, buff);	
	return true;
}

uint8_t Info_Send_PC(void)
{	
	buff[0] = (info_crc >>24) & 0xff;		
	buff[1] = (info_crc >>16) & 0xff;		
	buff[2] = (info_crc >> 8) & 0xff;		
	buff[3] = (info_crc >> 0) & 0xff;		
	buff[4] = (tp_crc >>24) & 0xff;		
	buff[5] = (tp_crc >>16) & 0xff;		
	buff[6] = (tp_crc >> 8) & 0xff;		
	buff[7] = (tp_crc >> 0) & 0xff;		
	buff[8] = (open_crc >>24) & 0xff;		
	buff[9] = (open_crc >>16) & 0xff;		
	buff[10] = (open_crc >> 8) & 0xff;		
	buff[11] = (open_crc >> 0) & 0xff;		
	buff[12] = (short_crc >>24) & 0xff;		
	buff[13] = (short_crc >>16) & 0xff;		
	buff[14] = (short_crc >> 8) & 0xff;		
	buff[15] = (short_crc >> 0) & 0xff;		
	buff[16] = (shortex_crc >>24) & 0xff;		
	buff[17] = (shortex_crc >>16) & 0xff;		
	buff[18] = (shortex_crc >> 8) & 0xff;		
	buff[19] = (shortex_crc >> 0) & 0xff;	
	buff[20] = (part_crc >>24) & 0xff;		
	buff[21] = (part_crc >>16) & 0xff;		
	buff[22] = (part_crc >> 8) & 0xff;		
	buff[23] = (part_crc >> 0) & 0xff;		
	buff[24] = (func_crc >>24) & 0xff;		
	buff[25] = (func_crc >>16) & 0xff;		
	buff[26] = (func_crc >> 8) & 0xff;		
	buff[27] = (func_crc >> 0) & 0xff;	
	buff[28] = (init_crc >>24) & 0xff;		
	buff[29] = (init_crc >>16) & 0xff;		
	buff[30] = (init_crc >> 8) & 0xff;		
	buff[31] = (init_crc >> 0) & 0xff;	
	buff[32] = (mode1_crc >>24) & 0xff;		
	buff[33] = (mode1_crc >>16) & 0xff;		
	buff[34] = (mode1_crc >> 8) & 0xff;		
	buff[35] = (mode1_crc >> 0) & 0xff;	
	buff[36] = (mode2_crc >>24) & 0xff;		
	buff[37] = (mode2_crc >>16) & 0xff;		
	buff[38] = (mode2_crc >> 8) & 0xff;		
	buff[39] = (mode2_crc >> 0) & 0xff;		
	buff[40] = (mode3_crc >>24) & 0xff;		
	buff[41] = (mode3_crc >>16) & 0xff;		
	buff[42] = (mode3_crc >> 8) & 0xff;		
	buff[43] = (mode3_crc >> 0) & 0xff;		
	buff[44] = (mode4_crc >>24) & 0xff;		
	buff[45] = (mode4_crc >>16) & 0xff;		
	buff[46] = (mode4_crc >> 8) & 0xff;		
	buff[47] = (mode4_crc >> 0) & 0xff;	
	
	tx_pc_data(ICT_PROJECT_RD, 48, buff);
	return true;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
