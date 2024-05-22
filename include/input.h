#ifndef INPUT_H
#define INPUT_H

#include "entity.h"

bool read_input(InputUnit*);
InputUnit* create_input();
void destroy_input(InputUnit*);
char* get_buffered_command(InputUnit*);

#endif