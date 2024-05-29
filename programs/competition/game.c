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

	if (IsKeyDown(KEY_DOWN)) 
		next_menu(menu);

	if (IsKeyDown(KEY_UP)) 
		prev_menu(menu);

	if (active_menu(menu) == 1)  {
		state_update(state, &keys, menu);
		interface_draw_frame(state);
	}
	else {

		if (state_info(state)->paused == true) {
			state_info(state)->paused = false;
		}

		BeginDrawing();
		ClearBackground(BLACK);

		background2 = LoadTextureFromImage(LoadImage("assets/background.png"));
		DrawTexturePro(background2, (Rectangle){0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, (Rectangle){0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, (Vector2){0,0}, 0, DARKGRAY);

		if (selected_menu(menu) == 1) {
			DrawText("> Play <", 50, 50, 40, BLUE);
			DrawText("  Help  ", 50, 100, 40, BLUE);

			if (IsKeyDown(KEY_ENTER)) {
				set_active_menu(menu, 1);
			}

		}
		else if (selected_menu(menu) == 2) {
			DrawText("  Play  ", 50, 50, 40, BLUE);
			DrawText("> Help <", 50, 100, 40, BLUE);
		}
		
		EndDrawing();
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



