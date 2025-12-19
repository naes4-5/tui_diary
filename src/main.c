#include "../includes/includes.h"
#include "cltools.c"
#include "findpaths.c"
#include "makenote.c"
#include <linux/limits.h>

DIR *initdir(const char *homePath);

int main(int argc, char *argv[]) {
    char *message;
    FILE *newnote;
    const char *homepath = getenv("HOME");
    if (!homepath) {
        return exit_error("Error opening home directory, aborting.\nTryagain.",
                          BADDIR);
    }
    char dierypath[PATH_MAX];
    int writ1 = snprintf(dierypath, sizeof(dierypath), "%s/.diery/", homepath);
    if (writ1 > sizeof(dierypath) || writ1 < 0) {
        return exit_error("Error getting path to '~/.diery'.\nTry again.",
                          BADDIR);
    }

    char *projectname = get_project_name();
    if (!projectname) {
        return exit_error("No git repository found, aborting", NOREPO);
    }

    char projectpath[PATH_MAX];
    int writ2 = snprintf(projectpath, sizeof(projectpath), "%s%s", dierypath,
                         projectname);
    if (writ2 > sizeof(dierypath) || writ2 < 0) {
        return exit_error("Error getting path to '~/.diery'.\nTry again.",
                          BADDIR);
    }

    char *configpath = config_path();
    if (!configpath) {
        return exit_error("Failed to open configuration", BADDIR);
    }
    printf("project name is %s\n", projectname);
    printf("path to config is %s\n", configpath);
    printf("path to diery is %s\n", dierypath);

    DIR *projectdir = initdir(homepath);
    if (!projectdir) {
        return exit_error("Directory faield to open, aborting.\nTry again.",
                          BADDIR);
    }

    operation operation = get_operation(argc, argv);
    if (operation == NOOP) {
        closedir(projectdir);
        return exit_error("Must have an operation:\nread\nwrite", BADOPERATION);
    }

    note_t notelevel = get_note_type(argv[2]);
    if (notelevel == INVALID) {
        closedir(projectdir);
        return exit_error("Flag must be valid:\n-p -> project level note",
                          BADFLAG);
    }

    if (operation == READ) {
        printf("Successfully read from %s\n", projectname);
    } else if (operation == WRITE) {
        printf("Successfully wrote to %s ", projectname);
        if (notelevel == PROJECT) {
            printf("as a project note!");
            goto note_complete;
        }
        newnote = mknote(projectname, dierypath);
        if (!newnote) {
            closedir(projectdir);
            return exit_error("Error opening file", BADDIR);
        }
    note_complete:
        printf("Note successfully written!\n");
    }

    if (newnote)
        fclose(newnote);
    closedir(projectdir);
    return 0;
}

DIR *initdir(const char *homePath) {
    static char path[PATH_MAX];

    int written = snprintf(path, sizeof(path), "%s/.diery/", homePath);
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
