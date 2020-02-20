/*
 * delay.c
 *
 *  Created on: 20 févr. 2020
 *      Author: lucasadam33400
 */

#include <stdio.h>

void delay_(uint32_t val) {
	for (uint32_t i = val * 10000; i > 0; i--)
		;
}
