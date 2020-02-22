#include "space_invaders.h"
#include "serial.h"
#include "vt100.h"
#include "timer.h"
#include "alea.h"
#include "delay.h"

const char spaceship_appearance[SPACESHIP_WIDTH] = "<|>";

const char ennemy_appearances[ENNEMY_TYPES][ENNEMY_WIDTH] = { { "(#)" },
		{ "{*}" }, { "[-]" } };

const char shield_appearance[SHIELD_HEIGHT][SHIELD_WIDTH] = { { " ##### " }, {
		"#######" }, { "#######" }, { "#######" } };

const char pause_message[] = "PAUSE";

uint8_t pause_max = strlen(pause_message);

const char menu_message[2][14] = { { "SPACE INVADERS" }, { "ENTER : PLAY  " } };

shield_hitbox shield_group[SHIELD_AMOUNT][SHIELD_HEIGHT][SHIELD_WIDTH];

shield_hitbox shield[SHIELD_HEIGHT][SHIELD_WIDTH];

shield_hitbox shield_part;

ennemy ennemy_1, ennemy_2, ennemy_3;

ennemy ennemy_group[LEVEL_AMOUNT][ENNEMY_AMOUNT];

shoot shoot_list[SHOOT_AMOUNT];

shoot spaceship_shoot, ennemy_1_shoot, ennemy_2_shoot, ennemy_3_shoot;

void init_sprites(void) {

	_spaceship.life = SPACESHIP_LIFE_MAX;
	for (int i = 0; i < SPACESHIP_WIDTH; i++)
		_spaceship.appearance[i] = spaceship_appearance[i];
	_spaceship.x = VT100_SCREEN_XMAX / 2;
	_spaceship.y = VT100_SCREEN_YMAX;

	spaceship_shoot.appearance = SPACESHIP_SHOOT;
	spaceship_shoot.exist = TRUE;

	ennemy_1.type = ENNEMY_TYPE_1;
	ennemy_1.life = ENNEMY_LIFE_MIN;
	for (int i = 0; i < ENNEMY_WIDTH; i++)
		ennemy_1.appearance[i] = ennemy_appearances[0][i];
	ennemy_1.shoot_type = ENNEMY_SHOOT_TYPE_1;
	ennemy_1.exist = TRUE;

	ennemy_1_shoot.appearance = ENNEMY_SHOOT_TYPE_1;
	ennemy_1_shoot.exist = TRUE;

	ennemy_2.type = ENNEMY_TYPE_2;
	ennemy_2.life = ENNEMY_LIFE_MIN;
	for (int i = 0; i < ENNEMY_WIDTH; i++)
		ennemy_2.appearance[i] = ennemy_appearances[1][i];
	ennemy_2.shoot_type = ENNEMY_SHOOT_TYPE_2;
	ennemy_2.exist = TRUE;

	ennemy_2_shoot.appearance = ENNEMY_SHOOT_TYPE_2;
	ennemy_2_shoot.exist = TRUE;

	ennemy_3.type = ENNEMY_TYPE_3;
	ennemy_3.life = ENNEMY_LIFE_MIN;
	for (int i = 0; i < ENNEMY_WIDTH; i++)
		ennemy_3.appearance[i] = ennemy_appearances[2][i];
	ennemy_3.shoot_type = ENNEMY_SHOOT_TYPE_3;
	ennemy_3.exist = TRUE;

	ennemy_3_shoot.appearance = ENNEMY_SHOOT_TYPE_3;
	ennemy_3_shoot.exist = TRUE;

	for (int j = 0; j < 4; j++) {
		for (int k = 0; k < (ENNEMY_AMOUNT / 8) * ENNEMY_WIDTH; k++) {
			ennemy_group[0][k + j] = ennemy_1;
			ennemy_group[0][k + j].x = k + VT100_SCREEN_XMIN;
			ennemy_group[0][k + j].y = j + VT100_SCREEN_YMIN;
		}
	}

	shield_part._object = SHIELD;
	shield_part.exist = TRUE;

	for (int i = 0; i < SHIELD_HEIGHT; i++) {
		for (int j = 0; j < SHIELD_WIDTH; j++) {
			if (shield_appearance[i][j] != ' ') {
				shield[i][j] = shield_part;
				shield[i][j].appearance = shield_appearance[i][j];
			}
		}
	}

	//init_tim_base(TIMER_2, 0x00ff);
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
				scroll_spaceship(LEFT);
			}
			break;

		case 'd':
			if (lev_num_flag != MENU) {
				scroll_spaceship(RIGHT);
			}
			break;

		case 0x0d: //ENTER button
			if (lev_num_flag != MENU) {
				init_shoot(SPACESHIP, _spaceship.x, _spaceship.y - 1);
			} else
				charge_level(LEVEL_1);
			break;

		case 'c':
			init_sprites();
			init_screen();
			charge_level(lev_num_flag);
			break;

		case 'l':
			if (lev_num_flag != LEVEL_3 + 1)
				lev_num_flag++;
			else
				lev_num_flag = LEVEL_1;

			charge_level(lev_num_flag);

			break;
		}
	}
}

void init_spaceship(void) {
	for (int i = 0; i < SPACESHIP_WIDTH; i++) {
		vt100_move(_spaceship.x + i - (SPACESHIP_WIDTH / 2), _spaceship.y);
		serial_putchar(_spaceship.appearance[i]);
	}
}

void erase_spaceship(void) {
	for (int i = 0; i < SPACESHIP_WIDTH; i++) {
		vt100_move(_spaceship.x + i - (SPACESHIP_WIDTH / 2), _spaceship.y);
		serial_putchar(' ');
	}

}

void scroll_spaceship(side _side) {
	switch (_side) {
	case LEFT:
		erase_spaceship();
		if (_spaceship.x > VT100_SCREEN_XMIN + (SPACESHIP_WIDTH / 2))
			_spaceship.x--;
		init_spaceship();
		break;
	case RIGHT:
		erase_spaceship();
		if (_spaceship.x < VT100_SCREEN_XMAX - (SPACESHIP_WIDTH / 2))
			_spaceship.x++;
		init_spaceship();
		break;
	}

}

void init_shoot(object _object, uint8_t x, uint8_t y) {

	switch (_object) {
	case SPACESHIP:
		shoot_list[shoot_index] = spaceship_shoot;
		break;
	case ENNEMY_TYPE_1:
		shoot_list[shoot_index] = ennemy_1_shoot;
		break;
	case ENNEMY_TYPE_2:
		shoot_list[shoot_index] = ennemy_2_shoot;
		break;
	case ENNEMY_TYPE_3:
		shoot_list[shoot_index] = ennemy_3_shoot;
		break;
	case SHIELD:
		/* nothing to do */
		break;
	}

	shoot_list[shoot_index].x = x;
	shoot_list[shoot_index].y = y;

	vt100_move(shoot_list[shoot_index].x, shoot_list[shoot_index].y);
	serial_putchar(shoot_list[shoot_index].appearance);

	shoot_index++;
	shoot_index %= SHOOT_AMOUNT;
}

void erase_shoot(shoot *_shoot) {
	vt100_move(_shoot->x, _shoot->y);
	serial_putchar(' ');
}

void scroll_shoot(void) {

	for (int i = 0; i < SHOOT_AMOUNT; i++) {
		if (shoot_list[i].exist == TRUE) {

			erase_shoot(&shoot_list[i]);

			if (shoot_list[i]._object == SPACESHIP) {
				shoot_list[i].y--;
				for (int j = 0; j < (ENNEMY_AMOUNT / 8); j++) {
					if ((shoot_list[i].x == ennemy_group[lev_num_flag - 1][j].x
							&& shoot_list[i].y
									== ennemy_group[lev_num_flag - 1][j].y
							&& ennemy_group[lev_num_flag - 1][j].exist == TRUE)) {

						erase_ennemy(&ennemy_group[lev_num_flag - 1][j]);
						ennemy_group[lev_num_flag - 1][j].exist = FALSE;
						shoot_list[i].exist = FALSE;
					}
				}

			} else {
				shoot_list[i].y++;
			}

			for (int j = 0; j < SHIELD_AMOUNT; j++) { //collision handling between a shoot and a shield
				for (int k = 0; k < SHIELD_HEIGHT; k++) {
					for (int l = 0; l < SHIELD_WIDTH; l++) {
						if (shield_group[j][k][l].x == shoot_list[i].x
								&& shield_group[j][k][l].y == shoot_list[i].y
								&& shield_group[j][k][l].exist == TRUE) {
							shield_group[j][k][l].exist = FALSE;
							shoot_list[i].exist = FALSE;
							erase_shoot(&shoot_list[i]);
						}
					}
				}
			}

			if (shoot_list[i].y >= VT100_SCREEN_YMAX) {
				shoot_list[i].exist = FALSE;
			} else if (shoot_list[i].exist == FALSE) {
				return;
			} else {
				vt100_move(shoot_list[i].x, shoot_list[i].y);
				serial_putchar(shoot_list[i].appearance);
			}
		}
	}
}
/*
 void init_ennemy(ennemy *_ennemy, uint8_t x, uint8_t y) {

 for (int i = 0; i < ENNEMY_WIDTH; i++) {
 vt100_move(x + i + (ENNEMY_WIDTH / 2), y);
 serial_putchar(_ennemy->appearance[i]);
 }
 }
 */

void erase_ennemy(ennemy *_ennemy) {
	for (int i = 0; i < ENNEMY_WIDTH; i++) {
		vt100_move(_ennemy->x, _ennemy->y);
		serial_putchar(' ');
	}
}

void scroll_ennemies(void) {

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
	shield_index = 0;
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
	}
}

void toggle_pause(void) {

	if (keyboard_flags & KFPAUS) {
		keyboard_flags &= ~KFPAUS;

		for (int i = 0; i < pause_max; i++) {
			vt100_move(VT100_SCREEN_XMAX / 2 - 2 + i, VT100_SCREEN_YMAX / 2);
			serial_putchar(' ');
		}
	} else {
		keyboard_flags |= KFPAUS;
		for (int i = 0; i < pause_max; i++) {
			vt100_move(VT100_SCREEN_XMAX / 2 - 2 + i, VT100_SCREEN_YMAX / 2);
			serial_putchar(pause_message[i]);
		}
	}
}

void init_menu(void) {

	lev_num_flag = MENU;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 14; j++) {
			vt100_move(j + VT100_SCREEN_XMAX / 4, i + 10);
			serial_putchar(menu_message[i][j]);
		}
	}

}

void init_level_1(void) {

	lev_num_flag = LEVEL_1;

	vt100_clear_screen();

	init_shield(10, 19);
	init_shield(40, 19);
	init_shield(70, 19);

	for (int j = 0; j < ENNEMY_WIDTH; j++) {
		vt100_move(ennemy_group[0][j].x + j, ennemy_group[0][0].y);
		serial_putchar(ennemy_group[0][j].appearance[j]);
	}
	/*
	 for (int i = 0; i < (ENNEMY_AMOUNT / 8) * ENNEMY_WIDTH * 4; i +=
	 ENNEMY_WIDTH) {
	 for (int j = 0; j < ENNEMY_WIDTH; j++) {
	 vt100_move(ennemy_group[0][i + j].x, ennemy_group[0][i + j].y);
	 serial_putchar(ennemy_group[0][i + j].appearance[j]);
	 }
	 }
	 */
	/*
	 for (int j = VT100_SCREEN_YMIN; j < 4; j++) {
	 for (int i = 0; i < ENNEMY_WIDTH * 8; i += ENNEMY_WIDTH) {
	 init_ennemy(&ennemy_1, i + 30, j);
	 }
	 }

	 for (int i = 0; i < ENNEMY_WIDTH * 8; i += ENNEMY_WIDTH) {
	 init_ennemy(&ennemy_2, i + 30, 4);
	 }
	 */
	init_spaceship();
}

void init_level_2(void) {

	lev_num_flag = LEVEL_2;

	vt100_clear_screen();

	init_shield(10, 19);
	init_shield(40, 19);
	init_shield(70, 19);

	init_spaceship();
}

void init_level_3(void) {

	lev_num_flag = LEVEL_3;

	vt100_clear_screen();

	init_shield(10, 19);
	init_shield(40, 19);
	init_shield(70, 19);

	init_spaceship();
}

#define tim2_irq TIM2_IRQHandler

void tim2_irq(void) {
	//scroll_shoot();
}

#define tim3_irq TIM3_IRQHandler

void tim3_irq(void) {

}
