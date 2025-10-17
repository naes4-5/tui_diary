#include "src/dirs.c"
#include "src/includes.h"
#include "src/makenote.c"

void mkentry(FILE* note);

int main() {
  initdirs();
  FILE *note = mknote("daily", "thoughts");
  mkentry(note);
  fclose(note);
  return 0;
}

void mkentry(FILE *note) {
  char entry[4096];
  printf("Please write your entry below. Type 'enter' to terminate.\n\n");
  fgets(entry, sizeof(entry), stdin);
  fprintf(note, "%s\n", entry);
}
