For my extra work I will be submitting oxoAI.c
This program will allow a user, who is assumed to not be attempting to
purposefully cause issues such as buffer overflows on text entry, to either:
-Play a game against another player by initialising the program with ./oxoAI (X/O)
-Play a game against an AI, which was designed making use of a minimax algorithm that should
make the best case scenario a draw. This can be started by initialising the program with
./oxoAI AI
-Start a game in which the AI will play itself, with the games being displayed, and a counter
showing the number of games that have occurred. This is included to test that the AI will
repeatedly tie matches when playing against itself, primarily tests the consistency of the algorithm.
To start the the program should be initialised with ./oxoAI AITEST

Note: When the AI play against each other, all games are identical, as the AI will always pick the same first move,
which will always be countered in the same way, and so on.

My implementation for the minimax algorithm was based on the information given in this guide:
https://medium.freecodecamp.org/how-to-make-your-tic-tac-toe-game-unbeatable-by-using-the-minimax-algorithm-9d690bad4b37

The guide provides an implementation of the algorithm in javascript, which I have used to design my implementation in C.

I did have to make changes as the game in my program is designed in a different way to the example provided, and
the most obvious issue being that I have implemented this algorithm in C instead of what appears to be JavaScript.
(It may say on the page, but if it does I didn't see it)
