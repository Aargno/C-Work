/* Classify a triangle according to the integer lengths of its sides. Gives some
extra details on the triangle provided */

#include "triangleDisp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

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

//Converts degrees to radians
double degToRad(double deg) {
    return deg * (M_PI / 180);
}

//Converts radians to degrees
double radToDeg(double rad) {
    return rad * (180 / M_PI);
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

//Finds are of triangle provided to it
double area(const double s1, const double s2, const double s3) {
    double s;
    double midResult;

    s = (s1 + s2 + s3) / 2;
    midResult = s * ((s - s1) * (s - s2) * (s - s3));
    return sqrt(midResult);

}

long double relatedRad(const double s1, const double s2, const double s3) {
    long double x;
    long double ans;

    x = (squareLong(s2) + squareLong(s3) - squareLong(s1)) / (2 * (s2 * s3));
    ans = acosl(x);

    return ans;
}

//Treating original formula as s1^2 = s2^2 + s3^2 - (2 * s2 * s3) Cos(S1 Angle)
long double relatedAngle(const double s1, const double s2, const double s3) {
    long double x;
    long double ans;

    x = (squareLong(s2) + squareLong(s3) - squareLong(s1)) / (2 * (s2 * s3));
    ans = acosl(x);

    return radToDeg(ans);
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

//Only draws triangles with a maximum side length of 200
void drawTriangle(int s1, int s2, int s3,long double s1Rad, long double s2Rad, long double s3Rad) {
  if (s1 <= 200 && s2 <= 200 && s3 <= 200) {
    if (s1 <= 31 && s2 <= 31 && s3 <= 31) {
      saveTriangleSketch(s1, s2, s3, s1Rad, s2Rad, s3Rad);
    }
    displayTriangle(s1, s2, s3, s1Rad, s2Rad, s3Rad);
  }
}

//Prints out some extra details about the triangle provided
void extraDetail(const char sa[], const char sb[], const char sc[]) {
    int s1, s2, s3;
    s1 = atoi(sa);
    s2 = atoi(sb);
    s3 = atoi(sc);
    long double s1Ang = relatedAngle(s1, s2, s3);
    long double s2Ang = relatedAngle(s2, s1, s3);
    long double s3Ang = relatedAngle(s3, s1, s2);
    printf("The area of the entered triangle is: %lf\n", area(s1,s2,s3));

    printf("The angle opposite to side length: %s is %Lf\n", sa, s1Ang);
    printf("The angle opposite to side length: %s is %Lf\n", sb, s2Ang);
    printf("The angle opposite to side length: %s is %Lf\n", sc, s3Ang);
    drawTriangle(s1, s2, s3, relatedRad(s1, s2, s3), relatedRad(s2, s1, s3), relatedRad(s3, s1, s2));
}

//Prints out some extra details about the triangle provided, specifically used
//when long values needed for triangle sides.
void extraDetailLong(const char sa[], const char sb[], const char sc[]) {
    long s1, s2, s3;

    s1 = atol(sa);
    s2 = atol(sb);
    s3 = atol(sc);

    printf("The area of the entered triangle is: %lf\n", area(s1,s2,s3));

    printf("The angle opposite to side length: %s is %Lf\n", sa, relatedAngle(s1, s2, s3));
    printf("The angle opposite to side length: %s is %Lf\n", sb, relatedAngle(s2, s1, s3));
    printf("The angle opposite to side length: %s is %Lf\n", sc, relatedAngle(s3, s1, s2));
}

// Classify a triangle, given side lengths as strings:
int triangle(const char sa[], const char sb[], const char sc[]) {
    int type;

    if (largeNumCheck(sa) || largeNumCheck(sb) || largeNumCheck(sc)) {
      return Illegal;
    }

    if (overflowPotential(sa,sb,sc)) {
      type = longTriCheck(sa,sb,sc);
      if (type != Impossible && type != Illegal) {
        extraDetailLong(sa,sb,sc);
      }
      return type;
    } else {
      type = intTriCheck(sa,sb,sc);
      if(type != Impossible && type != Illegal) {
        extraDetail(sa,sb,sc);
      }
      return type;
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

//Tests area function works correctly, to avoid issues with slight inaccuracies
//due to rounding, I am making sure that the answer provided by area is approximately
//correct
void testArea() {
    assert((fabsl(area(8, 8, 8) - 27.7128) < 0.0001) == true);
    assert((fabsl(area(8, 9, 10) - 34.197) < 0.001) == true);
    assert((fabsl(area(8, 8, 10) - 31.225) < 0.001) == true);
    assert(area(8,9,17) == 0);
}

//Tests that the function that tetrieves the angle opposite to the first side
//provided is working correctly
void testAngle() {
    assert(relatedAngle(8,8,8) == 60);
    assert((fabsl(relatedAngle(25,25,27) - 57.316) < 0.001) == true);
    assert(relatedAngle(5,4,3) == 90);
    assert((fabsl(relatedAngle(12,14,15) - 48.736) < 0.001) == true);
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
        dispTest();
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
