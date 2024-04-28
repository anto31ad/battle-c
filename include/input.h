#ifndef INPUT_H
#define INPUT_H

typedef struct {
    char* buffer;
    int buffer_size_max_b;
} InputUnit;

char* read_input(InputUnit*);
void init_input(InputUnit*);
void destroy_input(InputUnit*);

#endif