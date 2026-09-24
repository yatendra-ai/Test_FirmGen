#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    COMMAND_KIND_INVALID = 0,
    COMMAND_KIND_START,
    COMMAND_KIND_STOP,
    COMMAND_KIND_DIRECTION,
    COMMAND_KIND_FREQUENCY,
    COMMAND_KIND_DUTY,
    COMMAND_KIND_STATUS
} command_kind_t;

typedef struct {
    command_kind_t kind;
    bool clockwise;
    float value;
} command_t;

bool command_parser_parse(const char *line, command_t *command);

#endif
