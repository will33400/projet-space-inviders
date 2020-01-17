#include "vt100.h"
#include <stdint.h>
#include "uart.h"

void vt100_move(uint8_t x, uint8_t y)
{
	uart_send(VT100_ESC);
	uart_send('[');
	uart_send('0' + (y / 100));
	uart_send('0' + ((y / 10) % 10));
	uart_send('0' + (y % 10));
	uart_send(';');
	uart_send('0' + (x / 100));
	uart_send('0' + ((x / 10) % 10));
	uart_send('0' + (x % 10));
	uart_send('H');
}

void vt100_clear_screen(void)
{
	uart_send(VT100_ESC);
	uart_send('[');
	uart_send('2');
	uart_send('J');
	uart_send(VT100_ESC);
	uart_send('[');
	uart_send('?');
	uart_send('2');
	uart_send('5');
	uart_send('l');
}

