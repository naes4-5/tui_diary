#include "src/dirs.c"
#include "src/includes.h"
#include "src/makenote.c"

void mkentry(FILE *note);

int main() {
  initdirs();
  FILE *note = mknote("daily", "thoughts");
  mkentry(note);
  fclose(note);
  //DIR *dir;
  
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
