//#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
#include "uart.h"
#include "space_invaders.h"
#include "vt100.h"


int main(void) {
	
	uart_init();
	sei();
	//vt100_clear_screen();
	
	while (1)
	{
	}
	
	return 0;
}

ISR(USART_TX_vect, ISR_BLOCK) {
	transmission_complete();
}

ISR(USART_RX_vect, ISR_BLOCK) {
	keyboard_mode();
}


