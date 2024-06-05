//////////////////////////////////////////////////////////////////////////////
//
// Παράδειγμα δημιουργίας ενός παιχνιδιού χρησιμοποιώντας τη βιβλιοθήκη raylib
//
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "raylib.h"

#include "interface.h"
#include "state.h"
#include "menu.h"

State state;
Menu menu;
Texture background2;

void update_and_draw() {

	struct key_state keys = { IsKeyDown(KEY_UP) || IsKeyDown(KEY_W), IsKeyDown(KEY_LEFT)|| IsKeyDown(KEY_A), IsKeyDown(KEY_RIGHT)|| IsKeyDown(KEY_D), 
							  IsKeyDown(KEY_ENTER), IsKeyDown(KEY_SPACE), IsKeyDown(KEY_N), 
							  IsKeyPressed(KEY_P), IsKeyPressed(KEY_LEFT_ALT)};

	if (IsKeyPressed(KEY_DOWN)) { 
		next_menu(menu);
		
	}

	if (IsKeyPressed(KEY_UP)) 
		prev_menu(menu);

	if (active_menu(menu) == 1)  {
		state_update(state, &keys, menu);
		interface_draw_frame(state);
	}
	else {

		if (state_info(state)->paused == true) {
			state_info(state)->paused = false;
		}

		interface_draw_menu(menu);
		
	}

}

int main() {
	state = state_create();
	menu = menu_create();
	interface_init();

	// Η κλήση αυτή καλεί συνεχόμενα την update_and_draw μέχρι ο χρήστης να κλείσει το παράθυρο
	start_main_loop(update_and_draw);

	interface_close();

	return 0;
}



