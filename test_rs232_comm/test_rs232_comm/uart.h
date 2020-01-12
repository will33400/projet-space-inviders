/*
 * uart.h
 *
 * Created: 09/01/2020 14:02:53
 *  Author: lucasadam33400
 */ 


#ifndef UART_H_
#define UART_H_

#define UCSR0A_INIT (1<<UDRE0)

#define UCSR0B_TXIEN (1<<TXCIE0)
#define UCSR0B_RXIEN (1<<RXCIE0)
#define UCSR0B_RXEN (1<<RXEN0)
#define UCSR0B_TXEN (1<<TXEN0)
#define UCSR0B_INIT (UCSR0B_RXIEN | UCSR0B_TXIEN | UCSR0B_TXEN | UCSR0B_RXEN)
#define UCSR0B_SEND 0b01011000

#define UCSR0C_SB (1<<USBS0)
#define UCSR0C_MODE_1 ((1<<UCSZ01) | (1<<UCSZ00))
#define UCSR0C_INIT (UCSR0C_SB | UCSR0C_MODE_1)

#define BAUD_9600_8MHZ 51 
#define BAUD_115200_8MHZ 3

char uart_data[256];
char uart_data_long;

void uart_init(void) {
	//baud_rate 9600
	//ubrr_set = F_CPU/16/baud_rate-1
	
	UCSR0A = UCSR0A_INIT;
	/*
	Bit 7 RXC0: USART Receive Complete
	Bit 6 TXC0: USART Transmit Complete
	Bit 5 UDRE0: USART Data Register Empty
	Bit 4 FE0: Frame Error
	Bit 3 DOR0: Data OverRun
	Bit 2 UPE0: USART Parity Error
	Bit 1 U2X0: Double the USART Transmission Speed
	Bit 0 MPCM0: Multi-processor Communication Mode
	*/
	UCSR0B = UCSR0B_INIT;
	/*
	Bit 7 RXCIE0: RX Complete Interrupt Enable 0
	Bit 6 TXCIE0: TX Complete Interrupt Enable 0
	Bit 5 UDRIE0: USART Data Register Empty Interrupt Enable 0
	Bit 4 RXEN0: Receiver Enable 0
	Bit 3 TXEN0: Transmitter Enable 0
	Bit 2 UCSZ02: Character Size 0
	Bit 1 RXB80: Receive Data Bit 8 0
	Bit 0 TXB80: Transmit Data Bit 8 0
	*/
	UCSR0C = UCSR0C_INIT;
	/*
	Bits 7:6 UMSEL0n: USART Mode Select 0 n [n = 1:0]
	Bits 5:4 UPM0n: USART Parity Mode 0 n [n = 1:0]
	Bit 3 USBS0: USART Stop Bit Select 0
	Bit 2 UCSZ01 / UDORD0: USART Character Size / Data Order
	Bit 1 UCSZ00 / UCPHA0: USART Character Size / Clock Phase
	Bit 0 UCPOL0: Clock Polarity 0
	*/
	//UBRR0L = 0b00000001;
	UBRR0L = BAUD_9600_8MHZ;
	//UBRR0L = 3;
	/*
	Bits 7:0 UBRR0[7:0]: USART Baud Rate 0
	*/
	//UBRR0H = 0b00000000;
	UBRR0H = 0;
	/*
	Bits 3:0 UBRR0[3:0]: USART Baud Rate 0 n [n = 11:8]
	*/
	//UDR0 = 0b00000000;
	/*
	Bits 7:0 TXB / RXB[7:0]: USART Transmit / Receive Data Buffer
	*/
}

void uart_send(char data_buffer) {
	UCSR0B = UCSR0B_SEND;
	while (UCSR0B&(1>>TXCIE0));
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data_buffer;
}
/*
void uart_long_ascii_array_conversion(long data_buffer) {
	long dix_fmul;
	
	for(int index = 31; data_buffer >= 0; index--) {
		dix_fmul = data_buffer/pow_function(10,(long)index);
		dix_fmul %= pow_function(10, (long)31-index);
		dix_fmul += 48;
		uart_data[index] = (char)dix_fmul;
	}
}
*/
long pow_function(long value, long power) {
	long value_temp = value;
	for (long index = power; index >= 0; index--) value *= value_temp;
	return value;
}

//interrupts routines
void transmission_complete() {
	UCSR0B = ~(1<<TXCIE0);
	while (UCSR0B&(1<<TXCIE0));
}

void keyboard_mode() {
	UCSR0B = ~(1<<RXCIE0);
	while (UCSR0B&(1<<RXCIE0));
	char r = UDR0;
	uart_send(r);
}

#endif /* UART_H_ */