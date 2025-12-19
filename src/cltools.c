#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/includes.h"

// returns toPrint but red when printed
static char *red_error(const char *toPrint) {
    const char *start = "\x1b[31m";
    const char *suffix = "\x1b[0m";
    size_t len = strlen(start) + strlen(toPrint) + strlen(suffix) + 1;
    char *finalMessage = malloc(len);
    if (!finalMessage)
        return NULL;
    snprintf(finalMessage, len, "%s%s%s", start, toPrint, suffix);
    return finalMessage;
}

// returns the exit code & prints the error message (toPrint)
int exit_error(char *toPrint, exit_t code) {
    char *message = red_error(toPrint);
    fprintf(stderr, "%s\n", message);
    free(message);
    return code;
}

// returns the type of note based on the argument
note_t get_note_type(char *arg) {
    if (!arg)
        return NORMAL;
    if (arg[0] != '-' || strlen(arg) < 2)
        return INVALID;
    if (arg[1] == 'p')
        return PROJECT;
    else if (arg[1] != 'p')
        return INVALID;
    return NORMAL;
}

// returns the operation the user intends to do
operation get_operation(int argc, char *argv[]) {
    if (argc < 2 || strlen(argv[1]) > 5)
        return NOOP;
    if (!strcmp(argv[1], "write"))
        return WRITE;
    if (!strcmp(argv[1], "read"))
        return READ;
    return NOOP;
}
