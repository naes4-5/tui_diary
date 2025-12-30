#include "../include/findpaths.h"
#include <dirent.h>
#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// returns the path to the config for diery
char *config_path() {
    static char path[PATH_MAX];

    // Check for defaunt config
    const char *xdg = getenv("XDG_CONFIG_HOME");
    if (xdg) {
        int w = snprintf(path, sizeof(path), "%s/direy/config", xdg);
        if (w > PATH_MAX || w < 0) {
            return NULL;
        }
        return path;
    }

    // Check for backup config
    const char *home = getenv("HOME");
    if (home) {
        int w =
            snprintf(path, sizeof(path), "%s/.config/direy/config", home);
        if (w > PATH_MAX || w < 0) {
            return NULL;
        }
        return path;
    }

    // Default to the system config
    // Does require the command to be run with sudo
    return "/etc/direy/config";
}

// returns the name attatched to the local git repo
#define PROJECT_MAX 512
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

    static char project_name[PROJECT_MAX];
    int w =
        snprintf(project_name, sizeof(project_name), "%s", last_slash + 1);
    if (w >= PROJECT_MAX || w < 0) {
        return NULL;
    }

    // If '.git' is found, changes the element where the '.get' would be
    char *dot_git = strstr(project_name, ".git");
    if (dot_git)
        *dot_git = '\0';

    return project_name;
}

// returns the path to the directory relevant to the project
DIR *get_project_dir(const char *homePath, const char *projectname) {
    if (!homePath || !projectname) {
        return NULL;
    }
    static char path[PATH_MAX];

    int w = snprintf(path, sizeof(path), "%s/.diery/", homePath);
    if (w > PATH_MAX || w < 0) {
        return NULL;
    }

    if (mkdir(path, 0755) == -1 && errno != EEXIST) {
        perror("Error making ~/.diery");
        return NULL;
    }

    DIR *d = opendir(path);
    if (!d) {
        perror("Error opening ~/.diery");
    }
    return _make_project_dir(d, projectname, path);
}

// returns the directory for the relevant project based on the projectname
DIR *_make_project_dir(DIR *d, const char *projectname, char *pathtodiery) {
    struct dirent *entry;
    char projectpath[PATH_MAX];
    int w =
        snprintf(projectpath, sizeof(projectpath), "%s%s", pathtodiery, projectname);
    if (w > PATH_MAX || w < 0) {
        perror("Can't print to path");
        closedir(d);
        return NULL;
    }
    // check to see if it alread exists
    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, projectname)) {
            continue;
        }
        DIR *ret = opendir(projectpath);
        if (!ret) {
            perror("Can't open path");
            closedir(d);
            return NULL;
        }
        closedir(d);
        return ret;
    }
    closedir(d);
    // make & return the directory if not
    if (mkdir(projectpath, 0755) == -1 && errno != EEXIST) {
        perror("Error making ~/.diery/{project name}");
        return NULL;
    }
    DIR *ret = opendir(projectpath);
    if (!ret) {
        return NULL;
    }
    return ret;
}
