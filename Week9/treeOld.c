#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct node {
    int root;
    item val;
    int left;
    int right;
    bool visited;
}

struct tree {
    int length;
    int capacity;
    int start;
    int current;
    int nfp;
    node *stuff;
}

void expand(tree *t) {
    t->capacity++;
    t->stuff = realloc(l->stuff, l->capacity * (sizeof(int) * 4) + sizeof(bool));
    t->nfp = t->capacity - 1;
}

void setNfp() {
    bool setFlag = false;;
    for (int i = 0; i < t->capacity && !setFlag; i++) {
      if (t->stuff[i].val == 0) {
        t->nfp = i;
        setFlag = true;
      }
    }
    if (!setFlag) t->nfp = -1;
}

void resetTree(tree *t) {
    for (int i = 0; i < t->capacity; i++) {
      t->stuff[i].visited = false;
    }
}

//Creates a new binary tree, with value to initialise tree
tree *newTree(item x) {
    tree *t = malloc(sizeof(tree));
    t->length = 1;
    t->capacity = 1;
    t->start = 0;
    t->current = 0;
    t->nfp = -1;
    t->stuff = malloc(sizeof(int) * 4 + sizeof(bool));
}

//Adds a new item to the tree
void add(tree *t, item x) {
    node c = t->stuff[t->current];
    if(t->length >= t->capacity) expand(t);
    if (x < c.val && c.left == -1) {
      t->stuff[t->current].left = t->nfp;
      t->stuff[t->nfp].val = x;
      t->stuff[t->nfp].root = t->current;
      t->stuff[t->nfp].left = -1;
      t->stuff[t->nfp].right = -1;
      t->current = t->root;
      return;
    }
    if (x >= c.val && c.right == -1) {
      t->stuff[t->current].right = t->nfp;
      t->stuff[t->nfp].val = x;
      t->stuff[t->nfp].root = t->current;
      t->stuff[t->nfp].left = -1;
      t->stuff[t->nfp].right = -1;
      t->current = t->start;
      return;
    }
    if (x < c.val && c.left != -1) {
      t->current = c.left;
      add(t, x);
      return;
    }
    if (x >= c.val && c.right != -1) {
      t->current = c.right;
      add(t, x);
      return;
    }
}

//Performs preorder traversal of tree, printing each value when reached
void traversePrePrint(tree *t) {

}

//Performs inorder traversal of tree, printing each value when reached
void traverseInPrint(tree *t) {

}

//Performs postorder traversal of tree, printing each value when reached
void traversePostPrint(tree *t) {
    node c t->stuff[t->current];
    if (c.left == -1) {
      printf("%d\n", c.val);
      t->stuff[t->current].visited = true;
      return;
    } else if (t->stuff[c.left].visited == false) {
      t->current = c.left;
      traversePostPrint(tree *t);
    } else if ()
}

// //Performs postorder traversal of tree, printing each value when reached
// void traversePostPrint(tree *t) {
//     node c t->stuff[t->current];
//     if (c.left == -1) {
//       printf("%d\n", c.val);
//       t->stuff[t->current].visited = true;
//       return;
//     } else if (t->stuff[c.left].visited == false) {
//       t->current = c.left;
//       traversePostPrint(tree *t);
//     } else if ()
// }

//Searches tree for given value
void search(tree *t, item x) {

}

//Deletes given item from tree
void deleteItem(tree *t, item x) {

}

int treeMain() {

}
