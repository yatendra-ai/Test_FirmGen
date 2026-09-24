#include "command_transport_if.h"

void command_transport_line_initialize(command_transport_line_t *line, char *buffer, size_t capacity)
{
    if ((line != NULL) && (buffer != NULL) && (capacity > 1U)) {
        line->buffer = buffer;
        line->capacity = capacity;
        line->length = 0U;
        line->overflow = false;
        buffer[0] = '\0';
    }
}

bool command_transport_line_push(command_transport_line_t *line, char byte)
{
    if ((line == NULL) || (line->buffer == NULL)) {
        return false;
    }
    if ((byte == '\r') || (byte == '\n')) {
        if (line->overflow) {
            line->length = 0U;
            line->overflow = false;
            line->buffer[0] = '\0';
            return true;
        }
        line->buffer[line->length] = '\0';
        return true;
    }
    if (line->overflow) {
        return false;
    }
    if ((line->length + 1U) >= line->capacity) {
        line->overflow = true;
        return false;
    }
    line->buffer[line->length] = byte;
    line->length++;
    return false;
}
