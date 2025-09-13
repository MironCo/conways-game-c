#include "grid.h"

static grid_t world_grid = {0};

grid_t *getGrid(void)
{
    return &world_grid;
}

void grid_InitWorld() {
    world_grid.aliveCells = NULL;
    world_grid.nextGeneration = NULL;
    world_grid.generation = 0;
    pthread_mutex_init(&world_grid.lock, NULL);

    generateRandomState();
}

void generateRandomState() {
    srand(time(NULL));

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            // Generate random bool
            bool random_bool = rand() % 7 == 0; // 1 in 7 chance

            if (random_bool == true)
            {
                addCell(&world_grid.aliveCells, i, j);
            }
        }
    }

    // printAliveCells(world_grid.aliveCells);
}

void printAliveCells(cell_t *cells) {
    cell_t *current_cell;
    int count = 0;

    printf("Alive cells:\n");
    for (current_cell = cells; current_cell != NULL; current_cell = current_cell->hh.next)
    {
        printf("Cell at (%d, %d)\n", current_cell->x, current_cell->y);
        count++;
    }
    printf("Total alive cells: %d\n", count);
}

int getNumberOfAliveNeighbors(int x, int y) {
    int count = 0;

    for (int i = 0; i < 8; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        // bounds check
        if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE)
        {
            if (isCellAlive(&getGrid()->aliveCells, nx, ny))
                count++;
        }
    }

    return count;
}

void getDeadNeighborsForCell(cell_t **cells, int x, int y) {
    for (int i = 0; i < 8; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        // bounds check
        if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE)
        {
            // add the dead cell candidate
            if (!isCellAlive(&getGrid()->aliveCells, nx, ny) && !isCellAlive(&getGrid()->candidateDeadCells, nx, ny))
            {
                addCell(cells, nx, ny);
            }
        }
    }
}

void getDeadNeighborsForCellThreaded(cell_t **cells, cell_t **localCandidates, int x, int y) {
    for (int i = 0; i < 8; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        // bounds check
        if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE)
        {
            // add the dead cell candidate
            if (!isCellAlive(&getGrid()->aliveCells, nx, ny) && !isCellAlive(localCandidates, nx, ny))
            {
                addCell(cells, nx, ny);
            }
        }
    }
}

void wipeCurrentAliveCells() {
    cell_t *current, *tmp;
    HASH_ITER(hh, world_grid.aliveCells, current, tmp)
    {
        HASH_DEL(world_grid.aliveCells, current);
        free(current);
    }
}

void wipeNextGenerationAndCandidates() {
    cell_t *current, *tmp;

    if (world_grid.nextGeneration != NULL)
    {
        // Free nextGeneration
        HASH_ITER(hh, world_grid.nextGeneration, current, tmp)
        {
            HASH_DEL(world_grid.nextGeneration, current);
            free(current);
        }
    }

    if (world_grid.candidateDeadCells != NULL)
    {
        HASH_ITER(hh, world_grid.candidateDeadCells, current, tmp)
        {
            HASH_DEL(world_grid.candidateDeadCells, current);
            free(current);
        }
    }

    world_grid.nextGeneration = NULL;
    world_grid.candidateDeadCells = NULL;
}

bool determineFateOfLivingCell(cell_t *currentCell) {
    /*
    Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    Any live cell with two or three live neighbours lives on to the next generation.
    Any live cell with more than three live neighbours dies, as if by overpopulation.
    Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
    */

    // determine number of alive neighbors of this alive cell, follow the rules above
    int numberOfNeighbors = getNumberOfAliveNeighbors(currentCell->x, currentCell->y);

    // if (numberOfNeighbors < 2); cell needs to die aka not be added to the hashmap.
    if (numberOfNeighbors == 2 || numberOfNeighbors == 3)
        return true;

    return false;
    // if (numberOfNeighbors > 3); cell needs to die aka not be added to the hasmap;
}

bool determineFateOfDeadCell(cell_t *currentCell) {
    // determine number of alive neighbors of this alive cell, follow the rules above
    int numberOfNeighbors = getNumberOfAliveNeighbors(currentCell->x, currentCell->y);

    if (numberOfNeighbors == 3)
        return true;
    return false;
}

// old calculateNextState
void calculateNextState() {
    wipeNextGenerationAndCandidates();
    cell_t *cells = world_grid.aliveCells;
    cell_t *current_cell;

    for (current_cell = cells; current_cell != NULL; current_cell = current_cell->hh.next) {
        if (determineFateOfLivingCell(current_cell) == true)
            addCell(&getGrid()->nextGeneration, current_cell->x, current_cell->y);

        // collect dead candidates into the big hashmap of candidates
        getDeadNeighborsForCell(&world_grid.candidateDeadCells, current_cell->x, current_cell->y);
    }

    // do the same thing for each of the dead candidates, follow rule #4 above.
    // any cell past this point is not adjacent to the original alive cell
    for (current_cell = world_grid.candidateDeadCells; current_cell != NULL; current_cell = current_cell->hh.next) {
        if (determineFateOfDeadCell(current_cell) == true)
            addCell(&getGrid()->nextGeneration, current_cell->x, current_cell->y);
    }

    // swap the pointers
    wipeCurrentAliveCells();
    world_grid.aliveCells = world_grid.nextGeneration;
    world_grid.nextGeneration = NULL;
}

void calculateNextStateMultithreaded() {
    wipeNextGenerationAndCandidates();
    
    // Count alive cells to divide work among threads
    int cellCount = HASH_COUNT(world_grid.aliveCells);
    
    // If there are very few cells, just use single threaded
    if (cellCount < 100) {
        calculateNextState();
        return;
    }
    
    int cellsPerThread = cellCount / 4;
    int remainder = cellCount % 4;
    
    pthread_t threads[4];
    gridThreadData_t threadData[4];
    
    // Initialize thread data
    for (int i = 0; i < 4; i++) {
        threadData[i].threadID = i;
        threadData[i].localNextGeneration = malloc(sizeof(cell_t*));
        threadData[i].localCandidateDeadCells = malloc(sizeof(cell_t*));
        *threadData[i].localNextGeneration = NULL;
        *threadData[i].localCandidateDeadCells = NULL;
        
        threadData[i].lowerBound = i * cellsPerThread;
        if (i == 3) {
            threadData[i].upperBound = (i + 1) * cellsPerThread + remainder - 1;
        } else {
            threadData[i].upperBound = (i + 1) * cellsPerThread - 1;
        }
    }
    
    // Create threads
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, calculateNextStateBounds, &threadData[i]);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Merge results from all threads
    for (int i = 0; i < 4; i++) {
        mergeLocalResultsToMain(&threadData[i]);
    }
    
    // Clean up thread-local data
    for (int i = 0; i < 4; i++) {
        cell_t *current, *tmp;
        if (*threadData[i].localNextGeneration != NULL) {
            HASH_ITER(hh, *threadData[i].localNextGeneration, current, tmp) {
                HASH_DEL(*threadData[i].localNextGeneration, current);
                free(current);
            }
        }
        if (*threadData[i].localCandidateDeadCells != NULL) {
            HASH_ITER(hh, *threadData[i].localCandidateDeadCells, current, tmp) {
                HASH_DEL(*threadData[i].localCandidateDeadCells, current);
                free(current);
            }
        }
        free(threadData[i].localNextGeneration);
        free(threadData[i].localCandidateDeadCells);
    }
    
    // swap the pointers
    wipeCurrentAliveCells();
    world_grid.aliveCells = world_grid.nextGeneration;
    world_grid.nextGeneration = NULL;
}


// for threading
// divide original hashmap to 4 parts. (number_items /4)
// get the remainder (number_items % 4)
// thread one gets "0-(x/4) inclusive."
// thread two gets "((x/4) + 1) - (2(x/4))"
// thread three gets "(2(x/4) + 1) - (3(x/4))"
// thread four gets "3((x/4)+1) - count"

void mergeLocalResultsToMain(gridThreadData_t* threadData) {
    cell_t *current_cell, *tmp;
    
    pthread_mutex_lock(&world_grid.lock);
    HASH_ITER(hh, *threadData->localNextGeneration, current_cell, tmp) {
        addCell(&world_grid.nextGeneration, current_cell->x, current_cell->y);
    }
    pthread_mutex_unlock(&world_grid.lock);
}

void* calculateNextStateBounds(void* args) {
    gridThreadData_t* threadData = (gridThreadData_t*)args;
    int lowerBound = threadData->lowerBound;
    int upperBound = threadData->upperBound;
    
    cell_t *cells = world_grid.aliveCells;
    cell_t *current_cell;
    int index = 0;
    
    for (current_cell = cells; current_cell != NULL && index <= upperBound; current_cell = current_cell->hh.next) {
        if (index >= lowerBound) {
            if (determineFateOfLivingCell(current_cell) == true) {
                addCell(threadData->localNextGeneration, current_cell->x, current_cell->y);
            }
            
            getDeadNeighborsForCellThreaded(threadData->localCandidateDeadCells, threadData->localCandidateDeadCells, current_cell->x, current_cell->y);
        }
        index++;
    }

    for (current_cell = *threadData->localCandidateDeadCells; current_cell != NULL; current_cell = current_cell->hh.next) {
        if (determineFateOfDeadCell(current_cell) == true) {
            addCell(threadData->localNextGeneration, current_cell->x, current_cell->y);
        }
    }
    
    return NULL;
}

