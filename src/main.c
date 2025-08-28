#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"
#include "cell.h"
#include "raylib.h"
#include "grid.h"
#include "renderer.h"

int main(void) {   
    const int screenWidth = 1066;
    const int screenHeight = 600;
    
    double lastUpdateTime = 0.0;
    double updateInterval = 0.05f;    
    
    InitWindow(screenWidth, screenHeight, "Conway's Game Of Life In C");
    SetTargetFPS(60);

    grid_InitWorld();

    while (!WindowShouldClose()) {
        double currentTime = GetTime();
    
        if (currentTime - lastUpdateTime >= updateInterval) {
            calculateNextState();
            lastUpdateTime = currentTime;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        renderer_DrawFromPoint(getGrid(), 0, 0);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}