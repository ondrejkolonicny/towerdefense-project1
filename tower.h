#ifndef TOWER_H
#define TOWER_H

#include "definitions.h"

void place_tower(SDL_Point mouse_tile, Tile map[ROWS][COLS], Tower towers[ROWS * COLS], int tower_count);
void add_tower_to_towers(SDL_Point mouse_tile, Tower towers[ROWS * COLS], int tower_count);
void add_type_to_btn(SDL_Point mouse_tile, TowerButtonRect *tower_button);
void add_values_to_tower(Tower towers[ROWS * COLS], TowerButtonRect tower_button, int tower_count);
void draw_towers();
void draw_tower_projectiles();

#endif