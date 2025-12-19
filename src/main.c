#include "../includes/includes.h"
#include "makenote.c"

char *red_error(const char *toPrint);
int exit_error(char *toPrint, exit_t code);
operation get_operation(int argc, char *argv[]);
note_t get_arg(char *argv);
char *config_path();
char *get_project_name();
char *svalidflags();
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

    // Check for defaunt config
    const char *xdg = getenv("XDG_CONFIG_HOME");
    if (xdg) {
        int written = snprintf(path, sizeof(path), "%s/direy/config", xdg);
        if (written > sizeof(path) || written < 0) {
            return NULL;
        }
        return path;
    }

    // Check for backup config
    const char *home = getenv("HOME");
    if (home) {
        int written =
            snprintf(path, sizeof(path), "%s/.config/direy/config", home);
        if (written > sizeof(path) || written < 0) {
            return NULL;
        }
        return path;
    }

    // Default to the system config
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
