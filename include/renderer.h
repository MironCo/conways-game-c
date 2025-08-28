#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include "grid.h"

#define CELL_SIZE 3

void renderer_DrawFromPoint(grid_t* t, int x, int y);

#endif