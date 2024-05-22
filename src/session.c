#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "../include/session.h"

#define NULL_COORD (-1)
#define NO_SHIP (-1)
#define GET_GRID_INDEX(row, col, grid_size) ((row) * (grid_size) + (col))

const int GRID_SIZE = 10;
const int SHIPS_SIZE = 5;

const char SHIP_CARRIER_SIZE = 5;
const char SHIP_BATTLESHIP_SIZE = 4;
const char SHIP_CRUISER_SIZE = 3;
const char SHIP_SUBMARINE_SIZE = 3;
const char SHIP_DESTROYER_SIZE = 2;

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

    memset(grid->cells, NO_SHIP, tot_size * sizeof(ShipID));
}

void destroy_grid(Grid* grid) {
    free(grid->cells);
    grid->cells = NULL;
}

ShipID get_cell_occupant(Grid* grid, int row, int col) {
    return grid->cells[GET_GRID_INDEX(row, col, grid->size)];
}

Ship get_ship(ShipSet *shipset, ShipID ship_id) {
    return shipset->set[ship_id];
}

bool is_cell_empty(Grid* grid, int row, int col) {
    return get_cell_occupant(grid, row, col) < 0;
}

void init_callset(CallSet* calls, int size) {
    int tot_size = size * size;
    calls->cells = (bool*) malloc(sizeof(bool) * tot_size);
    calls->size = size;

    memset(calls->cells, false, tot_size * sizeof(bool));
}

void destroy_callset(CallSet* calls) {
    free(calls->cells);
    calls->cells = NULL;
}

bool is_cell_called(CallSet* calls, int row, int col) {
    return calls->cells[GET_GRID_INDEX(row, col, calls->size)];
}

void add_cell_call(CallSet * calls, int row, int col) {
    calls->cells[GET_GRID_INDEX(row, col, calls->size)] = true;
}

static bool hit(ShipSet* shipset_ptr, ShipID target_id) {
    Ship* ship_ptr = &shipset_ptr->set[target_id];
    hit_ship(ship_ptr);
    return is_ship_sunk(ship_ptr);
}

static bool init_game_ships(Session* session_ptr) {
    
    ShipSet* shipset_ptr = &session_ptr->ships;

    if (shipset_ptr != NULL) {
        destroy_shipset(shipset_ptr);
    }

    init_shipset(shipset_ptr, SHIPS_SIZE);

    shipset_ptr->size = 5;
    Ship* ships_arr = shipset_ptr->set;
    
    ships_arr[0].size = SHIP_CARRIER_SIZE;
    ships_arr[0].damage = 0;

    ships_arr[1].size = SHIP_BATTLESHIP_SIZE;
    ships_arr[1].damage = 0;

    ships_arr[2].size = SHIP_CRUISER_SIZE;
    ships_arr[2].damage = 0;

    ships_arr[3].size = SHIP_SUBMARINE_SIZE;
    ships_arr[3].damage = 0;

    ships_arr[4].size = SHIP_DESTROYER_SIZE;
    ships_arr[4].damage = 0;

    return true;
}

bool out_of_bounds(Grid* grid_ptr, int index) {
    return index >= 0 && index < grid_ptr->size; 
}

bool validate_rowcol(Grid* grid_ptr, int index) {
    return index > 0 && index <= grid_ptr->size; 
}

bool cells_available(Grid* grid_ptr, int row, int col, int shipsize, bool vert) {
    int max_index = grid_ptr->size;
    if (vert) {
        if (max_index - shipsize - row < 0)
            return false;

        for (int i = 0; i < shipsize; i++) {
            if (!is_cell_empty(grid_ptr, row+i, col))
                return false; 
        }
        return true;
    }
    //horizontal
    if (max_index - shipsize - col < 0)
        return false;

    for (int i = 0; i < shipsize; i++) {
        if (!is_cell_empty(grid_ptr, row, col+i))
            return false; 
    }
    return true;
}


bool fill_grid(Grid* grid_ptr, ShipSet* shipset_ptr) {
    for (int cur_id = 0; cur_id < shipset_ptr->size; cur_id++) {
        Ship* cur_ship = &shipset_ptr->set[cur_id];

        int row, col, vert_dir; 
        for (int attempts = 0; ; attempts++) {
            row = rand() % grid_ptr->size;
            col = rand() % grid_ptr->size;
            vert_dir = rand() % 2;

            if (cells_available(grid_ptr, row, col, cur_ship->size, vert_dir)) {
                for (int i = 0; i < cur_ship->size; i++) {
                    grid_ptr->cells[
                        GET_GRID_INDEX(row + (i * vert_dir), col + (i * (1 - vert_dir)), grid_ptr->size)
                    ] = cur_id; 
                }
                break;
            }

            if (attempts > 100) {
                return false;
            }
        }        
    }
    return true;
}

Session* create_session() {
    Session* session = (Session*) malloc(sizeof(Session));

    srand(time(0));
    init_shipset(&session->ships, SHIPS_SIZE);
    session->ships_left = SHIPS_SIZE;

    init_callset(&session->calls, GRID_SIZE);

    Grid* grid_ptr = &session->grid;
    init_grid(grid_ptr, GRID_SIZE);
    session->state = S_STATE_LOOP;

    return session;
}

bool start_session(Session* session_ptr) {
    if (!init_game_ships(session_ptr))
        return false;

    return fill_grid(&session_ptr->grid, &session_ptr->ships);
}

void destroy_session(Session* session) {
    destroy_shipset(&session->ships);
    destroy_grid(&session->grid);
    destroy_callset(&session->calls);
    free(session);
}

void call_coords(Session* session_ptr, int row, int col, DisplayUnit* display_ptr) {
    Grid* grid_ptr = &session_ptr->grid;

    //validating coords
    bool valid_row = validate_rowcol(&session_ptr->grid, row);
    bool valid_col = validate_rowcol(&session_ptr->grid, col);
    
    if (!valid_row && !valid_col) {
        sprintf(display_ptr->buffer, "row %d and col %d are outside range (1, %d)", row, col, grid_ptr->size);
        enqueue_buffered_message(display_ptr);
        return;
    } else if (!valid_row) {
        sprintf(display_ptr->buffer, "row %d is outside range (1, %d)", row, grid_ptr->size);
        enqueue_buffered_message(display_ptr);
        return;
    } else if (!valid_col) {
        sprintf(display_ptr->buffer, "col %d is outside range (1, %d)", row, grid_ptr->size);
        enqueue_buffered_message(display_ptr);
        return;
    }
    //validation successful, proceed to check if the cell was previously revealed
    int row_index = row - 1;
    int col_index = col - 1;
    CallSet* calls_ptr = &session_ptr->calls;
    if (is_cell_called(calls_ptr, row_index, col_index)) {
        sprintf(display_ptr->buffer, "(!) Cell (%d,%d) has already been revealed", row, col);
        enqueue_buffered_message(display_ptr);
        return;
    }
    //making the attack
    add_cell_call(calls_ptr, row_index, col_index);

    if (is_cell_empty(grid_ptr, row_index, col_index)) {
        sprintf(display_ptr->buffer, "Water!");
        enqueue_buffered_message(display_ptr);
        return;
    }

    ShipID ship_id = get_cell_occupant(grid_ptr, row_index, col_index);
    bool is_sunk = hit(&session_ptr->ships, ship_id);
    
    if (!is_sunk) {
        sprintf(display_ptr->buffer, "Hit!");
        enqueue_buffered_message(display_ptr);
        return;
    }
    //if ship has been sunk...
    session_ptr->ships_left--;

    sprintf(display_ptr->buffer, "Hit and sunk!");
    enqueue_buffered_message(display_ptr);

    //win condition
    if (session_ptr->ships_left <= 0) {
        session_ptr->state = S_STATE_WIN;
        sprintf(display_ptr->buffer, "You Won!");
        enqueue_buffered_message(display_ptr);
    }
}
