
#ifndef MINES_H
#define MINES_H

#include <stdbool.h>
#include "raylib.h"

#define TILE_SIZE 16

#define TILE_OPEN_0     (Rectangle) { 0,   0, TILE_SIZE, TILE_SIZE}
#define TILE_FLAG       (Rectangle) {16,   0, TILE_SIZE, TILE_SIZE}
#define TILE_CLOSED     (Rectangle) {32,   0, TILE_SIZE, TILE_SIZE}
#define TILE_PRESSED    (Rectangle) {48,   0, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_1     (Rectangle) { 0,  16, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_2     (Rectangle) {16,  16, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_3     (Rectangle) {32,  16, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_4     (Rectangle) {48,  16, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_5     (Rectangle) { 0,  32, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_6     (Rectangle) {16,  32, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_7     (Rectangle) {32,  32, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_8     (Rectangle) {48,  32, TILE_SIZE, TILE_SIZE}
#define TILE_MINE       (Rectangle) { 0,  48, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_MINE  (Rectangle) {16,  48, TILE_SIZE, TILE_SIZE}
#define TILE_ERROR      (Rectangle) {32,  48, TILE_SIZE, TILE_SIZE}
#define TILE_FALSE_FLAG (Rectangle) {48,  48, TILE_SIZE, TILE_SIZE}

typedef struct
{
    bool isOpen;
    bool isFlag;
    bool isMine;
    bool update;

    int around;
} Tile;

typedef struct
{
    int width;
    int height;
    int mines;

    Tile** field;
} Grid;

void GameInit (void);
void GameDeinit (void);
void GameDraw (void);
void GameUpdate (void);

#endif // MINES_H
