#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void lowerToUpper(char *c) {
  for (int i = 0; i < strlen(c); i++) {
    if (c[i] >= 'a' && c[i] <= 'z') c[i] = c[i] - 32;
  }
}

void upperFileConv(char *inp, char *outp) {
    int max = 100;
    char line[max];
    FILE *in = fopen(inp, "r");
    FILE *out = fopen(outp, "w");
    fgets(line, max, in);
    while (!feof(in)) {
      lowerToUpper(line);
      fprintf(out, "%s", line);
      fgets(line, max, in);
    }
    fclose(in);
    fclose(out);
}

int main(int argc, char **argv) {
  if (argc == 3) upperFileConv(argv[1], argv[2]);
}
