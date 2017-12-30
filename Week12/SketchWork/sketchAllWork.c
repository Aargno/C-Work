#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "display.h"

struct state {
  int xR;
  int xC;
  int yR;
  int yC;
  bool pen;
};

typedef struct state state;

typedef unsigned char byte; //Helps a bit with readability
typedef unsigned short twoByte;
typedef unsigned long fourByte;

state *initState() {
  state *s = malloc(sizeof(state));
  s->xR = 0;
  s->xC = 0;
  s->yR = 0;
  s->yC = 0;
  s->pen = false;
  return s;
}

void resetState(state *s) {
  s->xR = 0;
  s->xC = 0;
  s->yR = 0;
  s->yC = 0;
  s->pen = false;
}

void readBytes(byte *c, char *file) {
  int i = 1;
  FILE *in = fopen(file, "rb");
  fread(c+0, 1, 1, in);
  while (!feof(in)) {
    fread(c+i, 1, 1, in);
    i++;
  }
  fclose(in);
}

int fileLength(char *file) {
  int count = 0;
  char val;
  FILE *in = fopen(file, "rb");
  fread(&val, 1, 1, in);
  while (!feof(in)) {
    fread(&val, 1, 1, in);
    count++;
  }
  fclose(in);
  return count;
}

//Retrieves first two bits of byte provided
byte retrieveOp(byte c) {
   return c & 0xC0;
}

int retrieveByteNum(byte c) {
  return (c & 0x30) >> 4;
}

byte extendedOp(byte c) {
  return c & 0x0F;
}

twoByte packTwoBytes(byte a, byte b) {
  twoByte out = 0x0000;
  twoByte aT = a;
  twoByte bT = b;
  out = ((aT << 8) & 0xFF00) | out;
  out = (bT & 0x00FF) | out;
  return out;
}

fourByte packFourBytes(byte a, byte b, byte c, byte d) {
  fourByte out = 0x00000000;
  fourByte aT = a;
  fourByte bT = b;
  fourByte cT = c;
  fourByte dT = d;
  out = ((aT << 24) & 0xFF000000) | out;
  out = ((bT << 16) & 0x00FF0000) | out;
  out = ((cT << 8) & 0x0000FF00) | out;
  out = (dT & 0x000000FF) | out;
  return out;
}

int extendedArg(byte *cs, int num) {
  switch (num) {
    case 1: return (int8_t)cs[0];
    break;
    case 2: return (int16_t)packTwoBytes(cs[0], cs[1]);
    break;
    case 3: return (int32_t)packFourBytes(cs[0], cs[1], cs[2], cs[3]);
    break;
  }
  return 0;
}

//128 64 32 16 8 4 2 1
//0   0  1  1  1 1 1 1
//      3         F

int interpret6Bit(byte c, bool tComp) {
    if (!tComp) return (c & 0x3F);
    else {
      int num = c & 0x1F; //Gets first 5 bits of 2s Comp Num
      byte d = (c >> 5) & 0x01; //Gets the 6th bit of the passed byte
      if (d == 0x01) num -= 32; //If the 6th bit is set to 1, subtract 32
      return num;
    }
}

int retrieveArg(byte c, byte op) {
    switch (op) {
      case 0x00: return interpret6Bit(c, true);//Return twos complement
      break;
      case 0x40: return interpret6Bit(c, true);//Returns twos complement
      break;
      case 0x80: return interpret6Bit(c, false);//Returns unsigned val
      break;
      case 0xC0: return interpret6Bit(c, false);//Doesn't matter
      break;
    }
    return 0;
}

void xChange(int arg, state *s) { //Never draws, doesn't need display
  s->xC += arg;//Updates current position of X
}

void yChangeRem(int arg, state *s, display *d) { //Can draw needs diplay
  s->yC += arg; //Updates current of position of Y
  if (s->pen) line(d, s->xR, s->yR, s->xC, s->yC); // Draws line between current pos. of y, and remebered pos.
  s->xR = s->xC; //Remembers current position of X
  s->yR = s->yC; //Remembers current position of Y
}

int handleArg(int opBytes, byte cs[], int i) { //returns value i is to be incremented by in control
  byte *extend;
  switch (opBytes) {
    case 0: return 0;
    break;
    case 1: extend = malloc(sizeof(byte));
            extend[0] = cs[i+1];
            return extendedArg(extend, 1);
    break;
    case 2: extend = malloc(sizeof(byte) * 2);
            extend[0] = cs[i+1];
            extend[1] = cs[i+2];
            return extendedArg(extend, 2);
    break;
    case 3: extend = malloc(sizeof(byte) * 4);
            extend[0] = cs[i+1];
            extend[1] = cs[i+2];
            extend[2] = cs[i+3];
            extend[3] = cs[i+4];
            return extendedArg(extend, 3);
    break;
  }
  return 0;
}

int incrementHandle(int opBytes) {
  if (opBytes == 3) return 4;
  else return opBytes;
}

void call(byte op, int arg, state *s, display *d) {
  switch (op) {
    case 0x00: xChange(arg, s);
    break;
    case 0x40: yChangeRem(arg, s, d);
    break;
    case 0x80: pause(d, arg * 10); //Converts hundreths of second to milliseconds
    break;
  }
}

void extendedCall(byte op, int arg, state *s, display *d) {
  switch (op) {
    case 0xC0: xChange(arg, s);
    break;
    case 0xC1: yChangeRem(arg, s, d);
    break;
    case 0xC2: pause(d, arg * 10); //Converts hundreths of second to milliseconds
    break;
    case 0xC3: s->pen = !s->pen;
    break;
    case 0xC4: clear(d);
    break;
    case 0xC5: key(d);
    break;
    case 0xC6: colour(d, arg);
    break;
  }
}

//Will have control loop, to control looping through byte commands
void control(byte *c, int len, display *d) {
  state *s = initState();
  byte op;
  int opBytes;
  int arg;
  for (int i = 0; i < len; i++) {
    op = retrieveOp(c[i]);
    if (op == 0xC0) {
      opBytes = retrieveByteNum(c[i]);
      op = op | extendedOp(c[i]); //Combines extend op, and additional op not strictly necessary, but helps understanding
      arg = handleArg(opBytes, c, i);
      i += incrementHandle(opBytes);
      printf("OP: %x ARG: %d\n", op, arg);
      extendedCall(op, arg, s, d);
    }
    else {
      arg = retrieveArg(c[i], op);
      printf("OP: %x ARG: %d\n", op, arg);
      call(op, arg, s, d);
    }
  }
}

//Gets sketch file bytes, number of bytes, and initialises display
void draw(char *file) {
  int length = fileLength(file);
  byte *cmds = malloc(sizeof(char) * length);
  readBytes(cmds, file);
  display *d = newDisplay(file, 200, 200);
  control(cmds, length, d);
  end(d);
}

void testOp() {
  assert(retrieveOp(0x00) == 0x00);
  assert(retrieveOp(0x7F) == 0x40);
  assert(retrieveOp(0x9F) == 0x80);
  assert(retrieveOp(0xFF) == 0xC0);
}

void testArg() {
  assert(retrieveArg(0x01, 0x00) == 1);
  assert(retrieveArg(0x09, 0x00) == 9);
  assert(retrieveArg(0x10, 0x00) == 16);
  assert(retrieveArg(0x3F, 0x00) == -1);
  assert(retrieveArg(0x3E, 0x00) == -2);
  assert(retrieveArg(0x20, 0x00) == -32);
  assert(retrieveArg(0x01, 0x40) == 1);
  assert(retrieveArg(0x09, 0x40) == 9);
  assert(retrieveArg(0x10, 0x40) == 16);
  assert(retrieveArg(0x3F, 0x40) == -1);
  assert(retrieveArg(0x3E, 0x40) == -2);
  assert(retrieveArg(0x20, 0x40) == -32);
  assert(retrieveArg(0x01, 0x80) == 1);
  assert(retrieveArg(0x09, 0x80) == 9);
  assert(retrieveArg(0x10, 0x80) == 16);
  assert(retrieveArg(0x3F, 0x80) == 63);
  assert(retrieveArg(0x3E, 0x80) == 62);
  assert(retrieveArg(0x20, 0x80) == 32);
}

void testXChange() {
  state *s = initState();
  assert(s->xR == 0);
  assert(s->xC == 0);
  xChange(30, s);
  assert(s->xR == 0);
  assert(s->xC == 30);
  xChange(30, s);
  assert(s->xR == 0);
  assert(s->xC == 60);
  //Write discard function
}

void testYChange() {
  state *s = initState();
  display *d = newDisplay("test", 0, 0);
  assert(s->yR == 0);
  assert(s->yC == 0);
  yChangeRem(30, s, d);
  assert(s->yR == 30);
  assert(s->yC == 30);
  yChangeRem(30, s, d);
  assert(s->yR == 60);
  assert(s->yC == 60);
  assert(s->xR == 0);
  assert(s->xC == 0);
  xChange(30, s);
  assert(s->xR == 0);
  assert(s->xC == 30);
  yChangeRem(10, s, d);
  assert(s->xR == 30);
  assert(s->xC == 30);
  //Write discard function
}

void testByteNum() {
  assert(retrieveByteNum(0x00) == 0);
  assert(retrieveByteNum(0x10) == 1);
  assert(retrieveByteNum(0x20) == 2);
  assert(retrieveByteNum(0x30) == 3);
}

void testExtendOp() {
  assert(extendedOp(0xFC) == 0x0C);
  assert(extendedOp(0xFF) == 0x0F);
}

void testPack() {
  assert(packFourBytes(0x00, 0xFF, 0x00, 0xFF) == 0x00FF00FF);
  assert(packFourBytes(0xC0, 0xFF, 0x00, 0xFF) == 0xC0FF00FF);
  assert(packFourBytes(0xC0, 0xFF, 0xA0, 0xFF) == 0xC0FFA0FF);
  assert(packFourBytes(0x00, 0xFF, 0xA0, 0x00) == 0x00FFA000);
}

void test() {
  testOp();
  testArg();
  testXChange();
  testYChange();
  testByteNum();
  testExtendOp();
  testPack();
  printf("All tests pass\n");
}

int main (int argc, char **argv) {
    if (argc == 2) draw(argv[1]);
    else test();
}
