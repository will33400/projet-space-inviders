/*
 * alea.c
 *
 *  Created on: 22 févr. 2020
 *      Author: lucasadam33400
 */

#include "alea.h"

void LFSR_set(uint8_t val) {
	LFSR = val;
}

uint8_t LFSR_get(void) {
	return LFSR;
}

void LFSR_update(void) {
	LFSR >>= 1;
	LFSR |= (((LFSR >> 1) & 1) ^ ((LFSR >> 3) & 1) ^ ((LFSR >> 5) & 1)) << 7;
}
