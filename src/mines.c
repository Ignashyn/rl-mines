
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "mines.h"
#include "raylib.h"

static float scale;
static Grid grid;
static bool isDefeat;
static bool isVictory;
static Texture2D tiles;
static int frames;
static int secs;
static char title[50];

static void GridAlloc (int width, int height, int mines)
{
    grid.width = width;
    grid.height = height;
    grid.mines = mines;

    grid.field = calloc(height, sizeof(Tile*));
    for (int i = 0;i < height;++i) grid.field[i] = calloc(width, sizeof(Tile));
}

static void GridDealloc (void)
{
    for (int i = 0;i < grid.height;++i) free(grid.field[i]);
    free(grid.field);
}

static void GridFill (void)
{
    for (int i = 0;i < grid.mines;++i) {
        int x = rand() % grid.width;
        int y = rand() % grid.height;

        if (grid.field[y][x].isMine) --i;
        else {
            grid.field[y][x].isMine = true;
            for (int i = -1;i < 2;++i) {
                if (y+i < 0 || y+i >= grid.height) continue;
                for (int j = -1;j < 2;++j) {
                    if (x+j < 0 || x+j >= grid.width) continue;
                    grid.field[y+i][x+j].around++;
                }
            }
        }
    }
}

static void GridTileFlag (int x, int y)
{
    if (!grid.field[y][x].isOpen) {
        grid.field[y][x].isFlag = !grid.field[y][x].isFlag;
        grid.field[y][x].update = true;
    }
}

static void GridTileOpen (int x, int y)
{
    int flags = 0;

    if (grid.field[y][x].isFlag) return;
    
    if (grid.field[y][x].isOpen) {
        for (int i = -1;i < 2;++i) {
            if (y+i < 0 || y+i >= grid.height) continue;
            for (int j = -1;j < 2;++j) {
                if (x+j < 0 || x+j >= grid.width) continue;
                if (grid.field[y+i][x+j].isFlag) flags++;
            }
        }
    }

    grid.field[y][x].isOpen = true;
    grid.field[y][x].update = true;

    if (grid.field[y][x].around != flags) return;

    for (int i = -1;i < 2;++i) {
        if (y+i < 0 || y+i >= grid.height) continue;
        for (int j = -1;j < 2;++j) {
            if (x+j < 0 || x+j >= grid.width) continue;
            if (!grid.field[y+i][x+j].isOpen) {
                if (!grid.field[y+i][x+j].isFlag) {
                    grid.field[y+i][x+j].isOpen = true;
                    grid.field[y+i][x+j].update = true;
                    GridTileOpen(x+j, y+i);
                }
            }
        }
    }
}

static void GridUpdate (void)
{
    for (int y = 0;y < grid.height;++y) {
        for (int x = 0;x < grid.width;++x) {
            grid.field[y][x].update = true;
        }
    }
}

static void GridReinit (int width, int height, int mines)
{
    GridDealloc();
    GridAlloc(width, height, mines);
    GridFill();
    SetWindowSize(TILE_SIZE * scale * width, TILE_SIZE * scale * height);
    GridUpdate();
    frames = 0;
    secs = 0;
    sprintf(title, "raylib Minesweeper (time: %4d)", secs);
}

static void CheckIfDefeat (void)
{
    for (int y = 0;y < grid.height;++y) {
        for (int x = 0;x < grid.width;++x) {
            if (grid.field[y][x].isMine && grid.field[y][x].isOpen) {
                if (!isDefeat) {
                    isDefeat = true;
                    GridUpdate();
                }
            }
        }
    }
}

static void CheckIfVictory (void)
{
    int opens = 0;
    for (int y = 0;y < grid.height;++y) {
        for (int x = 0;x < grid.width;++x) {
            if (grid.field[y][x].isOpen) opens++;
        }
    }
    if (opens == grid.width * grid.height - grid.mines) {
        if (!isVictory) {
            isVictory = true;
            GridUpdate();
        }
    }
}

static Rectangle SelectTileTexture (int x, int y)
{
    if (grid.field[y][x].isOpen) {
        if (grid.field[y][x].isMine) {
            return TILE_OPEN_MINE;
        }
        switch (grid.field[y][x].around) {
            case 0: return TILE_OPEN_0;
            case 1: return TILE_OPEN_1;
            case 2: return TILE_OPEN_2;
            case 3: return TILE_OPEN_3;
            case 4: return TILE_OPEN_4;
            case 5: return TILE_OPEN_5;
            case 6: return TILE_OPEN_6;
            case 7: return TILE_OPEN_7;
            case 8: return TILE_OPEN_8;
        }
    } else if (grid.field[y][x].isFlag) {
        if (isDefeat) {
            if (!grid.field[y][x].isMine) return TILE_FALSE_FLAG;
        }
        return TILE_FLAG;
    } else {
        if (isDefeat) {
            if (grid.field[y][x].isMine) return TILE_MINE;
        }
        return TILE_CLOSED;
    }
}

void GameInit (void)
{
    scale = 1.5f;
    GridReinit(9, 9, 10);
    isDefeat = false;
    isVictory = false;
    tiles = LoadTexture("resources/tiles.png");
    frames = 0;
    secs = 0;
}

void GameDeinit (void)
{
    UnloadTexture(tiles);
    GridDealloc();
}

void GameDraw (void)
{
    BeginDrawing();
        for (int y = 0;y < grid.height;++y) {
            for (int x = 0;x < grid.width;++x) {
                if (grid.field[y][x].update) {
                    DrawTexturePro(
                        tiles,
                        SelectTileTexture(x, y),
                        (Rectangle) {
                            x * scale * TILE_SIZE,
                            y * scale * TILE_SIZE,
                            TILE_SIZE * scale,
                            TILE_SIZE * scale
                        },
                        (Vector2) {0, 0},
                        0,
                        (Color) {255, 255, 255, 255});
                    grid.field[y][x].update = false;
                }
            }
        }
    EndDrawing();
}

void GameUpdate (void)
{
    CheckIfDefeat();
    CheckIfVictory();

    if (!(isDefeat || isVictory)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            GridTileOpen(
                GetMouseX() / (TILE_SIZE * scale),
                GetMouseY() / (TILE_SIZE * scale));
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            GridTileFlag(
                GetMouseX() / (TILE_SIZE * scale),
                GetMouseY() / (TILE_SIZE * scale));
        }

        frames++;
        if (frames == 60) {
            secs++;
            frames = 0;
            sprintf(title, "raylib Minesweeper (time: %4d)", secs);
        }
    } else {
        sprintf(title, "raylib Minesweeper (%s%stime: %4d)", isDefeat ? "Defeat, " : "", isVictory ? "Victory, " : "", secs);
    }

    SetWindowTitle(title);

    if (IsKeyPressed(KEY_ONE)) {
        isDefeat = false;
        isVictory = false;
        GridReinit(9, 9, 10);
    }

    if (IsKeyPressed(KEY_TWO)) {
        isDefeat = false;
        isVictory = false;
        GridReinit(16, 16, 40);
    }

    if (IsKeyPressed(KEY_THREE)) {
        isDefeat = false;
        isVictory = false;
        GridReinit(30, 16, 99);
    }

    if (IsKeyPressed(KEY_R)) {
        isDefeat = false;
        isVictory = false;
        GridReinit(grid.width, grid.height, grid.mines);
    }

    if (IsKeyPressed(KEY_E)) {
        GridUpdate();
    }

    if (IsKeyPressed(KEY_MINUS)) {
        if (scale > 1.5f) scale -= 0.5f;
        SetWindowSize(TILE_SIZE * scale * grid.width, TILE_SIZE * scale * grid.height);
        GridUpdate();
    }

    if (IsKeyPressed(KEY_EQUAL)) {
        if (scale < 3.0f) scale += 0.5f;
        SetWindowSize(TILE_SIZE * scale * grid.width, TILE_SIZE * scale * grid.height);
        GridUpdate();
    }
}
