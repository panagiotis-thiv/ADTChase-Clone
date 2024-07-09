#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ADTVector.h"
#include "ADTList.h"
#include "state.h"
#include "vec2.h"
#include "menu.h"
#include "level.h"
#include "store.h"

//Yes, this is written so bad it's insane.

struct upgrades {

    int spaceshipHP;

    int selected_gun;
    int prev_gun;

    bool piston;
    bool rifle;
    bool shotgun;

    int bullets;
    int delay;
    int damage;

};

Store store_init() {

    Store store = malloc(sizeof(*store));

    store->spaceshipHP = 50;

    store->selected_gun = 0;
    store->prev_gun = 0;

    store->piston = true;
    store->rifle = false;
    store->shotgun = false;

    store->bullets = 50;
    store->delay = 70;
    store->damage = 15;

    return store;
}

//This part, yes this part...
void store_update(Store stats, int spaceshipHP, int selected_gun, int bullets, int delay, int damage) {

    if (spaceshipHP != -1)
        stats->spaceshipHP = spaceshipHP;

    if (selected_gun != -1)
        stats->selected_gun = selected_gun;

    if (bullets != -1)
        stats->bullets = bullets;
    
    if (bullets != -1)
        stats->delay = delay;

    if (damage != -1)
        stats->damage = damage;
}

int store_info(Store stats, StoreSetting option) {

    switch (option) {

    case spaceshipHP:
        return stats->spaceshipHP;
    case selected_gun:
        return stats->selected_gun;
    case bullets:
        return stats->bullets;
    case delay:
        return stats->delay;
    case damage:
        return stats->damage;
    case rifle:
        return stats->rifle;
    case shotgun:
        return stats->shotgun;
    }

    return 0;
}

void store_prev_gun(Store store, int prev_gun) {
    store->prev_gun = prev_gun;
}

int store_get_prev_gun(Store store) {
    return store->prev_gun;
}

void store_update_rifle(Store store, bool rifle) {
    store->rifle = rifle;
}

void store_update_shotgun(Store store, bool shotgun) {
    store->shotgun = shotgun;
}

//There has to be a better way to achieve this but right now 
int store_get_second_prev_gun(Store store) {
    return 3 - (store->selected_gun + store->prev_gun);
}