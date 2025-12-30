#include "../include/readnote.h"
#include "../include/cltools.h"
#include "../include/readwriteutils.h"
#include <limits.h>
#include <stdlib.h>

#define BUFF_MAX 4096

int read_note(const char *projectpath) {
    char extension[] = ".typ";
    long entrynum = _get_last_entry_number(projectpath);
    int notename_s = sizeof('#') + _get_max_long_length() + sizeof(extension) +
                     sizeof('\0');
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
    char extension[] = ".typ";
    char finalpath[PATH_MAX];
    int w = snprintf(finalpath, PATH_MAX, "%s%c%s%s", projectpath, '/',
                     projectname, extension);
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
