#pragma once

typedef struct levels* LvlStats;

typedef enum {

    asteroid_hp         = 127,
    asteroid_speed      = 227,
    spaceship_hp        = 327,
    reward              = 427,            
    passive_asteroids   = 527

} LvLSetting;

LvlStats level_init();

int level_info(LvlStats level, LvLSetting setting);

void level_update(LvlStats level, int asteroid_hp, int asteroid_speed, int spaceship_hp, int reward, bool passive_asteroids);