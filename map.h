#ifndef MAP_H
#define MAP_H

#include "definitions.h"

void init_map(Tile map[ROWS][COLS]);
void draw_map(SDL_Renderer *renderer, Tile map[ROWS][COLS]);
void fill_tile(SDL_Renderer *renderer, int row, int col, int size);

#endif