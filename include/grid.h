#ifndef GRID_H
#define GRID_H
#define GRID_SIZE 250
 
#include "common.h"
#include "cell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

static const int dx[8] = {-1, -1, -1,  0,  0,  1,  1,  1};
static const int dy[8] = {-1,  0,  1, -1,  1, -1,  0,  1};

void grid_InitWorld();
void generateRandomState();
void calculateNextState();
void calculateNextStateMultithreaded();
void mergeLocalResultsToMain(gridThreadData_t* threadData);
void* calculateNextStateBounds(void* args);
bool determineFateOfLivingCell(cell_t* currentCell);
bool determineFateOfDeadCell(cell_t* currentCell);
grid_t* getGrid(void);
int getNumberOfAliveNeighbors(int x, int y);
void getDeadNeighborsForCellThreaded(cell_t **cells, cell_t **localCandidates, int x, int y);
void printAliveCells(cell_t* cells);

#endif