#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TILE_SIZE 40
#define ROWS 15
#define COLS 20

typedef enum
{
    TYPE_GRASS,
    TYPE_PATH,
    TYPE_WATER

} TileType;

typedef struct
{
    TileType type;
    bool has_tower;
    bool is_buildable;

} Tile;

typedef struct
{
    SDL_Rect rect;
    bool is_placing_tower;
} Button;

void init_map(Tile map[ROWS][COLS]);
void draw_map(SDL_Renderer *renderer, Tile map[ROWS][COLS]);
void fill_tile(SDL_Renderer *renderer, int row, int col, int size);

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "main window", // title
        100,
        100,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN

    );

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // accelrated pozuiva gpu vsync at to jede na 60fps

    Tile map[ROWS][COLS];
    init_map(map);

    Uint64 now, last;

    SDL_Event event = {};
    int running = 1;

    Button tower_button = {
        .rect = {(COLS - 1) * TILE_SIZE, (ROWS - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE},
        .is_placing_tower = false,
    };

    while (running == 1)
    {
        while (SDL_PollEvent(&event))
        {

            if (event.type == SDL_QUIT)
                running = 0;
            else if (event.button.type == SDL_MOUSEBUTTONUP) // kdyz se zmackne tlacitko a zaroven je na miste aktivace tower tak se mi zapne placing_tower
            {
                SDL_Point mouse_coords = {
                    .x = event.button.x,
                    .y = event.button.y,
                };

                if (SDL_PointInRect(&mouse_coords, &tower_button.rect))
                    tower_button.is_placing_tower = !tower_button.is_placing_tower;
            }
        }

        // delta time -- pouziti: pohyb enemy, shooting of tower
        now = SDL_GetPerformanceCounter();
        last = now;
        double deltaTime = (now - last) / (double)SDL_GetPerformanceFrequency();

        draw_map(renderer, map);

        // if (placing_tower == 1) // start copying mouse movement(in some variable), create ghost tower -> onClick place tower to the nearest tile
        // {
        // }

        // double buffering
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

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

    map[14][19].is_buildable = false;
}

void draw_map(SDL_Renderer *renderer, Tile map[ROWS][COLS])
{

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
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

            if (i == 14 && j == 19)
            {
                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                fill_tile(renderer, i, j, TILE_SIZE);
            }
        }
    }
}

void fill_tile(SDL_Renderer *renderer, int row, int col, int size)
{
    SDL_Rect current_tile = {col * size, row * size, size, size};

    SDL_RenderFillRect(renderer, &current_tile);
}