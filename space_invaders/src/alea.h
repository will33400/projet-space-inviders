
#ifndef ALEA_H_
#define ALEA_H_

#include "stdint.h"

uint8_t LFSR;

void LFSR_set(uint8_t val);
uint8_t LFSR_get(void);
void LFSR_update(void);

#endif
