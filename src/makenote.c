#include "../includes/includes.h"
#include <linux/limits.h>
#include <time.h>

const char *days[7] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                       "Thursday", "Friday", "Saturday"};

const char *months[] = {"1.January/",  "2.February/",  "3.March/",
                        "4.April/",    "5.May/",       "6.June/",
                        "7.July/",     "8.August/",    "9.September/",
                        "10.October/", "11.November/", "12.December/"};

char *_get_daily_header();
FILE *_get_project_note(const char *projectpath, const char *projectname,
                        note_t notelevel);

// returns 0 if everything went well. Makes a new note with message written in
// it and the relevant path, indicated by notelevel
int make_note(const char *projectpath, const char *projectname,
              const char *message, note_t notelevel) {
    FILE *note = _get_project_note(projectpath, projectname, notelevel);
    if (!note) {
        return 1;
    }
    if (fprintf(note, "%s\n", message) < 0) {
        return 2;
    }
    fclose(note);
    return 0;
}

// Gets the note relevant to the projectpath, and if the notelevel calls for it,
// the projectname
FILE *_get_project_note(const char *projectpath, const char *projectname,
                        note_t notelevel) {
    char finalpath[PATH_MAX];
    int written;
    const char *header = _get_daily_header();
    if (!header) {
        perror("Error getting the header");
        return NULL;
    }
    if (notelevel == PROJECT) {
        written = snprintf(finalpath, sizeof(finalpath), "%s%s%s%s",
                           projectpath, "/", projectname, ".typ");
        if (written >= sizeof(finalpath) || written < 0) {
            free((void *)header);
            return NULL;
        }
        FILE *file = fopen(finalpath, "a");
        if (!file) {
            perror("Error opening file for note");
            free((void *)header);
            return NULL;
        }
        free((void *)header);
        return file;
    }
    written = snprintf(finalpath, sizeof(finalpath), "%s%s%s%s", projectpath,
                       "/", header, ".typ");
    if (written >= sizeof(finalpath) || written < 0) {
        free((void *)header);
        return NULL;
    }
    FILE *file = fopen(finalpath, "w");
    if (!file) {
        perror("Error opening file for note");
        free((void *)header);
        return NULL;
    }
    free((void *)header);
    return file;
}

char *_get_daily_header() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char *header = malloc(256);
    if (!header) {
        return NULL;
    }
    strftime(header, 256, "%y-%m-%d;%H:%M", t);
    return header;
}
