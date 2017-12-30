/* Classify a triangle according to the integer lengths of its sides. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

// Integer constants representing types of triangle.
enum { Equilateral, Isosceles, Right, Scalene, Flat, Impossible, Illegal };

//Square the integer provided to it
int square(int x) {
  return x * x;
}

//Squares long provided to it
long squareLong(long x) {
  return x * x;
}

//Checks if only digits are present in character array
bool isDigit(const char a[]) {
    int x;
    x = atoi(a);

    if (x == 0 && strcmp(a, "0") != 0) {
      return false;
    }
    return true;
}

//Checks the general validity of the string entered by the user
bool stringTest(const char a[]) {
    int length;
    length = strlen(a);

    for (int i = 0; i < length; i++) {
      if (a[i] > '9' || a[i] < '0') {
        return false;
      }
    }

    if (length > 1 && a[0] == '0') {
      return false;
    }

    return true;
}

//Checks if the provided measurements provide an equilateral triangle
bool equilateral(const int s1, const int s2, const int s3) {
    if (s1 == s2 && s2 == s3) {
      return true;
    }
    return false;
}

//Same as equilateral, but meant to handle values that require long type
bool equilateralLong(const long s1, const long s2, const long s3) {
    if (s1 == s2 && s2 == s3) {
      return true;
    }
    return false;
}

//Checks if provided measurements provide scalene triangle
bool scalene(const int s1, const int s2, const int s3) {
    if (s1 != s2 && s2 != s3 && s1 != s3) {
      return true;
    }
    return false;
}

//Same as scalene check, but designed to handle values that require long type
bool scaleneLong(const long s1, const long s2, const long s3) {
    if (s1 != s2 && s2 != s3 && s1 != s3) {
      return true;
    }
    return false;
}

//Checks if provided measurements make isosceles triangle
bool isosceles(const int s1, const int s2, const int s3) {
    if (s1 == s2 || s2 == s3 || s1 == s3) {
      return true;
    }
    return false;
}

//Same as isosceles check, but designed to handle values that require long type
bool isoscelesLong(const long s1, const long s2, const long s3) {
    if (s1 == s2 || s2 == s3 || s1 == s3) {
      return true;
    }
    return false;
}

//Checks if measurements provided make right angles triangle
bool right(const int s1, const int s2, const int s3) {
    if (square(s1) + square(s2) == square(s3)) {
      return true;
    }
    if (square(s2) + square(s3) == square(s1)) {
      return true;
    }
    if (square(s1) + square(s3) == square(s2)) {
      return true;
    }
    return false;
}

//Same as right angle check, but designed to handle values that require long type
bool rightLong(const long s1, const long s2, const long s3) {
    if (squareLong(s1) + squareLong(s2) == squareLong(s3)) {
      return true;
    }
    if (squareLong(s2) + squareLong(s3) == squareLong(s1)) {
      return true;
    }
    if (squareLong(s1) + squareLong(s3) == squareLong(s2)) {
      return true;
    }
    return false;
}

//Checks if values provided make flat triangle
bool flat(const int s1, const int s2, const int s3) {
    if(s1 + s2 == s3 || s2 + s3 == s1 || s1 + s3 == s2) {
      return true;
    }
    return false;
}

//Same as flat check, but designed to handle values that require long type
bool flatLong(const long s1, const long s2, const long s3) {
    if(s1 + s2 == s3 || s2 + s3 == s1 || s1 + s3 == s2) {
      return true;
    }
    return false;
}

//Checks if values provided can't make a triangle (two sides don't sum to be greater than other)
bool impossible(const int s1, const int s2, const int s3) {
    if (s1 + s2 < s3 || s2 + s3 < s1 || s1 + s3 < s2) {
      return true;
    }
    return false;
}

//Same as impossible check, but designed to handle values that require long type
bool impossibleLong(const long s1, const long s2, const long s3) {
    if (s1 + s2 < s3 || s2 + s3 < s1 || s1 + s3 < s2) {
      return true;
    }
    return false;
}

//Checks if illegal values have been provided
bool illegal(const int s1, const int s2, const int s3) {
    if (s1 <= 0 || s2 <= 0 || s3 <= 0) {
      return true;
    }
    return false;
}

//Same as illegal check, but designed to handle values that require long type
bool illegalLong(const long s1, const long s2, const long s3) {
    if (s1 <= 0 || s2 <= 0 || s3 <= 0) {
      return true;
    }
    return false;
}

//Checks if the number provided exceeds the maximum int value
bool largeNumCheck(const char a[]) {
    long s1;

    if (strlen(a) > 10) {
      return true;
    }

    s1 = atol(a);

    if (s1 > INT_MAX) {
      return true;
    }

    return false;
}

//Checks to see if the values provided could overflow when used with normal ints
bool overflowPotential(const char sa[], const char sb[], const char sc[]) {
    long s1, s2, s3;

    s1 = atol(sa);
    s2 = atol(sb);
    s3 = atol(sc);

    if (s1 + s2 > INT_MAX || s2 + s3 > INT_MAX || s1 + s3 > INT_MAX) {
      return true;
    }

    return false;
}

//Runs all triangle checks assuming int data type will be large enough to store sides
int intTriCheck(const char sa[], const char sb[], const char sc[]) {
    int s1, s2, s3;
    if (!isDigit(sa) || !isDigit(sb) || !isDigit(sc)) {
      return Illegal;
    }

    s1 = atoi(sa);
    s2 = atoi(sb);
    s3 = atoi(sc);

    if (!stringTest(sa) || !stringTest(sb) || !stringTest(sc)) {
      return Illegal;
    }
    if (illegal(s1,s2,s3)) {
      return Illegal;
    }
    if (impossible(s1,s2,s3)) {
      return Impossible;
    }
    if (equilateral(s1,s2,s3)) {
      return Equilateral;
    }
    if (isosceles(s1,s2,s3)) {
      return Isosceles;
    }
    if (right(s1,s2,s3)) {
      return Right;
    }
    if (flat(s1,s2,s3)) {  //All flats scalene, not all scalene flat
      return Flat;
    }
    if (scalene(s1,s2,s3)) {
      return Scalene;
    }
    return Impossible;
}

//Runs all triangle checks assuming long data type required to store measurements
int longTriCheck(const char sa[], const char sb[], const char sc[]) {
    long s1, s2, s3;
    if (!isDigit(sa) || !isDigit(sb) || !isDigit(sc)) {
      return Illegal;
    }

    s1 = atol(sa);
    s2 = atol(sb);
    s3 = atol(sc);

    if (!stringTest(sa) || !stringTest(sb) || !stringTest(sc)) {
      return Illegal;
    }
    if (illegalLong(s1,s2,s3)) {
      return Illegal;
    }
    if (impossibleLong(s1,s2,s3)) {
      return Impossible;
    }
    if (equilateralLong(s1,s2,s3)) {
      return Equilateral;
    }
    if (isoscelesLong(s1,s2,s3)) {
      return Isosceles;
    }
    if (rightLong(s1,s2,s3)) {
      return Right;
    }
    if (flatLong(s1,s2,s3)) {  //All flats scalene, not all scalene flat
      return Flat;
    }
    if (scaleneLong(s1,s2,s3)) {
      return Scalene;
    }
    return Impossible;
}

// Classify a triangle, given side lengths as strings:
int triangle(const char sa[], const char sb[], const char sc[]) {
    if (largeNumCheck(sa) || largeNumCheck(sb) || largeNumCheck(sc)) {
      return Illegal;
    }

    if (overflowPotential(sa,sb,sc)) {
      return longTriCheck(sa,sb,sc);
    } else {
      return intTriCheck(sa,sb,sc);
    }
}

// -----------------------------------------------------------------------------
// User interface and testing.

void print(int type) {
    switch (type) {
        case Equilateral: printf("Equilateral"); break;
        case Isosceles: printf("Isosceles"); break;
        case Right: printf("Right"); break;
        case Scalene: printf("Scalene"); break;
        case Flat: printf("Flat"); break;
        case Impossible: printf("Impossible"); break;
        case Illegal: printf("Impossible"); break;
    }
    printf("\n");
}

// Check that you haven't changed the triangle type constants.  (If you do, it
// spoils automatic marking, when your program is linked with a test program.)
void checkConstants() {
    assert(Equilateral==0 && Isosceles==1 && Right==2 && Scalene==3);
    assert(Flat==4 && Impossible==5 && Illegal==6);
}

// Tests 1 to 2: check equilateral
void testEquilateral() {
    assert(triangle("8", "8", "8") == Equilateral);
    assert(triangle("1073", "1073", "1073") == Equilateral);
}

// Tests 3 to 5: check isosceles
void testIsosceles() {
    assert(triangle("25", "25", "27") == Isosceles);
    assert(triangle("25", "27", "25") == Isosceles);
    assert(triangle("27", "25", "25") == Isosceles);
}

// Tests 6 to 14: check right angled
void testRight() {
    assert(triangle("3", "4", "5") == Right);
    assert(triangle("3", "5", "4") == Right);
    assert(triangle("5", "3", "4") == Right);
    assert(triangle("5", "12", "13") == Right);
    assert(triangle("5", "13", "12") == Right);
    assert(triangle("12", "5", "13") == Right);
    assert(triangle("12", "13", "5") == Right);
    assert(triangle("13", "5", "12") == Right);
    assert(triangle("13", "12", "5") == Right);
}

// Tests 15 to 20: check scalene
void testScalene() {
    assert(triangle("12", "14", "15") == Scalene);
    assert(triangle("12", "15", "14") == Scalene);
    assert(triangle("14", "12", "15") == Scalene);
    assert(triangle("14", "15", "12") == Scalene);
    assert(triangle("15", "12", "14") == Scalene);
    assert(triangle("15", "14", "12") == Scalene);
}

// Tests 21 to 25: check flat
void testFlat() {
    assert(triangle("7", "9", "16") == Flat);
    assert(triangle("7", "16", "9") == Flat);
    assert(triangle("9", "16", "7") == Flat);
    assert(triangle("16", "7", "9") == Flat);
    assert(triangle("16", "9", "7") == Flat);
}

// Tests 26 to 31: check impossible
void testImpossible() {
    assert(triangle("2", "3", "13") == Impossible);
    assert(triangle("2", "13", "3") == Impossible);
    assert(triangle("3", "2", "13") == Impossible);
    assert(triangle("3", "13", "2") == Impossible);
    assert(triangle("13", "2", "3") == Impossible);
    assert(triangle("13", "3", "2") == Impossible);
}

// Tests 32 to 42: check illegal
void testIllegal() {
    assert(triangle("0", "0", "0") == Illegal);
    assert(triangle("0", "10", "12") == Illegal);
    assert(triangle("10", "12", "0") == Illegal);
    assert(triangle("-1", "-1", "-1") == Illegal);
    assert(triangle("-1", "10", "12") == Illegal);
    assert(triangle("10", "-1", "12") == Illegal);
    assert(triangle("10", "12", "-1") == Illegal);
    assert(triangle("x", "y", "z") == Illegal);
    assert(triangle("3", "4y", "5") == Illegal);
    assert(triangle("10", "12", "13.4") == Illegal);
    assert(triangle("03", "4", "5") == Illegal);
}

// Tests 43 to 47: check the upper limits on lengths
void testLimits() {
    assert(triangle("2147483647","2147483647","2147483647") == Equilateral);
    assert(triangle("2147483648","2147483647","2147483647") == Illegal);
    assert(triangle("2147483647","2147483648","2147483647") == Illegal);
    assert(triangle("2147483647","2147483647","2147483648") == Illegal);
    assert(triangle("2147483649","2147483649","2147483649") == Illegal);
}

// Tests 48 to 50: check for correct handling of overflow
void testOverflow() {
    assert(triangle("1100000000","1705032704","1805032704") == Scalene);
    assert(triangle("2000000001","2000000002","2000000003") == Scalene);
    assert(triangle("150000002","666666671","683333338") == Scalene);
}

// Run tests on the triangle function.
void test() {
    checkConstants();
    testEquilateral();
    testIsosceles();
    testRight();
    testScalene();
    testFlat();
    testImpossible();
    testIllegal();
    testLimits();
    testOverflow();
    printf("All tests passed\n");
}

// Run the program or, if there are no arguments, test it.
int main(int n, char **argv) {
    if (n == 1) {
        test();
    }
    else if (n == 4) {
        int result = triangle(argv[1], argv[2], argv[3]);
        print(result);
    }
    else {
        fprintf(stderr, "Use e.g.: ./triangle 3 4 5\n");
        exit(1);
    }
    return 0;
}
