#ifndef SESSION_H_
#define SESSION_H_

#include "entity.h"
#include "display.h"

void init_shipset(ShipSet*, int);
void destroy_shipset(ShipSet*);

void init_grid(Grid*, int);
void destroy_grid(Grid*);
void fill_grid(Grid*, ShipSet*);

void init_callset(CallSet*, int);
void destroy_callset(CallSet*);

void hit_ship(Ship*);
bool is_ship_sunk(Ship*);
bool is_cell_empty(Grid*, int, int);
ShipID get_cell_occupant(Grid*, int, int);
bool is_cell_called(CallSet*, int, int);
void add_cell_call(CallSet*, int, int);

#endif