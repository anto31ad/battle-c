#ifndef SESSION_H_
#define SESSION_H_

#include "entity.h"
#include "display.h"

bool is_ship_sunk(Ship*);
bool is_cell_empty(Grid*, int, int);
ShipID get_cell_occupant(Grid*, int, int);
Ship get_ship(ShipSet* shipset, ShipID ship_id);
bool is_cell_called(CallSet*, int, int);
void add_cell_call(CallSet*, int, int);

void init_session(Session*);
bool start_session(Session*);
void destroy_session(Session*);
void call_coords(Session*, int, int, DisplayUnit*);

#endif