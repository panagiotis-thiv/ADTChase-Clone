#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "state.h"
#include "interface.h"
#include "vec2.h"
#include "menu.h"
#include "store.h"


// Assets
Texture spaceship_img;
Texture background;
Texture coin;
Texture heart;

Music background_music;


void interface_init() {
	// Αρχικοποίηση του παραθύρου
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);
    InitAudioDevice();


	// Φόρτωση εικόνων και ήχων
	spaceship_img = LoadTextureFromImage(LoadImage("assets/spaceship.png"));
	background = LoadTextureFromImage(LoadImage("assets/background.png"));
	coin = LoadTextureFromImage(LoadImage("assets/coin.png"));
	heart = LoadTextureFromImage(LoadImage("assets/heart.png"));

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
void interface_draw_frame(State state, Store store) {

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
	
	Rectangle coinRectangle = {0,0,coin.width,coin.height};
	Vector2 coinCenter = {coin.width / 2, coin.height/2};

	if (state_info(state)->coins >= 100)
		DrawTexturePro(coin, coinRectangle, (Rectangle){95, 28, coinRectangle.width,coinRectangle.height}, coinCenter, 0, WHITE);
	else if (state_info(state)->coins >= 20)
		DrawTexturePro(coin, coinRectangle, (Rectangle){80, 28, coinRectangle.width,coinRectangle.height}, coinCenter, 0, WHITE);
	else
		DrawTexturePro(coin, coinRectangle, (Rectangle){70, 28, coinRectangle.width,coinRectangle.height}, coinCenter, 0, WHITE);

	DrawText(TextFormat("%d", state_info(state)->coins), 10, 10, 40, GRAY);
	DrawFPS(SCREEN_WIDTH - 80, 0);

	if (state_info(state)->drawCoinsReward) {
		Vector2 coinPos = cartToRay(state, state_info(state)->coinsPos);
		DrawText(TextFormat("+ %d", state_info(state)->coinsReward), coinPos.x, coinPos.y, 20, GOLD);
	}

	Rectangle heartRectangle = {0,0,heart.width,heart.height};
	Vector2 heartCenter = {heart.width / 2, heart.height/2};	

	DrawTexturePro(heart, heartRectangle, (Rectangle){SCREEN_WIDTH - 20, SCREEN_HEIGHT - 20, heartRectangle.width,heartRectangle.height}, heartCenter, 0, WHITE);

	if (store_info(store, spaceshipHP) >= 100)
		DrawText(TextFormat("%d / %d", state_info(state)->spaceship->health, store_info(store, spaceshipHP)), SCREEN_WIDTH - 138, SCREEN_HEIGHT - 30, 20, GRAY);
	else if (store_info(store, spaceshipHP) >= 500)
		DrawText(TextFormat("%d / %d", state_info(state)->spaceship->health, store_info(store, spaceshipHP)), SCREEN_WIDTH - 155, SCREEN_HEIGHT - 30, 20, GRAY);
	else
		DrawText(TextFormat("%d / %d", state_info(state)->spaceship->health, store_info(store, spaceshipHP)), SCREEN_WIDTH - 120, SCREEN_HEIGHT - 30, 20, GRAY);

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
		DrawText("> Store <", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) - 40, 40, BLUE);
	else
		DrawText("  Store  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) - 40, 40, BLUE);
	
	if (selected_menu(menu) == 3) 
		DrawText("> Help <", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 20, 40, BLUE);
	else
		DrawText("  Help  ", SCREEN_WIDTH/2 - 100, ((SCREEN_HEIGHT/2)) + 20, 40, BLUE);

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
	
	//Level 2
	DrawCircleLines(350, 300, 55, GRAY);
	DrawCircleLines(350, 300, 56, GRAY);
	DrawCircleLines(350, 300, 57, GRAY);
	DrawCircleLines(350, 300, 58, GRAY);

	if (get_page(menu) == 2)
		DrawText("> Level 2 <", 300, 290, 20, WHITE);
	else
		DrawText("  Level 2  ", 300, 290, 20, WHITE);

	//Level 3
	DrawCircleLines(500, 300, 55, GRAY);
	DrawCircleLines(500, 300, 56, GRAY);
	DrawCircleLines(500, 300, 57, GRAY);
	DrawCircleLines(500, 300, 58, GRAY);

	if (get_page(menu) == 3)
		DrawText("> Level 3 <", 450, 290, 20, WHITE);
	else
		DrawText("  Level 3  ", 450, 290, 20, WHITE);

	//Level 4
	DrawCircleLines(650, 300, 55, GRAY);
	DrawCircleLines(650, 300, 56, GRAY);
	DrawCircleLines(650, 300, 57, GRAY);
	DrawCircleLines(650, 300, 58, GRAY);

	if (get_page(menu) == 4)
		DrawText("> Level 4 <", 600, 290, 20, WHITE);
	else
		DrawText("  Level 4  ", 600, 290, 20, WHITE);

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

	//Lines

	//Line 1->2
	DrawLine(259, 299, 292, 299, LIGHTGRAY);
	DrawLine(259, 300, 292, 300, LIGHTGRAY);
	DrawLine(259, 301, 292, 301, LIGHTGRAY);

	//Line 2->3
	DrawLine(408, 299, 445, 299, LIGHTGRAY);
	DrawLine(408, 300, 445, 300, LIGHTGRAY);
	DrawLine(408, 301, 445, 301, LIGHTGRAY);

	//Line 3->4
	DrawLine(558, 299, 592, 299, LIGHTGRAY);
	DrawLine(558, 300, 592, 300, LIGHTGRAY);
	DrawLine(558, 301, 592, 301, LIGHTGRAY);

	//Line above level 5
	DrawLine(425, 385, 424, 418, LIGHTGRAY);
	DrawLine(426, 385, 425, 418, LIGHTGRAY);
	DrawLine(427, 385, 426, 418, LIGHTGRAY);

	//Line below level 1 & 2
	DrawLine(200, 384, 425, 384, LIGHTGRAY);
	DrawLine(200, 385, 425, 385, LIGHTGRAY);
	DrawLine(200, 386, 425, 386, LIGHTGRAY);

	//Line below 1 connecting to line below 1 & 2
	DrawLine(199, 358, 199, 385, LIGHTGRAY);
	DrawLine(200, 358, 200, 385, LIGHTGRAY);
	DrawLine(201, 358, 201, 385, LIGHTGRAY);

	//Line below 2 connecting to line below 1 & 2
	DrawLine(349, 358, 349, 385, LIGHTGRAY);
	DrawLine(350, 358, 350, 385, LIGHTGRAY);
	DrawLine(351, 358, 351, 385, LIGHTGRAY);

	//Line below level 3 & 4
	DrawLine(425, 384, 650, 384, LIGHTGRAY);
	DrawLine(425, 385, 650, 385, LIGHTGRAY);
	DrawLine(425, 386, 650, 386, LIGHTGRAY);

	//Line below 3 connecting to line below 3 & 4
	DrawLine(499, 358, 499, 385, LIGHTGRAY);
	DrawLine(500, 358, 500, 385, LIGHTGRAY);
	DrawLine(501, 358, 501, 385, LIGHTGRAY);

	//Line below 4 connecting to line below 3 & 4
	DrawLine(649, 358, 649, 385, LIGHTGRAY);
	DrawLine(650, 358, 650, 385, LIGHTGRAY);
	DrawLine(651, 358, 651, 385, LIGHTGRAY);

	DrawText("Press [ALT] to go back.", 10, SCREEN_HEIGHT - 40, 20, DARKGREEN);
}

void draw_store_menu(Menu menu, State state, Store store) {
	DrawText("ADTChase", 10, 10, 30, DARKGREEN);

	DrawText("Store", SCREEN_WIDTH/2 - 80, SCREEN_HEIGHT/2 - 300, 50, DARKGREEN);
	
	Rectangle coinRectangle = {0,0,coin.width,coin.height};
	Vector2 coinCenter = {coin.width / 2, coin.height/2};

	if (state_info(state)->coins >= 100) {
		DrawTexturePro(coin, coinRectangle, (Rectangle){870, 28, coinRectangle.width,coinRectangle.height}, coinCenter, 0, WHITE);
		DrawText(TextFormat("%d", state_info(state)->coins), 780, 10, 40, GRAY);
	}
	else if (state_info(state)->coins >= 20) {
		DrawTexturePro(coin, coinRectangle, (Rectangle){870, 28, coinRectangle.width,coinRectangle.height}, coinCenter, 0, WHITE);
		DrawText(TextFormat("%d", state_info(state)->coins), 800, 10, 40, GRAY);
	}
	else {
		DrawText(TextFormat("%d", state_info(state)->coins), 820, 10, 40, GRAY);
		DrawTexturePro(coin, coinRectangle, (Rectangle){870, 28, coinRectangle.width,coinRectangle.height}, coinCenter, 0, WHITE);
	}
	
	if (get_page(menu) % 2 == 1) {

		DrawText("Spaceship Upgrades", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 200, 30, BLUE);
		DrawText("> > Gun Upgrades", SCREEN_WIDTH/2 + 180, SCREEN_HEIGHT/2 - 195, 25, GRAY);

		switch (store_info(store, spaceshipHP)) {
		case 50:
			DrawText("--> ", SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 50, 25, SKYBLUE);
			DrawText("50HP > > 70HP | ", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 50, 25, ORANGE);
			DrawText("-30 Coins", SCREEN_WIDTH/2 + 50, SCREEN_HEIGHT/2 - 50, 25, MAROON);
			break;

		case 70:
			DrawText("--> ", SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 50, 25, SKYBLUE);
			DrawText("70HP > > 100HP | ", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 50, 25, ORANGE);
			DrawText("-70 Coins", SCREEN_WIDTH/2 + 50, SCREEN_HEIGHT/2 - 50, 25, MAROON);
			break;

		case 100:
			DrawText("--> ", SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 50, 25, SKYBLUE);
			DrawText("100HP > > 160HP | ", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 50, 25, ORANGE);
			DrawText("-140 Coins", SCREEN_WIDTH/2 + 60, SCREEN_HEIGHT/2 - 50, 25, MAROON);
			break;

		case 160:
			DrawText("--> ", SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 50, 25, SKYBLUE);
			DrawText("160HP > > 250HP | ", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 50, 25, ORANGE);
			DrawText("-190 Coins", SCREEN_WIDTH/2 + 70, SCREEN_HEIGHT/2 - 50, 25, MAROON);
			break;

		case 250:
			DrawText("--> ", SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 50, 25, SKYBLUE);
			DrawText("250HP > > 500HP | ", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 50, 25, ORANGE);
			DrawText("-413 Coins", SCREEN_WIDTH/2 + 70, SCREEN_HEIGHT/2 - 50, 25, MAROON);
			break;

		case 500:
			DrawText("--> ", SCREEN_WIDTH/2 - 140, SCREEN_HEIGHT/2 - 50, 25, SKYBLUE);
			DrawText("500HP | ", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 50, 25, ORANGE);
			DrawText("MAX HP", SCREEN_WIDTH/2 + 5, SCREEN_HEIGHT/2 - 50, 25, MAROON);
			break;

		case 1000:
			DrawText("--> ", SCREEN_WIDTH/2 - 140, SCREEN_HEIGHT/2 - 50, 25, SKYBLUE);
			DrawText("1000HP ? ", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 50, 25, ORANGE);
			DrawText("NO WAY", SCREEN_WIDTH/2 + 15, SCREEN_HEIGHT/2 - 50, 25, MAROON);
			break;

		default:
			break;
		}


	}

	if (get_page(menu) % 2 == 0) {
		
		DrawText("Gun Upgrades", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 200, 30, BLUE);
		DrawText("Spaceship Upgrades < <", SCREEN_WIDTH/2 - 400, SCREEN_HEIGHT/2 - 195, 25, GRAY);
	}


	DrawText("Press [ALT] to go back.", 10, SCREEN_HEIGHT - 40, 20, DARKGREEN);
}


void interface_draw_menu(Menu menu, State state, Store store) {

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
		draw_store_menu(menu, state, store);
		break;
	case 3:
		draw_help_menu(menu);
		break;

	}
	
	EndDrawing();

}
