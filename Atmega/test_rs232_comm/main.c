//#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
#include "uart.h"
#include "space_invaders.h"



int main(void) {
	
	uart_init();
	
	sei();
	
	//_delay_ms(500);
	//uart_send(0x0A);
	//display_screen();
	test_1();
	/*
    while (1) {
    }*/
	return 0;
}

ISR(USART_TX_vect, ISR_BLOCK) {
	transmission_complete();
	//reti();
}

ISR(USART_RX_vect, ISR_BLOCK) {
	//keyboard_mode();
	transmission_complete();
	char r = UDR0;
	if(r == "a") {
		uart_send(0x0A);
	}
	//reti();
}


