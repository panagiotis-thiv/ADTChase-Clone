#pragma once

#include "state.h"
#include "menu.h"
#include "store.h"

// Αρχικοποιεί το interface του παιχνιδιού
void interface_init();

// Κλείνει το interface του παιχνιδιού
void interface_close();

// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state, Store store);

void interface_draw_menu(Menu menu, State state, Store store);

void draw_coinsReward(int coinsReward, Vector2 pos);