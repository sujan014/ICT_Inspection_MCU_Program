#include "task_ict_spec_save.h"

void UpdateIctSpecSdram(void)
{		 
	if( SPEC_SAVE_FLAG != SPEC_CHECK_FLAG )
	{
		dbgmsg("ICT Spec Save NG\n");
		//////////////////
		// info.csv update
		sprintf(project_name_main, "Project name");	
		sprintf(project_name_sub,  "Spec history");
		short_auto_pp 	 =  0;
		short_auto_mm 	 =  0;		
		//////////////////
		// tp.csv update
		tp_count =  0;
		//////////////////
		// open.csv update
		open_count = 0;
		//////////////////
		// shortbs.csv update
		short_bs_count = 0;
		//////////////////
		// shortex.csv update		
		short_ex_count =0;
		// part.csv update		
		part_count = 0;
		//////////////////
		// func.csv update		
		func_count = 0;
		//////////////////
		// Insp_init.csv update
		sprintf(init_name, "Init");
		init_sequence = 0;
		//////////////////
		// mode1.csv update
		sprintf(mode1_name, "mode1");
		mode1_sequence = 0;
		//////////////////
		// mode2.csv update
		sprintf(mode2_name, "mode2");
		mode2_sequence = 0;
		//////////////////
		// mode3.csv update
		sprintf(mode3_name, "mode3");
		mode3_sequence = 0;
		//////////////////
		// mode4.csv update
		sprintf(mode4_name, "mode4");
		mode4_sequence = 0;
		
		info_crc = 0;
		tp_crc = 0;
		open_crc = 0;
		short_crc = 0;
		shortex_crc = 0;
		part_crc = 0;
		func_crc = 0;
		mode1_crc = 0;
		mode2_crc = 0;
		mode3_crc = 0;
		mode4_crc = 0;
		init_crc = 0;
	}
	else
	{		
		dbgmsg("ICT Spec Save OK\n");
		//////////////////
		// info.csv update
		//dbgmsg("[info.csv] sdram update\n");
		sprintf(project_name_main, "%s", (char *)(PJ_MAIN_NAME_ADD) );	
		sprintf(project_name_sub,  "%s", (char *)(PJ_SUB_NAME_ADD)  );		
		//dbgmsg("- Promain Name : %s\n",project_name_main);
		//dbgmsg("- ProSub  Name : %s\n",project_name_sub);
		
		short_auto_pp 	 =  (*( (uint16_t *)( SH_AUTO_P_ADD ) ));
		short_auto_mm 	 =  (*( (uint16_t *)( SH_AUTO_N_ADD ) ));
		//dbgmsg("- Short Auto   : %03d-%03d\n",short_auto_pp,short_auto_mm);
				
		info_crc = set_crc32_init_value(CRC_INITIALVALUE);	
		for(int i=0;i<strlen(project_name_main);i++)	
		{
			info_crc = crc32_table[((info_crc >> 24) ^ project_name_main[i])] ^ (info_crc << 8);
		}
		for(int i=0;i<strlen(project_name_sub);i++)	
		{
			info_crc = crc32_table[((info_crc >> 24) ^ project_name_sub[i])] ^ (info_crc << 8);
		}
		info_crc = crc32_table[((info_crc >> 24) ^ ( (short_auto_pp>>8) & 0x00ff) ) ]  ^ (info_crc << 8);
		info_crc = crc32_table[((info_crc >> 24) ^ ( (short_auto_pp>>0) & 0x00ff) ) ]  ^ (info_crc << 8);
		info_crc = crc32_table[((info_crc >> 24) ^ ( (short_auto_mm>>8) & 0x00ff) ) ]  ^ (info_crc << 8);
		info_crc = crc32_table[((info_crc >> 24) ^ ( (short_auto_mm>>0) & 0x00ff) ) ]  ^ (info_crc << 8);
			
		dbgmsg("info_crc %08lX\n",info_crc);
		
		//////////////////
		// tp.csv update
		//dbgmsg("[tp.csv] sdram update\n");
		tp_count =  (*( (uint16_t *)TEST_POINT_TOTAL_ADD));
		if(tp_count > 720) tp_count = 0;
		
		tp_crc = set_crc32_init_value(CRC_INITIALVALUE);
		tp_crc = crc32_table[((tp_crc >> 24) ^ ( (tp_count>>8) & 0x00ff) ) ]  ^ (tp_crc << 8);
		tp_crc = crc32_table[((tp_crc >> 24) ^ ( (tp_count>>0) & 0x00ff) ) ]  ^ (tp_crc << 8);
		for( int cnt = 0; cnt < tp_count; cnt++ )
		{
			sprintf(tp_name[cnt], "%s", (char *)(TEST_POINT_NAME_ADD + (cnt * 32)));	
			tp_number[cnt] = (*( (uint16_t *)( TEST_POINT_NUMBER_ADD + cnt * 2 ) ));
			//dbgmsg("- %s, %d\n",tp_name[cnt],tp_number[cnt]);

			for(int i=0;i<strlen(tp_name[cnt]);i++)	
			{
				tp_crc = crc32_table[((tp_crc >> 24) ^ tp_name[cnt][i])] ^ (tp_crc << 8);
			}
			tp_crc = crc32_table[((tp_crc >> 24) ^ ( (tp_number[cnt]>>8) & 0x00ff) ) ]  ^ (tp_crc << 8);
			tp_crc = crc32_table[((tp_crc >> 24) ^ ( (tp_number[cnt]>>0) & 0x00ff) ) ]  ^ (tp_crc << 8);
		}
		dbgmsg("tp_crc %08lX\n",tp_crc);
		
		//////////////////
		// open.csv update
		//dbgmsg("[open.csv] sdram update\n");
		open_count = (*( (uint16_t *)OPEN_TOTAL_ADD));
		if(open_count > 720) open_count = 0;
		open_crc = set_crc32_init_value(CRC_INITIALVALUE);
		open_crc = crc32_table[((open_crc >> 24) ^ ( (open_count>>8) & 0x00ff) ) ]  ^ (open_crc << 8);
		open_crc = crc32_table[((open_crc >> 24) ^ ( (open_count>>0) & 0x00ff) ) ]  ^ (open_crc << 8);
		for(int cnt=0; cnt< open_count; cnt++)
		{
			open_tp1[cnt]		=	(*( (uint16_t *)( OPEN_TESTPOINT1_ADD    + cnt * 2 ) ));
			open_tp2[cnt] 		=	(*( (uint16_t *)( OPEN_TESTPOINT2_ADD    + cnt * 2 ) ));
			open_range[cnt] 	=	(*( (uint16_t *)( OPEN_TEST_RANGE_ADD    + cnt * 2 ) ));
			open_ok_cnt[cnt] 	=	(*( (uint16_t *)( OPEN_TEST_OK_COUNT_ADD + cnt * 2 ) ));
			open_ng_cnt[cnt] 	=	(*( (uint16_t *)( OPEN_TEST_NG_COUNT_ADD + cnt * 2 ) ));
			open_spec[cnt] 		=	(*(float *)&(*( (uint32_t *)( OPEN_TEST_SPEC_ADD + cnt * 4 ) )));
			//dbgmsg("- %d-%d, %d ,%d, %d, %.1f\n",open_tp1[cnt],open_tp2[cnt],open_range[cnt],open_ok_cnt[cnt],open_ng_cnt[cnt],open_spec[cnt]);
						
			uint16_t open_spec_temp = (uint16_t)(open_spec[cnt] * 100);
			
			open_crc = crc32_table[((open_crc >> 24) ^ ( (open_tp1[cnt]>>8) & 0x00ff) ) ]  ^ (open_crc << 8);
			open_crc = crc32_table[((open_crc >> 24) ^ ( (open_tp1[cnt]>>0) & 0x00ff) ) ]  ^ (open_crc << 8);

			open_crc = crc32_table[((open_crc >> 24) ^ ( (open_tp2[cnt]>>8) & 0x00ff) ) ]  ^ (open_crc << 8);
			open_crc = crc32_table[((open_crc >> 24) ^ ( (open_tp2[cnt]>>0) & 0x00ff) ) ]  ^ (open_crc << 8);

			open_crc = crc32_table[((open_crc >> 24) ^ ( (open_range[cnt]>>8) & 0x00ff) ) ]  ^ (open_crc << 8);
			open_crc = crc32_table[((open_crc >> 24) ^ ( (open_range[cnt]>>0) & 0x00ff) ) ]  ^ (open_crc << 8);

			open_crc = crc32_table[((open_crc >> 24) ^ ( (open_spec_temp>>8) & 0x00ff) ) ]  ^ (open_crc << 8);
			open_crc = crc32_table[((open_crc >> 24) ^ ( (open_spec_temp>>0) & 0x00ff) ) ]  ^ (open_crc << 8);

			open_crc = crc32_table[((open_crc >> 24) ^ ( (open_ok_cnt[cnt]>>8) & 0x00ff) ) ]  ^ (open_crc << 8);
			open_crc = crc32_table[((open_crc >> 24) ^ ( (open_ok_cnt[cnt]>>0) & 0x00ff) ) ]  ^ (open_crc << 8);

			open_crc = crc32_table[((open_crc >> 24) ^ ( (open_ng_cnt[cnt]>>8) & 0x00ff) ) ]  ^ (open_crc << 8);
			open_crc = crc32_table[((open_crc >> 24) ^ ( (open_ng_cnt[cnt]>>0) & 0x00ff) ) ]  ^ (open_crc << 8);		
		}
		dbgmsg("open_crc %08lX\n",open_crc);
		//////////////////
		// shortbs.csv update
		//dbgmsg("[shortbs.csv] sdram update\n");
		short_bs_count  =	(*( (uint16_t *)SHORT_TOTAL_ADD));		
		if(short_bs_count > 720) short_bs_count = 0;
		short_bs_range  =	(*( (uint16_t *)SHORT_TEST_RANGE_ADD));
		short_bs_ok_cnt =	(*( (uint16_t *)SHORT_TEST_OK_COUNT_ADD	));
		short_bs_ng_cnt =	(*( (uint16_t *)SHORT_TEST_NG_COUNT_ADD	)); 
		short_bs_spec   =	(*(float *) & (*( (uint32_t *)SHORT_TEST_SPEC_ADD		)));

		if (short_bs_count == 0)	// Sujan 221229			// To match PC and ARM CRC when short count = 0;
		{
			short_bs_range  =	0;
			short_bs_ok_cnt =	0;
			short_bs_ng_cnt =	0;
			short_bs_spec   =	0;
		}
		dbgmsg("short_bs_range %d\n",short_bs_range);
		dbgmsg("short_bs_ok_cnt %d\n",short_bs_ok_cnt);
		dbgmsg("short_bs_ng_cnt %d\n",short_bs_ng_cnt);
		dbgmsg("short_bs_spec %d\n",short_bs_spec);
		
		uint16_t short_spec_temp = (uint16_t)(short_bs_spec * 100);
		
		short_crc = set_crc32_init_value(CRC_INITIALVALUE);
		short_crc = crc32_table[((short_crc >> 24) ^ ( (short_bs_count>>8) & 0x00ff) ) ]  ^ (short_crc << 8);
		short_crc = crc32_table[((short_crc >> 24) ^ ( (short_bs_count>>0) & 0x00ff) ) ]  ^ (short_crc << 8);
		short_crc = crc32_table[((short_crc >> 24) ^ ( (short_bs_range>>8) & 0x00ff) ) ]  ^ (short_crc << 8);
		short_crc = crc32_table[((short_crc >> 24) ^ ( (short_bs_range>>0) & 0x00ff) ) ]  ^ (short_crc << 8);
		short_crc = crc32_table[((short_crc >> 24) ^ ( (short_bs_ok_cnt>>8) & 0x00ff) ) ]  ^ (short_crc << 8);
		short_crc = crc32_table[((short_crc >> 24) ^ ( (short_bs_ok_cnt>>0) & 0x00ff) ) ]  ^ (short_crc << 8);
		short_crc = crc32_table[((short_crc >> 24) ^ ( (short_bs_ng_cnt>>8) & 0x00ff) ) ]  ^ (short_crc << 8);
		short_crc = crc32_table[((short_crc >> 24) ^ ( (short_bs_ng_cnt>>0) & 0x00ff) ) ]  ^ (short_crc << 8);
		short_crc = crc32_table[((short_crc >> 24) ^ ( (short_spec_temp>>8) & 0x00ff) ) ]  ^ (short_crc << 8);
		short_crc = crc32_table[((short_crc >> 24) ^ ( (short_spec_temp>>0) & 0x00ff) ) ]  ^ (short_crc << 8);
		
		dbgmsg("MID short_crc %08lX\n",short_crc);
		for(int cnt=0; cnt< short_bs_count; cnt++)
		{
			short_bs_tp[cnt]	=	(*( (uint16_t *)( SHORT_TEST_POINT_ADD + cnt * 2 ) ));
			short_bs_group[cnt]	=	(*( (uint16_t *)( SHORT_TEST_GROUP_ADD + cnt * 2 ) ));
			//dbgmsg("- %d-%d, %d, %d, %d, %.1f\n",short_bs_tp[cnt],short_bs_group[cnt],short_bs_range,short_bs_ok_cnt,short_bs_ng_cnt,short_bs_spec);
			
			short_crc = crc32_table[((short_crc >> 24) ^ ( (short_bs_tp[cnt]>>8) & 0x00ff) ) ]  ^ (short_crc << 8);
			short_crc = crc32_table[((short_crc >> 24) ^ ( (short_bs_tp[cnt]>>0) & 0x00ff) ) ]  ^ (short_crc << 8);
				
			short_crc = crc32_table[((short_crc >> 24) ^ ( (short_bs_group[cnt]>>8) & 0x00ff) ) ]  ^ (short_crc << 8);
			short_crc = crc32_table[((short_crc >> 24) ^ ( (short_bs_group[cnt]>>0) & 0x00ff) ) ]  ^ (short_crc << 8);
		}
		dbgmsg("short_crc %08lX\n",short_crc);
		//////////////////
		// shortex.csv update
		//dbgmsg("[shortex.csv] sdram update\n");			
		short_ex_count = (*( (uint16_t *)SHEX_TOTAL_ADD));		
		if(short_ex_count > 720) short_ex_count = 0;
		shortex_crc = set_crc32_init_value(CRC_INITIALVALUE);
		shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (short_ex_count>>8) & 0x00ff) ) ]  ^ (shortex_crc << 8);
		shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (short_ex_count>>0) & 0x00ff) ) ]  ^ (shortex_crc << 8);
		
		for(int cnt=0; cnt< short_ex_count; cnt++)
		{
			short_ex_tp1[cnt]	=	(*( (uint16_t *)( SHEX_TESTPOINT1_ADD    + cnt * 2 ) ));
			short_ex_tp2[cnt]	=	(*( (uint16_t *)( SHEX_TESTPOINT2_ADD    + cnt * 2 ) ));
			short_ex_range[cnt]	=	(*( (uint16_t *)( SHEX_TEST_RANGE_ADD    + cnt * 2 ) ));
			short_ex_ok_cnt[cnt]=	(*( (uint16_t *)( SHEX_TEST_OK_COUNT_ADD + cnt * 2 ) ));
			short_ex_ng_cnt[cnt]=	(*( (uint16_t *)( SHEX_TEST_NG_COUNT_ADD + cnt * 2 ) ));
			short_ex_spec[cnt]	= (*(float *)&(*( (uint32_t *)( SHEX_TEST_SPEC_ADD + cnt * 4 ) )));
			//dbgmsg("- %d-%d, %d, %d, %d, %.1f\n",short_ex_tp1[cnt],short_ex_tp2[cnt],short_ex_range[cnt],short_ex_ok_cnt[cnt],short_ex_ng_cnt[cnt],short_ex_spec[cnt]);

			uint16_t shortex_spec_temp = (uint16_t)(short_ex_spec[cnt] * 100);
			
			shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (short_ex_tp1[cnt]>>8) & 0x00ff) ) ]  ^ (shortex_crc << 8);
			shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (short_ex_tp1[cnt]>>0) & 0x00ff) ) ]  ^ (shortex_crc << 8);
			shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (short_ex_tp2[cnt]>>8) & 0x00ff) ) ]  ^ (shortex_crc << 8);
			shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (short_ex_tp2[cnt]>>0) & 0x00ff) ) ]  ^ (shortex_crc << 8);
			shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (short_ex_range[cnt]>>8) & 0x00ff) ) ]  ^ (shortex_crc << 8);
			shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (short_ex_range[cnt]>>0) & 0x00ff) ) ]  ^ (shortex_crc << 8);
			shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (short_ex_ok_cnt[cnt]>>8) & 0x00ff) ) ]  ^ (shortex_crc << 8);
			shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (short_ex_ok_cnt[cnt]>>0) & 0x00ff) ) ]  ^ (shortex_crc << 8);
			shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (short_ex_ng_cnt[cnt]>>8) & 0x00ff) ) ]  ^ (shortex_crc << 8);
			shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (short_ex_ng_cnt[cnt]>>0) & 0x00ff) ) ]  ^ (shortex_crc << 8);
			shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (shortex_spec_temp>>8) & 0x00ff) ) ]  ^ (shortex_crc << 8);
			shortex_crc = crc32_table[((shortex_crc >> 24) ^ ( (shortex_spec_temp>>0) & 0x00ff) ) ]  ^ (shortex_crc << 8);
		}	
		
		dbgmsg("shortex_crc %08lX\n",shortex_crc);
		
		//////////////////
		// part.csv update
		//dbgmsg("[part.csv] sdram update\n");		
		part_count = (*( (uint16_t *)PART_TOTAL_ADD));
		if(part_count > 720) part_count = 0;
		part_crc = set_crc32_init_value(CRC_INITIALVALUE);
		part_crc = crc32_table[((part_crc >> 24) ^ ( (part_count>>8) & 0x00ff) ) ]  ^ (part_crc << 8);
		part_crc = crc32_table[((part_crc >> 24) ^ ( (part_count>>0) & 0x00ff) ) ]  ^ (part_crc << 8);
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
			
			uint16_t part1_spec_temp = (uint16_t)(part_spec_hi[cnt] * 100);
			uint16_t part2_spec_temp = (uint16_t)(part_spec_lo[cnt] * 100);
			
			for(int i=0;i<strlen(part_name[cnt]);i++)	
			{
				part_crc = crc32_table[((part_crc >> 24) ^ part_name[cnt][i])] ^ (part_crc << 8);
			}
			
			part_crc = crc32_table[((part_crc >> 24) ^ ( (part_tp1[cnt]>>8) & 0x00ff) ) ]  ^ (part_crc << 8);
			part_crc = crc32_table[((part_crc >> 24) ^ ( (part_tp1[cnt]>>0) & 0x00ff) ) ]  ^ (part_crc << 8);
			
			part_crc = crc32_table[((part_crc >> 24) ^ ( (part_tp2[cnt]>>8) & 0x00ff) ) ]  ^ (part_crc << 8);
			part_crc = crc32_table[((part_crc >> 24) ^ ( (part_tp2[cnt]>>0) & 0x00ff) ) ]  ^ (part_crc << 8);
			
			part_crc = crc32_table[((part_crc >> 24) ^ ( (part_range[cnt]>>8) & 0x00ff) ) ]  ^ (part_crc << 8);
			part_crc = crc32_table[((part_crc >> 24) ^ ( (part_range[cnt]>>0) & 0x00ff) ) ]  ^ (part_crc << 8);
			
			part_crc = crc32_table[((part_crc >> 24) ^ ( (part_ok_cnt[cnt]>>8) & 0x00ff) ) ]  ^ (part_crc << 8);
			part_crc = crc32_table[((part_crc >> 24) ^ ( (part_ok_cnt[cnt]>>0) & 0x00ff) ) ]  ^ (part_crc << 8);
			
			part_crc = crc32_table[((part_crc >> 24) ^ ( (part_ng_cnt[cnt]>>8) & 0x00ff) ) ]  ^ (part_crc << 8);
			part_crc = crc32_table[((part_crc >> 24) ^ ( (part_ng_cnt[cnt]>>0) & 0x00ff) ) ]  ^ (part_crc << 8);
			
			part_crc = crc32_table[((part_crc >> 24) ^ ( (part1_spec_temp>>8) & 0x00ff) ) ]  ^ (part_crc << 8);
			part_crc = crc32_table[((part_crc >> 24) ^ ( (part1_spec_temp>>0) & 0x00ff) ) ]  ^ (part_crc << 8);

			part_crc = crc32_table[((part_crc >> 24) ^ ( (part2_spec_temp>>8) & 0x00ff) ) ]  ^ (part_crc << 8);
			part_crc = crc32_table[((part_crc >> 24) ^ ( (part2_spec_temp>>0) & 0x00ff) ) ]  ^ (part_crc << 8);			
		}
		dbgmsg("part_crc %08lX\n",part_crc);
		//////////////////
		// func.csv update
		//dbgmsg("[func.csv] sdram update\n");		
		func_count = (*( (uint16_t *)FUNC_TOTAL_ADD));
		if(func_count > 720) func_count = 0;
		func_crc = set_crc32_init_value(CRC_INITIALVALUE);
		func_crc = crc32_table[((func_crc >> 24) ^ ( (func_count>>8) & 0x00ff) ) ]  ^ (func_crc << 8);
		func_crc = crc32_table[((func_crc >> 24) ^ ( (func_count>>0) & 0x00ff) ) ]  ^ (func_crc << 8);
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
			
			//dbgmsg("- %s, %s, %d, %d, %02X.%02X.%02X.%02X, %.1f, %s\n",func_name[cnt],find_func_str(func_type[cnt]),func_module[cnt],func_spec_dec[cnt],
			//func_spec_hex[cnt][0],func_spec_hex[cnt][1],func_spec_hex[cnt][2],func_spec_hex[cnt][3],
			//func_spec_float[cnt],func_spec_ascii);		
			
			for(int i=0;i<strlen(func_name[cnt]);i++)	
			{
				func_crc = crc32_table[((func_crc >> 24) ^ func_name[cnt][i])] ^ (func_crc << 8);
			}
			func_crc = crc32_table[((func_crc >> 24) ^ ( (func_type[cnt]>>8) & 0x00ff) ) ]  ^ (func_crc << 8);
			func_crc = crc32_table[((func_crc >> 24) ^ ( (func_type[cnt]>>0) & 0x00ff) ) ]  ^ (func_crc << 8);

			func_crc = crc32_table[((func_crc >> 24) ^ ( (func_module[cnt]>>8) & 0x00ff) ) ]  ^ (func_crc << 8);
			func_crc = crc32_table[((func_crc >> 24) ^ ( (func_module[cnt]>>0) & 0x00ff) ) ]  ^ (func_crc << 8);

			func_crc = crc32_table[((func_crc >> 24) ^ ( (func_log_flag[cnt]>>8) & 0x00ff) ) ]  ^ (func_crc << 8);
			func_crc = crc32_table[((func_crc >> 24) ^ ( (func_log_flag[cnt]>>0) & 0x00ff) ) ]  ^ (func_crc << 8);

			func_crc = crc32_table[((func_crc >> 24) ^ ( (func_panel_link[cnt]>>8) & 0x00ff) ) ]  ^ (func_crc << 8);
			func_crc = crc32_table[((func_crc >> 24) ^ ( (func_panel_link[cnt]>>0) & 0x00ff) ) ]  ^ (func_crc << 8);

			func_crc = crc32_table[((func_crc >> 24) ^ ( (func_spec_dec[cnt]>>8) & 0x00ff) ) ]  ^ (func_crc << 8);
			func_crc = crc32_table[((func_crc >> 24) ^ ( (func_spec_dec[cnt]>>0) & 0x00ff) ) ]  ^ (func_crc << 8);
			
			func_crc = crc32_table[(func_crc >> 24) ^ ((func_spec_hex[cnt][0] ) & 0x00ff)] ^ (func_crc << 8);
			func_crc = crc32_table[(func_crc >> 24) ^ ((func_spec_hex[cnt][1] ) & 0x00ff)] ^ (func_crc << 8);
			func_crc = crc32_table[(func_crc >> 24) ^ ((func_spec_hex[cnt][2] ) & 0x00ff)] ^ (func_crc << 8);
			func_crc = crc32_table[(func_crc >> 24) ^ ((func_spec_hex[cnt][3] ) & 0x00ff)] ^ (func_crc << 8);
			
			uint16_t func_spec_temp = (uint16_t)(func_spec_float[cnt] * 100); 
			
			func_crc = crc32_table[((func_crc >> 24) ^ ( (func_spec_temp>>8) & 0x00ff) ) ]  ^ (func_crc << 8);
			func_crc = crc32_table[((func_crc >> 24) ^ ( (func_spec_temp>>0) & 0x00ff) ) ]  ^ (func_crc << 8);
			
			for(int i=0;i<strlen(func_spec_ascii[cnt]);i++)	
			{
				func_crc = crc32_table[((func_crc >> 24) ^ func_spec_ascii[cnt][i])] ^ (func_crc << 8);
			}
		}
		dbgmsg("func_crc %08lX\n",func_crc);
		//////////////////
		// Insp_init.csv update
		//dbgmsg("[init.csv] sdram update\n");	
		sprintf(init_name, "%s", (char *)(INIT_NAME_ADD ));
		init_sequence = (*( (uint16_t *)INIT_SEQ_ADD));
		if(init_sequence > 100)  init_sequence = 0;
		init_crc = set_crc32_init_value(CRC_INITIALVALUE);
		init_crc = crc32_table[((init_crc >> 24) ^ ( (init_sequence>>8) & 0x00ff) ) ]  ^ (init_crc << 8);
		init_crc = crc32_table[((init_crc >> 24) ^ ( (init_sequence>>0) & 0x00ff) ) ]  ^ (init_crc << 8);
		for(int i=0;i<strlen(init_name);i++)	
		{
			init_crc = crc32_table[((init_crc >> 24) ^ init_name[i])] ^ (init_crc << 8);
		}

		for( int cnt = 0; cnt < init_sequence; cnt++ )
		{
			init_operation[cnt]	 = (*( (uint16_t *)( INIT_RUN_ADD    + cnt * 2 ) ));	
			init_number[cnt]	 = (*( (uint16_t *)( INIT_NUBMER_ADD + cnt * 2 ) ));
			init_status[cnt]	 = (*( (uint16_t *)( INIT_STATE_ADD  + cnt * 2 ) ));	
			init_time[cnt]		 = (*( (uint16_t *)( INIT_TIME_ADD   + cnt * 2 ) ));		
			//dbgmsg("- %s, %s, %d, %s, %d\n",init_name,find_mode_str(init_operation[cnt]),init_number[cnt],find_state_str(init_status[cnt]),init_time[cnt]);
			
			init_crc = crc32_table[((init_crc >> 24) ^ ( (init_operation[cnt]>>8) & 0x00ff) ) ]	^ (init_crc << 8);
			init_crc = crc32_table[((init_crc >> 24) ^ ( (init_operation[cnt]>>0) & 0x00ff) ) ]	^ (init_crc << 8);
			init_crc = crc32_table[((init_crc >> 24) ^ ( (init_number[cnt]>>8) & 0x00ff) ) ]  	^ (init_crc << 8);
			init_crc = crc32_table[((init_crc >> 24) ^ ( (init_number[cnt]>>0) & 0x00ff) ) ]  	^ (init_crc << 8);
			init_crc = crc32_table[((init_crc >> 24) ^ ( (init_status[cnt]>>8) & 0x00ff) ) ]	^ (init_crc << 8);
			init_crc = crc32_table[((init_crc >> 24) ^ ( (init_status[cnt]>>0) & 0x00ff) ) ]  	^ (init_crc << 8);
			init_crc = crc32_table[((init_crc >> 24) ^ ( (init_time[cnt]>>8) & 0x00ff) ) ]  	^ (init_crc << 8);
			init_crc = crc32_table[((init_crc >> 24) ^ ( (init_time[cnt]>>0) & 0x00ff) ) ]  	^ (init_crc << 8);
		}	
		dbgmsg("init_crc %08lX\n",init_crc);
		//////////////////
		// mode1.csv update
		//dbgmsg("[mode1.csv] sdram update\n");	
		sprintf(mode1_name, "%s", (char *)(MODE1_NAME_ADD ));
		mode1_sequence = (*( (uint16_t *)MODE1_SEQ_ADD));
		if(mode1_sequence > 100)  mode1_sequence = 0;
		
		mode1_crc = set_crc32_init_value(CRC_INITIALVALUE);
		mode1_crc = crc32_table[((mode1_crc >> 24) ^ ( (mode1_sequence>>8) & 0x00ff) ) ]  ^ (mode1_crc << 8);
		mode1_crc = crc32_table[((mode1_crc >> 24) ^ ( (mode1_sequence>>0) & 0x00ff) ) ]  ^ (mode1_crc << 8);
		for(int i=0;i<strlen(mode1_name);i++)	
		{
			mode1_crc = crc32_table[((mode1_crc >> 24) ^ mode1_name[i])] ^ (mode1_crc << 8);
		}
		for( int cnt = 0; cnt < mode1_sequence; cnt++ )
		{
			mode1_operation[cnt] = (*( (uint16_t *)( MODE1_RUN_ADD    + cnt * 2 ) ));	
			mode1_number[cnt]	 = (*( (uint16_t *)( MODE1_NUBMER_ADD + cnt * 2 ) ));
			mode1_status[cnt]	 = (*( (uint16_t *)( MODE1_STATE_ADD  + cnt * 2 ) ));	
			mode1_time[cnt]		 = (*( (uint16_t *)( MODE1_TIME_ADD   + cnt * 2 ) ));
			//dbgmsg("- %s, %s, %d, %s, %d\n",mode1_name,find_mode_str(mode1_operation[cnt]),mode1_number[cnt],find_state_str(mode1_status[cnt]),mode1_time[cnt]);
			
			mode1_crc = crc32_table[((mode1_crc >> 24) ^ ( (mode1_operation[cnt]>>8) & 0x00ff) ) ]	^ (mode1_crc << 8);
			mode1_crc = crc32_table[((mode1_crc >> 24) ^ ( (mode1_operation[cnt]>>0) & 0x00ff) ) ]	^ (mode1_crc << 8);
			mode1_crc = crc32_table[((mode1_crc >> 24) ^ ( (mode1_number[cnt]>>8) & 0x00ff) ) ]  	^ (mode1_crc << 8);
			mode1_crc = crc32_table[((mode1_crc >> 24) ^ ( (mode1_number[cnt]>>0) & 0x00ff) ) ]  	^ (mode1_crc << 8);
			mode1_crc = crc32_table[((mode1_crc >> 24) ^ ( (mode1_status[cnt]>>8) & 0x00ff) ) ]		^ (mode1_crc << 8);
			mode1_crc = crc32_table[((mode1_crc >> 24) ^ ( (mode1_status[cnt]>>0) & 0x00ff) ) ]  	^ (mode1_crc << 8);
			mode1_crc = crc32_table[((mode1_crc >> 24) ^ ( (mode1_time[cnt]>>8) & 0x00ff) ) ]  		^ (mode1_crc << 8);
			mode1_crc = crc32_table[((mode1_crc >> 24) ^ ( (mode1_time[cnt]>>0) & 0x00ff) ) ]  		^ (mode1_crc << 8);
		}
		dbgmsg("mode1_crc %08lX\n",mode1_crc);
		//////////////////
		// mode2.csv update
		//dbgmsg("[mode2.csv] sdram update\n");	
		sprintf(mode2_name, "%s", (char *)(MODE2_NAME_ADD ));
		mode2_sequence = (*( (uint16_t *)MODE2_SEQ_ADD));
		if(mode2_sequence > 100)  mode2_sequence = 0;
		mode2_crc = set_crc32_init_value(CRC_INITIALVALUE);
		mode2_crc = crc32_table[((mode2_crc >> 24) ^ ( (mode2_sequence>>8) & 0x00ff) ) ]  ^ (mode2_crc << 8);
		mode2_crc = crc32_table[((mode2_crc >> 24) ^ ( (mode2_sequence>>0) & 0x00ff) ) ]  ^ (mode2_crc << 8);
		for(int i=0;i<strlen(mode2_name);i++)	
		{
			mode2_crc = crc32_table[((mode2_crc >> 24) ^ mode2_name[i])] ^ (mode2_crc << 8);
		}
		for( int cnt = 0; cnt < mode2_sequence; cnt++ )
		{
			mode2_operation[cnt] = (*( (uint16_t *)( MODE2_RUN_ADD    + cnt * 2 ) ));	
			mode2_number[cnt]	 = (*( (uint16_t *)( MODE2_NUBMER_ADD + cnt * 2 ) ));
			mode2_status[cnt]	 = (*( (uint16_t *)( MODE2_STATE_ADD  + cnt * 2 ) ));	
			mode2_time[cnt]		 = (*( (uint16_t *)( MODE2_TIME_ADD   + cnt * 2 ) ));
			//dbgmsg("- %s, %s, %d, %s, %d\n",mode2_name,find_mode_str(mode2_operation[cnt]),mode2_number[cnt],find_state_str(mode2_status[cnt]),mode2_time[cnt]);
			
			mode2_crc = crc32_table[((mode2_crc >> 24) ^ ( (mode2_operation[cnt]>>8) & 0x00ff) ) ]	^ (mode2_crc << 8);
			mode2_crc = crc32_table[((mode2_crc >> 24) ^ ( (mode2_operation[cnt]>>0) & 0x00ff) ) ]	^ (mode2_crc << 8);
			mode2_crc = crc32_table[((mode2_crc >> 24) ^ ( (mode2_number[cnt]>>8) & 0x00ff) ) ]  	^ (mode2_crc << 8);
			mode2_crc = crc32_table[((mode2_crc >> 24) ^ ( (mode2_number[cnt]>>0) & 0x00ff) ) ]  	^ (mode2_crc << 8);
			mode2_crc = crc32_table[((mode2_crc >> 24) ^ ( (mode2_status[cnt]>>8) & 0x00ff) ) ]		^ (mode2_crc << 8);
			mode2_crc = crc32_table[((mode2_crc >> 24) ^ ( (mode2_status[cnt]>>0) & 0x00ff) ) ]  	^ (mode2_crc << 8);
			mode2_crc = crc32_table[((mode2_crc >> 24) ^ ( (mode2_time[cnt]>>8) & 0x00ff) ) ]  		^ (mode2_crc << 8);
			mode2_crc = crc32_table[((mode2_crc >> 24) ^ ( (mode2_time[cnt]>>0) & 0x00ff) ) ]  		^ (mode2_crc << 8);
		}
		dbgmsg("mode2_crc %08lX\n",mode2_crc);
		//////////////////
		// mode3.csv update
		//dbgmsg("[mode3.csv] sdram update\n");	
		sprintf(mode3_name, "%s", (char *)(MODE3_NAME_ADD ));
		mode3_sequence = (*( (uint16_t *)MODE3_SEQ_ADD));
		if(mode3_sequence > 100)  mode3_sequence = 0;
		mode3_crc = set_crc32_init_value(CRC_INITIALVALUE);
		mode3_crc = crc32_table[((mode3_crc >> 24) ^ ( (mode3_sequence>>8) & 0x00ff) ) ]  ^ (mode3_crc << 8);
		mode3_crc = crc32_table[((mode3_crc >> 24) ^ ( (mode3_sequence>>0) & 0x00ff) ) ]  ^ (mode3_crc << 8);
		for(int i=0;i<strlen(mode3_name);i++)	
		{
			mode3_crc = crc32_table[((mode3_crc >> 24) ^ mode3_name[i])] ^ (mode3_crc << 8);
		}
		for( int cnt = 0; cnt < mode3_sequence; cnt++ )
		{
			mode3_operation[cnt] = (*( (uint16_t *)( MODE3_RUN_ADD    + cnt * 2 ) ));	
			mode3_number[cnt]	 = (*( (uint16_t *)( MODE3_NUBMER_ADD + cnt * 2 ) ));
			mode3_status[cnt]	 = (*( (uint16_t *)( MODE3_STATE_ADD  + cnt * 2 ) ));	
			mode3_time[cnt]		 = (*( (uint16_t *)( MODE3_TIME_ADD   + cnt * 2 ) ));
			//dbgmsg("- %s, %s, %d, %s, %d\n",mode3_name,find_mode_str(mode3_operation[cnt]),mode3_number[cnt],find_state_str(mode3_status[cnt]),mode3_time[cnt]);
			
			mode3_crc = crc32_table[((mode3_crc >> 24) ^ ( (mode3_operation[cnt]>>8) & 0x00ff) ) ]	^ (mode3_crc << 8);
			mode3_crc = crc32_table[((mode3_crc >> 24) ^ ( (mode3_operation[cnt]>>0) & 0x00ff) ) ]	^ (mode3_crc << 8);
			mode3_crc = crc32_table[((mode3_crc >> 24) ^ ( (mode3_number[cnt]>>8) & 0x00ff) ) ]  	^ (mode3_crc << 8);
			mode3_crc = crc32_table[((mode3_crc >> 24) ^ ( (mode3_number[cnt]>>0) & 0x00ff) ) ]  	^ (mode3_crc << 8);
			mode3_crc = crc32_table[((mode3_crc >> 24) ^ ( (mode3_status[cnt]>>8) & 0x00ff) ) ]		^ (mode3_crc << 8);
			mode3_crc = crc32_table[((mode3_crc >> 24) ^ ( (mode3_status[cnt]>>0) & 0x00ff) ) ]  	^ (mode3_crc << 8);
			mode3_crc = crc32_table[((mode3_crc >> 24) ^ ( (mode3_time[cnt]>>8) & 0x00ff) ) ]  		^ (mode3_crc << 8);
			mode3_crc = crc32_table[((mode3_crc >> 24) ^ ( (mode3_time[cnt]>>0) & 0x00ff) ) ]  		^ (mode3_crc << 8);
		}
		dbgmsg("mode3_crc %08lX\n",mode3_crc);
		//////////////////
		// mode4.csv update
		//dbgmsg("[mode4.csv] sdram update\n");	
		sprintf(mode4_name, "%s", (char *)(MODE4_NAME_ADD ));
		mode4_sequence = (*( (uint16_t *)MODE4_SEQ_ADD));
		if(mode4_sequence > 100)  mode4_sequence = 0;
		mode4_crc = set_crc32_init_value(CRC_INITIALVALUE);
		mode4_crc = crc32_table[((mode4_crc >> 24) ^ ( (mode4_sequence>>8) & 0x00ff) ) ]  ^ (mode4_crc << 8);
		mode4_crc = crc32_table[((mode4_crc >> 24) ^ ( (mode4_sequence>>0) & 0x00ff) ) ]  ^ (mode4_crc << 8);
		for(int i=0;i<strlen(mode4_name);i++)	
		{
			mode4_crc = crc32_table[((mode4_crc >> 24) ^ mode4_name[i])] ^ (mode4_crc << 8);
		}
		for( int cnt = 0; cnt < mode4_sequence; cnt++ )
		{
			mode4_operation[cnt] = (*( (uint16_t *)( MODE4_RUN_ADD    + cnt * 2 ) ));	
			mode4_number[cnt]	 = (*( (uint16_t *)( MODE4_NUBMER_ADD + cnt * 2 ) ));
			mode4_status[cnt]	 = (*( (uint16_t *)( MODE4_STATE_ADD  + cnt * 2 ) ));	
			mode4_time[cnt]		 = (*( (uint16_t *)( MODE4_TIME_ADD   + cnt * 2 ) ));
			//dbgmsg("- %s, %s, %d, %s, %d\n",mode4_name,find_mode_str(mode4_operation[cnt]),mode4_number[cnt],find_state_str(mode4_status[cnt]),mode4_time[cnt]);
			
			mode4_crc = crc32_table[((mode4_crc >> 24) ^ ( (mode4_operation[cnt]>>8) & 0x00ff) ) ]	^ (mode4_crc << 8);
			mode4_crc = crc32_table[((mode4_crc >> 24) ^ ( (mode4_operation[cnt]>>0) & 0x00ff) ) ]	^ (mode4_crc << 8);
			mode4_crc = crc32_table[((mode4_crc >> 24) ^ ( (mode4_number[cnt]>>8) & 0x00ff) ) ]  	^ (mode4_crc << 8);
			mode4_crc = crc32_table[((mode4_crc >> 24) ^ ( (mode4_number[cnt]>>0) & 0x00ff) ) ]  	^ (mode4_crc << 8);
			mode4_crc = crc32_table[((mode4_crc >> 24) ^ ( (mode4_status[cnt]>>8) & 0x00ff) ) ]		^ (mode4_crc << 8);
			mode4_crc = crc32_table[((mode4_crc >> 24) ^ ( (mode4_status[cnt]>>0) & 0x00ff) ) ]  	^ (mode4_crc << 8);
			mode4_crc = crc32_table[((mode4_crc >> 24) ^ ( (mode4_time[cnt]>>8) & 0x00ff) ) ]  		^ (mode4_crc << 8);
			mode4_crc = crc32_table[((mode4_crc >> 24) ^ ( (mode4_time[cnt]>>0) & 0x00ff) ) ]  		^ (mode4_crc << 8);
		}
		dbgmsg("mode4_crc %08lX\n",mode4_crc);
	}

	// short test count cal.
	short_tt_count = 0;
	for(int cnt = 0; cnt < (short_bs_count); cnt++)
	{
		for(int cnt1 = cnt+1; cnt1 < short_bs_count; cnt1++)
		{
			if(short_bs_group[cnt] != short_bs_group[cnt1])	break;   

			uint8_t skip_flag = false;
			for(int cnt2 = 0; cnt2 < short_ex_count; cnt2++)
			{
				if((short_bs_tp[cnt] == short_ex_tp1[cnt2]) && (short_bs_tp[cnt1] == short_ex_tp2[cnt2]) && (short_ex_range[cnt2] == 0xFF) )
				{
					skip_flag = true;
				}
				else if((short_bs_tp[cnt] == short_ex_tp2[cnt2]) && (short_bs_tp[cnt1] == short_ex_tp1[cnt2]) && (short_ex_range[cnt2] == 0xFF) )
				{
					skip_flag = true;
				}
				else if((short_bs_tp[cnt] == short_ex_tp1[cnt2]) && (short_bs_tp[cnt1] == short_ex_tp2[cnt2]) && (short_ex_range[cnt2] != 0xFF) )
				{
					break;
				}
				else if((short_bs_tp[cnt] == short_ex_tp2[cnt2]) && (short_bs_tp[cnt1] == short_ex_tp1[cnt2]) && (short_ex_range[cnt2] != 0xFF) )
				{
					break;
				}
			}
			if (skip_flag == true) continue;

			short_tt_count++;
		}
	}
	//dbgmsg("Project Main Name : %s\n",	 project_name_main);
	//dbgmsg("Project Sub  Name : %s\n",	 project_name_sub);
	//dbgmsg(" - tp_count       : %4d\n", tp_count);
	//dbgmsg(" - open_count     : %4d\n", open_count);
	//dbgmsg(" - short_count    : %4d\n", short_bs_count);
	//dbgmsg(" - short_tt_count : %4d\n", short_tt_count);
	//dbgmsg(" - short_ex_count : %4d\n", short_ex_count);
	//dbgmsg(" - part_count     : %4d\n", part_count);
	//dbgmsg(" - func_count     : %4d\n", func_count);
}

uint16_t findTpName(uint16_t Tp)
{
    for(uint16_t find_idx = 0; find_idx < tp_count; find_idx++)
    {
        if(Tp == tp_number[find_idx])   return find_idx;
    }
    return 0;
}

void UpdateGui(void)
{
	ProjectDisplay();
	ModeNameUpdate();	
	IctSpecDisp();
}
void IctSpecDisp(void)
{
	char text[100];    
	hItem = WM_GetDialogItem(hSyswin1,MENU1_ICT_ST_TEXT);
	TEXT_SetTextColor(hItem, GUI_WHITE);	
	TEXT_SetBkColor(hItem, GUI_DARKBLUE);	
	sprintf(text, " - TP(%d)\n - Open(%d)/Short bs(%d)/Short ex(%d)\n - Part(%d)/Func(%d)",tp_count,open_count,short_bs_count, short_ex_count, part_count, func_count);
	TEXT_SetText(hItem,text);
	GUI_Exec();
	
	dbgmsg(" - TP(%d)\n - Open(%d)/Short bs(%d)/Short ex(%d)\n - Part(%d)/Func(%d)\n",tp_count,open_count,short_bs_count, short_ex_count, part_count, func_count);
}
char* find_func_str(uint16_t fn)
{
		 if(fn == LSI)			return  "LSI";	
	else if(fn == STM)			return  "STM";	
	else if(fn == SYNA)			return  "SYNA";	
	else if(fn == ATMEL)		return  "ATMEL";
	else if(fn == SiW)			return  "SiW";
	else if(fn == MELFAS)		return  "MELFAS";	
	else if(fn == SPIFLASH)		return  "SPIFLASH";	
	else if(fn == MFS)			return  "MFS";	
	else if(fn == PMIC)			return  "PMIC";
	else if(fn == EEPROM)		return  "EEPROM";
	else if(fn == TPIC)			return  "TPIC";
	
	
	else					return  "None";	
}

uint16_t pasing_func(char *fn)
{		
		 if( strncmp(fn,"S6SY711",5) == 0 )	return LSI;	
	else if( strncmp(fn,"LSI_IC",5) == 0 )	return LSI;	
	else if( strncmp(fn,"STMICRO",5) == 0 )	return STM;	
	else if( strncmp(fn,"Synaptic",5) == 0 )return SYNA;
	else if( strncmp(fn,"ATMEL",5) == 0 )	return ATMEL;
	else if( strncmp(fn,"SiW",3) == 0 )		return SiW;
	else if( strncmp(fn,"MSS100",5) == 0 )	return MELFAS;
	else if( strncmp(fn,"FLASH", 5) == 0 )	return SPIFLASH;
	else if( strncmp(fn,"MFS_ISC",7) == 0 )	return MFS;
	else if( strncmp(fn,"PMIC",4) == 0 )    return PMIC;
	else if( strncmp(fn,"EEP",3) == 0 )     return EEPROM;	
	else if( strncmp(fn,"TPIC",4) == 0 )    return TPIC;
	else if( strncmp(fn,"LXS",4) == 0 )    return LXS;	
	
	else									return 0;
}

char* find_mode_str(uint16_t md)
{
	if(md == SOL_RUN)			return "SOL";
	else if(md == MARKING_UNIT)	return "MARKING";
	else if(md == WAIT_KEY) 	return "WAIT_KEY";	
	else if(md == LOG_SAVE) 	return "LOG_SAVE";	
	else if(md == RELAY)	 	return "RELAY";	
	else if(md == VCC)	 		return "VCC";	
	else if(md == TACT_TIME)	return "TACT_TIME";	
	else if(md == START_FLOW)	return "START_FLOW";	
	else if(md == END_FLOW)		return "END_FLOW";	
	else if(md == OPEN_TEST)	return "OPEN_TEST";	
	else if(md == SHORT_TEST)	return "SHORT_TEST";	
	else if(md == PART_TEST)	return "PART_TEST";	
	else if(md == FUNC_TEST)	return "FUNC_TEST";	
	else if(md == DISP_RESULT)	return "DISP_RESULT";	
	else if(md == CY_SEN_CHK)	return "CY_SEN_CHK";
	else if(md == SAFE_SEN_CHK)	return "SAFE_SEN";
	else if(md == AUTO_ALIGN)	return "AUTO_ALIGN";
	else						return "None";
}
uint16_t pasing_mode(char* op)
{	
	if(strncmp(op,"SOL_RUN",7)==0)				return SOL_RUN;	 		// SOL
	else if(strncmp(op,"MARKING_UNIT",11)==0)	return MARKING_UNIT;	// MAKING
	else if(strncmp(op,"WAIT_KEY",8)==0)		return WAIT_KEY; 		// WAIT_KEY
	else if(strncmp(op,"LOG_SAVE",8)==0)		return LOG_SAVE; 		// LOG
	else if(strncmp(op,"RELAY",5)==0)			return RELAY;	 		// RY
	else if(strncmp(op,"VCC",3)==0)				return VCC;	 			// END
	else if(strncmp(op,"TACT_TIME",9)==0)		return TACT_TIME;		// TIME
	else if(strncmp(op,"START_FLOW",10)==0)		return START_FLOW;		// START
	else if(strncmp(op,"END_FLOW",8)==0)		return END_FLOW;		// END
	else if(strncmp(op,"OPEN_TEST",9)==0)		return OPEN_TEST;		// OPEN
	else if(strncmp(op,"SHORT_TEST",10)==0)		return SHORT_TEST;		// SHORT
	else if(strncmp(op,"PART_TEST",9)==0)		return PART_TEST;		// PART
	else if(strncmp(op,"FUNC_TEST",9)==0)		return FUNC_TEST;		// FUNC
	else if(strncmp(op,"DISP_RESULT",11)==0)	return DISP_RESULT;		// DISP_RESULT
	else if(strncmp(op,"CYL_SEN",6)==0)			return CY_SEN_CHK;		// CY_SEN_CHK
	else if(strncmp(op,"SAFE_SEN",7)==0)		return SAFE_SEN_CHK;	// SAFE_SEN
	else if(strncmp(op,"AUTO_ALIGN",7)==0)		return AUTO_ALIGN;		// SAFE_SEN
	else										return 0;				// none
}


char* find_state_str(uint16_t st)
{
		 if( st == ON_STATE)	return "ON";
	else if( st == OFF_STATE)	return "OFF";
	else if( st == ST12_STATE)	return "ST12";
	else if( st == ST1_STATE)	return "ST1";
	else if( st == ST2_STATE)	return "ST2";
	else if( st == AUTO_STATE)	return "AUTO";
	else if( st == STEP_STATE)	return "STEP";
	else if( st == USB_STATE)	return "USB";
	else if( st == PC_STATE)	return "PC";
	else						return "None";
}
uint16_t pasing_state(char* st)
{	
		 if( strncmp(st,"ON", 2) == 0) 			return ON_STATE;
	else if( strncmp(st,"OFF", 3) == 0) 		return OFF_STATE;
	else if( strncmp(st,"ST1+ST2", 7) == 0) 	return ST12_STATE;
	else if( strncmp(st,"ST1", 3) == 0) 		return ST1_STATE;
	else if( strncmp(st,"ST2", 3) == 0) 		return ST2_STATE;
	else if( strncmp(st,"AUTO", 4) == 0) 		return AUTO_STATE;
	else if( strncmp(st,"STEP", 4) == 0) 		return STEP_STATE;
	else if( strncmp(st,"USB", 3) == 0) 		return USB_STATE;
	else if( strncmp(st,"PC", 2) == 0) 			return PC_STATE;
	else										return 0;		
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
const char *info_nm_path 	=  "0:/Spec/01.ICT_SPEC/01.project.csv";
const char *info_tp_path 	=  "0:/Spec/01.ICT_SPEC/02.tp.csv";
const char *info_op_path 	=  "0:/Spec/01.ICT_SPEC/03.open.csv";
const char *info_sb_path 	=  "0:/Spec/01.ICT_SPEC/04.short.csv";
const char *info_se_path 	=  "0:/Spec/01.ICT_SPEC/05.short_ex.csv";
const char *info_pt_path 	=  "0:/Spec/01.ICT_SPEC/06.part.csv";
const char *info_fn_path 	=  "0:/Spec/01.ICT_SPEC/07.func.csv";
const char *info_m1_path 	=  "0:/Spec/01.ICT_SPEC/Operation/01.Insp_mode1.csv";
const char *info_m2_path 	=  "0:/Spec/01.ICT_SPEC/Operation/02.Insp_mode2.csv";
const char *info_m3_path 	=  "0:/Spec/01.ICT_SPEC/Operation/03.Insp_mode3.csv";
const char *info_m4_path 	=  "0:/Spec/01.ICT_SPEC/Operation/04.Insp_mode4.csv";
const char *info_mi_path	=  "0:/Spec/01.ICT_SPEC/Operation/05.Insp_init.csv";
uint8_t check_ict_file(void)
{	
	uint8_t er_flag = 0;
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_nm_path, FA_READ) != FR_OK)	{	dbgmsg("miss project.csv\n"); 		er_flag++;	}	f_close(&file);
		if(f_open(&file, info_tp_path, FA_READ) != FR_OK)	{	dbgmsg("miss tp.csv\n"); 			er_flag++;	}	f_close(&file);
		if(f_open(&file, info_op_path, FA_READ) != FR_OK)	{	dbgmsg("miss open.csv\n"); 			er_flag++;	}	f_close(&file);
		if(f_open(&file, info_sb_path, FA_READ) != FR_OK)	{	dbgmsg("miss short.csv\n"); 		er_flag++;	}	f_close(&file);
		if(f_open(&file, info_se_path, FA_READ) != FR_OK)	{	dbgmsg("miss short_ex.csv\n"); 		er_flag++;	}	f_close(&file);
		if(f_open(&file, info_pt_path, FA_READ) != FR_OK)	{	dbgmsg("miss part.csv\n"); 			er_flag++;	}	f_close(&file);
		if(f_open(&file, info_fn_path, FA_READ) != FR_OK)	{	dbgmsg("miss func.csv\n"); 			er_flag++;	}	f_close(&file);
		if(f_open(&file, info_m1_path, FA_READ) != FR_OK)	{	dbgmsg("miss Insp_mode1.csv\n"); 	er_flag++;	}	f_close(&file);
		if(f_open(&file, info_m2_path, FA_READ) != FR_OK)	{	dbgmsg("miss Insp_mode2.csv\n"); 	er_flag++;	}	f_close(&file);
		if(f_open(&file, info_m3_path, FA_READ) != FR_OK)	{	dbgmsg("miss Insp_mode3.csv\n"); 	er_flag++;	}	f_close(&file);
		if(f_open(&file, info_m4_path, FA_READ) != FR_OK)	{	dbgmsg("miss Insp_mode4.csv\n"); 	er_flag++;	}	f_close(&file);
		if(f_open(&file, info_mi_path, FA_READ) != FR_OK)	{	dbgmsg("miss Insp_init.csv\n"); 	er_flag++;	}	f_close(&file);
				
		f_mount(0, NULL);
	
		if(er_flag == 0)	return true;
		else				return false;
	}
	else
	{
		return false;
	}
}

uint8_t ictspec_erase(void)
{
	dbgmsg("spec_erase : FLASH_Sector_20\n");
	FLASH_Unlock();
	FLASH_EraseSector(FLASH_Sector_20, VoltageRange_3);
	FLASH_Lock();	
	return true;
}
uint8_t Info_Save(void)
{	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_nm_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();			
			dbgmsg("Info_Save  : %s\n",info_nm_path);
						
			char Buff[256]={0};
			int spec_cnt=0;
			while(f_gets(Buff, 256, &file))
			{
				if(spec_cnt == 0)	// Project Main name
				{						
					strtok(Buff,","); 
					char* ptr_name = strtok(NULL,","); 
					//dbgmsg("[%s]%s\n",__func__,ptr_name);
					
					uint32_t pj_main_name_add = PJ_MAIN_NAME_ADD;
					uint32_t pj_main_name_cnt = strlen(ptr_name);
					uint32_t pj_main_name_max = 180;
											
					for(int i = 0; i < pj_main_name_max; i++)
					{
						if( i == pj_main_name_max -1 )
						{
							FLASH_ProgramByte( pj_main_name_add + i,  NULL );
						}
						else if( i < pj_main_name_cnt )
						{
							if( (ptr_name[i] != '\n') && (ptr_name[i] != '\r') )	{	FLASH_ProgramByte( pj_main_name_add + i,  ptr_name[i] );  		}
							else													{	FLASH_ProgramByte( pj_main_name_add + i,  NULL );  			}
						}
						else
						{
							FLASH_ProgramByte( pj_main_name_add + i,  NULL );
						}
					}
					spec_cnt++;
				}
				else if(spec_cnt == 1)	// Project Sub name
				{
					strtok(Buff,","); 
					char* ptr_name = strtok(NULL,","); 
					//dbgmsg("[%s]%s\n",__func__,ptr_name);
					
					uint32_t pj_sub_name_add = PJ_SUB_NAME_ADD;
					uint32_t pj_sub_name_cnt = strlen(ptr_name);
					uint32_t pj_sub_name_max = 180;
											
					for(int i = 0; i < pj_sub_name_max; i++)
					{
						if( i == pj_sub_name_max -1 )
						{
							FLASH_ProgramByte( pj_sub_name_add + i,  NULL );
						}
						else if( i < pj_sub_name_cnt )
						{
							if( (ptr_name[i] != '\n') && (ptr_name[i] != '\r') )	{	FLASH_ProgramByte( pj_sub_name_add + i,  ptr_name[i] );  	}
							else													{	FLASH_ProgramByte( pj_sub_name_add + i,  NULL );  		}
						}
						else
						{
							FLASH_ProgramByte( pj_sub_name_add + i,  NULL );
						}
					}	
							
					spec_cnt++;
				}
				else if(spec_cnt == 2)  //
				{
					// not used.
					//strtok(Buff,","); 
					//char* ptr_number1 = strtok(NULL,","); 
					//char* ptr_number2 = strtok(NULL,",");
					//FLASH_ProgramHalfWord( MUX_ON_P_ADD,  atoi(ptr_number1));	
					//FLASH_ProgramHalfWord( MUX_ON_N_ADD,  atoi(ptr_number2));	
					spec_cnt++;
				}
				else if(spec_cnt == 3)  // auto short pin
				{
					strtok(Buff,","); 
					char* ptr_number1 = strtok(NULL,","); 
					char* ptr_number2 = strtok(NULL,",");  
					//dbgmsg("[%s]%s-%s\n",__func__,ptr_number1,ptr_number2);
					
					FLASH_ProgramHalfWord( SH_AUTO_P_ADD,  atoi(ptr_number1));	
					FLASH_ProgramHalfWord( SH_AUTO_N_ADD,  atoi(ptr_number2));	

					spec_cnt++;
				}
			}
			
			FLASH_Lock();
		}	
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&file);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}


uint8_t Tp_Save(void)
{	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_tp_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			dbgmsg("Tp_Save    : %s\n",info_tp_path);
		
			char Buff[256]={0};
			int spec_cnt=0;
			int cnt=0;

			while(f_gets(Buff, 256, &file))
			{	
				if(spec_cnt == 0) 	//test point label read
				{
					spec_cnt++;
				}
				else
				{
					cnt++;				
					if(cnt > 720) break;
					
					strtok(Buff,","); 
					char* ptr_name   = strtok(NULL,","); 
					char* ptr_number = strtok(NULL,","); 
					//dbgmsg("[%s]%s, %s\n", __func__, ptr_name, ptr_number);
					
					// tp name 
					uint32_t tp_name_add = TEST_POINT_NAME_ADD + (cnt - 1) * 32;
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
					uint32_t tp_number_add = TEST_POINT_NUMBER_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( tp_number_add,  atoi(ptr_number));				
				}
			}
			
			if(cnt <= 720)	FLASH_ProgramHalfWord(TEST_POINT_TOTAL_ADD, cnt);
			else			FLASH_ProgramHalfWord(TEST_POINT_TOTAL_ADD, 720);
			
		
			FLASH_Lock();
		}	
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&file);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}

uint8_t Open_Save(void)
{	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_op_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			dbgmsg("Open_Save  : %s\n",info_op_path);	
			
			char Buff[256]={0};
			int spec_cnt=0;
			int cnt=0;

			while(f_gets(Buff, 256, &file))
			{	
				if(spec_cnt == 0) 	//test point label read
				{
					spec_cnt++;
				}
				else
				{
					cnt++;
					if(cnt > 720) break;
					strtok(Buff,","); 
					char* ptr_tp1  = strtok(NULL,","); 
					char* ptr_tp2  = strtok(NULL,","); 
					char* ptr_rg   = strtok(NULL,","); 
					char* ptr_spec = strtok(NULL,","); 
					char* ptr_ok   = strtok(NULL,","); 
					char* ptr_ng   = strtok(NULL,","); 
					
					//dbgmsg("[%s]%s, %s, %s, %s, %s, %s\n", __func__, ptr_tp1, ptr_tp2, ptr_rg, ptr_spec, ptr_ok, ptr_ng);
					
					// open tp1
					uint32_t open_tp1_add = OPEN_TESTPOINT1_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( open_tp1_add,  atoi(ptr_tp1));
					
					// open tp2
					uint32_t open_tp2_add = OPEN_TESTPOINT2_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( open_tp2_add,  atoi(ptr_tp2));
					
					// open range
					uint32_t open_range_add = OPEN_TEST_RANGE_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( open_range_add,  atoi(ptr_rg));
					
					// open spec
					uint32_t open_spec_add = OPEN_TEST_SPEC_ADD + (cnt - 1) * 4;		
					float spec_temp = atof(ptr_spec);
					FLASH_ProgramWord( open_spec_add,  *(uint32_t *)&(spec_temp) );
					
					// open ok
					uint32_t open_ok_add = OPEN_TEST_OK_COUNT_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( open_ok_add,  atoi(ptr_ok));
					
					// open ng
					uint32_t open_ng_add = OPEN_TEST_NG_COUNT_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( open_ng_add,  atoi(ptr_ng));				
				}
			}
			if(cnt <= 720)	FLASH_ProgramHalfWord(OPEN_TOTAL_ADD, cnt);
			else			FLASH_ProgramHalfWord(OPEN_TOTAL_ADD, 720);
		
		
			FLASH_Lock();	
		}	
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&file);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}

uint8_t Short_bs_Save(void)
{	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_sb_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			dbgmsg("Shortbs_sv : %s\n",info_sb_path);
				
			char Buff[256]={0};
			int spec_cnt=0;
			int cnt=0;

			while(f_gets(Buff, 256, &file))
			{	
				if(spec_cnt == 0) 	//test point label read
				{
					spec_cnt++;
				}
				else if(spec_cnt == 1) 	//first line(tp,gr,range,spec,okcnt,ngcnt)
				{
					cnt++;
					if(cnt > 720) break;
					strtok(Buff,","); 
					char* ptr_tp   = strtok(NULL,","); 
					char* ptr_gr   = strtok(NULL,","); 
					char* ptr_rg   = strtok(NULL,","); 
					char* ptr_spec = strtok(NULL,","); 
					char* ptr_ok   = strtok(NULL,","); 
					char* ptr_ng   = strtok(NULL,","); 
					//dbgmsg("[%s]%s, %s, %s, %s, %s, %s\n", __func__, ptr_tp, ptr_gr, ptr_rg, ptr_spec, ptr_ok, ptr_ng);				
					// short tp
					uint32_t short_tp_add = SHORT_TEST_POINT_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( short_tp_add,  atoi(ptr_tp));					
					// short group
					uint32_t short_group_add = SHORT_TEST_GROUP_ADD + (cnt - 1) * 2;
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

					spec_cnt++;
				}
				else//other line(tp,gr)
				{
					cnt++;
					strtok(Buff,","); 
					char* ptr_tp  = strtok(NULL,","); 
					char* ptr_gr   = strtok(NULL,","); 
					//dbgmsg("[%s]%s, %s\n", __func__, ptr_tp, ptr_gr);
					// short tp
					uint32_t short_tp_add = SHORT_TEST_POINT_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( short_tp_add,  atoi(ptr_tp));					
					// short group
					uint32_t short_group_add = SHORT_TEST_GROUP_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( short_group_add,  atoi(ptr_gr));
				}
			}			
			if(cnt <= 720)	FLASH_ProgramHalfWord(SHORT_TOTAL_ADD, cnt);
			else			FLASH_ProgramHalfWord(SHORT_TOTAL_ADD, 720);
			
			FLASH_Lock();
		}	
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&file);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}

uint8_t Short_ex_save(void)
{	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_se_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			dbgmsg("Shortex_sv : %s\n",info_se_path);
			
			char Buff[256]={0};
			int spec_cnt=0;
			int cnt=0;

			while(f_gets(Buff, 256, &file))
			{	
				if(spec_cnt == 0) 	//test point label read
				{
					spec_cnt++;
				}
				else
				{
					cnt++;
					if(cnt > 720) break;
					strtok(Buff,","); 
					char* ptr_tp1  = strtok(NULL,","); 
					char* ptr_tp2  = strtok(NULL,","); 
					char* ptr_rg   = strtok(NULL,","); 
					char* ptr_spec = strtok(NULL,","); 
					char* ptr_ok   = strtok(NULL,","); 
					char* ptr_ng   = strtok(NULL,","); 
					//dbgmsg("[%s]%s, %s, %s, %s, %s, %s\n", __func__, ptr_tp1, ptr_tp2, ptr_rg, ptr_spec, ptr_ok, ptr_ng);
						
					// short ex tp1
					uint32_t shex_tp1_add = SHEX_TESTPOINT1_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( shex_tp1_add,  atoi(ptr_tp1));					
					// short ex tp2
					uint32_t shex_tp2_add = SHEX_TESTPOINT2_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( shex_tp2_add,  atoi(ptr_tp2));					
					// short ex range
					uint32_t shex_range_add = SHEX_TEST_RANGE_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( shex_range_add,  atoi(ptr_rg));					
					// short ex spec
					uint32_t shex_spec_add = SHEX_TEST_SPEC_ADD + (cnt - 1) * 4;		
					float spec_temp = atof(ptr_spec);
					FLASH_ProgramWord( shex_spec_add,  *(uint32_t *)&(spec_temp) );					
					// short ex ok
					uint32_t shex_ok_add = SHEX_TEST_OK_COUNT_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( shex_ok_add,  atoi(ptr_ok));					
					// short ex ng
					uint32_t shex_ng_add = SHEX_TEST_NG_COUNT_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( shex_ng_add,  atoi(ptr_ng));
				}
			}			
			if(cnt <= 720)	FLASH_ProgramHalfWord(SHEX_TOTAL_ADD, cnt);
			else			FLASH_ProgramHalfWord(SHEX_TOTAL_ADD, 720);			
			
			FLASH_Lock();
		}	
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&file);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}
uint8_t Part_Save(void)
{	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_pt_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			dbgmsg("Part_Save  : %s\n",info_pt_path); 
			
			char Buff[256]={0};
			int spec_cnt=0;
			int cnt=0;

			while(f_gets(Buff, 256, &file))
			{	
				if(spec_cnt == 0) 	//test point label read
				{
					spec_cnt++;
				}
				else
				{
					cnt++;
					if(cnt > 720) break;
					strtok(Buff,","); 
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
					uint32_t part_name_add = PART_TEST_NAME_ADD + (cnt - 1) * 32;
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
					uint32_t part_tp1_add = PART_TESTPOINT1_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( part_tp1_add,  atoi(ptr_tp1));
					// part tp2
					uint32_t part_tp2_add = PART_TESTPOINT2_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( part_tp2_add,  atoi(ptr_tp2));
					// part range
					uint32_t part_range_add = PART_TEST_RANGE_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( part_range_add,  atoi(ptr_rg));
					// part upper
					uint32_t part_upper_add = PART_TEST_UPPER_ADD + (cnt - 1) * 4;		
					float up_temp = atof(ptr_hi);
					FLASH_ProgramWord( part_upper_add,  *(uint32_t *)&(up_temp) );
					// part lower
					uint32_t part_lower_add = PART_TEST_LOWER_ADD + (cnt - 1) * 4;		
					float low_temp = atof(ptr_lo);
					FLASH_ProgramWord( part_lower_add,  *(uint32_t *)&(low_temp) );
					// part ok
					uint32_t part_ok_add = PART_TEST_OK_COUNT_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( part_ok_add,  atoi(ptr_ok));
					// part ng
					uint32_t part_ng_add = PART_TEST_NG_COUNT_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( part_ng_add,  atoi(ptr_ng));				
				}
			}
			if(cnt <= 720)	FLASH_ProgramHalfWord(PART_TOTAL_ADD, cnt);
			else			FLASH_ProgramHalfWord(PART_TOTAL_ADD, 720);
		
		
			FLASH_Lock();
		}	
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&file);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}
uint8_t Func_Save(void)
{	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_fn_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			dbgmsg("Func_Save  : %s\n",info_fn_path);
	
			char Buff[256]={0};
			int spec_cnt=0;
			int cnt=0;

			while(f_gets(Buff, 256, &file))
			{	
				if(spec_cnt == 0) 	//test point label read
				{
					spec_cnt++;
				}
				else
				{
					cnt++;
					if(cnt > 180) break;
					strtok(Buff,","); 
					char* ptr_name  		= strtok(NULL,","); 
					char* ptr_type  		= strtok(NULL,","); 
					char* ptr_mode  		= strtok(NULL,","); 
					char* ptr_log   		= strtok(NULL,","); 
					char* ptr_panel_link 	= strtok(NULL,","); 
					char* ptr_spec_dec   	= strtok(NULL,","); 
					char* ptr_spec_hex   	= strtok(NULL,","); 
					char* ptr_spec_float 	= strtok(NULL,","); 
					char* ptr_spec_ascii 	= strtok(NULL,",");
					//dbgmsg("[%s]%s, %s, %s, %s, %s, %s, %s, %s\n", __func__, ptr_name, ptr_type, ptr_mode, ptr_log, ptr_panel_link, ptr_spec_dec,ptr_spec_hex,ptr_spec_float,ptr_spec_ascii);
					
					/////////////// func name	/////////////
					uint32_t func_name_add = FUNC_TEST_NAME_ADD + (cnt - 1) * 32;
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
					uint32_t func_type_add = FUNC_TEST_TYPE_ADD + (cnt - 1) * 2;				
					uint16_t type = pasing_func(ptr_type);
					FLASH_ProgramHalfWord( func_type_add,  type);
					
					/////////////// func module	/////////////
					uint32_t func_module_add = FUNC_TEST_MODE_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( func_module_add,  atoi(ptr_mode));
																		
					/////////////// func dec spec	/////////////
					uint32_t func_spec1_add = FUNC_TEST_SPEC1_ADD + (cnt - 1) * 2;	
					FLASH_ProgramHalfWord( func_spec1_add,  atoi(ptr_spec_dec) );
													
					/////////////// func hex spec /////////////
					uint32_t func_spec2_add = FUNC_TEST_SPEC2_ADD + (cnt - 1) * 4;					
					char* ptr1 = strtok(ptr_spec_hex,"."); 	FLASH_ProgramByte(func_spec2_add + 0, strtol(ptr1, NULL, 16)); 	//
					char* ptr2 = strtok(NULL,"."); 			FLASH_ProgramByte(func_spec2_add + 1, strtol(ptr2, NULL, 16)); 	//
					char* ptr3 = strtok(NULL,"."); 			FLASH_ProgramByte(func_spec2_add + 2, strtol(ptr3, NULL, 16)); 	//
					char* ptr4 = strtok(NULL,","); 			FLASH_ProgramByte(func_spec2_add + 3, strtol(ptr4, NULL, 16)); 	//
									
					/////////////// func float spec	/////////////
					uint32_t func_spec3_add = FUNC_TEST_SPEC3_ADD + (cnt - 1) * 4;		
					float spec3_temp = atof(ptr_spec_float);
					FLASH_ProgramWord( func_spec3_add,  *(uint32_t *)&(spec3_temp) );
					
					/////////////// func ascii spec	/////////////
					uint32_t func_spec4_add = FUNC_TEST_SPEC4_ADD + (cnt - 1) * 16;
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
					uint32_t func_log_add = FUNC_TEST_LOG_ADD + (cnt - 1) * 2;
					if( strncmp(ptr_log,"OK",2) == 0 ) 		FLASH_ProgramHalfWord( func_log_add, true);
					else									FLASH_ProgramHalfWord( func_log_add, false);	
					
					/////////////// func panel link	/////////////
					uint32_t func_panellink_add = FUNC_TEST_PANELLINK_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( func_panellink_add,  atoi(ptr_panel_link));
				}
			}
			if(cnt <= 180)	FLASH_ProgramHalfWord(FUNC_TOTAL_ADD, cnt);
			else			FLASH_ProgramHalfWord(FUNC_TOTAL_ADD, 180);
		
		
			FLASH_Lock();
		}	
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&file);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}
uint8_t mode_Savei(void)
{	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_mi_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			dbgmsg("mode_Savei : %s\n",info_mi_path);
								
			char Buff[256]={0};
			int spec_cnt=0;
			int cnt=0;

			while(f_gets(Buff, 256, &file))
			{	
				if(spec_cnt == 0) 	//test point label read
				{
					char*    name 	  = strtok(Buff,","); 	
					
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
									
					spec_cnt++;
				}
				else if(spec_cnt == 1)
				{
					spec_cnt++;
				}
				else
				{
					cnt++;
					if(cnt > 100) break;
					char* sq = strtok(Buff,",");
					char* op = strtok(NULL,",");
					char* st = strtok(NULL,",");
					char* nm = strtok(NULL,",");
					char* tm = strtok(NULL,",");
					//dbgmsg("[%s]%s, %s, %s, %s, %s\n", __func__, sq, op, st, nm, tm);
					
					uint16_t mode = pasing_mode(op);					
					uint32_t add = INIT_RUN_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  mode);
														
					add = INIT_NUBMER_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  atoi(nm));

					uint16_t state = pasing_state(st);
					add = INIT_STATE_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  state);
					
					add = INIT_TIME_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  atoi(tm));			
				}
			}
			if(cnt <= 100)	FLASH_ProgramHalfWord(INIT_SEQ_ADD, cnt);
			else			FLASH_ProgramHalfWord(INIT_SEQ_ADD, 100);

			FLASH_Lock();
		}	
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&file);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}
uint8_t mode_Save1(void)
{	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_m1_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			dbgmsg("mode_Save1 : %s\n",info_m1_path);
		
			char Buff[256]={0};
			int spec_cnt=0;
			int cnt=0;

			while(f_gets(Buff, 256, &file))
			{	
				if(spec_cnt == 0) 	//test point label read
				{
					char*    name 	  = strtok(Buff,","); 	
					
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
									
					spec_cnt++;
				}
				else if(spec_cnt == 1)
				{
					spec_cnt++;
				}
				else
				{
					cnt++;
					if(cnt > 100) break;
					char* sq = strtok(Buff,",");
					char* op = strtok(NULL,",");
					char* st = strtok(NULL,",");
					char* nm = strtok(NULL,",");
					char* tm = strtok(NULL,",");
					//dbgmsg("[%s]%s, %s, %s, %s, %s\n", __func__, sq, op, st, nm, tm);
					
					uint16_t mode = pasing_mode(op);					
					uint32_t add = MODE1_RUN_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  mode);
														
					add = MODE1_NUBMER_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  atoi(nm));

					uint16_t state = pasing_state(st);
					add = MODE1_STATE_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  state);
					
					add = MODE1_TIME_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  atoi(tm));			
				}
			}
			if(cnt <= 100)	FLASH_ProgramHalfWord(MODE1_SEQ_ADD, cnt);
			else			FLASH_ProgramHalfWord(MODE1_SEQ_ADD, 100);
			
			FLASH_Lock();
		}	
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&file);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}
uint8_t mode_Save2(void)
{	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_m2_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			dbgmsg("mode_Save2 : %s\n",info_m2_path);
					
			char Buff[256]={0};
			int spec_cnt=0;
			int cnt=0;

			while(f_gets(Buff, 256, &file))
			{	
				if(spec_cnt == 0) 	//test point label read
				{
					char*    name 	  = strtok(Buff,","); 	
					
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
									
					spec_cnt++;
				}
				else if(spec_cnt == 1)
				{
					spec_cnt++;
				}
				else
				{
					cnt++;
					if(cnt > 100) break;
					char* sq = strtok(Buff,",");
					char* op = strtok(NULL,",");
					char* st = strtok(NULL,",");
					char* nm = strtok(NULL,",");
					char* tm = strtok(NULL,",");
					//dbgmsg("[%s]%s, %s, %s, %s, %s\n", __func__, sq, op, st, nm, tm);
					
					uint16_t mode = pasing_mode(op);					
					uint32_t add = MODE2_RUN_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  mode);
														
					add = MODE2_NUBMER_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  atoi(nm));

					uint16_t state = pasing_state(st);
					add = MODE2_STATE_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  state);
					
					add = MODE2_TIME_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  atoi(tm));			
				}
			}
			if(cnt <= 100)	FLASH_ProgramHalfWord(MODE2_SEQ_ADD, cnt);
			else			FLASH_ProgramHalfWord(MODE2_SEQ_ADD, 100);
			
			FLASH_Lock();
		}	
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&file);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}
uint8_t mode_Save3(void)
{	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_m3_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			dbgmsg("mode_Save3 : %s\n",info_m3_path);
								
			char Buff[256]={0};
			int spec_cnt=0;
			int cnt=0;

			while(f_gets(Buff, 256, &file))
			{	
				if(spec_cnt == 0) 	//test point label read
				{
					char*    name 	  = strtok(Buff,","); 	
					
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
									
					spec_cnt++;
				}
				else if(spec_cnt == 1)
				{
					spec_cnt++;
				}
				else
				{
					cnt++;
					if(cnt > 100) break;
					char* sq = strtok(Buff,",");
					char* op = strtok(NULL,",");
					char* st = strtok(NULL,",");
					char* nm = strtok(NULL,",");
					char* tm = strtok(NULL,",");
					//dbgmsg("[%s]%s, %s, %s, %s, %s\n", __func__, sq, op, st, nm, tm);
					
					uint16_t mode = pasing_mode(op);					
					uint32_t add = MODE3_RUN_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  mode);
														
					add = MODE3_NUBMER_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  atoi(nm));

					uint16_t state = pasing_state(st);
					add = MODE3_STATE_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  state);
					
					add = MODE3_TIME_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  atoi(tm));			
				}
			}
			if(cnt <= 100)	FLASH_ProgramHalfWord(MODE3_SEQ_ADD, cnt);
			else			FLASH_ProgramHalfWord(MODE3_SEQ_ADD, 100);
			
			FLASH_Lock();
		}	
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&file);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}
uint8_t mode_Save4(void)
{	
	if ( f_mount( 0, &fatfs ) == FR_OK )
	{
		if(f_open(&file, info_m4_path, FA_READ) == FR_OK)
		{
			FLASH_Unlock();
			dbgmsg("mode_Save4 : %s\n",info_m4_path);
								
			char Buff[256]={0};
			int spec_cnt=0;
			int cnt=0;

			while(f_gets(Buff, 256, &file))
			{	
				if(spec_cnt == 0) 	//test point label read
				{
					char*    name 	  = strtok(Buff,","); 	
					
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
									
					spec_cnt++;
				}
				else if(spec_cnt == 1)
				{
					spec_cnt++;
				}
				else
				{
					cnt++;
					if(cnt > 100) break;
					char* sq = strtok(Buff,",");
					char* op = strtok(NULL,",");
					char* st = strtok(NULL,",");
					char* nm = strtok(NULL,",");
					char* tm = strtok(NULL,",");
					//dbgmsg("[%s]%s, %s, %s, %s, %s\n", __func__, sq, op, st, nm, tm);
					
					uint16_t mode = pasing_mode(op);					
					uint32_t add = MODE4_RUN_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  mode);
														
					add = MODE4_NUBMER_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  atoi(nm));

					uint16_t state = pasing_state(st);
					add = MODE4_STATE_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  state);
					
					add = MODE4_TIME_ADD + (cnt - 1) * 2;
					FLASH_ProgramHalfWord( add,  atoi(tm));			
				}
			}
			if(cnt <= 100)	FLASH_ProgramHalfWord(MODE4_SEQ_ADD, cnt);
			else			FLASH_ProgramHalfWord(MODE4_SEQ_ADD, 100);
			
			FLASH_Lock();
		}	
		else
		{
			f_mount(0, NULL);
			return false;
		}
		f_close(&file);
	}
	else
	{
		return false;
	}
	f_mount(0, NULL);

	return true;
}


uint8_t Spec_Make(void)
{
	FLASH_Unlock();
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);		
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);			
	
	FLASH_ProgramWord(SPEC_SAVE_YEAR_ADD, 	RTC_DateStr.RTC_Year  & 0xff );	// SPEC file down year 
	FLASH_ProgramWord(SPEC_SAVE_MONTH_ADD, 	RTC_DateStr.RTC_Month & 0xff );	// SPEC file down month
	FLASH_ProgramWord(SPEC_SAVE_DAY_ADD, 	RTC_DateStr.RTC_Date  & 0xff );	// SPEC file down date
		
	FLASH_ProgramWord(SPEC_SAVE_HOUR_ADD, 	RTC_TimeStr.RTC_Hours  & 0xff); // SPEC file down year 
	FLASH_ProgramWord(SPEC_SAVE_MIN_ADD, 	RTC_TimeStr.RTC_Minutes & 0xff);// SPEC file down month
	
	FLASH_ProgramWord(SPEC_SAVE_FLAG_ADD, SPEC_CHECK_FLAG );	// SPEC file down date
	
	dbgmsg("20%02d-%02d-%02d, %02d:%02d[%X] ICT Spec Save OK!\n",SPEC_SAVE_YEAR,SPEC_SAVE_MONTH,SPEC_SAVE_DAY,SPEC_SAVE_HOUR,SPEC_SAVE_MIN,SPEC_SAVE_FLAG);
	
	FLASH_Lock();
	
	return true;
}
void IctSpecSaveflow(void)
{	
	if(usb_host_connected_flag == 0)
	{
		Sys1Popup(true, GUI_DARKRED,"USB Unmount\n(Press ESC KEY)");
		Beep(250);
	}
	else
	{
		Sys1Popup(true, GUI_DARKBLUE, "USB Mount");
		Beep(250);
		
		if(check_ict_file() == true)
		{
			if(ictspec_erase()==false){	goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "ICT Spec Area\nErase OK");		
			if(Info_Save()==false)	  {	goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Information\nUpdate OK");		
			if(Tp_Save()==false)	  {	goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Test Point\nUpdate OK");			
			if(Open_Save()==false)	  {	goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Open Spec\nUpdate OK");			
			if(Short_bs_Save()==false){	goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Short Base Spec\nUpdate OK");	
			if(Short_ex_save()==false){	goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Short Extend Spec\nUpdate OK");	
			if(Part_Save()==false)	  {	goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Part Spec\nUpdate OK");			
			if(Func_Save()==false)	  {	goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Function Spec\nUpdate OK");		
			if(mode_Savei()==false)	  {	goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Inspector Init\nUpdate OK");		
			if(mode_Save1()==false)	  {	goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Inspector Mode1\nUpdate OK");	
			if(mode_Save2()==false)	  {	goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Inspector Mode2\nUpdate OK");	
			if(mode_Save3()==false)	  {	goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Inspector Mode3\nUpdate OK");	
			if(mode_Save4()==false)	  {	goto errflow; }; Beep(1);	Sys1Popup(true, GUI_DARKBLUE, "Inspector Mode4\nUpdate OK");	
			
			Spec_Make();		
			
			Sys1Popup(true, GUI_DARKBLUE, "ICT Spec\nUpdate finsh");
			Beep(250);
		}
		else
		{
			errflow:
			ictspec_erase();
			
			Sys1Popup(true, GUI_DARKRED, "File Error\n(Press ESC KEY)");
			BUZZER(true); 
			EscWait();
			BUZZER(false);			
		}		
	}
	
	UpdateIctSpecSdram();
	UpdateGui();
	
	Delay_ms(100);
	Sys1Popup(false, GUI_DARKRED, "");
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
