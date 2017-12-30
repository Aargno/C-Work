#include <stdbool.h>

typedef int item;

struct node;
typedef struct node node;

//Creates a new binary tree, with value to initialise tree
node *newNode(item x);

//Adds a new item to the tree
void add(node *t, item x);

void addNode(node *t, node* a);

//Performs preorder traversal of tree, printing each value when reached
void traversePrePrint(node *t);

//Performs inorder traversal of tree, printing each value when reached
void traverseInPrint(node *t);

//Performs postorder traversal of tree, printing each value when reached
void traversePostPrint(node *t);

//Searches tree for given value
void search(node *t, item x);

//Deletes first instance of given item from tree
void deleteItem(node *t, item x);
