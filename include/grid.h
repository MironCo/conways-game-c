#ifndef GRID_H
#define GRID_H
#define GRID_SIZE 250

#include "cell.h"
#include <stdbool.h>

static const int dx[8] = {-1, -1, -1,  0,  0,  1,  1,  1};
static const int dy[8] = {-1,  0,  1, -1,  1, -1,  0,  1};

typedef struct {
    cell_t* aliveCells;
    cell_t* nextGeneration;
    cell_t* candidateDeadCells;
    int generation;
} grid_t;

void grid_InitWorld();
void generateRandomState();
void calculateNextState();
bool determineFateOfLivingCell(cell_t* currentCell);
bool determineFateOfDeadCell(cell_t* currentCell);
grid_t* getGrid(void);
int getNumberOfAliveNeighbors(int x, int y);
void printAliveCells(cell_t* cells);

#endif