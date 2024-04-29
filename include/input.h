#ifndef INPUT_H
#define INPUT_H

#include "entity.h"

bool read_input(InputUnit*);
void init_input(InputUnit*);
void destroy_input(InputUnit*);
char* get_buffered_command(InputUnit*);

#endif