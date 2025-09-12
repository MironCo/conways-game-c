#include "cell.h"


void createKey(char* dest, int x, int y) {
    snprintf(dest, 32, "%d, %d", x, y);
}

void addCell(cell_t** cells, int x, int y) {
    cell_t* newCell = malloc(sizeof(cell_t));
    newCell->x = x;
    newCell->y = y;
    createKey(newCell->coord_key, x, y);
    HASH_ADD_STR(*cells, coord_key, newCell);   
}

void removeCell(cell_t** cells, int x, int y) {
    char keyToFind[32];
    createKey(keyToFind, x, y);

    cell_t* foundCell;

    HASH_FIND_STR(*cells, keyToFind, foundCell);

    if (foundCell != NULL) {
        HASH_DEL(*cells, foundCell);
        free(foundCell);
    }
}

bool isCellAlive(cell_t** cells, int x, int y) {
    char keyToFind[32];
    createKey(keyToFind, x, y);

    cell_t* foundCell;

    HASH_FIND_STR(*cells, keyToFind, foundCell);
    return foundCell != NULL;
}