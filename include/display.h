#ifndef DISPLAY_H
#define DISPLAY_H

#include "entity.h"

void init_display(DisplayUnit*, Session*);
void destroy_display(DisplayUnit*);
void update_display(App*);
bool push_message(DisplayUnit*, MessageType);

#endif