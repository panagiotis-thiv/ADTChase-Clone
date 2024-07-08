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

struct upgrades {

    int spaceshipHP;

    int gun;
    int bullets;
    int delay;

};

Store store_init() {

    Store store = malloc(sizeof(*store));

    store->spaceshipHP = 50;
    store->gun = 1;
    store->bullets = 25;
    store->delay = 30;

    return store;
}

void store_update(Store stats, int spaceshipHP, int gun, int bullets, int delay) {

    if (spaceshipHP != -1)
        stats->spaceshipHP = spaceshipHP;

    if (gun != -1)
        stats->gun = gun;

    if (bullets != -1)
        stats->bullets = bullets;
    
    if (bullets != -1)
        stats->delay = delay;

}

int store_info(Store stats, StoreSetting option) {

    switch (option) {

    case spaceshipHP:
        return stats->spaceshipHP;
    case gun:
        return stats->gun;
    case bullets:
        return stats->bullets;
    case delay:
        return stats->delay;
    }

    return 0;
}