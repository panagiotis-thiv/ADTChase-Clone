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

State level1;
State level2;
State level3;
State level4;
State level5;

State state;
Menu menu;
LvlStats level;
Texture background2;

int selected = 1;

bool play = false;

void menu_update() {

	//Changes which option of the menu is selected 
	if (IsKeyPressed(KEY_DOWN))
		next_menu(menu);

	if (IsKeyPressed(KEY_UP)) 
		prev_menu(menu);

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
	case 4:
		if (IsKeyPressed(KEY_ENTER)) { 
			set_active_menu(menu, 4);
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
	case 4:
		set_max_page(menu, 2);
		break;
	default:
		break;

	}

	if (IsKeyPressed(KEY_LEFT))
		set_page_prev(menu);

	if (IsKeyPressed(KEY_RIGHT))
		set_page_next(menu);

	//TODO


	if (active_menu(menu) == 1 && IsKeyPressed(KEY_ENTER)) {
		
		switch (get_page(menu)) {
		case 1:
			if (level1 == NULL) {
				level = level_init();
				level1 = state_create(level);
			}

			state = level1;
			play = true;
			break;
		case 2:
			// if (level2 == NULL) {
			// 	level = level_init();
			// 	level2 = state_create(level);
			// }

			// state = level2;
			// play = true;
			break;
		case 3:
			// if (level3 == NULL) {
			// 	level = level_init();
			// 	level3 = state_create(level);
			// }

			// state = level3;
			// play = true;
			break;
		case 4:
			// if (level4 == NULL) {
			// 	level = level_init();
			// 	level4 = state_create(level);
			// }

			// state = level4;
			// play = true;
			break;
		case 5:
			// if (level5 == NULL) {
			// 	level = level_init();
			// 	level5 = state_create(level);
			// }

			// state = level5;
			// play = true;
			break;
		default:
			break;
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
		interface_draw_frame(state);
	}
	else
		interface_draw_menu(menu);

}

int main() {
	//state = state_create();
	menu = menu_create(4);
	interface_init();

	// Η κλήση αυτή καλεί συνεχόμενα την update_and_draw μέχρι ο χρήστης να κλείσει το παράθυρο
	start_main_loop(update_and_draw);

	interface_close();

	return 0;
}



