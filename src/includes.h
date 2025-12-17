#pragma once

#include <dirent.h>
#include <errno.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static const char *months[] = {"1.January/",  "2.February/",  "3.March/",
                               "4.April/",    "5.May/",       "6.June/",
                               "7.July/",     "8.August/",    "9.September/",
                               "10.October/", "11.November/", "12.December/"};

typedef enum Flag { PROJECT, NORMAL, INVALID } FlgTyp;

typedef enum Operation { WRITE, READ, NOOP } Operatin;
