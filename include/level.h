#pragma once

typedef struct levels* LvlStats;

typedef enum {

    asteroid_hp         = 100,
    asteroid_speed      = 200,
    reward              = 300,            
    passive_asteroids   = 400

} LvLSetting;

LvlStats level_init();

int level_info(LvlStats level, LvLSetting setting);

void level_update(LvlStats level, int asteroid_hp, int asteroid_speed, int spaceship_hp, int reward, bool passive_asteroids);