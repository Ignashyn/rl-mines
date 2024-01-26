
#include <stdlib.h>
#include <time.h>

#include "mines.h"
#include "raylib.h"

int main (void)
{
    srand(time(NULL));

    SetTraceLogLevel(LOG_ERROR);
    SetTargetFPS(60);

    InitWindow(TILE_SIZE * 1.5f * 9, TILE_SIZE * 1.5f * 9, "raylib Minesweeper");
    GameInit();
    while (!WindowShouldClose()) {
        GameDraw();
        GameUpdate();
    }
    GameDeinit();
    CloseWindow();

    return 0;
}
