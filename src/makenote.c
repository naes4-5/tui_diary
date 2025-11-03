#include "includes.h"
#include <time.h>

int getmonth() {
  time_t fill = time(NULL);
  struct tm *now = localtime(&fill);
  return now->tm_mon;
}

char *dailyheader() {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  char *header = malloc(256);
  strftime(header, 256, "*%d-%m-%y@%H:%M*", t);
  return header;
}

FILE *mknote(bool daily, const char *title) {
  if (!strcmp("", title)) {
    fprintf(stderr, "Note must have a title\n");
    return NULL;
  }

  const char *header = daily ? dailyheader() : NULL;
  if (daily && header == NULL) {
    fprintf(stderr, "Error in creating header\n");
    return NULL;
  }
  char notedir[256];
  
  int written = snprintf(notedir, sizeof(notedir), "Diary/");
  if (written < 0 || written >= sizeof(notedir)) {
    free((void *)header);
    return NULL;
  }

  if (header) {
    written += snprintf(notedir + written, sizeof(notedir) - written, "%s", header);
    if (written >= sizeof(notedir)) {
      free((void *)header);
      return NULL;
    }
  }

  snprintf(notedir + written, sizeof(notedir) - written, "%s%s.typ", months[getmonth()], title);

  FILE *f = fopen(notedir, "w");
  if (!f) {
    fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    return NULL;
  }
  if (header) {
    fprintf(f, "%s\n\n", header);
  }
  free((void *)header);
  return f;
}
