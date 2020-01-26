#include "space_invaders.h"
//#include "serial.h"
#include "vt100.h"
#include "uart.h"
#include <avr/io.h>

uint8_t x = 40;
uint8_t y = 24;

const uint8_t spaceship_appearance[3] = { '<', '|', '>' };

const uint8_t ennemy_appearances[3][3] = { { '(', '#', ')' }, { '{', '*', '}' },
{ '[', '-', ']' } };

const uint8_t shield_appearance[4][5] = { { ' ', '#', '#', '#', ' ' }, { '#',
'#', '#', '#', '#' }, { ' ', '#', '#', '#', '#' }, { '#', '#', '#', '#',
'#' } };

uint8_t screen_matrix[24][80] = { { 32 }, { 32 }, { 32 }, { 32 }, { 32 },
{ 32 }, { 32 }, { 32 }, { 32 }, { 32 }, { 32 }, { 32 }, { 32 }, { 32 },
{ 32 }, { 32 }, { 32 }, { 32 }, { 32 }, { 32 }, { 32 }, { 32 }, { 32 },
{ 32 } };

void init_pins() {
	
	PORTC |= (1<<PORTC4) | (1<<PORTC5);
	PCIFR |= 1;
	PCMSK1 |= (1<<PINC4) | (1<<PINC5);
	
}

void init_screen() {
	//uart_init(9600);

	vt100_move(0, 0);
	vt100_clear_screen();
	
	init_pins();

	_spaceship.life = SPACESHIP_LIFE_MAX;
	for (int i = 0; i < 3; i++)
	//_spaceship.spaceship_appearance[i] = spaceship_appearance[i];
	_spaceship.spaceship_shoot = SPACESHIP_SHOOT;
	_spaceship.coordinates_x = 40;
	_spaceship.coordinates_y = 24;
	
	init_spaceship();
}

void init_spaceship() {
	for (int i = 0; i < 3; i++) {
		vt100_move(x + i, y);
		uart_send(_spaceship.spaceship_appearance[i]);
	}
}

void scroll_spaceship() {
	uint8_t _char = PINC;
	switch (_char) {
		case (1<<PINC5):
		if (x > 1)
		x--;
		init_screen();
		init_spaceship();
		break;

		case (1<<PINC4):
		if (x < 78)
		x++;
		init_screen();
		init_spaceship();
		break;
	}
}

void init_ennemy(uint8_t ennemy_type) {

}

void scroll_ennemies(uint8_t ennemy_type, uint8_t x, uint8_t y) {

}

void init_shield(uint8_t x, uint8_t y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			vt100_move(x + i, y + j);
			uart_send(shield_appearance[i][j]);
		}
	}
}

void init_level_1() {
	//init ennemies sprites
	//init obstacles sprites
	init_shield(10, 10);
	init_spaceship();
}

void init_level_2() {

}

void init_level_3() {

}
