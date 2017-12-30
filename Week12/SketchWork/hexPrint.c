#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void hexPrint(char *c) {
  unsigned char val;
  FILE *in = fopen(c, "rb");
  fread(&val, 1, 1, in);
  while (!feof(in)) {
    printf("%x\n", val);
    fread(&val, 1, 1, in);
  }
  fclose(in);
}

int interpret6Bit() {
    unsigned char c = 0xE0;
    // if (!2Comp) return (c & 0x3F);
    // else {
      int num = c & 0x1F;
      unsigned char d = (c >> 5) & 0x01;
      if (d == 0x01) num -= 32;
      printf("%d\n", num);
    // }
    return 1;
}

int main(int argc, char **argv) {
  interpret6Bit();
  // if (argc == 2) hexPrint(argv[1]);
}
