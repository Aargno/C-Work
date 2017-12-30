In my list program, I have implemented the list by defining the list struct with
a starter pointer (start), an end pointer (end), and a Next Free Pointer (nfp).
The nfp was included to help me keep track of where the next free position in the
array used to store the list was. This either meant an empty location, indicated by a 0
in this implementation, or is allocated when a section of the list is "deleted".

In my implementation of the newList function, I mimicked the same technique, that
was used in the text task set in Week 7. In conjunction with this I have included a
discard function also to free up used memory.

The start, end, atStart, and atEnd functions are fairly self-explanatory.
However, as I have not utilised sentinel nodes, my start function will move to
the first node, rather than to before it. The end function does the same for its
respective node.

Due to how I have designed my list struct, when moving forward or backward in the list,
I make use of the node pointed to by the current node pointer, and either go to the
node it points to being before or after it, depending on whether I am moving forward or
backward through the list. If the current node is the start or end node, the function
will not do anything.

My insertBefore and insertAfter functions, simply put a node in the location in the node
array, pointed to by the nfp, and if the array is full, reallocating the memory it is given
to expand it. Then, the new node is given the provided value, with it's before pointer, being
set to the before pointer of the current node, it's after pointer being set to the pointer
to the current node, the after pointer of the node originally before the current pointer being
set to point to the new node, and the before pointer of the current node being set to point to
the new node. The same process is essentially used when inserting a new node after the current node.

getBefore and getAfter returns the value of the node either before or after the node currently being
pointed to.

setBefore and setAfter updates the value of the node either before or after the node currently being
pointed to with the provided value.

When my deleteBefore function, works by removing all pointers to the location in the node array,
containing the node that is to be deleted, and setting pointers to the deleted node to "point around"
it. This is done, in the case of deleteBefore by setting the before pointer of the current node, to the
before pointer of the node to be deleted, and the after pointer of the node before the one to be deleted to
the current node. The deleteAfter function works in a similar way.

I have also included functions that will allow a user to interact with my program and use the functions
described above to create and manipulate a list structure.
These functions include :-
-A call function that takes a command and argument
-A function the can print out all values in a list in order
-Functions that can print out specific values in the list
-Some functions to handle input strings
