#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"
#include "cell.h"
#include "raylib.h"
#include "grid.h"
#include "renderer.h"

int main(void) {   
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "Conway's Game Of Life In C");
    SetTargetFPS(60);

    grid_InitWorld();

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        renderer_DrawFromPoint(getGrid(), 0, 0);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}