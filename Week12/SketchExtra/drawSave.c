#include "display.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct state {
  int xR;
  int xC;
  int yR;
  int yC;
  bool pen;
};

typedef struct state state;

typedef unsigned char byte;

void clearStrings(char *c1, char *c2, char *c3) {
    for (int i = 0; i < strlen(c1); i++) c1[i] = ' ';
    for (int i = 0; i < strlen(c2); i++) c2[i] = ' ';
    for (int i = 0; i < strlen(c3); i++) c3[i] = ' ';
}

void clearTwoStrings(char *c1, char *c2) {
    for (int i = 0; i < strlen(c1); i++) c1[i] = ' ';
    for (int i = 0; i < strlen(c2); i++) c2[i] = ' ';
}

void removeSpace(char *c1) {
  for (int i = 0; i < strlen(c1); i++) if (c1[i] == ' ') c1[i] = '\0';
}

void removeGarbage(char *c1) {
  for (int i = 0; i < strlen(c1); i++) {
    if (!(c1[i] >= 'a' && c1[i] <= 'z') && !(c1[i] >= 'A' && c1[i] <= 'Z')) c1[i] = '\0';
  }
}

state *initState() {
  state *s = malloc(sizeof(state));
  s->xR = 0;
  s->xC = 0;
  s->yR = 0;
  s->yC = 0;
  s->pen = false;
  return s;
}

//Specific function to split user input into two sections around space
void splitInput(char *c1, char *c2, char *input) {
    int i = 0;
    int j;
    for (i = 0; i < strlen(input) && input[i] != ' ' && input[i] != '\n'; i++) {//Tests for new line character, to account for string with no space
        if (input[i] != '\n' && input[i] != '\0') c1[i] = input[i];
    }
    removeSpace(c1);
    removeGarbage(c1);
    i++;//Moves past the space in the input
    for (j = 0; i < strlen(input) && input[i] != ' '; i++) {
        if (input[i] != '\n' && input[i] != '\0') c2[j] = input[i];
        j++;
    }
    removeSpace(c2);
}

bool isDigit(const char *a) {
    int x;
    x = atoi(a);

    if (x == 0 && strcmp(a, "0") != 0) {
      return false;
    }
    return true;
}

bool rangeCheck(int arg, int low, int up) {
  return (arg >= low && arg <= up);
}

byte intTo6Bit(int x) {
  return x & 0x3F;
}

byte genCommands(byte op, byte arg) {
  op = op & 0xC0;
  arg = arg & 0x3F;
  return (op | arg);
}

void getOps(byte ops[]) {
  ops[0] = 0x00; //Change x
  ops[1] = 0x40; //Change y/draw/remember
  ops[2] = 0x80; //Pause
  ops[3] = 0xC0; //Pen toggle
}

void xChange(int arg, state *s) { //Never draws, doesn't need display
  s->xC += arg;//Updates current position of X
}

void yChange(int arg, state *s, display *d) { //Can draw needs diplay
  s->yC += arg; //Updates current of position of Y
  if (s->pen) line(d, s->xR, s->yR, s->xC, s->yC); // Draws line between current pos. of y, and remebered pos.
  s->xR = s->xC; //Remembers current position of X
  s->yR = s->yC; //Remembers current position of Y
}

byte call(state *s, char *cmd, int arg, display *d) {
    byte ops[4];
    getOps(ops);
    if (strcmp(cmd, "xChange") == 0) {
       if (rangeCheck(arg, -32, 31)) {
         xChange(arg, s);
         return genCommands(ops[0], intTo6Bit(arg));
       }
       else (printf("Invalid argument, must be (-32 -> 31)\n"));
    }
    if (strcmp(cmd, "yChange") == 0) {
      if (rangeCheck(arg, -32, 31)) {
        yChange(arg, s, d);
        return genCommands(ops[1], intTo6Bit(arg));
      }
      else (printf("Invalid argument, must be (-32 -> 31)\n"));
    }
    if (strcmp(cmd, "pause") == 0) {
      if (rangeCheck(arg, 0, 63)) {
        pause(d, arg*10);
        return genCommands(ops[2], intTo6Bit(arg));
      }
      else (printf("Invalid argument, must be (0 -> 63)\n"));
    }
    if (strcmp(cmd, "penTog") == 0) {
      s->pen = !s->pen;
      return genCommands(ops[3], intTo6Bit(3));
    }
    if (strcmp(cmd, "help") == 0) {
        printf("Available Commands Are:\n"
                "xChange - Adds arg to current x position (-32 -> 31)\n"
                "yChange - Adds arg to current y position, if pen is toogle on, a line is drawn, and the new pos. is remembered (-32 -> 31)\n"
                "pause - Pauses display for arg in milliseconds (0 -> 63)\n"
                "penTog - Toggles pen on/off\n"
                "Save - Saves the current drawing as a sketch file, cant save with no commands entered\n"
                "Exit - Closes the program without saving sketch\n");
    }
    return 0xC0; //Treated as invalid return value, could never be returned by any valid command
}

void fileWrite(byte bytes[], int len, char *name) {
  FILE *write;
  strcat(name, ".sketch");
  write = fopen(name, "wb");
  fwrite(bytes, 1, len, write);
  fclose(write);
}

void trimName(char *name, char *in) {
  int i;
  for (i = 0; i < strlen(in); i++) {
    if (in[i] != '\n' && in[i] != ' ') name[i] = in[i];
    else break;
  }
  name[i] = '\0';
}

bool validName(char *name) {
  for (int i = 0; i < strlen(name); i++) {
    if (!(name[i] >= 'a' && name[i] <= 'z') && !(name[i] >= 'A' && name[i] <= 'Z')) return false;
  }
  return true;
}

void saveFile(byte *bytes, int len) {
  char *in = malloc(10);
  char *name = malloc(20);
  do {
    clearTwoStrings(in, name);
    printf("Please enter a name for your sketch (Only use a-z)\n"
           "Any spaces entered into the name will cause the name to be cut off at that space.\n");
    fgets(in, 15, stdin);
    trimName(name, in);
    removeGarbage(name);
  } while(!validName(name));
  fileWrite(bytes, len, name);
}

void drawControl() {
    display *d = newDisplay("Record", 200, 200);
    state *s = initState();
    int len = 0;
    byte *bytes = malloc(0);
    byte instruction;
    char *in = malloc(17);
    char *cmd = malloc(9);
    char *arg = malloc(5);

    while (strcmp(cmd, "Save") != 0 && strcmp(cmd, "Exit") != 0) {
      clearStrings(cmd, arg, in);
      printf("\nPlease enter a command followed by an argument (within the range specified by help),\n"
      "invalid commands won't be accepted.\n"
      "If no argument is required, please only enter the command.\n"
      "Please enter 'help' for a list of commands\nEnter 'Exit' to close the program\n");
      fgets(in, 19, stdin);
      splitInput(cmd, arg, in);
      if (isDigit(arg) && strcmp(cmd, "Exit") != 0 && strcmp(cmd, "Save") != 0) instruction = call(s, cmd, atoi(arg), d);
      else if (strcmp(cmd, "Exit") != 0 && strcmp(cmd, "Save") != 0) instruction = call(s, cmd, 0, d);
      else if (strcmp(cmd, "Save") == 0 && len != 0) saveFile(bytes, len);
      if (instruction != 0xC0) {
        bytes = realloc(bytes, (++len) * sizeof(byte));
        bytes[len - 1] = instruction;
      }
    }
    end(d);
}

void testIntTo6Bit() {
  assert(intTo6Bit(-1) == 0x3F);
  assert(intTo6Bit(0) == 0x00);
  assert(intTo6Bit(2) == 0x02);
  assert(intTo6Bit(15) == 0x0F);
  assert(intTo6Bit(31) == 0x1F);
  assert(intTo6Bit(-32) == 0x20);
}

void testValidName() {
  assert(validName("test") == true);
  assert(validName("TeSt") == true);
  assert(validName("1234567") == false);
}

void testTrimName() {
  char test[100] = "test";
  char out[100];
  trimName(out, test);
  assert(strcmp(out, "test") == 0);
  strcpy(test, "test name");
  strcpy(out, "");
  trimName(out, test);
  assert(strcmp(out, "test") == 0);
}

void testRangeCheck() {
  assert(rangeCheck(15, 15, 25) == true);
  assert(rangeCheck(20, 15, 25) == true);
  assert(rangeCheck(25, 15, 25) == true);
  assert(rangeCheck(10, 15, 25) == false);
  assert(rangeCheck(30, 15, 25) == false);
}

void testGenCommand() {
  byte ops[4];
  getOps(ops);
  assert(genCommands(ops[0], 0x0F) == 0x0F);
  assert(genCommands(ops[1], 0x4F) == 0x4F);
  assert(genCommands(ops[2], 0x8F) == 0x8F);
  assert(genCommands(ops[3], 0xCF) == 0xCF);
  assert(genCommands(ops[0], 0x1F) == 0x1F);
  assert(genCommands(ops[1], 0x1F) == 0x5F);
  assert(genCommands(ops[2], 0x1F) == 0x9F);
  assert(genCommands(ops[3], 0x1F) == 0xDF);
}

void test() {
  testIntTo6Bit();
  testValidName();
  testTrimName();
  testRangeCheck();
  testGenCommand();
  printf("All tests pass\n");
}

int main (int argc, char **argv) {
    if (argc == 2) {
      if (strcmp(argv[1], "test") == 0) test();
    }
    else drawControl();
}
