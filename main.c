#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *f = fopen("README.md", "w");

  printf("I love balls yo, jk frfr");
  fclose(f);
  return 0;
}
