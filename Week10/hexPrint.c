#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void hexPrint(char *c) {
  unsigned int val;
  FILE *in = fopen(c, "rb");
  fread(&val, 1, 1, in);
  while (!feof(in)) {
    printf("%x\n", val);
    fread(&val, 1, 1, in);
  }
  fclose(in);
}

int main(int argc, char **argv) {
  if (argc == 2) hexPrint(argv[1]);
}
