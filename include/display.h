#ifndef DISPLAY_H
#define DISPLAY_H

#include "entity.h"

DisplayUnit* create_display();
void destroy_display(DisplayUnit*);
void render_session_display(App*);
void enqueue_buffered_message(DisplayUnit*);

#endif