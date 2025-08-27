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