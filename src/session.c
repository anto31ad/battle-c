#include <stdlib.h>
#include <string.h>

#include "../include/session.h"

#define NULL_COORD (-1)
#define NO_SHIP (-1)

const int GRID_SIZE = 10;
const int SHIPS_SIZE = 5;

const char SHIP_CARRIER_SIZE = 5;
const char SHIP_BATTLESHIP_SIZE = 4;
const char SHIP_CRUISER_SIZE = 3;
const char SHIP_SUBMARINE_SIZE = 3;
const char SHIP_DESTROYER_SIZE = 2;

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

    memset(grid->cells, NO_SHIP, tot_size * sizeof(ShipID));
}

void destroy_grid(Grid* grid) {
    free(grid->cells);
    grid->cells = NULL;
}

ShipID get_cell_occupant(Grid* grid, int row, int col) {
    return grid->cells[get_index(row, col, grid->size)];
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
    return calls->cells[get_index(row, col, calls->size)];
}

void add_cell_call(CallSet * calls, int row, int col) {
    calls->cells[get_index(row, col, calls->size)] = true;
}

static bool hit(ShipSet* shipset_ptr, ShipID target_id) {
    Ship* ship_ptr = &shipset_ptr->set[target_id];
    hit_ship(ship_ptr);
    return is_ship_sunk(ship_ptr);
}

static void init_game_ships(Session* session_ptr) {
    
    ShipSet* shipset_ptr = &session_ptr->ships;

    if (shipset_ptr != NULL) {
        destroy_shipset(shipset_ptr);
    }

    init_shipset(shipset_ptr, SHIPS_SIZE);

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

    return;
}

void fill_grid(Grid* grid_ptr, ShipSet* shipset_ptr) {
    return;
}

void init_session(Session* session) {
    session->game_won = false;

    init_shipset(&session->ships, SHIPS_SIZE);
    session->ships_left = SHIPS_SIZE;
    
    Grid* grid_ptr = &session->grid;
    init_grid(grid_ptr, GRID_SIZE);
    fill_grid(grid_ptr, &session->ships);

    init_callset(&session->calls, GRID_SIZE);

    return;
}

void destroy_session(Session* session) {
    destroy_shipset(&session->ships);
    destroy_grid(&session->grid);
    destroy_callset(&session->calls);
}

bool validate_row(Session* session, int row) {
    return row > 0 && row <= session->grid.size; 
}

bool validate_col(Session* session, int col) {
    return col > 0 && col <= session->grid.size; 
}

bool validate_coords(Session* session_ptr, int row, int col, DisplayUnit* display_ptr) {
    int errors = 0;
    
    if (!validate_row(session_ptr, row)) {
        push_message(display_ptr, MSG_ROW_OUT_OF_BOUNDS);
        errors++;
    }
    if (!validate_col(session_ptr, col)) {
        push_message(display_ptr, MSG_COL_OUT_OF_BOUNDS);
        errors++;
    }
    return !errors;
}

void call_coords(Session* session_ptr, int row, int col, DisplayUnit* display_ptr) {
    CallSet* calls_ptr = &session_ptr->calls;


    if (!validate_coords(session_ptr, row, col, display_ptr)) {
        session_ptr->last_called_row = NULL_COORD;
        session_ptr->last_called_col = NULL_COORD;
        return;
    }
    
    row--; col--; //offset

    session_ptr->last_called_row = row;
    session_ptr->last_called_col = col;

    if (is_cell_called(calls_ptr, row, col)) {
        push_message(display_ptr, MSG_COORDS_ALREADY_CALLED);
        return;
    }

    Grid* grid_ptr = &session_ptr->grid;

    add_cell_call(calls_ptr, row, col);

    if (is_cell_empty(grid_ptr, row, col)) {
        push_message(display_ptr, MSG_WATER);
        return;
    }

    push_message(display_ptr, MSG_HIT);

    ShipID ship_id = get_cell_occupant(grid_ptr, row, col);
    bool is_sunk = hit(&session_ptr->ships, ship_id);
    
    if (is_sunk) {
        session_ptr->ships_left--;
        push_message(display_ptr, MSG_SUNK);

        if (session_ptr->ships_left <= 0) {
            session_ptr->game_won = true;
            push_message(display_ptr, MSG_GAME_WON);
        }
    }
}
