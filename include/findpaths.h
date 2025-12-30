#pragma once 

#include <dirent.h>

char *config_path();
char *get_project_name();
DIR *get_project_dir(const char *homePath, const char *projectname);
DIR *_make_project_dir(DIR *d, const char *projectname, char *pathtodiery);
