#include "main.h"

#define CRC_INIT_VALUE 0xFFFFFFFF

uint32_t Set_CRC32InitValue(uint32_t start);
void InitCRC32(void);
uint32_t Get_CRC32(unsigned char newData, uint32_t oldData);
uint32_t Get_CRC32Reference(void);
void Set_CRC32Reference(uint32_t target);
uint32_t Get_CRC32Target(void);
void Set_CRC32Target(uint32_t source);
