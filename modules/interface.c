#include "raylib.h"

#include "state.h"
#include "interface.h"
#include "vec2.h"
#include <math.h>

// Assets
Texture spaceship_img;
Texture asteroid_img;
//Sound game_over_snd;


void interface_init() {
	// Αρχικοποίηση του παραθύρου
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);
    InitAudioDevice();


	// Φόρτωση εικόνων και ήχων
	spaceship_img = LoadTextureFromImage(LoadImage("assets/spaceship.png"));
	asteroid_img = LoadTextureFromImage(LoadImage("assets/asteroid.png"));
	
	//game_over_snd = LoadSound("assets/game_over.mp3");
}

void interface_close() {
	CloseAudioDevice();
	CloseWindow();
}

//Μετατροπή καρτισιανές συντεταγμένες σε συντεταγμένες για την raylib
Vector2 cartToRay(State state, Vector2 vec) {

	Vector2 rayCenter = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
	Vector2 spaceshipPos = state_info(state)->spaceship->position;
	
	return (Vector2){rayCenter.x - spaceshipPos.x  + vec.x, rayCenter.y - (spaceshipPos.y * (-1)) + (vec.y * (-1))};


}

//BUG OTAN PAS KATW APLA DEN YPARXEI TIPOTA MALLON EXEI NA KANEI ME TO POS

// Draw game (one frame)
void interface_draw_frame(State state) {
	BeginDrawing();
	ClearBackground(BLACK);

	Rectangle spaceshipRectangle = {0,0,spaceship_img.width,spaceship_img.height};
	Vector2 spaceshipCenter = {spaceship_img.width / 2, spaceship_img.height/2};

	float rotation = atan2(state_info(state)->spaceship->orientation.y, state_info(state)->spaceship->orientation.x *(-1)) * RAD2DEG;

	DrawTexturePro(spaceship_img,spaceshipRectangle,(Rectangle){SCREEN_WIDTH/2, SCREEN_HEIGHT/2, spaceshipRectangle.width,spaceshipRectangle.height},
				   spaceshipCenter,rotation,WHITE);

	Vector2 topleft = {(state_info(state)->spaceship->position.x + (2*SCREEN_WIDTH))*(-1), state_info(state)->spaceship->position.y + (2*SCREEN_HEIGHT)};
	Vector2 bottomright = {state_info(state)->spaceship->position.x + (2*SCREEN_WIDTH), (state_info(state)->spaceship->position.y + (2*SCREEN_HEIGHT))*(-1)};

	List objects = state_objects(state, topleft, bottomright);

	for (ListNode node = list_first(objects); node != LIST_EOF; node = list_next(objects, node)) {
		Object obj = list_node_value(objects, node);
		
		if (obj->type == ASTEROID) {
			Vector2 objPos = cartToRay(state, obj->position);
			DrawCircleLines(objPos.x, objPos.y, obj->size/2, LIGHTGRAY);
		} 
		else {
			Vector2 objPos = cartToRay(state, obj->position);
			DrawCircle(objPos.x, objPos.y, obj->size, GREEN);
		}


	}
	
	DrawText(TextFormat("%i", state_info(state)->score), 10, 10, 40, GRAY);
	DrawFPS(SCREEN_WIDTH - 80, 0);

	if (state_info(state)->paused) {
	DrawText(
		"PRESS [P] TO PLAY AGAIN",
		 GetScreenWidth() / 2 - MeasureText("PRESS [P] TO PLAY AGAIN", 20) / 2,
		 GetScreenHeight() / 2 - 50, 20, GRAY
		);
	}

	// Ηχος, αν είμαστε στο frame που συνέβη το game_over
	// if(state->game_over)
	// 	PlaySound(game_over_snd);

	EndDrawing();
}
