/* Play noughts and crosses (tic-tac-toe) between two human players. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

//-----------------------------------------------------------------------------
// Setting up types and constants.  Don't change this section.

// Constants for player X, player O, and neither N.
enum player { X, O, N };
typedef enum player player;

// Constants to represent validity checking.
enum validity { Unchecked, OK, BadFormat, BadLetter, BadDigit, BadCell };
typedef enum validity validity;

// A game object contains the entire current state of a game. It contains the
// grid with each cell recording which player owns it, the player whose turn it
// is next, and the number of moves made in total.
struct game {
    player grid[3][3];
    player next;
    int moves;
};
typedef struct game game;

struct action {
    int score;
    char action[3];
};
typedef struct action action;

//-----------------------------------------------------------------------------
// Functions providing the logic of the game.  Develop these.

// Initialize a game, with the given player to move first.
void newGame(game *g, player first) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) g->grid[i][j] = N;
    }
    g->next = first;
    g->moves = 0;
}

//Puts 2 characters into string, specific for creating posisition text
void charsToStr(int n, char text[n][3], char c, char z) {
    text[n][0] = c;
    text[n][1] = z;
    text[n][2] = '\0';
}

// Convert the letter in a valid move string such as "b2" into a row index.
int row(char *text) {
    switch (text[0]) {
      case 'a': return 0;
      case 'b': return 1;
      case 'c': return 2;
    }
    return -1;
}

//Reverts row number to string value
char revRow(int n) {
    switch (n) {
      case 0: return 'a';
      case 1: return 'b';
      case 2: return 'c';
    }
    return 'z';
}

// Convert the digit in a valid move string such as "b2" into a column index.
int col(char *text) {
  switch (text[1]) {
    case '1': return 0;
    case '2': return 1;
    case '3': return 2;
  }
  return -1;
}

//Reverts column number to string value
char revCol(int n) {
    switch (n) {
      case 0: return '1';
      case 1: return '2';
      case 2: return '3';
    }
    return 'z';
}

// Check whether a move string typed in by the user such as "b2" is valid, and
// the corresponding cell is available.  Return a validity code.
validity valid(game *g, char *text) {
    if (strlen(text) != 2) return BadFormat;
    if (row(text) == -1) return BadLetter;
    if (col(text) == -1) return BadDigit;
    if (g->grid[row(text)][col(text)] != N) return BadCell;
    return OK;
}

// Make a move at the given valid position.
void move(game *g, int r, int c) {
    g->grid[r][c] = g->next;
    g->moves++;
    if (g->next == X) {
      g->next = O;
    } else g->next = X;
}

//Updates the game board with the given player, used for minimax
void update(game *g, int r, int c, player p) {
    g->grid[r][c] = p;
}

// Check if the given line is a winning one, and return the winning player.
player line(player x, player y, player z) {
    if (x == y && y == z) return z;
    return N;
}

// Check whether a player has won, and return the winning player.
player won(game *g) {
    if (line(g->grid[0][0], g->grid[0][1], g->grid[0][2]) != N) return line(g->grid[0][0], g->grid[0][1], g->grid[0][2]);
    if (line(g->grid[1][0], g->grid[1][1], g->grid[1][2]) != N) return line(g->grid[1][0], g->grid[1][1], g->grid[1][2]);
    if (line(g->grid[2][0], g->grid[2][1], g->grid[2][2]) != N) return line(g->grid[2][0], g->grid[2][1], g->grid[2][2]);
    if (line(g->grid[0][0], g->grid[1][0], g->grid[2][0]) != N) return line(g->grid[0][0], g->grid[1][0], g->grid[2][0]);
    if (line(g->grid[0][1], g->grid[1][1], g->grid[2][1]) != N) return line(g->grid[0][1], g->grid[1][1], g->grid[2][1]);
    if (line(g->grid[0][2], g->grid[1][2], g->grid[2][2]) != N) return line(g->grid[0][2], g->grid[1][2], g->grid[2][2]);
    if (line(g->grid[0][0], g->grid[1][1], g->grid[2][2]) != N) return line(g->grid[0][0], g->grid[1][1], g->grid[2][2]);
    if (line(g->grid[0][2], g->grid[1][1], g->grid[2][0]) != N) return line(g->grid[0][2], g->grid[1][1], g->grid[2][0]);
    return N;
}

// Check whether the game has ended in a draw.
bool drawn(game *g) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (g->grid[i][j] == N) return false;
      }
    }
    return true;
}

//Finds the number of free cells on the board
int numFree(game g) {
    int count = 0;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if(g.grid[i][j] == N) count++;
      }
    }
    return count;
}

//Finds all available cells on the game board
void getAvail(int n, game g, char cells[n][3]) {
    int count = 0;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if(g.grid[i][j] == N) {
          charsToStr(count, cells, revRow(i), revCol(j));
          count++;
        }
      }
    }
}

//Checks if the game is in a terminal (finished) state
action checkTerm(game g) {
    if (won(&g) == X) {
      action act;
      act.score = -10;
      return act;
    }
  if (won(&g) == O) {
      action act;
      act.score = 10;
      return act;
    }
    if (drawn(&g)) {
      action act;
      act.score = 0;
      return act;
    }
    action act;
    act.score = -1;
    return act;
}

//Checks terminal states for the test AI
action checkTermTest(game g) {
    if (won(&g) == O) {
      action act;
      act.score = -10;
      return act;
    }
  if (won(&g) == X) {
      action act;
      act.score = 10;
      return act;
    }
    if (drawn(&g)) {
      action act;
      act.score = 0;
      return act;
    }
    action act;
    act.score = -1;
    return act;
}

//Finds the best move based on whether the human X or AI O is playing.
action bestMoveFind(player p, action moves[], int max) {

  int bestMove;

  if(p == O) {
    int bestScore = -1000;
    for (int i = 0; i < max; i++) {
      if (moves[i].score > bestScore) {
        bestScore = moves[i].score;
        bestMove = i;
      }
    }
  } else {
    int bestScore = 1000;
    for (int i = 0; i < max; i++) {
      if(moves[i].score < bestScore) {
        bestScore = moves[i].score;
        bestMove = i;
      }
    }
  }

  return moves[bestMove];

}

//Finds the best move for the testing AI
action bestMoveFindAiTest(player p, action moves[], int max) {

  int bestMove;

  if(p == X) {
    int bestScore = -1000;
    for (int i = 0; i < max; i++) {
      if (moves[i].score > bestScore) {
        bestScore = moves[i].score;
        bestMove = i;
      }
    }
  } else {
    int bestScore = 1000;
    for (int i = 0; i < max; i++) {
      if(moves[i].score < bestScore) {
        bestScore = moves[i].score;
        bestMove = i;
      }
    }
  }

  return moves[bestMove];

}

//Minimax algorithm to determine AIs next move, will assume player is X
//This algorithm should ensure that the AI is essentially unbeatable, rendering
//the best case scenario a draw.
action minimax(game g, player p) {
    int max = numFree(g);
    char emptyCells[max][3];
    action moves[max];

    getAvail(max, g, emptyCells);

    if (checkTerm(g).score != -1) return checkTerm(g);

    for (int i = 0; i < max; i++) {
      action act;
      strcpy(act.action, emptyCells[i]);
      update(&g, row(emptyCells[i]), col(emptyCells[i]), p);
      if (p == O) {
        act.score = minimax(g, X).score;
      } else {
        act.score = minimax(g, O).score;
      }

      g.grid[row(emptyCells[i])][col(emptyCells[i])] = N;
      moves[i] = act;
    }

    return bestMoveFind(p, moves, max);
}

//Minimax logic for testing AI
action minimaxAiTest(game g, player p) {
    int max = numFree(g);
    char emptyCells[max][3];
    action moves[max];

    getAvail(max, g, emptyCells);

    if (checkTermTest(g).score != -1) return checkTermTest(g);

    for (int i = 0; i < max; i++) {
      action act;
      strcpy(act.action, emptyCells[i]);
      update(&g, row(emptyCells[i]), col(emptyCells[i]), p);
      if (p == O) {
        act.score = minimaxAiTest(g, X).score;
      } else {
        act.score = minimaxAiTest(g, O).score;
      }

      g.grid[row(emptyCells[i])][col(emptyCells[i])] = N;
      moves[i] = act;
    }

    return bestMoveFindAiTest(p, moves, max);
}

//-----------------------------------------------------------------------------
// Playing the game: Don't change this section until after submitting.

// Convert a player constant into a character for printing.
char show(player c) {
    return (c == X) ? 'X' : (c == O) ? 'O' : ' ';
}

// Print a validity error message.
void printInvalid(validity v) {
    if (v == BadFormat) printf("Type a letter and a digit");
    else if (v == BadLetter) printf("Type a letter a, b or c");
    else if (v == BadDigit) printf("Type a digit 1, 2 or 3");
    else if (v == BadCell) printf("Choose an empty cell");
    printf("\n");
}

//Prints a straight line to console
void printLines(int len) {
    for (int i = 0; i < len; i++) printf("-");
    printf("\n");
}

//Prints row of game grid to console
void printRow(game *g, int row) {
    printf(" %c | %c | %c", show(g->grid[row][0]), show(g->grid[row][1]), show(g->grid[row][2]));
    printf("\n");
}

// Display the grid.
void display(game *g) {
    for(int i = 0; i < 3; i++) {
      printLines(12);
      printRow(g, i);
    }
    printLines(12);
}

// Ask the current player for their move, putting it into the given array.
// Ask repeatedly until the user types in a valid move.
//Specifically handles a human players move
void huMove(game *g) {
    char text[100];
    bool validFlag = false;

    while (!validFlag) {

      printf("Could %c input a valid move in the form a1 \n(a-c for rows 1-3) (1-3 for columns 1-3)\n", show(g->next));
      scanf("%s", text);

      if (valid(g, text) != OK) {
        printInvalid(valid(g, text));
      } else {
        validFlag = true;
      }

    }
    move(g, row(text), col(text));
}

//Handles calling of AI logic, and performing AIs move
void aiMove(game *g) {
    char text[100];

    strcpy(text, minimax(*g, O).action);
    move(g, row(text), col(text));
}

//Runs the AI that will be used to test and play against the main AI
void testAiMove(game *g) {
  char text[100];

  strcpy(text, minimaxAiTest(*g, X).action);
  move(g, row(text), col(text));
}

//Play game, human player against AI
void playAI(char player) {
    bool finish = false;
    game g;
    newGame(&g, player);

    while(!finish) {
      display(&g);
      if (g.next == X) huMove(&g);
      else if (g.next == O) aiMove(&g);
      if (won(&g) != N) {
        finish = true;
        printf("%c WINS!\n", show(won(&g)));
      } else if(drawn(&g)) {
        finish = true;
        printf("IT'S A DRAW!\n");
      }
    }

    display(&g);
}

// Play the game interactively between two human players who take turns.
void play(char player) {
    bool finish = false;
    game g;
    newGame(&g, player);

    while(!finish) {
      display(&g);
      huMove(&g);
      if (won(&g) != N) {
        finish = true;
        printf("%c WINS!\n", show(won(&g)));
      } else if(drawn(&g)) {
        finish = true;
        printf("IT'S A DRAW!\n");
      }
    }

    display(&g);
}

//Runs the game with two AI playing against eachother, until all wanted iterations
//of the game have been run, or the AI does anything but draw.
void testAI(char player) {
  bool finish = false;
  bool draw = true;
  int count = 0;
  game g;

  while (draw && count < 1000) {
    newGame(&g, player);

    while(!finish) {
      display(&g);
      if (g.next == X) testAiMove(&g);
      else if (g.next == O) aiMove(&g);
      if (won(&g) != N) {
        finish = true;
        printf("%c WINS!\n", show(won(&g)));
        draw = false;
      } else if(drawn(&g)) {
        finish = true;
        printf("IT'S A DRAW!\n");
      }
    }
    finish = false;
    count++;
    display(&g);
    printf("%d\n", count);

  }

}

//-----------------------------------------------------------------------------
// Testing and running: Don't change this section.

// Make sure the constants haven't been changed, so that automarking works.
void checkConstants() {
    assert(X == 0 && O == 1 && N == 2);
    assert(Unchecked == 0 && OK == 1 && BadFormat == 2);
    assert(BadLetter == 3 && BadDigit == 4 && BadCell == 5);
}

// Test initialization of the game (tests 1 to 8)
void testNew(game *g) {
    *g = (struct game) {{{X,X,X},{X,X,X},{X,X,X}},N,-1};
    newGame(g, X);
    assert(g->grid[0][0] == N);
    assert(g->grid[1][1] == N);
    assert(g->grid[2][2] == N);
    assert(g->grid[0][2] == N);
    assert(g->grid[2][1] == N);
    assert(g->next == X);
    assert(g->moves == 0);
    newGame(g, O);
    assert(g->next == O);
}

// Test that the row/col functions give the right answers (tests 9 to 14)
void testRowCol() {
    assert(row("a3") == 0);
    assert(row("b2") == 1);
    assert(row("c2") == 2);
    assert(col("b1") == 0);
    assert(col("b2") == 1);
    assert(col("a3") == 2);
}

// Test invalid input, or trying to play in an occupied cell (tests 15 to 22)
void testInvalid(game *g) {
    newGame(g, X);
    assert(valid(g, "d2") == BadLetter);
    assert(valid(g, "b0") == BadDigit);
    assert(valid(g, "b4") == BadDigit);
    assert(valid(g, "2b") == BadLetter);
    assert(valid(g, "b2x") == BadFormat);
    assert(valid(g, "b") == BadFormat);
    assert(valid(g, "") == BadFormat);
    *g = (game) {{{N,N,N},{N,N,N},{N,X,N}},O,1};
    assert(valid(g, "c2") == BadCell);
}

// Test making a move (tests 23 to 28)
void testMove(game *g) {
    newGame(g, X);
    move(g, row("b2"), col("b2"));
    assert(g->grid[1][1] == X);
    assert(g->next == O);
    assert(g->moves == 1);
    move(g, row("a3"), col("a3"));
    assert(g->grid[0][2] == O);
    assert(g->next == X);
    assert(g->moves == 2);
}

// Test the line function (tests 29 to 36)
void testLine() {
    assert(line(X, X, X) == X);
    assert(line(O, O, O) == O);
    assert(line(X, O, O) == N);
    assert(line(O, X, O) == N);
    assert(line(O, O, X) == N);
    assert(line(N, N, N) == N);
    assert(line(X, N, N) == N);
    assert(line(O, O, N) == N);
}

// Test winning lines (tests 37 to 44)
void testWin(game *g) {
    *g = (game) {{{X,X,X},{N,O,N},{N,O,N}},O,5};
    assert(won(g) == X);
    *g = (game) {{{N,O,N},{X,X,X},{N,O,N}},O,5};
    assert(won(g) == X);
    *g = (game) {{{N,O,N},{N,O,N},{X,X,X}},O,5};
    assert(won(g) == X);
    *g = (game) {{{O,N,N},{O,X,N},{O,N,X}},X,5};
    assert(won(g) == O);
    *g = (game) {{{N,O,N},{X,O,N},{N,O,X}},X,5};
    assert(won(g) == O);
    *g = (game) {{{N,N,O},{X,N,O},{N,N,O}},X,5};
    assert(won(g) == O);
    *g = (game) {{{X,N,O},{N,X,O},{N,N,X}},O,5};
    assert(won(g) == X);
    *g = (game) {{{X,N,O},{N,O,X},{O,N,N}},X,5};
    assert(won(g) == O);
}

// Test no winning line (tests 45 to 48)
void testNoWin(game *g) {
    *g = (game) {{{N,N,N},{N,N,N},{N,N,N}},X,0};
    assert(won(g) == N);
    *g = (game) {{{O,N,X},{X,X,O},{O,X,N}},O,7};
    assert(won(g) == N);
    *g = (game) {{{X,O,X},{X,O,O},{O,X,O}},X,9};
    assert(won(g) == N);
    *g = (game) {{{O,O,X},{X,X,O},{O,X,X}},O,9};
    assert(won(g) == N);
}

// Test drawn games (tests 49 to 50)
void testDraw(game *g) {
    *g = (game) {{{O,N,X},{X,X,O},{O,X,N}},O,7};
    assert(drawn(g) == false);
    *g = (game) {{{O,O,X},{X,X,O},{O,X,X}},O,9};
    assert(drawn(g) == true);
}

//Tests that the best move find algorithm will identify the correct move as the best
void testBestMoveFind() {
    action moves[5];
    strcpy(moves[0].action, "a1");
    moves[0].score = 1;
    strcpy(moves[1].action, "a2");
    moves[1].score = 2;
    strcpy(moves[2].action, "a3");
    moves[2].score = 3;
    strcpy(moves[3].action, "b1");
    moves[3].score = 5;
    strcpy(moves[4].action, "b2");
    moves[4].score = 4;
    assert(strcmp(bestMoveFind(O, moves, 5).action, "b1") == 0);
    assert(strcmp(bestMoveFind(X, moves, 5).action, "a1") == 0);
}

void testCheckTerm() {
    game g;
    g = (game) {{{X,X,X},{N,O,N},{N,O,N}},O,5};
    assert(checkTerm(g).score == -10);
    g = (game) {{{O,N,N},{O,X,N},{O,N,X}},X,5};
    assert(checkTerm(g).score == 10);
    g = (game) {{{O,O,X},{X,X,O},{O,X,X}},O,9};
    assert(checkTerm(g).score == 0);
    g = (game) {{{N,N,N},{N,N,N},{N,N,N}},X,0};
    assert(checkTerm(g).score == -1);
}

// Unit testing.
void test() {
    game *g = malloc(sizeof(game));
    checkConstants();
    testNew(g);
    testRowCol();
    testInvalid(g);
    testMove(g);
    testLine();
    testWin(g);
    testNoWin(g);
    testDraw(g);
    testBestMoveFind();
    testCheckTerm();
    free(g);
    printf("All tests passed.\n");
}

// Run the program with no args to carry out the tests, or with one arg (the
// player to go first) to play the game.
int main(int n, char *args[n]) {
    if (n == 1) test();
    else if (n == 2 && strcmp(args[1],"X") == 0) play(X);
    else if (n == 2 && strcmp(args[1],"O") == 0) play(O);
    else if (n == 2 && strcmp(args[1],"AI") == 0) playAI(X);
    else if (n == 2 && strcmp(args[1],"AITEST") == 0) testAI(X);
    else {
        fprintf(stderr, "Use: ./oxo  OR  ./oxo X  OR  ./oxo O\n");
        exit(1);
    }
    return 0;
}
