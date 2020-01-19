//#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
#include "uart.h"
#include "space_invaders.h"
#include "vt100.h"


ISR(USART_TX_vect, ISR_BLOCK) {
	transmission_complete();
	//uart_init();
}

ISR(USART_RX_vect, ISR_BLOCK) {
	vt100_clear_screen();
	keyboard_mode();
	//uart_init();
}

int main(void) {
	
	uart_init();
	sei();
	
	vt100_move(0,0);
	
	vt100_clear_screen();
	uart_send('a');
	vt100_move(5,5);
	uart_send('e');
	
	while(1) {
		
	}
	
	
}


