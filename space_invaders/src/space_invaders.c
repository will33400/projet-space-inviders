#include "space_invaders.h"
#include "serial.h"
#include "vt100.h"
#include "timer.h"
#include "alea.h"
#include "delay.h"

static uint8_t keyboard_flags;
static uint8_t shield_index;

#define KFPAUS 0x01
static level_num lev_num_flag;

static char spaceship_appearance[OBJECT_HEIGHT_S][OBJECT_WIDTH_S] = { "<|>" };

static char ennemy_appearances[ENNEMY_TYPES][OBJECT_WIDTH_S] = { { "(#)" }, {
		"{*}" }, { "[-]" } };

static char shield_appearance[SHIELD_HEIGHT][SHIELD_WIDTH] = { { " ##### " }, {
		"#######" }, { "#######" }, { "#######" } };

static const char pause_message[] = "PAUSE";

static const char game_over_message[] = "GAME OVER";

static const char levels_messages[] = "LEVEL ";

static const char score_message[] = "SCORE ";

static const char highscore_message[] = "HIGHSCORE ";

static const char life_message[] = "LIFE ";

static uint8_t pause_max = strlen(pause_message);
static uint8_t game_over_max = strlen(game_over_message);
static uint8_t levels_messages_max = strlen(levels_messages);
static uint8_t score_message_max = strlen(score_message);
static uint8_t highscore_message_max = strlen(highscore_message);
static uint8_t life_message_max = strlen(life_message);

static char menu_message[2][14] = { { "SPACE INVADERS" }, { "S : PLAY      " } };

static object_hitbox shield_group[SHIELD_AMOUNT][SHIELD_HEIGHT][SHIELD_WIDTH];
static object_hitbox shield[SHIELD_HEIGHT][SHIELD_WIDTH];
static object_hitbox shield_part;

static object_hitbox spaceship_shoot, ennemy_1_shoot, ennemy_2_shoot,
		ennemy_3_shoot;

static flying_object_s spaceship, ennemy_s_1, ennemy_s_2, ennemy_s_3;

static level_s level_1, level_2, level_3, level_dummy;

static flying_object_s temp_left;
static flying_object_s temp_right;
static flying_object_s temp_up;
static flying_object_s temp_down;

uint32_t highscore;
uint32_t highscore_temp;

void destroy_hitbox(object_hitbox *object) {
	vt100_move(object->x, object->y);
	serial_putchar(' ');
	object->exist = FALSE;
	object->x = 0;
	object->y = 0;
}

void init_flying_object(flying_object_s *object_struct, object_size size,
		object _object, char sprites[OBJECT_HEIGHT_L][OBJECT_WIDTH_L],
		uint8_t life) {

	switch (size) {
	case SMALL:
		object_struct->size = SMALL;
		for (int i = 0; i < OBJECT_HEIGHT_S; i++) {
			for (int j = 0; j < OBJECT_WIDTH_S; j++) {
				object_struct->_hitbox_array.hitbox_s[i][j].appearance =
						sprites[i][j];
				object_struct->_hitbox_array.hitbox_s[i][j]._object = _object;
				object_struct->_hitbox_array.hitbox_s[i][j].exist = TRUE;
			}
		}
		break;
	case MEDIUM:
		object_struct->size = MEDIUM;
		for (int i = 0; i < OBJECT_HEIGHT_M; i++) {
			for (int j = 0; j < OBJECT_WIDTH_M; j++) {
				object_struct->_hitbox_array.hitbox_m[i][j].appearance =
						sprites[i][j];
				object_struct->_hitbox_array.hitbox_m[i][j]._object = _object;
				object_struct->_hitbox_array.hitbox_m[i][j].exist = TRUE;
			}
		}
		break;
	case LARGE:
		object_struct->size = LARGE;
		for (int i = 0; i < OBJECT_HEIGHT_L; i++) {
			for (int j = 0; j < OBJECT_WIDTH_L; j++) {
				object_struct->_hitbox_array.hitbox_l[i][j].appearance =
						sprites[i][j];
				object_struct->_hitbox_array.hitbox_l[i][j]._object = _object;
				object_struct->_hitbox_array.hitbox_l[i][j].exist = TRUE;
			}
		}
		break;
	}

	object_struct->exist = TRUE;
	object_struct->life = life;
}

void destroy_flying_object(flying_object_s *object_struct) {

	switch (object_struct->size) {
	case SMALL:
		for (int i = 0; i < OBJECT_HEIGHT_S; i++) {
			for (int j = 0; j < OBJECT_WIDTH_S; j++) {
				destroy_hitbox(&object_struct->_hitbox_array.hitbox_s[i][j]);
			}
		}
		break;
	case MEDIUM:
		for (int i = 0; i < OBJECT_HEIGHT_M; i++) {
			for (int j = 0; j < OBJECT_WIDTH_M; j++) {
				destroy_hitbox(&object_struct->_hitbox_array.hitbox_m[i][j]);
			}
		}
		break;
	case LARGE:
		for (int i = 0; i < OBJECT_HEIGHT_L; i++) {
			for (int j = 0; j < OBJECT_WIDTH_L; j++) {
				destroy_hitbox(&object_struct->_hitbox_array.hitbox_l[i][j]);
			}
		}
		break;
	}
	object_struct->exist = FALSE;
	erase_flying_object(object_struct);
}

void update_coordinates(flying_object_s *object_struct, uint8_t x, uint8_t y) {

	switch (object_struct->size) {
	case SMALL:
		for (int i = 0; i < OBJECT_HEIGHT_S; i++) {
			for (int j = 0; j < OBJECT_WIDTH_S; j++) {
				object_struct->_hitbox_array.hitbox_s[i][j].x = j + x;
				object_struct->_hitbox_array.hitbox_s[i][j].y = i + y;
			}
		}
		break;
	case MEDIUM:
		for (int i = 0; i < OBJECT_HEIGHT_M; i++) {
			for (int j = 0; j < OBJECT_WIDTH_M; j++) {
				object_struct->_hitbox_array.hitbox_m[i][j].x = j + x;
				object_struct->_hitbox_array.hitbox_m[i][j].y = i + y;
			}
		}
		break;
	case LARGE:
		for (int i = 0; i < OBJECT_HEIGHT_L; i++) {
			for (int j = 0; j < OBJECT_WIDTH_L; j++) {
				object_struct->_hitbox_array.hitbox_l[i][j].x = j + x;
				object_struct->_hitbox_array.hitbox_l[i][j].y = i + y;
			}
		}
		break;
	}
}

void display_flying_object(flying_object_s *object_struct) {

	switch (object_struct->size) {
	case SMALL:
		if (object_struct->exist == TRUE) {
			for (int i = 0; i < OBJECT_HEIGHT_S; i++) {
				for (int j = 0; j < OBJECT_WIDTH_S; j++) {
					vt100_move(object_struct->_hitbox_array.hitbox_s[i][j].x,
							object_struct->_hitbox_array.hitbox_s[i][j].y);
					serial_putchar(
							object_struct->_hitbox_array.hitbox_s[i][j].appearance);
				}
			}
		}
		break;
	case MEDIUM:
		if (object_struct->exist == TRUE) {
			for (int i = 0; i < OBJECT_HEIGHT_M; i++) {
				for (int j = 0; j < OBJECT_WIDTH_M; j++) {
					vt100_move(object_struct->_hitbox_array.hitbox_m[i][j].x,
							object_struct->_hitbox_array.hitbox_m[i][j].y);
					serial_putchar(
							object_struct->_hitbox_array.hitbox_m[i][j].appearance);
				}
			}
		}
		break;
	case LARGE:
		if (object_struct->exist == TRUE) {
			for (int i = 0; i < OBJECT_HEIGHT_L; i++) {
				for (int j = 0; j < OBJECT_WIDTH_L; j++) {
					vt100_move(object_struct->_hitbox_array.hitbox_l[i][j].x,
							object_struct->_hitbox_array.hitbox_l[i][j].y);
					serial_putchar(
							object_struct->_hitbox_array.hitbox_l[i][j].appearance);
				}
			}
		}
		break;
	}
}

void erase_flying_object(flying_object_s *object_struct) {

	switch (object_struct->size) {
	case SMALL:
		for (int i = 0; i < OBJECT_HEIGHT_S; i++) {
			for (int j = 0; j < OBJECT_WIDTH_S; j++) {
				vt100_move(object_struct->_hitbox_array.hitbox_s[i][j].x,
						object_struct->_hitbox_array.hitbox_s[i][j].y);
				serial_putchar(' ');
			}
		}
		break;
	case MEDIUM:
		for (int i = 0; i < OBJECT_HEIGHT_M; i++) {
			for (int j = 0; j < OBJECT_WIDTH_M; j++) {
				vt100_move(object_struct->_hitbox_array.hitbox_m[i][j].x,
						object_struct->_hitbox_array.hitbox_m[i][j].y);
				serial_putchar(' ');
			}
		}
		break;
	case LARGE:
		for (int i = 0; i < OBJECT_HEIGHT_L; i++) {
			for (int j = 0; j < OBJECT_WIDTH_L; j++) {
				vt100_move(object_struct->_hitbox_array.hitbox_l[i][j].x,
						object_struct->_hitbox_array.hitbox_l[i][j].y);
				serial_putchar(' ');
			}
		}
		break;
	}

}

void scroll_flying_object(flying_object_s *object_struct, side _side,
		int8_t offset) {

	int8_t x = 0;
	int8_t y = 0;

	if (object_struct->exist == FALSE)
		return;

	if (flying_object_move_limit(object_struct, _side, offset) == TRUE) {

		erase_flying_object(object_struct);

		switch (_side) {
		case LEFT:
			x--;
			x *= offset;
			break;
		case RIGHT:
			x++;
			x *= offset;
			break;
		case UP:
			y--;
			y *= offset;
			break;
		case DOWN:
			y++;
			y *= offset;
			break;
		}

		switch (object_struct->size) {
		case SMALL:
			update_coordinates(object_struct,
					object_struct->_hitbox_array.hitbox_s[0][0].x + x,
					object_struct->_hitbox_array.hitbox_s[0][0].y + y);
			break;
		case MEDIUM:
			update_coordinates(object_struct,
					object_struct->_hitbox_array.hitbox_m[0][0].x + x,
					object_struct->_hitbox_array.hitbox_m[0][0].y + y);
			break;
		case LARGE:
			update_coordinates(object_struct,
					object_struct->_hitbox_array.hitbox_l[0][0].x + x,
					object_struct->_hitbox_array.hitbox_l[0][0].y + y);
			break;
		}
		display_flying_object(object_struct);
	}

}

_bool flying_object_move_limit(flying_object_s *object_struct, side _side,
		int8_t offset) {
	switch (object_struct->size) {
	case SMALL:
		switch (_side) {
		case LEFT:
			if (object_struct->_hitbox_array.hitbox_s[0][0].x
					< VT100_SCREEN_XMIN + offset)
				return FALSE;
			break;
		case RIGHT:
			if (object_struct->_hitbox_array.hitbox_s[0][0].x + OBJECT_WIDTH_S
					- 1 > VT100_SCREEN_XMAX - offset)
				return FALSE;
			break;
		case UP:
			if (object_struct->_hitbox_array.hitbox_s[0][0].y
					< VT100_SCREEN_YMIN + offset)
				return FALSE;
			break;
		case DOWN:
			if (object_struct->_hitbox_array.hitbox_s[0][0].y + OBJECT_HEIGHT_S
					- 1 > VT100_SCREEN_YMAX - offset)
				return FALSE;
			break;
		}
		return TRUE;
		break;
	case MEDIUM:
		switch (_side) {
		case LEFT:
			if (object_struct->_hitbox_array.hitbox_m[0][0].x
					< VT100_SCREEN_XMIN + offset)
				return FALSE;
			break;
		case RIGHT:
			if (object_struct->_hitbox_array.hitbox_m[0][0].x + OBJECT_WIDTH_M
					- 1 > VT100_SCREEN_XMAX - offset)
				return FALSE;
			break;
		case UP:
			if (object_struct->_hitbox_array.hitbox_m[0][0].y
					< VT100_SCREEN_YMIN + offset)
				return FALSE;
			break;
		case DOWN:
			if (object_struct->_hitbox_array.hitbox_m[0][0].y + OBJECT_HEIGHT_M
					- 1 > VT100_SCREEN_YMAX - offset)
				return FALSE;
			break;
		}
		return TRUE;
		break;
	case LARGE:
		switch (_side) {
		case LEFT:
			if (object_struct->_hitbox_array.hitbox_l[0][0].x
					< VT100_SCREEN_XMIN + offset)
				return FALSE;
			break;
		case RIGHT:
			if (object_struct->_hitbox_array.hitbox_l[0][0].x + OBJECT_WIDTH_L
					- 1 > VT100_SCREEN_XMAX - offset)
				return FALSE;
			break;
		case UP:
			if (object_struct->_hitbox_array.hitbox_l[0][0].y
					< VT100_SCREEN_YMIN + offset)
				return FALSE;
			break;
		case DOWN:
			if (object_struct->_hitbox_array.hitbox_l[0][0].y + OBJECT_HEIGHT_L
					- 1 > VT100_SCREEN_YMAX - offset)
				return FALSE;
			break;
		}
		return TRUE;
		break;
	}
	return FALSE;
}

_bool object_hitbox_collision(object_hitbox *object_1, object_hitbox *object_2) {
	if ((object_1->exist) && (object_2->exist) && (object_1->x == object_2->x)
			&& (object_1->y == object_2->y))
		return TRUE;
	return FALSE;
}

void object_list_append(flying_object_s *object_struct,
		flying_object_s *flying_object_array, uint8_t index) {

	*(flying_object_array + index) = *object_struct;

}

void object_list_destroy(flying_object_s *flying_object_array,
		uint8_t array_size) {

	for (int i = 0; i < array_size; i++) {
		destroy_flying_object(flying_object_array + i);
	}
}

void object_list_display(flying_object_s *flying_object_array,
		uint8_t array_size) {

	for (int i = 0; i < array_size; i++)
		display_flying_object(flying_object_array + i);

}

void object_list_erase(flying_object_s *flying_object_array, uint8_t array_size) {
	for (int i = 0; i < array_size; i++) {
		destroy_flying_object(flying_object_array + i);
		update_coordinates((flying_object_array + i), 0, 0);
	}

}

void object_list_scroll(flying_object_s *flying_object_array,
		uint8_t array_size, side _side, int8_t offset) {
	switch (_side) {
	case LEFT:
		for (int i = 0; i < array_size; i++)
			scroll_flying_object((flying_object_array + i), _side, offset);
		break;
	case RIGHT:
		for (int i = array_size - 1; i >= 0; i--)
			scroll_flying_object((flying_object_array + i), _side, offset);
		break;
	case UP:
		for (int i = 0; i < array_size; i++)
			scroll_flying_object((flying_object_array + i), _side, offset);
		break;
	case DOWN:
		for (int i = array_size - 1; i >= 0; i--)
			scroll_flying_object((flying_object_array + i), _side, offset);
		break;
	}
}

void init_objects(void) {

	char sprite_temp[OBJECT_HEIGHT_L][OBJECT_WIDTH_L];

	for (int i = 0; i < OBJECT_WIDTH_S; i++)
		sprite_temp[OBJECT_HEIGHT_S - 1][i] =
				spaceship_appearance[OBJECT_HEIGHT_S - 1][i];

	init_flying_object(&spaceship, SMALL, SPACESHIP, sprite_temp,
	SPACESHIP_LIFE_MAX);
	update_coordinates(&spaceship,
			((VT100_SCREEN_XMAX / 2) - (OBJECT_WIDTH_S / 2)),
			VT100_SCREEN_YMAX - 1);

	for (int i = 0; i < OBJECT_WIDTH_S; i++)
		sprite_temp[OBJECT_HEIGHT_S - 1][i] = ennemy_appearances[0][i];

	init_flying_object(&ennemy_s_1, SMALL, ENNEMY_TYPE_1, sprite_temp,
	ENNEMY_LIFE_MIN);

	for (int i = 0; i < OBJECT_WIDTH_S; i++)
		sprite_temp[OBJECT_HEIGHT_S - 1][i] = ennemy_appearances[1][i];

	init_flying_object(&ennemy_s_2, SMALL, ENNEMY_TYPE_2, sprite_temp,
	ENNEMY_LIFE_MIN);

	for (int i = 0; i < OBJECT_WIDTH_S; i++)
		sprite_temp[OBJECT_HEIGHT_S - 1][i] = ennemy_appearances[2][i];

	init_flying_object(&ennemy_s_3, SMALL, ENNEMY_TYPE_3, sprite_temp,
	ENNEMY_LIFE_MIN);

	spaceship_shoot.appearance = SPACESHIP_SHOOT;
	spaceship_shoot.exist = TRUE;

	ennemy_1_shoot.appearance = ENNEMY_SHOOT_TYPE_1;
	ennemy_1_shoot.exist = TRUE;

	ennemy_2_shoot.appearance = ENNEMY_SHOOT_TYPE_2;
	ennemy_2_shoot.exist = TRUE;

	ennemy_3_shoot.appearance = ENNEMY_SHOOT_TYPE_3;
	ennemy_3_shoot.exist = TRUE;

	shield_part._object = SHIELD;
	shield_part.exist = TRUE;

	shield_index = 0;

	for (int i = 0; i < SHIELD_HEIGHT; i++) {
		for (int j = 0; j < SHIELD_WIDTH; j++) {
			if (shield_appearance[i][j] != ' ') {
				shield[i][j] = shield_part;
				shield[i][j].appearance = shield_appearance[i][j];
			}
		}
	}

}

void init_levels(void) {

	/**/

}

void init_screen(void) {
	serial_init(SERIAL_BAUD);

	vt100_move(0, 0);
	vt100_clear_screen();
}

void compute_keyboard(void) {
	uint8_t _char = serial_get_last_char();
	if (_char == 'p') {
		toggle_pause();
	}
	if (!(keyboard_flags & KFPAUS)) {
		switch (_char) {
		case 'q':
			if (lev_num_flag != MENU) {
				switch (lev_num_flag) {
				case MENU:
					/* nothing to do */
					break;
				case LEVEL_1:
					scroll_flying_object(&level_1.spaceship, LEFT, 1);
					break;
				case LEVEL_2:
					scroll_flying_object(&level_2.spaceship, LEFT, 1);
					break;
				case LEVEL_3:
					scroll_flying_object(&level_3.spaceship, LEFT, 1);
					break;
				case GAME_OVER:
					/* nothing to do */
					break;
				}

			}
			break;

		case 'd':
			switch (lev_num_flag) {
			case MENU:
				/* nothing to do */
				break;
			case LEVEL_1:
				scroll_flying_object(&level_1.spaceship, RIGHT, 1);
				break;
			case LEVEL_2:
				scroll_flying_object(&level_2.spaceship, RIGHT, 1);
				break;
			case LEVEL_3:
				scroll_flying_object(&level_3.spaceship, RIGHT, 1);
				break;
			case GAME_OVER:
				/* nothing to do */
				break;
			}
			break;

		case 0x0d: //ENTER button

			switch (lev_num_flag) {
			case MENU:
				/* nothing to do */
				break;
			case LEVEL_1:
				init_shoot(SPACESHIP, &level_1,
						level_1.spaceship._hitbox_array.hitbox_s[0][0].x
								+ (OBJECT_WIDTH_S / 2),
						level_1.spaceship._hitbox_array.hitbox_s[0][0].y - 1);
				break;
			case LEVEL_2:
				init_shoot(SPACESHIP, &level_2,
						level_2.spaceship._hitbox_array.hitbox_s[0][0].x
								+ (OBJECT_WIDTH_S / 2),
						level_2.spaceship._hitbox_array.hitbox_s[0][0].y - 1);
				break;
			case LEVEL_3:
				init_shoot(SPACESHIP, &level_3,
						level_3.spaceship._hitbox_array.hitbox_s[0][0].x
								+ (OBJECT_WIDTH_S / 2),
						level_3.spaceship._hitbox_array.hitbox_s[0][0].y - 1);
				break;
			case GAME_OVER:
				/* nothing to do */
				break;
			}
			break;

		case 's':
			if (lev_num_flag == MENU)
				charge_level(LEVEL_1);
			break;

			/*case 'c':
			 init_objects();
			 init_screen();
			 charge_level(lev_num_flag);
			 break;*/

			/*case 'l':
			 if (lev_num_flag != LEVEL_3 + 1)
			 lev_num_flag++;
			 else

			 charge_level(LEVEL_1);

			 break;*/
		}
	}
}

void init_shoot(object _object, level_s *level, uint8_t x, uint8_t y) {

	switch (_object) {
	case SPACESHIP:
		level->shoot_list[level->shoot_index] = spaceship_shoot;
		level->shoot_list[level->shoot_index]._object = SPACESHIP;
		break;
	case ENNEMY_TYPE_1:
		level->shoot_list[level->shoot_index] = ennemy_1_shoot;
		level->shoot_list[level->shoot_index]._object = ENNEMY_TYPE_1;
		break;
	case ENNEMY_TYPE_2:
		level->shoot_list[level->shoot_index] = ennemy_2_shoot;
		level->shoot_list[level->shoot_index]._object = ENNEMY_TYPE_2;
		break;
	case ENNEMY_TYPE_3:
		level->shoot_list[level->shoot_index] = ennemy_3_shoot;
		level->shoot_list[level->shoot_index]._object = ENNEMY_TYPE_3;
		break;
	case SHIELD:
		/* nothing to do */
		break;
	}

	level->shoot_list[level->shoot_index].x = x;
	level->shoot_list[level->shoot_index].y = y;

	vt100_move(level->shoot_list[level->shoot_index].x,
			level->shoot_list[level->shoot_index].y);
	serial_putchar(level->shoot_list[level->shoot_index].appearance);

	level->shoot_index++;
	level->shoot_index %= SHOOT_AMOUNT;
}

void erase_shoot(object_hitbox *_shoot) {
	vt100_move(_shoot->x, _shoot->y);
	serial_putchar(' ');
}

void shoot_events(level_s *level) {

	object_hitbox temp_1;
	object_hitbox temp_2;
	flying_object_s temp_3;

	for (int i = 0; i < SHOOT_AMOUNT; i++) {
		if (level->shoot_list[i].exist == TRUE) {

			erase_shoot(&level->shoot_list[i]);

			if (level->shoot_list[i]._object == ENNEMY_TYPE_1
					|| level->shoot_list[i]._object == ENNEMY_TYPE_2
					|| level->shoot_list[i]._object == ENNEMY_TYPE_3) {
				level->shoot_list[i].y++;

				for (int k = 0; k < OBJECT_WIDTH_S; k++) { //collision ennemy shoot/spaceship
					temp_1 =
							level->spaceship._hitbox_array.hitbox_s[OBJECT_HEIGHT_S
									- 1][k];
					temp_2 = level->shoot_list[i];
					if (object_hitbox_collision(&temp_1, &temp_2)) {
						level->spaceship.life--;
						if (level->spaceship.life == 0) {
							destroy_flying_object(&level->spaceship);
							destroy_hitbox(&temp_2);
							level->shoot_list[i] = temp_2;
						}
						return;
					}
				}

				for (int j = 0; j < SHOOT_AMOUNT; j++) { //collision ennemy shoot/spaceship shoot
					temp_1 = level->shoot_list[i];
					temp_2 = level->shoot_list[j];
					if (object_hitbox_collision(&temp_1, &temp_2)
							&& temp_2._object == SPACESHIP) {
						destroy_hitbox(&temp_1);
						destroy_hitbox(&temp_2);
						level->shoot_list[i] = temp_1;
						level->shoot_list[j] = temp_2;
						level->score += SHOOT_COLLISION_SCORE;
						return;
					}
				}
			}

			if (level->shoot_list[i]._object == SPACESHIP) {
				level->shoot_list[i].y--;

				switch (level->ennemy_size) {
				case SMALL:
					for (int j = 0; j < ENNEMY_AMOUNT; j++) { //collision spaceship shoot/ennemy
						for (int k = 0; k < OBJECT_HEIGHT_S; k++) {
							for (int l = 0; l < OBJECT_WIDTH_S; l++) {
								temp_1 =
										level->ennemy_array[j]._hitbox_array.hitbox_s[k][l];
								temp_2 = level->shoot_list[i];
								temp_3 = level->ennemy_array[j];
								if (object_hitbox_collision(&temp_1, &temp_2)) {
									temp_3.life--;
									if (temp_3.life == 0) {
										destroy_flying_object(&temp_3);
										destroy_hitbox(&temp_2);
										level->ennemy_array[j] = temp_3;
										level->shoot_list[i] = temp_2;
										level->ennemy_num--;
										level->score += ENNEMY_COLLISION_SCORE;
									}
									return;
								}
							}
						}
					}
					break;
				case MEDIUM:
					for (int j = 0; j < ENNEMY_AMOUNT; j++) { //collision spaceship shoot/ennemy
						for (int k = 0; k < OBJECT_HEIGHT_M; k++) {
							for (int l = 0; l < OBJECT_WIDTH_M; l++) {
								temp_1 =
										level->ennemy_array[j]._hitbox_array.hitbox_m[k][l];
								temp_2 = level->shoot_list[i];
								temp_3 = level->ennemy_array[j];
								if (object_hitbox_collision(&temp_1, &temp_2)) {
									temp_3.life--;
									if (temp_3.life == 0) {
										destroy_flying_object(&temp_3);
										destroy_hitbox(&temp_2);
										level->ennemy_array[j] = temp_3;
										level->shoot_list[i] = temp_2;
										level->ennemy_num--;
										level->score += ENNEMY_COLLISION_SCORE;
									}
									return;
								}
							}
						}
					}
					break;
				case LARGE:
					for (int j = 0; j < ENNEMY_AMOUNT; j++) { //collision spaceship shoot/ennemy
						for (int k = 0; k < OBJECT_HEIGHT_L; k++) {
							for (int l = 0; l < OBJECT_WIDTH_L; l++) {
								temp_1 =
										level->ennemy_array[j]._hitbox_array.hitbox_l[k][l];
								temp_2 = level->shoot_list[i];
								temp_3 = level->ennemy_array[j];
								if (object_hitbox_collision(&temp_1, &temp_2)) {
									temp_3.life--;
									if (temp_3.life == 0) {
										destroy_flying_object(&temp_3);
										destroy_hitbox(&temp_2);
										level->ennemy_array[j] = temp_3;
										level->shoot_list[i] = temp_2;
										level->ennemy_num--;
										level->score += ENNEMY_COLLISION_SCORE;
									}
									return;
								}
							}
						}
					}
					break;
				}

				for (int j = 0; j < SHOOT_AMOUNT; j++) { //collision spaceship shoot/ennemy shoot
					temp_1 = level->shoot_list[i];
					temp_2 = level->shoot_list[j];
					if ((object_hitbox_collision(&temp_1, &temp_2)
							&& temp_2._object == ENNEMY_TYPE_1)
							|| (object_hitbox_collision(&temp_1, &temp_2)
									&& temp_2._object == ENNEMY_TYPE_2)
							|| (object_hitbox_collision(&temp_1, &temp_2)
									&& temp_2._object == ENNEMY_TYPE_3)) {
						destroy_hitbox(&temp_1);
						destroy_hitbox(&temp_2);
						level->shoot_list[i] = temp_1;
						level->shoot_list[j] = temp_2;
						level->score += SHOOT_COLLISION_SCORE;
						return;
					}
				}
			}

			for (int j = 0; j < SHIELD_AMOUNT; j++) { //collision handling between a shoot and a shield
				for (int k = 0; k < SHIELD_HEIGHT; k++) {
					for (int l = 0; l < SHIELD_WIDTH; l++) {
						temp_1 = shield_group[j][k][l];
						temp_2 = level->shoot_list[i];
						if ((object_hitbox_collision(&temp_1, &temp_2) == TRUE)
								&& shield_group[j][k][l].exist == TRUE) {
							destroy_hitbox(&temp_1);
							destroy_hitbox(&temp_2);
							shield_group[j][k][l] = temp_1;
							level->shoot_list[i] = temp_2;
						}
					}
				}
			}
			if (level->shoot_list[i].y >= VT100_SCREEN_YMAX) {
				level->shoot_list[i].exist = FALSE;
			} else if (level->shoot_list[i].exist == FALSE) {
				return;
			} else {
				vt100_move(level->shoot_list[i].x, level->shoot_list[i].y);
				serial_putchar(level->shoot_list[i].appearance);
			}

		}
	}
}

void init_shield(uint8_t x, uint8_t y) {
	for (int i = 0; i < SHIELD_HEIGHT; i++) {
		for (int j = 0; j < SHIELD_WIDTH; j++) {
			shield_group[shield_index][i][j] = shield[i][j];
			vt100_move(x + j - (SHIELD_WIDTH / 2), y + i);
			serial_putchar(shield[i][j].appearance);
			shield_group[shield_index][i][j].x = x + j - (SHIELD_WIDTH / 2); // put on each hitbox their coordinates
			shield_group[shield_index][i][j].y = y + i;
		}
	}
	shield_index++;
}

void charge_level(level_num level) {
	switch (level) {
	case MENU:
		init_menu();
		break;
	case LEVEL_1:
		init_level_1();
		break;
	case LEVEL_2:
		init_level_2();
		break;
	case LEVEL_3:
		init_level_3();
		break;
	case GAME_OVER:
		init_game_over();
		break;
	}
}

void display_level_number(level_num level) {
	for (int j = 0; j < levels_messages_max; j++) {
		vt100_move((VT100_SCREEN_WIDTH - levels_messages_max) / 2 + j,
		VT100_SCREEN_HEIGHT / 2);
		serial_putchar(levels_messages[j]);
	}
	vt100_move((VT100_SCREEN_WIDTH + levels_messages_max) / 2,
	VT100_SCREEN_HEIGHT / 2);

	serial_putchar('0' + level);
	delay_(1000);

	for (int j = 0; j < levels_messages_max + 1; j++) {
		vt100_move((VT100_SCREEN_WIDTH - levels_messages_max) / 2 + j,
		VT100_SCREEN_HEIGHT / 2);
		serial_putchar(' ');
	}
}

void display_score(uint32_t score) {
	for (int i = 0; i < score_message_max; i++) {
		vt100_move((SCORE_POS_X - score_message_max - 6) + i,
		SCORE_POS_Y);
		serial_putchar(score_message[i]);
	}
	vt100_move(SCORE_POS_X - 6, SCORE_POS_Y);
	serial_putchar('0' + (score / 10000000));
	vt100_move(SCORE_POS_X - 5, SCORE_POS_Y);
	serial_putchar('0' + ((score / 100000) % 10));
	vt100_move(SCORE_POS_X - 4, SCORE_POS_Y);
	serial_putchar('0' + ((score / 10000) % 10));
	vt100_move(SCORE_POS_X - 3, SCORE_POS_Y);
	serial_putchar('0' + ((score / 1000) % 10));
	vt100_move(SCORE_POS_X - 2, SCORE_POS_Y);
	serial_putchar('0' + ((score / 100) % 10));
	vt100_move(SCORE_POS_X - 1, SCORE_POS_Y);
	serial_putchar('0' + ((score / 10) % 10));
	vt100_move(SCORE_POS_X, SCORE_POS_Y);
	serial_putchar('0' + (score % 10));
}

void display_life(uint8_t life) {
	for (int i = 0; i < life_message_max; i++) {
		vt100_move(LIFE_POS_X + i, LIFE_POS_Y);
		serial_putchar(life_message[i]);
	}
	vt100_move(LIFE_POS_X + life_message_max, LIFE_POS_Y);
	serial_putchar('0' + ((life / 100) % 10));
	vt100_move(LIFE_POS_X + life_message_max + 1, LIFE_POS_Y);
	serial_putchar('0' + ((life / 10) % 10));
	vt100_move(LIFE_POS_X + life_message_max + 2, LIFE_POS_Y);
	serial_putchar('0' + (life % 10));
}

void toggle_pause(void) {

	if (keyboard_flags & KFPAUS) {
		keyboard_flags &= ~KFPAUS;

		for (int i = 0; i < pause_max; i++) {
			vt100_move(VT100_SCREEN_XMAX / 2 - 2 + i,
			VT100_SCREEN_YMAX / 2);
			serial_putchar(' ');
		}
	} else {
		keyboard_flags |= KFPAUS;
		for (int i = 0; i < pause_max; i++) {
			vt100_move(VT100_SCREEN_XMAX / 2 - 2 + i,
			VT100_SCREEN_YMAX / 2);
			serial_putchar(pause_message[i]);
		}
	}
}

void init_menu(void) {

	vt100_clear_screen();

	lev_num_flag = MENU;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 14; j++) {
			vt100_move(j + VT100_SCREEN_XMAX / 4, i + 10);
			serial_putchar(menu_message[i][j]);
		}
	}

	if (highscore_temp > highscore)
		highscore = highscore_temp;

	for (int i = 0; i < highscore_message_max; i++) {
		vt100_move((SCORE_POS_X / 2) + i,
		SCORE_POS_Y / 2);
		serial_putchar(highscore_message[i]);
	}

	vt100_move((SCORE_POS_X / 2) + highscore_message_max, SCORE_POS_Y / 2);
	serial_putchar('0' + (highscore / 10000000));
	vt100_move((SCORE_POS_X / 2) + highscore_message_max + 1, SCORE_POS_Y / 2);
	serial_putchar('0' + ((highscore / 100000) % 10));
	vt100_move((SCORE_POS_X / 2) + highscore_message_max + 2, SCORE_POS_Y / 2);
	serial_putchar('0' + ((highscore / 10000) % 10));
	vt100_move((SCORE_POS_X / 2) + highscore_message_max + 3, SCORE_POS_Y / 2);
	serial_putchar('0' + ((highscore / 1000) % 10));
	vt100_move((SCORE_POS_X / 2) + highscore_message_max + 4, SCORE_POS_Y / 2);
	serial_putchar('0' + ((highscore / 100) % 10));
	vt100_move((SCORE_POS_X / 2) + highscore_message_max + 5, SCORE_POS_Y / 2);
	serial_putchar('0' + ((highscore / 10) % 10));
	vt100_move((SCORE_POS_X / 2) + highscore_message_max + 6, SCORE_POS_Y / 2);
	serial_putchar('0' + (highscore % 10));
}

void init_level_1(void) {

	lev_num_flag = LEVEL_1;

	init_objects();

	vt100_clear_screen();

	display_level_number(LEVEL_1);

	uint8_t j = VT100_SCREEN_XMIN;
	uint8_t k = VT100_SCREEN_YMIN;

	level_1 = level_dummy;

	level_1.ennemy_num = ENNEMY_AMOUNT;
	level_1.ennemy_move_side = RIGHT;
	level_1.spaceship = spaceship;

	for (int i = 0; i < ENNEMY_AMOUNT; i++) {

		update_coordinates(&ennemy_s_1, (j * (OBJECT_WIDTH_S)), k);
		object_list_append(&ennemy_s_1, level_1.ennemy_array, i);

		j++;
		j %= 9;
		if (j == 0) {
			j = VT100_SCREEN_YMIN;
			k++;
		}
	}

	level_1.ennemy_size = SMALL;
	level_1.refresh_frequ = 130;

	init_shield(10, 18);
	init_shield(40, 18);
	init_shield(70, 18);

	display_score(0);
	display_life(level_1.spaceship.life);

	object_list_display(level_1.ennemy_array, ENNEMY_AMOUNT);
	display_flying_object(&level_1.spaceship);
}

void init_level_2(void) {

	lev_num_flag = LEVEL_2;

	init_objects();

	vt100_clear_screen();

	display_level_number(LEVEL_2);

	uint8_t j = VT100_SCREEN_XMIN;
	uint8_t k = VT100_SCREEN_YMIN;

	level_2 = level_dummy;

	level_2.score = level_1.score;

	level_2.ennemy_num = ENNEMY_AMOUNT;
	level_2.ennemy_move_side = RIGHT;
	level_2.spaceship = spaceship;

	for (int i = 0; i < ENNEMY_AMOUNT; i++) {

		update_coordinates(&ennemy_s_2, (j * (OBJECT_WIDTH_S)), k);
		object_list_append(&ennemy_s_2, level_2.ennemy_array, i);

		j++;
		j %= 9;
		if (j == 0) {
			j = VT100_SCREEN_YMIN;
			k++;
		}
	}

	level_2.ennemy_size = SMALL;
	level_2.refresh_frequ = 130;

	init_shield(10, 18);
	init_shield(40, 18);
	init_shield(70, 18);

	object_list_display(level_2.ennemy_array, ENNEMY_AMOUNT);
	display_flying_object(&level_2.spaceship);
}

void init_level_3(void) {

	lev_num_flag = LEVEL_3;

	init_objects();

	vt100_clear_screen();

	display_level_number(LEVEL_3);

	uint8_t j = VT100_SCREEN_XMIN;
	uint8_t k = VT100_SCREEN_YMIN;

	level_3 = level_dummy;

	level_3.score = level_2.score;

	level_3.ennemy_num = ENNEMY_AMOUNT;
	level_3.ennemy_move_side = RIGHT;
	level_3.spaceship = spaceship;

	for (int i = 0; i < ENNEMY_AMOUNT; i++) {

		update_coordinates(&ennemy_s_3, (j * (OBJECT_WIDTH_S)), k);
		object_list_append(&ennemy_s_3, level_3.ennemy_array, i);

		j++;
		j %= 9;
		if (j == 0) {
			j = VT100_SCREEN_YMIN;
			k++;
		}
	}

	level_3.ennemy_size = SMALL;
	level_3.refresh_frequ = 130;

	init_shield(10, 18);
	init_shield(40, 18);
	init_shield(70, 18);

	object_list_display(level_3.ennemy_array, ENNEMY_AMOUNT);
	display_flying_object(&level_3.spaceship);
}

void init_game_over(void) {

	lev_num_flag = GAME_OVER;

	vt100_clear_screen();

	for (int i = 0; i < game_over_max; i++) {
		vt100_move(VT100_SCREEN_XMAX / 2 - 2 + i,
		VT100_SCREEN_YMAX / 2);
		serial_putchar(game_over_message[i]);
	}

	delay_(5000);

	vt100_clear_screen();

	charge_level(MENU);
}

void levels_events(level_s *level) {

	if (!(keyboard_flags & KFPAUS) && lev_num_flag != MENU
			&& lev_num_flag != GAME_OVER) {

		level->alea_ennemy_array_i = LFSR_get();
		level->alea_ennemy_array_i %= ENNEMY_AMOUNT;

		switch (level->ennemy_size) {
		case SMALL:
			temp_left._hitbox_array.hitbox_s[0][0].x =
			VT100_SCREEN_WIDTH / 2;
			temp_right._hitbox_array.hitbox_s[0][0].x =
			VT100_SCREEN_WIDTH / 2;
			temp_up._hitbox_array.hitbox_s[0][0].y = VT100_SCREEN_HEIGHT / 2;
			temp_down._hitbox_array.hitbox_s[0][0].y =
			VT100_SCREEN_HEIGHT / 2;

			for (int i = 0; i < ENNEMY_AMOUNT; i++) {
				if (level->ennemy_array[i].exist) {
					if (temp_left._hitbox_array.hitbox_s[0][0].x
							> level->ennemy_array[i]._hitbox_array.hitbox_s[0][0].x)
						temp_left = level->ennemy_array[i];
					if (temp_right._hitbox_array.hitbox_s[0][0].x
							< level->ennemy_array[i]._hitbox_array.hitbox_s[0][0].x)
						temp_right = level->ennemy_array[i];
					if (temp_up._hitbox_array.hitbox_s[0][0].y
							> level->ennemy_array[i]._hitbox_array.hitbox_s[0][0].y)
						temp_up = level->ennemy_array[i];
					if (temp_down._hitbox_array.hitbox_s[0][0].y
							< level->ennemy_array[i]._hitbox_array.hitbox_s[0][0].y)
						temp_down = level->ennemy_array[i];
				}
			}
			break;
		case MEDIUM:
			temp_left._hitbox_array.hitbox_m[0][0].x =
			VT100_SCREEN_WIDTH / 2;
			temp_right._hitbox_array.hitbox_m[0][0].x =
			VT100_SCREEN_WIDTH / 2;
			temp_up._hitbox_array.hitbox_m[0][0].y = VT100_SCREEN_HEIGHT / 2;
			temp_down._hitbox_array.hitbox_m[0][0].y =
			VT100_SCREEN_HEIGHT / 2;

			for (int i = 0; i < ENNEMY_AMOUNT; i++) {
				if (level->ennemy_array[i].exist) {
					if (temp_left._hitbox_array.hitbox_m[0][0].x
							> level->ennemy_array[i]._hitbox_array.hitbox_m[0][0].x)
						temp_left = level->ennemy_array[i];
					if (temp_right._hitbox_array.hitbox_m[0][0].x
							< level->ennemy_array[i]._hitbox_array.hitbox_m[0][0].x)
						temp_right = level->ennemy_array[i];
					if (temp_up._hitbox_array.hitbox_m[0][0].y
							> level->ennemy_array[i]._hitbox_array.hitbox_m[0][0].y)
						temp_up = level->ennemy_array[i];
					if (temp_down._hitbox_array.hitbox_m[0][0].y
							< level->ennemy_array[i]._hitbox_array.hitbox_m[0][0].y)
						temp_down = level->ennemy_array[i];
				}
			}
			break;
		case LARGE:
			temp_left._hitbox_array.hitbox_l[0][0].x =
			VT100_SCREEN_WIDTH / 2;
			temp_right._hitbox_array.hitbox_l[0][0].x =
			VT100_SCREEN_WIDTH / 2;
			temp_up._hitbox_array.hitbox_l[0][0].y = VT100_SCREEN_HEIGHT / 2;
			temp_down._hitbox_array.hitbox_l[0][0].y =
			VT100_SCREEN_HEIGHT / 2;

			for (int i = 0; i < ENNEMY_AMOUNT; i++) {
				if (level->ennemy_array[i].exist) {
					if (temp_left._hitbox_array.hitbox_l[0][0].x
							> level->ennemy_array[i]._hitbox_array.hitbox_l[0][0].x)
						temp_left = level->ennemy_array[i];
					if (temp_right._hitbox_array.hitbox_l[0][0].x
							< level->ennemy_array[i]._hitbox_array.hitbox_l[0][0].x)
						temp_right = level->ennemy_array[i];
					if (temp_up._hitbox_array.hitbox_l[0][0].y
							> level->ennemy_array[i]._hitbox_array.hitbox_l[0][0].y)
						temp_up = level->ennemy_array[i];
					if (temp_down._hitbox_array.hitbox_l[0][0].y
							< level->ennemy_array[i]._hitbox_array.hitbox_l[0][0].y)
						temp_down = level->ennemy_array[i];
				}
			}
			break;
		}

		if (level->ennemy_move_side == RIGHT) {
			object_list_scroll(level->ennemy_array, ENNEMY_AMOUNT, RIGHT, 1);
			if (flying_object_move_limit(&temp_right, RIGHT, 2) == FALSE) {
				object_list_scroll(level->ennemy_array, ENNEMY_AMOUNT, DOWN, 1);
				level->ennemy_move_side = LEFT;
			}
		} else if (level->ennemy_move_side == LEFT) {
			object_list_scroll(level->ennemy_array, ENNEMY_AMOUNT, LEFT, 1);
			if (flying_object_move_limit(&temp_left, LEFT, 2) == FALSE) {
				object_list_scroll(level->ennemy_array, ENNEMY_AMOUNT, DOWN, 1);
				level->ennemy_move_side = RIGHT;
			}
		}

		while (level->ennemy_num != 0) {
			if (level->ennemy_array[level->alea_ennemy_array_i].exist) {
				switch (level->ennemy_size) {
				case SMALL:
					init_shoot(
							level->ennemy_array[level->alea_ennemy_array_i]._hitbox_array.hitbox_s[OBJECT_HEIGHT_S
									- 1][0]._object, level,
							level->ennemy_array[level->alea_ennemy_array_i]._hitbox_array.hitbox_s[OBJECT_HEIGHT_S
									- 1][0].x + (OBJECT_WIDTH_S / 2),
							level->ennemy_array[level->alea_ennemy_array_i]._hitbox_array.hitbox_s[OBJECT_HEIGHT_S
									- 1][0].y + 1);
					break;
				case MEDIUM:
					init_shoot(
							level->ennemy_array[level->alea_ennemy_array_i]._hitbox_array.hitbox_m[OBJECT_HEIGHT_M
									- 1][0]._object, level,
							level->ennemy_array[level->alea_ennemy_array_i]._hitbox_array.hitbox_m[OBJECT_HEIGHT_M
									- 1][0].x + (OBJECT_WIDTH_M / 2),
							level->ennemy_array[level->alea_ennemy_array_i]._hitbox_array.hitbox_m[OBJECT_HEIGHT_M
									- 1][0].y + 1);
					break;
				case LARGE:
					init_shoot(
							level->ennemy_array[level->alea_ennemy_array_i]._hitbox_array.hitbox_l[OBJECT_HEIGHT_L
									- 1][0]._object, level,
							level->ennemy_array[level->alea_ennemy_array_i]._hitbox_array.hitbox_l[OBJECT_HEIGHT_L
									- 1][0].x + (OBJECT_WIDTH_L / 2),
							level->ennemy_array[level->alea_ennemy_array_i]._hitbox_array.hitbox_l[OBJECT_HEIGHT_L
									- 1][0].y + 1);
					break;
				}
				break;
			} else {
				level->alea_ennemy_array_i++;
				level->alea_ennemy_array_i %= ENNEMY_AMOUNT;
			}
		}

		delay_(level->refresh_frequ);

		for (int i = 0; i < 2; i++) {
			shoot_events(level);
			highscore_temp = level->score;
		}

		if (level->spaceship.life != level->life_temp) {
			display_life(level->spaceship.life);
			level->life_temp = level->spaceship.life;
		}
		if (level->score != level->score_temp) {
			display_score(level->score);
			level->score_temp = level->score;
		}

		switch (temp_down.size) {
		case SMALL:
			if (temp_down._hitbox_array.hitbox_s[OBJECT_HEIGHT_S - 1][0].y
					== VT100_SCREEN_HEIGHT - 1 || level->spaceship.life == 0)
				charge_level(GAME_OVER);
			break;
		case MEDIUM:
			if (temp_down._hitbox_array.hitbox_m[OBJECT_HEIGHT_M - 1][0].y
					== VT100_SCREEN_HEIGHT - 1 || level->spaceship.life == 0)
				charge_level(GAME_OVER);
			break;
		case LARGE:
			if (temp_down._hitbox_array.hitbox_l[OBJECT_HEIGHT_L - 1][0].y
					== VT100_SCREEN_HEIGHT - 1 || level->spaceship.life == 0)
				charge_level(GAME_OVER);
			break;
		}

		if (level->ennemy_num == 0) {
			lev_num_flag++;
			lev_num_flag %= GAME_OVER;
			charge_level(lev_num_flag);
		}
	}
}

void charge_levels_events(void) {
	LFSR_update();
	compute_keyboard();
	switch (lev_num_flag) {
	case MENU:
		/* nothing to do */
		break;
	case LEVEL_1:
		levels_events(&level_1);
		break;
	case LEVEL_2:
		levels_events(&level_2);
		break;
	case LEVEL_3:
		levels_events(&level_3);
		break;
	case GAME_OVER:
		/* nothing to do */
		break;
	}
}

level_num get_level_flag(void) {
	return lev_num_flag;
}

#define tim2_irq TIM2_IRQHandler

void tim2_irq(void) {

}

#define tim3_irq TIM3_IRQHandler

void tim3_irq(void) {

}
