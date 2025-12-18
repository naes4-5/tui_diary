// test from new branch I hope
// Ooooooooooooooooooh, that failed, but this one def wont frfr on skibidi
// And now here's a second commit for ya, we'll see how this one squashes. 

#include "src/includes.h"
#include "src/makenote.c"

char *red_error(const char *toPrint);
Operatin get_operation(int argc, char *argv[], char *title);
FlgTyp get_arg(char *argv);
char *config_path();
char *get_project_name();
void initcheck();
int initdirs();

int main(int argc, char *argv[]) {
    char *title = malloc(16);
    char *message;
    char *project;
    memcpy(title, "balls", sizeof("balls"));
    Operatin operation = get_operation(argc, argv, title);
    switch (operation) {
    case NOOP:
        message = red_error("Must include an operation:\nread\nwrite");
        fprintf(stderr, "%s\n", message);
        free(message);
        free(title);
        return 1;
    case READ:
        switch (get_arg(argv[2])) {
        case PROJECT:
            project = get_project_name();
            printf("Project read from %s\n", project);
            break;
        case NORMAL:
            printf("Normal read\n");
            break;
        case INVALID:
            message = red_error("Flag must be valid:\n-p");
            fprintf(stderr, "%s\n", message);
            free(title);
            free(message);
            return 1;
        }
        break;
    case WRITE:
        printf("Successfully wrote!\n");
        break;
    }
    initcheck();
    FILE *note = mknote(title);
    if (!note) {
        fprintf(stderr, "Error in opening, aborting\n");
        free(title);
        return 1;
    }
    char *path = config_path();
    printf("path is %s\n", path);
    fprintf(note, "%s\n", argv[argc - 1]);
    fclose(note);
    free(title);
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

FlgTyp get_arg(char *arg) {
    if (!arg)
        return NORMAL;
    if (arg[0] != '-' || strlen(arg) < 2)
        return INVALID;
    if (arg[1] == 'p')
        return PROJECT;
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

Operatin get_operation(int argc, char *argv[], char *title) {
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
    initdirs();
}

int initdirs() {
    if (mkdir("Diary", 0744) == -1) {
        fprintf(stderr, "Directory creation failed: %s\n", strerror(errno));
        return 1;
    }

    for (int i = 0; i < sizeof(months) / sizeof(months[0]); i++) {
        char dir[32];
        snprintf(dir, sizeof(dir), "Diary/%s", months[i]);
        if (mkdir(dir, 0744) == -1) {
            if (errno == EEXIST) {
                fprintf(
                    stderr,
                    "Directory creation failed, \"%s\" already exists: %s\n",
                    dir, strerror(errno));
            } else {
                fprintf(stderr, "\"mkdir\" failed: %s\n", strerror(errno));
            }
        }
    }

    return 0;
}
