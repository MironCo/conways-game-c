#ifndef CELL_H
#define CELL_H

#include "common.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void createKey(char* dest, int x, int y);
void addCell(cell_t** cells, int x, int y);
void removeCell(cell_t** cells, int x, int y);
bool isCellAlive(cell_t**, int x, int y);

#endif