#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "generate.h"
#include "pathfind.h"
#include "termaze.h"

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

input handling:
    - allow user to select maze generation and pathfinding algos
    - possible control other parameters like maze size, etc.

main program logic:
    - coordinate generation, pathfinding, and visualization
    - user options for algorithms and visualization process

utility functions:
    - check if cell is within boundaries
    - check is cell is on edge

*/

#define ANSI_COLOR_RED "\x1b[0;31m"
#define ANSI_COLOR_GREEN "\x1b[0;32m"
#define ANSI_COLOR_YELLOW "\x1b[0;33m"
#define ANSI_COLOR_BLUE "\x1b[0;34m"
#define ANSI_COLOR_MAGENTA "\x1b[0;35m"
#define ANSI_COLOR_CYAN "\x1b[0;36m"
#define ANSI_COLOR_WHITE "\x1b[0;37m"
#define ANSI_COLOR_RESET "\x1b[0m"

/*
we need to think of maze as carving through the walls
we need nodes on every odd valued cell (odd x, odd y)
*/

// Initialize maze with walls (1)
void initialize_maze(Maze *maze) {
  maze->rows = ROWS;
  maze->cols = COLS;
  // Set start point in middle
  maze->start.row = ROWS / 2 - 1;
  maze->start.col = COLS / 2;

  // Initialize maze grid with walls
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      maze->grid[i][j] = WALL;
      if (i % 2 != 0 && j % 2 != 0) {
        maze->grid[i][j] = NODE;
      }
    }
  }

  // Set start point on grid
  maze->grid[maze->start.row][maze->start.col] = START;
}

void set_end_point(Maze *maze) {
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
  case 2:                          // left
    row = 1 + rand() % (ROWS - 1); // 1 - (ROWS-1)
    col = 0;
    break;
  case 3:                          // right
    row = 1 + rand() % (ROWS - 1); // 1 - (ROWS-1)
    col = COLS - 1;
    break;
  }

  // Set end point
  maze->end.row = row;
  maze->end.col = col;
  // Set end point on grid
  maze->grid[maze->end.row][maze->end.col] = END;
}

void display_maze(const Maze *maze) {
  for (int i = 0; i < maze->rows; i++) {
    for (int j = 0; j < maze->cols; j++) {
      switch (maze->grid[i][j]) {
      case EMPTY:
        printf(" "); // Empty
        break;
      case WALL:
        printf("#"); // Wall
        break;
      case NODE:
        printf(" ");
        break;
      case START:
        printf(ANSI_COLOR_GREEN "@" ANSI_COLOR_RESET); // Start
        break;
      case END:
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

  Maze maze;

  initialize_maze(&maze);

  set_end_point(&maze);
  recursive_backtracking(&maze, maze.start.row, maze.start.col);
  display_maze(&maze);
  return 0;
}