#include "../include/readwriteutils.h"
#include "../include/cltools.h"
#include "../include/includes.h"
#include <ctype.h>
#include <limits.h>
#include <string.h>

#define BUFF_MAX 4096

// returns the next number based on previous entries for the project in
// projectpath
long _get_last_entry_number(const char *projectpath) {
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

// prints the contents of the FILE
int print_note(FILE *note) {
    char buff[BUFF_MAX];
    size_t bytes_read;
    while ((bytes_read = fread(buff, 1, BUFF_MAX, note)) > 0) {
        fwrite(buff, 1, bytes_read, stdout);
    }
    return 0;
}

// returns the next note name based on the files currently in the projectpath
// directory. Includes file extension.
char *_get_next_note_name(const char *projectpath) {
    long max = _get_last_entry_number(projectpath);
    if (max == -1) {
        exit_error("error geting next entry number", BADDIR);
        return NULL;
    }
    char extension[] = ".typ";
    int nextnote_s = sizeof('#') + _get_max_long_length() + sizeof(extension);
    char *nextnote = malloc(nextnote_s);
    if (!nextnote) {
        perror("error mallocing");
        return NULL;
    }
    int w = snprintf(nextnote, nextnote_s, "%s%ld%s", "#", max + 1,
                     extension);
    if (w >= nextnote_s || w < 0) {
        exit_error("error retrieving next note's name", 1);
        free(nextnote);
        return NULL;
    }
    return nextnote;
}

// returns the project note ready to be appended to, or NULL otherwise
FILE *_get_project_note(const char *projectpath, const char *projectname) {
    char finalpath[PATH_MAX];
    int w = snprintf(finalpath, sizeof(finalpath), "%s%s%s%s", projectpath, "/",
                     projectname, ".typ");
    if (w >= PATH_MAX || w < 0) {
        return NULL;
    }
    FILE *file = fopen(finalpath, "a");
    if (!file) {
        perror("Error opening file for note");
        return NULL;
    }
    return file;
}
