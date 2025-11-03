#include "src/dirs.c"
#include "src/includes.h"
#include "src/makenote.c"
#include <linux/limits.h>
#include <unistd.h>

void mkentry(FILE *note);
void initcheck(void);

int main(int argc, char *argv[]) {
  bool daily = true;
  initcheck();
  FILE *note = mknote(daily, "thoughts");
  if (!note) {
    fprintf(stderr, "Error in opening, aborting.\n");
    return 1;
  }
  mkentry(note);
  fclose(note);

  return 0;
}

void mkentry(FILE *note) {
  size_t size = 4096;
  char *entry = malloc(size);
  if (!entry) {
    fprintf(stderr, "Error in allocation: %s\n", strerror(errno));
    return;
  }
  printf("Please write your entry below. Type '|' + 'enter' to terminate.\n\n");
  int next;
  int i;
  for (i = 0; i < size - 2 && (next = getchar()) != '|' && next != EOF; ++i) {
    entry[i] = (char)next;
  }
  entry[i++] = '\n';
  entry[i] = '\0';
  fwrite(entry, sizeof(char), i, note);
  free(entry);
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
