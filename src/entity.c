#include <stdlib.h>

#include "../include/entity.h"

int get_index(int row, int col, int size) {
    return row * size + col;
}

void hit_ship(Ship* ship) {
    ship->damage++;
}

bool is_ship_sunk(Ship* ship) {
    return ship->damage >= ship->size;
}

void init_shipset(ShipSet* shipset, int size) {
    shipset->size = size;
    shipset->set = (Ship*) malloc(sizeof(Ship) * size);
}

void destroy_shipset(ShipSet* shipset) {
    free(shipset->set);
    shipset->set = NULL;
}

void init_grid(Grid* grid, int size) {
    int tot_size =  size * size;
    grid->cells = (ShipID*) malloc(sizeof(ShipID) * tot_size);
    grid->size = size;

    for (int i = 0; i < tot_size; i++) {
        grid->cells[i] = -1;
    }
}

void destroy_grid(Grid* grid) {
    free(grid->cells);
    grid->cells = NULL;
}

bool is_cell_empty(Grid* grid, int row, int col) {
    return get_cell_occupant(grid, row, col) < 0;
}

ShipID get_cell_occupant(Grid* grid, int row, int col) {
    return grid->cells[get_index(row, col, grid->size)];
}

void init_callset(CallSet* calls, int size) {
    int tot_size = size * size;
    calls->cells = (bool*) malloc(sizeof(bool) * tot_size);
    calls->size = size;

    for (int i = 0; i < tot_size; i++) {
        calls->cells[i] = false;
    }
}

void destroy_callset(CallSet* calls) {
    free(calls->cells);
    calls->cells = NULL;
}

bool is_cell_called(CallSet* calls, int row, int col) {
    return calls->cells[get_index(row, col, calls->size)];
}

void add_cell_call(CallSet * calls, int row, int col) {
    calls->cells[get_index(row, col, calls->size)] = true;
}
