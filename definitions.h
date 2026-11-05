#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800
#define ROWS 15
#define COLS 20
#define TILE_SIZE 40
#define AMOUNT_OF_TOWER_TYPES 3

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

#endif