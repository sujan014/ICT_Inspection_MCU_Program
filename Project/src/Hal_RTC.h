#ifndef __HAL_RTC__H__
#define __HAL_RTC__H__

#include "main.h"

#define RTC_CLOCK_SOURCE_LSE   	/* LSE used as RTC source clock */
#define RTC_PARM_ADD  			0x08070000

#define BOOT_FLAG			0x424F4F54	//BOOT
#define APP_FLAG			0x4150504C	//APPL
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// [RTC_BKP_DR1]	bootload flag
#define WR_BOOT(x)			RTC_WriteBackupRegister(RTC_BKP_DR1, x)
#define RD_BOOT				RTC_ReadBackupRegister(RTC_BKP_DR1)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// [RTC_BKP_DR2]	QTY
#define WR_QTY(x) 			RTC_WriteBackupRegister(RTC_BKP_DR2, x)
#define RD_QTY				RTC_ReadBackupRegister(RTC_BKP_DR2)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// [RTC_BKP_DR3]	OK		
#define WR_OK(x)  			RTC_WriteBackupRegister(RTC_BKP_DR3, x)
#define RD_OK  				RTC_ReadBackupRegister(RTC_BKP_DR3)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// [RTC_BKP_DR4]	NG	
#define WR_NG(x)  			RTC_WriteBackupRegister(RTC_BKP_DR4, x)
#define RD_NG  				RTC_ReadBackupRegister(RTC_BKP_DR4)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// [RTC_BKP_DR5]	Mode Flag
#define CR_MODE				(RTC_WriteBackupRegister(RTC_BKP_DR5, RTC_ReadBackupRegister(RTC_BKP_DR5) & (0xFFFFFFF0) ) )
#define RD_MODE				((RTC_ReadBackupRegister(RTC_BKP_DR5) >> 0) & 0xF) 
#define WR_MODE(x)			CR_MODE;		(RTC_WriteBackupRegister(RTC_BKP_DR5, RTC_ReadBackupRegister(RTC_BKP_DR5) | ( x << 0 & 0x0000000F )))
// [RTC_BKP_DR5] 	RTC County Flag	
#define CR_TBD				(RTC_WriteBackupRegister(RTC_BKP_DR5, RTC_ReadBackupRegister(RTC_BKP_DR5) & (0xFFFFFF0F) ) )
#define RD_TBD				((RTC_ReadBackupRegister(RTC_BKP_DR5) >> 4) & 0xF) 
#define WR_TBD(x)			CR_TBD; 		(RTC_WriteBackupRegister(RTC_BKP_DR5, RTC_ReadBackupRegister(RTC_BKP_DR5) | ( x << 4 & 0x000000F0 )))
// [RTC_BKP_DR5] 	App/dbg
#define CR_DBG_MD			(RTC_WriteBackupRegister(RTC_BKP_DR5, RTC_ReadBackupRegister(RTC_BKP_DR5) & (0xFFFFF0FF) ) )
#define RD_DBG_MD			((RTC_ReadBackupRegister(RTC_BKP_DR5) >> 8) & 0xF) 
#define WR_DBG_MD(x)		CR_DBG_MD;		(RTC_WriteBackupRegister(RTC_BKP_DR5, RTC_ReadBackupRegister(RTC_BKP_DR5) | ( x << 8 & 0x00000F00 )))
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NG_CONTINUE
#define CR_NG_CONTINUE		(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) & (0xFFFFFFF0)))
#define RD_NG_CONTINUE		((RTC_ReadBackupRegister(RTC_BKP_DR6) >> 0) & 0xF) 
#define WR_NG_CONTINUE(x)	CR_NG_CONTINUE; 	(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) | ( x << 0  & 0x0000000F )))
// FW_WRITE
#define CR_FW_WRITE			(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) & (0xFFFFFF0F)))
#define RD_FW_WRITE			true//((RTC_ReadBackupRegister(RTC_BKP_DR6) >> 4) & 0xF) 
#define WR_FW_WRITE(x)		CR_FW_WRITE;		(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) | ( x << 4  & 0x000000F0 )))
// SDC LOG
#define CR_SDC_SAVE			(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) & (0xFFFFF0FF)))
#define RD_SDC_SAVE			((RTC_ReadBackupRegister(RTC_BKP_DR6) >> 8) & 0xF) 
#define WR_SDC_SAVE(x)		CR_SDC_SAVE;		(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) | ( x << 8  & 0x00000F00 )))
// LOG Unit
#define CR_LOG_UNIT			(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) & (0xFFFF0FFF)))
#define RD_LOG_UNIT			((RTC_ReadBackupRegister(RTC_BKP_DR6) >>12) & 0xF) 
#define WR_LOG_UNIT(x)		CR_LOG_UNIT;		(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) | ( x << 12 & 0x0000F000 )))
// SUB Project (Serial <-> Data File Name)
#define CR_SUB_DISP			(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) & (0xFFF0FFFF)))
#define RD_SUB_DISP			((RTC_ReadBackupRegister(RTC_BKP_DR6) >>16) & 0xF) 
#define WR_SUB_DISP(x)		CR_SUB_DISP;		(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) | ( x << 16 & 0x000F0000 )))
// Short All
#define CR_SHORT_ALL		(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) & (0xFF0FFFFF)))
#define RD_SHORT_ALL		((RTC_ReadBackupRegister(RTC_BKP_DR6) >>20) & 0xF) 
#define WR_SHORT_ALL(x)		CR_SHORT_ALL;		(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) | ( x << 20 & 0x00F00000 )))

// Full Write
#define CR_FULL_WRITE		(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) & (0xF0FFFFFF)))
#define RD_FULL_WRITE		((RTC_ReadBackupRegister(RTC_BKP_DR6) >>24) & 0xF) 
#define WR_FULL_WRITE(x)		CR_FULL_WRITE;		(RTC_WriteBackupRegister(RTC_BKP_DR6, RTC_ReadBackupRegister(RTC_BKP_DR6) | ( x << 24 & 0x0F000000 )))

// LED_CH1
#define CR_LED_CH1			(RTC_WriteBackupRegister(RTC_BKP_DR7, RTC_ReadBackupRegister(RTC_BKP_DR7) & (0xFFFFFF00)))
#define RD_LED_CH1			((RTC_ReadBackupRegister(RTC_BKP_DR7) >> 0) & 0xFF) 
#define WR_LED_CH1(x)		CR_LED_CH1; 	(RTC_WriteBackupRegister(RTC_BKP_DR7, RTC_ReadBackupRegister(RTC_BKP_DR7) | ( x << 0  & 0x000000FF )))
// LED_CH2
#define CR_LED_CH2			(RTC_WriteBackupRegister(RTC_BKP_DR7, RTC_ReadBackupRegister(RTC_BKP_DR7) & (0xFFFF00FF)))
#define RD_LED_CH2			((RTC_ReadBackupRegister(RTC_BKP_DR7) >> 8) & 0xFF) 
#define WR_LED_CH2(x)		CR_LED_CH2; 	(RTC_WriteBackupRegister(RTC_BKP_DR7, RTC_ReadBackupRegister(RTC_BKP_DR7) | ( x << 8  & 0x0000FF00 )))
// LED_CH3
#define CR_LED_CH3			(RTC_WriteBackupRegister(RTC_BKP_DR7, RTC_ReadBackupRegister(RTC_BKP_DR7) & (0xFF00FFFF)))
#define RD_LED_CH3			((RTC_ReadBackupRegister(RTC_BKP_DR7) >> 16) & 0xFF) 
#define WR_LED_CH3(x)		CR_LED_CH3; 	(RTC_WriteBackupRegister(RTC_BKP_DR7, RTC_ReadBackupRegister(RTC_BKP_DR7) | ( x << 16  & 0x00FF0000 )))
// LED_CH4
#define CR_LED_CH4			(RTC_WriteBackupRegister(RTC_BKP_DR7, RTC_ReadBackupRegister(RTC_BKP_DR7) & (0x00FFFFFF)))
#define RD_LED_CH4			((RTC_ReadBackupRegister(RTC_BKP_DR7) >> 24) & 0xFF) 
#define WR_LED_CH4(x)		CR_LED_CH4; 	(RTC_WriteBackupRegister(RTC_BKP_DR7, RTC_ReadBackupRegister(RTC_BKP_DR7) | ( x << 24  & 0xFF000000 )))


void RTC_Config(void);
void RTC_Initial(void);

//void bk_sram_write(uint8_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize);
//void bk_sram_read(uint8_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize);

void bk_sram_write(uint32_t WriteAddress, uint8_t data);
uint8_t bk_sram_read(uint32_t ReadAddress);
#endif //__HAL_RTC__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

