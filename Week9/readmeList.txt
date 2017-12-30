In my list program, the list struct is defined as containing a start node, an
end node, and a current node. Upon initialisation the start and end nodes will
point to each other, and the current node will be set to the start node.

Each node contains a value, and pointers to the nodes before and after it.

When moving to the start of the list, the current node is just set to the stored
start node, to move to the end of the list the same process is used.

The atStart and atEnd functions are meant to just that the current node is
equivalent to the relevant start or end node.

The forward and backward functions work by setting the current node stored in the
list struct to be equal to the node that is pointed to as being either before or
after the current node.

The insertBefore and insertAfter functions, first check that the current node isn't
either at the start of end of the list, depending on which function is running,
and then creates a new node, with pointers being changed so that it could be considered
to be "between" the current node and the one that comes after it.

The getBefore and getAfter functions just return the value stored in the node either
before or after the current node, after checking that the current node isn't
the start or end node, with the one being checked for depending on which function is called.

The deleteBefore and deleteAfter functions also check to ensure that the current
node isn't either the start or end node. After this, to delete the node either before
or after the current node, the before and after node pointers of the deleted node are given
to the node after the deleted node as a before pointer, and the current node as an after
pointer respectively. The memory storing the deleted node struct is then freed.

I have included some relatively basic test-functions, but to ensure that the Program
worked generally I have also added an interactive element, that will allow a user to
manipulate a created list using my created functions. I used this to do slightly more
thorough testing to ensure all of the functions I created will work together properly.

(When running the interactive list, the current list will always be displayed above the command
input instructions. Also, when the program starts, the current node will be the start node, so
commands that concern the before node will be rejected.)

Note: For testing purposes, the getBefore and getAfter functions return a 0 if they
are called at the beginning or end of the list. Obviously this is impractical, but has
been used to simplify testing in this case.
