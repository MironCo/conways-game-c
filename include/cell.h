#ifndef CELL_H
#define CELL_H

#include <uthash.h>
#include <stdbool.h>

typedef struct {
    int x;
    int y;
    char coord_key[32];
    UT_hash_handle hh;
} cell_t;

void createKey(char* dest, int x, int y);
void addCell(cell_t** cells, int x, int y);
void removeCell(cell_t** cells, int x, int y);
bool isCellAlive(cell_t**, int x, int y);

#endif