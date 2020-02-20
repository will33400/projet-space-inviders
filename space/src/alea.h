
#ifndef ALEA_H
#define ALEA_H

uint8_t LFSR;

void LFSR_update() {
	LFSR >>= 1;
	LFSR |= (((LFSR >> 1) & 1) ^ ((LFSR >> 3) & 1) ^ ((LFSR >> 5) & 1)) << 7;
 }


#endif
