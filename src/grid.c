#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "grid.h"
#include "cell.h"

static grid_t world_grid = {0};


grid_t* getGrid(void) {
    return &world_grid;
}


void grid_InitWorld() {
    world_grid.aliveCells = NULL;
    world_grid.nextGeneration = NULL;
    world_grid.generation = 0;

    generateRandomState();
}

void generateRandomState() {
    srand(time(NULL));
    
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            // Generate random bool
            bool random_bool = rand() % 5 == 0;  // 20% chance (1 in 5)

            if (random_bool == true) {
                addCell(&world_grid.aliveCells, i, j);
            }
        }
    }

    //printAliveCells(world_grid.aliveCells);
}

void printAliveCells(cell_t* cells) {
    cell_t* current_cell;
    int count = 0;
    
    printf("Alive cells:\n");
    for (current_cell = cells; current_cell != NULL; current_cell = current_cell->hh.next) {
        printf("Cell at (%d, %d)\n", current_cell->x, current_cell->y);
        count++;
    }
    printf("Total alive cells: %d\n", count);
}

int getNumberOfAliveNeighbors(int x, int y) {
    int count = 0;
    
    for(int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
    
        // bounds check
        if(nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE) {
            if (isCellAlive(&getGrid()->aliveCells, nx, ny)) count++;
        }
    }

    return count;
}

void getDeadNeighborsForCell(cell_t** cells, int x, int y) {
    for(int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
    
        // bounds check
        if(nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE) {
            // add the dead cell candidate
            if (!isCellAlive(&getGrid()->aliveCells, nx, ny) && !isCellAlive(&getGrid()->candidateDeadCells, nx, ny)) {
                addCell(cells, nx, ny);
            }
        }
    }
}


void wipeCurrentAliveCells() {
    cell_t *current, *tmp;
    HASH_ITER(hh, world_grid.aliveCells, current, tmp) {
        HASH_DEL(world_grid.aliveCells, current);
        free(current);
    }
}

void wipeNextGenerationAndCandidates() {
    cell_t *current, *tmp;

    if (world_grid.nextGeneration != NULL) {
        // Free nextGeneration
        HASH_ITER(hh, world_grid.nextGeneration, current, tmp) {
            HASH_DEL(world_grid.nextGeneration, current);
            free(current);
        }
    }

    if (world_grid.candidateDeadCells != NULL) {
        HASH_ITER(hh, world_grid.candidateDeadCells, current, tmp) {
            HASH_DEL(world_grid.candidateDeadCells, current);
            free(current);
        }
    }

    world_grid.nextGeneration = NULL;
    world_grid.candidateDeadCells = NULL;
}

void calculateNextState() {
    wipeNextGenerationAndCandidates();
    cell_t* cells = world_grid.aliveCells;
    cell_t* current_cell;

    for (current_cell = cells; current_cell != NULL; current_cell = current_cell->hh.next) {
        /*
        Any live cell with fewer than two live neighbours dies, as if by underpopulation.
        Any live cell with two or three live neighbours lives on to the next generation.
        Any live cell with more than three live neighbours dies, as if by overpopulation.
        Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
        */

        //determine number of alive neighbors of this alive cell, follow the rules above
        int numberOfNeighbors = getNumberOfAliveNeighbors(current_cell->x, current_cell->y);

        //if (numberOfNeighbors < 2); cell needs to die aka not be added to the hashmap.
        if (numberOfNeighbors == 2 || numberOfNeighbors == 3) addCell(&getGrid()->nextGeneration, current_cell->x, current_cell->y);
        // if (numberOfNeighbors > 3); cell needs to die aka not be added to the hasmap;

        //collect dead candidates into the big hashmap of candidates
        getDeadNeighborsForCell(&world_grid.candidateDeadCells, current_cell->x, current_cell->y);
    }


    // do the same thing for each of the dead candidates, follow rule #4 above. 
    // any cell past this point is not adjacent to the original alive cell
     for (current_cell = world_grid.candidateDeadCells; current_cell != NULL; current_cell = current_cell->hh.next) {
        //determine number of alive neighbors of this alive cell, follow the rules above
        int numberOfNeighbors = getNumberOfAliveNeighbors(current_cell->x, current_cell->y);

        if (numberOfNeighbors == 3) addCell(&getGrid()->nextGeneration, current_cell->x, current_cell->y);
    }

    //swap the pointers
    wipeCurrentAliveCells();
    world_grid.aliveCells = world_grid.nextGeneration;
    world_grid.nextGeneration = NULL;
}
