#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct node node;

struct node {
    node* bef;
    item val;
    node* aft;
};

struct list {
    node* start;
    node* end;
    node* current;
};

void clearStrings(char *c1, char *c2, char *c3) {
    for (int i = 0; i < strlen(c1); i++) c1[i] = ' ';
    for (int i = 0; i < strlen(c2); i++) c2[i] = ' ';
    for (int i = 0; i < strlen(c3); i++) c3[i] = ' ';
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

node *newNode(item x) {
    node *n = malloc(sizeof(node));
    n->val = x;
    n->bef = NULL;
    n->aft = NULL;
    return n;
}

// Create a new empty list.
list *newList() {
    list *l = malloc(sizeof(list));
    l->start = malloc(sizeof(node));
    l->end = malloc(sizeof(node));
    l->current = malloc(sizeof(node));
    l->start->bef = NULL;
    l->start->aft = l->end;
    l->end->bef = l->start;
    l->end->aft = NULL;
    l->current = l->start;
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
    node *c = l->current;
    if (l->current == l->end);
    else l->current = c->aft;
}

void backward(list *l) {
    node *c = l->current;
    if (l->current == l->start);
    else l->current = c->bef;
}

// Insert an item before or after the current position (i.e. at the current
// position, but with the current position ending up after or before the new
// item).
void insertBefore(list *l, item x) {
    node *c = l->current;
    node *b = c->bef;
    node *new;
    if (l->current != l->start) {
      new = newNode(x);
      new->aft = c;
      new->bef = b;
      b->aft = new;
      c->bef = new;
    } printf("at start of list\n");
}

void insertAfter(list *l, item x) {
    node *c = l->current;
    node *a = c->aft;
    node *new;
    if (l->current != l->end) {
      new = newNode(x);
      new->bef = c;
      new->aft = a;
      a->bef = new;
      c->aft = new;
    } else printf("at end of list");
}

// Get the item before or after the current position. It is an error to call
// getBefore when at the start, or getAfter when at the end.
item getBefore(list *l) {
    node *c  = l->current;
    if (l->current == l->start) return 0; //Returns empty element as we are looking before first element
    return c->bef->val;
}

item getAfter(list *l) {
    node *c  = l->current;
    if (l->current == l->end) return 0; //Returns empty element as we are looking after last element
    return c->aft->val;
}

// Set the item before or after the current position. It is an error to call
// setBefore when at the start, or setAfter when at the end.
void setBefore(list *l, item x) {
    node *b  = l->current->bef;
    if (l->current == l->start) printf("at start of list\n");
    else b->val = x;
}

void setAfter(list *l, item x) {
    node *a  = l->current->aft;
    if (l->current == l->end) printf("at end of list\n");
    else a->val = x;
}

// Delete the item before or after the current position. It is an error to call
// deleteBefore when at the start, or deleteAfter when at the end.
void deleteBefore(list *l) {
    node *c = l->current;
    node *b = c->bef;
    if (c != l->start && b != l->start) {
      c->bef = b->bef;
      b->bef->aft = c;
      free(b);
    }
}

void deleteAfter(list *l) {
    node *c = l->current;
    node *a = c->aft;
    if (c != l->end && a != l->end) {
      c->aft = a->aft;
      a->aft->bef = c;
      free(a);
    }
}

void printList(list *l) {
    node *cur = l->current;
    start(l);
    printf("\n");
    printf("start ");
    if (cur == l->start) printf("<");
    printf("\n");
    forward(l);
    while (l->current != l->end) {
      printf("%d ", l->current->val);
      if (l->current == cur) printf("<");
      printf("\n");
      forward(l);
    }
    printf("end ");
    if (cur == l->end) printf("<");
    printf("\n");
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
    printList(l);
    while (strcmp(cmd, "Exit") != 0) {
      clearStrings(cmd, arg, in); //Ensures all strings are completely cleared, to eliminate errors with command entering
      printf("\nPlease enter a command followed by an argument (-99->999), invalid commands won't be accepted.\n"
      "If no argument is required, please only enter the command.\n"
      "Please enter 'help' for a list of commands\nEnter 'Exit' to close the program\n");
      fgets(in, 21, stdin); //Two longer than needed to ensure \n is captured aswell
      splitInput(cmd, arg, in); //Issue: Program wont work if no second argument entered
      if (isDigit(arg) && strcmp(cmd, "Exit\n") != 0) call(l, cmd, atoi(arg));
      else if (strcmp(cmd, "Exit") != 0) {
        call(l, cmd, 0); //Prevents invalid digits being entered to arguments, uses default reject value 0
      }
    }
}

//-------------------------------------------------------------------------------------

void testNewList() {
    list *l1 = newList();
    assert(l1->current == l1->start);
    assert(l1->start->bef == NULL);
    assert(l1->start->aft == l1->end);
    assert(l1->end->bef == l1->start);
    assert(l1->end->aft == NULL);
}

void testStart() {
    list *l1 = newList();
    start(l1);
    assert(l1->current == l1->start);
}

void testEnd() {
   list *l1 = newList();
   end(l1);
   assert(l1->current == l1->end);
}

void testAtStart() {
    list *l1 = newList();
    end(l1);
    assert(atStart(l1) == false);
    start(l1);
    assert(atStart(l1));
}

void testAtEnd() {
    list *l1 = newList();
    start(l1);
    assert(atEnd(l1) == false);
    end(l1);
    assert(atEnd(l1));
}

void testForward() {
    list *l1 = newList();
    start(l1);
    insertAfter(l1, 5);
    assert(l1->current == l1->start); //Testing that the current location in the list is the item in position 2 of the node array
    forward(l1);
    assert(l1->current->val == 5);
    forward(l1);
    assert(l1->current == l1->end);
}

void testBackward() {
    list *l1 = newList();
    end(l1);
    insertBefore(l1, 5);
    assert(l1->current == l1->end); //Testing that the current location in the list is the item in position 2 of the node array
    backward(l1);
    assert(l1->current->val == 5);
    backward(l1);
    assert(l1->current == l1->start);
}

void testInsBef() {
    list *l1 = newList();
    end(l1);
    insertBefore(l1, 5);
    backward(l1);
    assert(l1->current->val == 5);
    assert(l1->current->aft == l1->end);
    assert(l1->current->bef == l1->start);
}

void testInsAft() {
    list *l1 = newList();
    start(l1);
    insertAfter(l1, 5);
    forward(l1);
    assert(l1->current->val == 5);
    assert(l1->current->aft == l1->end);
    assert(l1->current->bef == l1->start);
}

void testGetBef() {
    list *l1 = newList();
    start(l1);
    insertAfter(l1, 5);
    assert(getBefore(l1) == 0); //I'm treating 0 as a null value in this program in order to simplify error checking
    end(l1);
    assert(getBefore(l1) == 5);
}

void testGetAft() {
    list *l1 = newList();
    end(l1);
    insertBefore(l1, 5);
    assert(getAfter(l1) == 0); //I'm treating 0 as an error value in this program in order to simplify error checking
    start(l1);
    assert(getAfter(l1) == 5);
}

void testSetBef() {
    //It would be difficult to test if the list is unchanged when the call is rejected, but due to the fact that
    //the function just returns upon a rejected call, I feel confident that it works as intended.
    list *l1 = newList();
    end(l1);
    insertBefore(l1, 4);
    backward(l1);
    assert(l1->current->val == 4);
    forward(l1);
    setBefore(l1, 5);
    backward(l1);
    assert(l1->current->val == 5);
}

void testSetAft() {
    list *l1 = newList();
    start(l1);
    setAfter(l1, 4);
    forward(l1);
    assert(l1->current->val == 4);
    backward(l1);
    setAfter(l1, 5);
    forward(l1);
    assert(l1->current->val == 5);
}

void testDelBef() {
    list *l1 = newList();
    end(l1);
    backward(l1);
    assert(l1->current == l1->start);
    forward(l1);
    insertBefore(l1, 5);
    backward(l1);
    assert(l1->current->val == 5);
    assert(l1->current->bef == l1->start);
    assert(l1->current->aft == l1->end);
    forward(l1);
    assert(l1->current == l1->end);
    deleteBefore(l1);
    backward(l1);
    assert(l1->current == l1->start);
}

void testDelAft() {
    list *l1 = newList();
    start(l1);
    forward(l1);
    assert(l1->current == l1->end);
    backward(l1);
    insertAfter(l1, 5);
    forward(l1);
    assert(l1->current->val == 5);
    assert(l1->current->bef == l1->start);
    assert(l1->current->aft == l1->end);
    backward(l1);
    assert(l1->current == l1->start);
    deleteAfter(l1);
    forward(l1);
    assert(l1->current == l1->end);
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
