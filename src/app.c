#include <stdio.h>
#include <string.h>

#include "../include/app.h"
#include "../include/session.h"

#define start_screen() puts("\033[?1049h\033[H")
#define exit_screen() puts("\033[?1049l")
#define CMD_CMP(s1, s2) (!strcmp((s1), (s2)))

bool handle_coords(Session* session_ptr, DisplayUnit* display_ptr, const char* coords_str) {
    int row, col;
    int coords_read = sscanf(coords_str, "%d %d", &row, &col);

    if (coords_read == 2) {
        call_coords(session_ptr, row, col, display_ptr);
        return true;
    }
    return false;
}

void handle_command(App* app_ptr, const char* command) {
    InputUnit* input_ptr = &app_ptr->input;
    DisplayUnit* display_ptr = &app_ptr->display;
    Session* session_ptr = &app_ptr->session;

    //strcmp returns 0 if equal, so !strcmp 
    if (CMD_CMP(command, "")) {
        sprintf(display_ptr->buffer, "(!) Please type in a command");
        enqueue_buffered_message(display_ptr);
        return;
    } else if (CMD_CMP(command, "quit")) {
        app_ptr->quit_requested = true;
        return;
    } else if (CMD_CMP(command, "help")) {
        sprintf(display_ptr->buffer, "help");
        enqueue_buffered_message(display_ptr);
        return;
    } else if (handle_coords(session_ptr, display_ptr, command)) {
        return;
    }
    sprintf(display_ptr->buffer, "(!) Unknown command");
    enqueue_buffered_message(display_ptr);
}

char* read_command(InputUnit* input_ptr, DisplayUnit* display_ptr) {
    if (!read_input(input_ptr)) {
        sprintf(display_ptr->buffer, "(!) error while reading input");
        enqueue_buffered_message(display_ptr);
        return NULL;
    }
    return get_buffered_command(input_ptr);
}

void update(App* app_ptr) {
    render_session_display(app_ptr);

    char* command = read_command(&app_ptr->input, &app_ptr->display);
    handle_command(app_ptr, command);
}

void init_app(App* app) {
    init_input(&app->input);
    init_session(&app->session);
    init_display(&app->display, &app->session);

    if(!start_session(&app->session)) {
        app->quit_requested = true;
        return;
    }
    app->quit_requested = false;
}

void destroy_app(App* app) {
    destroy_session(&app->session);
    destroy_input(&app->input);
    destroy_display(&app->display);
}

bool app_should_close(App * app_ptr) {
    return app_ptr->session.state == S_STATE_WIN || app_ptr->quit_requested;
}

void run(App* app_ptr) {
    init_app(app_ptr);

    //start_screen();

    while (!app_should_close(app_ptr))
        update(app_ptr);

    destroy_app(app_ptr);

    //exit_screen();
}
