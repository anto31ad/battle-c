#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>

typedef struct {
    unsigned int size;
    unsigned int damage;
    char name;
} Ship;

typedef int ShipID;

typedef struct {
    unsigned int size;
    ShipID* cells;
} Grid;

typedef struct {
    unsigned int size;
    bool* cells;
} CallSet;

typedef struct {
    unsigned int size;
    Ship* set;
} ShipSet;

enum SessionState {
    S_STATE_PEEK,
    S_STATE_LOOP,
    S_STATE_WIN,
    S_STATE_EXIT,
};

typedef struct {
    Grid grid;
    CallSet calls;
    ShipSet ships;
    int ships_left;
    enum SessionState state;
} Session;

typedef struct {
    char** msg_queue;
    int msg_first;
    int msg_last;
    char* buffer;
    bool grid_peek;
    char** ship_color;
} DisplayUnit;

typedef struct {
    char* buffer;
    int buffer_size_max_b;
} InputUnit;

typedef struct {
    Session* session;
    InputUnit* input;
    DisplayUnit* display;
    bool quit_requested;
} App;


#endif