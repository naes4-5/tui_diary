#pragma once

#include "../includes/includes.h"

// returns toPrint but red when printed
static char *red_error(const char *toPrint) {
    const char *prefix = "\x1b[31m";
    const char *suffix = "\x1b[0m";
    size_t len = strlen(prefix) + strlen(toPrint) + strlen(suffix) + 1;
    char *finalMessage = malloc(len);
    if (!finalMessage)
        return NULL;
    snprintf(finalMessage, len, "%s%s%s", prefix, toPrint, suffix);
    return finalMessage;
}

// returns the exit code & prints the error message (to_print)
int exit_error(char *to_print, exit_t code) {
    char *message = red_error(to_print);
    fprintf(stderr, "%s\n", message);
    free(message);
    return code;
}

// returns the type of note based on the argument for a write operation
note_t get_note_type_write(int argc, char *argv[]) {
    if (argc < 3) {
        return INVALID;
    }
    for (int i = 3; i < argc && argv[i - 1][0] == '-'; i++) {
        switch (argv[i - 1][1]) {
        case 'p':
            return PROJECT;
        // Yet to be implemented
        case 'a':
            return AMEND;
        }
    }
    return NORMAL;
}

// returns the type of note based on the argument for a read operation
note_t get_note_type_read(int argc, char *argv[]) {
    if (argc < 3) {
        return NORMAL;
    }
    for (int i = 3; i <= argc && argv[i - 1][0] == '-'; i++) {
        switch (argv[i - 1][1]) {
        case 'p':
            return PROJECT;
        // Yet to be implemented
        case 'n':
            return BACK;
        }
    }
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
