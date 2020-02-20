
#ifndef SPACE_INVADERS_H_
#define SPACE_INVADERS_H_

#include <string.h>
#include <stdint.h>

#define SHOOT_AMOUNT 10

#define SERIAL_BAUD 115200

/* spaceship default parameters */
#define SPACESHIP_LIFE_MAX 3
#define SPACESHIP_SHOOT '|'

/* screen settings */
#define SCREEN_DIMENSION_X 80
#define SCREEN_DIMENSION_Y 24

/* ennemies default parameters */
#define ENNEMY_LIFE_MIN 1
#define ENNEMY_LIFE_MAX 10

#define ENNEMY_SHOOT_TYPE_1 '#'
#define ENNEMY_SHOOT_TYPE_2 '*'
#define ENNEMY_SHOOT_TYPE_3 '-'

typedef enum {
	FALSE, TRUE
} _bool;

typedef enum {
	SPACESHIP, ENNEMY_TYPE_1, ENNEMY_TYPE_2, ENNEMY_TYPE_3
} object;

typedef enum {
	LEFT, RIGHT
} side;

typedef enum {
	MENU, LEVEL_1, LEVEL_2, LEVEL_3
} level_num;

typedef struct {
	object type;
	uint8_t life;
	uint8_t shoot_type;
	char appearance[3];
	uint8_t x;
	uint8_t y;
	_bool exist;
} ennemy;

typedef struct {
	uint8_t appearance;
	object _object;
	uint8_t x;
	uint8_t y;
	uint8_t exist;
} shoot;

struct spaceship {
	uint8_t life;
	uint8_t appearance[3];
	uint8_t x;
	uint8_t y;
} _spaceship;

uint8_t keyboard_flags;

uint8_t shoot_index;

#define KFPAUS 0x01

level_num lev_num_flag;

/* init functions */
void update_screen_matrix();

void init_sprites();
void init_screen();

void compute_keyboard();

void init_spaceship();
void erase_spaceship();
void scroll_spaceship(side _side);

void init_shoot(object _object, uint8_t x, uint8_t y);
void erase_shoot(shoot *_shoot);
void scroll_shoot();

void init_ennemy(ennemy *_ennemy, uint8_t x, uint8_t y);
void erase_ennemy(ennemy *_ennemy);
void scroll_ennemies(ennemy *_ennemy, uint8_t x, uint8_t y);

void init_shield(uint8_t x, uint8_t y);

void charge_level(level_num level);

void toggle_pause();

void init_menu();
void init_level_1();
void init_level_2();
void init_level_3();

#endif /* SPACE_INVADERS_H_ */
