#include "renderer.h"
#include "grid.h"

void renderer_DrawFromPoint(grid_t* t, int x, int y) {
    cell_t* current_cell;
    
    for (current_cell = t->aliveCells; current_cell != NULL; current_cell = current_cell->hh.next) {
        int drawX = current_cell->x * CELL_SIZE;
        int drawY = current_cell->y * CELL_SIZE;
        
        DrawRectangle(drawX, drawY, CELL_SIZE, CELL_SIZE, BLACK);
    }
}

void renderer_DrawViewport(grid_t* t, Camera2D camera, int screenWidth, int screenHeight) {
    cell_t* current_cell;
    
    for (current_cell = t->aliveCells; current_cell != NULL; current_cell = current_cell->hh.next) {
        int drawX = current_cell->x * CELL_SIZE;
        int drawY = current_cell->y * CELL_SIZE;
        
        // Convert to screen coordinates to check if visible
        Vector2 screenPos = GetWorldToScreen2D((Vector2){drawX, drawY}, camera);
        
        // Only draw if rectangle would be visible on screen (with some padding)
        if (screenPos.x > -CELL_SIZE && screenPos.x < screenWidth + CELL_SIZE &&
            screenPos.y > -CELL_SIZE && screenPos.y < screenHeight + CELL_SIZE) {
            
            DrawRectangle(drawX, drawY, CELL_SIZE, CELL_SIZE, BLACK);
        }
    }
}