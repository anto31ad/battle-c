#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/display.h"
#include "../include/session.h"

#define BUFFER_SIZE (100)
#define MAX_SINGLE_DIGIT_NUMBER (9)

const char WATER_CHAR = '~';
const char HIT_CHAR = 'X';
const char UNKNOWN_CHAR = '*';
const int MSG_QUEUE_SIZE = 4;
const char* LONG_WHITESPACE = "    ";
const char* SHORT_WHITESPACE = "   ";


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
    printf(" %s", LONG_WHITESPACE);
    for (int col = 0; col < grid_size; col++) {
        display_col = col+1;
        if (display_col > MAX_SINGLE_DIGIT_NUMBER) {
            printf("%d%s", display_col, SHORT_WHITESPACE);
        } else {
            printf("%d%s", display_col, LONG_WHITESPACE);
        }
    }
    printf("\n\n");
}

void print_grid(Grid* grid_ptr) {
    int grid_size = grid_ptr->size;
    int display_row, display_col;

    printf("\n\n");
    print_col_header(grid_size);

    // ROWS
    for (int row = 0; row < grid_size; row++) {
        display_row = row+1;
        if (display_row > MAX_SINGLE_DIGIT_NUMBER) {
            printf("%d%s", display_row, SHORT_WHITESPACE);
        } else {
            printf("%d%s", display_row, LONG_WHITESPACE);
        }
        for (int col = 0; col < grid_size; col++) {
            display_col = col+1;
            if (is_cell_empty(grid_ptr, row, col)) {
                printf("%c", WATER_CHAR);
            } else {
                printf("%d", get_cell_occupant(grid_ptr, row, col));
            }
            //alignment
            if (display_col > MAX_SINGLE_DIGIT_NUMBER) {
                printf("%s", SHORT_WHITESPACE);
            } else {
                printf("%s", LONG_WHITESPACE);
            }
        }
        printf("\n\n");
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
        if (display_row > MAX_SINGLE_DIGIT_NUMBER) {
            printf("%d%s", display_row, SHORT_WHITESPACE);
        } else {
            printf("%d%s", display_row, LONG_WHITESPACE);
        }
        for (int col = 0; col < grid_size; col++) {
            display_col = col+1;
            if (is_cell_called(&session->calls, row, col)) {
                if (is_cell_empty(&session->grid, row, col)) {
                    printf("%c", WATER_CHAR);
                } else {
                    printf("%c", HIT_CHAR);
                }
            } else {
                printf("%c", UNKNOWN_CHAR);
            }
            //alignment
            if (display_col > MAX_SINGLE_DIGIT_NUMBER) {
                printf("%s", SHORT_WHITESPACE);
            } else {
                printf("%s", LONG_WHITESPACE);
            }
        }
        printf("\n\n");
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

    print_calls(app_ptr->session);

    while(!no_messages(display_ptr)) {
        pop_message(display_ptr);
    }
    printf("[battle-c] ");
}
