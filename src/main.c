#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"
#include "cell.h"
#include "raylib.h"
#include "renderer.h"
#include "threader.h"

int main(void) {
    const int screenWidth = 1066;
    const int screenHeight = 600;

    double lastUpdateTime = 0.0;
    double updateInterval = 0.25f;
    bool isRunning = true;

    InitWindow(screenWidth, screenHeight, "Conway's Game Of Life In C");
    SetTargetFPS(60);

    Camera2D camera = { 0 };
    camera.target = (Vector2){ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
    camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    grid_InitWorld();
    setSingleThreaded(false);

    while (!WindowShouldClose()) {
        double currentTime = GetTime();

        // Toggle simulation with spacebar
        if (IsKeyPressed(KEY_SPACE)) {
            isRunning = !isRunning;
        }

        // Camera controls with WASD
        float cameraSpeed = 200.0f / camera.zoom; // Adjust speed based on zoom
        if (IsKeyDown(KEY_D)) camera.target.x += cameraSpeed * GetFrameTime();
        if (IsKeyDown(KEY_A)) camera.target.x -= cameraSpeed * GetFrameTime();
        if (IsKeyDown(KEY_S)) camera.target.y += cameraSpeed * GetFrameTime();
        if (IsKeyDown(KEY_W)) camera.target.y -= cameraSpeed * GetFrameTime();

        if (isRunning && currentTime - lastUpdateTime >= updateInterval) {
            threader_startCalculationIfReady();
            lastUpdateTime = currentTime;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);

        renderer_DrawViewport(getGrid(), camera, screenWidth, screenHeight);

        EndMode2D();

        // UI Elements in bottom left
        int uiY = screenHeight - 70;
        DrawRectangle(5, uiY, 200, 65, WHITE);
        DrawRectangleLines(5, uiY, 200, 65, BLACK);

        grid_t* grid = getGrid();
        char generationText[64];
        snprintf(generationText, sizeof(generationText), "Generation: %d", grid->generation);
        DrawText(generationText, 10, uiY + 5, 16, BLACK);

        const char* statusText = isRunning ? "Running (SPACE to pause)" : "Paused (SPACE to start)";
        Color statusColor = isRunning ? GREEN : RED;
        DrawText(statusText, 10, uiY + 25, 14, statusColor);

        DrawText("WASD: Pan camera", 10, uiY + 45, 12, GRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}