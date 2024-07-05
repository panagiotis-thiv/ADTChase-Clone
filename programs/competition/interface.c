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

void draw_main_menu(Menu menu) {

	DrawText("GAME IS NOT FINISHED DON'T PLAY", SCREEN_WIDTH/2 - 350, SCREEN_HEIGHT/2 + 300, 40, DARKGREEN);

	DrawText("ADTChase", SCREEN_WIDTH/2 - 180, SCREEN_HEIGHT/2 - 300, 70, DARKGREEN);

	if (selected_menu(menu) == 1)
		DrawText("> Play <", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 100, 40, BLUE);
	else 
		DrawText("  Play  ", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 100, 40, BLUE);
	
	if (selected_menu(menu) == 2)
		DrawText("> Stats <", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) - 40, 40, BLUE);
	else
		DrawText("  Stats  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) - 40, 40, BLUE);

	if (selected_menu(menu) == 3)
		DrawText("> Store <", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 20, 40, BLUE);
	else
		DrawText("  Store  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 20, 40, BLUE);
	
	if (selected_menu(menu) == 4) 
		DrawText("> Help <", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 80, 40, BLUE);
	else
		DrawText("  Help  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 80, 40, BLUE);
}

void draw_help_menu(Menu menu) {
	
	DrawText("ADTChase", 10, 10, 30, DARKGREEN);

	DrawText("Help", SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2 - 300, 50, DARKGREEN);

	if (get_page(menu) == 1) {
		DrawText("  1/2 > ", SCREEN_WIDTH - 140, SCREEN_HEIGHT - 50, 40, DARKGREEN);

		DrawText("ADTChase is a simple game based on the\nclassic asteroid game.\nThere are currently 5 levels\nyou have to beat", SCREEN_WIDTH/2 - 400 , SCREEN_HEIGHT/2 - 200, 40, BLUE);
		DrawText("To beat each level you have to destroy\nthe \"core\" of that level, which is a uni-\nque asteroid. It appears after a while\nand it looks different.", SCREEN_WIDTH/2 - 400 , SCREEN_HEIGHT/2 + 50, 40, BLUE);
	}

	if (get_page(menu) == 2) {
		DrawText("< 2/2   ", SCREEN_WIDTH - 140, SCREEN_HEIGHT - 50, 40, DARKGREEN);

		DrawText("To destroy each core you will need to\nupgrade your stats (damage, type of\ngun) which you can purchase through\nthe store using coins.", SCREEN_WIDTH/2 - 400 , SCREEN_HEIGHT/2 - 200, 40, BLUE);
		DrawText("To get coins you will need to destroy\nasteroids. But be careful not to collide\nwith them or you will lose coins!", SCREEN_WIDTH/2 - 400 , SCREEN_HEIGHT/2 + 50, 40, BLUE);
	}

	DrawText("Press [ALT] to go back.", 10, SCREEN_HEIGHT - 40, 20, DARKGREEN);

}


void draw_level_menu(Menu menu) {

	DrawText("ADTChase", SCREEN_WIDTH/2 - 180, SCREEN_HEIGHT/2 - 300, 70, DARKGREEN);

	//Level 1
	DrawCircleLines(200, 300, 55, GRAY);
	DrawCircleLines(200, 300, 56, GRAY);
	DrawCircleLines(200, 300, 57, GRAY);
	DrawCircleLines(200, 300, 58, GRAY);

	if (get_page(menu) == 1)
		DrawText("> Level 1 <", 150, 290, 20, WHITE);
	else
		DrawText("  Level 1  ", 150, 290, 20, WHITE);

	//Line 1->2
	DrawLine(259, 299, 292, 299, LIGHTGRAY);
	DrawLine(259, 300, 292, 300, LIGHTGRAY);
	DrawLine(259, 301, 292, 301, LIGHTGRAY);
	DrawLine(259, 302, 292, 302, LIGHTGRAY);
	
	//Level 2
	DrawCircleLines(350, 300, 55, GRAY);
	DrawCircleLines(350, 300, 56, GRAY);
	DrawCircleLines(350, 300, 57, GRAY);
	DrawCircleLines(350, 300, 58, GRAY);

	if (get_page(menu) == 2)
		DrawText("> Level 2 <", 300, 290, 20, WHITE);
	else
		DrawText("  Level 2  ", 300, 290, 20, WHITE);
	
	//Line 2->3
	DrawLine(408, 299, 445, 299, LIGHTGRAY);
	DrawLine(408, 300, 445, 300, LIGHTGRAY);
	DrawLine(408, 301, 445, 301, LIGHTGRAY);
	DrawLine(408, 302, 445, 302, LIGHTGRAY);

	//Level 3
	DrawCircleLines(500, 300, 55, GRAY);
	DrawCircleLines(500, 300, 56, GRAY);
	DrawCircleLines(500, 300, 57, GRAY);
	DrawCircleLines(500, 300, 58, GRAY);

	if (get_page(menu) == 3)
		DrawText("> Level 3 <", 450, 290, 20, WHITE);
	else
		DrawText("  Level 3  ", 450, 290, 20, WHITE);

	//Line 3->4
	DrawLine(558, 299, 592, 299, LIGHTGRAY);
	DrawLine(558, 300, 592, 300, LIGHTGRAY);
	DrawLine(558, 301, 592, 301, LIGHTGRAY);
	DrawLine(558, 302, 592, 302, LIGHTGRAY);


	//Level 4
	DrawCircleLines(650, 300, 55, GRAY);
	DrawCircleLines(650, 300, 56, GRAY);
	DrawCircleLines(650, 300, 57, GRAY);
	DrawCircleLines(650, 300, 58, GRAY);

	if (get_page(menu) == 4)
		DrawText("> Level 4 <", 600, 290, 20, WHITE);
	else
		DrawText("  Level 4  ", 600, 290, 20, WHITE);

	//Line 2.5->5
	DrawLine(425, 300, 424, 418, LIGHTGRAY);
	DrawLine(426, 300, 425, 418, LIGHTGRAY);

	DrawLine(427, 300, 426, 418, LIGHTGRAY);
	DrawLine(428, 300, 427, 418, LIGHTGRAY);

	//Level 5
	DrawCircleLines(425, 480, 55, DARKPURPLE);
	DrawCircleLines(425, 480, 56, DARKPURPLE);
	DrawCircleLines(425, 480, 57, DARKPURPLE);
	DrawCircleLines(425, 480, 58, DARKPURPLE);
	DrawCircleLines(425, 480, 59, DARKPURPLE);
	DrawCircleLines(425, 480, 60, DARKPURPLE);
	DrawCircleLines(425, 480, 61, DARKPURPLE);
	DrawCircleLines(425, 480, 62, DARKPURPLE);
	
	if (get_page(menu) == 5)
		DrawText("> Level 5 <", 375, 470, 20, WHITE);
	else
		DrawText("  Level 5  ", 375, 470, 20, WHITE);

	


	DrawText("Press [ALT] to go back.", 10, SCREEN_HEIGHT - 40, 20, DARKGREEN);
}

void interface_draw_menu(Menu menu) {

	BeginDrawing();
	ClearBackground(BLACK);

	DrawTexturePro(background, (Rectangle){0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, (Rectangle){0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, (Vector2){0,0}, 0, DARKGRAY);

	switch (active_menu(menu)) 	{
	case 0:
		draw_main_menu(menu);
		break;
	case 1:
		draw_level_menu(menu);
		break;
	case 2:
		//draw_stats_menu(menu);
		break;
	case 3:
		//draw_store_menu(menu);
		break;
	case 4:
		draw_help_menu(menu);
		break;

	}
	
	EndDrawing();

}
