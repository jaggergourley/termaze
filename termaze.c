#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "generate.h"
#include "pathfind.h"

/*
maze generation algorithms:
    - recursive backtracking
    - binary tree algorithm
    - prim's algorithm
    - kruskal's algorithm
    - eller's algorithm

pathfinding algorithms:
    - dfs
    - bfs
    - a*
    - dijkstra's
    - bellman-ford
    - floyd-warshall

visualization:
    - ascii art w/ colors in terminal
    - show process of generate and pathfind

main program logic:
    - coordinate generation, pathfinding, and visualization
    - user options for algorithms and visualization process

*/

#define ANSI_COLOR_RED "\033[0;31m"
#define ANSI_COLOR_GREEN "\033[0;32m"
#define ANSI_COLOR_YELLOW "\033[0;33m"
#define ANSI_COLOR_BLUE "\033[0;34m"
#define ANSI_COLOR_MAGENTA "\033[0;35m"
#define ANSI_COLOR_CYAN "\033[0;36m"
#define ANSI_COLOR_WHITE "\033[0;37m"
#define ANSI_COLOR_RESET "\033[0m"
#define ROWS 21
#define COLS 80

/*
maze:
    - 2d array where each cell is location
    - 0: empty cell
    - 1: wall
    - 2: start point
    - 3: end point
*/
// Define maze data structure
int maze[ROWS][COLS];

// Initialize max with walls (1)
void initialize_maze() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      maze[i][j] = 1; // Set all cells to walls
    }
  }
  maze[ROWS / 2][COLS / 2] = 2; // Set start point in middle
}

void set_end_point() {
  int side = rand() % 4; // 0: top, 1: bot, 2: left, 3: right

  int row, col;
  switch (side) {
  case 0: // top
    row = 0;
    col = 1 + rand() % (COLS - 1); // 1 - (COLS-1)
    break;
  case 1: // bottom
    row = ROWS - 1;
    col = 1 + rand() % (COLS - 1); // 1 - (COLS-1)
    break;
  case 2:                    // left
    row = 1 + rand() % ROWS; // 1 - (ROWS-1)
    col = 0;
    break;
  case 3:                    // right
    row = 1 + rand() % ROWS; // 1 - (ROWS-1)
    col = COLS - 1;
    break;
  }

  maze[row][col] = 3; // Set end point
}

void display_maze() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      switch (maze[i][j]) {
      case 0:
        printf(" "); // Empty
        break;
      case 1:
        printf("#"); // Wall
        break;
      case 2:
        printf(ANSI_COLOR_GREEN "@" ANSI_COLOR_RESET); // Start
        break;
      case 3:
        printf(ANSI_COLOR_GREEN "?" ANSI_COLOR_RESET); // End
        break;
      }
    }
    printf("\n");
  }
}

int main() {
  // Seed random number generator with current time
  srand(time(NULL));

  initialize_maze();
  set_end_point();
  display_maze();
  return 0;
}