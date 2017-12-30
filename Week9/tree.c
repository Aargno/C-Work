#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct node {
    item val;
    node* left;
    node* right;
};

void discard(node *n) {
    n->val = 0;
    free(n->left);
    free(n->right);
    free(n);
}

//Retrieves the LCA node
int retrieveLCA(node *n) {
    if (n->left == NULL) {
      return n->val;
    }
    return retrieveLCA(n->left);
}

//Deletes LCA node
void deleteLCA(node *n) {
    node *l = n->left;
    if (l->left == NULL) {
      n->left = l->right;
    }
    discard(l);
}

//Creates node, used to populate binary tree, due to nature of pointers, node also acts as binary tree
node *newNode(item x) {
    node *n = malloc(sizeof(node));
    n->val = x;
    n->left = NULL;
    n->right = NULL;
    return n;
}

//Adds a new item to the tree
void add(node *t, item x) {
    if (x < t->val && t->left == NULL) t->left = newNode(x);
    else if (x < t->val) add(t->left, x);
    if (x >= t->val && t-> right == NULL) t->right = newNode(x);
    else if (x >= t->val) add(t->right, x);
}

//Adds an existing node to the tree, primarily used in deletion
void addNode(node *t, node* a) {
    if (a->val < t->val && t->left == NULL) t->left = a;
    else if (a->val < t->val) addNode(t->left, a);
    if (a->val >= t->val && t-> right == NULL) t->right = a;
    else if (a->val >= t->val) addNode(t->right, a);
}

//Performs preorder traversal of tree, printing each value when reached
void traversePrePrint(node *t) {
    if (t == NULL) return;
    printf("%d ", t->val);
    traversePrePrint(t->left);
    traversePrePrint(t->right);
}

//Performs inorder traversal of tree, printing each value when reached
void traverseInPrint(node *t) {
    if (t == NULL) return;
    traverseInPrint(t->left);
    printf("%d ", t->val);
    traverseInPrint(t->right);
}

//Performs postorder traversal of tree, printing each value when reached
void traversePostPrint(node *t) {
    if (t == NULL) return;
    traversePostPrint(t->left);
    traversePostPrint(t->right);
    printf("%d ", t->val);
}

//Searches tree for given value
void search(node *t, item x) {
    if (t->val == x) {
      printf("Value %d found\n", x);
      return;
    } else if (t == NULL) {
      printf("Value %d not found\n", x);
      return;
    } else if (x < t->val) {
      search(t->left, x);
    } else if(x >= t->val) {
      search(t->right, x);
    }
}

//Deletes given item from tree
void deleteItem(node *t, item x) {
    int lca;
    if (t == NULL) {
      printf("Value not found\n");
      return;
    }
    if (t->val == x) {
      lca = retrieveLCA(t->right);
      deleteLCA(t->right);
      t->val = lca;
    } else if (x >= t->val && t->right != NULL) deleteItem(t->right, x);
    else if (x < t->val && t->right != NULL) deleteItem(t->left, x);
  }

//--------------------------------------------------------------------------------

void testNewNode() {
  node *n = newNode(0);
  assert(n->val == 0);
  assert(n->left == NULL);
  assert(n->right == NULL);
  discard(n);
}

void testAdd() {
  node *n = newNode(0);
  add(n, -1);
  add(n, 1);
  assert(n->val == 0);
  assert(n->left->val == -1);
  assert(n->right->val == 1);
  assert(n->left->left == NULL);
  assert(n->left->right == NULL);
  assert(n->right->left == NULL);
  assert(n->right->right == NULL);
  discard(n);
}

void testAddNode() {
  node *n = newNode(0);
  node *a = newNode(2);
  node *b = newNode(1);
  node *c = newNode(3);
  addNode(n, a);
  addNode(n, b);
  addNode(n, c);
  assert(n->right->val == 2);
  assert(n->right->left->val == 1);
  assert(n->right->right->val == 3);
  discard(n);
}

//In the print test functions, I'm visually confirming that the functions, that I have
//written to traverse my tree do so correctly.
void testPreOrderPrint() {
  node *n = newNode(5);
  node *a = newNode(2);
  node *b = newNode(1);
  node *c = newNode(7);
  node *d = newNode(8);
  addNode(n, a);
  addNode(n, b);
  addNode(n, c);
  addNode(n, d);
  traversePrePrint(n);
  printf("\n");
}

void testInOrderPrint() {
  node *n = newNode(5);
  node *a = newNode(2);
  node *b = newNode(1);
  node *c = newNode(7);
  node *d = newNode(8);
  addNode(n, a);
  addNode(n, b);
  addNode(n, c);
  addNode(n, d);
  traverseInPrint(n);
  printf("\n");
}

void testPostOrderPrint() {
  node *n = newNode(5);
  node *a = newNode(2);
  node *b = newNode(1);
  node *c = newNode(7);
  node *d = newNode(8);
  addNode(n, a);
  addNode(n, b);
  addNode(n, c);
  addNode(n, d);
  traversePostPrint(n);
  printf("\n");
}

void testDelete() {
  node *n = newNode(0);
  node *a = newNode(2);
  node *b = newNode(1);
  node *c = newNode(4);
  node *d = newNode(3);
  addNode(n, a);
  addNode(n, b);
  addNode(n, c);
  addNode(n, d);
  deleteItem(n, 2);
  assert(n->right->val == 3);
  assert(n->right->left->val == 1);
  assert(n->right->right->left == NULL);
  n = newNode(0);
  a = newNode(2);
  b = newNode(1);
  c = newNode(4);
  d = newNode(3);
  addNode(n, a);
  addNode(n, b);
  addNode(n, c);
  addNode(n, d);
  deleteItem(n, 0);
  assert(n->val == 1);
  assert(n->right->val == 2);
  assert(n->right->left == NULL);
  discard(n);
}

void test() {
  testNewNode();
  testAdd();
  testAddNode();
  testPreOrderPrint();
  testInOrderPrint();
  testPostOrderPrint();
  testDelete();
  printf("All tests pass\n");
}

int treeMain() {
    test();
}
