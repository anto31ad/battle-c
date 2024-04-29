#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/app.h"

static void handle_command(App* app_ptr, char* command) {
    InputUnit* input_ptr = &app_ptr->input;
    DisplayUnit* display_ptr = &app_ptr->display;
    Session* session_ptr = &app_ptr->session;

    bool read_successful = read_input(input_ptr);

    if (!read_successful) {
        push_message(display_ptr, ERR_INPUT_READ);
        return;
    }

    if (strcmp(command, "quit")) {
        push_message(display_ptr, QUIT);
    } else if (strcmp(command, "help")) {
        push_message(display_ptr, HELP);
    };
}

void request_quit(Session* session_ptr) {
    session_ptr->quit_requested = true;
}


void update(App* app_ptr) {
    


    update_display(app_ptr);

    
    
    

    char* command = get_buffered_command(input_ptr);

    if (strcmp(command, "") == 0) {
        printf("(!) Please type in a command\n");
        return;
    }

    int errors = 0;
    int row;
    int col;
    int coords_read = sscanf(command, "%d %d", &row, &col);

    if (coords_read == 2) {

        
        if (errors > 0) return;

        printf("row: %d, col: %d\n", row, col);
    } else {
        printf("Command: %s\n", command);
    }

    if (strcmp(command, "quit") == 0) {
        request_quit(session_ptr);
        return; 
    }
}

void init_app(App* app) {
    init_input(&app->input);
    init_session(&app->session);
    init_display(&app->display, &app->session);
}

void destroy_app(App* app) {
    destroy_session(&app->session);
    destroy_input(&app->input);
    destroy_display(&app->display);
}

bool app_should_close(App * app_ptr) {
    return app_ptr->session.quit_requested;
}

void run(App* app_ptr) {
    init_app(app_ptr);

    while (app_should_close(app_ptr))
        update(app_ptr);

    destroy_app(app_ptr);
}