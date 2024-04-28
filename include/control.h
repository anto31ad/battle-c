#ifndef CONTROL_H
#define CONTROL_H

#include <stdbool.h>

#include "input.h"
#include "display.h"
#include "entity.h"

typedef struct {
    Session session;
    InputUnit input;
    DisplayUnit display;
    bool quit_requested;
} App;

void init_app(App*);
void destroy_app(App*);
void update(App*);
bool app_should_close(App*);
bool is_game_won(Session*);

#endif
