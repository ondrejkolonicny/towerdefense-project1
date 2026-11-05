#include "tower.h"

#include "definitions.h"
#include <stdbool.h>

// if placable == true turns off placing tower button and then changes the pressed tile value of has tower to true
void place_tower(SDL_Point mouse_tile, Tile map[ROWS][COLS], Tower towers[ROWS * COLS], int tower_count)
{

    map[mouse_tile.y][mouse_tile.x].has_tower = true;
    add_tower_to_towers(mouse_tile, towers, tower_count);
}

// adds tower to towers and sets its values
void add_tower_to_towers(SDL_Point mouse_tile, Tower towers[ROWS * COLS], int tower_count)
{
    towers[tower_count].x = mouse_tile.x;
    towers[tower_count].y = mouse_tile.y;
}

// adds towerType to the button
void add_type_to_btn(SDL_Point mouse_tile, TowerButtonRect *tower_button)
{
    if (mouse_tile.x == COLS - 1)
        tower_button->type = TYPE_NORMAL;

    else if (mouse_tile.x == COLS - 2)
        tower_button->type = TYPE_RANGE;

    else if (mouse_tile.x == COLS - 3)
        tower_button->type = TYPE_SLOW;
}

// adds values to current tower from the button
void add_values_to_tower(Tower towers[ROWS * COLS], TowerButtonRect tower_button, int tower_count)
{
    if (tower_button.type == TYPE_NORMAL)
    {
        towers[tower_count].type = TYPE_NORMAL;
        towers[tower_count].cost = 100;
        towers[tower_count].radius = 3;
        towers[tower_count].attack_rate = 5;
    }

    else if (tower_button.type == TYPE_RANGE)
    {
        towers[tower_count].type = TYPE_RANGE;
        towers[tower_count].cost = 250;
        towers[tower_count].radius = 5;
        towers[tower_count].attack_rate = 2;
    }

    else if (tower_button.type == TYPE_SLOW)
    {
        towers[tower_count].type = TYPE_SLOW;
        towers[tower_count].cost = 175;
        towers[tower_count].radius = 3;
        towers[tower_count].attack_rate = 3;
    }
}