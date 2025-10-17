#include "includes.h"
#include <time.h>

void daily(FILE *f) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  char *header;
  strftime(header, 256, "%d/%m/%y @ %H:%S", t);
  fprintf(f, "%s\n\n", header);
}

void mknote(const char *isDaily) {

  FILE *f;

  if (strcmp("daily", isDaily)) {
    daily(f);
  }
  fclose(f);
}
