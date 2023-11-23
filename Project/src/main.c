#include "main.h" 

float sys_ver = 1.01;
//221107 Safety Sensor Autonics -> KI Bar
//       Range 3 하한치 Display 수정 10이하시 0 -> 5이사시 0

void StmHalInit(void)
{	
	RTC_Initial();
	//////////////////
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);		
	SysTick_Config(SystemCoreClock / 1000000);
	
	//////////////////
	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host, &USBH_MSC_cb,&USR_cb);
	//////////////////
	NVIC_Initial();
	//////////////////
	Usart1_Initial(460800);	// 460800, 921600
	Usart2_Initial(230400);	// 460800, 921600
	Usart3_Initial(230400);	// debug port(FIX)
	//////////////////
	Sdram_Initial();		// SRAM initial(TFT LCD)
	GUI_Init();         	// STemwin initial(TFT LCD GUI LIB)
	//////////////////
	Sys_Timer2();			// Delay Timer2 initial(us)
	Sys_Timer3(true);		// Cap   Timer3 initial
	//Sys_Timer4();			// Back  Timer3 initial(ms)
	//////////////////
	GPIO_Initial();			// GPIO init
	CapDacInit();			// Cap DAC
	AdcInit();				// 
	//////////////////
	SPI2_Init();			// SPI2 Init
	LCDTouch_Init();		// Touch IC
	MCP23S17_Init1(); 		// GPIO Expand1(SW4,LED2,BEEP,EVCC CHECK RY,SOL8)
	MCP23S17_Init2(); 		// GPIO Expand2(FN RY8, VCC3, IO4)
	MCP23S17_Init3(); 		// GPIO Expand3(SEN IN8, IO8)
	MCP23S17_Init4(); 	// GPIO Expand4(IO8, IO8)
	////////////////// 
}

int main(void)
{
  //////////////////////////////////////////////////////
	StmHalInit();
  Delay_ms(2500);  
	BoardSerialRead();		// board serial read
	dbgmsg("\n\n\nKM Board(%08lX)\n",serial_num);
	//////////////////////////////////////////////////////	
	MuxInit();
	dbgmsg("[MuxInit] OK\n");
	MuxOnRes();
	dbgmsg("[MuxOnRes] OK\n");
  
	//////////////////////////////////////////////////////
	UpdateIctSpecSdram();	// ict   spec read(flash -> sdram)
	dbgmsg("[UpdateIctSpecSdram] OK\n");
	UpdatePanelSpecSdram();	// panel spec raed(flash -> sdram)
	dbgmsg("[UpdatePanelSpecSdram] OK\n");
	UpdateDatafile();		// data  file read(flash -> sdram)	
	dbgmsg("[UpdateDatafile] OK\n");
	//////////////////////////////////////////////////////
	Updatepmicfile();
	Updateeepfile();
	//5Updatetpicfile();
	//--------------------------------------
	//LogoDisplay( &KI_346_059, 500 );
	dbgmsg("[LogoDisplay] OK\n");	
	ProjectDisplay();
	dbgmsg("[ProjectDisplay] OK\n");
	LcdDisplayInit();
	dbgmsg("[LcdDisplayInit] OK\n");
	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////	
	if(RD_DBG_MD == 0)		dbgmsg("KM Board V%1.2f(D)\nInsp START\n",sys_ver);
	else					dbgmsg("KM Board V%1.2f(R)\nInsp START\n",sys_ver);
	//////////////////////////////////////////////////////	
	WR_BOOT(APP_FLAG);		
	//////////////////////////////////////////////////////	
	//////////////////////////////////////////////////////

	Beep(250);
	
	olb_led_control(1,RD_LED_CH1);		Delay_ms(5);
	olb_led_control(2,RD_LED_CH2);		Delay_ms(5);
	olb_led_control(3,RD_LED_CH3);		Delay_ms(5);
	olb_led_control(4,RD_LED_CH4);		Delay_ms(5);
	
	pc_rx_enable = true;
	
	
	//////////////////
	SPI2_Init();			// SPI2 Init
	LCDTouch_Init();		// Touch IC
	MCP23S17_Init1(); 		// GPIO Expand1(SW4,LED2,BEEP,EVCC CHECK RY,SOL8)
	MCP23S17_Init2(); 		// GPIO Expand2(FN RY8, VCC3, IO4)
	MCP23S17_Init3(); 		// GPIO Expand3(SEN IN8, IO8)
	MCP23S17_Init4(); 		// GPIO Expand4(IO8, IO8)
	////////////////// 
		
	operation(init_sequence, init_operation, init_number, init_status, init_time);
	OKLED(false);NGLED(false);
      
	MuxInit();
	dbgmsg("[MuxInit] OK\n");
	MuxOnRes();
	dbgmsg("[MuxOnRes] OK\n"); 
	MuxInit();
  
  //----------------------------221108 MCH
   EvccControl(1,false);
   EvccControl(2,true);
   FuncRYControl(255,false,100);
   RYBD_1ON(false);
   RYBD_2ON(false);
   RYBD_3ON(false);
   RYBD_4ON(false);
   RYBD_5ON(false);
   RYBD_6ON(false);
   AVDD_ON(true);
   DVDD_ON(true);  	
  
  Delay_ms(100);
  
	while(1)
	{
		if(is_pc_connected_flag     == false)	IsPcCheck();
		if(usb_host_connected_flag  == false)	USBH_Process(&USB_OTG_Core, &USB_Host);
				
		uint8_t SAFE_RET = SafeSensorCheck();		
		uint8_t KET_RET = KeyScan();	
		//uint8_t SEN_RET = Sen_Scan();

		if(KET_RET == 0x00)
		{
			//KET_RET = Check_Short_auto();
		}
		
		if(is_pc_connected_flag == false)	// 단품검사
		{			
			switch ( KET_RET )
			{	
				case ATS_ON:			TestStart();					break;
				case ST1_ON|ST2_ON:		TestStart();					break;
				case MOD_ON|ST1_ON:		InspModeChange(RD_MODE);		break;
				case MOD_ON|ST2_ON:		MenuChange();					break;
				case MOD_ON|ESC_ON:		InspCountReset();				break;
				//////////////////////////////////		
				case ST1_ON:			TestResultDisp(1,false);		break;
				case ST2_ON:			TestResultDisp(0,false);		break;
				case ESC_ON:			TestResultDisp(2,false);		break;
				case MOD_ON:			TestResultDisp(2,true);			break;
			}
		}
		else if(is_pc_connected_flag == true)	// PC 검사기
		{
			static uint8_t pre_key = 0;			
			static uint8_t pre_safe = 0;			
			static uint8_t pre_sen = 0;			

			if( (KET_RET != pre_key) )		key_value_pc(KET_RET);
			if( (SAFE_RET != pre_safe) )	safe_value_pc(SAFE_RET);
//			if( (SEN_RET != pre_sen) )		sen_value_pc(SEN_RET);
			
			pre_key = KET_RET;
			pre_safe = SAFE_RET;
		//	pre_sen = SEN_RET;
						
			if(test_state_flag == (ST1_ON|ST2_ON))
			{				
				test_state_flag = 0x00;
				
				//if(SAFE_RET == 0)
				{					
					dbgmsg("TestStart\n");
					TestStart();
				}
				//else
				{
				//	dbgmsg("No sample\n");
				//	MainPopup(true, GUI_LIGHTRED,"No sample");	
				}
				
				test_state_flag = 0x00;
			}
		}
		Delay_ms(5);
	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
