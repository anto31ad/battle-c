#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>

typedef enum {
    MSG_NULL,
    MSG_ERR_INPUT_READ,
    MSG_QUIT,
    MSG_HELP,
    MSG_UNKNOWN_CMD,
    MSG_EMPTY_CMD,
    MSG_ROW_OUT_OF_BOUNDS,
    MSG_COL_OUT_OF_BOUNDS,
    MSG_COORDS_ALREADY_CALLED,
    MSG_WATER,
    MSG_HIT,
    MSG_SUNK,
    MSG_GAME_WON,
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
} Session;

typedef struct {
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
    bool quit_requested;
} App;


#endif