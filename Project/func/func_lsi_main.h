#ifndef _FUNC_LSI_MAIN_H__
#define _FUNC_LSI_MAIN_H__

#include "main.h"

// SYSTEM INFO COMMAND
#define READ_PANEL_INFO     0x23
#define READ_DEVICE_ID      0x52
#define READ_BOOT_STATUS    0x55
#define READ_FIRMWARE_VER   0xA3
#define READ_PARAMETER_VER  0xA4
#define READ_IMAGE_VER      0xA5

// SYSTEM CONTROL COMMAND
#define SENSE_ON            0x10
#define SENSE_OFF           0x11
#define SW_RESET            0x12
#define SET_TOUCH_FN        0x30
#define GET_TOUCH_FN        0x30
#define SET_APP_MODE        0xE4
#define SET_FORCE_MODE		0xBE	// only crown

// TOUCH EVENT COMMAND
#define READ_ONE_EVENT      0x60
#define READ_ALL_EVENT      0x61
#define FLUSH_ALL_EVENT     0x62

#define TX_CH 17
#define RX_CH 35
#define FORCE  3

#define TX_MAX 50
#define RX_MAX 50
#define FO_MAX 5

#pragma pack( push , 1)

typedef struct 
{
	uint32_t Signature;
	uint32_t Version;
	uint32_t Totalsize;
	uint32_t Checksum;
	uint32_t ImgFileVer;
	uint32_t ImgFileDate;
	uint32_t ImgFileDesc;
	uint32_t FirmwareVer;
	uint32_t FirmwareDate;
	uint32_t FirmwareDesc;
	uint32_t ParameterVer;
	uint32_t ParameterData;
	uint32_t ParameterDesc;
	uint32_t NumverOfChunk;
	uint32_t Flag;
	uint32_t Reserved;
}ImgHeader;

typedef struct
{
	uint8_t CM_Min;			//  0	
	uint8_t CM_Max;			//  1
	uint8_t CM_RX_Slope;	//  2
	uint8_t CM_TX_Slope;	//  3
	uint8_t RX_PAD_Open;	//  4
	uint8_t TX_PAD_Open;	//  5
	uint8_t RX_GND_Short;	//  6
	uint8_t TX_GND_Short;	//  7
	uint8_t RX_RX_Short;	//  8
	uint8_t TX_TX_Short;	//  9
	uint8_t TX_RX_Short;	// 10
	uint8_t RX_GAP;			// 11
	uint8_t TX_GAP;			// 12
	uint8_t Reserved;		// 13:15
	uint8_t FO_PAD_Open;	// 16
	uint8_t FO_GND_Short;	// 17
	uint8_t FO_FO_Short;	// 18
	uint8_t Reserved1;		// 19:31
}gTSP_RESULT;

typedef struct 
{
	uint32_t Signature;		//  0
	uint32_t Version;		//  4
	uint32_t Totalsize;		//  8
	uint32_t Checksum;		// 12
	uint32_t TestResult;	// 16
	uint32_t TRYcount;		// 20
	uint32_t PASScount;		// 24
	uint32_t FAILcount;		// 28
	uint8_t RX_Num;			// 29
	uint8_t TX_Num;			// 30
	uint8_t FORCE_CH0_Num;	// 31
	uint8_t FORCE_CH1_Num;	// 32
	int16_t Mutual_Cap_Min;	// 34
	int16_t Mutual_Cap_Max;	// 36

	uint32_t TXRX_Slope_l;	// 40
	uint32_t TXRX_Slope_m;	// 40

	uint32_t TXRX_Open_l;		// 48
	uint32_t TXRX_Open_m;		// 48
	
	uint32_t TXRX_ShortToGndGap_l;// 56
	uint32_t TXRX_ShortToGndGap_m;// 56
	
	uint32_t TXRX_ShortToTXRX_l;// 64
	uint32_t TXRX_ShortToTXRX_m;// 64
	
	uint32_t TXRX_ShortToRXTX_l;// 72
	uint32_t TXRX_ShortToRXTX_m;// 72
}gTSP_REPORT;

typedef struct 
{
	uint8_t TX[20];
	uint8_t RX[40];
	uint8_t FO[5]; 
}TXRX_ShortToRXTX;

typedef struct 
{
	uint8_t TX[20];
	uint8_t RX[40];
	uint8_t FO[5]; 
}TXRX_ShortToTXRX;

typedef struct 
{
	uint8_t TX[20];
	uint8_t RX[40];
	uint8_t FO[5]; 
}TXRX_ShortToGndGap;

typedef struct 
{
	uint8_t TX[20];
	uint8_t RX[40];
	uint8_t FO[5]; 
}TXRX_Open;

typedef struct 
{
	uint8_t TX[20];
	uint8_t RX[40];
	uint8_t FO[5]; 
}TXRX_Slope;

typedef struct 
{
	uint32_t ChunkSignature;
	uint32_t ChunkAddr;
	uint32_t ChunkSize;
}ImgChunk;
#pragma pack(pop)

#define TIF_STATUS_CMD_OK			(0)
#define TIF_STATUS_ERROR 			(1u<<7)
#define TIF_STATUS_UNKNOWN_CMD 		(TIF_STATUS_ERROR | (1))
#define TIF_STATUS_CMD_FAIL			(TIF_STATUS_ERROR | (2))
#define TIF_STATUS_BAD_PARAMETER 	(TIF_STATUS_ERROR | (3))
#define TIF_STATUS_CHECKSUM_FAIL 	(TIF_STATUS_ERROR | (4))

/* General Message Format */
typedef struct
{
	unsigned char Sync; // Sync code
	unsigned char TxSizeMSB; // Txdata size in bytes (High byte)
	unsigned char TxSizeLSB; // ( Low byte)
	unsigned char RxSizeMSB; // Rx data size in bytes (High byte)
	unsigned char RxSizeLSB; // ( Low byte)
	unsigned char Command; // command ID
	unsigned char Data[1]; // optional data for command
} TIF_MESSAGE;

/* Device ACK Message Format */
typedef struct
{
	unsigned char Sync; // Sync code
	unsigned char TxSizeMSB; // Txdata size in bytes (High byte)
	unsigned char TxSizeLSB; // ( Low byte)
	unsigned char RxSizeMSB; // Rx data size in bytes (High byte)
	unsigned char RxSizeLSB; // ( Low byte)
	unsigned char Command; // received command
	unsigned char CmdResult; // result of the received command
	unsigned char Data[1]; // optional data
} TIF_MESSAGE_CMDACK;


extern uint32_t TX_NUM, RX_NUM, FO_NUM;

extern int16_t CM_OFFSET[TX_MAX * RX_MAX];
extern int16_t CS_OFFSET_TX[TX_MAX];
extern int16_t CS_OFFSET_RX[RX_MAX];
extern int16_t CF_OFFSET[FO_MAX];
 
extern int16_t TX_SHORT_GND[TX_MAX];
extern int16_t RX_SHORT_GND[RX_MAX];
extern int16_t TX_SHORT_TX[TX_MAX];
extern int16_t RX_SHORT_RX[RX_MAX];
extern int16_t TX_SHORT_RX[TX_MAX];
extern int16_t RX_SHORT_TX[RX_MAX];
 
extern int16_t TX_SHORT_GND_GAP[TX_MAX];
extern int16_t RX_SHORT_GND_GAP[RX_MAX];
extern int16_t TX_SHORT_TX_GAP[TX_MAX];
extern int16_t RX_SHORT_RX_GAP[RX_MAX];
extern int16_t TX_SHORT_RX_GAP[TX_MAX];
extern int16_t RX_SHORT_TX_GAP[RX_MAX];
 
extern int16_t JIT_CMR_AVR[TX_MAX*RX_MAX];	//cmr avr
extern int16_t JIT_CMR_MM[TX_MAX*RX_MAX];	//cmr mm
extern int16_t JIT_CMR_GAP[TX_MAX*RX_MAX];	//cmr gap
extern int16_t JIT_CSR_TX_AVR[TX_MAX];	//csr tx
extern int16_t JIT_CSR_RX_AVR[RX_MAX];	//csr rx
extern int16_t JIT_CSR_TX_GAP[TX_MAX];	//csr tx gap
extern int16_t JIT_CSR_RX_GAP[RX_MAX];	//csr rx gap
extern char TX_SHORT_GND_BUFF[64];
extern char RX_SHORT_GND_BUFF[64];
extern char TX_SHORT_TX_BUFF[64];
extern char RX_SHORT_RX_BUFF[64];
extern char TX_SHORT_RX_BUFF[64];
extern char RX_SHORT_TX_BUFF[64];
extern char IC_Type[16];         

extern uint8_t rx_temp[5000];
extern uint8_t tx_temp[5000];


extern gTSP_REPORT	Tsp_Report;
extern gTSP_RESULT	Tsp_Result;
uint8_t LsiModule(uint16_t index);


uint8_t LSI_Write(uint8_t *pData, uint32_t rSize);
uint8_t LSI_Write_Command(uint8_t command);
uint8_t LSI_Write_Data(uint8_t *pData, uint32_t rSize, uint8_t Addr);
uint8_t LSI_Read_Data(uint8_t *rData, uint32_t rSize, uint8_t Addr);

#endif //_MAIN__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

