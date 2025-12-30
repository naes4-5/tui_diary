#pragma once

#include <stdio.h>

#define BUFF_MAX 4096

long _get_last_entry_number(const char *projectpath);
size_t _get_max_long_length();
int print_note(FILE *note);
char *_get_next_note_name(const char *projectpath);
FILE *_get_project_note(const char *projectpath, const char *projectname);
