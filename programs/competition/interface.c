#include "raylib.h"

#include "state.h"
#include "interface.h"
#include "vec2.h"
#include "menu.h"
#include <math.h>

// Assets
Texture spaceship_img;
Texture background;

Music background_music;


void interface_init() {
	// Αρχικοποίηση του παραθύρου
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);
    InitAudioDevice();


	// Φόρτωση εικόνων και ήχων
	spaceship_img = LoadTextureFromImage(LoadImage("assets/spaceship.png"));
	background = LoadTextureFromImage(LoadImage("assets/background.png"));

	background_music = LoadMusicStream("assets/background_music.mp3");

	PlayMusicStream(background_music);
}

void interface_close() {
	CloseAudioDevice();
	CloseWindow();
}

//Μετατροπή καρτισιανές συντεταγμένες σε συντεταγμένες για την raylib
Vector2 cartToRay(State state, Vector2 vec) {

	Vector2 rayCenter = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
	Vector2 spaceshipPos = state_info(state)->spaceship->position;
	
	return (Vector2){rayCenter.x - spaceshipPos.x + vec.x, rayCenter.y - (spaceshipPos.y * (-1)) + (vec.y * (-1))};

}

// Draw game (one frame)
void interface_draw_frame(State state) {

	UpdateMusicStream(background_music);
	BeginDrawing();
	ClearBackground(BLACK);

	DrawTexturePro(background, (Rectangle){0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, (Rectangle){0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, (Vector2){0,0}, 0, DARKGRAY);

	Rectangle spaceshipRectangle = {0,0,spaceship_img.width,spaceship_img.height};
	Vector2 spaceshipCenter = {spaceship_img.width / 2, spaceship_img.height/2};

	float rotation = atan2(state_info(state)->spaceship->orientation.y, state_info(state)->spaceship->orientation.x *(-1)) * RAD2DEG;

	DrawTexturePro(spaceship_img,spaceshipRectangle,(Rectangle){SCREEN_WIDTH/2, SCREEN_HEIGHT/2, spaceshipRectangle.width,spaceshipRectangle.height},
				   spaceshipCenter,rotation,WHITE);

	Vector2 topleft = {state_info(state)->spaceship->position.x + ((3*SCREEN_WIDTH)*(-1)), state_info(state)->spaceship->position.y + (3*SCREEN_HEIGHT)};
	Vector2 bottomright = {state_info(state)->spaceship->position.x + (3*SCREEN_WIDTH), state_info(state)->spaceship->position.y + ((3*SCREEN_HEIGHT)*(-1))};

	List objects = state_objects(state, topleft, bottomright);

	for (ListNode node = list_first(objects); node != LIST_EOF; node = list_next(objects, node)) {
		Object obj = list_node_value(objects, node);
		
		if (obj->type == ASTEROID) {
			Vector2 objPos = cartToRay(state, obj->position);
			DrawCircleLines(objPos.x, objPos.y, obj->size/2, LIGHTGRAY);
		} 
		else {
			Vector2 objPos = cartToRay(state, obj->position);
			DrawCircle(objPos.x, objPos.y, obj->size, LIME);
		}


	}
	
	DrawText(TextFormat("%d", state_info(state)->score), 10, 10, 40, GRAY);
	DrawFPS(SCREEN_WIDTH - 80, 0);

	if (state_info(state)->paused) {
		PauseMusicStream(background_music);
		DrawText(
			"PRESS [P] TO PLAY AGAIN OR [ALT] TO EXIT",
		 	GetScreenWidth() / 2 - MeasureText("PRESS [P] TO PLAY AGAIN OR [ALT] TO EXIT", 20) / 2,
			 GetScreenHeight() / 2 - 50, 20, GRAY
		);
	} 
	else 
		ResumeMusicStream(background_music);

	EndDrawing();
}

void interface_draw_menu(Menu menu) {

	BeginDrawing();
	ClearBackground(BLACK);

	DrawTexturePro(background, (Rectangle){0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, (Rectangle){0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, (Vector2){0,0}, 0, DARKGRAY);

	DrawText("ADTChase", SCREEN_WIDTH/2 - 180, SCREEN_HEIGHT/2 - 300, 70, DARKGREEN);

	if (selected_menu(menu) == 1) {
		DrawText("> Play <", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 100, 40, BLUE);
		DrawText("  Stats  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) - 40, 40, BLUE);
		DrawText("  Store  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 20, 40, BLUE);
		DrawText("  Help  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 80, 40, BLUE);

		if (IsKeyDown(KEY_ENTER)) {
			set_active_menu(menu, 1);
		}

	}
	else if (selected_menu(menu) == 2) {
		DrawText("  Play  ", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 100, 40, BLUE);
		DrawText("> Stats <", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) - 40, 40, BLUE);
		DrawText("  Store  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 20, 40, BLUE);
		DrawText("  Help  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 80, 40, BLUE);
	}
	else if (selected_menu(menu) == 3) {
		DrawText("  Play  ", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 100, 40, BLUE);
		DrawText("  Stats  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) - 40, 40, BLUE);
		DrawText("> Store <", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 20, 40, BLUE);
		DrawText("  Help  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 80, 40, BLUE);
	}
	else if (selected_menu(menu) == 4) {
		DrawText("  Play  ", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 100, 40, BLUE);
		DrawText("  Stats  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) - 40, 40, BLUE);
		DrawText("  Store  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 20, 40, BLUE);
		DrawText("> Help <", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 80, 40, BLUE);

		//DrawText("ADTChase is a simple game based on the\nclassic asteroid game.\nThere are currently 5 levels\nyou have to beat", SCREEN_WIDTH/2 - 400 , SCREEN_HEIGHT/2 - 100, 40, BLUE);
		//DrawText("To beat each level you have to destroy\nthe \"core\" of that level, which is a uni-\nque asteroid. It appears after a while\nand it looks different.", SCREEN_WIDTH/2 - 400 , SCREEN_HEIGHT/2 - 100, 40, BLUE);
		//DrawText("To destroy each core you will need to\nupgrade your stats (damage, type of\ngun) which you can purchase through\nthe store using coins.", SCREEN_WIDTH/2 - 400 , SCREEN_HEIGHT/2 - 100, 40, BLUE);
		//DrawText("To get coins you will need to destroy\nasteroids. But be careful not to collide\nwith them or you will lose coins!", SCREEN_WIDTH/2 - 400 , SCREEN_HEIGHT/2 - 100, 40, BLUE);

	}
	
	EndDrawing();

}