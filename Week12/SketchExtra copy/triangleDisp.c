#include "display.h"
#include "triangleDisp.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

void swap(int array[], int i, int j) {
  int temp = array[i];
  array[i] = array[j];
  array[j] = temp;
}

void swapLD(long double array[], int i, int j) {
  long double temp = array[i];
  array[i] = array[j];
  array[j] = temp;
}

void sortRelative(int n, int array[], long double relArray[]) {
  if (n<=1) return;
  for (int i = 0; i < n-1; i++) {
    if(array[i] > array[n-1]){
      swap(array, i, n-1);
      swapLD(relArray, i, n-1);
    }
  }
  sortRelative(n-1, array, relArray);
}

//Gets the phorizontal coordinate that the upper tip of the triangle rests on
int findHorizPoint(int s, double rad) {
  //Using adjacent = cos(x) * hyp
  return cosl(rad) * s;
}

int findHeight(int s, double rad) {
  return ceil(sinl(rad) * s);
}

void displayTriangle(int s1, int s2, int s3,long double s1Rad, long double s2Rad, long double s3Rad) {
  //Provided triangle details are sorted, to maintain consistency in program results
  int coords[] = {s1,s2,s3};
  long double rads[] = {s1Rad,s2Rad,s3Rad};
  sortRelative(4, coords, rads);
  display *d = newDisplay("Triangle", 200, 200);
  int tipHoriz = findHorizPoint(coords[2], rads[1]);
  int height = findHeight(coords[1], rads[1]);
  //Treating s1 as base
  line(d, (100 - (coords[0] / 2)), (100 - (height / 2)), (100 + (coords[0] / 2)), (100 - (height / 2)));
  line(d, (100 + (coords[0] / 2)), (100 - (height / 2)), ((100 - (coords[0] / 2)) + tipHoriz), (100 + (height / 2)));
  line(d, ((100 - (coords[0] / 2)) + tipHoriz), (100 + (height / 2)), (100 - (coords[0] / 2)), (100 - (height / 2)));
  //Pause display for 2 seconds while rest of program ends
  end(d);
}

void getOps(byte ops[]) {
  ops[0] = 0x00; //Change x
  ops[1] = 0x40; //Change y/draw/remember
  ops[2] = 0x80; //Pause
  ops[3] = 0xC0; //Pen toggle
}

byte intTo6Bit(int x) {
  return x & 0x3F;
}

byte genCommands(byte op, byte arg) {
  op = op & 0xC0;
  arg = arg & 0x3F;
  return (op | arg);
}

void genBytes(byte bytes[], int coords[], int tipHoriz, int height) {
  byte ops[4];
  getOps(ops);
  byte penArg = intTo6Bit(3);
  // Describes following actions
  // line(d, 30, 30, (30 + coords[0]), 30);
  // line(d, (30 + coords[0]), 30, (30 + tipHoriz), (30 + height));
  // line(d, (30 + tipHoriz), (30 + height), 30, 30);
  bytes[0] = genCommands(ops[0], intTo6Bit(30));
  bytes[1] = genCommands(ops[1], intTo6Bit(30));
  bytes[2] = genCommands(ops[3], penArg);
  bytes[3] = genCommands(ops[0], intTo6Bit(coords[0]));
  bytes[4] = genCommands(ops[1], intTo6Bit(0));
  bytes[5] = genCommands(ops[0], intTo6Bit(-coords[0] + tipHoriz));
  bytes[6] = genCommands(ops[1], intTo6Bit(height));
  bytes[7] = genCommands(ops[0], intTo6Bit(-tipHoriz));
  bytes[8] = genCommands(ops[1], intTo6Bit(-height));
  bytes[9] = genCommands(ops[3], penArg);
}

void fileWrite(byte bytes[], int n) {
  FILE *write;

  write = fopen("triangle.sketch","wb");

  fwrite(bytes, 1, n, write);
  fclose(write);
}

void saveTriangleSketch(int s1, int s2, int s3, long double s1Rad, long double s2Rad, long double s3Rad) {
  int coords[] = {s1,s2,s3};
  long double rads[] = {s1Rad,s2Rad,s3Rad};
  sortRelative(4, coords, rads);
  int tipHoriz = findHorizPoint(coords[2], rads[1]);
  int height = findHeight(coords[1], rads[1]);
  byte bytes[10];
  genBytes(bytes, coords, tipHoriz, height);
  fileWrite(bytes, 10);
}

void testIntTo6Bit() {
  assert(intTo6Bit(-1) == 0x3F);
  assert(intTo6Bit(0) == 0x00);
  assert(intTo6Bit(2) == 0x02);
  assert(intTo6Bit(15) == 0x0F);
  assert(intTo6Bit(31) == 0x1F);
  assert(intTo6Bit(-32) == 0x20);
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

void testSort() {
  int coords[] = {3,6,1};
  long double rads[] = {0.2,0.1,0.5};
  sortRelative(3, coords, rads);
  assert(coords[0] == 1);
  assert(rads[0] == 0.5);
  assert(coords[1] == 3);
  assert(rads[1] == 0.2);
  assert(coords[2] == 6);
  assert(rads[2] == 0.1);
}

void dispTest() {
  testIntTo6Bit();
  testGenCommand();
  testSort();
  printf("All display tests pass\n");
}
