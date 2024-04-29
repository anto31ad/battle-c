#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>

const int GRID_SIZE = 10;
const int SHIPS_SIZE = 5;

const char SHIP_CARRIER_SIZE = 5;
const char SHIP_BATTLESHIP_SIZE = 4;
const char SHIP_CRUISER_SIZE = 3;
const char SHIP_SUBMARINE_SIZE = 3;
const char SHIP_DESTROYER_SIZE = 2;

typedef enum {
    NULL_MSG,
    ERR_INPUT_READ,
    QUIT,
    HELP,
    UNKNOWN_CMD,
    COORDS_OUT_OF_BOUNDS,
    COORDS_ALREADY_CALLED,
    WATER,
    SHIP_HIT,
    SHIP_SUNK,
    GAME_WON,
} MessageType;

typedef struct {
    int size;
    int damage;
} Ship;

typedef int ShipID;

typedef struct {
    int size;
    ShipID* cells;
} Grid;

typedef struct {
    int size;
    bool* cells;
} CallSet;

typedef struct {
    int size;
    Ship* set;
} ShipSet;

typedef struct {
    Grid grid;
    CallSet calls;
    ShipSet ships;
    int ships_left;
    int last_called_row;
    int last_called_col;
    bool game_won;
    bool quit_requested;
} Session;

typedef struct {
    char* buffer;
    MessageType* msg_queue;
    int msg_first;
    int msg_last;
} DisplayUnit;

typedef struct {
    char* buffer;
    int buffer_size_max_b;
} InputUnit;

typedef struct {
    Session session;
    InputUnit input;
    DisplayUnit display;
} App;


#endif