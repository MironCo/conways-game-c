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
    double updateInterval = 0.5f;    
    
    InitWindow(screenWidth, screenHeight, "Conway's Game Of Life In C");
    SetTargetFPS(60);

    Camera2D camera = { 0 };
    camera.target = (Vector2){ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
    camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    grid_InitWorld();

    while (!WindowShouldClose()) {
        double currentTime = GetTime();

        // Camera controls with WASD
        float cameraSpeed = 200.0f / camera.zoom; // Adjust speed based on zoom
        if (IsKeyDown(KEY_D)) camera.target.x += cameraSpeed * GetFrameTime();
        if (IsKeyDown(KEY_A)) camera.target.x -= cameraSpeed * GetFrameTime();
        if (IsKeyDown(KEY_S)) camera.target.y += cameraSpeed * GetFrameTime();
        if (IsKeyDown(KEY_W)) camera.target.y -= cameraSpeed * GetFrameTime();
        
        if (currentTime - lastUpdateTime >= updateInterval) {
            calculateNextState();
            lastUpdateTime = currentTime;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);

        renderer_DrawViewport(getGrid(), camera, screenWidth, screenHeight);

        EndMode2D();
        
        // Optional: Show controls
        DrawText("WASD: Pan, Q/E: Zoom", 10, 10, 20, BLACK);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}