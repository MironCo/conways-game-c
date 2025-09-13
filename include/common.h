#ifndef COMMON_H
#define COMMON_H

#include "uthash.h"
#include <stdbool.h>
#include <pthread.h>

typedef struct {
    int x;
    int y;
    char coord_key[32];
    UT_hash_handle hh;
} cell_t;

// Thread datas
typedef struct {
    int threadID;
    int lowerBound;
    int upperBound;
    cell_t** localCandidateDeadCells;
    cell_t** localNextGeneration;
} gridThreadData_t;

typedef struct {
    cell_t* aliveCells;
    cell_t* nextGeneration;
    cell_t* candidateDeadCells;
    int generation;
    bool singleThreaded;
    bool updating;
    pthread_mutex_t lock;
} grid_t;

typedef struct {
    int x;
    int y;
    cell_t** cells;
} gridAddCellData_t;

#endif