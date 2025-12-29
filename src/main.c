#include "../includes/includes.h"
#include "cltools.c"
#include "findpaths.c"
#include "readnote.c"
#include "writenote.c"
#include <linux/limits.h>

exit_t check_env(const char *homepath, char dierypath[],
                 const char *projectname, char projectpath[],
                 const char *configpath, DIR *projectdir);
int check_operation(int argc, char *argv[], operation op, note_t *notelevel);

int main(int argc, char *argv[]) {
    const char *homepath = getenv("HOME");
    char dierypath[PATH_MAX];
    char *projectname = get_project_name();
    char projectpath[PATH_MAX];
    char *configpath = config_path();
    DIR *projectdir = get_project_dir(homepath, projectname);
    exit_t env_check = check_env(homepath, dierypath, projectname, projectpath,
                                 configpath, projectdir);
    if (env_check != NONE) {
        return exit_error("error parsing environment variables", env_check);
    }

    operation op = get_operation(argc, argv);
    note_t notelevel;
    int op_check = check_operation(argc, argv, op, &notelevel);
    if (op_check != NONE) {
        return exit_error("error parsing operation", op_check);
    }

    if (op == READ) {
        int noteexit;
        switch (notelevel) {
        case PROJECT:
            noteexit = read_project(projectpath, projectname);
            break;
        case NORMAL:
            noteexit = read_note(projectpath, projectname);
            break;
        case INVALID:
        case AMEND:
        case BACK:
            closedir(projectdir);
            return exit_error("bad operation for the read operation", BADOPERATION);
        }
        if (noteexit) {
            closedir(projectdir);
            return exit_error("could not read note", noteexit);
        }
    } else if (op == WRITE) {
        int noteexit =
            make_note(projectpath, projectname, argv[argc - 1], notelevel);
        if (noteexit) {
            closedir(projectdir);
            return exit_error("could not make note", noteexit);
        }
    }

    closedir(projectdir);
    return 0;
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
int check_operation(int argc, char *argv[], operation op, note_t *notelevel) {
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
