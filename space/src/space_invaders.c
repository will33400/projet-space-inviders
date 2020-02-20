#include "space_invaders.h"
#include "serial.h"
#include "vt100.h"
#include "timer.h"
#include "alea.h"
#include "delay.h"

const uint8_t spaceship_appearance[3] = "<|>";

const uint8_t ennemy_appearances[3][3] = { { "(#)" }, { "{*}" }, { "[-]" } };

const uint8_t shield_appearance[4][7] = { { " ##### " }, { "#######" }, {
		"#######" }, { "#######" } };

const uint8_t pause_message[] = "PAUSE";

const uint8_t menu_message[2][20] = { { "SPACE INVADERS" }, { "ENTER : PLAY" } };

//ennemy *ennemies[][];

ennemy ennemy_1, ennemy_2, ennemy_3;

shoot shoot_list[SHOOT_AMOUNT];

shoot spaceship_shoot, ennemy_1_shoot, ennemy_2_shoot, ennemy_3_shoot;

void init_sprites() {

	_spaceship.life = SPACESHIP_LIFE_MAX;
	for (int i = 0; i < 3; i++)
		_spaceship.appearance[i] = spaceship_appearance[i];
	_spaceship.x = VT100_SCREEN_XMAX / 2;
	_spaceship.y = VT100_SCREEN_YMAX;

	spaceship_shoot.appearance = SPACESHIP_SHOOT;
	spaceship_shoot.exist = TRUE;

	ennemy_1.type = ENNEMY_TYPE_1;
	ennemy_1.life = 1;
	for (int i = 0; i < 3; i++)
		ennemy_1.appearance[i] = ennemy_appearances[1][i];
	ennemy_1.shoot_type = ENNEMY_SHOOT_TYPE_1;
	ennemy_1.exist = TRUE;

	ennemy_1_shoot.appearance = ENNEMY_SHOOT_TYPE_1;
	ennemy_1_shoot.exist = TRUE;

	ennemy_2.type = ENNEMY_TYPE_2;
	ennemy_2.life = 1;
	for (int i = 0; i < 3; i++)
		ennemy_2.appearance[i] = ennemy_appearances[2][i];
	ennemy_2.shoot_type = ENNEMY_SHOOT_TYPE_2;
	ennemy_2.exist = TRUE;

	ennemy_2_shoot.appearance = ENNEMY_SHOOT_TYPE_2;
	ennemy_2_shoot.exist = TRUE;

	ennemy_3.type = ENNEMY_TYPE_3;
	ennemy_3.life = 1;
	for (int i = 0; i < 3; i++)
		ennemy_3.appearance[i] = ennemy_appearances[3][i];
	ennemy_3.shoot_type = ENNEMY_SHOOT_TYPE_3;
	ennemy_3.exist = TRUE;

	ennemy_3_shoot.appearance = ENNEMY_SHOOT_TYPE_3;
	ennemy_3_shoot.exist = TRUE;

	init_tim_base(TIMER_2, 0x00ff);
}

void init_screen() {
	serial_init(SERIAL_BAUD);

	vt100_move(0, 0);
	vt100_clear_screen();
}

void compute_keyboard() {
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
			//shoot _shoot;
			//for (int i = 0; )
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

void init_spaceship() {
	for (int i = 0; i < 3; i++) {
		vt100_move(_spaceship.x + i - 1, _spaceship.y);
		serial_putchar(_spaceship.appearance[i]);
	}
}

void erase_spaceship() {
	for (int i = 0; i < 3; i++) {
		vt100_move(_spaceship.x + i - 1, _spaceship.y);
		serial_putchar(' ');
	}

}

void scroll_spaceship(side _side) {
	switch (_side) {
	case LEFT:
		erase_spaceship();
		if (_spaceship.x > 2)
			_spaceship.x--;
		init_spaceship();
		break;
	case RIGHT:
		erase_spaceship();
		if (_spaceship.x < 79)
			_spaceship.x++;
		init_spaceship();
		break;
	}

}

void init_shoot(object _object, uint8_t x, uint8_t y) {
	if (_object == SPACESHIP)
		shoot_list[shoot_index] = spaceship_shoot;
	else
		shoot_list[shoot_index] = spaceship_shoot;

	shoot_list[shoot_index].x = x;
	shoot_list[shoot_index].y = y;

	vt100_move(shoot_list[shoot_index].x, shoot_list[shoot_index].y);
	serial_putchar(shoot_list[shoot_index].appearance);
	/*
	 do {
	 if (shoot_index >= SHOOT_AMOUNT)
	 shoot_index = 0;
	 shoot_index++;
	 } while (shoot_list[shoot_index].exist == TRUE);*/
	shoot_index++;
	shoot_index %= SHOOT_AMOUNT;
}

void erase_shoot(shoot *_shoot) {
	vt100_move(_shoot->x, _shoot->y);
	serial_putchar(' ');
}

void scroll_shoot() {

	for (int i = 0; i < SHOOT_AMOUNT; i++) {
		if (shoot_list[i].exist == TRUE) {

			erase_shoot(&shoot_list[i]);

			if (shoot_list[i]._object == SPACESHIP)
				shoot_list[i].y--;
			else
				shoot_list[i].y++;

			if (shoot_list[i].y >= VT100_SCREEN_YMAX) {
				shoot_list[i].exist = FALSE;
			} else {
				vt100_move(shoot_list[i].x, shoot_list[i].y);
				serial_putchar(shoot_list[i].appearance);

			}
		}
	}
}

void init_ennemy(ennemy *_ennemy, uint8_t x, uint8_t y) {

	for (int i = 0; i < 3; i++) {
		vt100_move(x + i + 1, y);
		serial_putchar(_ennemy->appearance[i]);
	}
}

void scroll_ennemies(ennemy *_ennemy, uint8_t x, uint8_t y) {

}

void init_shield(uint8_t x, uint8_t y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 7; j++) {
			vt100_move(x + j - 3, y + i);
			serial_putchar(shield_appearance[i][j]);
		}
	}
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
	}
}

void toggle_pause() {
	if (keyboard_flags & KFPAUS) {
		keyboard_flags &= ~KFPAUS;
		for (int i = 0; i < 5; i++) {
			vt100_move(VT100_SCREEN_XMAX + i, VT100_SCREEN_YMAX / 2);
			serial_putchar(' ');
		}
	} else {
		keyboard_flags |= KFPAUS;
		for (int i = 0; i < 5; i++) {
			vt100_move(VT100_SCREEN_XMAX / 2 - 2 + i,
			VT100_SCREEN_YMAX / 2 + 1);
			serial_putchar(pause_message[i]);
		}
	}
}

void init_menu() {
	for (int i; i < 2; i++) {
		for (int j; j < 20; j++) {
			vt100_move(j, i);
			serial_putchar(menu_message[i][j]);
		}

	}
}

void init_level_1() {

	lev_num_flag = LEVEL_1;

	vt100_clear_screen();

	init_shield(10, 20);
	init_shield(40, 20);
	init_shield(70, 20);
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 20; i += 3) {
			init_ennemy(&ennemy_1, i + 30, j);
		}
	}

	for (int i = 0; i < 20; i += 3) {
		init_ennemy(&ennemy_2, i + 30, 4);
	}

	init_spaceship();
}

void init_level_2() {

	lev_num_flag = LEVEL_2;

	vt100_clear_screen();

	init_shield(10, 20);
	init_shield(40, 20);
	init_shield(70, 20);

	init_spaceship();
}

void init_level_3() {

	lev_num_flag = LEVEL_3;

	vt100_clear_screen();

	init_shield(10, 20);
	init_shield(40, 20);
	init_shield(70, 20);

	init_spaceship();
}

#define tim2_irq TIM2_IRQHandler

void tim2_irq(void) {
	//scroll_shoot();
}

#define tim3_irq TIM3_IRQHandler

void tim3_irq(void) {

}
