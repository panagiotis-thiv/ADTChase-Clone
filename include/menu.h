#pragma once

#include "raylib.h"
#include "ADTList.h"

typedef struct menu* Menu;

Menu menu_create();

int selected_menu(Menu menu);

void next_menu(Menu menu);

void prev_menu(Menu menu);

int active_menu(Menu menu);

void set_active_menu(Menu menu, int select);