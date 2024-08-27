//////////////////////////////////////////////////////////////////////////////
//
// Παράδειγμα δημιουργίας ενός παιχνιδιού χρησιμοποιώντας τη βιβλιοθήκη raylib
//
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "raylib.h"
#include "interface.h"
#include "state.h"
#include "menu.h"
#include "level.h"
#include "global_stats.h"

State st_level1;
State st_level2;
State st_level3;
State st_level4;

State eliminate;
State hol; //(hol = higher or lower)
State boss;

Levels level1;
Levels level2;
Levels level3;
Levels level4;

State state;
Menu menu;
Texture background2;

GlobalStats stats;
Gun gamble;

int selected = 1;

float messageTimer = 0.0f;       	// Timer for how long the message has been displayed
const float displayDuration = 0.8f; // Duration to display the message (in seconds)

float coreSpawnTimer = 0.0f;
float coreHideTimer = 0.0f;
float coreTPTimer = 0.0f;

const float coreSpawnDelay = 10.0f;

bool play = false;

bool play_eliminate = false;
bool playing_eliminate = false;
bool reset_elimate = false;

bool play_hol = false;
bool playing_hol = false;
bool reset_hol = false;

void UpdateUserInput() {

	 	int key = GetKeyPressed();

		//Για αυτό το σημείο χρειάστηκε αρκετό ψάξιμο, σίγουρα δεν θα το έβρισκα μόνος μου...
        if (key >= '0' && key <= '9' && gs_user_input(stats)->charCount < 4) {
            gs_user_input(stats)->inputBuffer[gs_user_input(stats)->charCount++] = (char)key;
            gs_user_input(stats)->inputBuffer[gs_user_input(stats)->charCount] = '\0';
        }

        if (IsKeyPressed(KEY_BACKSPACE) && gs_user_input(stats)->charCount > 0) {
            gs_user_input(stats)->inputBuffer[--gs_user_input(stats)->charCount] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER)) { 
            if (gs_user_input(stats)->charCount > 0) {
                gs_user_input(stats)->coinsEntered = atoi(gs_user_input(stats)->inputBuffer);
                gs_user_input(stats)->isEnteringInput = false;
                memset(gs_user_input(stats)->inputBuffer, 0, sizeof(gs_user_input(stats)->inputBuffer));
                gs_user_input(stats)->charCount = 0;

				if (gs_user_input(stats)->coinsEntered > gs_player_info(stats)->coins) 
					gs_user_input(stats)->coinsEntered = 0;
            }
        }

		if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT_ALT)) { 
            gs_user_input(stats)->isEnteringInput = false;
            memset(gs_user_input(stats)->inputBuffer, 0, sizeof(gs_user_input(stats)->inputBuffer));
            gs_user_input(stats)->charCount = 0;
        }
}

void menu_update() {
	
	if (gs_user_input(stats)->isEnteringInput)
		UpdateUserInput();

	//Changes which option of the menu is selected 
	if (selected_menu(menu) != 0) {
		if (IsKeyPressed(KEY_DOWN))
			next_menu(menu);

		if (IsKeyPressed(KEY_UP)) 
			prev_menu(menu);
	}

	//Goes to the previous menu (mostly the main menu)
	if (IsKeyPressed(KEY_LEFT_ALT)) {

		if (active_menu(menu) != 1 || get_page(menu) <= 6) {
			if (!play) {
				set_active_menu(menu, 0);
				set_page(menu, 1);
				set_selected(menu, selected);
				//selected = 1;
			} else {
				set_page(menu, state_info(state)->level_number);
				play = false;
			}
		}
		else {
			gs_guns_info(stats)->selected_gun = gs_guns_info(stats)->prev_gun;
			if (play_eliminate) {
				set_page(menu, state_info(state)->level_number+1);
				playing_eliminate = true;
				play_eliminate = false;
				if (state_info(state)->win && state_info(state)->level_number == 6) {
					gs_player_info(stats)->coins +=	state_info(state)->eliminate_reward;
					gs_user_input(stats)->coinsEntered = 0;
					reset_elimate = true;
					playing_eliminate = false;
				}
			} else if (play_hol) {
				set_page(menu, state_info(state)->level_number+1);
				playing_hol = true;
				play_hol = false;
				if (state_info(state)->win && state_info(state)->level_number == 7) {
					gs_player_info(stats)->coins +=	state_info(state)->hol_reward;
					gs_user_input(stats)->coinsEntered = 0;
					reset_hol = true;
					playing_hol = false;
				}
			} else
				set_page(menu, 6);
		}
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
		set_max_page(menu, 9);
		break;
	case 2:
		set_max_page(menu, 9);

		if (IsKeyPressed(KEY_LEFT)) {
			if (get_page(menu) == 9)
				set_page(menu, 2);
			else
				set_page(menu, 1);
		}

		if (IsKeyPressed(KEY_RIGHT)) {
			if (get_page(menu) == 1 || get_page(menu) == 3 || get_page(menu) == 5)
				set_page(menu, 2);
			else
				set_page(menu, 9);
		}
		

		if (IsKeyPressed(KEY_DOWN))
			set_page(menu, get_page(menu) + 2);

		if (IsKeyPressed(KEY_UP) && get_page(menu) != 2 && get_page(menu) != 9) 
			set_page(menu, get_page(menu) - 2);

		if (get_page(menu) < 1)
			set_page(menu, 1);

		if (get_page(menu) == 7)
			set_page(menu, 5);

		if ((get_page(menu) == 3) && IsKeyPressed(KEY_ENTER)) {
			
			switch (gs_store_info(stats)->spaceship_hp) {
			case 50:
				if (gs_player_info(stats)->coins >= 30) {
					gs_player_info(stats)->coins -= 30;
					gs_store_info(stats)->spaceship_hp = 70;			
					gs_player_info(stats)->spaceship_hp += 20;	
				}
				break;

			case 70:
				
				if (gs_player_info(stats)->coins >= 70) {
					gs_player_info(stats)->coins -= 70;
					gs_store_info(stats)->spaceship_hp = 100;			
					gs_player_info(stats)->spaceship_hp += 30;		
				}
				break;

			case 100:
				
				if (gs_player_info(stats)->coins >= 140) {
					gs_player_info(stats)->coins -= 140;
					gs_store_info(stats)->spaceship_hp = 160;			
					gs_player_info(stats)->spaceship_hp += 60;	

				}
				break;

			case 160:
				if (gs_player_info(stats)->coins >= 190) {
					gs_player_info(stats)->coins -= 190;
					gs_store_info(stats)->spaceship_hp = 250;			
					gs_player_info(stats)->spaceship_hp += 90;	
				}
				break;

			case 250:
				if (gs_player_info(stats)->coins >= 413) {
					gs_player_info(stats)->coins -= 413;
					gs_store_info(stats)->spaceship_hp = 500;			
					gs_player_info(stats)->spaceship_hp += 250;	
				}
				break;

			case 500:
				if (gs_player_info(stats)->coins >= 550) {
					gs_player_info(stats)->coins -= 500;
					gs_store_info(stats)->spaceship_hp = 1000;			
					gs_player_info(stats)->spaceship_hp += 500;	
				}
				break;

			default:
				break;
			}

		}

		if ((get_page(menu) == 5) && IsKeyPressed(KEY_ENTER) && gs_player_info(stats)->spaceship_hp < gs_store_info(stats)->spaceship_hp && gs_player_info(stats)->coins >= 15) {

			gs_player_info(stats)->spaceship_hp += 10;
			gs_player_info(stats)->coins -= 15;

			if (gs_player_info(stats)->spaceship_hp > gs_store_info(stats)->spaceship_hp)
				gs_player_info(stats)->spaceship_hp = gs_store_info(stats)->spaceship_hp;
			
		}

		if ((get_page(menu) == 4) && IsKeyPressed(KEY_ENTER)) {
			
			if (!gs_store_info(stats)->rifle) {
				
				if (gs_player_info(stats)->coins >= 100) {
					gs_player_info(stats)->coins -= 100;
					gs_store_info(stats)->rifle = true;
					gs_store_info(stats)->slot1 = gs_guns_info(stats)->selected_gun;
					gs_guns_info(stats)->selected_gun = gs_guns_info(stats)->rifle;
				}
			}
			else {
				gs_guns_info(stats)->prev_gun = gs_guns_info(stats)->selected_gun;
				gs_guns_info(stats)->selected_gun = gs_store_info(stats)->slot1;
				gs_store_info(stats)->slot1 = gs_guns_info(stats)->prev_gun;
			}
		}	

		if ((get_page(menu) == 6) && IsKeyPressed(KEY_ENTER)) {
			
			if (!gs_store_info(stats)->sniper) {

				if (gs_player_info(stats)->coins >= 287) {
					gs_player_info(stats)->coins -= 287;
					gs_store_info(stats)->sniper = true;
					gs_store_info(stats)->slot2 = gs_guns_info(stats)->selected_gun;
					gs_guns_info(stats)->selected_gun = gs_guns_info(stats)->sniper;
				}

			}
			else {
				gs_guns_info(stats)->prev_gun = gs_guns_info(stats)->selected_gun;
				gs_guns_info(stats)->selected_gun = gs_store_info(stats)->slot2;
				gs_store_info(stats)->slot2 = gs_guns_info(stats)->prev_gun;
			}	
		}

		if ((get_page(menu) == 8) && IsKeyPressed(KEY_ENTER)) {
			

			if (gs_guns_info(stats)->selected_gun == gs_guns_info(stats)->pistol && gs_player_info(stats)->coins >= 10 && gs_guns_info(stats)->pistol->bullets < 50) {
				gs_player_info(stats)->coins -= 10;
				gs_guns_info(stats)->pistol->bullets += 8;

				if (gs_guns_info(stats)->pistol->bullets > 50)
					gs_guns_info(stats)->pistol->bullets = 50;
			}

			if (gs_guns_info(stats)->selected_gun == gs_guns_info(stats)->rifle && gs_player_info(stats)->coins >= 20 && gs_guns_info(stats)->rifle->bullets < 100) {
				gs_player_info(stats)->coins -= 20;
				gs_guns_info(stats)->rifle->bullets += 30;

				if (gs_guns_info(stats)->rifle->bullets > 100)
					gs_guns_info(stats)->rifle->bullets = 100;
			}

			if (gs_guns_info(stats)->selected_gun == gs_guns_info(stats)->sniper && gs_player_info(stats)->coins >= 35 && gs_guns_info(stats)->sniper->bullets < 25) {
				gs_player_info(stats)->coins -= 35;
				gs_guns_info(stats)->sniper->bullets += 8;

				if (gs_guns_info(stats)->sniper->bullets > 25)
					gs_guns_info(stats)->sniper->bullets = 25;
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

		if (active_menu(menu) != 1 || get_page(menu) != 7)
			if (IsKeyPressed(KEY_LEFT))
				set_page_prev(menu);


		if (active_menu(menu) != 1 || get_page(menu) != 6)
			if (IsKeyPressed(KEY_RIGHT))
				set_page_next(menu);

	}

	if (active_menu(menu) == 1 && IsKeyPressed(KEY_ENTER)) {
		
		if (!play) {
			switch (get_page(menu)) {
			case 1:			
				state = st_level1;
				play = true;
				break;
			case 2:
				if (gs_levels_info(stats)->level2 == 1 || gs_levels_info(stats)->level2 == 2) {

					if (st_level2 == NULL) {
						level2 = level_create(100, 25, 2, 1.35, 150, 250, 5);
						st_level2 = state_create(level2, stats);
					}

					state = st_level2;
					play = true;
				}
				break;
			case 3:
				if (gs_levels_info(stats)->level3 == 1 || gs_levels_info(stats)->level3 == 2) {
					
					if (st_level3 == NULL) {
						level3 = level_create(500, 10, 3, 0.5, 20, 425, 4);
						st_level3 = state_create(level3, stats);
					}

					state = st_level3;
					play = true;
				}
				break;
			case 4:
				if (gs_levels_info(stats)->level4 == 1 || gs_levels_info(stats)->level4 == 2) {

					if (st_level4 == NULL) {
						level4 = level_create(19, 5, 4, 2, 150, 500, 2);
						st_level4 = state_create(level4, stats);
					}

					state = st_level4;
					play = true;
				}
				break;
			case 5:
				if (gs_levels_info(stats)->level4 == 1) {
					// state = boss;
					// play = true;
				}
				break;
			//Γιατί όχι το παρακάτω case εδώ αλλά στο τέλος αυτής της if;
			//case 6:
			// 	if (gs_levels_info(stats)->level2 == 2)
			//	break;
			}
		}	

		if (get_page(menu) == 7 && gs_user_input(stats)->coinsEntered > 0 && !play_eliminate && !playing_hol) {
			if (gamble == NULL) 
				gamble = gs_gun_create(999, 30, 100);
			
			if (gs_guns_info(stats)->selected_gun != gamble) {
				gs_guns_info(stats)->prev_gun = gs_guns_info(stats)->selected_gun;
				gs_guns_info(stats)->selected_gun = gamble;
			}
			
			if (eliminate == NULL) 
				eliminate = state_create_eliminate(stats);
			
			if (reset_elimate && !playing_eliminate) {
				state_destroy(eliminate);
				eliminate = state_create_eliminate(stats);
				reset_elimate = false;
			}

			if (!playing_eliminate) 
				gs_player_info(stats)->coins -= gs_user_input(stats)->coinsEntered;

			state = eliminate;
			play_eliminate = true;
		}

		if (get_page(menu) == 8 && (!playing_eliminate && !playing_hol))
			gs_user_input(stats)->isEnteringInput = true;

		if (get_page(menu) == 9 && gs_user_input(stats)->coinsEntered > 0 && !play_hol && !playing_eliminate) {
			if (gamble == NULL) 
				gamble = gs_gun_create(999, 30, 100);
			
			if (gs_guns_info(stats)->selected_gun != gamble) {
				gs_guns_info(stats)->prev_gun = gs_guns_info(stats)->selected_gun;
				gs_guns_info(stats)->selected_gun = gamble;
			}
			
			if (hol == NULL) 
				hol = state_create_hol(stats);
			
			if (reset_hol && !playing_hol) {
				state_destroy(hol);
				hol = state_create_hol(stats);
				reset_hol = false;
			}

			if (!playing_hol) 
				gs_player_info(stats)->coins -= gs_user_input(stats)->coinsEntered;
			
			state = hol;
			play_hol = true;
		}

		//Γιατί κάθε φορά που πάταγα enter, και τα προηγούμενα ήταν μετά από αυτό το case, ήταν σαν πάταγα enter 2 φορές συνεχόμενα
		if (get_page(menu) == 6 && gs_levels_info(stats)->level2 == 2 && !play) 
			set_page(menu, 7);
	}
}

void check_win() {

	if (state_info(state)->paused && state_info(state)->win) {

		switch (state_info(state)->level_number)
		{
		case 1:
			gs_levels_info(stats)->level1++;
			gs_levels_info(stats)->level2++;
			break;
		case 2:
			gs_levels_info(stats)->level2++;
			gs_levels_info(stats)->level3++;
			break;
		case 3:
			gs_levels_info(stats)->level3++;
			gs_levels_info(stats)->level4++;
			break;
		case 4:
			gs_levels_info(stats)->level4++;
			gs_levels_info(stats)->level5++;
			break;
		case 5:
			gs_levels_info(stats)->level5++;
			break;
		}
		
		state_info(state)->win = false;
	}

}


void check_coreTime() {

	if (!state_info(state)->win && (!state_info(state)->paused || IsKeyDown(KEY_N))) {

		if (!state_info(state)->core)
			coreSpawnTimer += GetFrameTime();
		
		if (coreSpawnTimer > coreSpawnDelay) {
			state_info(state)->spawn_core = true;
			coreSpawnTimer = 0.0f;
		}
		
		if (state_info(state)->core) {

			if (!state_info(state)->isCoreHidden)
				coreHideTimer += GetFrameTime();
			
			if (state_info(state)->isCoreHidden)
				coreTPTimer += GetFrameTime();
		}

		if (state_info(state)->isCoreHidden) 
			coreHideTimer = 0.0f;

		if (coreHideTimer > 6.0f) 
			state_info(state)->hide_core = true;	

		if (coreTPTimer > 4.0f) {
			state_info(state)->tp_core = true;
			coreTPTimer = 0.0f;
		}
	}
	else if (!state_info(state)->paused || IsKeyDown(KEY_N)) {
		coreSpawnTimer = 0.0f;
		coreHideTimer = 0.0f;
		coreTPTimer = 0.0f;
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
		check_coreTime();
		check_win();
		interface_draw_frame(state, stats);
	} else if (play_eliminate) {
		state_update_eliminate(state, &keys, menu);	
		interface_draw_frame(state, stats);
	} else if (play_hol) {
		state_update_hol(state, &keys, menu);
		interface_draw_frame(state, stats);
	}
 	else
		interface_draw_menu(menu, state, stats);
}

int main() {
	level1 = level_create(50, 5, 1, 1, 50, 100, 3);
	stats = gs_create();
	st_level1 = state_create(level1, stats);
	state = st_level1;

	menu = menu_create(3);
	interface_init();
	
	//For testing
	// gs_levels_info(stats)->level2 = 2;
	// gs_levels_info(stats)->level3 = 1;
	// gs_levels_info(stats)->level4 = 1;

	// Η κλήση αυτή καλεί συνεχόμενα την update_and_draw μέχρι ο χρήστης να κλείσει το παράθυρο
	start_main_loop(update_and_draw);

	interface_close();

	return 0;
}



