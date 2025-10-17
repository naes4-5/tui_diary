#include "includes.h"
#include <time.h>

const char *months[] = {"1.January/",  "2.February/",  "3.March/",
                        "4.April/",    "5.May/",       "6.June/",
                        "7.July/",     "8.August/",    "9.September/",
                        "10.October/", "11.November/", "12.December/"};
int getmonth() {
  time_t fill = time(NULL);
  struct tm *now = localtime(&fill);

  char *cmonth = malloc(3);
  strftime(cmonth, 3, "%m", now);

  return strtol(cmonth, NULL, 10);
}

void daily(FILE *f) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  
  char *header;
  strftime(header, 256, "%d/%m/%y @ %H:%M", t);
  fprintf(f, "%s\n\n", header);
}

void mknote(const char *isDaily) {

  char notedir[64];
  snprintf(notedir, 64, "Diary/%s", months[getmonth()-1]);

  FILE *f;
  if (strcmp("daily", isDaily)) {
    daily(f);
  }
  fclose(f);
}
