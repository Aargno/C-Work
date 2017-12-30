In the case that only one program is meant to be submitted for this section,
could you please mark the program in drawSave.c

I've also submitted the display.h and display.c files as these are necessary
for my programs.

The main files with new code are triangleDisp.c, drawSave.c, and a small section
at the top of oxo.c. The majority of oxo.c and triangleExtraDetails.c function
in the exact same way as they did when they were initially submitted, they are
being submitted again as my extra work builds off of these programs.

I've also included my makefile to simplify compiling the programs.

For my extra work this week, I added a basic graphical interface to both the oxo
and triangle programs, and also created a program which can be used to draw an
image using commands, and allows the user to save the drawn image to file.

Oxo - The only new functions added to oxo are located at the top of the file.
The added functions primarily define how shapes should be drawn, with one of the
functions managing the actual drawing of the grid. All features of the original
oxo program are available.

Triangle - The new functionality for this program has been defined in the
triangleDisp.c file. The added functionality will take any triangle that will
fit in a 200 x 200 display, and draw it. Due to how the numbers in my program
end up being rounded, some of the triangles displayed will sometimes be slightly
out of proportion. (Given height/"centre of triangle" position may be off by 1,
also due to this, I can't really think of how to right an automatic test for the
findHorizPoint and findHeight functions.) Also, if the triangle has all sides
with length less than 31 and greater than 0, the provided triangle will
automatically be saved as triangle.sketch.

Draw - This program simply allows a user to draw an image, using the commands
that are available in the sketch program. The user can then save this drawn
image, and name the sketch file it will be stored in.

Note: Any programs that create sketch files, can only create sketch files Using
the non-extended op codes.
