#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/control.h"
#include "../include/constants.h"


static bool hit(ShipSet* ships, ShipID target_id) {
    Ship* ship_ptr = &ships->set[target_id];
    hit_ship(ship_ptr);
    return is_ship_sunk(ship_ptr);
}

static void init_game_ships(Session* session) {
    
    ShipSet* ships = &session->ships;

    if (ships != NULL) {
        destroy_shipset(ships);
    }

    init_shipset(ships, SHIPS_SIZE);

    Ship* ships_arr = ships->set;
    
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

bool is_game_won(Session* session) {
    return session->game_won;
}


static void fill_grid(Grid* grid) {
    return;
}


static void handle_command() {

}


void init_session(Session* session) {
    session->game_won = false;

    init_shipset(&session->ships, SHIPS_SIZE);
    session->ships_left = SHIPS_SIZE;
    
    Grid* grid_ptr = &session->grid;
    init_grid(grid_ptr, GRID_SIZE);
    fill_grid(grid_ptr);

    init_callset(&session->calls, GRID_SIZE);

    return;
}


void destroy_session(Session* session) {
    destroy_shipset(&session->ships);
    destroy_grid(&session->grid);
    destroy_callset(&session->calls);
}


bool validate_row(Session* session, int row) {
    return row > 0 && row < session->grid.size; 
}

bool validate_col(Session* session, int col) {
    return col > 0 && col < session->grid.size; 
}


void strike(Session* session, int row, int col) {
    CallSet* calls_ptr = &session->calls;

    if (is_cell_called(calls_ptr, row, col)) {
        return;
    }

    Grid* grid_ptr = &session->grid;
    if (is_cell_empty(grid_ptr, row, col)) {
        add_cell_call(calls_ptr, row, col);
        return;
    }

    ShipID ship_id = get_cell_occupant(grid_ptr, row, col);
    bool is_sunk = hit(&session->ships, ship_id);
    
    if (is_sunk) {
        session->ships_left--;
    }
}

void update(App* app) {
    
    InputUnit* input = &app->input;
    DisplayUnit* display = &app->display;
    Session* session = &app->session;

    //display_hits(display, session);

    display_coords_prompt(display);
    char* next_cmd = read_next_command(input);
    
    if (next_cmd == NULL) {
        printf("(!) Error in reading the command\n");
        return;
    }

    if (strcmp(next_cmd, "") == 0) {
        printf("(!) Please type in a command\n");
        return;
    }

    int errors = 0;
    int row;
    int col;
    int coords_read = sscanf(next_cmd, "%d %d", &row, &col);

    if (coords_read == 2) {

        if (!validate_row(session, row)) {
            printf("(!) row %d is out of bounds\n", row);
            errors++;
        }

        if (!validate_col(session, col)) {
            printf("(!) col %d is out of bounds\n", col);
            errors++;
        }

        if (errors > 0) return;

        printf("row: %d, col: %d\n", row, col);
    } else {
        printf("Command: %s\n", next_cmd);
    }


    if (strcmp(next_cmd, "quit") == 0) {
        app->quit_requested = true;
        return; 
    }

    fflush(stdout);
}

void init_app(App* app) {
    init_input(&app->input);
    init_session(&app->session);
    init_display(&app->display, &app->session);
}

void destroy_app(App* app) {
    destroy_session(&app->session);
    destroy_input(&app->input);
    destroy_display(&app->display);
}

void request_quit(App* app) {
    app->quit_requested = true;
}

bool app_should_close(App* app) {
    return app->quit_requested;
}

