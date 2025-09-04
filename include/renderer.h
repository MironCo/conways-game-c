#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include "grid.h"

#define CELL_SIZE 5

void renderer_DrawFromPoint(grid_t* t, int x, int y);
void renderer_DrawViewport(grid_t* t, Camera2D camera, int screenWidth, int screenHeight);

#endif