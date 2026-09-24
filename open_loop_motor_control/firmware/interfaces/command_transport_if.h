#ifndef COMMAND_TRANSPORT_IF_H
#define COMMAND_TRANSPORT_IF_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char *buffer;
    size_t capacity;
    size_t length;
    bool overflow;
} command_transport_line_t;

void command_transport_line_initialize(command_transport_line_t *line, char *buffer, size_t capacity);
bool command_transport_line_push(command_transport_line_t *line, char byte);

#endif
