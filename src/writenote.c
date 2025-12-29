#pragma once

#include "../includes/includes.h"
#include "cltools.c"
#include "readnote.c"
#include <limits.h>
#include <time.h>

const char *days[7] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                       "Thursday", "Friday", "Saturday"};

const char *months[] = {"1.January/",  "2.February/",  "3.March/",
                        "4.April/",    "5.May/",       "6.June/",
                        "7.July/",     "8.August/",    "9.September/",
                        "10.October/", "11.November/", "12.December/"};

char *_get_next_note_name(const char *projectpath, const char *projectname);
FILE *_get_project_note(const char *projectpath, const char *projectname,
                        note_t notelevel);
long _get_last_entry_number(const char *projectpath, const char *projectname);
char *_get_daily_header();
size_t _get_max_long_length();

// returns 0 if everything went well. Makes a new note with message written in
// it and the relevant path, indicated by notelevel
int write_note(const char *projectpath, const char *projectname,
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
    char *notename = _get_next_note_name(projectpath, projectname);
    if (!notename) {
        perror("Error getting the header");
        return NULL;
    }
    if (notelevel == PROJECT) {
        written = snprintf(finalpath, sizeof(finalpath), "%s%s%s%s",
                           projectpath, "/", projectname, ".typ");
        if (written >= sizeof(finalpath) || written < 0) {
            free(notename);
            return NULL;
        }
        FILE *file = fopen(finalpath, "a");
        if (!file) {
            perror("Error opening file for note");
            free(notename);
            return NULL;
        }
        free(notename);
        return file;
    }
    written = snprintf(finalpath, sizeof(finalpath), "%s%s%s", projectpath, "/",
                       notename);
    if (written >= sizeof(finalpath) || written < 0) {
        free(notename);
        return NULL;
    }
    FILE *file = fopen(finalpath, "w");
    if (!file) {
        perror("Error opening file for note");
        free(notename);
        return NULL;
    }
    free(notename);
    return file;
}

// returns the next note name based on the files currently in the projectpath
// directory. Includes file extension.
char *_get_next_note_name(const char *projectpath, const char *projectname) {
    long max = _get_last_entry_number(projectpath, projectname);
    if (max == -1) {
        exit_error("error geting next entry number", BADDIR);
        return NULL;
    }
    char extension[] = ".typ";
    char *nextnote =
        malloc(sizeof('#') + _get_max_long_length() + sizeof(extension));
    if (!nextnote) {
        perror("error mallocing");
        return NULL;
    }
    int w = snprintf(nextnote, sizeof(nextnote), "%s%ld%s", "#", max + 1,
                     extension);
    if (w >= sizeof(nextnote) || w < 0) {
        exit_error("error retrieving next note's name", 1);
        free(nextnote);
        return NULL;
    }
    return nextnote;
}

// returns the time of the current day in the format '%y-%m-%d;%H:%M'
char *_get_daily_header() { // CURRENTLY NOT IN USE
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char *header = malloc(256);
    if (!header) {
        return NULL;
    }
    strftime(header, 256, "%y-%m-%d;%H:%M", t);
    return header;
}
