#include "includes.h"
#include <time.h>

int getmonth() {
  time_t fill = time(NULL);
  struct tm *now = localtime(&fill);

  char *cmonth = malloc(3);
  strftime(cmonth, 3, "%m", now);
  int ret = strtol(cmonth, NULL, 10);
  
  free(cmonth);
  return ret;
}

void dailyheader(FILE *f) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  char *header = malloc(256);
  strftime(header, 256, "*%d/%m/%y @ %H:%M*", t);
  fprintf(f, "%s\n\n", header);
  free(header);
}

FILE *mknote(const char *isDaily, const char *title) {
  if (!strcmp("", title)) {
    fprintf(stderr, "Note must have a title\n");
    return NULL;
  }

  char notedir[64];
  snprintf(notedir, 64, "Diary/%s%s.md", months[getmonth() - 1], title);

  FILE *f = fopen(notedir, "w");
  if (f != NULL && !strcmp("daily", isDaily)) {
    dailyheader(f);
  } else if (f == NULL) {
    fprintf(stderr, "Error occurred: %s\n", strerror(errno));
  }
  return f;
}
