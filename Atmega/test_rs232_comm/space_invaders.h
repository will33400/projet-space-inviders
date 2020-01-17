
#ifndef SPACE_INVADERS_H_
#define SPACE_INVADERS_H_

#include <string.h>
#include <stdint.h>

/* spaceship default parameters */
#define SPACESHIP_LIFE_MAX 3
#define SPACESHIP_SPEED 0

#define SPACESHIP_SHOOT_TYPE '|'

struct spaceship {
	uint8_t life;
	//char appearance[3] = {'<','|','>'};
	uint8_t coordinates[2];
}ship;

/* ennemies default parameters */
#define ENNEMY_LIFE_MIN 1
#define ENNEMY_LIFE_MAX 10

enum ennemy_types {
	ENNEMY_TYPE_1 = 0x0, 
	ENNEMY_TYPE_2 = 0x1, 
	ENNEMY_TYPE_3 = 0x2,
};
/*
const char ennemy_appearances[3][3] = {
	{'(','#',')'},
	{'{','*','}'},
	{'[','"',']'}
};*/

#define ENNEMY_SHOOT_TYPE_1 '#'
#define ENNEMY_SHOOT_TYPE_2 '*'
#define ENNEMY_SHOOT_TYPE_3 '"'

struct ennemy {
	uint8_t life;
	uint8_t shooting_rate;
	uint8_t shoot_type;
	char appearance[3];
	uint8_t coordinates[2];
}ennemy_type;

/* screen settings */
#define SCREEN_DIMENSION_X 150
#define SCREEN_DIMENSION_Y 50

/*const char start_screen[][] = {
	{'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
};*/

/* init functions */

//inline void init_screen();

//inline void init_spaceship();

//inline void init_ennemies();
void init_level_1();
void init_level_2();
void init_level_3();

#endif /* SPACE_INVADERS_H_ */
