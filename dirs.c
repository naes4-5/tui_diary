#include "includes.h"

int initdirs() {
  if(mkdir("Diary", 0744) == -1) {
    fprintf(stderr, "Directory creation failed: %s\n", strerror(errno));
    return 1;
  }
  
  const char* months[] = {
    "1.January",
    "2.February",
    "3.March",
    "4.April",
    "5.May",
    "6.June",
    "7.July",
    "8.August",
    "9.September",
    "10.October",
    "11.November",
    "12.December"
  };

  for(int i = 0; i < sizeof(months)/sizeof(months[0]); i++) {
    char dir[32];
    snprintf(dir, sizeof(dir), "Diary/%s", months[i]);
    if(mkdir(dir, 0744) == -1) {
      if(errno == EEXIST) {
        fprintf(stderr, "Directory creation failed, \"%s\" already exists: %s\n", dir, strerror(errno));
      } else {
        fprintf(stderr, "\"mkdir\" failed: %s\n", strerror(errno));
      }
    }
  }
  
  return 0;
}
