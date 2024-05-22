
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/input.h"

const char INPUT_MAX_SIZE = 5;

InputUnit* create_input() {
    InputUnit* input = (InputUnit*) malloc(sizeof(input));

    input->buffer_size_max_b = sizeof(char) * (INPUT_MAX_SIZE + 1);
    input->buffer = (char*) malloc( input->buffer_size_max_b );

    return input;
}

void destroy_input(InputUnit* input) {
    free(input->buffer);
    input->buffer = NULL;
    free(input);
}

void empty_buffer() {
    char c;
    while ( (c = getchar())!= '\n' && c != EOF);
}

bool read_input(InputUnit* input) {
    if (!input->buffer)
        return false;

    char* line_ptr = fgets(input->buffer, input->buffer_size_max_b, stdin);

    if (!line_ptr)
        return false;

    char* newline = strchr(input->buffer, '\n');
    newline ? *newline = '\0' : empty_buffer();

    return true;
}

char* get_buffered_command(InputUnit* input) {
    return input->buffer;
}
