#ifndef SPACE_INVADERS_H_
#define SPACE_INVADERS_H_

#include <string.h>
#include <stdint.h>

/* protocol settings */
#define SERIAL_BAUD 115200

/* shoot settings */
#define SHOOT_AMOUNT 32

/* spaceship default parameters */
#define SPACESHIP_LIFE_MAX 5
#define SPACESHIP_SHOOT '|'

/* ennemies default parameters */
#define ENNEMY_LIFE_MIN 1
#define ENNEMY_LIFE_MAX 10

#define ENNEMY_SHOOT_TYPE_1 '#'
#define ENNEMY_SHOOT_TYPE_2 '*'
#define ENNEMY_SHOOT_TYPE_3 '-'

#define ENNEMY_TYPES 3

#define ENNEMY_AMOUNT 64

/* flying objects parameters */

#define OBJECT_WIDTH_S 3
#define OBJECT_WIDTH_M 5
#define OBJECT_WIDTH_L 7

#define OBJECT_HEIGHT_S 1
#define OBJECT_HEIGHT_M 2
#define OBJECT_HEIGHT_L 4

/* shields default parameters */

#define SHIELD_WIDTH 7
#define SHIELD_HEIGHT 4
#define SHIELD_AMOUNT 3

/* levels settings */

#define LEVEL_AMOUNT 3
#define SCORE_POS_X VT100_SCREEN_XMAX
#define SCORE_POS_Y VT100_SCREEN_YMAX
#define LIFE_POS_X VT100_SCREEN_XMIN
#define LIFE_POS_Y VT100_SCREEN_YMAX

#define SHOOT_COLLISION_SCORE 10
#define ENNEMY_COLLISION_SCORE 200

typedef enum {
	FALSE, TRUE
} _bool;

typedef enum {
	SPACESHIP, ENNEMY_TYPE_1, ENNEMY_TYPE_2, ENNEMY_TYPE_3, SHIELD
} object;

typedef enum {
	SMALL, MEDIUM, LARGE
} object_size;

typedef enum {
	LEFT, RIGHT, UP, DOWN
} side;

typedef enum {
	MENU, LEVEL_1, LEVEL_2, LEVEL_3, GAME_OVER
} level_num;

typedef struct {
	uint8_t appearance;
	object _object;
	uint8_t x;
	uint8_t y;
	_bool exist;
} object_hitbox;

typedef union {
	object_hitbox hitbox_s[OBJECT_HEIGHT_S][OBJECT_WIDTH_S];
	object_hitbox hitbox_m[OBJECT_HEIGHT_M][OBJECT_WIDTH_M];
	object_hitbox hitbox_l[OBJECT_HEIGHT_L][OBJECT_WIDTH_L];
} hitbox_array;

typedef struct {
	uint8_t life;
	_bool exist;
	object_size size;
	hitbox_array _hitbox_array;
} flying_object_s;

typedef struct {
	side ennemy_move_side;
	uint8_t alea_ennemy_array_i;
	uint8_t ennemy_num;
	flying_object_s ennemy_array[ENNEMY_AMOUNT];
	flying_object_s spaceship;
	object_hitbox shoot_list[SHOOT_AMOUNT];
	uint8_t shoot_index;
	uint32_t score;
	uint32_t score_temp;
	uint8_t life_temp;
	object_size ennemy_size;
	uint32_t refresh_frequ;
} level_s;

/* main purpose commands flying object / hitbox */

void destroy_hitbox(object_hitbox *object);

void init_flying_object(flying_object_s *object_struct, object_size size,
		object _object, char sprites[OBJECT_HEIGHT_L][OBJECT_WIDTH_L],
		uint8_t life);

void destroy_flying_object(flying_object_s *object_struct);

void update_coordinates(flying_object_s *object_struct, uint8_t x, uint8_t y);

void display_flying_object(flying_object_s *object_struct);

void erase_flying_object(flying_object_s *object_struct);

void scroll_flying_object(flying_object_s *object_struct, side _side,
		int8_t offset);

_bool flying_object_move_limit(flying_object_s *object_struct, side _side,
		int8_t offset);

_bool object_hitbox_collision(object_hitbox *object_1, object_hitbox *object_2);

void object_list_append(flying_object_s *object_struct,
		flying_object_s *flying_object_array, uint8_t index);

void object_list_destroy(flying_object_s *flying_object_array,
		uint8_t array_size);

void object_list_display(flying_object_s *flying_object_array,
		uint8_t array_size);

void object_list_erase(flying_object_s *flying_object_array, uint8_t array_size);

void object_list_scroll(flying_object_s *flying_object_array,
		uint8_t array_size, side _side, int8_t offset);

/* special commands (other)*/

void init_objects(void);
void init_levels(void);
void init_screen(void);

void compute_keyboard(void);

void init_shoot(object _object, level_s *level, uint8_t x, uint8_t y);
void erase_shoot(object_hitbox *_shoot);
void shoot_events(level_s *level);

void init_shield(uint8_t x, uint8_t y);

void charge_level(level_num level);
void display_level_number(level_num level);
void display_score(uint32_t score);
void display_life(uint8_t life);

void toggle_pause(void);

void init_menu(void);
void init_level_1(void);
void init_level_2(void);
void init_level_3(void);
void init_game_over(void);

void levels_events(level_s *level);
void charge_levels_events(void);

level_num get_level_flag(void);

#endif /* SPACE_INVADERS_H_ */
