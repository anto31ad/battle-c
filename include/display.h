#ifndef DISPLAY_H
#define DISPLAY_H

#include "entity.h"

typedef struct {
    char* grid_buffer;
} DisplayUnit;

void init_display(DisplayUnit*, Session*);
void destroy_display(DisplayUnit*);
void display_message(DisplayUnit*, char*);
void display_hits(DisplayUnit*, Session*);
void display_coords_prompt(DisplayUnit*);
void display_start(DisplayUnit*);
void display_win(DisplayUnit*);
void display_quit(DisplayUnit*);

#endif