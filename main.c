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

} Tile;

void draw_map(SDL_Renderer *renderer, Tile map[ROWS][COLS]);
void fill_tile(SDL_Renderer *renderer, int row, int col);

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

    SDL_Event event = {};
    int running = 1;

    Tile map[ROWS][COLS] =
        {
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}},
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_WATER, false}, {TYPE_WATER, false}, {TYPE_WATER, false}, {TYPE_WATER, false}, {TYPE_WATER, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
            {{TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_WATER, false}, {TYPE_WATER, false}, {TYPE_WATER, false}, {TYPE_WATER, false}, {TYPE_WATER, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_WATER, false}, {TYPE_WATER, false}, {TYPE_WATER, false}, {TYPE_WATER, false}, {TYPE_WATER, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_PATH, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
            {{TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}, {TYPE_GRASS, false}},
    };

    while (running == 1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        draw_map(renderer, map);

        SDL_RenderPresent(renderer); // double buffering
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
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
                fill_tile(renderer, i, j);
            }

            else if (map[i][j].type == TYPE_PATH)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                fill_tile(renderer, i, j);
            }

            else if (map[i][j].type == TYPE_WATER)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                fill_tile(renderer, i, j);
            }
        }
    }
}

void fill_tile(SDL_Renderer *renderer, int row, int col)
{
    SDL_Rect current_tile = {col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE};

    SDL_RenderFillRect(renderer, &current_tile);
}