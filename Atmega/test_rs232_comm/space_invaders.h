
#ifndef SPACE_INVADERS_H_
#define SPACE_INVADERS_H_

#include <string.h>
#include <stdint.h>

/* spaceship default parameters */
#define SPACESHIP_LIFE_MAX 3
#define SPACESHIP_SHOOT '|'

struct spaceship {
	uint8_t life;
	uint8_t spaceship_appearance[3];
	uint8_t spaceship_shoot;
	uint8_t coordinates_x;
	uint8_t coordinates_y;
} _spaceship;

/* ennemies default parameters */
#define ENNEMY_LIFE_MIN 1
#define ENNEMY_LIFE_MAX 10

enum ennemy_types {
	ENNEMY_TYPE_1 = 0x0,
	ENNEMY_TYPE_2 = 0x1,
	ENNEMY_TYPE_3 = 0x2,
};

#define ENNEMY_SHOOT_TYPE_1 '#'
#define ENNEMY_SHOOT_TYPE_2 '*'
#define ENNEMY_SHOOT_TYPE_3 '-'

struct ennemy {
	uint8_t life;
	uint8_t shooting_rate;
	uint8_t shoot_type;
	char appearance[3];
	uint8_t coordinates_x;
	uint8_t coordinates_y;
} ennemy_1, ennemy_2, ennemy_3;

/* screen settings */
#define SCREEN_DIMENSION_X 80
#define SCREEN_DIMENSION_Y 24

/* init functions */
void init_pins();

void update_screen_matrix();

void init_screen();

void init_spaceship();

void scroll_spaceship();

void init_ennemy(uint8_t ennemy_type);

void scroll_ennemies(uint8_t ennemy_type, uint8_t x, uint8_t y);

void init_shield(uint8_t x, uint8_t y);

void init_level_1();
void init_level_2();
void init_level_3();

#endif /* SPACE_INVADERS_H_ */
