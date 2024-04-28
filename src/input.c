
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/input.h"

const char INPUT_MAX_SIZE = 5;

void init_input(InputUnit* input) {
    input->buffer_size_max_b = sizeof(char) * (INPUT_MAX_SIZE + 1);
    input->buffer = (char*) malloc( input->buffer_size_max_b );
}

void destroy_input(InputUnit* input) {
    free(input->buffer);
    input->buffer = NULL;
}

char* read_next_command(InputUnit* input) {

    if (input->buffer == NULL) {
        return NULL;
    }

    char* line_ptr = fgets(input->buffer, input->buffer_size_max_b, stdin);

    if (line_ptr != NULL) {

        char* newline = strchr(input->buffer, '\n');

        if (newline != NULL) {
            *newline = '\0';
        } else {
            char c;
            while ( (c = getchar())!= '\n' && c != EOF);
        }

        return input->buffer;
    }
    return NULL;
}
