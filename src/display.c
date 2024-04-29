#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/display.h"
#include "../include/session.h"

const char WATER_CHAR = '~';
const char HIT_CHAR = 'X';
const char UNKNOWN_CHAR = '*';
const int MSG_QUEUE_SIZE = 4;

void init_display(DisplayUnit* display_ptr, Session* session) {
    char** buffer = &display_ptr->buffer;
    int grid_size = session->grid.size;

    if (!*buffer)
        free(*buffer);

    int buffer_size = (grid_size + 1) * (grid_size + 1) * 3;
    *buffer = (char*) malloc(sizeof(char) * buffer_size);

    if (!display_ptr->msg_queue)
        free(display_ptr->msg_queue);
    
    display_ptr->msg_queue = (MessageType*) malloc(sizeof(MessageType) * MSG_QUEUE_SIZE);
    display_ptr->msg_first = -1;
    display_ptr->msg_last = 0;

}

void destroy_display(DisplayUnit* display_ptr) {
    if (display_ptr->buffer != NULL) {
        free(display_ptr->buffer);
        display_ptr->buffer = NULL;
    }
}

void buffer_flush(char** buffer_ptr) {
    **buffer_ptr = '\0';
}

void buffer_append(char** buffer_ptr, const char* str) {
    sprintf(*buffer_ptr, "%s\n\n", *buffer_ptr);
}

void generate_hits_grid(char** buffer_ptr, Session* session) {
    int grid_size = session->grid.size;

    buffer_flush(buffer_ptr);
    sprintf(*buffer_ptr, "%s\n\n", *buffer_ptr);

    // COLUMS
    sprintf(*buffer_ptr, "%s\t", *buffer_ptr);
    for (int col = 0; col < grid_size; col++) {
        sprintf(*buffer_ptr, "%s%d\t", *buffer_ptr, col+1);
    }
    sprintf(*buffer_ptr, "%s\n\n", *buffer_ptr);

    // ROWS
    for (int row = 0; row < grid_size; row++) {

        sprintf(*buffer_ptr, "%s%d\t", *buffer_ptr, row+1);

        for (int col = 0; col < grid_size; col++) {
            if (is_cell_called(&session->calls, row, col)) {
                if (is_cell_empty(&session->grid, row, col)) {
                    sprintf(*buffer_ptr, "%s%c\t", *buffer_ptr, WATER_CHAR);
                } else {
                    sprintf(*buffer_ptr, "%s%c\t", *buffer_ptr, HIT_CHAR);
                }
            } else {
                sprintf(*buffer_ptr, "%s%c\t", *buffer_ptr, UNKNOWN_CHAR);
            }
        }

        sprintf(*buffer_ptr, "%s\n\n", *buffer_ptr);
    }

   
}

bool no_messages(DisplayUnit* display_ptr) {
    return (display_ptr->msg_first - display_ptr->msg_first);
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

    if (!no_messages(display_ptr))
        return NULL_MSG;
    
    MessageType popped_msg =  display_ptr->msg_queue[*first_ptr];
    
    *first_ptr = (*first_ptr + 1) % MSG_QUEUE_SIZE;
    return popped_msg;
}

void process_message(Session* session, char** buffer_ptr, MessageType msg_type) {
 
    switch (msg_type) {
        case UNKNOWN_CMD:
            sprintf(*buffer_ptr, "(!) unknown command");

        case ERR_INPUT_READ:
            sprintf(*buffer_ptr, "(!) error while reading the command");

        default: return;
    }

    sprintf(*buffer_ptr, "\n");
}

int count_buffered_lines(DisplayUnit* display_ptr) {
    char** buffer_ptr = &display_ptr->buffer;

    int lines = 0;

    for (const char *ptr = *buffer_ptr; *ptr != '\0'; ptr++) {
        if (*ptr == '\n') {
            lines++;
        }
    }

    // if ((*buffer_ptr)[0] != '\0' && (*buffer_ptr)[strlen(*buffer_ptr) - 1] != '\n') {
    //     lines++;
    // }
    return lines;
}

void flush_display(DisplayUnit* display_ptr) {
    //int* lines_num_ptr = &display_ptr->print_lines_num;

    char cursor_buffer[8];  //TODO check correct size
    
    //move cursor some lines up
    sprintf(cursor_buffer, "\033[%dA", count_buffered_lines(display_ptr));
    printf("%s", cursor_buffer);

    //clear and move to beginning
    printf("\033[K");
}

void update_display(App* app_ptr) {
    
    DisplayUnit *display_ptr = &app_ptr->display;

    flush_display(display_ptr);
    
    char** buffer_ptr = &display_ptr->buffer;
    
    generate_hits_grid(buffer_ptr, &app_ptr->session);

    while(!no_messages(display_ptr)) {
        MessageType cur_msg = pop_message(display_ptr);
        process_message(&app_ptr->session, buffer_ptr, cur_msg);
    }

    sprintf(*buffer_ptr, "type \'<col> <row>\' > ");
    printf("%s", *buffer_ptr);
}
