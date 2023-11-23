#include "func_mfs_touch.h"

#ifdef MFF_ISC
/* Firmware info */
#define FW_CHIP_CODE "T4H1"
#define FW_TYPE_TAIL

/* Flash info */
#define FLASH_SIZE (127 * 1024)

/* ISC info */
#define ISC_PAGE_SIZE 128

/* ISC command */
#define ISC_CMD_ENTER       {0xFB, 0x4A, 0x00, 0x65, 0x00, 0x00, 0x00, 0x00}
#define ISC_CMD_ERASE_PAGE  {0xFB, 0x4A, 0x00, 0x8F, 0x00, 0x00, 0x00, 0x00}
#define ISC_CMD_READ_PAGE   {0xFB, 0x4A, 0x00, 0xC2, 0x00, 0x00, 0x00, 0x00}
#define ISC_CMD_WRITE_PAGE  {0xFB, 0x4A, 0x00, 0xA5, 0x00, 0x00, 0x00, 0x00}
#define ISC_CMD_READ_STATUS {0xFB, 0x4A, 0x36, 0xC2, 0x00, 0x00, 0x00, 0x00}
#define ISC_CMD_EXIT        {0xFB, 0x4A, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00}

/* ISC status */
#define ISC_STATUS_BUSY 0x96
#define ISC_STATUS_DONE 0xAD

/* Bootloader info */
#define BL_PAGE_SIZE   512
#define BL_PACKET_SIZE 256 /* 512, 256, 128 or 64 */

typedef struct  {
	u8 tail_mark[4];
	char chip_name[4];
	u32 bin_start_addr;
	u32 bin_length;

	u16 ver_boot;
	u16 ver_core;
	u16 ver_app;
	u16 ver_param;
	u8 boot_start;
	u8 boot_end;
	u8 core_start;
	u8 core_end;
	u8 app_start;
	u8 app_end;
	u8 param_start;
	u8 param_end;

	u8 checksum_type;
	u8 hw_category;
	u16 param_id;
	u32 param_length;
	u32 build_date;
	u32 build_time;

	u32 reserved1;
	u32 reserved2;
	u16 reserved3;
	u16 tail_size;
	u32 crc;
} melfas_bin_tail;

#define MELFAS_BIN_TAIL_MARK {0x4D, 0x42, 0x54, 0x01} /* M B T 0x01 */
#define MELFAS_BIN_TAIL_SIZE 64

#define sID_Err  0xa1
#define AddM_Err 0xa2
#define AddL_Err 0xa3
#define DATA_Err 0xa4

//#define EN_PIN(x)		x ? GPIO_SetBits(GPIOB,GPIO_Pin_10) : GPIO_ResetBits(GPIOB,GPIO_Pin_10) //210510 MCH
//#define RST_PIN(x)		x ? GPIO_SetBits(GPIOB,GPIO_Pin_11) : GPIO_ResetBits(GPIOB,GPIO_Pin_11) //210510 MCH


int i2c_transfer_write(char *write_buf, unsigned int write_len);
int i2c_transfer_read(char *write_buf, unsigned int write_len, char *read_buf, unsigned int read_len);

uint8_t slave_addr = 0x68;                //0x34
uint8_t slave_addr_default = 0x68;        //0x34

extern uint8_t MFS_STATE ;

//----------------------------
extern uint8_t mfs_test(void);
//-----------------------------
uint8_t I2C_Add_Fine(void)
{
	int i = 0;
	char text[50];
	uint8_t SA_1 = slave_addr_default;
	uint8_t SA_2 = 0x10*2;

	dbgmsg("Into the I2C_Add_Fine\n"); 	
	
	i2c2_start();
	
	if(melfas_i2c1_tx( (slave_addr & 0xFE) ) != 0)
	{
		i2c2_stop();
		dbgmsg("Slave Addr Write ACK Err : %02X\n",slave_addr); 		
		//return false; 
	}
	Delay_ms(100);
	i2c2_start();  Delay_us(10);
	
	if(melfas_i2c1_tx( (SA_1 & 0xFE) ) != 0)
	{
		i2c2_stop();
		dbgmsg("Slave Addr Write ACK Err : %02X\n",SA_1); 		
	}
	else
	{
		i2c2_stop();    
		slave_addr = SA_1;	
		dbgmsg("S.A OK : 0x%02X\n",slave_addr);
		sprintf(text,"S.A OK : 0x%02X",SA_1);    
		ListView_MEAS(text);
		return true;
	}
	
	Delay_ms(100);
	i2c2_start();  Delay_us(10);
	
	if(melfas_i2c1_tx( (SA_2 & 0xFE) ) != 0)
	{
		i2c2_stop();
		dbgmsg("Slave Addr Write ACK Err : %02X\n",SA_2); 
	}
	else
	{
		i2c2_stop();    
		slave_addr = SA_2;	
		dbgmsg("S.A OK : 0x%02X\n",slave_addr);
		sprintf(text,"S.A OK : 0x%02X",SA_1);    
		ListView_MEAS(text);
		return true;
	}
	Delay_ms(100);
	for(i = 0; i<256; i++)
	{
		i2c2_start();  Delay_us(10);
				
		if(melfas_i2c1_tx( (i & 0xFE) ) != 0)
		{
			i2c2_stop();
			dbgmsg("Slave Addr Write ACK Err\n"); 
		}
		else
		{
			i2c2_stop();    
			dbgmsg("S.A OK : 0x%02X\n",slave_addr);
			sprintf(text,"S.A Found : 0x%02X",i); 
			ListView_MEAS(text);
			slave_addr = i;	
			return true;
		}
	}
	ListView_MEAS("S.A not Found");
	
	return false;
}
int i2c_transfer_write(char *write_buf, unsigned int write_len)
{
	s32 i;
	u8 TempData8;
	u8 count = 0;
	uint32_t REP_NUM = 5;

	RETRY_WD:
	
	i2c2_start();	
	
	if(melfas_i2c1_tx( (slave_addr & 0xFE) ) != 0)
	{
		if( count++ < REP_NUM) goto	RETRY_WD;
		i2c2_stop();
		dbgmsg("Slave Addr Write Err : %02X\n", slave_addr); 
		return false; 
	}	
	
	for(i=0; i < write_len; i++)
	{
		TempData8 = *write_buf++; 
		
		if(melfas_i2c1_tx( (u8)TempData8 ) != 0)
		{
			if( count++ < REP_NUM) goto	RETRY_WD;
			i2c2_stop();
			dbgmsg("Data wr err(%03d/%03d)\n",i+1,write_len);
			return false; 
		}			
	}
	i2c2_stop();
	
	return true;
}

int i2c_transfer_read(char *write_buf, unsigned int write_len, char *read_buf, unsigned int read_len)  ////// JSH
{
	s32 i;
	u8 TempData8;
	u8 count = 0;
	uint32_t REP_NUM = 5;

	RETRY_RD:
	
	i2c2_start();
	
	if(melfas_i2c1_tx( (slave_addr & 0xFE) ) != 0)
	{
		if( count++ < REP_NUM) goto 	RETRY_RD;
		i2c2_stop();
		dbgmsg("Slave Addr Write Err\n"); 
		return false; 
	}

	for(i=0; i < write_len; i++)
	{
		TempData8 = *write_buf++; 
		
		if(melfas_i2c1_tx( (u8)TempData8 ) != 0)
		{
			if( count++ < REP_NUM) goto 	RETRY_RD;
			i2c2_stop();
			dbgmsg("Data wr err(%03d/%03d)\n",i+1,write_len);
			return false; 
		}			
	}
	
	i2c2_stop(); 	
	
	i2c2_start();	
	
	if(melfas_i2c1_tx( (slave_addr & 0xFE) | 0x01 ) != 0)
	{
		if( count++ < REP_NUM) goto 	RETRY_RD;  
		i2c2_stop();
		dbgmsg("Slave Addr Read Err\n"); 
		return false; 
	}	
				
	for(i=0; i<read_len; i++)
	{
		if( i == (read_len-1) ) 	TempData8 = i2c2_rx(0);
		else     	           		TempData8 = i2c2_rx(1);
		*read_buf=TempData8;
		read_buf++;
	}
	read_buf = read_buf - read_len;
	
	i2c2_stop();
	
	return true;
}
/*
* ISC - Read status
*/
static int isc_read_status(void)
{
	char cmd[8] = ISC_CMD_READ_STATUS;    ////  #define ISC_CMD_READ_STATUS {0xFB, 0x4A, 0x36, 0xC2, 0x00, 0x00, 0x00, 0x00}
	char result = 0;
	int cnt = 100;
	int ret = 0;

	dbgmsg("%s [START]\n", __func__);
	do 
	{
		if(i2c_transfer_read(cmd, 8, &result, 1) == false)
		{
			ret = -1;        
			dbgmsg("%s [ERROR] i2c_transfer\n", __func__);
			goto error;
		}
		if (result == (char)ISC_STATUS_DONE)		
		{
			ret = 0;
			break;						
		}
		else if (result == (char)ISC_STATUS_BUSY) 
		{
			ret = -1;
			Delay_us(700);
		}		
		else
		{
			ret = -1;			
			Delay_us(700);	  
			dbgmsg("%s [ERROR] wrong value [0x%02X]\n", __func__, result);	 
		}
	}
	
	while (--cnt);

	if (!cnt) 
	{
		dbgmsg("%s [ERROR] count overflow - cnt [%d] status [0x%02X]\n", __func__, cnt, result); 
		goto error;
	}

	 dbgmsg("%s [DONE]\n", __func__);
	 return ret;
	
   error:
	 dbgmsg("%s [ERROR]\n", __func__);
	 return ret;
}
/*
* ISC - Erase page
*/
static int isc_erase_page(int addr)
{
	char write_buf[8] = ISC_CMD_ERASE_PAGE;

	dbgmsg("%s [START]\n", __func__);

	write_buf[4] = (u8)((addr >> 24) & 0xFF);	
	write_buf[5] = (u8)((addr >> 16) & 0xFF);
	write_buf[6] = (u8)((addr >>  8) & 0xFF);
	write_buf[7] = (u8)((addr >>  0) & 0xFF);

	if(i2c_transfer_write(write_buf, 8) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);  goto ERROR;
	}
	if (isc_read_status() != 0) 	
	{
		goto ERROR;
	}
	dbgmsg("%s [DONE] - Addr[0x%08X]\n", __func__, addr);
	return 0;

	ERROR:
	dbgmsg("%s [ERROR]\n", __func__);
	return 1;
}
/*
* ISC : Enable
*/
#define ISC_CMD_MODE_READ   {0xFB, 0xB7, 0xDD, 0xC2, 0x00, 0x00, 0x00, 0x00}
#define ISC_CMD_MODE_WRITE  {0xFB, 0xB7, 0xDD, 0xA5, 0x00, 0x00, 0x00, 0x00}
static int isc_enable(void)
{
	char read_wbuf[8] = ISC_CMD_MODE_READ;
	char read_rbuf[ISC_PAGE_SIZE] = {0, };
	char write_wbuf[8 + 16] = ISC_CMD_MODE_WRITE;
	int i;
	
	dbgmsg("%s [START]\n", __func__);

	if(i2c_transfer_read(read_wbuf, 8, read_rbuf, ISC_PAGE_SIZE) == false)
	{
		dbgmsg("%s [ERROR] i2c_transfer\n", __func__);
		goto error;
	}
	
	for (i = 0; i < 14; i++) 
	{
		write_wbuf[8 + i] = read_rbuf[i];
	}
	write_wbuf[8 + 14] = 0x30;
	write_wbuf[8 + 15] = 0x00;

	if(i2c_transfer_write(write_wbuf, (8+16)) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}

	dbgmsg("%s [DONE]\n", __func__);
	return 0;

	error:
	dbgmsg("%s [ERROR]\n", __func__);
	return -1;
}
/*
* ISC - Read page
*/
static int isc_read_page(int addr, char *read_buf)
{
	char write_buf[8] = ISC_CMD_READ_PAGE;
	
	dbgmsg("%s [START]\n", __func__);

	write_buf[4] = (u8)((addr >> 24) & 0xFF);
	write_buf[5] = (u8)((addr >> 16) & 0xFF);
	write_buf[6] = (u8)((addr >> 8) & 0xFF);
	write_buf[7] = (u8)(addr & 0xFF);
	
	if(i2c_transfer_read(write_buf, 8, read_buf, ISC_PAGE_SIZE) == false)
	{
		dbgmsg("%s [ERROR] i2c_transfer\n", __func__);
		goto ERROR;
	}

	dbgmsg("%s [DONE] - Addr[0x%08X]\n", __func__, addr);
	return 0;
	
	ERROR:
	dbgmsg("%s [ERROR]\n", __func__);
	return 1;
}

/*
* ISC - Write page
*/
static int isc_write_page(int addr, const u8 *write_data, int length)
{
	char write_buf[8 + ISC_PAGE_SIZE] = ISC_CMD_WRITE_PAGE;

	dbgmsg("%s [START]\n", __func__);
	
	if (length > ISC_PAGE_SIZE) 
	{
		dbgmsg("%s [ERROR] page length overflow\n", __func__);
		goto ERROR;
	}

	write_buf[4] = (u8)((addr >> 24) & 0xFF);
	write_buf[5] = (u8)((addr >> 16) & 0xFF);
	write_buf[6] = (u8)((addr >> 8) & 0xFF);
	write_buf[7] = (u8)(addr & 0xFF);

	memcpy(&write_buf[8], write_data, length);

	if(i2c_transfer_write(write_buf, (8 + length)) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto ERROR;
	}
	
	if (isc_read_status() != 0) 
	{
		goto ERROR;
	}

	dbgmsg("%s [DONE] - Addr[0x%08X] Length[%d]\n", __func__, addr, length);
	return 0;

	ERROR:
	dbgmsg("%s [ERROR]\n", __func__);
	return 1;
}
/*
* ISC - Enter ISC mode
*/
static int isc_enter(void)
{
	char write_buf[8] = ISC_CMD_ENTER;

	dbgmsg("%s [START]\n", __func__);
			
	I2C_Add_Fine(); //210906
		
	if(i2c_transfer_write(write_buf, 8) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto ERROR;
	}

	if (isc_read_status() != 0) 
	{
		goto ERROR;
	}

	dbgmsg("%s [DONE]\n", __func__);
	return 0;

	ERROR:
	dbgmsg("%s [ERROR]\n", __func__);
	return 1;
}
/*
* ISC - Exit ISC mode
*/
static int isc_exit(void)
{
	char write_buf[8] = ISC_CMD_EXIT;

	dbgmsg("%s [START]\n", __func__);

	if(i2c_transfer_write(write_buf, 8) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto ERROR;
	}
	dbgmsg("%s [DONE]\n", __func__);
	return 0;

	ERROR:
	dbgmsg("%s [ERROR]\n", __func__);
	return 1;
}

/*
* Bootloader - Read status
*/
static int bl_read_status(void)
{
	char write_buf[2];
	char result = 0;
	int cnt = 1000;
	int ret = 0;

	dbgmsg("%s [START]\n", __func__);

	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_STATUS;

	I2C_Add_Fine();	//201029 MCH 210906

	do 
	{		
		if(i2c_transfer_read(write_buf, 2, &result, 1) == false)
		{
			dbgmsg("%s [ERROR] i2c_transfer\n", __func__);
			ret = -1;
			//goto error;
		}
		
		if (result == (char)MIP4_BOOT_STATUS_DONE) 
		{
			dbgmsg("%s - Done\n", __func__);
			ret = 0;
			break;
		}
		else if (result == MIP4_BOOT_STATUS_BUSY) 
		{
			dbgmsg("%s - Busy\n", __func__);
			ret = -1;
			Delay_us(600);
		}
		else if (result == MIP4_BOOT_STATUS_ERROR) 
		{
			dbgmsg("%s - Error\n", __func__);
			ret = -1;
			goto error;
		}
		else 
		{
			dbgmsg("%s [ERROR] wrong value [0x%02X]\n", __func__, result);
			ret = -1;
			Delay_us(600);
		}
	} 
	while (--cnt);

	if (!cnt) 
	{
		dbgmsg("%s [ERROR] count overflow - cnt [%d] status [0x%02X]\n", __func__, cnt, result);
		goto error;
	}
	dbgmsg("%s [DONE]\n", __func__);
	return ret;

error:
	dbgmsg("%s [ERROR]\n", __func__);
	return ret;
}

/*
* Bootloader - Change mode
*/
static int bl_change_mode(u8 mode)
{
	char write_buf[8];
	char read_buf[8];
	int cnt = 10;
	//int cnt = 100;
	int ret = 0;

	dbgmsg("%s [START]\n", __func__);
		
	do 
	{
		/* Write */
		write_buf[0] = MIP4_R0_BOOT;
		write_buf[1] = MIP4_R1_BOOT_MODE;
		write_buf[2] = mode;
	
		I2C_Add_Fine();// 201029 MCH 210906
		
		if(i2c_transfer_write(write_buf, 3) == false)
		{
			dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
			goto error;
		}
		dbgmsg("%s - Write : Mode [%d]\n", __func__, mode);

		/* Wait */
		Delay_ms(200);

		/* Read */
		write_buf[0] = MIP4_R0_BOOT;
		write_buf[1] = MIP4_R1_BOOT_MODE;	
		
		//if(mode == 2)I2C_Add_Fine();//201029 MCH
		
		if(i2c_transfer_read(write_buf, 2, read_buf, 1) == false)		
		{
			dbgmsg("%s [ERROR] i2c_transfer\n", __func__);
			ret = -1;
			goto error;
		}
		dbgmsg("%s - Read : Mode [%d]\n", __func__, read_buf[0]);

		if (read_buf[0] == mode) 
		{
			break;
		}
	} while (--cnt);

	if (!cnt) {
		dbgmsg("%s [ERROR] count overflow - cnt [%d]\n", __func__, cnt);
		goto error;
	}

	dbgmsg("%s [DONE]\n", __func__);
	return ret;

	error:
	dbgmsg("%s [ERROR]\n", __func__);
	return ret;
}

/*
* Bootloader - Read info
*/
static int bl_read_info(u16 *buf_addr)
{
	char write_buf[8];
	char read_buf[8];
	int ret = 0;

	dbgmsg("%s [START]\n", __func__);

	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_BUF_ADDR;
	
	if(i2c_transfer_read(write_buf, 2, read_buf, 2) == false)
	{
		dbgmsg("%s [ERROR] i2c_transfer\n", __func__);
		ret = -1;
		goto error;
	}

	*buf_addr = (u16)((read_buf[1] << 8) | read_buf[0]);
	
	dbgmsg("%s - Buf Addr [0x%08X]\n", __func__, *buf_addr);

	dbgmsg("%s [DONE]\n", __func__);
	return ret;

	error:
	dbgmsg("%s [ERROR]\n", __func__);
	return ret;
}
/*
* Bootloader - Erase mass
*/
static int bl_erase_mass(void)
{
	char write_buf[3];

	dbgmsg("%s [START]\n", __func__);

	/* Command */
	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_CMD;
	write_buf[2] = MIP4_BOOT_CMD_MASS_ERASE;
	
	if(i2c_transfer_write(write_buf, 3) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}

	/* Status */
	if (bl_read_status() != 0) 
	{
		goto error;
	}

	dbgmsg("%s [DONE]\n", __func__);
	return 0;

	error:
	dbgmsg("%s [ERROR]\n", __func__);
	return 1;
}
/*
* Bootloader - Program page
*/
static int bl_program_page(int offset, const u8 *data, int length, int buf_addr)
{
	char write_buf[2 + BL_PAGE_SIZE];
	int buf_offset = 0;

	dbgmsg("%s [START]\n", __func__);

	if (length > BL_PAGE_SIZE) 
	{
		dbgmsg("%s [ERROR] page length overflow\n", __func__);
		goto error;
	}

	/* Addr */
	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_TARGET_ADDR;
	write_buf[2] = (u8)(offset & 0xFF);
	write_buf[3] = (u8)((offset >> 8) & 0xFF);
	write_buf[4] = (u8)((offset >> 16) & 0xFF);
	write_buf[5] = (u8)((offset >> 24) & 0xFF);
	
	if(i2c_transfer_write(write_buf, 6) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}

	/* Size */
	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_SIZE;
	write_buf[2] = (u8)(length & 0xFF);
	write_buf[3] = (u8)((length >> 8) & 0xFF);
	write_buf[4] = (u8)((length >> 16) & 0xFF);
	write_buf[5] = (u8)((length >> 24) & 0xFF);

	if(i2c_transfer_write(write_buf, 6) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}
	dbgmsg("%s - Size [%d]\n", __func__, length);

	/* Data */
	for (buf_offset = 0; buf_offset < length; buf_offset += BL_PACKET_SIZE) 
	{
		write_buf[0] = (u8)(((buf_addr + buf_offset) >> 8) & 0xFF);
		write_buf[1] = (u8)((buf_addr + buf_offset) & 0xFF);
		memcpy(&write_buf[2], &data[buf_offset], BL_PACKET_SIZE);
		if(i2c_transfer_write(write_buf, (2 + BL_PACKET_SIZE)) == false)
		{
			dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
			goto error;
		}
	}

	/* Command */
	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_CMD;
	write_buf[2] = MIP4_BOOT_CMD_PROGRAM;
	if(i2c_transfer_write(write_buf, 3) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}

	/* Status */
	if (bl_read_status() != 0) 
	{
		goto error;
	}

	dbgmsg("%s [DONE]\n", __func__);
	return 0;

	error:
	dbgmsg("%s [ERROR]\n", __func__);
	return 1;
}
/*
* Bootloader - Write page
*/
static int bl_write_page(int offset, const u8 *data, int length, int buf_addr)
{
	char write_buf[2 + BL_PAGE_SIZE];
	int buf_offset = 0;

	dbgmsg("%s [START]\n", __func__);

	if (length > BL_PAGE_SIZE) 
	{
		dbgmsg("%s [ERROR] page length overflow\n", __func__);
		goto error;
	}

	/* Addr */
	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_TARGET_ADDR;
	write_buf[2] = (u8)(offset & 0xFF);
	write_buf[3] = (u8)((offset >> 8) & 0xFF);
	write_buf[4] = (u8)((offset >> 16) & 0xFF);
	write_buf[5] = (u8)((offset >> 24) & 0xFF);
	
	if(i2c_transfer_write(write_buf, 6) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}
	dbgmsg("%s - Addr [0x%06X]\n", __func__, offset);

	/* Size */
	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_SIZE;
	write_buf[2] = (u8)(length & 0xFF);
	write_buf[3] = (u8)((length >> 8) & 0xFF);
	write_buf[4] = (u8)((length >> 16) & 0xFF);
	write_buf[5] = (u8)((length >> 24) & 0xFF);
	if(i2c_transfer_write(write_buf, 6) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}
	dbgmsg("%s - Size [%d]\n", __func__, length);

	/* Data */
	for (buf_offset = 0; buf_offset < length; buf_offset += BL_PACKET_SIZE)
	{
		write_buf[0] = (u8)(((buf_addr + buf_offset) >> 8) & 0xFF);
		write_buf[1] = (u8)((buf_addr + buf_offset) & 0xFF);
		memcpy(&write_buf[2], &data[buf_offset], BL_PACKET_SIZE);
		if(i2c_transfer_write(write_buf, (2 + BL_PACKET_SIZE)) == false)
		{
			dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
			goto error;
		}
		dbgmsg("%s - PacketSize [%d] BufOffset [0x%08X]\n", __func__, BL_PACKET_SIZE, buf_offset);
	}

	/* Command */
	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_CMD;
	write_buf[2] = MIP4_BOOT_CMD_WRITE;
	if(i2c_transfer_write(write_buf, 3) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}

	/* Status */
	if (bl_read_status() != 0) 
	{
		goto error;
	}

	dbgmsg("%s [DONE]\n", __func__);
	return 0;

error:
	dbgmsg("%s [ERROR]\n", __func__);
	return 1;
}

/*
* Bootloader - Read page
*/
static int bl_read_page(int offset, u8 *data, int length, int buf_addr)
{
	char write_buf[8];
	char read_buf[BL_PACKET_SIZE];
	int buf_offset = 0;

	dbgmsg("%s [START]\n", __func__);

	/* Addr */
	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_TARGET_ADDR;
	write_buf[2] = (u8)(offset & 0xFF);
	write_buf[3] = (u8)((offset >> 8) & 0xFF);
	write_buf[4] = (u8)((offset >> 16) & 0xFF);
	write_buf[5] = (u8)((offset >> 24) & 0xFF);
	
	//I2C_Add_Fine();	//201029 MCH	
	
	if(i2c_transfer_write(write_buf, 6) == false)
	{
	  dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}
	dbgmsg("%s - Addr [0x%06X]\n", __func__, offset);

	/* Size */
	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_SIZE;
	write_buf[2] = (u8)(length & 0xFF);
	write_buf[3] = (u8)((length >> 8) & 0xFF);
	write_buf[4] = (u8)((length >> 16) & 0xFF);
	write_buf[5] = (u8)((length >> 24) & 0xFF);
	if(i2c_transfer_write(write_buf, 6) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}
	dbgmsg("%s - Size [%d]\n", __func__, length);

	/* Command */
	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_CMD;
	write_buf[2] = MIP4_BOOT_CMD_READ;
	if(i2c_transfer_write(write_buf, 3) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}
	/* Status */
	if (bl_read_status() != 0) 
	{
		goto error;
	}

	/* Read */
	for (buf_offset = 0; buf_offset < length; buf_offset += BL_PACKET_SIZE) 
	{
		write_buf[0] = (u8)(((buf_addr + buf_offset) >> 8) & 0xFF);
		write_buf[1] = (u8)((buf_addr  + buf_offset) & 0xFF);

		if(i2c_transfer_read(write_buf, 2, read_buf, BL_PACKET_SIZE) == false)
		{
			dbgmsg("%s [ERROR] i2c_transfer\n", __func__);
			goto error;
		}
		memcpy(&data[buf_offset], read_buf, BL_PACKET_SIZE);
		dbgmsg("%s - PacketSize [%d] BufOffset [0x%08X]\n", __func__, BL_PACKET_SIZE, buf_offset);
	}

	dbgmsg("%s [DONE]\n", __func__);
	return 0;
	
	 error:
	dbgmsg("%s [ERROR]\n", __func__);
	return 1;
}
/*
* Bootloader - Erase page
*/
static int bl_erase_page(unsigned int addr)
{
	char write_buf[6];

	dbgmsg("%s [START]\n", __func__);

	/* Addr */
	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_TARGET_ADDR;
	write_buf[2] = (u8)(addr & 0xFF);
	write_buf[3] = (u8)((addr >> 8) & 0xFF);
	write_buf[4] = (u8)((addr >> 16) & 0xFF);
	write_buf[5] = (u8)((addr >> 24) & 0xFF);
	
	I2C_Add_Fine();// 201029 MCH 210906
	
	if(i2c_transfer_write(write_buf, 6) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}
	dbgmsg("%s - Addr [0x%06X]\n", __func__, addr);

	/* Size */
	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_SIZE;
	write_buf[2] = (u8)(BL_PAGE_SIZE & 0xFF);
	write_buf[3] = (u8)((BL_PAGE_SIZE >> 8) & 0xFF);
	write_buf[4] = (u8)((BL_PAGE_SIZE >> 16) & 0xFF);
	write_buf[5] = (u8)((BL_PAGE_SIZE >> 24) & 0xFF);
	if(i2c_transfer_write(write_buf, 6) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}
	dbgmsg("%s - Size [%d]\n", __func__, BL_PAGE_SIZE);

	/* Command */
	write_buf[0] = MIP4_R0_BOOT;
	write_buf[1] = MIP4_R1_BOOT_CMD;
	write_buf[2] = MIP4_BOOT_CMD_ERASE;
	if(i2c_transfer_write(write_buf, 3) == false)
	{
		dbgmsg("%s [ERROR] i2c_master_send\n", __func__);
		goto error;
	}

	/* Status */
	if (bl_read_status() != 0) 
	{
		goto error;
	}
	dbgmsg("%s [DONE]\n", __func__);
	return 0;

error:
	dbgmsg("%s [ERROR]\n", __func__);
	return 1;
}
/*
* Bootloader - Enter bootloader mode
*/
static int bl_enter(void)
{
	dbgmsg("%s [START]\n", __func__);

	if (bl_change_mode(MIP4_BOOT_MODE_BOOT) != 0) 
	{
		goto error;
	}

	dbgmsg("%s [DONE]\n", __func__);
	return 0;

error:
	dbgmsg("%s [ERROR]\n", __func__);
	return 1;
}
/*
* Bootloader - Exit bootloader mode
*/
static int bl_exit(void)
{
	dbgmsg("%s [START]\n", __func__);
	uint8_t cnt;

	for(cnt=0; cnt <10; cnt++)
	{	
	if(bl_change_mode(MIP4_BOOT_MODE_APP) == 0)break;
	}	
	
	if(cnt >= 10)return 1;
	else return 0;
	
/*if (bl_change_mode(MIP4_BOOT_MODE_APP) != 0) 
	{
		goto error;
	}

	////MSG("%s [DONE]\n", __func__);
	return 0;

error:
	////MSG("%s [ERROR]\n", __func__);
	return 1;*/
}
/*
* Reset chip
*/
static void mip4_ts_reset(void)
{
	dbgmsg("%s [START]\n", __func__);
	
	/*VCC3_ON(true);
	Delay_ms(10);
	VTSP_OFF(true);
	Delay_ms(50);
	EvccControl(1,false);
	EvccControl(2,false);
	FuncRYControl(255,false,100); // Relay All Off
	FuncRYControl(1,true,0);
	FuncRYControl(2,true,100);
	FuncRYControl(6,true,0);
	FuncRYControl(7,true,300); // VIN 방전
	
	FuncRYControl(1,false,0);
	FuncRYControl(2,false,100);
	FuncRYControl(6,false,0);
	FuncRYControl(7,false,100); // VIN 방전
	
	FuncRYControl(2,true,50);
	FuncRYControl(1,true,0);
	FuncRYControl(4,true,0);
	FuncRYControl(5,true,250);
	EvccControl(1,true);
	EvccControl(2,true);
	VTSP_OFF(false);
	Delay_ms(50);
	VCC3_ON(false);
	Delay_ms(500);*/
	dbgmsg("%s [DONE]\n", __func__);
}
/*
* Read chip firmware version
*/
int mip4_ts_get_fw_version(u8 *ver_buf)
{
	//u8 rbuf[8];
	//u8 wbuf[2];
	
	char rbuf[8];
	char wbuf[2];
	
	
	int i;

	wbuf[0] = MIP4_R0_INFO;
	wbuf[1] = MIP4_R1_INFO_VERSION_BOOT;
	if(i2c_transfer_read(wbuf, 2, rbuf, 8) == false)
	{
		goto error;
	};
	for (i = 0; i < FW_MAX_SECT_NUM; i++) 
	{
		ver_buf[0 + i * 2] = rbuf[1 + i * 2];
		ver_buf[1 + i * 2] = rbuf[0 + i * 2];
	}
	return 0;

error:
	for (i = 0; i < (FW_MAX_SECT_NUM * 2); i++) 
	{
		ver_buf[i] = 0xFF;
	}

	////MSG("%s [ERROR]\n", __func__);
	return 1;
}
/*
* Read chip firmware version for u16
*/
int mip4_ts_get_fw_version_u16(u16 *ver_buf_u16)
{
	u8 rbuf[8];
	int i;

	if (mip4_ts_get_fw_version(rbuf))
	{
		goto error;
	}

	for (i = 0; i < FW_MAX_SECT_NUM; i++) 
	{
		ver_buf_u16[i] = (rbuf[0 + i * 2] << 8) | rbuf[1 + i * 2];
	}

	return true;

error:
	for (i = 0; i < FW_MAX_SECT_NUM; i++) 
	{
		ver_buf_u16[i] = 0xFFFF;
	}

	dbgmsg("%s [ERROR]\n", __func__);
	return false;
}
/*
* Read chip crc
*/
int mip4_ts_get_crc(u8 *crc_buf)
{
	//u8 rbuf[8];
	//u8 wbuf[2];
	char  rbuf[8];
	char  wbuf[2];
	
	int i;

	wbuf[0] = MIP4_R0_INFO;
	wbuf[1] = MIP4_R1_INFO_CHECKSUM_PRECALC;
	if(i2c_transfer_read(wbuf, 2, rbuf, 2) == false)
	{
		goto error;
	};

	for (i = 0; i < 1; i++) 
	{
		crc_buf[0 + i * 2] = rbuf[1 + i * 2];
		crc_buf[1 + i * 2] = rbuf[0 + i * 2];
	}

	return 0;

error:
	for (i = 0; i < (1 * 2); i++) 
	{
		crc_buf[i] = 0xFF;
	}

	dbgmsg("%s [ERROR]\n", __func__);
	return 1;
}

/*
* Read chip firmware version for u16
*/
int mip4_ts_get_crc_u16(u16 *crc_buf_u16)
{
	u8 rbuf[8];
	int i;

	if (mip4_ts_get_crc(rbuf))
	{
		goto error;
	}

	for (i = 0; i < 1; i++) 
	{
		crc_buf_u16[i] = (rbuf[0 + i * 2] << 8) | rbuf[1 + i * 2];
	}

	return true;

error:
	for (i = 0; i < 1; i++) 
	{
		crc_buf_u16[i] = 0xFFFF;
	}

	dbgmsg("%s [ERROR]\n", __func__);
	return false;
}

int mip4_ts_get_product(u8 *rbuf)
{
	//u8 wbuf[2];
	char wbuf[2];
	
	int i;

	wbuf[0] = MIP4_R0_INFO;
	wbuf[1] = MIP4_R1_INFO_PRODUCT_NAME;
	if(i2c_transfer_read(wbuf, 2, rbuf, 16) == false)
	{
		goto error;
	};

	return true;

error:
	for (i = 0; i < 16; i++) 
	{
		rbuf[i] = 0xFF;
	}

	////MSG("%s [ERROR]\n", __func__);
	return false;
}

/*
* Flash bootloader or full image
*/
int mip4_ts_flash_bl(uint8_t *fw_data, uint32_t fw_size, char force)
{
	int ret=0, retry=3, flash_addr = 0, bin_addr = 0, bin_size = 0, per = 0;
	int page_size,packet_size;
	
	char text[50]; //210510 MCH
	
	melfas_bin_tail *bin_info;
	
	u8 rbuf[BL_PAGE_SIZE];
	u8 *bin_data;
	u8 tail_mark[4] = MELFAS_BIN_TAIL_MARK;	
	
	u16 tail_size = 0;
	u16 ver_chip[FW_MAX_SECT_NUM];
	
	if(I2C_Add_Fine() == false)
	{
		//ListView_MEAS("[ERROR] wrong tail size");
		ret = fw_err_download;
		goto exit;
	} //210906
	
	dbgmsg("%s [START]\n", __func__);
	
	/* Check tail size */
	tail_size = (fw_data[fw_size - 5] << 8) | fw_data[fw_size - 6];
			
	dbgmsg("Tail Size : [%d], Mefas Tail Size : [%d]\n",tail_size, MELFAS_BIN_TAIL_SIZE);		
	if (tail_size != MELFAS_BIN_TAIL_SIZE) 
	{ 				
		dbgmsg("%s [ERROR] wrong tail size [%d]\n", __func__, tail_size);
		ListView_MEAS("[ERROR] wrong tail size");
		ret = fw_err_file_type;
		goto error_file;
	}
	
	/* Check bin format */
	if (memcmp(&fw_data[fw_size - tail_size], tail_mark, 4)) 
	{
		dbgmsg("%s [ERROR] wrong tail mark\n", __func__);
		ListView_MEAS("[ERROR] wrong tail mark");
		ret = fw_err_file_type;
		goto error_file;
	}
	
	/* Read bin info */
	bin_info = (melfas_bin_tail *)&fw_data[fw_size - tail_size];
	
	/* Check bin size */
	if (bin_info->bin_start_addr + bin_info->bin_length == fw_size) 
	{
		bin_size = bin_info->bin_start_addr + bin_info->bin_length;
	}	
	else
	{
		dbgmsg("%s [ERROR] wrong bin size\n", __func__);
		ListView_MEAS("[ERROR] wrong bin size");
		ret = fw_err_file_type;
		goto error_file;
	}
	
	if (bin_size > FLASH_SIZE) 
	{
		dbgmsg("%s [ERROR] bin size overflow\n", __func__);
		ListView_MEAS("[ERROR] bin size overflow");
		ret = fw_err_file_type;
		goto error_file;
	}
	
	dbgmsg("Bin version [0x%04X 0x%04X 0x%04X 0x%04X]\n", bin_info->ver_boot, bin_info->ver_core, bin_info->ver_app, bin_info->ver_param);	
	if (force == true) 
	{
		/* Force update */
		dbgmsg("Force update\n");
		ListView_MEAS("Force update");
	}
	else
	{
		/* Check status */
		retry = 3;
		while (retry--) 
		{
			if (mip4_ts_get_fw_version_u16(ver_chip)) 
			{
				mip4_ts_reset();
			}
			else 
			{
				break;
			}
		}
		if (retry < 0) 
		{
			dbgmsg("%s [ERROR] Unknown status\n", __func__);
			dbgmsg("Need to flash bootloader.\n");
		} 
		else 
		{
			dbgmsg("Chip firmware version [0x%04X 0x%04X 0x%04X 0x%04X]\n", ver_chip[0], ver_chip[1], ver_chip[2], ver_chip[3]);
  
			/* Check version */
			if ((ver_chip[0] == 0xFF) && (ver_chip[1] == 0xFF) && (ver_chip[2] == 0xFF) && (ver_chip[3] == 0xFF)) 
			{
				dbgmsg("Need to flash bootloader.\n");
			}
			else 
			{
				ret = fw_err_none;
				goto exit;
			}
		}
	}
  
	/* Read bin data */
	bin_data = (((uint8_t *)INPUTDATA_ADDRESS));
	
	/* Enter ISC mode */	
	dbgmsg("%s - Enter ISC mode\n", __func__);
	ListView_MEAS("Enter ISC mode");
	
	page_size = ISC_PAGE_SIZE;
	packet_size = page_size;	
	dbgmsg("%s - Size : page[%d] packet[%d]\n", __func__, page_size, packet_size);
	
	retry = 3;
	while (retry--) 
	{
		if (isc_enter() != 0) 
		{
			dbgmsg("%s [ERROR] isc_enter\n", __func__);
			ListView_MEAS("[ERROR] isc_enter");
		}
		else 
		{
			break;
		}
	}
	
	if (retry < 0) 
	{
		dbgmsg("%s [ERROR] isc_enter retry limit\n", __func__);
		ListView_MEAS("[ERROR] isc_enter retry limit");
		
		ret = fw_err_download;
		goto error_update;
	}
	
	/* Enable */
	ListView_MEAS("Enable");
  
	dbgmsg("%s - Enable\n", __func__);  
	ret = isc_enable();
  
	if (ret != 0) 
	{  
		dbgmsg("%s [ERROR] isc_enable\n", __func__);
  
		ret = fw_err_download;
  		ListView_MEAS("[ERROR] Enable");
		
		goto error_update;
	}   
	/* Erase */
	ListView_MEAS("Erase");	
	for (flash_addr = 0; flash_addr < FLASH_SIZE; flash_addr += page_size) 
	{	
		per = (int)(((float)flash_addr / (float)FLASH_SIZE)*100.0);
		dbgmsg("%s - Erase page : addr[0x%08X]\n", __func__, flash_addr);
		if(per % 10 == 0)
		{	
			sprintf(text,"Erase : %d %%", per);		
			ListView_MEAS(text);	
		}			
  
		if (isc_erase_page(flash_addr)) 
		{
			dbgmsg("%s [ERROR] isc_erase_page : addr[0x%08X]\n", __func__, flash_addr);
			sprintf(text,"[ERROR] Erase : 0x%08X", flash_addr);
			ListView_MEAS(text);
			goto error_update;
		}
	}
	
	/* Write & Verify */		
		ListView_MEAS("Write & Verify");	
		dbgmsg("%s - Write & Verify\n", __func__);
		for (flash_addr = bin_size - page_size; flash_addr >= 0; flash_addr -= packet_size) 
		{
			bin_addr = flash_addr;

			dbgmsg("Write & Verify Addr = 0x%08X\n", bin_addr);
			
			per = (int)(((float)(FLASH_SIZE-flash_addr) / (float)FLASH_SIZE)*100.0);
			if(per % 10 == 0)
			{	
				sprintf(text,"Write/Verify : %d %%", per);		
				ListView_MEAS(text);	
			}	

			/* Write */
			dbgmsg("%s - Write : addr - bin[0x%08X] flash[0x%08X]\n", __func__, bin_addr, flash_addr);		
			if (isc_write_page(flash_addr, &bin_data[bin_addr], page_size)) 
			{
				dbgmsg("%s [ERROR] isc_write_page : addr - bin[0x%08X] flash[0x%08X]\n", __func__, bin_addr, flash_addr);
				sprintf(text,"[ERROR] Write : 0x%08X", flash_addr);
				ListView_MEAS(text);
				ret = fw_err_download;
				goto error_update;
			}
			
		/* Verify */
		dbgmsg("%s - Read : addr - bin[0x%08X] flash[0x%08X]\n", __func__, bin_addr, flash_addr);
		if (isc_read_page(flash_addr, rbuf)) 
		{
			dbgmsg("%s [ERROR] isc_read_page : addr - bin[0x%08X] flash[0x%08X]\n", __func__, bin_addr, flash_addr);
			sprintf(text,"[ERROR] Read : 0x%08X", flash_addr);
			ListView_MEAS(text);
			ret = fw_err_download;
			goto error_update;
		}
		
		if (memcmp(rbuf, &bin_data[bin_addr], page_size)) 
		{
			dbgmsg("%s [ERROR] Verify failed : addr - bin[0x%08X] flash[0x%08X]\n", __func__, bin_addr, flash_addr);
			sprintf(text,"[ERROR] Verify : 0x%08X", flash_addr);
			ListView_MEAS(text);
			ret = fw_err_download;
			
			goto error_update;
		}
	}	
	/* Exit download mode */
	
	ListView_MEAS("Exit ISC mode");
	dbgmsg("%s - Exit ISC mode\n", __func__);
	if (isc_exit() != 0) 
	{
		ListView_MEAS("[ERROR] isc_exit");
		dbgmsg("%s [ERROR] isc_exit\n", __func__);
		ret = fw_err_download;
		goto error_update;
	}
	//Delay_ms(100);//MCH
	/* Reset chip */
	mip4_ts_reset();
	free(bin_data);
	ListView_MEAS("OK");
	goto exit;
	error_update:
	free(bin_data);
	/* Reset chip */
	mip4_ts_reset();
	goto exit;
	error_file:
	dbgmsg("%s [ERROR]\n", __func__);
	goto exit;
	exit:
	dbgmsg("%s [DONE]\n", __func__);
		
	if(ret == 0)	ret = PASS_RESULT; 
	else			ret = FAIL_RESULT;
	
	return ret;
}

uint8_t PID_RD_H = 0; 
uint8_t PID_RD_L = 0;
uint8_t VID_RD_H = 0;
uint8_t VID_RD_L = 0;
uint8_t SA_RD_H  = 0;	
uint8_t SA_RD_L  = 0;

uint8_t PID_WR_H = 0; 
uint8_t PID_WR_L = 0;
uint8_t VID_WR_H = 0;
uint8_t VID_WR_L = 0;
uint8_t SA_WR_H  = 0;	
uint8_t SA_WR_L  = 0;

int mip4_ts_Configuration(void)
{
	struct melfas_bin_tail *bin_info;
	int ret = 0;
	int retry = 3;
	int i;
	u8 rbuf[BL_PAGE_SIZE];
	u8 tbuf[BL_PAGE_SIZE];
	int flash_addr = 0;
	int flash_addr_start = 0;
	int flash_addr_end = 0;
	int flash_addr_offset = 0;
	int bin_addr = 0;
	int bin_addr_start = 0;
	int bin_addr_end = 0;
	int bin_size = 0;
	u8 *bin_data;
	u16 tail_size = 0;
	u8 tail_mark[4] = MELFAS_BIN_TAIL_MARK;
	u16 ver_chip[FW_MAX_SECT_NUM];
	u16 buf_addr = 0;
	int page_size;
	int packet_size;
	char text[100];
	float per = 0;
	
	/*if(I2C_Add_Fine() == false)
	{
		ret = fw_err_download;
		goto error_update;
	}*/ //210906 
	dbgmsg("%s [START]\n", __func__);
	
	/* Enter bootloader mode */
	dbgmsg("%s - Enter bootloader mode\n", __func__);
	ListView_MEAS("Enter bootloader mode");

	page_size = BL_PAGE_SIZE;
	packet_size = BL_PACKET_SIZE;
	dbgmsg("%s - Size : page[%d] packet[%d]\n", __func__, page_size, packet_size);
	retry = 3;
	while (retry--) 
	{
		if (bl_enter() != 0) 
		{
			dbgmsg("%s [ERROR] bl_enter\n", __func__);
			ListView_MEAS("[ERROR] bl_enter");
		}
		else 
		{
			break;
		}
	}

	if (retry < 0) 
	{
		dbgmsg("%s [ERROR] bl_enter retry limit\n", __func__);
		ListView_MEAS("[ERROR] bl_enter retry limit");
		ret = fw_err_download;
		goto error_update;
	}
	
	/* Read info */
	if (bl_read_info(&buf_addr)) 
	{
		dbgmsg("%s [ERROR] bl_read_info\n", __func__);
		ListView_MEAS("[ERROR] bl_read_info");
		ret = fw_err_download;
		goto error_update;
	}
	dbgmsg("%s - Buffer Addr [0x%08X]\n", __func__, buf_addr);
	sprintf(text,"Buffer Addr [0x%08X]\n", buf_addr);
	ListView_MEAS(text);
	flash_addr_start = 0x1FE00;
	flash_addr_end   = flash_addr_start + 512;	
	ListView_MEAS("Erase");	
	
	/* Erase */
	for (flash_addr = flash_addr_end - page_size; flash_addr >= flash_addr_start; flash_addr -= page_size) 
	{
		dbgmsg("%s - Erase page : addr[0x%08X]\n", __func__, flash_addr);
		if (bl_erase_page(flash_addr)) 
		{
			dbgmsg( "%s [ERROR] bl_erase_page : addr[0x%08X]\n", __func__, flash_addr);
			sprintf(text,"[ERROR] Erase : 0x%08X", flash_addr);
			ListView_MEAS(text);
			
			ret = fw_err_download;
			goto error_update;
		}
	}

	for (i = 0; i < BL_PAGE_SIZE; i++)		tbuf[i] = 0xff;
	
	tbuf[0]  = PID_WR_L; 
	tbuf[1]  = PID_WR_H; 
	tbuf[2]  = 0x00; 
	tbuf[3]  = 0x00;
	tbuf[4]  = VID_WR_L; 
	tbuf[5]  = VID_WR_H; 
	tbuf[6]  = 0x00; 
	tbuf[7]  = 0x00;
	tbuf[8]  = SA_WR_L;  
	tbuf[9]  = SA_WR_H;  
	tbuf[10] = 0x00; 
	tbuf[11] = 0x00;
	
	dbgmsg("%s - Write & Verify\n", __func__);
	dbgmsg("%s - Size : Page[%d] Packet[%d]\n", __func__, page_size, packet_size);
	
	flash_addr_start = 0x1FE00;
	flash_addr_end   = flash_addr_start + page_size;
	flash_addr = flash_addr_start;
	
	/* Write */
	dbgmsg("%s - Write : addr - bin[0x%08X] flash[0x%08X]\n", __func__, bin_addr, flash_addr);	
	ListView_MEAS("Write");	
	if (bl_write_page(flash_addr, &tbuf[bin_addr], page_size, buf_addr)) 
	{
		dbgmsg("%s [ERROR] bl_write_page : addr - bin[0x%08X] flash[0x%08X]\n", __func__, bin_addr, flash_addr);
		sprintf(text,"[ERROR] Write : 0x%08X", flash_addr);
		ListView_MEAS(text);
		ret = fw_err_download;
		goto error_update;
	}
	/* Verify */
	dbgmsg("%s - Read : addr - bin[0x%08X] flash[0x%08X]\n", __func__, bin_addr, flash_addr);
	ListView_MEAS("Read");	
	if (bl_read_page(flash_addr, rbuf, page_size, buf_addr))
	{
		dbgmsg("%s [ERROR] bl_read_page : addr - bin[0x%08X] flash[0x%08X]\n", __func__, bin_addr, flash_addr);
		sprintf(text,"[ERROR] Read : 0x%08X", flash_addr);
		ListView_MEAS(text);
		ret = fw_err_download;
		goto error_update;
	}		

	ListView_MEAS("Verify");	
	if (memcmp(rbuf, &tbuf[bin_addr], page_size)) 
	{
		dbgmsg("%s [ERROR] Verify failed : addr - bin[0x%08X] flash[0x%08X]\n", __func__, bin_addr, flash_addr);
		sprintf(text,"[ERROR] Verify : 0x%08X", flash_addr);
		
		ListView_MEAS(text);
		ret = fw_err_download;
		goto error_update;
	}
	/* Exit download mode */
	dbgmsg("%s - Exit bootloader mode\n", __func__);
	ListView_MEAS("Exit bootloader mode");

	
	if (bl_exit() != 0) 
	{
		dbgmsg("%s [ERROR] bl_exit\n", __func__);
		ListView_MEAS("[ERROR] bl_exit");
		ret = fw_err_download;
		goto error_update;
	}

	/* Reset chip */
	mip4_ts_reset();
	ListView_MEAS("OK");

	uptodate:
	goto exit;
	
	error_update:
	free(bin_data);

	/* Reset chip */
	mip4_ts_reset();
	
	goto exit;
	error_file:
	dbgmsg("%s [ERROR]\n", __func__);

	goto exit;
	
	exit:
	
	dbgmsg("%s [DONE]\n", __func__);
	
	if(ret == 0)	ret = true; 
	else			ret = false;

	return ret;	
}

int mip4_config_Read(void)
{	
	struct melfas_bin_tail *bin_info;
	int ret = 0;
	int retry = 3;
	int i;
	u8 rbuf[BL_PAGE_SIZE];
	u8 tbuf[BL_PAGE_SIZE];
	int flash_addr = 0;
	int flash_addr_start = 0;
	int flash_addr_end = 0;
	int flash_addr_offset = 0;
	int bin_addr = 0;
	int bin_addr_start = 0;
	int bin_addr_end = 0;
	int bin_size = 0;
	u8 *bin_data;
	u16 tail_size = 0;
	u8 tail_mark[4] = MELFAS_BIN_TAIL_MARK;
	u16 ver_chip[FW_MAX_SECT_NUM];
	u16 buf_addr = 0;
	int page_size;
	int packet_size;
	char text[100];
	float per = 0;
	
	if(I2C_Add_Fine() == false)
	{	
		ret = fw_err_download;
		goto error_update;
	} //210906
	
	dbgmsg("%s [START]\n", __func__);
	
	/* Enter bootloader mode */
	dbgmsg("%s - Enter bootloader mode\n", __func__);
	ListView_MEAS("Enter bootloader mode");

	page_size = BL_PAGE_SIZE;
	packet_size = BL_PACKET_SIZE;
	dbgmsg("%s - Size : page[%d] packet[%d]\n", __func__, page_size, packet_size);
	retry = 3;
	
	while (retry--) 
	{
		if (bl_enter() != 0) 
		{
			dbgmsg("%s [ERROR] bl_enter\n", __func__);
			ListView_MEAS("[ERROR] bl_enter");
		}
		else 
		{
			break;
		}
	}
	
	if (retry < 0) 
	{
		dbgmsg("%s [ERROR] bl_enter retry limit\n", __func__);
		ListView_MEAS("[ERROR] bl_enter retry limit");
		ret = fw_err_download;
		goto error_update;
	}
	
	/* Read info */
	if (bl_read_info(&buf_addr)) 
	{
		dbgmsg("%s [ERROR] bl_read_info\n", __func__);
		ListView_MEAS("[ERROR] bl_read_info");
		ret = fw_err_download;
		goto error_update;
	}
	
	dbgmsg("%s - Buffer Addr [0x%08X]\n", __func__, buf_addr);
	sprintf(text,"Buffer Addr [0x%08X]\n", buf_addr);
	ListView_MEAS(text);
	
	flash_addr_start = 0x1FE00;
	flash_addr_end   = flash_addr_start + page_size;
	flash_addr = flash_addr_start;
	
	/* Verify */
	dbgmsg("%s - Read : addr - bin[0x%08X] flash[0x%08X]\n", __func__, bin_addr, flash_addr);
	sprintf(text,"Read : 0x%08X", flash_addr);
	ListView_MEAS(text);
	
	if (bl_read_page(flash_addr, rbuf, page_size, buf_addr))
	{
		dbgmsg("%s [ERROR] bl_read_page : addr - bin[0x%08X] flash[0x%08X]\n", __func__, bin_addr, flash_addr);
		sprintf(text,"[ERROR] Read : 0x%08X", flash_addr);
		ListView_MEAS(text);
		ret = fw_err_download;
		goto error_update;
	}		


	PID_RD_H = rbuf[1]; 
	PID_RD_L = rbuf[0];
	VID_RD_H = rbuf[5];
	VID_RD_L = rbuf[4];
	SA_RD_H  = rbuf[9];	
	SA_RD_L  = rbuf[8];
		
	dbgmsg("PID_RD_H : %02X, PID_RD_L : %02X, VID_RD_H : %02X, VID_RD_L : %02X, SA_RD_H : %02X, SA_RD_L : %02X\n",PID_RD_H,PID_RD_L,VID_RD_H,VID_RD_L,SA_RD_H,SA_RD_L);	
	
	/* Exit download mode */
	dbgmsg("%s - Exit bootloader mode\n", __func__);
	ListView_MEAS("Exit bootloader mode");

	if (bl_exit() != 0) 
	{
		dbgmsg("%s [ERROR] bl_exit\n", __func__);
		ListView_MEAS("[ERROR] bl_exit");
		ret = fw_err_download;
		goto error_update;
	}

	/* Reset chip */
	mip4_ts_reset();
	
	ListView_MEAS("OK");

	uptodate:
	goto exit;

	error_update:
	free(bin_data);

	/* Reset chip */
	mip4_ts_reset();
	
	goto exit;
	
	
	error_file:
	dbgmsg("%s [ERROR]\n", __func__);
	goto exit;
	

	exit:
	if(ret == 0)	ret = true; 
	else			ret = false;
	
	return ret;	
}

static void gpio_init(uint16_t index)
{
	PID_RD_H = PID_RD_L = 0;
	VID_RD_H = VID_RD_L = 0;
	SA_RD_H  = SA_RD_L  = 0;
	
	PID_WR_H = PID_WR_L = 0;
	VID_WR_H = VID_WR_L = 0;
	SA_WR_H  = SA_WR_L  = 0;
	
	GPIO_InitTypeDef GPO;
	GPO.GPIO_Mode  = GPIO_Mode_OUT;
	//GPO.GPIO_OType = GPIO_OType_OD;
	GPO.GPIO_OType = GPIO_OType_PP;
	GPO.GPIO_Speed = GPIO_Speed_100MHz;
	GPO.GPIO_PuPd  = GPIO_PuPd_UP;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//	GPO.GPIO_Pin = GPIO_Pin_10;	    GPIO_Init(GPIOB, &GPO);
//	GPO.GPIO_Pin = GPIO_Pin_11;	    GPIO_Init(GPIOB, &GPO);

	//EN_PIN(0);
	//RST_PIN(0);
}
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
void MFS_Power_On(void)
{
	RYBD_1ON(true);
	Delay_ms(100);
	RYBD_2ON(true);
	Delay_ms(100);
	RYBD_3ON(true);
	Delay_ms(100);
	RYBD_4ON(true);
	Delay_ms(100);
	RYBD_5ON(true);
	Delay_ms(100);
	RYBD_6ON(true);
	Delay_ms(100);
	
	FuncRYControl(1,true,0);//GND Contact
    FuncRYControl(2,false,0);//GND - EEP WP Contact
	FuncRYControl(3,false,0);//SCL SDA
    FuncRYControl(4,true,0);//SWCLK, SWDIO - GND Contact
    FuncRYControl(5,true,0);//USB_M,P
    FuncRYControl(6,false,250);//
	EvccControl(1,false);// 1.8V Off
	EvccControl(2,false);// 3.3V ON (Level Shift Voltage)
	Delay_ms(100);
	
	DVDD_ON(true);
	Delay_ms(5);
	AVDD_ON(true);
	Delay_ms(1000); 
	
	
	
	//------------------ 221108 MCH
	/*RYBD_1ON(true);
	Delay_ms(100);
	RYBD_2ON(true);
	Delay_ms(100);
	RYBD_3ON(true);
	Delay_ms(100);
	RYBD_4ON(true);
	Delay_ms(100);
	FuncRYControl(2,true,50);
	FuncRYControl(1,true,0);
	FuncRYControl(4,true,0);
	FuncRYControl(5,true,250);
	EvccControl(1,true);
	EvccControl(2,true);
	VTSP_OFF(false);
	Delay_ms(50);
	VCC3_ON(false);
	Delay_ms(500);*/
	//----------------- 221108 MCH
}
void MFS_Power_Off(void)
{
	//----------------------------221108 MCH
	AVDD_ON(false);
	Delay_ms(50);
	DVDD_ON(false);
	Delay_ms(100);
	
	Discharge1_ON(true);
	Discharge2_ON(true);
	Delay_ms(500);  
	Discharge1_ON(false);
	Discharge2_ON(false);
	Delay_ms(50);
	
	FuncRYControl(255,false,0);
	
	RYBD_1ON(false);
	Delay_ms(100);
	RYBD_2ON(false);
	Delay_ms(100);
	RYBD_3ON(false);
	Delay_ms(100);
	RYBD_4ON(false);
	Delay_ms(100);
	RYBD_5ON(false);
	Delay_ms(100);
	RYBD_6ON(false);
	Delay_ms(100);
	
	
	//-----------------------------
	//-----잔류전원 제거 Flow
	//------------------
}
uint8_t MFF_INIT(uint16_t cnt)
{ 
	dbgmsg("%s\n",__func__); 
	MFS_Power_On();
	Delay_ms(500);
	ListView_MEAS("OK");

	return true;
}
uint8_t MFF_FirmwareWrite(uint16_t cnt)
{
	dbgmsg("%s\n",__func__);
	
	if(RD_FW_WRITE == false)
    {
        ListView_MEAS("SKIP");
        return SKIP_RESULT;
    }
		else
		{
			ListView_RESULT(USB_WRITE,AUTO_STATE);
		}
		
	uint8_t ret = mip4_ts_flash_bl(INPUT_DATA,In_data.size,true);
	
	if( ret == true) 	ListView_MEAS("Download Pass");
	else				ListView_MEAS("Download Fail");
	
	return ret;
}
uint8_t MFF_VID_INIT(uint16_t cnt)
{
	char text[50];
	
	VID_WR_H = func_spec_hex[cnt][2]; 	
	VID_WR_L = func_spec_hex[cnt][3];
	
	sprintf(text,"OK(%02X%02X)",VID_WR_H,VID_WR_L);
	ListView_MEAS(text);

	return true;
}
uint8_t MFF_PID_INIT(uint16_t cnt)
{
	char text[50];
	
	PID_WR_H = func_spec_hex[cnt][2]; 		
	PID_WR_L = func_spec_hex[cnt][3];
	
	sprintf(text,"OK(%02X%02X)",PID_WR_H,PID_WR_L);
	ListView_MEAS(text);

	return true;
}
uint8_t MFF_SA_INIT(uint16_t cnt)
{
	char text[50];
	
	SA_WR_H = func_spec_hex[cnt][2]; 		
	SA_WR_L = func_spec_hex[cnt][3];
	
	sprintf(text,"OK(%02X%02X)",SA_WR_H,SA_WR_L);
	ListView_MEAS(text);

	return true;
}

uint8_t MFF_ConfigWrite(uint16_t cnt)
{

	dbgmsg("%s\n",__func__);
	if(RD_FW_WRITE == false)
    {
        ListView_MEAS("SKIP");
        return SKIP_RESULT;
    }   
	if(mip4_ts_Configuration() == false)
	{
        ListView_MEAS("NG");
		return false;
	}
	else
	{
        ListView_MEAS("OK");
		return true;
	}
	return true;
}
uint8_t MFF_ConfigRead(uint16_t cnt)
{
	if(mip4_config_Read() == false)
	{
        ListView_MEAS("NG");
		return false;
	}
	else
	{
        ListView_MEAS("OK");
		return true;
	}
	return true;
}
uint8_t MFF_VID_CHECK(uint16_t cnt)
{	
	uint8_t ret = 0;
	uint8_t VER_H = 0;
	uint8_t VER_L = 0;
	
	char text[50];
		
	VER_H = VID_RD_H; 		VER_L = VID_RD_L;
	
	sprintf(text,"%02X%02X/%02X%02X",VER_H,VER_L,func_spec_hex[cnt][2],func_spec_hex[cnt][3]);
	ListView_MEAS(text);	
	
	if( !((VER_H == func_spec_hex[cnt][2]) && (VER_L == func_spec_hex[cnt][3]) ) )
	{			
		ret = false;
	}
	else
	{
		ret = true;
	}
	
	return ret;
}
uint8_t MFF_PID_CHECK(uint16_t cnt)
{
	char text[50];
	
	uint8_t ret = 0;
	uint8_t VER_H = 0;
	uint8_t VER_L = 0;
		
	VER_H = PID_RD_H; 		VER_L = PID_RD_L;
	
	sprintf(text,"%02X%02X/%02X%02X",VER_H,VER_L,func_spec_hex[cnt][2],func_spec_hex[cnt][3]);
	ListView_MEAS(text);	
	
	if( !((VER_H == func_spec_hex[cnt][2]) && (VER_L == func_spec_hex[cnt][3]) ) )
	{			
		ret = false;
	}
	else
	{
		ret = true;
	}
	
	return ret;
}
uint8_t MFF_SA_CHECK(uint16_t cnt)
{
	char text[50];
	
	uint8_t ret = 0;
	uint8_t VER_H = 0;
	uint8_t VER_L = 0;
		
	VER_H = SA_RD_H; 		VER_L = SA_RD_L;
	
	sprintf(text,"%02X%02X/%02X%02X",VER_H,VER_L,func_spec_hex[cnt][2],func_spec_hex[cnt][3]);
	ListView_MEAS(text);	
	
	if( !((VER_H == func_spec_hex[cnt][2]) && (VER_L == func_spec_hex[cnt][3]) ) )
	{			
		ret = false;
	}
	else
	{
		ret = true;
	}
	
	return ret;
}
uint8_t MFF_BOOT_CHECK(uint16_t cnt)
{	
	int ret = false;
	
	char text[50];
	uint8_t VER_H = 0;
	uint8_t VER_L = 0;

	u16 ver_chip[FW_MAX_SECT_NUM];
	
	dbgmsg("%s\n",__func__);

	if(I2C_Add_Fine() == false)
	{
		ListView_MEAS("S.A not Found");
		ret = false;
		goto exit;
	} //210906 
	
	if (mip4_ts_get_fw_version_u16(ver_chip)==false)
	{
		ListView_MEAS("Unknown chip");
		ret = false;
		goto exit;
	}
			
	VER_H = (ver_chip[0]>>8)&0xff; 		VER_L = ver_chip[0]&0xff;
	
	sprintf(text,"%02X%02X/%02X%02X",VER_H,VER_L,func_spec_hex[cnt][2],func_spec_hex[cnt][3]);
	ListView_MEAS(text);	
	
	if( !((VER_H == func_spec_hex[cnt][2]) && (VER_L == func_spec_hex[cnt][3]) ) )
	{			
		ret = false;
	}
	else
	{
		ret = true;
	}
	
	exit:
	return ret;
}
uint8_t MFF_CORE_CHECK(uint16_t cnt)
{
	int ret = false;
	char text[50];
	uint8_t VER_H = 0;
	uint8_t VER_L = 0;
	
	u16 ver_chip[FW_MAX_SECT_NUM];
	
	dbgmsg("%s\n",__func__);
	sprintf(text,"%s",__func__);
	
	if(I2C_Add_Fine() == false)
	{
		ListView_MEAS("S.A not Found");
		ret = false;
		goto exit;
	} //210906
	
	if (mip4_ts_get_fw_version_u16(ver_chip)==false)
	{
		ListView_MEAS("Unknown chip");
		ret = false;
		goto exit;
	}
				
	VER_H = (ver_chip[1]>>8)&0xff; 		VER_L = ver_chip[1]&0xff;
	
	sprintf(text,"%02X%02X/%02X%02X",VER_H,VER_L,func_spec_hex[cnt][2],func_spec_hex[cnt][3]);
	ListView_MEAS(text);	
	
	if( !((VER_H == func_spec_hex[cnt][2]) && (VER_L == func_spec_hex[cnt][3]) ) )
	{			
		ret = false;
	}
	else
	{
		ret = true;
	}
	
	exit:
	return ret;
}
uint8_t MFF_APP_CHECK(uint16_t cnt)
{
	int ret = false;
	
	char text[50];
	uint8_t VER_H = 0;
	uint8_t VER_L = 0;

	u16 ver_chip[FW_MAX_SECT_NUM];
	
	dbgmsg("%s\n",__func__);
	sprintf(text,"%s",__func__);
	
	if(I2C_Add_Fine() == false)
	{
		ListView_MEAS("S.A not Found");
		ret = false;
		goto exit;
	} //210806
	
	if (mip4_ts_get_fw_version_u16(ver_chip)==false)
	{
		ListView_MEAS("Unknown chip");
		ret = false;
		goto exit;
	}
				
	VER_H = (ver_chip[2]>>8)&0xff; 		VER_L = ver_chip[2]&0xff;
	
	sprintf(text,"%02X%02X/%02X%02X",VER_H,VER_L,func_spec_hex[cnt][2],func_spec_hex[cnt][3]);
	ListView_MEAS(text);	
	
	if( !((VER_H == func_spec_hex[cnt][2]) && (VER_L == func_spec_hex[cnt][3]) ) )
	{			
		ret = false;
	}
	else
	{
		ret = true;
	}
	
	exit:
	return ret;
}
uint8_t MFF_PARA_CHECK(uint16_t cnt)
{
	int ret = false;
	
	char text[50];
	uint8_t VER_H = 0;
	uint8_t VER_L = 0;

	u16 ver_chip[FW_MAX_SECT_NUM];
	
	dbgmsg("%s\n",__func__);
	sprintf(text,"%s",__func__);
	
	if(I2C_Add_Fine() == false)
	{
		ListView_MEAS("S.A not Found");
		ret = false;
		goto exit;
	} //210906
	
	if (mip4_ts_get_fw_version_u16(ver_chip)==false)
	{
		ListView_MEAS("Unknown chip");
		ret = false;
		goto exit;
	}
	VER_H = (ver_chip[3]>>8)&0xff; 		VER_L = ver_chip[3]&0xff;
	
	sprintf(text,"%02X%02X/%02X%02X",VER_H,VER_L,func_spec_hex[cnt][2],func_spec_hex[cnt][3]);
	ListView_MEAS(text);	
	
	if( !((VER_H == func_spec_hex[cnt][2]) && (VER_L == func_spec_hex[cnt][3]) ) )
	{			
		ret = false;
	}
	else
	{
		ret = true;
	}
	
	exit:
	return ret;
}

uint8_t MFF_CRC_CHECK(uint16_t cnt)
{	
	char text[50];
	int ret = false;
	uint8_t VER_H = 0;
	uint8_t VER_L = 0;
	
	u16 ver_chip[FW_MAX_SECT_NUM];
	
	dbgmsg("%s\n",__func__);
	sprintf(text,"%s",__func__);
	
	if(I2C_Add_Fine() == false)
	{
		ListView_MEAS("S.A not Found");
		ret = false;
		goto exit;
	} //210906
	
	if (mip4_ts_get_crc_u16(ver_chip)==false)
	{
		ListView_MEAS("Unknown chip");
		ret = false;
		goto exit;
	}
	VER_H = (ver_chip[0]>>8)&0xff; 		VER_L = ver_chip[0]&0xff;
	
	sprintf(text,"%02X%02X/%02X%02X",VER_H,VER_L,func_spec_hex[cnt][2],func_spec_hex[cnt][3]);
	ListView_MEAS(text);	
	
	if( !((VER_H == func_spec_hex[cnt][2]) && (VER_L == func_spec_hex[cnt][3]) ) )
	{			
		ret = false;
	}
	else
	{
		ret = true;
	}
	
	exit:
	return ret;
}
uint8_t MFF_PD_CHECK(uint16_t cnt)
{	
	char text[50];
	int ret = false;

	u8 pdt[16];
	
	dbgmsg("%s\n",__func__);
	sprintf(text,"%s",__func__);
	
	if(I2C_Add_Fine() == false)
	{
		ListView_MEAS("S.A not Found");
		ret = false;
		goto exit;
	} //210906
	
	if (mip4_ts_get_product(pdt)==false)
	{
		ListView_MEAS("Unknown chip");
		ret = false;
		goto exit;
	}
			
	sprintf(text,"%s / %s", pdt, func_spec_ascii[cnt]);	
	ListView_MEAS(text);
		
	if(strcmp(pdt, func_spec_ascii[cnt]) == 0)	ret = true;
	else										ret = false;
	
	
	exit:
	
	return ret;
}

uint8_t USB_FirmwareWrite(uint16_t index);

uint8_t Downwait_Flag = 0;
uint8_t receving = false;

extern void MFS_STATE_CLEAR();

uint8_t USB_FirmwareWrite(uint16_t cnt)
{
	int ret = false;
	int timeout = 700;
	int timeoutcnt_for_esc = 0;
	const int RETRY_USB = 2;
	uint8_t retry_cnt = 0;
	
	while(1)
	{
		//-----------------------------------------------
		//FuncRYControl(1,true,0);//GND Contact
		FuncRYControl(1,false,0);//GND Contact through Relay bd
		FuncRYControl(2,false,0);//GND - EEP WP Contact
		FuncRYControl(3,false,0);//SCL SDA
		//FuncRYControl(4,true,0);//SWCLK, SWDIO - GND Contact
		FuncRYControl(4,false,0);
		FuncRYControl(5,true,0);//USB_M,P
		FuncRYControl(6,false,250);//
		EvccControl(1,false);// 1.8V Off
		EvccControl(2,false);// 3.3V ON (Level Shift Voltage)
		Delay_ms(100);
		DVDD_ON(true);
		Delay_ms(10);
		AVDD_ON(true);
		RYBD_6ON(true);		// external 12V on
		Delay_ms(500); 	//Delay_ms(500); 

		switch_Hold();
		//-------------------------------------------------	

		if(RD_FW_WRITE == false)
		{
			ListView_RESULT(USB_VERIFY,AUTO_STATE);
			tx_pc_data(MFS_VERIFY, 1, 0);
		}
		else 
		{
			ListView_RESULT(USB_WRITE,AUTO_STATE);
			tx_pc_data(MFS_WRITE, 1, 0);
		}

		Delay_ms(500);

		while(1)
		{
			Downwait_Flag = MFS_STATE;

			if(Downwait_Flag ==1)
			{
				Downwait_Flag =0 ;
			}

			else if(Downwait_Flag ==2)
			{
				Downwait_Flag =0 ;

				if(RD_FW_WRITE == false)
				{
					ListView_MEAS("Verify Pass");
				}
				else ListView_MEAS("Download Pass");
				MFS_STATE_CLEAR();
				ret = true;

				//--------------------
				EvccControl(1,false); //
				EvccControl(2,false);
				Delay_ms(50);
				RYBD_6ON(false);	// external 12V off
				AVDD_ON(false);		
				Delay_ms(50);
				DVDD_ON(false);
				Delay_ms(100);

				Discharge1_ON(true);
				Discharge2_ON(true);
				Delay_ms(500);  
				Discharge1_ON(false);
				Discharge2_ON(false);
				Delay_ms(50);
				//----------------------------	

				FuncRYControl(255,0,50);	//FuncRYControl(255,0,250);

				break;
			}
			else if(Downwait_Flag ==3)
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
				MFS_STATE_CLEAR();
				ret = false;
				//-------------------
				RYBD_6ON(false);	// external 12V off
				AVDD_ON(false);
				Delay_ms(50);
				DVDD_ON(false);
				Delay_ms(100);

				Discharge1_ON(true);
				Discharge2_ON(true);
				Delay_ms(500);  
				Discharge1_ON(false);
				Discharge2_ON(false);
				Delay_ms(50);

				FuncRYControl(255,0,50);	//FuncRYControl(255,0,250);
				break;			
			}

			Delay_ms(10);    
		}
		
		if (ret == true)
			break;
		
		retry_cnt++;
		if ( retry_cnt >= RETRY_USB)
		{
			break;
		}
		else
		{
			Delay_ms(2000);
			ListView_MEAS("Retry");
			Delay_ms(2000);
		}
	}

	switch_Hold();
	return ret;
}

typedef enum
{
	MFF_ISC_INIT = 1,
	MFF_ISC_FirmwareWrite,
	MFF_ISC_VID_INIT,
	MFF_ISC_PID_INIT,
	MFF_ISC_SA_INIT,
	MFF_ISC_ConfigWrite,
	MFF_ISC_ConfigRead,
	MFF_ISC_VID_CHECK,
	MFF_ISC_PID_CHECK,
	MFF_ISC_SA_CHECK,
	MFF_ISC_BOOT_CHECK,
	MFF_ISC_CORE_CHECK,
	MFF_ISC_APP_CHECK,
	MFF_ISC_PARA_CHECK,
	MFF_ISC_CRC_CHECK,
	MFF_ISC_PD_CHECK,
	MFF_USB_FirmwareWrite = 20,
}MFF_INSP_TYPE;

uint8_t MELFAS_ISC(uint16_t index) 
{
	uint8_t ret = false;
	dbgmsg("   MFFFlash ==> %02d\n", func_module[index]);
	dbgmsg("index No : %d\n", index);
	
	switch(func_module[index]) 
	{
			case MFF_ISC_INIT :				  ret = MFF_INIT(index);			break;	// ok
			case MFF_ISC_FirmwareWrite :	  ret = MFF_FirmwareWrite(index);	break;	// ok
			case MFF_ISC_VID_INIT :			  ret = MFF_VID_INIT(index);		break;	// ok
			case MFF_ISC_PID_INIT :			  ret = MFF_PID_INIT(index);		break;	// ok
			case MFF_ISC_SA_INIT :			  ret = MFF_SA_INIT(index);		break;	// ok
			case MFF_ISC_ConfigWrite :	      ret = MFF_ConfigWrite(index);	break;
			case MFF_ISC_ConfigRead :		  ret = MFF_ConfigRead(index);	break;	// ok
			case MFF_ISC_VID_CHECK :		  ret = MFF_VID_CHECK(index);		break;	// ok
			case MFF_ISC_PID_CHECK :		  ret = MFF_PID_CHECK(index);		break;	// ok
			case MFF_ISC_SA_CHECK :			  ret = MFF_SA_CHECK(index);		break;	// ok
			case MFF_ISC_BOOT_CHECK :		  ret = MFF_BOOT_CHECK(index);	break;	// ok
			case MFF_ISC_CORE_CHECK :		  ret = MFF_CORE_CHECK(index);	break;	// ok
			case MFF_ISC_APP_CHECK :		  ret = MFF_APP_CHECK(index);		break;	// ok
			case MFF_ISC_PARA_CHECK :		  ret = MFF_PARA_CHECK(index);	break;	// ok
			case MFF_ISC_CRC_CHECK :		  ret = MFF_CRC_CHECK(index);		break;	// ok
			case MFF_ISC_PD_CHECK :			  ret = MFF_PD_CHECK(index);		break;	// ok
			case MFF_USB_FirmwareWrite :	  ret = USB_FirmwareWrite(index);	break;	// ok
			default : ret = false;											break;	// ok
	}
	return ret;	
}

#endif //


























