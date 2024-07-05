#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ADTVector.h"
#include "ADTList.h"
#include "state.h"
#include "vec2.h"
#include "menu.h"
#include "level.h"

struct levels {

    int asteroid_hp;
    int asteroid_speed;
    int spaceship_hp;   
    int reward;                 //Coins from destroying an asteroid (not exact, more like a max amount)

    bool passive_asteroids;     //Whether there are asteroids that can deal damage in addition to just colliding with them, they shoot projectiles or something similar.

};

LvlStats level_init() {

    LvlStats level = malloc(sizeof(*level));

    level->asteroid_hp = 50;
    level->asteroid_speed = 1;
    level->passive_asteroids = true;
    level->reward = 5;
    level->spaceship_hp = 0;

    return level;
}

int level_info(LvlStats level, LvLSetting setting) {

    switch (setting) {
    case asteroid_hp:
        return level->asteroid_hp;
    case asteroid_speed:
        return level->asteroid_speed;
    case spaceship_hp:
        return level->spaceship_hp;
    case reward:
        return level->reward;
    case passive_asteroids:
        return level->passive_asteroids;
    }

    return 0;
}

void level_update(LvlStats level, int asteroid_hp, int asteroid_speed, int spaceship_hp, int reward, bool passive_asteroids) {

    level->asteroid_hp = asteroid_hp;
    level->asteroid_speed = asteroid_speed;
    level->passive_asteroids = passive_asteroids;
    level->reward = reward;
    level->spaceship_hp = spaceship_hp;

}