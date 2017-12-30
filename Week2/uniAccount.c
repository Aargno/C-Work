/*
This program will take a standard UoB Username and Password as inputs, and do
some basic checks on their validity.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

enum {UserFail,PassFail, BothPass, BothFail};

//Checks if the passed character is a digit
bool digit(const char c) {
    if (c < '0' || c > '9') {
      return false;
    }
    return true;
}

//Checks for the presence of a capital letter
bool capLetter(const char c) {
    if (c < 'A' || c > 'Z') {
      return false;
    }
    return true;
}

//Checks for the presence of a lower case letter
bool lowCaseLetter(const char c) {
    if (c < 'a' || c > 'z') {
      return false;
    }
    return true;
}

//Checks for the presence of special characters (In this case not digit or standard letter)
bool specialChar(const char c) {
    if (!digit(c) && !capLetter(c) && !lowCaseLetter(c)) {
      return true;
    }
    return false;
}

//Checks that all the character types required for a valid password are present
bool typeReq(int n, const char pass[]) {
    int passes = 0;
    bool flag = false;

    for (int i = 0; i < n && !flag; i++) {
      if (digit(pass[i])) {
        flag = true;
        passes++;
      }
    }
    if (flag == true) {
      flag = false;
    }

    for (int i = 0; i < n && !flag; i++) {
      if (capLetter(pass[i])) {
        flag = true;
        passes++;
      }
    }
    if (flag == true) {
      flag = false;
    }

    for (int i = 0; i < n && !flag; i++) {
      if (lowCaseLetter(pass[i])) {
        flag = true;
        passes++;
      }
    }
    if (flag == true) {
      flag = false;
    }

    for (int i = 0; i < n && !flag; i++) {
      if (specialChar(pass[i])) {
        flag = true;
        passes++;
      }
    }
    if (flag == true) {
      flag = false;
    }

    if (passes >= 3) {
      return true;
    }

    return false;
}

//Checks to see if there is a matching 3 character pattern betwen the password and username
bool patternCheck(int un, const char user[], int pn, const char pass[]) {
    for (int i = 0; i < pn - 2; i++) {
      for (int j = 0; j < un - 2; j++) {
        if (pass[i] == user[j] && pass[i + 1] == user[j + 1] &&  pass[i + 2] == user[j + 2]) {
          return false;
        }
      }
    }
    return true;
}

//Checks the validity of the provided username
bool userCheck(const char user[]) {
    bool errFlag = false;

    if (strlen(user) != 7) {
      errFlag = true;
    }

    for (int i = 0; i < 2; i++) {
      if (!lowCaseLetter(user[i])) {
        errFlag = true;
      }
    }

    for (int i = 2; i < 7; i++) {
      if (!digit(user[i])) {
        errFlag = true;
      }
    }

    if (errFlag) {
      return false;
    }

    return true;

}

//Checks validity of provided password
int passCheck(const char user[], const char pass[]) {
    int length = strlen(pass);
    bool errFlag = false;

    if (length < 10 || length > 100) {
      errFlag = true;
    }

    if (!typeReq(length, pass)) {
      errFlag = true;
    }

    if (!patternCheck(strlen(user), user, length, pass)) {
      errFlag = true;
    }

    if (errFlag) {
      return false;
    }

    return true;

}

//Runs checks to see which if either of the username and password are valid
int validate(const char user[], const char pass[]) {
    if (!userCheck(user) && !passCheck(user, pass)) {
      printf("Both username and password are invalid!\n");
      return BothFail;
    }

    if (userCheck(user) && !passCheck(user, pass)) {
      printf("Username is valid, password is invalid!\n");
      return PassFail;
    }

    if (!userCheck(user) && passCheck(user, pass)) {
      printf("Username is invalid, password is valid!\n");
      return UserFail;
    }

    if (userCheck(user) && passCheck(user, pass)) {
      printf("Both username and password are valid!\n");
      return BothPass;
    }
    return -1;
}

//Tests

//Tests to ensure the username is properly validated
void testUsers() {
    //Valid username
    assert(validate("ar17092", "Pa1bNdEkD!kd") == BothPass);
    //Username has too many leading letters
    assert(validate("ara7092", "Pa1bNdEkD!kd") == UserFail);
    //Username has too many digits
    assert(validate("a217092", "Pa1bNdEkD!kd") == UserFail);
    //Username too long
    assert(validate("ar170924", "Pa1bNdEkD!kd") == UserFail);
    //Username too short
    assert(validate("ar1709", "Pa1bNdEkD!kd") == UserFail);
}

//Tests to ensure passwords are properly validated
void testPass() {
    //Tests to ensure password that is too short isn't valdiated
    assert(validate("ar17092", "Pa1!") == PassFail);
    //Tests to ensure that password longer than 100 characters is rejected
    {
      //In a block so 101 long character array is removed from memory when not
      //in use
      char passTest[101] = "Ag!2";
      assert(validate("ar17092", passTest) == PassFail);
    }
    //Tests to ensure that password not containing three of the required
    //character types is rejected (contains low case and num)
    assert(validate("ar17092", "pa1bndekdkd") == PassFail);
    //Tests to ensure that password not containing three of the required
    //character types is rejected (contains caps and special char)
    assert(validate("ar17092", "PACBNDEKD!KD") == PassFail);
    //Tests to ensure that password not containing three of the required
    //character types is rejected (contains caps and num)
    assert(validate("ar17092", "35125617D908") == PassFail);
    //Tests to ensure that password not containing three of the required
    //character types is rejected (contains low case and special char)
    assert(validate("ar17092", "psacnosacn!#") == PassFail);
    //Tests to ensure that password containing all 3 character pattern found
    //in username is rejected
    assert(validate("ar17092", "Pa1bar1kD!kd") == PassFail);
    //Tests to ensure valid password containing three types of character is
    //accepted
    assert(validate("ar17092", "Pa1bNdEkD2kd") == BothPass);
    //Tests to ensure that password containing all character types is accepted
    assert(validate("ar17092", "Pa1bar3kD!kd") == BothPass);

}

//Runs all the tests provided for the program
void tests() {
    testUsers();
    testPass();
    printf("All tests pass!\n");
}

int main(int argc, char *argv[]) {

    if (argc == 1) {
      tests();
      return 0;
    }

    if (argc != 3) {
      printf("Please provide both a username and password sepearated by a space when opening the program.");
    }

    if (argc == 3) {
      if (validate(argv[1], argv[2]) == -1) {
        printf("Something went wrong.");
      }
    }

    return 0;

}
