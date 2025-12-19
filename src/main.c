#include "../includes/includes.h"
#include "makenote.c"
#include "findpaths.c"
#include "cltools.c"

DIR *initdir();

int main(int argc, char *argv[]) {
    char *message;
    char *project = get_project_name();
    if (!project) {
        return exit_error("No git repository found, aborting", NOREPO);
    }

    DIR *projectdir = initdir();
    if (!projectdir) {
        return exit_error("Directory faield to open, aborting.\nTry again.",
                          BADDIR);
    }

    operation operation = get_operation(argc, argv);
    if (operation == NOOP) {
        closedir(projectdir);
        return exit_error("Must have an operation:\nread\nwrite", BADOPERATION);
    }

    note_t notelevel = get_arg(argv[2]);
    if (notelevel == INVALID) {
        closedir(projectdir);
        return exit_error("Flag must be valid:\n-p -> project level note",
                          BADFLAG);
    }

    if (operation == READ) {
        printf("Successfully read from %s\n", project);
    } else if (operation == WRITE) {
        printf("Successfully wrote to %s ", project);
        if (notelevel == PROJECT) {
            printf("as a project note!");
        }
        printf("\n");
    }

    char *path = config_path();
    if (!path) {
        closedir(projectdir);
        return exit_error("Failed to open configuration", BADDIR);
    }
    printf("path is %s\n", path);

    closedir(projectdir);
    return 0;
}

DIR *initdir() {
    static char path[PATH_MAX];

    // Find the relevant folder
    const char *homePath = getenv("HOME");
    if (!homePath) {
        return NULL;
    }

    int written = snprintf(path, sizeof(path), "%s/.diery", homePath);
    if (written > sizeof(path) || written < 0) {
        return NULL;
    }

    if (mkdir(path, 0755) == -1 && errno != EEXIST) {
        perror("Error making ~/.diery");
        return NULL;
    }

    DIR *d;
    struct dirent *entry;
    d = opendir(path);
    if (!d) {
        perror("Error opening ~/.diery");
    }
    return d;
}
