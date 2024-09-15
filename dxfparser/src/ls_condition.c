#include "ls_condition.h"

int line_entity_condition(int code, const char* value) {
    return (code == 0 && strcmp(value, "LINE\n") == 0);
}

int coordinate_condition(int code, const char* value) {
    return (code == 10 || code == 20);
}