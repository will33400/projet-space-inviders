
#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f4xx.h"

typedef enum {
	TIMER_2, TIMER_3,
} t_timer;

void init_tim_base(t_timer type, uint32_t delay_val);

#endif
