#pragma once

#include "includes.h"
#include <dirent.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

int exit_error(char *to_print, exit_t code);
note_t get_note_type_write(int argc, char *argv[]);
note_t get_note_type_read(int argc, char *argv[]);
operation get_operation(int argc, char *argv[]);
exit_t check_env(const char *homepath, char dierypath[],
                 const char *projectname, char projectpath[],
                 const char *configpath, DIR *projectdir);
exit_t check_operation(int argc, char *argv[], operation op, note_t *notelevel);
