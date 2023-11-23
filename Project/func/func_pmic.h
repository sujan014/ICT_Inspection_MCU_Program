#include "main.h"

uint8_t LGD_pmic_init(uint16_t index); 
//uint8_t LGD_pmic_read(uint8_t _type);
uint8_t LGD_pmic_read(void);
//uint8_t LGD_pmic_write(uint8_t _type);
uint8_t LGD_pmic_write(void);
//uint8_t LGD_pmic_write_flow(uint16_t index, uint8_t _type);
uint8_t LGD_pmic_write_flow(uint16_t index);
//uint8_t LGD_pmic_read_flow(uint16_t index, uint8_t _type);
uint8_t LGD_pmic_read_flow(uint16_t index);
uint8_t PMIC_Function(uint16_t index);

void PMIC_Power_On(void);
void PMIC_Power_Off(void);

#define type_DAC	0
#define type_EPRM	1







