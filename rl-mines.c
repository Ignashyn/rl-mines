
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "raylib.h"

#define TILE_SIZE       16

#define TILE_OPEN_0     (Rectangle) { 0,   0, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_1     (Rectangle) {16,   0, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_2     (Rectangle) {32,   0, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_3     (Rectangle) { 0,  16, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_4     (Rectangle) {16,  16, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_5     (Rectangle) {32,  16, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_6     (Rectangle) { 0,  32, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_7     (Rectangle) {16,  32, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_8     (Rectangle) {32,  32, TILE_SIZE, TILE_SIZE}
#define TILE_MINE       (Rectangle) { 0,  48, TILE_SIZE, TILE_SIZE}
#define TILE_OPEN_MINE  (Rectangle) {16,  48, TILE_SIZE, TILE_SIZE}
#define TILE_FALSE_FLAG (Rectangle) {32,  48, TILE_SIZE, TILE_SIZE}
#define TILE_FLAG       (Rectangle) { 0,  64, TILE_SIZE, TILE_SIZE}
#define TILE_CLOSED     (Rectangle) {16,  64, TILE_SIZE, TILE_SIZE}

typedef struct
{
    bool isOpen;
    bool isFlag;
    bool isMine;

    int around;
} Tile;

typedef struct
{
    int width;
    int height;
    int mines;

    Tile** field;
} Grid;

static int frames;
static int secs;
static float scale;
static bool isDefeat;
static bool isVictory;
static bool isGameOn;
static bool isGridFilled;
static Grid grid;
static Texture2D tiles;
static RenderTexture2D framebuf;
static char title[50];

void GridAlloc (int width, int height, int mines)
{
    grid.width = width;
    grid.height = height;
    grid.mines = mines;

    grid.field = calloc(height, sizeof(Tile*));
    for (int i = 0;i < height;++i) grid.field[i] = calloc(width, sizeof(Tile));
}

void GridDealloc (void)
{
    for (int i = 0;i < grid.height;++i) free(grid.field[i]);
    free(grid.field);
}

void GridFill (int curX, int curY)
{
    for (int mines = 0;mines < grid.mines;) {
        int x = rand() % grid.width;
        int y = rand() % grid.height;

        if (grid.field[y][x].isMine) continue;
        if (x == curX || y == curY) continue;

        mines++;
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

void GridTileFlag (int x, int y)
{
    if (!grid.field[y][x].isOpen) {
        grid.field[y][x].isFlag = !grid.field[y][x].isFlag;
    }
}

void GridTileOpen (int x, int y)
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

    if (grid.field[y][x].around != flags) return;

    for (int i = -1;i < 2;++i) {
        if (y+i < 0 || y+i >= grid.height) continue;
        for (int j = -1;j < 2;++j) {
            if (x+j < 0 || x+j >= grid.width) continue;
            if (!grid.field[y+i][x+j].isOpen) {
                if (!grid.field[y+i][x+j].isFlag) {
                    grid.field[y+i][x+j].isOpen = true;
                    GridTileOpen(x+j, y+i);
                }
            }
        }
    }
}

void GridReinit (int width, int height, int mines)
{
    frames = 0;
    secs = 0;

    isDefeat = false;
    isVictory = false;
    isGameOn = false;
    isGridFilled = false;

    GridDealloc();
    GridAlloc(width, height, mines);

    SetWindowSize(TILE_SIZE * width * scale, TILE_SIZE * height * scale);
    framebuf = LoadRenderTexture(TILE_SIZE * width * scale, TILE_SIZE * height * scale);
    sprintf(title, "raylib Minesweeper (time: %4d)", secs);
}

void CheckIfDefeat (void)
{
    for (int y = 0;y < grid.height;++y) {
        for (int x = 0;x < grid.width;++x) {
            if (grid.field[y][x].isMine && grid.field[y][x].isOpen) {
                if (!isDefeat) {
                    isDefeat = true;
                }
            }
        }
    }
}

void CheckIfVictory (void)
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
        }
    }
}

Rectangle SelectTileTexture (int x, int y)
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
    InitWindow(9 * TILE_SIZE * scale, 9 * TILE_SIZE * scale, "raylib Minesweeper");

    char *logopath = (char*)GetApplicationDirectory();
    strcat(logopath, "res/logo.png");

    if (!FileExists(logopath)) {
        exit(1);
    } else {
        Image logo = LoadImage(logopath);
        SetWindowIcon(logo);
    }

    char *tilespath = (char*)GetApplicationDirectory();
    strcat(tilespath, "res/tiles.png");

    if (!FileExists(tilespath)) {
        exit(1);
    } else {
        tiles = LoadTexture(tilespath);
    }

    scale = 1.5f;
    frames = 0;
    secs = 0;

    isDefeat = false;
    isVictory = false;
    isGameOn = false;
    isGridFilled = false;

    GridReinit(9, 9, 10);
    framebuf = LoadRenderTexture(TILE_SIZE * scale * grid.width, TILE_SIZE * scale * grid.height);
}

void GameDeinit (void)
{
    UnloadTexture(tiles);
    UnloadRenderTexture(framebuf);
    GridDealloc();
    CloseWindow();
}

void GameDraw (void)
{
    BeginTextureMode(framebuf);
        for (int y = 0;y < grid.height;++y) {
            for (int x = 0;x < grid.width;++x) {
                DrawTexturePro(
                    tiles,
                    SelectTileTexture(x, y),
                    (Rectangle) {TILE_SIZE * scale * x, TILE_SIZE * scale * y, TILE_SIZE * scale, TILE_SIZE * scale},
                    (Vector2) {0, 0},
                    0.0f,
                    WHITE
                );
            }
        }
    EndTextureMode();

    BeginDrawing();
        DrawTexturePro(
            framebuf.texture,
            (Rectangle) {0, 0, framebuf.texture.width, -framebuf.texture.height},
            (Rectangle) {0, 0, framebuf.texture.width, framebuf.texture.height},
            (Vector2) {0, 0},
            0.0f,
            WHITE
        );
    EndDrawing();
}

void GameUpdate (void)
{
    CheckIfDefeat();
    CheckIfVictory();

    if (!(isDefeat || isVictory)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (!isGameOn) {
                isGameOn = true;
            }

            if (!isGridFilled) {
                GridFill(
                    GetMouseX() / (TILE_SIZE * scale),
                    GetMouseY() / (TILE_SIZE * scale));

                isGridFilled = true;
            }

            GridTileOpen(
                GetMouseX() / (TILE_SIZE * scale),
                GetMouseY() / (TILE_SIZE * scale));
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            GridTileFlag(
                GetMouseX() / (TILE_SIZE * scale),
                GetMouseY() / (TILE_SIZE * scale));
        }

        if (isGameOn) {
            frames++;
            if (frames == 60) {
                secs++;
                frames = 0;
                sprintf(title, "raylib Minesweeper (time: %4d)", secs);
            }
        }
    } else {
        sprintf(title, "raylib Minesweeper (%s%stime: %4d)", isDefeat ? "Defeat, " : "", isVictory ? "Victory, " : "", secs);
    }

    SetWindowTitle(title);

    if (IsKeyPressed(KEY_ONE)) {      
        GridReinit(9, 9, 10);
    }

    if (IsKeyPressed(KEY_TWO)) {
        GridReinit(16, 16, 40);
    }

    if (IsKeyPressed(KEY_THREE)) {
        GridReinit(30, 16, 99);
    }

    if (IsKeyPressed(KEY_R)) {
        GridReinit(grid.width, grid.height, grid.mines);
    }

    if (IsKeyPressed(KEY_MINUS)) {
        if (scale > 1.5f) scale -= 0.5f;
        SetWindowSize(TILE_SIZE * scale * grid.width, TILE_SIZE * scale * grid.height);
        framebuf = LoadRenderTexture(TILE_SIZE * scale * grid.width, TILE_SIZE * scale * grid.height);
    }

    if (IsKeyPressed(KEY_EQUAL)) {
        if (scale < 3.0f) scale += 0.5f;
        SetWindowSize(TILE_SIZE * scale * grid.width, TILE_SIZE * scale * grid.height);
        framebuf = LoadRenderTexture(TILE_SIZE * scale * grid.width, TILE_SIZE * scale * grid.height);
    }
}

int main (void)
{
    srand(time(NULL));

    SetTraceLogLevel(LOG_ERROR);
    SetTargetFPS(60);

    GameInit();    
    while (!WindowShouldClose()) {
        GameUpdate();
        GameDraw();
    }
    GameDeinit();

    return 0;
}
