#pragma once

typedef struct upgrades* Store;

typedef enum {

    spaceshipHP     = 500,
    selected_gun    = 600,
    bullets         = 700,
    delay           = 800,
    damage          = 900,
    rifle           = 1000,
    shotgun         = 1100

} StoreSetting;

Store store_init();

void store_update(Store stats, int spaceshipHP, int selected_gun, int bullets, int delay, int damage);

int store_info(Store stats, StoreSetting option);

void store_prev_gun(Store store, int prev_gun);

int store_get_prev_gun(Store store);

void store_update_rifle(Store store, bool rifle);

void store_update_shotgun(Store store, bool shotgun);

int store_get_second_prev_gun(Store store);