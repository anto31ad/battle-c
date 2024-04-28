#ifndef ENTITY_H
#define ENTITY_H


#include <stdbool.h>


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
    bool game_won;
} Session;


void init_shipset(ShipSet*, int);
void destroy_shipset(ShipSet*);

void init_grid(Grid*, int);
void destroy_grid(Grid*);

void init_callset(CallSet*, int);
void destroy_callset(CallSet*);

void hit_ship(Ship*);
bool is_ship_sunk(Ship*);
bool is_cell_empty(Grid*, int, int);
ShipID get_cell_occupant(Grid*, int, int);
bool is_cell_called(CallSet*, int, int);
void add_cell_call(CallSet*, int, int);

#endif