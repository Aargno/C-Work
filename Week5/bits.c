#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>

//Checks if a number is divisible by a given power of 2 0 If not, 1 if yes with remainder,
//2 if exact, -1 for error
int checkDiv(int n, int expo) {
    bool flag;
    flag = fabs(n / pow(2, expo)) >= 1;
    if (!flag) return 0;
    if (flag) {
      flag = fabs(n / pow(2, expo)) == 1;
      if (!flag) return 1;
      return 2;
    }
    return -1;
}

//Increments a binary number by 1
void binInc(char s[33]) {
    bool carry = true;

    for (int count = 31; count >= 0 && carry; count--) {
      if (s[count] == '0' && carry) {
          s[count] = '1';
          carry = false;
      } else if (s[count] == '1' && carry) {
          s[count] = '0';
      }
      if (count == 0 && carry) printf("Overflow error\n");
    }

}

//Flips all of the bits in a binary number (as I'm using a string I can't just not the number)
void binFlip(char s[33]) {
    for (int count = 0; count < 32; count++) {
      if (s[count] == '0') {
        s[count] = '1';
      } else if (s[count] == '1') {
        s[count] = '0';
      }
    }
}

//Convert an int into a binary string of 32 bits.
void binary(int n, char s[33]) {
    int expo = 31;
    int valN = abs(n);
    s[32] = '\0';

    for (int count = 0; count < 32; count++) {
      if (checkDiv(valN, expo) == 2 && count == 0) {
        s[count] = '1';
        for (count = 1; count < 32; count++) s[count] = '0';
        break;
      }
      if (checkDiv(valN, expo) == 1 || checkDiv(valN, expo) == 2) {
        s[count] = '1';
        valN = valN - pow(2, expo);
      } else {
        s[count] = '0';
      }

      expo--;
    }

    if (n < 0) {
      binFlip(s);
      binInc(s);
    }

}

char hexConv(char s[5]) {
    int expo = 3;
    int result = 0;
    for (int count = 0; count < 4; count++) {
        if (s[count] == '1') result = result + pow(2, expo);
        expo--;
    }
    // printf("%d\n", result);
    switch (result) {
      case 0: return '0';
      case 1: return '1';
      case 2: return '2';
      case 3: return '3';
      case 4: return '4';
      case 5: return '5';
      case 6: return '6';
      case 7: return '7';
      case 8: return '8';
      case 9: return '9';
      case 10: return 'A';
      case 11: return 'B';
      case 12: return 'C';
      case 13: return 'D';
      case 14: return 'E';
      case 15: return 'F';
    }
    return 'Z';
}

// Convert an int into a hex string of 8 hex digits.
void hex(int n, char s[9]) {
    int hexCount = 0;
    int splitCount = 0;
    char bin[33];
    char split[5];
    binary(n, bin);

    s[8] = '\0';
    split[4] = '\0';

    for (int count = 0; count < 32; count++) {
      split[splitCount] = bin[count];
      if (splitCount == 3) {
        s[hexCount] = hexConv(split);
        splitCount = 0;
        hexCount++;
        continue;
      }
      splitCount++;
    }

}

// Construct a colour from its components.
int colour(int r, int g, int b, int a) {
    int c = (r << 24) | (g << 16) | (b << 8) | a;
    return c;
}

// Unpack a colour into its components.
void components(int c, int rgba[4]) {
    rgba[0] = (c >> 24) & 0xFF;
    rgba[1] = (c >> 16) & 0xFF;
    rgba[2] = (c >> 8) & 0xFF;
    rgba[3] = c & 0xFF;
}

// Form a 3D point from three signed 10-bit integers.
int point(int x, int y, int z) {
    //0x3FF, 3 b16 in binary is 0011, elminates the two extra bits
    return ((x & 0x3FF) << 20 | (y & 0x3FF) << 10 | (z & 0x3FF));
}

// Separate a position into three signed 10-bit coordinates.
void coordinates(int p, int xyz[3]) {
    //0x3FF used to retrieve 10 relevant bits
    int x = (p >> 20) & 0x3FF;
    //0xFFFFFC00 used to set every non recovered bit to 1, C sets two unused bits
    //from the hex character
    if ((x & 0x200) != 0) x = 0xFFFFFC00 | x;
    xyz[0] = x;

    int y = (p >> 10) & 0x3FF;
    if ((y & 0x200) != 0) y = 0xFFFFFC00 | y;
    xyz[1] = y;

    int z = p & 0x3FF;
    if ((z & 0x200) != 0) z = 0xFFFFFC00 | z;
    xyz[2] = z;
}

// ----------------------------------------------------------------------------
// Testing and user interface.

// Do a single test of the binary function with given input and output.
// Initialise s with incorrect values to give predictable results.
bool checkBinary(int in, char out[33]) {
    char s[33];
    for (int i=0; i<33; i++) s[i] = 'x';
    binary(in, s);
    return strcmp(s, out) == 0;
}

// Tests 1 to 13
void testBinary() {
    assert(checkBinary(0, "00000000000000000000000000000000"));
    assert(checkBinary(1, "00000000000000000000000000000001"));
    assert(checkBinary(2, "00000000000000000000000000000010"));
    assert(checkBinary(3, "00000000000000000000000000000011"));
    assert(checkBinary(4, "00000000000000000000000000000100"));
    assert(checkBinary(100, "00000000000000000000000001100100"));
    assert(checkBinary(2147483647, "01111111111111111111111111111111"));
    assert(checkBinary(-1, "11111111111111111111111111111111"));
    assert(checkBinary(-2, "11111111111111111111111111111110"));
    assert(checkBinary(-3, "11111111111111111111111111111101"));
    assert(checkBinary(-4, "11111111111111111111111111111100"));
    assert(checkBinary(-100, "11111111111111111111111110011100"));
    assert(checkBinary(-2147483648, "10000000000000000000000000000000"));
}

// Do a single test of the hex function with given input and output.
bool checkHex(int in, char out[9]) {
    char s[9];
    for (int i=0; i<9; i++) s[i] = 'x';
    hex(in, s);
    return strcmp(s, out) == 0;
}

// Tests 14 to 28
void testHex() {
    assert(checkHex(0, "00000000"));
    assert(checkHex(1, "00000001"));
    assert(checkHex(2, "00000002"));
    assert(checkHex(10, "0000000A"));
    assert(checkHex(15, "0000000F"));
    assert(checkHex(16, "00000010"));
    assert(checkHex(100, "00000064"));
    assert(checkHex(2147483647, "7FFFFFFF"));
    assert(checkHex(-1, "FFFFFFFF"));
    assert(checkHex(-2, "FFFFFFFE"));
    assert(checkHex(-3, "FFFFFFFD"));
    assert(checkHex(-4, "FFFFFFFC"));
    assert(checkHex(-16, "FFFFFFF0"));
    assert(checkHex(-100, "FFFFFF9C"));
    assert(checkHex(-2147483648, "80000000"));
}

// Tests 29 to 33
void testColour() {
    assert(colour(255,0,0,0) == 0xFF000000);
    assert(colour(0,255,0,0) == 0x00FF0000);
    assert(colour(0,0,255,0) == 0x0000FF00);
    assert(colour(0,0,0,255) == 0x000000FF);
    assert(colour(1,2,3,4) == 0x01020304);
}

// Do a single test of the components function with given input and output.
bool checkComponents(int n, int r, int g, int b, int a) {
    int rgba[4];
    for (int i=0; i<4; i++) rgba[i] = -1;
    components(n, rgba);
    return rgba[0] == r && rgba[1] == g && rgba[2] == b && rgba[3] == a;
}

// Tests 34 to 38
void testComponents() {
    assert(checkComponents(0xFF000000, 255, 0, 0, 0));
    assert(checkComponents(0x00FF0000, 0, 255, 0, 0));
    assert(checkComponents(0x0000FF00, 0, 0, 255, 0));
    assert(checkComponents(0x000000FF, 0, 0, 0, 255));
    assert(checkComponents(0x01020304, 1, 2, 3, 4));
}

// Tests 39 to 44
void testPoint() {
    assert(point(0,0,0) == 0);
    assert(point(1,3,7) == 0x00100C07);
    assert(point(1,3,-7) == 0x00100FF9);
    assert(point(1,-3,7) == 0x001FF407);
    assert(point(-1,3,7) == 0x3FF00C07);
    assert(point(-1,-3,-7) == 0x3FFFF7F9);
}

// Do a single test of the coordinates function with given input and output.
bool checkCoordinates(int p, int x, int y, int z) {
    int xyz[3];
    for (int i=0; i<3; i++) xyz[i] = 1000000000;
    coordinates(p, xyz);
    return xyz[0] == x && xyz[1] == y && xyz[2] == z;
}

// Tests 45 to 50
void testCoordinates() {
    assert(checkCoordinates(0, 0, 0, 0));
    assert(checkCoordinates(0x00100C07, 1, 3, 7));
    assert(checkCoordinates(0x00100FF9, 1, 3, -7));
    assert(checkCoordinates(0x001FF407, 1, -3, 7));
    assert(checkCoordinates(0x3FF00C07, -1, 3, 7));
    assert(checkCoordinates(0x3FFFF7F9, -1, -3, -7));
}

void test() {
    testBinary();
    testHex();
    testColour();
    testComponents();
    testPoint();
    testCoordinates();
    printf("All tests pass");
}

// Print a number in binary.
void printBinary(int n) {
    char s[33];
    binary(n, s);
    printf("%s\n", s);
}

// Print a number in hex.
void printHex(int n) {
    char s[9];
    hex(n, s);
    printf("%s\n", s);
}

// Print a number in hex or binary, or run tests.
int main(int n, char *args[n]) {
    if (n == 1) test();
    else if (n == 2) printHex(atoi(args[1]));
    else if (n == 3 && strcmp(args[1],"-b")==0) printBinary(atoi(args[2]));
    else printf("Use:   ./bits   or   ./bits n   or   ./bits -b n");
    return 0;
}
