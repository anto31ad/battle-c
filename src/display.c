#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/display.h"
#include "../include/session.h"

const char WATER_CHAR = '~';
const char HIT_CHAR = 'X';
const char UNKNOWN_CHAR = '*';
const int MSG_QUEUE_SIZE = 4;
const char* WHITESPACE = "   ";

void init_display(DisplayUnit* display_ptr, Session* session) {
    int grid_size = session->grid.size;

    if (!display_ptr->msg_queue)
        free(display_ptr->msg_queue);
    
    display_ptr->msg_queue = (MessageType*) malloc(sizeof(MessageType) * MSG_QUEUE_SIZE);
    display_ptr->msg_first = 0;
    display_ptr->msg_last = 0;

}

void destroy_display(DisplayUnit* display_ptr) {
    if (!display_ptr->msg_queue) {
        free(display_ptr->msg_queue);
        display_ptr->msg_queue = NULL;
    }
}

void print_grid(Grid* grid_ptr) {
    int grid_size = grid_ptr->size;

    printf("\n\n");

    // COLUMS
    printf("%s", WHITESPACE);
    for (int col = 0; col < grid_size; col++) {
        printf("%d%s", col+1, WHITESPACE);
    }
    printf("\n\n");

    // ROWS
    for (int row = 0; row < grid_size; row++) {

        printf("%d%s", row+1, WHITESPACE);

        for (int col = 0; col < grid_size; col++) {
            if (is_cell_empty(grid_ptr, row, col)) {
                printf("%c%s", WATER_CHAR, WHITESPACE);
            } else {
                printf("%d%s",
                    get_cell_occupant(grid_ptr, row, col),
                    WHITESPACE);
            }
        }
        printf("\n\n");
    }
}

void print_calls(Session* session) {
    int grid_size = session->grid.size;

    printf("\n\n");

    // COLUMS
    printf("%s", WHITESPACE);
    for (int col = 0; col < grid_size; col++) {
        printf("%d%s", col+1, WHITESPACE);
    }
    printf("\n\n");

    // ROWS
    for (int row = 0; row < grid_size; row++) {

        printf("%d%s", row+1, WHITESPACE);

        for (int col = 0; col < grid_size; col++) {
            if (is_cell_called(&session->calls, row, col)) {
                if (is_cell_empty(&session->grid, row, col)) {
                    printf("%c%s", WATER_CHAR, WHITESPACE);
                } else {
                    printf("%c%s", HIT_CHAR, WHITESPACE);
                }
            } else {
                printf("%c%s", UNKNOWN_CHAR, WHITESPACE);
            }
        }
        printf("\n\n");
    }
}

static inline bool no_messages(DisplayUnit* display_ptr) {
    return !(display_ptr->msg_first - display_ptr->msg_last);
}

bool push_message(DisplayUnit* display_ptr, MessageType msg_type) {
    int* last_ptr = &display_ptr->msg_last;

    display_ptr->msg_queue[*last_ptr] = msg_type;
    *last_ptr = (*last_ptr + 1) % MSG_QUEUE_SIZE;

    return true;
}

MessageType pop_message(DisplayUnit* display_ptr) {
    int* last_ptr = &display_ptr->msg_last;
    int* first_ptr = &display_ptr->msg_first;

    if (no_messages(display_ptr))
        return MSG_NULL;
    
    MessageType popped_msg =  display_ptr->msg_queue[*first_ptr];
    
    *first_ptr = (*first_ptr + 1) % MSG_QUEUE_SIZE;
    return popped_msg;
}

void process_message(Session* session, MessageType msg_type) {
    switch (msg_type) {
        case MSG_UNKNOWN_CMD:
            printf("(!) unknown command");
            break;

        case MSG_ERR_INPUT_READ:
            printf("(!) error while reading the command");
            break;

        case MSG_EMPTY_CMD:
            printf("(!) please type in a command");
            break;

        case MSG_WATER:
            printf("Water!");
            break;

        case MSG_COORDS_ALREADY_CALLED:
            printf("(!) Coordinates (%d,%d) already called!",
                session->last_called_col + 1,
                session->last_called_row + 1);
            break;

        default:
            printf( "(!) unknown message type");
    }

    printf("\n\n");
}

static inline void flush_display() {
    //\033[H move cursor upper left corner
    //\033[2J clear entire screen or terminal window
    //\033[3J" clear scrollback
    printf("\033[2J\033[3J");
}

void update_display(App* app_ptr) {
    DisplayUnit *display_ptr = &app_ptr->display;

    //flush_display();

    print_calls(&app_ptr->session);

    while(!no_messages(display_ptr)) {
        MessageType cur_msg = pop_message(display_ptr);
        process_message(&app_ptr->session, cur_msg);
    }
    printf("[battle-c] ");
}
