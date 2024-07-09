//////////////////////////////////////////////////////////////////////////////
//
// Παράδειγμα δημιουργίας ενός παιχνιδιού χρησιμοποιώντας τη βιβλιοθήκη raylib
//
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib.h"
#include "interface.h"
#include "state.h"
#include "menu.h"
#include "level.h"
#include "store.h"

State level1;
State level2;
State level3;
State level4;
State level5;

State state;
Menu menu;
LvlStats level;
Store store;

Texture background2;

int selected = 1;

float messageTimer = 0.0f;       	// Timer for how long the message has been displayed
const float displayDuration = 0.8f; // Duration to display the message (in seconds)

bool play = false;

void menu_update() {

	//Changes which option of the menu is selected 
	if (selected_menu(menu) != 0) {
		if (IsKeyPressed(KEY_DOWN))
			next_menu(menu);

		if (IsKeyPressed(KEY_UP)) 
			prev_menu(menu);
	}

	//Goes to the main menu
	if (IsKeyPressed(KEY_LEFT_ALT)) {
		set_active_menu(menu, 0);
		set_page(menu, 1);
		set_selected(menu, selected);
		selected = 1;
		play = false;
	}

	//Sets the selected menu as the active one if player presses enter.
	switch (selected_menu(menu)) {
	case 1:
		if (IsKeyPressed(KEY_ENTER)) {
			set_active_menu(menu, 1);
			selected = selected_menu(menu);
			set_selected(menu, 0);
			return;
		}
	case 2:
		if (IsKeyPressed(KEY_ENTER)) {
			set_active_menu(menu, 2);
			selected = selected_menu(menu);
			set_selected(menu, 0);
			return;
		}
	case 3:
		if (IsKeyPressed(KEY_ENTER)) {
			set_active_menu(menu, 3);
			selected = selected_menu(menu);
			set_selected(menu, 0);
			return;
		}
	default:
		break;
	}


	//Page settings. Page is the settings of each menu. For example in the help menu you have two "pages". 
	//To not confuse the menus each time you change the amount of "pages" available.

	switch (active_menu(menu)) {
	case 1:
		set_max_page(menu, 5);
		break;
	case 2:
		set_max_page(menu, 8);

		if (IsKeyPressed(KEY_LEFT))
			set_page(menu, 1);

		if (IsKeyPressed(KEY_RIGHT))
			set_page(menu, 2);

		if (IsKeyPressed(KEY_DOWN))
			set_page(menu, get_page(menu) + 2);

		if (IsKeyPressed(KEY_UP) && get_page(menu) != 2) 
			set_page(menu, get_page(menu) - 2);

		if (get_page(menu) < 1)
			set_page(menu, 1);

		if (get_page(menu) == 7)
			set_page(menu, 5);

		if ((get_page(menu) == 3) && IsKeyPressed(KEY_ENTER)) {
			
			switch (store_info(store, spaceshipHP)) {
			case 50:
				if (state_info(state)->coins >= 30) {
					state_info(state)->coins = state_info(state)->coins - 30;
					store_update(store, 70, -1, -1, -1, -1);			
					state_info(state)->spaceship->health = state_info(state)->spaceship->health + 20;	
				}
				break;

			case 70:
				
				if (state_info(state)->coins >= 70) {
					state_info(state)->coins = state_info(state)->coins - 70;
					store_update(store, 100, -1, -1, -1, -1);				
					state_info(state)->spaceship->health = state_info(state)->spaceship->health + 30;		
				}
				break;

			case 100:
				
				if (state_info(state)->coins >= 140) {
					state_info(state)->coins = state_info(state)->coins - 140;
					store_update(store, 160, -1, -1, -1, -1);				
					state_info(state)->spaceship->health = state_info(state)->spaceship->health + 60;	

				}
				break;

			case 160:
				if (state_info(state)->coins >= 190) {
					state_info(state)->coins = state_info(state)->coins - 190;
					store_update(store, 250, -1, -1, -1, -1);
					state_info(state)->spaceship->health = state_info(state)->spaceship->health + 90;	
				}
				break;

			case 250:
				if (state_info(state)->coins >= 413) {
					state_info(state)->coins = state_info(state)->coins - 413;
					store_update(store, 500, -1, -1, -1, -1);
					state_info(state)->spaceship->health = state_info(state)->spaceship->health + 250;	
				}
				break;

			case 500:
				if (state_info(state)->coins >= 550) {
					state_info(state)->coins = state_info(state)->coins - 550;
					store_update(store, 1000, -1, -1, -1, -1);
					state_info(state)->spaceship->health = state_info(state)->spaceship->health + 500;	
				}
				break;

			default:
				break;
			}

		}

		if ((get_page(menu) == 5) && IsKeyPressed(KEY_ENTER) && state_info(state)->spaceship->health < store_info(store, spaceshipHP) && state_info(state)->coins >= 15) {

			state_info(state)->spaceship->health = state_info(state)->spaceship->health + 10;
			state_info(state)->coins = state_info(state)->coins - 15;

			if (state_info(state)->spaceship->health > store_info(store, spaceshipHP))
				state_info(state)->spaceship->health = store_info(store, spaceshipHP);
			
		}

		if ((get_page(menu) == 4) && IsKeyPressed(KEY_ENTER)) {
			
			if (store_info(store, rifle) == 0) {
				
				if (state_info(state)->coins >= 100) {
					store_prev_gun(store, store_info(store, selected_gun));
					store_update(store, -1, 1, 100, 5, 9);
					state_info(state)->coins = state_info(state)->coins - 100;
					store_update_rifle(store, true);
				}
			}
			else {
				switch (store_get_prev_gun(store)) {
					case 0:
						store_prev_gun(store, store_info(store, selected_gun));
						store_update(store, -1, 0, 50, 70, 15);
						break;
					case 1:
						store_prev_gun(store, store_info(store, selected_gun));
						store_update(store, -1, 1, 100, 5, 9);
						break;
					case 2:
						store_prev_gun(store, store_info(store, selected_gun));
						store_update(store, -1, 2, 25, 110, 50);
						break;
					default:
						break;
				}
			}
		}	

		if ((get_page(menu) == 6) && IsKeyPressed(KEY_ENTER)) {
			
			if (store_info(store, shotgun) == 0) {

				if (state_info(state)->coins >= 287) {
					store_prev_gun(store, store_info(store, selected_gun));
					store_update(store, -1, 2, 25, 115, 50);
					state_info(state)->coins = state_info(state)->coins - 287;
					store_update_shotgun(store, true);
				}

			}
			else {
				switch (store_get_second_prev_gun(store)) {
				case 0:
					store_update(store, -1, 0, 50, 70, 15);
					break;
				case 1:
					store_update(store, -1, 1, 100, 5, 9);
					break;
				case 2:
					store_update(store, -1, 2, 25, 110, 50);
					break;
				default:
					break;
				}
			}	
		}

		if ((get_page(menu) == 8) && IsKeyPressed(KEY_ENTER)) {

			switch (store_info(store, selected_gun)) {
			case 0:
				
				if (state_info(state)->coins >= 10 && state_info(state)->spaceship->pistol_bullets < 50) {

					state_info(state)->coins = state_info(state)->coins - 10;
					state_info(state)->spaceship->pistol_bullets = state_info(state)->spaceship->pistol_bullets + 8;

					if (state_info(state)->spaceship->pistol_bullets > 50)
						state_info(state)->spaceship->pistol_bullets = 50;

				}

				break;
			case 1:

				if (state_info(state)->coins >= 20 && state_info(state)->spaceship->rifle_bullets < 100) {

					state_info(state)->coins = state_info(state)->coins - 20;
					state_info(state)->spaceship->rifle_bullets = state_info(state)->spaceship->rifle_bullets + 30;
					
					if (state_info(state)->spaceship->rifle_bullets > 100)
						state_info(state)->spaceship->rifle_bullets = 100;

				}
				break;
			case 2:
				if (state_info(state)->coins >= 35 && state_info(state)->spaceship->shotgun_bullets < 25) {

					state_info(state)->coins = state_info(state)->coins - 35;
					state_info(state)->spaceship->shotgun_bullets = state_info(state)->spaceship->shotgun_bullets + 8;
					
					if (state_info(state)->spaceship->shotgun_bullets > 25)
						state_info(state)->spaceship->shotgun_bullets = 25;

				}
				break;
			default:
				break;
			}

		}
			
		break;
	case 3:
		set_max_page(menu, 2);
		break;
	default:
		break;

	}
	
	if (active_menu(menu) != 2) {
		if (IsKeyPressed(KEY_LEFT))
			set_page_prev(menu);

		if (IsKeyPressed(KEY_RIGHT))
			set_page_next(menu);
	}

	if (active_menu(menu) == 1 && IsKeyPressed(KEY_ENTER)) {
		
		switch (get_page(menu)) {
		case 1:			
			state = level1;
			play = true;
			break;
		case 2:
			// if (level2 == NULL) {
			// 	level = level_init();
			// 	level2 = state_create(level, store);
			// }

			// state = level2;
			// play = true;
			break;
		case 3:
			// if (level3 == NULL) {
			// 	level = level_init();
			// 	level3 = state_create(level, store);
			// }

			// state = level3;
			// play = true;
			break;
		case 4:
			// if (level4 == NULL) {
			// 	level = level_init();
			// 	level4 = state_create(level, store);
			// }

			// state = level4;
			// play = true;
			break;
		case 5:
			// if (level5 == NULL) {
			// 	level = level_init();
			// 	level5 = state_create(level, store);
			// }

			// state = level5;
			// play = true;
			break;
		default:
			break;
		}

	}

}
	
void check_drawCoinsReward() {

	if (state_info(state)->drawCoinsReward) {
		messageTimer += GetFrameTime(); 	

		if (messageTimer > displayDuration) {
			state_info(state)->drawCoinsReward = false;
			messageTimer = 0.0f;
		}
	}
}

void update_and_draw() {

	struct key_state keys = { IsKeyDown(KEY_UP) || IsKeyDown(KEY_W), IsKeyDown(KEY_LEFT)|| IsKeyDown(KEY_A), IsKeyDown(KEY_RIGHT)|| IsKeyDown(KEY_D), 
							  IsKeyDown(KEY_ENTER), IsKeyDown(KEY_SPACE), IsKeyDown(KEY_N), 
							  IsKeyPressed(KEY_P), IsKeyPressed(KEY_LEFT_ALT)};

	menu_update();

	if (play) {
		state_update(state, &keys, menu);
		check_drawCoinsReward();
		interface_draw_frame(state, store);
	}
	else
		interface_draw_menu(menu, state, store);
}

int main() {
	level = level_init();
	store = store_init();
	level1 = state_create(level, store);
	state = level1;

	menu = menu_create(3);
	interface_init();

	// Η κλήση αυτή καλεί συνεχόμενα την update_and_draw μέχρι ο χρήστης να κλείσει το παράθυρο
	start_main_loop(update_and_draw);

	interface_close();

	return 0;
}



