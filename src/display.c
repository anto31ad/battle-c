#include "../include/display.h"
#include <stdio.h>
#include <stdlib.h>

const char WATER_CHAR = '~';
const char HIT_CHAR = 'X';
const char UNKNOWN_CHAR = '*';


void init_display(DisplayUnit* display, Session* session) {
    char** buffer = &display->buffer;
    int grid_size = session->grid.size;

    if (*buffer == NULL) {
        int buffer_size = (grid_size + 1) * (grid_size + 1) * 3;
        *buffer = (char*) malloc(sizeof(char) * buffer_size);
    }
}

void destroy_display(DisplayUnit* display) {
    if (display->buffer != NULL) {
        free(display->buffer);
        display->buffer = NULL;
    }
}


void display_hits(DisplayUnit* display, Session* session) {
    char** buffer = &display->buffer;

    int grid_size = session->grid.size;

    **buffer = '\0';
    sprintf(*buffer, "%s\n\n", *buffer);

    // COLUMS
    sprintf(*buffer, "%s\t", *buffer);
    for (int col = 0; col < grid_size; col++) {
        sprintf(*buffer, "%s%d\t", *buffer, col+1);
    }
    sprintf(*buffer, "%s\n\n", *buffer);

    // ROWS
    for (int row = 0; row < grid_size; row++) {

        sprintf(*buffer, "%s%d\t", *buffer, row+1);

        for (int col = 0; col < grid_size; col++) {
            if (is_cell_called(&session->calls, row, col)) {
                if (is_cell_empty(&session->grid, row, col)) {
                    sprintf(*buffer, "%s%c\t", *buffer, WATER_CHAR);
                } else {
                    sprintf(*buffer, "%s%c\t", *buffer, HIT_CHAR);
                }
            } else {
                sprintf(*buffer, "%s%c\t", *buffer, UNKNOWN_CHAR);
            }
        }

        sprintf(*buffer, "%s\n\n", *buffer);
    }

    printf("%s", *buffer);
}

void display_message(DisplayUnit* display, char* message) {
    printf("%s", message);
}

void display_coords_prompt(DisplayUnit* display) {
    printf("type \'<col> <row>\' > ");
}

void display_start(DisplayUnit* display) {
    printf("Starting new game \n");
}

void display_win(DisplayUnit* display) {
    printf("\n Every ship has been sunk, you win! \n");
}

void display_quit(DisplayUnit* display) {
    printf("\n Quitting...\n");
}
