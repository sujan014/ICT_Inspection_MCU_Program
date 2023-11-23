#include "main.h"

uint8_t eep_init(uint16_t index);

//uint8_t eep_read(void);
//uint8_t eep_write(void);

uint8_t eep_write_flow(uint16_t index);
uint8_t eep_read_flow(uint16_t index);
uint8_t EEP_Function(uint16_t index);

void EEP_Power_On(void);
void EEP_Power_Off(void);

