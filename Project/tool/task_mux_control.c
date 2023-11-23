#include "task_mux_control.h"

float mux_res = 0;

void MuxOnRes(void)
{      	
	mux_res = 0;		
	MUXPinOnP64N64(63,64);	Delay_ms(1);	
	ModeSelect(0);			Delay_ms(5);
	
	for(int i=0;i<5;i++)	mux_res = Measure(0);
	
	if(mux_res > 50)	
	{
		dbgmsg(">> Mux_On_Res:%.2f : NG\n",mux_res);
		mux_res = 28;
	}
	else
	{
		dbgmsg(">> Mux_On_Res:%.2f : OK\n",mux_res);
	}
}

void DataSetup(uint8_t pin)
{	
	#ifdef VHC4051
		switch(pin)
		{						
			case 0: MUX_D8(1); MUX_D7(1); MUX_D6(1); MUX_D5(1);	MUX_D4(1); MUX_D3(1); MUX_D2(1); MUX_D1(1); break;	// 0xFF, 0b11111111	//	init
			case 1: MUX_D8(1); MUX_D7(1); MUX_D6(1); MUX_D5(1);	MUX_D4(1); MUX_D3(1); MUX_D2(1); MUX_D1(0); break;	// 0xFE, 0b11111110
			case 2: MUX_D8(1); MUX_D7(1); MUX_D6(1); MUX_D5(1);	MUX_D4(1); MUX_D3(1); MUX_D2(0); MUX_D1(1); break;	// 0xFD, 0b11111101
			case 3: MUX_D8(1); MUX_D7(1); MUX_D6(1); MUX_D5(1);	MUX_D4(1); MUX_D3(0); MUX_D2(1); MUX_D1(1); break;	// 0xFB, 0b11111011
			case 4: MUX_D8(1); MUX_D7(1); MUX_D6(1); MUX_D5(1);	MUX_D4(0); MUX_D3(1); MUX_D2(1); MUX_D1(1); break;	// 0xF7, 0b11110111
			case 5: MUX_D8(1); MUX_D7(1); MUX_D6(1); MUX_D5(0);	MUX_D4(1); MUX_D3(1); MUX_D2(1); MUX_D1(1); break;	// 0xEF, 0b11101111
			case 6: MUX_D8(1); MUX_D7(1); MUX_D6(0); MUX_D5(1);	MUX_D4(1); MUX_D3(1); MUX_D2(1); MUX_D1(1); break;	// 0xDF, 0b11011111
			case 7: MUX_D8(1); MUX_D7(0); MUX_D6(1); MUX_D5(1);	MUX_D4(1); MUX_D3(1); MUX_D2(1); MUX_D1(1); break;	// 0xBF, 0b10111111
			case 8: MUX_D8(0); MUX_D7(1); MUX_D6(1); MUX_D5(1);	MUX_D4(1); MUX_D3(1); MUX_D2(1); MUX_D1(1); break;	// 0x7F, 0b01111111
		}
	#endif

	#ifdef DG408LE
		switch(pin)
		{
			case 0: MUX_D8(0); MUX_D7(0); MUX_D6(0); MUX_D5(0);	MUX_D4(0); MUX_D3(0); MUX_D2(0); MUX_D1(0);	break;	// 0x00, 0b00000000	//	init
			case 1: MUX_D8(0); MUX_D7(0); MUX_D6(0); MUX_D5(0);	MUX_D4(0); MUX_D3(0); MUX_D2(0); MUX_D1(1);	break;	// 0x01, 0b00000001
			case 2: MUX_D8(0); MUX_D7(0); MUX_D6(0); MUX_D5(0);	MUX_D4(0); MUX_D3(0); MUX_D2(1); MUX_D1(0);	break;	// 0x02, 0b00000010
			case 3: MUX_D8(0); MUX_D7(0); MUX_D6(0); MUX_D5(0);	MUX_D4(0); MUX_D3(1); MUX_D2(0); MUX_D1(0);	break;	// 0x04, 0b00000100
			case 4: MUX_D8(0); MUX_D7(0); MUX_D6(0); MUX_D5(0);	MUX_D4(1); MUX_D3(0); MUX_D2(0); MUX_D1(0);	break; 	// 0x08, 0b00001000
			case 5: MUX_D8(0); MUX_D7(0); MUX_D6(0); MUX_D5(1);	MUX_D4(0); MUX_D3(0); MUX_D2(0); MUX_D1(0);	break; 	// 0x10, 0b00010000
			case 6: MUX_D8(0); MUX_D7(0); MUX_D6(1); MUX_D5(0);	MUX_D4(0); MUX_D3(0); MUX_D2(0); MUX_D1(0);	break; 	// 0x20, 0b00100000
			case 7: MUX_D8(0); MUX_D7(1); MUX_D6(0); MUX_D5(0);	MUX_D4(0); MUX_D3(0); MUX_D2(0); MUX_D1(0);	break; 	// 0x40, 0b01000000
			case 8: MUX_D8(1); MUX_D7(0); MUX_D6(0); MUX_D5(0);	MUX_D4(0); MUX_D3(0); MUX_D2(0); MUX_D1(0);	break; 	// 0x80, 0b10000000
		}
	#endif
	Delay_us(1);
}
	
void MUX_P_ADD(uint8_t pin)
{
	//dbgmsg("[%s] %d\n",__func__,pin);
    switch(pin)
    {
		case 1:   MUX_PA2(0); MUX_PA1(0); MUX_PA0(0); break;	//000 D-S0 ON
        case 2:   MUX_PA2(0); MUX_PA1(0); MUX_PA0(1); break;	//001 D-S1 ON
        case 3:   MUX_PA2(0); MUX_PA1(1); MUX_PA0(0); break;	//010 D-S2 ON
        case 4:   MUX_PA2(0); MUX_PA1(1); MUX_PA0(1); break;	//011 D-S3 ON
        case 5:   MUX_PA2(1); MUX_PA1(0); MUX_PA0(0); break;	//100 D-S4 ON
        case 6:   MUX_PA2(1); MUX_PA1(0); MUX_PA0(1); break;	//101 D-S5 ON
        case 7:   MUX_PA2(1); MUX_PA1(1); MUX_PA0(0); break;	//110 D-S6 ON
        case 8:   MUX_PA2(1); MUX_PA1(1); MUX_PA0(1); break;	//111 D-S7 ON
    }
}
void MUX_N_ADD(uint8_t pin)
{    
	//dbgmsg("[%s] %d\n",__func__,pin);
    switch(pin)
	{
		case 1:   MUX_NA2(0); MUX_NA1(0); MUX_NA0(0); break;	//000 D-S0 ON
		case 2:   MUX_NA2(0); MUX_NA1(0); MUX_NA0(1); break;	//001 D-S1 ON
		case 3:   MUX_NA2(0); MUX_NA1(1); MUX_NA0(0); break;	//010 D-S2 ON
		case 4:   MUX_NA2(0); MUX_NA1(1); MUX_NA0(1); break;	//011 D-S3 ON
		case 5:   MUX_NA2(1); MUX_NA1(0); MUX_NA0(0); break;	//100 D-S4 ON
		case 6:   MUX_NA2(1); MUX_NA1(0); MUX_NA0(1); break;	//101 D-S5 ON
		case 7:   MUX_NA2(1); MUX_NA1(1); MUX_NA0(0); break;	//110 D-S6 ON
		case 8:   MUX_NA2(1); MUX_NA1(1); MUX_NA0(1); break;	//111 D-S7 ON
	}
}	
void MUX_PCS(uint8_t slot)
{	
	switch(slot)
	{
		case 0:		PCS_A_EN(1); ;PCS_A3(0); PCS_A2(0); PCS_A1(0); PCS_A_EN(1);	break;	// add:0 0 0, EN:1	= ALL Low
		case 1:		PCS_A_EN(1); ;PCS_A3(0); PCS_A2(0); PCS_A1(0); PCS_A_EN(0);	break;	// add:0 0 0, EN:0	= Y0 High
		case 2:		PCS_A_EN(1); ;PCS_A3(0); PCS_A2(0); PCS_A1(1); PCS_A_EN(0);	break;	// add:0 0 1, EN:0	= Y1 High 
		case 3:		PCS_A_EN(1); ;PCS_A3(0); PCS_A2(1); PCS_A1(0); PCS_A_EN(0);	break;	// add:0 1 0, EN:0	= Y2 High
		case 4:		PCS_A_EN(1); ;PCS_A3(0); PCS_A2(1); PCS_A1(1); PCS_A_EN(0);	break;	// add:0 1 1, EN:0	= Y3 High
		case 5:		PCS_A_EN(1); ;PCS_A3(1); PCS_A2(0); PCS_A1(0); PCS_A_EN(0);	break;	// add:1 0 0, EN:0	= Y4 High
		case 6:		PCS_A_EN(1); ;PCS_A3(1); PCS_A2(0); PCS_A1(1); PCS_A_EN(0);	break;	// add:1 0 1, EN:0	= Y5 High
		case 7:		PCS_A_EN(1); ;PCS_A3(1); PCS_A2(1); PCS_A1(0); PCS_A_EN(0);	break;	// add:1 1 0, EN:0	= Y6 High
		case 8:		PCS_A_EN(1); ;PCS_A3(1); PCS_A2(1); PCS_A1(1); PCS_A_EN(0);	break;	// add:1 1 1, EN:0	= Y7 High
        default:	PCS_A_EN(1); ;PCS_A3(0); PCS_A2(0); PCS_A1(0); PCS_A_EN(1);	break;	// add:0 0 0, EN:1	= ALL Low 
	}
}
void MUX_NCS(uint8_t slot)
{	
	switch(slot)
	{
		case 0:		NCS_A_EN(1); NCS_A3(0); NCS_A2(0); NCS_A1(0); NCS_A_EN(1);	break;	// add:0 0 0, EN:1	= ALL Low
		case 1:		NCS_A_EN(1); NCS_A3(0); NCS_A2(0); NCS_A1(0); NCS_A_EN(0);	break;	// add:0 0 0, EN:0	= Y0 High
		case 2:		NCS_A_EN(1); NCS_A3(0); NCS_A2(0); NCS_A1(1); NCS_A_EN(0);	break;	// add:0 0 1, EN:0	= Y1 High 
		case 3:		NCS_A_EN(1); NCS_A3(0); NCS_A2(1); NCS_A1(0); NCS_A_EN(0);	break;	// add:0 1 0, EN:0	= Y2 High
		case 4:		NCS_A_EN(1); NCS_A3(0); NCS_A2(1); NCS_A1(1); NCS_A_EN(0);	break;	// add:0 1 1, EN:0	= Y3 High
		case 5:		NCS_A_EN(1); NCS_A3(1); NCS_A2(0); NCS_A1(0); NCS_A_EN(0);	break;	// add:1 0 0, EN:0	= Y4 High
		case 6:		NCS_A_EN(1); NCS_A3(1); NCS_A2(0); NCS_A1(1); NCS_A_EN(0);	break;	// add:1 0 1, EN:0	= Y5 High
		case 7:		NCS_A_EN(1); NCS_A3(1); NCS_A2(1); NCS_A1(0); NCS_A_EN(0);	break;	// add:1 1 0, EN:0	= Y6 High
		case 8:		NCS_A_EN(1); NCS_A3(1); NCS_A2(1); NCS_A1(1); NCS_A_EN(0);	break;	// add:1 1 1, EN:0	= Y7 High
        default:	NCS_A_EN(1); NCS_A3(0); NCS_A2(0); NCS_A1(0); NCS_A_EN(1);	break;	// add:0 0 0, EN:1	= ALL Low
	}
}	
void MUX_P_EN(uint8_t slot, uint16_t pin)
{	
	const uint8_t MUX_PIN = 8;//fix
	//dbgmsg("[%s] %d-%d\n",__func__,slot,pin);
	
		 if( ((MUX_PIN*0)<pin) && (pin<=(MUX_PIN*1)))	{ pin=pin-(MUX_PIN*0); MUX_P_ADD(pin); DataSetup(1); MUX_PCS(slot);}
	else if( ((MUX_PIN*1)<pin) && (pin<=(MUX_PIN*2)))	{ pin=pin-(MUX_PIN*1); MUX_P_ADD(pin); DataSetup(2); MUX_PCS(slot);}
	else if( ((MUX_PIN*2)<pin) && (pin<=(MUX_PIN*3)))	{ pin=pin-(MUX_PIN*2); MUX_P_ADD(pin); DataSetup(3); MUX_PCS(slot);}
	else if( ((MUX_PIN*3)<pin) && (pin<=(MUX_PIN*4)))	{ pin=pin-(MUX_PIN*3); MUX_P_ADD(pin); DataSetup(4); MUX_PCS(slot);}
	else if( ((MUX_PIN*4)<pin) && (pin<=(MUX_PIN*5)))	{ pin=pin-(MUX_PIN*4); MUX_P_ADD(pin); DataSetup(5); MUX_PCS(slot);}
	else if( ((MUX_PIN*5)<pin) && (pin<=(MUX_PIN*6)))	{ pin=pin-(MUX_PIN*5); MUX_P_ADD(pin); DataSetup(6); MUX_PCS(slot);}
	else if( ((MUX_PIN*6)<pin) && (pin<=(MUX_PIN*7)))	{ pin=pin-(MUX_PIN*6); MUX_P_ADD(pin); DataSetup(7); MUX_PCS(slot);}
	else if( ((MUX_PIN*7)<pin) && (pin<=(MUX_PIN*8)))	{ pin=pin-(MUX_PIN*7); MUX_P_ADD(pin); DataSetup(8); MUX_PCS(slot);}
		
}
void MUX_N_EN(uint8_t slot, uint16_t pin)
{
	const uint8_t MUX_PIN = 8;//fix
	//dbgmsg("[%s] %d-%d\n",__func__,slot,pin);
	
		 if( ((MUX_PIN*0)<pin) && (pin<=(MUX_PIN*1)))	{ pin=pin-(MUX_PIN*0); MUX_N_ADD(pin); DataSetup(1); MUX_NCS(slot);}
	else if( ((MUX_PIN*1)<pin) && (pin<=(MUX_PIN*2)))	{ pin=pin-(MUX_PIN*1); MUX_N_ADD(pin); DataSetup(2); MUX_NCS(slot);}
	else if( ((MUX_PIN*2)<pin) && (pin<=(MUX_PIN*3)))	{ pin=pin-(MUX_PIN*2); MUX_N_ADD(pin); DataSetup(3); MUX_NCS(slot);}
	else if( ((MUX_PIN*3)<pin) && (pin<=(MUX_PIN*4)))	{ pin=pin-(MUX_PIN*3); MUX_N_ADD(pin); DataSetup(4); MUX_NCS(slot);}
	else if( ((MUX_PIN*4)<pin) && (pin<=(MUX_PIN*5)))	{ pin=pin-(MUX_PIN*4); MUX_N_ADD(pin); DataSetup(5); MUX_NCS(slot);}
	else if( ((MUX_PIN*5)<pin) && (pin<=(MUX_PIN*6)))	{ pin=pin-(MUX_PIN*5); MUX_N_ADD(pin); DataSetup(6); MUX_NCS(slot);}
	else if( ((MUX_PIN*6)<pin) && (pin<=(MUX_PIN*7)))	{ pin=pin-(MUX_PIN*6); MUX_N_ADD(pin); DataSetup(7); MUX_NCS(slot);}
	else if( ((MUX_PIN*7)<pin) && (pin<=(MUX_PIN*8)))	{ pin=pin-(MUX_PIN*7); MUX_N_ADD(pin); DataSetup(8); MUX_NCS(slot);}		
}
void MuxInit(void) 
{		
	//dbgmsg("[%s] \n",__func__);
	CLR_273(1);
	DataSetup(0);
	for(int slot=1;slot<9;slot++)
	{
		MUX_PCS(slot);
		MUX_NCS(slot);
	}
	MUX_PCS(0);
	MUX_NCS(0);
}
void MUXPinOn(uint16_t HI, uint16_t LO) 
{
	const uint8_t SLOT_PIN = 60;//fix
	//dbgmsg("\n\n[%s] %d-%d\n",__func__,HI,LO);
	MuxInit();		
	
		 if( ((SLOT_PIN*0)<HI) && (HI<=(SLOT_PIN*1)) )	{ HI=HI-(SLOT_PIN*0);	MUX_P_EN(1,HI);	}   // slot1(001-060)
	else if( ((SLOT_PIN*1)<HI) && (HI<=(SLOT_PIN*2)) )	{ HI=HI-(SLOT_PIN*1);	MUX_P_EN(2,HI);	}   // slot2(061-120)
	else if( ((SLOT_PIN*2)<HI) && (HI<=(SLOT_PIN*3)) )	{ HI=HI-(SLOT_PIN*2);	MUX_P_EN(3,HI);	}   // slot3(121-180)
	else if( ((SLOT_PIN*3)<HI) && (HI<=(SLOT_PIN*4)) )	{ HI=HI-(SLOT_PIN*3);	MUX_P_EN(4,HI);	}   // slot4(181-240)
	else if( ((SLOT_PIN*4)<HI) && (HI<=(SLOT_PIN*5)) )	{ HI=HI-(SLOT_PIN*4);	MUX_P_EN(5,HI);	}   // slot5(241-300)
	else if( ((SLOT_PIN*5)<HI) && (HI<=(SLOT_PIN*6)) )	{ HI=HI-(SLOT_PIN*5);	MUX_P_EN(6,HI);	}   // slot6(301-360)
	else if( ((SLOT_PIN*6)<HI) && (HI<=(SLOT_PIN*7)) )	{ HI=HI-(SLOT_PIN*6);	MUX_P_EN(7,HI);	}   // slot7(361-420)
	else if( ((SLOT_PIN*7)<HI) && (HI<=(SLOT_PIN*8)) )	{ HI=HI-(SLOT_PIN*7);	MUX_P_EN(8,HI);	}   // slot8(421-480)

		 if( ((SLOT_PIN*0)<LO) && (LO<=(SLOT_PIN*1)) )	{ LO=LO-(SLOT_PIN*0);	MUX_N_EN(1,LO);	}   // slot1(001-060)
	else if( ((SLOT_PIN*1)<LO) && (LO<=(SLOT_PIN*2)) )	{ LO=LO-(SLOT_PIN*1);	MUX_N_EN(2,LO);	}   // slot2(061-120)
	else if( ((SLOT_PIN*2)<LO) && (LO<=(SLOT_PIN*3)) )	{ LO=LO-(SLOT_PIN*2);	MUX_N_EN(3,LO);	}   // slot3(121-180)
	else if( ((SLOT_PIN*3)<LO) && (LO<=(SLOT_PIN*4)) )	{ LO=LO-(SLOT_PIN*3);	MUX_N_EN(4,LO);	}   // slot4(181-240)
	else if( ((SLOT_PIN*4)<LO) && (LO<=(SLOT_PIN*5)) )	{ LO=LO-(SLOT_PIN*4);	MUX_N_EN(5,LO);	}   // slot5(241-300)
	else if( ((SLOT_PIN*5)<LO) && (LO<=(SLOT_PIN*6)) )	{ LO=LO-(SLOT_PIN*5);	MUX_N_EN(6,LO);	}   // slot6(301-360)
	else if( ((SLOT_PIN*6)<LO) && (LO<=(SLOT_PIN*7)) )	{ LO=LO-(SLOT_PIN*6);	MUX_N_EN(7,LO);	}   // slot7(361-420)
	else if( ((SLOT_PIN*7)<LO) && (LO<=(SLOT_PIN*8)) )	{ LO=LO-(SLOT_PIN*7);	MUX_N_EN(8,LO);	}   // slot8(421-480)
}
void MUXPinOnP64N64(uint16_t HI, uint16_t LO) // PIN 64PIN MAP
{       
	const uint8_t SLOT_PIN = 64;//fix
	MuxInit();		
	
		 if( ((SLOT_PIN*0)<HI) && (HI<=(SLOT_PIN*1)) )	{ HI=HI-(SLOT_PIN*0);	MUX_P_EN(1,HI);	}   // slot1(001-064)
	//else if( ((SLOT_PIN*1)<HI) && (HI<=(SLOT_PIN*2)) )	{ HI=HI-(SLOT_PIN*1);	MUX_P_EN(2,HI);	}   // slot2(065-128)
	//else if( ((SLOT_PIN*2)<HI) && (HI<=(SLOT_PIN*3)) )	{ HI=HI-(SLOT_PIN*2);	MUX_P_EN(3,HI);	}   // slot3(129-192)
	//else if( ((SLOT_PIN*3)<HI) && (HI<=(SLOT_PIN*4)) )	{ HI=HI-(SLOT_PIN*3);	MUX_P_EN(4,HI);	}   // slot4(193-256)
	//else if( ((SLOT_PIN*4)<HI) && (HI<=(SLOT_PIN*5)) )	{ HI=HI-(SLOT_PIN*4);	MUX_P_EN(5,HI);	}   // slot5(257-320)
	//else if( ((SLOT_PIN*5)<HI) && (HI<=(SLOT_PIN*6)) )	{ HI=HI-(SLOT_PIN*5);	MUX_P_EN(6,HI);	}   // slot6(321-384)
	//else if( ((SLOT_PIN*6)<HI) && (HI<=(SLOT_PIN*7)) )	{ HI=HI-(SLOT_PIN*6);	MUX_P_EN(7,HI);	}   // slot7(385-448)
	//else if( ((SLOT_PIN*7)<HI) && (HI<=(SLOT_PIN*8)) )	{ HI=HI-(SLOT_PIN*7);	MUX_P_EN(8,HI);	}   // slot8(449-512)

		 if( ((SLOT_PIN*0)<LO) && (LO<=(SLOT_PIN*1)) )	{ LO=LO-(SLOT_PIN*0);	MUX_N_EN(1,LO);	}   // slot1(001-064)
	//else if( ((SLOT_PIN*1)<LO) && (LO<=(SLOT_PIN*2)) )	{ LO=LO-(SLOT_PIN*1);	MUX_N_EN(2,LO);	}   // slot2(065-128)
	//else if( ((SLOT_PIN*2)<LO) && (LO<=(SLOT_PIN*3)) )	{ LO=LO-(SLOT_PIN*2);	MUX_N_EN(3,LO);	}   // slot3(129-192)
	//else if( ((SLOT_PIN*3)<LO) && (LO<=(SLOT_PIN*4)) )	{ LO=LO-(SLOT_PIN*3);	MUX_N_EN(4,LO);	}   // slot4(193-256)
	//else if( ((SLOT_PIN*4)<LO) && (LO<=(SLOT_PIN*5)) )	{ LO=LO-(SLOT_PIN*4);	MUX_N_EN(5,LO);	}   // slot5(257-320)
	//else if( ((SLOT_PIN*5)<LO) && (LO<=(SLOT_PIN*6)) )	{ LO=LO-(SLOT_PIN*5);	MUX_N_EN(6,LO);	}   // slot6(321-384)
	//else if( ((SLOT_PIN*6)<LO) && (LO<=(SLOT_PIN*7)) )	{ LO=LO-(SLOT_PIN*6);	MUX_N_EN(7,LO);	}   // slot7(385-448)
	//else if( ((SLOT_PIN*7)<LO) && (LO<=(SLOT_PIN*8)) )	{ LO=LO-(SLOT_PIN*7);	MUX_N_EN(8,LO);	}   // slot8(449-512)
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
