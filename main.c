#include "src/makenote.c"
#include "src/dirs.c"
#include "src/includes.h"

int main() {
  initdirs();
  FILE* note = mknote("daily", "thoughts");

  return 0;
}
