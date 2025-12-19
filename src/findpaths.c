#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// returns the path to the config for diery
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

// returns the name attatched to the local git repo
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
