#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

//This setNfp is only used when a new value is added, to check for any unused spaces in the node array,
//the NFP when an item is deleted is handled in the deletion method
void setNfp(list *l) {
   bool setFlag = false;
   for (int i = 0; i < l->capacity && !setFlag; i++) {
     if (l->stuff[i].val == 0 && i != l->start && i != l->end) {
       l->nfp = i;
       setFlag = true;
     }
   }
   if (!setFlag) l->nfp = -1;
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
    l->capacity = 2;
    // list *l = malloc(6 * sizeof(int) + (sizeof(int) * 3) * l->capacity);
    l->nfp = 0;
    l->start = 0;
    l->end = 1;
    l->current = 0;
    l->stuff = (node*)malloc(l->capacity * (3 * sizeof(int)));
    l->length = 2;
    for (int i = 0; i < l->capacity; i++) l->stuff[i].val = 0; //Fills sentinel nodes with zeroes, will assume zero to be
                                                               //completely unused node in list
    l->stuff[0].aft = 1;
    l->stuff[1].bef = 0;
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
    if (l->current == l->end);
    else l->current = l->stuff[l->current].aft;
}

void backward(list *l) {
    if (l->current == l->start);
    else l->current = l->stuff[l->current].bef;
}

// Insert an item before or after the current position (i.e. at the current
// position, but with the current position ending up after or before the new
// item).
void insertBefore(list *l, item x) {
    node c = l->stuff[l->current];
    if (l->length >= l->capacity) expand(l);
    // if (l->nfp == -1) return;
    l->stuff[l->nfp].val = x;
    if (l->current == l->start) {
        l->start = l->nfp;
        l->stuff[l->current].bef = l->start;
        l->stuff[l->nfp].aft = l->current;
    } else {
        l->stuff[l->nfp].bef = l->stuff[l->current].bef;
        l->stuff[l->nfp].aft = l->current;
        l->stuff[c.bef].aft = l->nfp;
        l->stuff[l->current].bef = l->nfp;
    }
    l->length++;
    setNfp(l);
}

void insertAfter(list *l, item x) {
  node c = l->stuff[l->current];
  if (l->length >= l->capacity) expand(l);
  // if (l->nfp == -1) return;
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
    if (l->current == l->start);
    else l->stuff[c.bef].val = x;
}

void setAfter(list *l, item x) {
    node c  = l->stuff[l->current];
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
        if (l->nfp != l->start)  {
          l->stuff[n.bef].aft = l->current;
          //Sets the ptr to the deleted element, to point to the element after it
          l->stuff[c.bef].val = 0;
          l->stuff[l->current].bef = n.bef;
        }
        else l->start = l->current;
    }
    l->length--;
}

void deleteAfter(list *l) {
  node c  = l->stuff[l->current];
  node n = l->stuff[c.aft];
  if (l->current == l->end);
  else {
      l->nfp = l->stuff[l->current].aft;
      if (l->nfp != l->end) l->stuff[n.aft].bef = l->current;
      //Sets the ptr to the deleted element, to point to the element before it
      if (l->nfp != l->end) l->stuff[l->current].aft = n.aft;
      else l->end = l->current;
  }
  l->length--;
}

list *initList() {
  list *l = malloc(sizeof(list));
  // list *l = malloc(5 * sizeof(int) + (sizeof(int) * 3) * 3);
  l->nfp = 0;
  l->start = 0;
  l->end = 0;
  l->current = 0;
  l->length = 0;
  l->capacity = 1;
  l->stuff = (node*)malloc(1 * (3 * sizeof(int)));
  l->stuff[0].val = 0;
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

void call(list *l, char *cmd, int arg) {
    if (strcmp(cmd, "start") == 0) start(l);
    if (strcmp(cmd, "end") == 0) end(l);
    if (strcmp(cmd, "insertBefore") == 0) insertBefore(l, 6);
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
    assert(l1->stuff[1].val == l2->stuff[1].val);
    assert(l1->stuff[2].val == l2->stuff[2].val);
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

int listMain() {
    tests();
}
