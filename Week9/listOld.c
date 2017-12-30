#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct node {
    int bef;
    item val;
    int aft;
};

typedef struct node node;

struct list {
    int nfp;
    int start;
    int end;
    int current;
    int length;
    int capacity;
    node *stuff;
};

void clearStrings(char *c1, char *c2, char *c3) {
    for (int i = 0; i < strlen(c1); i++) c1[i] = ' ';
    for (int i = 0; i < strlen(c2); i++) c2[i] = ' ';
    for (int i = 0; i < strlen(c3); i++) c3[i] = ' ';
}

void capString(char *c) {
    for (int i = 0; i < strlen(c); i++) {
      if (c[i] == ' ' || c[i] == '\n') c[i] = '\0';
    }
}

//Specific function to split user input into two sections around space
void splitInput(char *c1, char *c2, char *input) {
    int i = 0;
    int j;
    for (i = 0; i < strlen(input) && input[i] != ' ' && input[i] != '\n'; i++) {//Tests for new line character, to account for string with no space
        if (input[i] != '\n' && input[i] != '\0') c1[i] = input[i];
    }
    if (c1[i] == ' ') c1[i] = '\0';
    i++;//Moves past the space in the input
    for (j = 0; i < strlen(input) && input[i] != ' '; i++) {
        if (input[i] != '\n' && input[i] != '\0') c2[j] = input[i];
        j++;
    }
    if (c2[j] == ' ') c2[j] = '\0';
}

bool isDigit(const char *a) {
    int x;
    x = atoi(a);

    if (x == 0 && strcmp(a, "0") != 0) {
      return false;
    }
    return true;
}

//This setNfp is only used when a new value is added, to check for any unused spaces in the node array,
//the NFP when an item is deleted is handled in the deletion method
void setNfp(list *l) {
   bool setFlag = false;
   for (int i = 0; i < l->capacity && !setFlag; i++) {
     if (l->stuff[i].val == 0) {
       l->nfp = i;
       setFlag = true;
     }
   }
   if (!setFlag) l->nfp = -1;
}

//Function to ensure that the start and end nodes have the correct ending ptrs, for checking purposes
//The values of the start and end pointers only matter for testing purposes to make sure pointers are moved properly
//Easier with known values
void setEndPtr(list *l) {
    if (l->stuff[l->start].bef != -1) l->stuff[l->start].bef = -1;
    if (l->stuff[l->end].aft != -2) l->stuff[l->end].aft = -2;
}

void expand(list *l) {
   l->capacity++;
   l->stuff = realloc(l->stuff, l->capacity * (3 * sizeof(int)));
   l->stuff[l->capacity - 1].val = 0;
   setNfp(l);
}

void discard(list *l) {
    l->nfp = 0;
    l->start = 0;
    l->end = 0;
    l->current = 0;
    l->length = 0;
    l->capacity = 0;
    free(l->stuff);
}

// Create a new empty list.
list *newList() {
    list *l = malloc(sizeof(list));
    l->capacity = 1;
    l->nfp = 0;
    l->start = 0;
    l->end = 0;
    l->current = 0;
    l->stuff = (node*)malloc(l->capacity * (3 * sizeof(int)));
    l->length = 0;
    //Fills array with zeroes, will assume zero to be empty element in list
    for (int i = 0; i < l->capacity; i++) {
      l->stuff[i].val = 0;
      l->stuff[i].bef = -1; //Gives testable values for the ptrs that signify the start and end nodes.
      l->stuff[i].aft = -2;
    }
    return l;
}

// Set the current position before the first item or after the last item, e.g.
// to begin a forward or backward traversal.
void start(list *l) {
    l->current = l->start;
}

void end(list *l) {
    l->current = l->end;
}

// Check whether the current position is at the start or end, e.g. to test
// whether a traversal has finished.
bool atStart(list *l) {
    return l->current == l->start;
}

bool atEnd(list *l) {
    return l->current == l->end;
}

// Move the current position one place forwards or backwards.  It is an error
// to call forward when at the end of the list, or backward when at the start.
void forward(list *l) {
    if (l->current == l->end || l->stuff[l->current].aft == -2);
    else l->current = l->stuff[l->current].aft;
}

void backward(list *l) {
    if (l->current == l->start || l->stuff[l->current].bef == -1);
    else l->current = l->stuff[l->current].bef;
}

// Insert an item before or after the current position (i.e. at the current
// position, but with the current position ending up after or before the new
// item).
void insertBefore(list *l, item x) {
    node c = l->stuff[l->current];
    if (x == 0) return; //Due to the way my program is desgined an entry of X will be rejected, so as to simplify
    //finding the NFP and some testing.
    if (l->nfp == -1) expand(l);
    l->stuff[l->nfp].val = x;
    if (l->current == l->start) {
        l->start = l->nfp;
        l->stuff[l->nfp].bef = l->stuff[l->current].bef;
        l->stuff[l->nfp].aft = l->current;
        l->stuff[l->current].bef = l->nfp;
    } else {
        l->stuff[l->nfp].bef = l->stuff[l->current].bef;
        l->stuff[l->nfp].aft = l->current;
        l->stuff[c.bef].aft = l->nfp;
        l->stuff[l->current].bef = l->nfp;
    }
    l->length++;
    setEndPtr(l);
    setNfp(l);
}

void insertAfter(list *l, item x) {
  node c = l->stuff[l->current];
  if (x == 0) return;
  if (l->nfp == -1) expand(l);
  l->stuff[l->nfp].val = x;
  if (l->current == l->end) {
      l->end = l->nfp;
      l->stuff[l->current].aft = l->end;
      l->stuff[l->nfp].bef = l->current;
  } else {
      l->stuff[l->nfp].aft = l->stuff[l->current].aft;
      l->stuff[l->nfp].bef = l->current;
      l->stuff[c.aft].bef = l->nfp;
      l->stuff[l->current].aft = l->nfp;
  }
  l->length++;
  setEndPtr(l);
  setNfp(l);
}

// Get the item before or after the current position. It is an error to call
// getBefore when at the start, or getAfter when at the end.
item getBefore(list *l) {
    node c  = l->stuff[l->current];
    if (l->current == l->start) return 0; //Returns empty element as we are looking before first element
    return l->stuff[c.bef].val;
}

item getAfter(list *l) {
    node c  = l->stuff[l->current];
    if (l->current == l->end) return 0; //Returns empty element as we are looking after last element
    return l->stuff[c.aft].val;
}

// Set the item before or after the current position. It is an error to call
// setBefore when at the start, or setAfter when at the end.
void setBefore(list *l, item x) {
    node c  = l->stuff[l->current];
    if (x == 0) return;
    if (l->current == l->start);
    else l->stuff[c.bef].val = x;
}

void setAfter(list *l, item x) {
    node c  = l->stuff[l->current];
    if (x == 0) return;
    if (l->current == l->end);
    else l->stuff[c.aft].val = x;
}

// Delete the item before or after the current position. It is an error to call
// deleteBefore when at the start, or deleteAfter when at the end.
void deleteBefore(list *l) {
    node c  = l->stuff[l->current];
    node n = l->stuff[c.bef];
    if (l->current == l->start);
    else {
        l->nfp = l->stuff[l->current].bef;
        l->stuff[n.bef].aft = l->current;
        //Sets the ptr to the deleted element, to point to the element after it
        l->stuff[l->current].bef = n.bef;
        if (l->nfp == l->start) l->start = l->current;
    }
    l->length--;
}

void deleteAfter(list *l) {
  node c = l->stuff[l->current];
  node n = l->stuff[c.aft];
  if (l->current == l->end);
  else {
      l->nfp = l->stuff[l->current].aft;
      l->stuff[n.aft].bef = l->current;
      //Sets the ptr to the deleted element, to point to the element before it
      l->stuff[l->current].aft = n.aft;
      if (l->nfp == l->end) l->end = l->current;
  }
  l->length--;
}

void printList(list *l) {
    bool endFlag = false;
    int cur = l->current;
    start(l);
    while (!endFlag) {
      if (l->current == l->end) endFlag = true;
      printf("\n%d", l->stuff[l->current].val);
      if (cur == l->current) printf("<");
      forward(l);
    }
    l->current = cur;
    printf("\n");
}

void call(list *l, char *cmd, int arg) {
    if (strcmp(cmd, "start") == 0) start(l);
    if (strcmp(cmd, "end") == 0) end(l);
    if (strcmp(cmd, "forward") == 0) forward(l);
    if (strcmp(cmd, "backward") == 0) backward(l);
    if (strcmp(cmd, "insertBefore") == 0) insertBefore(l, arg);
    if (strcmp(cmd, "insertAfter") == 0) insertAfter(l, arg);
    if (strcmp(cmd, "getBefore") == 0) getBefore(l);
    if (strcmp(cmd, "getAfter") == 0) getAfter(l);
    if (strcmp(cmd, "setBefore") == 0) setBefore(l, arg);
    if (strcmp(cmd, "setAfter") == 0) setAfter(l, arg);
    if (strcmp(cmd, "deleteBefore") == 0) deleteBefore(l);
    if (strcmp(cmd, "deleteAfter") == 0) deleteAfter(l);
    if (strcmp(cmd, "printList") == 0) printList(l);
    if (strcmp(cmd, "printBefore") == 0) printf("%d\n", getBefore(l));
    if (strcmp(cmd, "printAfter") == 0) printf("%d\n", getAfter(l));
    if (strcmp(cmd, "help") == 0) {
        printf("Available Commands Are:\n"
                "start - Moves to start of list (first node)\n"
                "end - Moves to end of list (last node)\n"
                "insertBefore x - Inserts integer x before the current value in the list, 0 is rejected\n"
                "insertAfter x - Inserts integer x after the current value in the list, 0 is rejected\n"
                "getBefore - Retrieves the value before the current postion in the list\n"
                "getAfter - Retrieves the value after the current postion in the list\n"
                "setBefore x - Sets the value before the current position in the list to integer x, 0 is rejected\n"
                "setAfter x - Sets the value after the current position in the list to integer x, 0 is rejected\n"
                "deleteBefore - Deletes the node in the list before the current node, sets deleted node as available space\n"
                "deleteAfter - Deletes the node in the list after the current node, sets deleted node as available space\n"
                "printList - Prints every value stored in the list\n"
                "printBefore - Prints the value before the current loccation in the list\n"
                "printAfter - Prints the value after the current location in the list\n");
    }
    printList(l);
}

void listControl() {
    list *l = newList();
    char *in = malloc(19);
    char *cmd = malloc(15);
    char *arg = malloc(3);
    while (strcmp(cmd, "Exit") != 0) {
      clearStrings(cmd, arg, in); //Ensures all strings are completely cleared, to eliminate errors with command entering
      printf("\nPlease enter a command followed by an argument (0 will be rejected), invalid commands won't be accepted.\n"
      "If you don't need to enter an argument, please juset enter any 3 character string\n"
      "Please enter 'help' for a list of commands\nEnter 'Exit' to close the program\n");
      fgets(in, 21, stdin); //Two longer than needed to ensure \n is captured aswell
      splitInput(cmd, arg, in); //Issue: Program wont work if no second argument entered
      if (isDigit(arg) && strcmp(cmd, "Exit\n") != 0) call(l, cmd, atoi(arg));
      else if (strcmp(cmd, "Exit") != 0) {
        call(l, cmd, 0); //Prevents invalid digits being entered to arguments, uses default reject value 0
      }
    }
    discard(l);
}

//-------------------------------------------------------------------------------------

list *initList() {
  list *l = malloc(sizeof(list));
  l->nfp = 0;
  l->start = 0;
  l->end = 0;
  l->current = 0;
  l->length = 0;
  l->capacity = 1;
  l->stuff = (node*)malloc(1 * (3 * sizeof(int)));
  l->stuff[0].val = 0;
  l->stuff[0].bef = -1;
  l->stuff[0].aft = -2;
  return l;
}

list *initTestList(int len) {
  list *l = malloc(sizeof(list));
  l->nfp = 0;
  l->start = 0;
  l->end = 1;
  l->current = 0;
  l->length = len;
  l->capacity = len;
  l->stuff = (node*)malloc(len * (3 * sizeof(int)));
  l->stuff[0].val = 1;
  l->stuff[1].val = 3;
  l->stuff[2].val = 2;
  l->stuff[0].aft = 2;
  l->stuff[2].aft = 1;
  l->stuff[1].bef = 2;
  l->stuff[2].bef = 0;
  if (len > 3) {
    for (int i = 3; i < len; i++) l->stuff[i].val = 0; //Ensures that all values in extended test lists, are set to 0
    //and not whatver was stored in the provided memory, aids with testing.
  }
  return l;
}

void testInsertBefore(list *l) {
    l->stuff[3].val = 4;
    l->stuff[3].bef = 2;
    l->stuff[3].aft = 1;
    l->stuff[1].bef = 3;
    l->stuff[2].aft = 3;
}

void testInsertAfter(list *l) {
    l->stuff[3].val = 4;
    l->stuff[3].bef = 0;
    l->stuff[3].aft = 2;
    l->stuff[0].aft = 3;
    l->stuff[2].bef = 3;
}

void testNewList() {
    list *l1 = newList();
    list *l2 = initList();
    assert(l1->nfp == l2->nfp);
    assert(l1->start == l2->start);
    assert(l1->end == l2->end);
    assert(l1->current == l2->current);
    assert(l1->length == l2->length);
    assert(l1->stuff[0].val == l2->stuff[0].val);
    discard(l1);
    discard(l2);
}

void testStart() {
    list *l1 = initTestList(3);
    start(l1);
    assert(l1->current == l1->start);
    l1 = newList();
    start(l1);
    assert(l1->current == l1->start && l1->start == l1->end);
    discard(l1);
}

void testEnd() {
   list *l1 = initTestList(3);
   end(l1);
   assert(l1->current == l1->end);
   l1 = newList();
   end(l1);
   assert(l1->current == l1->start && l1->start == l1->end);
   discard(l1);
}

void testAtStart() {
    list *l1 = initTestList(3);
    end(l1);
    assert(atStart(l1) == false);
    start(l1);
    assert(atStart(l1));
    discard(l1);
}

void testAtEnd() {
    list *l1 = initTestList(3);
    start(l1);
    assert(atEnd(l1) == false);
    end(l1);
    assert(atEnd(l1));
    discard(l1);
}

void testForward() {
    list *l1 = initTestList(3);
    start(l1);
    forward(l1);
    assert(l1->current == 2); //Testing that the current location in the list is the item in position 2 of the node array
    forward(l1);
    assert(l1->current == 1);
    forward(l1);
    assert(l1->current == 1);
    discard(l1);
}

void testBackward() {
    list *l1 = initTestList(3);
    end(l1);
    backward(l1);
    assert(l1->current == 2); //Testing that the current location in the list is the item in position 2 of the node array
    backward(l1);
    assert(l1->current == 0);
    backward(l1);
    assert(l1->current == 0);
    discard(l1);
}

void testInsBef() {
    list *l1 = initTestList(4);
    list *l2 = initTestList(4);
    setNfp(l1); //Ensures the correct NFP is ready
    end(l1);
    insertBefore(l1, 4);
    testInsertBefore(l2);
    // assert(testListEq(l1, l2, false));
    assert(l1->stuff[3].bef == l2->stuff[3].bef);
    assert(l1->stuff[3].val == l2->stuff[3].val);
    assert(l1->stuff[3].aft == l2->stuff[3].aft);
    assert(l1->stuff[1].bef == l2->stuff[1].bef);
    assert(l1->stuff[2].aft == l2->stuff[2].aft);
    l1 = newList();
    start(l1);
    insertBefore(l1, 1);
    assert(l1->length == 1);
    assert(l1->nfp == -1);
    assert(l1->capacity == 1);
    assert(l1->stuff[0].val == 1);
    assert(l1->stuff[0].bef == -1);
    assert(l1->stuff[0].aft == -2);
    assert(l1->start == 0);
    start(l1);
    assert(l1->stuff[l1->current].val == 1);
    insertBefore(l1, 2);
    assert(l1->stuff[0].val == 1);
    assert(l1->stuff[0].bef == 1);
    assert(l1->stuff[0].aft == -2);
    assert(l1->stuff[1].val == 2);
    assert(l1->stuff[1].aft == 0);
    assert(l1->stuff[1].bef == -1);
    assert(l1->start == 1);
    discard(l1);
    discard(l2);
}

void testInsAft() {
    list *l1 = initTestList(4);
    list *l2 = initTestList(4);
    setNfp(l1); //Ensures the correct NFP is ready
    start(l1);
    insertAfter(l1, 4);
    testInsertAfter(l2);
    assert(l1->stuff[3].bef == l2->stuff[3].bef);
    assert(l1->stuff[3].val == l2->stuff[3].val);
    assert(l1->stuff[3].aft == l2->stuff[3].aft);
    assert(l1->stuff[0].aft == l2->stuff[0].aft);
    assert(l1->stuff[2].bef == l2->stuff[2].bef);
    l1 = newList();
    start(l1);
    insertAfter(l1, 1);
    assert(l1->length == 1);
    assert(l1->nfp == -1);
    assert(l1->capacity == 1);
    assert(l1->stuff[0].val == 1);
    assert(l1->stuff[0].bef == -1);
    assert(l1->stuff[0].aft == -2);
    assert(l1->end == 0);
    start(l1);
    assert(l1->stuff[l1->current].val == 1);
    insertAfter(l1, 2);
    assert(l1->stuff[0].val == 1);
    assert(l1->stuff[0].aft == 1);
    assert(l1->stuff[0].bef == -1);
    assert(l1->stuff[1].val == 2);
    assert(l1->stuff[1].bef == 0);
    assert(l1->stuff[1].aft == -2);
    assert(l1->end == 1);
    discard(l1);
    discard(l2);
}

void testGetBef() {
    list *l1 = initTestList(3);
    start(l1);
    assert(getBefore(l1) == 0); //I'm treating 0 as a null value in this program in order to simplify error checking
    forward(l1);
    assert(getBefore(l1) == 1);
    discard(l1);
}

void testGetAft() {
    list *l1 = initTestList(3);
    end(l1);
    assert(getAfter(l1) == 0); //I'm treating 0 as a null value in this program in order to simplify error checking
    backward(l1);
    assert(getAfter(l1) == 3);
    discard(l1);
}

void testSetBef() {
    //It would be difficult to test if the list is unchanged when the call is rejected, but due to the simplicity of the
    //setBefore function, I fell confident that it works as intended.
    list *l1 = initTestList(3);
    end(l1);
    setBefore(l1, 5);
    assert(l1->stuff[2].val == 5);
    backward(l1);
    setBefore(l1, 5);
    assert(l1->stuff[0].val == 5);
    discard(l1);
}

void testSetAft() {
    list *l1 = initTestList(3);
    start(l1);
    setAfter(l1, 5);
    assert(l1->stuff[2].val == 5);
    forward(l1);
    setAfter(l1, 5);
    assert(l1->stuff[1].val == 5);
    discard(l1);
}

void testDelBef() {
    list *l1 = initTestList(3);
    end(l1);
    deleteBefore(l1);
    assert(l1->stuff[1].bef == 0);
    assert(l1->stuff[0].aft == 1);
    assert(l1->nfp == 2);
    l1 = initTestList(3);
    start(l1);
    forward(l1);
    deleteBefore(l1);
    assert(l1->start == 2);
    l1 = newList();
    start(l1);
    insertBefore(l1, 5);
    insertAfter(l1, 6);
    end(l1);
    assert(l1->stuff[1].bef == 0);
    assert(l1->stuff[1].aft == -2);
    assert(l1->end == 1);
    assert(l1->start == 0);
    assert(l1->length == 2);
    deleteBefore(l1);
    assert(l1->stuff[1].bef == -1);
    assert(l1->nfp == 0);
    assert(l1->length == 1);
    assert(l1->start == l1->end);
    discard(l1);
}

void testDelAft() {
    list *l1 = initTestList(3);
    start(l1);
    deleteAfter(l1);
    assert(l1->stuff[1].bef == 0);
    assert(l1->stuff[0].aft == 1);
    assert(l1->nfp == 2);
    l1 = initTestList(3);
    end(l1);
    backward(l1);
    deleteAfter(l1);
    assert(l1->end == 2);
    l1 = newList();
    start(l1);
    insertBefore(l1, 5);
    insertAfter(l1, 6);
    start(l1);
    assert(l1->stuff[0].bef == -1);
    assert(l1->stuff[0].aft == 1);
    assert(l1->end == 1);
    assert(l1->start == 0);
    assert(l1->length == 2);
    deleteAfter(l1);
    assert(l1->stuff[1].aft == -2);
    assert(l1->nfp == 1);
    assert(l1->length == 1);
    assert(l1->start == l1->end);
    discard(l1);
}

void tests() {
    testNewList();
    testStart();
    testEnd();
    testAtStart();
    testAtEnd();
    testForward();
    testBackward();
    testInsBef();
    testInsAft();
    testGetBef();
    testGetAft();
    testSetBef();
    testSetAft();
    testDelBef();
    testDelAft();
    printf("All tests passed\n");
}

int listMain(int argv, char **argc) {
    if (argv == 1) tests();
    else if (argv == 2 && strcmp(argc[1], "input") == 0) listControl();
    else printf("Please either enter no arguments, or enter 'input' to interact with the program\n");
}
