#include "../includes/includes.h"
#include "makenote.c"
#include <stdio.h>

char *red_error(const char *toPrint);
int exit_error(char *toPrint, exit_t code);
operation get_operation(int argc, char *argv[]);
note_t get_arg(char *argv);
char *config_path();
char *get_project_name();
char *svalidflags();
void initcheck();

int main(int argc, char *argv[]) {
    char *message;
    operation operation = get_operation(argc, argv);
    note_t noteLevel = get_arg(argv[2]);
    char *project = get_project_name();
    if (!project) {
        return exit_error("No git repository found, aborting", NOREPO);
    }

    // Complete the operation
    switch (operation) {
    case READ:
        if (noteLevel == INVALID) {
            return exit_error("Flag must be valid:\n-p -> project level note",
                              BADFLAG);
        }
        printf("Successfully read");
        if (project) {
            printf(" from %s", project);
        }
        printf("\n");
        break;
    case WRITE:
        printf("Successfully wrote");
        if (project) {
            printf(" to %s", project);
        }
        printf("\n");
        break;
    case NOOP:
        return exit_error("Must have an operation:\nread\nwrite", BADOPERATION);
        break;
    }

    initcheck();
    // FILE *note = mknote("balls");
    // if (!note) {
    //     fprintf(stderr, "Error in opening, aborting\n");
    //     return 1;
    // }
    char *path = config_path();
    printf("path is %s\n", path);
    // fprintf(note, "%s\n", argv[argc - 1]);
    // fclose(note);
    return 0;
}

char *red_error(const char *toPrint) {
    const char *start = "\x1b[31m";
    const char *suffix = "\x1b[0m";
    size_t len = strlen(start) + strlen(toPrint) + strlen(suffix) + 1;
    char *finalMessage = malloc(len);
    if (!finalMessage)
        return NULL;
    snprintf(finalMessage, len, "%s%s%s", start, toPrint, suffix);
    return finalMessage;
}

int exit_error(char *toPrint, exit_t code) {
    char *message = red_error(toPrint);
    fprintf(stderr, "%s\n", message);
    free(message);
    printf("%d\n", code);
    return code;
}

note_t get_arg(char *arg) {
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

char *config_path() {
    static char path[512];
    const char *xdg = getenv("XDG_CONFIG_HOME");
    if (xdg) {
        snprintf(path, sizeof(path), "%s/direy/config", xdg);
        return path;
    }
    const char *home = getenv("HOME");
    if (home) {
        snprintf(path, sizeof(path), "%s/.config/direy/config", home);
        return path;
    }
    return "/etc/direy/config";
}

char *get_project_name() {
    FILE *fp = popen("git config --get remote.origin.url", "r");
    if (!fp)
        return NULL;

    char url[512];
    if (fgets(url, sizeof(url), fp) == NULL) {
        pclose(fp);
        return NULL;
    }
    pclose(fp);

    // Remove newline
    url[strcspn(url, "\n")] = 0;

    // Find the last '/' || ':' in the url, right before the repo name
    char *last_slash = strrchr(url, '/');
    if (!last_slash)
        last_slash = strrchr(url, ':');
    if (!last_slash)
        return NULL;

    static char project_name[512];
    int written =
        snprintf(project_name, sizeof(project_name), "%s", last_slash + 1);
    if (written >= sizeof(project_name) || written < 0) {
        return NULL;
    }

    // If '.git' is found, changes the element where the '.get' would be
    char *dot_git = strstr(project_name, ".git");
    if (dot_git)
        *dot_git = '\0';

    return project_name;
}

operation get_operation(int argc, char *argv[]) {
    if (argc < 2 || strlen(argv[1]) > 5) {
        return NOOP;
    }
    if (!strcmp(argv[1], "write"))
        return WRITE;
    if (!strcmp(argv[1], "read"))
        return READ;
    return NOOP;
}

void initcheck() {
    char path[PATH_MAX];
    if (getcwd(path, sizeof(path)) == NULL) {
        fprintf(stderr, "Error occured: %s\n", strerror(errno));
    }
    printf("path: %s\n", path);

    DIR *d;
    struct dirent *entry;
    d = opendir(path);
    if (!d) {
        fprintf(stderr, "Error occured: %s\n", strerror(errno));
    }
    while ((entry = readdir(d)) != NULL) {
        char buf[64];
        sprintf(buf, "%s", entry->d_name);
        if (!strcmp(buf, "Diary")) {
            closedir(d);
            return;
        }
    }
    closedir(d);
}
