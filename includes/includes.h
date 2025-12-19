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

typedef enum Level { PROJECT, NORMAL, INVALID } note_t;
typedef enum Operation { WRITE, READ, NOOP } operation;
typedef enum ExitType { BADOPERATION = 1, BADFLAG, NOREPO, BADDIR } exit_t;
