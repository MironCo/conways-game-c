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
            bool random_bool = rand() % 7 == 0;  // 10% chance (1 in 10)

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

void calculateNextState() {
    cell_t* cells = world_grid.aliveCells;
    cell_t* current_cell;

    for (current_cell = cells; current_cell != NULL; current_cell = current_cell->hh.next) {
        //determine number of alive neighbors of this alive cell

        /*
        Any live cell with fewer than two live neighbours dies, as if by underpopulation.
        Any live cell with two or three live neighbours lives on to the next generation.
        Any live cell with more than three live neighbours dies, as if by overpopulation.
        Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
        */

        //follow the rules above

        // do the same thing for each of the neighbors, follow the rules above. 
        // any cell past this point is not adjacent to the original alive cell

    }
}