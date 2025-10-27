#include "includes.h"

int initdirs() {
  if(mkdir("Diary", 0744) == -1) {
    fprintf(stderr, "Directory creation failed: %s\n", strerror(errno));
    return 1;
  }
  
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
