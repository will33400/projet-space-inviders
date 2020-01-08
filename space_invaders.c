
#include <stdio.h>
#include <string.h>

/* spaceship default parameters */
#define SPACESHIP_LIFE_MAX 3
#define SPACESHIP_SPEED 0
#define SPACESHIP_APPEARANCE '<|>'
#define SPACESHIP_SHOOT_TYPE '|'

/* ennemies default parameters */
#define ENNEMY_LIFE_MIN 1
#define ENNEMY_LIFE_MAX 10

enum ennemy_types {
    ENNEMY_TYPE_1 = 0x0,
    ENNEMY_TYPE_2 = 0x1,
    ENNEMY_TYPE_3 = 0x2,
}

#define ENNEMY_APPEARANCE_1 '(#)'
#define ENNEMY_APPEARANCE_2 '{*}'
#define ENNEMY_APPEARANCE_3 '["]'

#define ENNEMY_SHOOT_TYPE_1 '#'
#define ENNEMY_SHOOT_TYPE_2 '*'
#define ENNEMY_SHOOT_TYPE_3 '"'

/* screen settings */
#define SCREEN_DIMENSION_X 150
#define SCREEN_DIMENSION_Y 50

typedef struct spaceship spaceship;

struct spaceship {
    uint8_t life;
    char    appearance[3];
    uint8_t coordinates[2];
}

typedef struct ennemy ennemy;

struct ennemy {
    uint8_t life;
    uint8_t shooting_rate;
    uint8_t shoot_type;
    char    appearance[3];
    uint8_t coordinates[2];
}

typedef struct coordinates coordinates;

struct coordinates {
    uint8_t x;
    uint8_t y;
}

void init_screen() {

}

void init_spaceship() {

}

ennemy init_ennemy(enum ennemy_types type) {
    switch(type) {
        case ENNEMY_TYPE_1:
        ennemy.life = 1;
        ennemy.shooting_rate = 0;
        ennemy.shoot_type = ENNEMY_SHOOT_TYPE_1;
        ennemy.appearance = ENNEMY_APPEARANCE_1;
        ennemy.coordinates = 0;
        break;

        case ENNEMY_TYPE_2:
        ennemy.life = 1;
        ennemy.shooting_rate = 0;
        ennemy.shoot_type = ENNEMY_SHOOT_TYPE_2;
        ennemy.appearance = ENNEMY_APPEARANCE_2;
        ennemy.coordinates = 0;
        break;

        case ENNEMY_TYPE_3:
        ennemy.life = 5;
        ennemy.shooting_rate = 0;
        ennemy.shoot_type = ENNEMY_SHOOT_TYPE_3;
        ennemy.appearance = ENNEMY_APPEARANCE_3;
        ennemy.coordinates = 0;
        break;
    }
}

coordinates update_coordinates(uint8_t x, uint8_t y) {
    coordinates.x = x;
    coordinates.y = y;
}