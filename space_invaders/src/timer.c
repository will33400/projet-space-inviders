/*
 * timer.c
 *
 *  Created on: 27 janv. 2020
 *      Author: lucasadam33400
 */

#include "timer.h"

void init_tim_base(t_timer type, uint32_t delay_val) {

	TIM_TimeBaseInitTypeDef str_TIM;
	NVIC_InitTypeDef str_NVIC;

	str_TIM.TIM_Prescaler = TIM_TS_ITR0;
	str_TIM.TIM_Period = delay_val;
	str_TIM.TIM_ClockDivision = TIM_CKD_DIV1;
	str_TIM.TIM_CounterMode = TIM_CounterMode_Down;
	str_TIM.TIM_RepetitionCounter = TIM_OPMode_Repetitive;

	str_NVIC.NVIC_IRQChannelCmd = ENABLE;
	str_NVIC.NVIC_IRQChannelPreemptionPriority = 0;
	str_NVIC.NVIC_IRQChannelSubPriority = 0;

	switch (type) {

	case TIMER_2:

		TIM_TimeBaseInit(TIM2, &str_TIM);

		str_NVIC.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_Init(&str_NVIC);
		break;

	case TIMER_3:

		TIM_TimeBaseInit(TIM3, &str_TIM);

		str_NVIC.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_Init(&str_NVIC);

		break;
	}
}
/*
#define tim2_irq TIM2_IRQHandler

 void tim2_irq(void) {

}

#define tim3_irq TIM3_IRQHandler

 void tim3_irq(void) {

}
 */
