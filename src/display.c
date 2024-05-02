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
    char** buffer = &display_ptr->buffer;
    int grid_size = session->grid.size;

    if (!*buffer)
        free(*buffer);

    //TODO fitting size and separate buffers
    int buffer_size = (grid_size + 1) * (grid_size + 1) * 5;
    *buffer = (char*) malloc(sizeof(char) * buffer_size);

    if (!display_ptr->msg_queue)
        free(display_ptr->msg_queue);
    
    display_ptr->msg_queue = (MessageType*) malloc(sizeof(MessageType) * MSG_QUEUE_SIZE);
    display_ptr->msg_first = 0;
    display_ptr->msg_last = 0;

}

void destroy_display(DisplayUnit* display_ptr) {
    if (display_ptr->buffer != NULL) {
        free(display_ptr->buffer);
        display_ptr->buffer = NULL;
    }
}

void buffer_flush(char* buffer) {
    *buffer = '\0';
}

void generate_hits_grid(char* buffer, Session* session) {
    int grid_size = session->grid.size;

    strcat(buffer, "\n\n");

    // COLUMS
    strcat(buffer, WHITESPACE);
    for (int col = 0; col < grid_size; col++) {
        sprintf(buffer, "%s%d%s", buffer, col+1, WHITESPACE);
    }
    strcat(buffer, "\n\n");

    // ROWS
    for (int row = 0; row < grid_size; row++) {

        sprintf(buffer, "%s%d%s", buffer, row+1, WHITESPACE);

        for (int col = 0; col < grid_size; col++) {
            if (is_cell_called(&session->calls, row, col)) {
                if (is_cell_empty(&session->grid, row, col)) {
                    sprintf(buffer, "%s%c%s", buffer, WATER_CHAR, WHITESPACE);
                } else {
                    sprintf(buffer, "%s%c%s", buffer, HIT_CHAR, WHITESPACE);
                }
            } else {
                sprintf(buffer, "%s%c%s", buffer, UNKNOWN_CHAR, WHITESPACE);
            }
        }

        strcat(buffer, "\n\n");
    }

   
}

bool no_messages(DisplayUnit* display_ptr) {
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

void process_message(Session* session, char* buffer, MessageType msg_type) {
 
    switch (msg_type) {
        case MSG_UNKNOWN_CMD:
            strcat(buffer, "(!) unknown command");
            break;

        case MSG_ERR_INPUT_READ:
            strcat(buffer, "(!) error while reading the command");
            break;

        case MSG_EMPTY_CMD:
            strcat(buffer, "(!) please type in a command");
            break;

        case MSG_WATER:
            strcat(buffer, "Water!");
            break;

        case MSG_COORDS_ALREADY_CALLED:
            sprintf(buffer,
                "%s(%d,%d) already called!",
                buffer,
                session->last_called_col,
                session->last_called_row);
            break;

        default:
            strcat(buffer, "(!) unknown message type");
    }

    strcat(buffer, "\n");
}

int count_buffered_lines(DisplayUnit* display_ptr) {
    char** buffer_ptr = &display_ptr->buffer;

    int lines = 0;

    for (const char *ptr = *buffer_ptr; *ptr != '\0'; ptr++) {
        if (*ptr == '\n') {
            lines++;
        }
    }

    if ((*buffer_ptr)[0] != '\0' /*&& (*buffer_ptr)[strlen(*buffer_ptr) - 1] != '\n'*/) {
        lines++;
    }
    return lines;
}

void flush_display(DisplayUnit* display_ptr) {
    //int* lines_num_ptr = &display_ptr->print_lines_num;

    char cursor_buffer[8];  //TODO check correct size
    
    //move cursor some lines up
    sprintf(cursor_buffer, "\033[%dA", count_buffered_lines(display_ptr));
    printf("%s", cursor_buffer);

    //clear and move to beginning
    //printf("\033[2K");

    //\033[H move cursor upper left corner
    //\033[2J clear entire screen or terminal window
    //\033[3J" clear scrollback
    printf("\033[2J\033[3J");
}

void update_display(App* app_ptr) {
    
    DisplayUnit *display_ptr = &app_ptr->display;

    flush_display(display_ptr);
    
    char* buffer = display_ptr->buffer;
    buffer_flush(buffer);
    
    generate_hits_grid(buffer, &app_ptr->session);

    while(!no_messages(display_ptr)) {
        MessageType cur_msg = pop_message(display_ptr);
        process_message(&app_ptr->session, buffer, cur_msg);
    }

    strcat(buffer, "[battle-c] ");
    printf("%s", buffer);
}
