#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

struct text {
    int size;
    char *array;
};

// Create a new text structure, containing a copy of the given string.
// Note that this function is impossible to test fully.
text *new(char *s) {
    text *t = malloc(sizeof(int) + sizeof(char)*strlen(s) + 1); //Allocates memory to text struct
    t->size = strlen(s) + 1; //Sets size to be the length of the provided string, +1 to account for the term char
    t->array = (char *) malloc(strlen(s) + 1); //allocates memory space to the char array in text
    strcpy(t->array, s); //Copies provided string into the text struct
    return t; //Returns text struct
}

// Free up both the space for the struct and the space for the character array.
// Be careful not to access anything after it has been freed.
// Note that this function is imnpossible to test.
void discard(text *t) {
    t->size = 0;
    free(t->array);
}

int length(text *t) {
    return t->size - 1; //Accounts for size contatining terminating character
}

char get(text *t, int i) {
    return t->array[i];
}

void set(text *t, int i, char c) {
    t->array[i] = c;
}

text *copy(text *t) {
    text *t2 = malloc(sizeof(int) + sizeof(char)*t->size);;
    t2->size = t->size;
    t2->array = (char *) malloc(t->size);
    strcpy(t2->array, t->array);
    return t2;
}

int sumText(text *t) {
    int sum = 0;
    for (int i = 0; i < t->size; i++) sum += t->array[i];
    return sum;
}

int compare(text *t1, text *t2) {
    if (strcmp(t1->array, t2->array) == 0) return 0;
    else if (sumText(t1) < sumText(t2)) return -1;
    return 1;
}

void append(text *t1, text *t2) {
    char *temp;
    t1->size += t2->size;
    temp = malloc(t1->size);
    strcpy(temp, t1->array);
    t1->array = malloc(t1->size + t2->size);
    strcpy(t1->array, temp);
    strcat(t1->array, t2->array);
}

text *slice(text *t, int i, int j) {
    int count = 0;
    text *t2 = malloc(sizeof(int) + sizeof(char)*((j - i) + 1));
    t2->size = (j - i) + 1;
    t2->array = (char *) malloc(t2->size);
    for (int x = i; x < j; x++) {
      t2->array[count] = t->array[x];
      count++;
    }
    return t2;
}

int find(text *t, text *p) {
    int i, max;
    max = (t->size - p->size); //Gets difference in sizes, so loop doesn't pass end of array
    bool found = false;
    for (i = 0; i <= max && !found; i++) {
      found = true;
      for (int j = 0; j < (p->size - 1) && found; j++) {
        if (t->array[i + j] != p->array[j]) found = false;
      }
    }
    if (found) return --i;//Acounts for loop incrementing i ex
    return -1;
}

//-----------------------------------------------------------------------------
// Tests and a textMain function (renamed to main when compiled for testing).

// Check whether a text is like a given string, as far as can be checked
// without knowing how much memory has been allocated.
bool like(text *t, char *s) {
    if (t == NULL) return false;
    if (t->size <= strlen(s)) return false;
    if (strcmp(t->array, s) != 0) return false;
    return true;
}

// Check that everything is as expected after calling new.
void testNew() {
    text *t = new("");
    assert(like(t, ""));
    text *t2 = new("cat");
    assert(like(t2, "cat"));
    discard(t);
    discard(t2);
}

void testLength() {
    text *t = new("cat");
    assert(length(t) == 3);
    discard(t);
}

void testGet() {
    text *t = new("cat");
    assert(get(t,0) == 'c');
    assert(get(t,1) == 'a');
    assert(get(t,2) == 't');
    discard(t);
}

void testSet() {
    text *t = new("cat");
    set(t, 0, 'b');
    set(t, 2, 'r');
    assert(like(t, "bar"));
    discard(t);
}

void testCopy() {
    text *t = new("cat");
    text *t2 = copy(t);
    assert(like(t2, "cat"));
    discard(t);
    discard(t2);
}

void testCompare() {
    text *t = new("cat");
    text *t2 = new("cat");
    assert(compare(t, t2) == 0);
    text *t3 = new("cas");
    assert(compare(t, t3) > 0);
    text *t4 = new("cau");
    assert(compare(t, t4) < 0);
    discard(t);
    discard(t2);
    discard(t3);
    discard(t4);
}

void testAppend() {
    text *t = new("car");
    text *t2 = new("pet");
    append(t, t2);
    assert(like(t, "carpet"));
    discard(t);
    discard(t2);
}

void testSlice() {
    text *t = new("carpet");
    text *t2 = slice(t, 0, 3);
    assert(like(t2, "car"));
    text *t3 = slice(t, 3, 6);
    assert(like(t3, "pet"));
    discard(t);
    discard(t2);
    discard(t3);
}

void testFind() {
    text *t = new("carpet");
    text *t2 = new("car");
    assert(find(t, t2) == 0);
    text *t3 = new("pet");
    assert(find(t, t3) == 3);
    text *t4 = new("cat");
    assert(find(t, t4) == -1);
    discard(t);
    discard(t2);
    discard(t3);
    discard(t4);
}

// Test the library.  (With a real library, a renaming scheme would be needed.)
int textMain() {
    testNew();
    testLength();
    testGet();
    testSet();
    testCopy();
    testCompare();
    testAppend();
    testSlice();
    testFind();
    printf("Tests all pass.");
    return 0;
}
