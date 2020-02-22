
#ifndef SPACE_INVADERS_H_
#define SPACE_INVADERS_H_

#include <string.h>
#include <stdint.h>

/* protocol settings */
#define SERIAL_BAUD 115200

/* shoot settings */
#define SHOOT_AMOUNT 10

/* spaceship default parameters */
#define SPACESHIP_LIFE_MAX 3
#define SPACESHIP_SHOOT '|'
#define SPACESHIP_WIDTH 3

/* ennemies default parameters */
#define ENNEMY_LIFE_MIN 1
#define ENNEMY_LIFE_MAX 10

#define ENNEMY_SHOOT_TYPE_1 '#'
#define ENNEMY_SHOOT_TYPE_2 '*'
#define ENNEMY_SHOOT_TYPE_3 '-'

#define ENNEMY_WIDTH 3
#define ENNEMY_TYPES 3

#define ENNEMY_AMOUNT 32

/* shields default parameters */

#define SHIELD_WIDTH 7
#define SHIELD_HEIGHT 4
#define SHIELD_AMOUNT 3

/* levels settings */

#define LEVEL_AMOUNT 3

typedef enum {
	FALSE, TRUE
} _bool;

typedef enum {
	SPACESHIP, ENNEMY_TYPE_1, ENNEMY_TYPE_2, ENNEMY_TYPE_3, SHIELD
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
	uint8_t appearance[ENNEMY_WIDTH];
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
	uint8_t appearance[SPACESHIP_WIDTH];
	uint8_t x;
	uint8_t y;
} _spaceship;

typedef struct {
	uint8_t appearance;
	object _object;
	uint8_t x;
	uint8_t y;
	_bool exist;
} shield_hitbox;

uint8_t keyboard_flags;

uint8_t shoot_index;

uint8_t shield_index;

#define KFPAUS 0x01

level_num lev_num_flag;

/* init functions */
void update_screen_matrix(void);

void init_sprites(void);
void init_screen(void);

void compute_keyboard(void);

void init_spaceship(void);
void erase_spaceship(void);
void scroll_spaceship(side _side);

void init_shoot(object _object, uint8_t x, uint8_t y);
void erase_shoot(shoot *_shoot);
void scroll_shoot(void);

void init_ennemy(ennemy *_ennemy, uint8_t x, uint8_t y);
void erase_ennemy(ennemy *_ennemy);
void scroll_ennemies(void);

void init_shield(uint8_t x, uint8_t y);

void charge_level(level_num level);

void toggle_pause(void);

void init_menu(void);
void init_level_1(void);
void init_level_2(void);
void init_level_3(void);

#endif /* SPACE_INVADERS_H_ */
