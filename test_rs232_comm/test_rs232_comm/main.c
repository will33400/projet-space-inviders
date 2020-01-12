#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"

ISR(USART_TX_vect) {
	transmission_complete();
}

ISR(USART_RX_vect) {
	keyboard_mode();
}

int main(void) {
	
	uart_init();
	
	sei();
	
	_delay_ms(500);
	
    while (1) {
    }
}
