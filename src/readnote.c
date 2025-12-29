#pragma once

#include "../includes/includes.h"
#include "cltools.c"
#include <ctype.h>
#include <limits.h>

#define BUFF_MAX 4096

long _get_last_entry_number(const char *projectpath, const char *projectname);
size_t _get_max_long_length();
int print_note(FILE *note);

int read_note(const char *projectpath, const char *projectname) {
    char extension[] = ".typ";
    long entrynum = _get_last_entry_number(projectpath, projectname);
    size_t notename_s = sizeof('#') + _get_max_long_length() +
                        sizeof(*extension) + sizeof('\0');
    char *notename = malloc(notename_s);
    if (!notename) {
        return exit_error("error mallocing from read_note", 1);
    }
    int w = snprintf(notename, notename_s, "%c%ld%s%c", '#', entrynum,
                     extension, '\0');
    if (w >= notename_s || w < 0) {
        free(notename);
        return exit_error("error snprintfing to notename from read_note", 2);
    }
    
    char finalpath[PATH_MAX];
    w = snprintf(finalpath, PATH_MAX, "%s%c%s", projectpath, '/', notename);
    free(notename);
    if (w >= PATH_MAX || w < 0) {
        return exit_error("error snprintfing to finalpath from read_note", 3);
    }
    
    FILE *note = fopen(finalpath, "r");
    if (!note) {
        perror("error opening note to read");
        return BADDIR;
    }
    print_note(note);
    fclose(note);
    return 0;
}

int read_project(const char *projectpath, const char *projectname) {
    char finalpath[PATH_MAX];
    int w = snprintf(finalpath, PATH_MAX, "%s%s", projectpath, projectpath);
    if (w >= PATH_MAX || w < 0) {
        return exit_error("error snprintfing to finalptah from read_project",
                          3);
    }
    FILE *projectnote = fopen(finalpath, "r");
    if (!projectnote) {
        perror("error opening projectnote");
        return 1;
    }
    print_note(projectnote);
    fclose(projectnote);
    return 0;
}

// returns the next number based on previous entries for the project in
// projectpath
long _get_last_entry_number(const char *projectpath, const char *projectname) {
    DIR *projectdir = opendir(projectpath);
    if (!projectdir) {
        perror("error opening project directory");
        return -1;
    }
    struct dirent *entry;
    long max = 0;
    while ((entry = readdir(projectdir)) != NULL) {
        if (strstr(entry->d_name, "#") == NULL)
            continue;
        char c;
        char nums[256] = {0};
        int j = 0;
        for (int i = 1; (c = entry->d_name[i]) != '\0'; i++) {
            if (isdigit(c)) {
                nums[j++] = c;
                continue;
            }
            if (c == '.') {
                break;
            }
        }
        long conv = atoi(nums);
        if (conv > max) {
            max = conv;
        }
    }
    closedir(projectdir);
    return max;
}

// returns the number of bytes needed to store LONG_MAX in a string
size_t _get_max_long_length() {
    int ret = 0;
    long max = LONG_MAX;
    while (max >= 1) {
        max /= 10;
        ret++;
    }
    return ret;
}

int print_note(FILE *note) {
    char buff[BUFF_MAX];
    size_t bytes_read;
    while ((bytes_read = fread(buff, 1, BUFF_MAX, note)) > 0) {
        fwrite(buff, 1, bytes_read, stdout);
    }
    return 0;
}
