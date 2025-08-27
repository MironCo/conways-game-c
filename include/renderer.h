#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include "grid.h"

#define CELL_SIZE 10

void renderer_DrawFromPoint(grid_t* t, int x, int y);

#endif