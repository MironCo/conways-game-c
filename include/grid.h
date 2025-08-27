#ifndef GRID_H
#define GRID_H
#define GRID_SIZE 100

#include "cell.h"

typedef struct {
    cell_t* aliveCells;
    cell_t* nextGeneration;
    int generation;
} grid_t;

void grid_InitWorld();
void generateRandomState();
void calculateNextState();
grid_t* getGrid(void);
void printAliveCells(cell_t* cells);

#endif