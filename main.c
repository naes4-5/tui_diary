#include "src/includes.h"
#include "src/makenote.c"
#include <linux/limits.h>
#include <string.h>
#include <unistd.h>

void mkentry(FILE *note, const char *entry);
Operatin getOperation(int argc, char *argv[], char *title);
char *getArg(char *argv);
void initcheck(void);
int initdirs();

int main(int argc, char *argv[]) {
  char *title = malloc(16);
  memcpy(title, "", sizeof(""));
  Operatin operation = getOperation(argc, argv, title);
  initcheck();
  FILE *note = mknote(title);
  if (!note) {
    fprintf(stderr, "Error in opening, aborting.\n");
    free(title);
    return 1;
  }
  mkentry(note, argv[argc - 1]);

  fclose(note);
  free(title);
  return 0;
}

void mkentry(FILE *note, const char *entry) {
  if (note != NULL) fprintf(note, "%s\n", entry);
}

char *getArg(char *arg) {
  if (arg[0] != '-' || strlen(arg) < 2) {
    fprintf(stderr, "invalid argument");
    return NULL;
  }
  if (strlen(arg) == 2)
    return arg + 1;
  return arg + 2;
}

Operatin getOperation(int argc, char *argv[], char *title) {
  char *arg = malloc(8);
  memcpy(arg, argv[1], strlen(argv[1]));
  if (!strcmp(arg, "write")) {
    return WRITE;
  }
  return READ;
}

void initcheck(void) {
  char path[PATH_MAX];
  if (getcwd(path, sizeof(path)) == NULL) {
    fprintf(stderr, "Error occured: %s\n", strerror(errno));
  }

  DIR *d;
  struct dirent *entry;
  d = opendir(path);
  if (!d) {
    fprintf(stderr, "Error occured: %s\n", strerror(errno));
  }
  while ((entry = readdir(d)) != NULL) {
    char buf[64];
    sprintf(buf, "%s", entry->d_name);
    if (!strcmp(buf, "Diary")) {
      closedir(d);
      return;
    }
  }
  closedir(d);
  initdirs();
}

int initdirs() {
  if (mkdir("Diary", 0744) == -1) {
    fprintf(stderr, "Directory creation failed: %s\n", strerror(errno));
    return 1;
  }

  for (int i = 0; i < sizeof(months) / sizeof(months[0]); i++) {
    char dir[32];
    snprintf(dir, sizeof(dir), "Diary/%s", months[i]);
    if (mkdir(dir, 0744) == -1) {
      if (errno == EEXIST) {
        fprintf(stderr,
                "Directory creation failed, \"%s\" already exists: %s\n", dir,
                strerror(errno));
      } else {
        fprintf(stderr, "\"mkdir\" failed: %s\n", strerror(errno));
      }
    }
  }

  return 0;
}
