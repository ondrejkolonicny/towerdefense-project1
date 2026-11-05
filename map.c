#include "map.h"

#include "definitions.h"
#include <string.h>

// copies a map to map and then sets some default values for certain tiles
void init_map(Tile map[ROWS][COLS])
{
    Tile init_map[ROWS][COLS] =
        {
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}},
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_WATER}, {TYPE_WATER}, {TYPE_WATER}, {TYPE_WATER}, {TYPE_WATER}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
            {{TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_WATER}, {TYPE_WATER}, {TYPE_WATER}, {TYPE_WATER}, {TYPE_WATER}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_WATER}, {TYPE_WATER}, {TYPE_WATER}, {TYPE_WATER}, {TYPE_WATER}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_PATH}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
            {{TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}, {TYPE_GRASS}},
    };

    memcpy(map, init_map, sizeof(init_map));

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (map[i][j].type == TYPE_GRASS)
                map[i][j].is_buildable = true;
        }
    }

    map[ROWS - 1][COLS - 1].is_buildable = false; // towerRect
    map[ROWS - 1][COLS - 2].is_buildable = false;
    map[ROWS - 1][COLS - 3].is_buildable = false;
}

// goes through types of tiles and adds seperate colours and fills them
void draw_map(SDL_Renderer *renderer, Tile map[ROWS][COLS])
{

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++) // musi se pak na konci predelat na textury, asi funkce na prirazeni jednotlivych textur jeste do tehle
        {
            if (map[i][j].type == TYPE_GRASS)
            {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                fill_tile(renderer, i, j, TILE_SIZE);
            }

            else if (map[i][j].type == TYPE_PATH)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                fill_tile(renderer, i, j, TILE_SIZE);
            }

            else if (map[i][j].type == TYPE_WATER)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                fill_tile(renderer, i, j, TILE_SIZE);
            }

            if (i == ROWS - 1 && (j == COLS - 1 || j == COLS - 2 || j == COLS - 3))
            {
                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                fill_tile(renderer, i, j, TILE_SIZE);
            }
        }
    }
}

// fills a square at row, col of size size
void fill_tile(SDL_Renderer *renderer, int row, int col, int size)
{
    SDL_Rect current_tile = {col * size, row * size, size, size};

    SDL_RenderFillRect(renderer, &current_tile);
}