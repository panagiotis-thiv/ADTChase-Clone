#include "raylib.h"

#include "state.h"
#include "interface.h"

// Assets
//Texture bird_img;
//Sound game_over_snd;


void interface_init() {
	// Αρχικοποίηση του παραθύρου
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);
    InitAudioDevice();


	// Φόρτωση εικόνων και ήχων
	//bird_img = LoadTextureFromImage(LoadImage("assets/bird.png"));
	//game_over_snd = LoadSound("assets/game_over.mp3");
}

void interface_close() {
	CloseAudioDevice();
	CloseWindow();
}


// Draw game (one frame)
void interface_draw_frame(State state) {
	BeginDrawing();
	// Καθαρισμός, θα τα σχεδιάσουμε όλα από την αρχή
	ClearBackground(BLACK);

	// Σχεδιάζουμε τον χαρακτήρα και τις 2 μπάλες
	//DrawTexture(bird_img, state->character.x, state->character.y, WHITE);
	DrawCircle(SCREEN_WIDTH/2+state_info(state)->spaceship->position.x, SCREEN_HEIGHT/2+(state_info(state)->spaceship->position.y*(-1)), state_info(state)->spaceship->size, BLUE);
	DrawLine(0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, WHITE);
	DrawCircle(SCREEN_WIDTH/2+state_info(state)->spaceship->orientation.x,SCREEN_HEIGHT/2+(state_info(state)->spaceship->orientation.y*(-1)),5,RED);
	//DrawCircle(state->ball2.position.x, state->ball2.position.y, state->ball2.radius, RED);
	//List obj = state_objects(state, (Vector2){-1000000000000,1000000000000}, (Vector2){1000000000000,-1000000000000});

	
	// Σχεδιάζουμε το σκορ και το FPS counter
	DrawText(TextFormat("%i", state_info(state)->score), 10, 10, 40, GRAY);
	DrawFPS(SCREEN_WIDTH - 80, 0);

	// Αν το παιχνίδι έχει τελειώσει, σχεδιάζομαι το μήνυμα για να ξαναρχίσει
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
