//#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
#include "uart.h"
#include "space_invaders.h"
#include "vt100.h"

int pin_scroll;

ISR(USART_TX_vect, ISR_BLOCK) {
	transmission_complete();
	//uart_init();
}

ISR(USART_RX_vect, ISR_BLOCK) {
	//vt100_clear_screen();
	pin_scroll = 1;
	//uart_init();
}

/*
ISR(PCINT1_vect, ISR_BLOCK) {
	
	int test_pin = PINC;
	test_pin &= 0x30;
	if(test_pin == (1<<PINC4)) pin_scroll = (1<<PINC4);
	else pin_scroll = (1<<PINC5);
	
}
*/
int main(void) {
	
	uart_init(9600);
	sei();
	//uart_send('A');
	init_screen();
	//init_spaceship();
	while(1) {
		if(pin_scroll == 1) {
			keyboard_mode();
			pin_scroll = 0;
		}
	}
	
	
}


