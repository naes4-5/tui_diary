#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *f = fopen("README.md", "w");
  char readme[100];
  sprintf(readme, "# Ts is the best diary you ever did see.");
  
  fputs(readme, f);

  printf("This has gotta be the best way to do ts frfr\n");
  fclose(f);
  return 0;
}
