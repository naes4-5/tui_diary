#pragma once

typedef enum Level { PROJECT, NORMAL, AMEND, FFILE, INVALID, BACK } note_t;
typedef enum Operation { WRITE, READ, NOOP } operation;
typedef enum ExitType { NONE, BADOPERATION, BADFLAG, NOREPO, BADDIR } exit_t;
