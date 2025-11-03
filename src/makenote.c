#include "includes.h"
#include <time.h>

const char *days[7] = {
  "Sunday", "Monday", "Tuesday",
  "Wednesday", "Thursday", 
  "Friday", "Saturday"
};

int getmonth() {
  time_t fill = time(NULL);
  struct tm *now = localtime(&fill);
  return now->tm_mon;
}

int getday() {
  time_t fill = time(NULL);
  struct tm *now = localtime(&fill);
  return now->tm_mday;
}

char *dailyheader() {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  char *header = malloc(256);
  strftime(header, 256, "Made on %d-%m-%y, %H:%M", t);
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
  printf("Header: %s\n", header);
  
  char notedir[256];
  int written = snprintf(notedir, sizeof(notedir), "Diary/");
  if (written < 0 || written >= sizeof(notedir)) {
    free((void *)header);
    return NULL;
  }

  written += snprintf(notedir + written, sizeof(notedir) - written, "%s%s", months[getmonth() - 1], title);
  if (daily) {
    written += snprintf(notedir + written, sizeof(int) + 1, ":%d", getday());
  }
  written += snprintf(notedir + written, sizeof(notedir) - written, ".typ");

  FILE *f = fopen(notedir, "w");
  if (f == NULL) {
    fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    free((void *) header);
    return NULL;
  }
  if (header) {
    fprintf(f, "%s\n\n", header);
  }
  free((void *)header);
  return f;
}
