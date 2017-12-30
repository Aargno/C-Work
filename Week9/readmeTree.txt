For my extra work, I decided to try and use the node struct from my list
program to create a series of functions that allow me to store data in a binary tree.
I have included most of the functionality found in my list structure. However, unlike
my list structure, I have not added an interactive element as it would be quite difficult
to represent the structure of a binary tree through text.

I have also included functions to traverse a given binary tree in 3 different ways
(pre-order, in-order, and post-order) and print the results, both to ensure these functions work
correctly, and that the tree is correctly storing provided data.

My delete functions, in their current implementation will currently only delete the
first instance of the item it has been told to delete, and due tot he way my tree is set-up
if the value is duplicated, anywhere in the tree, it will probably be replaced with the same value.
However, this is how the delete function is intended to work, as when an item is to be deleted,
it is replaced with it's Lowest Common Ancestor (LCA), which is then in turn removed from the tree,
with its right node being set as the left node of the preceding node.

The functions to test the traversal method are intended to be used to visually confirm
that the traversal functions are working properly.
