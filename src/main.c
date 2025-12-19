#include "../includes/includes.h"
#include "cltools.c"
#include "findpaths.c"
#include "makenote.c"
#include <linux/limits.h>

int main(int argc, char *argv[]) {
    const char *homepath = getenv("HOME");
    if (!homepath) {
        return exit_error("Error opening home directory, aborting.\nTryagain.",
                          BADDIR);
    }
    char dierypath[PATH_MAX];
    int written =
        snprintf(dierypath, sizeof(dierypath), "%s/.diery/", homepath);
    if (written > sizeof(dierypath) || written < 0) {
        return exit_error("Error getting path to '~/.diery'.\nTry again.",
                          BADDIR);
    }

    char *projectname = get_project_name();
    if (!projectname) {
        return exit_error("No git repository found, aborting", NOREPO);
    }

    char projectpath[PATH_MAX];
    written = snprintf(projectpath, sizeof(projectpath), "%s%s", dierypath,
                       projectname);
    if (written > sizeof(dierypath) || written < 0) {
        return exit_error("Error getting path to '~/.diery'.\nTry again.",
                          BADDIR);
    }

    char *configpath = config_path();
    if (!configpath) {
        return exit_error("Failed to open configuration", BADDIR);
    }

    operation op = get_operation(argc, argv);
    if (op == NOOP) {
        return exit_error("Must have an operation:\nread\nwrite", BADOPERATION);
    }

    note_t notelevel = get_note_type(argc, argv);
    if (notelevel == INVALID) {
        return exit_error("Flag must be valid:\n-p -> project level note",
                          BADFLAG);
    }

    // Need the DIR for reading files
    DIR *projectdir = get_project_dir(homepath, projectname);
    if (!projectdir) {
        return exit_error("Directory faileld to open, aborting.\nTry again.",
                          BADDIR);
    }

    if (op == READ) {
        printf("Successfully read from %s\n", projectname);
    } else if (op == WRITE) {
        int noteexit =
            make_note(projectpath, projectname, argv[argc - 1], notelevel);
        if (noteexit) {
            return exit_error("could not make note", noteexit);
        }
    }

    closedir(projectdir);
    return 0;
}
