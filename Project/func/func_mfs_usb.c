#include "main.h"

#ifdef MFF_USB

uint8_t USB_FirmwareWrite(uint16_t index);

uint8_t Downwait_Flag = 0;
uint8_t receving = false;


uint8_t USB_FirmwareWrite(uint16_t cnt)
{
		int ret = false;
	
		FuncRYControl(255,0,250);
		EvccControl(1,0);
		EvccControl(2,0);

		FuncRYControl(1,1,0);	// dvdd - gnd
		FuncRYControl(2,1,0);	// avdd - gnd
		FuncRYControl(5,1,0);	// sck - miso
		FuncRYControl(6,1,250);	// mosi - cs

		EvccControl(1,1);	Delay_ms(250);
	
		
	if(RD_FW_WRITE == false)
    {
				tx_pc_data(MFS_VERIFY, 1, 0);
    }   
	else tx_pc_data(MFS_WRITE, 1, 0);
    
    
	Delay_ms(1000);
	
	while(1)
	{

		switch(Downwait_Flag)
		{
		    case 1 :
			Beep(100); 
			Downwait_Flag = 0;			
			ListView_MEAS("Downloading...");	
			break;		
			case 2 : 
			
			if(RD_FW_WRITE == false)
			{
				ListView_MEAS("Verify");
			}
			Downwait_Flag = 3;
			break;
			
			case 10 :		
				Downwait_Flag = 4;
			break;
		
			default : break;	
		}
	
		if(Downwait_Flag ==3)
		{
			Downwait_Flag =0 ;
		
			if(RD_FW_WRITE == false)
			{
				ListView_MEAS("Verify PASS");
			}
			else ListView_MEAS("PASS");	
			ret = true;
			EvccControl(1,0);	
			EvccControl(2,0);	Delay_ms(250);
			FuncRYControl(255,0,250);
			receving = false;
			break;
		}
		
		else if(( ( KeyRead() >> 2 ) & 0x01 ) == 0)
		{	
			Downwait_Flag =0 ;
			ListView_MEAS("Cancel");
			ret = false;
			EvccControl(1,0);	
			EvccControl(2,0);	Delay_ms(250);
			FuncRYControl(255,0,250);
			receving = false;
			break;
		}
		else if(Downwait_Flag ==4)
		{
			Downwait_Flag =0 ;
			
			if(RD_FW_WRITE == false)
			{
				ListView_MEAS("Verify Fail");
			}
			else 
			{
				ListView_MEAS("Download Fail");
			}
			ret = false;
			EvccControl(1,0);	
			EvccControl(2,0);	Delay_ms(250);
			FuncRYControl(255,0,250);
			break;			
		}
	}
	return ret;
}


typedef enum
{
	MFF_USB_FirmwareWrite = 20,
}MFF_INSP_TYPE;

uint8_t MELFAS_ISC_USB(uint16_t index) 
{
	uint8_t ret = false;
	dbgmsg("   MFSUSB ==> %02d\n", func_module[index]);
	dbgmsg("index No : %d\n", index);
	
	switch(func_module[index]) 
	{
			case MFF_USB_FirmwareWrite :	ret = USB_FirmwareWrite(index);	break;	// ok
			default : ret = false;											break;	// ok
	}
	return ret;	
}
#endif