#include "../include/writenote.h"
#include "../include/readwriteutils.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// returns 0 if everything went well.
int write_project_note(const char *projectpath, const char *projectname,
                       const char *message) {
    FILE *projectnote = _get_project_note(projectpath, projectname);
    if (!projectnote) {
        perror("error opening file");
        return 1;
    }
    if (fprintf(projectnote, "%s\n", message) < 0) {
        return 2;
    }

    fclose(projectnote);
    return 0;
}

// returns 0 if everything went well. Makes a new note with message written in
// it and the relevant path, indicated by notelevel
int write_normal_note(const char *projectpath, const char *message) {
    FILE *note = _get_next_normal_note(projectpath);
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
FILE *_get_next_normal_note(const char *projectpath) {
    char finalpath[PATH_MAX];
    char *notename = _get_next_note_name(projectpath);
    if (!notename) {
        perror("Error getting the header");
        return NULL;
    }
    int w = snprintf(finalpath, sizeof(finalpath), "%s%s%s", projectpath, "/",
                     notename);
    if (w >= PATH_MAX || w < 0) {
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

int write_from_file(const char *projectpath, const char *filename) {
    FILE *note = fopen(filename, "r");
    if (!note) {
        perror("Error opening file for reading");
        return 1;
    }
    char buff[BUFF_MAX];
    size_t bytes_read;
    while ((bytes_read = fread(buff, 1, BUFF_MAX, note)) > 0) {}
    fclose(note);
    if (buff[strlen(buff) - 1] == '\n') {
        buff[strlen(buff) - 1] = '\0';
    }
    int messlen = strlen(buff) + sizeof('\0');
    char *message = malloc(messlen);
    int w = snprintf(message, messlen, "%s", buff);
    if (w >= messlen || w < 0) {
        return 2;
    }
    int ret = write_normal_note(projectpath, message);
    free(message);
    return ret;
}
