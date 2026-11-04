#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800
#define ROWS 15
#define COLS 20
#define TILE_SIZE 40

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

typedef enum
{
    TYPE_NORMAL,
    TYPE_RANGE,
    TYPE_SLOW

} TowerType;

typedef struct
{
    SDL_Rect rect;
    bool is_placing_tower;
    TowerType type;

} TowerButtonRect;

typedef struct
{
    int x;
    int y;
    int size;
    int radius;
    int attack_rate;
    int cost;
    TowerType type;

} Tower;

void init_map(Tile map[ROWS][COLS]);
void draw_map(SDL_Renderer *renderer, Tile map[ROWS][COLS]);
void fill_tile(SDL_Renderer *renderer, int row, int col, int size);
void place_tower(SDL_Point mouse_tile, Tile map[ROWS][COLS], Tower towers[ROWS * COLS], int tower_count);
void add_tower_to_towers(SDL_Point mouse_tile, Tower towers[ROWS * COLS], int tower_count);
void add_type_to_btn(SDL_Point mouse_tile, TowerButtonRect *tower_button);
void add_values_to_tower(Tower towers[ROWS * COLS], TowerButtonRect tower_button, int tower_count);
void draw_towers();
void draw_enemies();
void draw_tower_projectiles();

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

    Tower towers[ROWS * COLS] = {0}; // do tohoto se pridava v add_towers
    int tower_count = 0;

    Uint64 now, last;

    SDL_Event event = {};
    int running = 1;

    TowerButtonRect tower_button = {
        .rect = {(COLS - 3) * TILE_SIZE, (ROWS - 1) * TILE_SIZE, 3 * TILE_SIZE, TILE_SIZE},
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
        last = now;
        double deltaTime = (now - last) / (double)SDL_GetPerformanceFrequency();

        draw_map(renderer, map);

        // double buffering
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

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

// program inicializuje mapu a pak na ni prida tlacitko na pridani towerky, pokud se na nej klikne, zapne se pokladani towerky,
// pokud potom uzivatel klikne na placable tile na mape tak se tam prida towerka, taky se potom prida do arraye towerek (towers),
// coz zatim zapise pouze jeho x a y hodnoty (cols a rows), prida se na prvni index

// tower button
// delsi rect dole vpravo, podle typu a costu, zleva doprava....
// kdyz kliknde na towerRect tak prvne zjistim, do jake casti towerRect klikl pomoci fce add_type_to_btn
//      podle toho urcim cost, radius a type