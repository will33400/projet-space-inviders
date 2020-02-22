/*
 * alea.c
 *
 *  Created on: 22 févr. 2020
 *      Author: lucasadam33400
 */

#include "alea.h"

void LFSR_update(void) {
	LFSR >>= 1;
	LFSR |= (((LFSR >> 1) & 1) ^ ((LFSR >> 3) & 1) ^ ((LFSR >> 5) & 1)) << 7;
}
