#pragma once

#include "includes.h"
#include <stdio.h>

int write_project_note(const char *projectpath, const char *projectname,
                       const char *message);
int write_normal_note(const char *projectpath, const char *message);
FILE *_get_next_normal_note(const char *projectpath);
