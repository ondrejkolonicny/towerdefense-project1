#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#include "definitions.h"
#include "map.h"
#include "tower.h"
#include "enemy.h"

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "main window", // title
        100,
        100,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN // idk asi ze jde videt

    );

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // accelrated pozuiva gpu vsync at to jede na 60fps

    Tile map[ROWS][COLS] = {0};
    init_map(map);

    Tower towers[ROWS * COLS] = {0};
    int tower_count = 0;

    Uint64 last = SDL_GetPerformanceCounter();
    Uint64 now;

    SDL_Event event = {};
    int running = 1;

    TowerButtonRect tower_button = {
        .rect = {(COLS - AMOUNT_OF_TOWER_TYPES) * TILE_SIZE, (ROWS - 1) * TILE_SIZE, AMOUNT_OF_TOWER_TYPES * TILE_SIZE, TILE_SIZE},
        .is_placing_tower = false,
    };

    while (running == 1)
    {
        while (SDL_PollEvent(&event))
        {

            if (event.type == SDL_QUIT)
                running = 0;

            else if (event.button.type == SDL_MOUSEBUTTONUP)
            {
                SDL_Point mouse_coords = {
                    .x = event.button.x,
                    .y = event.button.y,
                };

                SDL_Point mouse_tile = {
                    .x = mouse_coords.x / TILE_SIZE, // columns
                    .y = mouse_coords.y / TILE_SIZE, // rows
                };

                if (SDL_PointInRect(&mouse_coords, &tower_button.rect))
                {
                    tower_button.is_placing_tower = true;
                    add_type_to_btn(mouse_tile, &tower_button);
                    if (tower_button.type == TYPE_RANGE)
                        printf("type range\n");
                    else if (tower_button.type == TYPE_NORMAL)
                        printf("type normal\n");
                    else if (tower_button.type == TYPE_SLOW)
                        printf("type slow\n");
                }

                else if (tower_button.is_placing_tower)
                {
                    if (map[mouse_tile.y][mouse_tile.x].is_buildable && !map[mouse_tile.y][mouse_tile.x].has_tower)
                    {
                        place_tower(mouse_tile, map, towers, tower_count);
                        add_values_to_tower(towers, tower_button, tower_count);
                        tower_count++;
                        tower_button.is_placing_tower = false;
                    }
                }

                printf("placing towr active? :%d\n", tower_button.is_placing_tower);
                printf("tower count: %d\n", tower_count);
                // printf("%d\n%d\n", towers[1].x, towers[1].y); // ukazuje druhou towerku pridanou na mapu
                printf("\n");
            }
        }

        // delta time -- pouziti: pohyb enemy, shooting of tower
        now = SDL_GetPerformanceCounter();
        double deltaTime = (now - last) / (double)SDL_GetPerformanceFrequency();
        last = now;

        draw_map(renderer, map);

        // double buffering
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// program inicializuje mapu a pak na ni prida tlacitko na pridani towerky, pokud se na nej klikne, zapne se pokladani towerky,
// pokud potom uzivatel klikne na placable tile na mape tak se tam prida towerka, taky se potom prida do arraye towerek (towers),
// coz zatim zapise pouze jeho x a y hodnoty (cols a rows), prida se na prvni index