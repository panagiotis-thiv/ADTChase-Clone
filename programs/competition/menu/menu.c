#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ADTVector.h"
#include "ADTList.h"
#include "state.h"
#include "interface.h"
#include "menu.h"
#include "vec2.h"


struct menu {

    int m_selected;
    int m_active;
    int options;
    //Vector options;

};

Menu menu_create() {

    Menu menu = malloc(sizeof(*menu));

    menu->m_selected = 1;
    menu->m_active = 0;
    menu->options = 2;

    return menu;
}

int selected_menu(Menu menu) {
    return menu->m_selected;
}

void next_menu(Menu menu) {

    menu->m_selected++;
    if (menu->m_selected > menu->options) 
        menu->m_selected = menu->options;
}  

void prev_menu(Menu menu) {

    menu->m_selected--;
    if (menu->m_selected < 1) 
        menu->m_selected = 1;
}

int active_menu(Menu menu) {
    return menu->m_active;
}

void set_active_menu(Menu menu, int select) {
    menu->m_active = select;
}