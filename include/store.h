#pragma once

typedef struct upgrades* Store;

typedef enum {

    spaceshipHP = 500,
    gun         = 600,
    bullets     = 700,
    delay       = 800

} StoreSetting;

Store store_init();

void store_update(Store stats, int spaceshipHP, int gun, int bullets, int delay);

int store_info(Store stats, StoreSetting option);