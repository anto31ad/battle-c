#ifndef DISPLAY_H
#define DISPLAY_H

#include "entity.h"

void init_display(DisplayUnit*, Session*);
void destroy_display(DisplayUnit*);
void render_session_display(App*);
void enqueue_buffered_message(DisplayUnit*);

#endif