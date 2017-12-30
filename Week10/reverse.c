#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void reverse(char *in, char *out) {
  int j = strlen(in) - 1;
  for (int i = 0; i < strlen(in); i++) {
    if (in[j] == '\n') i--; //Makes it so, \n wrriten into output string is overwritten on next loop
    out[i] = in[j];
    j--;
  }
}

void askInput() {
  char *in = malloc(100);
  char *out = malloc (100);
  bool end = false;
  while (!end) {
    printf("> ");
    if (fgets(in, 102, stdin) == NULL) end = true; //fgets returns NULL when EOF is encountered
    if (strcmp(in, ".\n") == 0) end = true; //\n accounts for end of line char included in string
    reverse(in, out);
    printf("%s\n", out);
  }
}

int main(int argc, char **argv) {
  askInput();
}
