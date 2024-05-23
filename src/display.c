#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/display.h"
#include "../include/session.h"

#define BUFFER_SIZE (100)
#define MAX_ONE_DIGIT (9)
#define CHAR_WATER '~'
#define CHAR_HIT 'X'
#define CHAR_UNKNOWN '*'
#define COLOR_WATER "\033[34m"
#define COLOR_BASE "\033[0m"
#define COLOR_SHIP "\033[31m"
#define COLOR_HIT "\033[33m"
#define COLOR_KNOWN "\033[32m"
#define STR_SPACE_LONG "   "
#define STR_SPACE_SHORT "  "
#define MSG_QUEUE_SIZE (4)

DisplayUnit* create_display() {
    DisplayUnit* display_ptr = (DisplayUnit*) malloc(sizeof(DisplayUnit));
    display_ptr->msg_queue = (char**) malloc(sizeof(char*) * MSG_QUEUE_SIZE);

    for (int i = 0; i < MSG_QUEUE_SIZE; i++) {
        display_ptr->msg_queue[i] = (char*) malloc(sizeof(char) * BUFFER_SIZE);
        display_ptr->msg_queue[i][0] = '\0';
    }
    
    display_ptr->buffer = (char*) malloc(sizeof(char) * BUFFER_SIZE);
    display_ptr->buffer[0] = '\0';

    display_ptr->msg_first = 0;
    display_ptr->msg_last = 0;

    display_ptr->grid_peek = false;

    return display_ptr;
}

void destroy_display(DisplayUnit* display_ptr) {
    if (display_ptr->msg_queue) {
        for (int i = 0; i < MSG_QUEUE_SIZE; i++) {
            free(display_ptr->msg_queue[i]);
        }
        free(display_ptr->msg_queue);
        display_ptr->msg_queue = NULL;
    }
    if (display_ptr->buffer) {
        free(display_ptr->buffer);
        display_ptr->buffer = NULL;
    }
    free(display_ptr);
}

void print_col_header(int grid_size) {
    int display_col;
    printf(" %s", STR_SPACE_LONG);
    for (int col = 0; col < grid_size; col++) {
        display_col = col+1;
        if (display_col > MAX_ONE_DIGIT) {
            printf("%d%s", display_col, STR_SPACE_SHORT);
        } else {
            printf("%d%s", display_col, STR_SPACE_LONG);
        }
    }
    printf("\n\n");
}

void print_grid(Session* session) {
    Grid* grid = &session->grid;
    ShipSet* shipset = &session->ships;
    CallSet* calls = &session->calls;
    int grid_size = grid->size;
    int display_row, display_col;
    unsigned int cur_COLOR_SHIP;

    printf("\n\n");
    print_col_header(grid_size);

    // ROWS
    for (int row = 0; row < grid_size; row++) {
        display_row = row+1;
        if (display_row > MAX_ONE_DIGIT) {
            printf("%d%s", display_row, STR_SPACE_SHORT);
        } else {
            printf("%d%s", display_row, STR_SPACE_LONG);
        }
        for (int col = 0; col < grid_size; col++) {
            display_col = col+1;
            if (is_cell_empty(grid, row, col)) {
                printf("%s%c", COLOR_WATER, CHAR_WATER);
            } else {
                if (is_cell_called(calls, row, col)) {
                    printf("%s%d",
                        COLOR_HIT,
                        get_cell_occupant(grid, row, col));
                } else {
                    printf("%s%d",
                        COLOR_SHIP,
                        get_cell_occupant(grid, row, col));
                }
            }
            //alignment
            if (display_col > MAX_ONE_DIGIT) {
                printf("%s", STR_SPACE_SHORT);
            } else {
                printf("%s", STR_SPACE_LONG);
            }
        }
        printf("\n\n%s", COLOR_BASE);
    }
}

void print_calls(Session* session) {
    int grid_size = session->grid.size;
    int display_row, display_col;

    printf("\n\n");
    print_col_header(grid_size);

    // ROWS
    for (int row = 0; row < grid_size; row++) {
        display_row = row+1;
        if (display_row > MAX_ONE_DIGIT) {
            printf("%d%s", display_row, STR_SPACE_SHORT);
        } else {
            printf("%d%s", display_row, STR_SPACE_LONG);
        }
        for (int col = 0; col < grid_size; col++) {
            display_col = col+1;
            if (is_cell_called(&session->calls, row, col)) {
                if (is_cell_empty(&session->grid, row, col)) {
                    printf("%s%c", COLOR_WATER, CHAR_WATER);
                } else {
                    printf("%s%c", COLOR_HIT, CHAR_HIT);
                }
            } else {
                printf("%s%c", COLOR_BASE, CHAR_UNKNOWN);
            }
            //alignment
            if (display_col > MAX_ONE_DIGIT) {
                printf("%s", STR_SPACE_SHORT);
            } else {
                printf("%s", STR_SPACE_LONG);
            }
        }
        printf("\n\n%s", COLOR_BASE);
    }
}

static inline bool no_messages(DisplayUnit* display_ptr) {
    return !(display_ptr->msg_first - display_ptr->msg_last);
}

void enqueue_buffered_message(DisplayUnit* display_ptr) {
    int* last_ptr = &display_ptr->msg_last;

    if(!strcmp(display_ptr->buffer, "")) {
        return;
    }
    strcpy(display_ptr->msg_queue[*last_ptr], display_ptr->buffer);
    *last_ptr = (*last_ptr + 1) % MSG_QUEUE_SIZE;

    display_ptr->buffer[0] = '\0';
}

void pop_message(DisplayUnit* display_ptr) {
    int* last_ptr = &display_ptr->msg_last;
    int* first_ptr = &display_ptr->msg_first;

    if (no_messages(display_ptr))
        return;
    
    printf("%s\n", display_ptr->msg_queue[*first_ptr]);
    
    *first_ptr = (*first_ptr + 1) % MSG_QUEUE_SIZE;
}

// static inline void flush_display() {
//     //\033[H move cursor upper left corner
//     //\033[2J clear entire screen or terminal window
//     //\033[3J" clear scrollback
//     printf("\033[2J\033[3J");
// }

void render_session_display(App* app_ptr) {
    DisplayUnit *display_ptr = app_ptr->display;

    if (display_ptr->grid_peek) {
        print_grid(app_ptr->session);
        display_ptr->grid_peek = false;
    }

    print_calls(app_ptr->session);

    while(!no_messages(display_ptr)) {
        pop_message(display_ptr);
    }
    printf("[battle-c] ");
}
