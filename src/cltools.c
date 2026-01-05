#include "../include/cltools.h"
#include <string.h>

// returns to_print but red when printed
static char *red_error(const char *to_print) {
    const char *prefix = "\x1b[31m";
    const char *suffix = "\x1b[0m";
    size_t len = strlen(prefix) + strlen(to_print) + strlen(suffix) + 1;
    char *finalMessage = malloc(len);
    if (!finalMessage)
        return NULL;
    snprintf(finalMessage, len, "%s%s%s", prefix, to_print, suffix);
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
        case 'f':
            return FFILE;
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
        case 'd':
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

// checks all of the environment variables and ensures that they will all
// function correctly.
exit_t check_env(const char *homepath, char dierypath[],
                 const char *projectname, char projectpath[],
                 const char *configpath, DIR *projectdir) {
    if (!homepath) {
        return exit_error("Error opening home directory, aborting.\nTryagain.",
                          BADDIR);
    }
    int written = snprintf(dierypath, PATH_MAX, "%s/.diery/", homepath);
    if (written > PATH_MAX || written < 0) {
        return exit_error("Error getting path to '~/.diery'.\nTry again.",
                          BADDIR);
    }
    if (!projectname) {
        return exit_error("No git repository found, aborting", NOREPO);
    }
    written = snprintf(projectpath, PATH_MAX, "%s%s", dierypath, projectname);
    if (written > PATH_MAX || written < 0) {
        return exit_error("Error getting path to '~/.diery'.\nTry again.",
                          BADDIR);
    }
    if (!configpath) {
        return exit_error("Failed to open configuration", BADDIR);
    }
    if (!projectdir) {
        return exit_error("Directory faileld to open, aborting.\nTry again.",
                          BADDIR);
    }
    return NONE;
}

// parses the operations for the user and ensures that they're valid.
exit_t check_operation(int argc, char *argv[], operation op,
                       note_t *notelevel) {
    switch (op) {
    case WRITE:
        *notelevel = get_note_type_write(argc, argv);
        break;
    case READ:
        *notelevel = get_note_type_read(argc, argv);
        break;
    case NOOP:
        return exit_error("Must have an operation:\nread\nwrite", BADOPERATION);
        break;
    }
    if (*notelevel == INVALID) {
        return exit_error("Flag must be valid:\n-p -> project level note",
                          BADFLAG);
    }
    return NONE;
}
