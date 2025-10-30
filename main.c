#include "src/dirs.c"
#include "src/includes.h"
#include "src/makenote.c"
#include <linux/limits.h>
#include <unistd.h>

void mkentry(FILE *note);
void initcheck(void);

int main() {
  initcheck();
  /*
  initdirs();
  FILE *note = mknote("daily", "thoughts");
  mkentry(note);
  fclose(note);
  */

  return 0;
}

void mkentry(FILE *note) {
  size_t size = 4096;
  char entry[size];
  printf("Please write your entry below. Type '|' + 'enter' to terminate.\n\n");
  char next;
  int i;
  for (i = 0; i < size && (next = getchar()) != '|'; ++i) {
    entry[i] = next;
  }
  // I genuinely don't understand why I need to have this much fluff to include
  // a newline on the note. It is way too much nonsense for all this.
  entry[i] = '\n';
  entry[i + 1] = '\0';
  fprintf(note, "%s\n", entry);
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
