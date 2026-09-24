#include "command_parser.h"
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static const char *skip_spaces(const char *text)
{
    while ((*text == ' ') || (*text == '\t')) {
        text++;
    }
    return text;
}

static bool end_of_text(const char *text)
{
    return (*skip_spaces(text) == '\0');
}

static bool parse_value(const char *text, float *value)
{
    char *end;
    const char *start = skip_spaces(text);

    if ((*start == '\0') || (*start == '-')) {
        return false;
    }
    *value = strtof(start, &end);
    if ((end == start) || !isfinite(*value) || (*value < 0.0f)) {
        return false;
    }
    return end_of_text(end);
}

bool command_parser_parse(const char *line, command_t *command)
{
    const char *text;

    if ((line == NULL) || (command == NULL)) {
        return false;
    }
    command->kind = COMMAND_KIND_INVALID;
    command->clockwise = true;
    command->value = 0.0f;
    text = skip_spaces(line);

    if (strcmp(text, "MOTOR START") == 0) {
        command->kind = COMMAND_KIND_START;
    } else if (strcmp(text, "MOTOR STOP") == 0) {
        command->kind = COMMAND_KIND_STOP;
    } else if (strcmp(text, "DIR CW") == 0) {
        command->kind = COMMAND_KIND_DIRECTION;
        command->clockwise = true;
    } else if (strcmp(text, "DIR CCW") == 0) {
        command->kind = COMMAND_KIND_DIRECTION;
        command->clockwise = false;
    } else if (strcmp(text, "STATUS") == 0) {
        command->kind = COMMAND_KIND_STATUS;
    } else if (strncmp(text, "FREQ ", 5U) == 0) {
        if (parse_value(text + 5, &command->value)) {
            command->kind = COMMAND_KIND_FREQUENCY;
        }
    } else if (strncmp(text, "DUTY ", 5U) == 0) {
        if (parse_value(text + 5, &command->value)) {
            command->kind = COMMAND_KIND_DUTY;
        }
    } else {
        command->kind = COMMAND_KIND_INVALID;
    }
    return command->kind != COMMAND_KIND_INVALID;
}
