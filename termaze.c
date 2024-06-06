#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "generate.h"
#include "pathfind.h"
#include "termaze.h"

/*
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
#define ANSI_BKGRND_BLACK "\x1b[0;40m"
#define ANSI_BKGRND_RED "\x1b[0;41m"
#define ANSI_BKGRND_GREEN "\x1b[0;42m"
#define ANSI_BKGRND_YELLOW "\x1b[0;43m"
#define ANSI_BKGRND_BLUE "\x1b[0;44m"
#define ANSI_BKGRND_MAGENTA "\x1b[0;45m"
#define ANSI_BKGRND_CYAN "\x1b[0;46m"
#define ANSI_BKGRND_WHITE "\x1b[0;47m"

void initialize_maze(Maze *maze) {
  maze->rows = ROWS;
  maze->cols = COLS;
  // Set start point in middle
  maze->start.row = ROWS / 2;
  maze->start.col = COLS / 2;

  // Initialize maze grid with walls
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      maze->grid[i][j] = WALL;
      // Set all (odd x, odd y) as NODE for carving maze
      if (i % 2 != 0 && j % 2 != 0) {
        maze->grid[i][j] = NODE;
      }
    }
  }
}

void set_end_point(Maze *maze) {
  int side = rand() % 4; // 0: top, 1: bot, 2: left, 3: right

  int row, col;
  // Continue to set randomly if not reachable (blocked by WALL)
  switch (side) {
  case 0: // top
    row = 1;
    col = 1 + rand() % (COLS - 2);
    while (maze->grid[row][col] != EMPTY) {
      col = 1 + rand() % (COLS - 2);
    }
    break;
  case 1: // bottom
    row = ROWS - 2;
    col = 1 + rand() % (COLS - 2);
    while (maze->grid[row][col] != EMPTY) {
      col = 1 + rand() % (COLS - 2);
    }
    break;
  case 2: // left
    row = 1 + rand() % (ROWS - 2);
    col = 1;
    while (maze->grid[row][col] != EMPTY) {
      row = 1 + rand() % (ROWS - 2);
    }
    break;
  case 3: // right
    row = 1 + rand() % (ROWS - 2);
    col = COLS - 2;
    while (maze->grid[row][col] != EMPTY) {
      row = 1 + rand() % (ROWS - 2);
    }
    break;
  }

  // Set end point
  maze->end.row = row;
  maze->end.col = col;
  // Set end point on grid
  maze->grid[maze->end.row][maze->end.col] = END;
}

void display_maze(const Maze *maze) {

  // Clear the screen
  // printf("\x1b[2J");

  for (int i = 0; i < maze->rows; i++) {
    for (int j = 0; j < maze->cols; j++) {
      switch (maze->grid[i][j]) {
      case EMPTY:
        mvprintw(i, j, ANSI_BKGRND_BLACK " " ANSI_COLOR_RESET); // Empty
        break;
      case WALL:
        mvprintw(i, j, ANSI_BKGRND_WHITE " " ANSI_COLOR_RESET); // Wall
        break;
      case NODE:
        mvprintw(i, j, ANSI_BKGRND_RED " " ANSI_COLOR_RESET); // Node
        break;
      case VISITED:
        mvprintw(i, j, ANSI_BKGRND_GREEN " " ANSI_COLOR_RESET); // Visited
        break;
      case START:
        mvprintw(i, j, ANSI_BKGRND_GREEN "@" ANSI_COLOR_RESET); // Start
        break;
      case END:
        mvprintw(i, j, ANSI_BKGRND_GREEN "?" ANSI_COLOR_RESET); // End
        break;
      }
    }
  }
}

int is_valid_cell(const Maze *maze, int row, int col) {
  // Valid cell is inside the borders of the maze
  return (row > 0 && row < ((maze->rows) - 1) && col > 0 &&
          col < ((maze->cols) - 1));
}

void visualization(Maze *maze) {
  // Sleeps and clears screen before reprinting to show
  // process of generation/pathfinding
  usleep(15000);
  refresh();
  display_maze(maze);
}

int main() {
  // Seed random number generator with current time
  srand(time(NULL));

  Maze maze;

  initscr();
  noecho();
  curs_set(FALSE);

  initialize_maze(&maze);
  display_maze(&maze);

  char generate;
  int valid = 0;

  while (!valid) {
    mvprintw(0, ROWS + 1, "Select the maze generation algorithm:\n");
    mvprintw(0, ROWS + 2, "B/b: Binary Tree, ");
    mvprintw(20, ROWS + 2, "R/r: Resursive Backtracking,\n");
    mvprintw(0, ROWS + 3, "P/p: Prim's Algorithm, ");
    mvprintw(20, ROWS + 3, "K/k: Kruskal's Algorithm\n");
    mvprintw(0, ROWS + 4, "Make your choice and press enter: ");

    generate = getchar();
    while (getchar() != '\n')
      ;

    switch (generate) {
    case 'B':
    case 'b':
      binary_tree(&maze);
      valid = 1;
      break;
    case 'R':
    case 'r':
      recursive_backtracking(&maze, maze.start.row, maze.start.col);
      valid = 1;
      break;
    case 'P':
    case 'p':
      prim(&maze);
      valid = 1;
      break;
    case 'K':
    case 'k':
      kruskal(&maze);
      valid = 1;
      break;
    default:
      mvprintw(0, ROWS + 5, "Invalid Choice\n");
      break;
    }
  }

  // Set endpoint after generation to ensure that it is reachable
  set_end_point(&maze);

  // Set start point on grid
  maze.grid[maze.start.row][maze.start.col] = START;
  system("clear"); // Clear screen before displaying
  display_maze(&maze);

  char pathfind;
  valid = 0;

  while (!valid) {
    mvprintw(0, ROWS + 1, "Select the maze pathfinding algorithm:\n");
    mvprintw(0, ROWS + 2, "B/b: Breadth First Search, ");
    mvprintw(20, ROWS + 2, "D/d: Depth First Search\n");
    mvprintw(0, ROWS + 3, "Make your choice and press enter: ");

    pathfind = getchar();
    while (getchar() != '\n')
      ;

    switch (pathfind) {
    case 'B':
    case 'b':
      bfs(&maze);
      valid = 1;
      break;
    case 'D':
    case 'd':
      dfs(&maze, maze.start.row, maze.start.col);
      valid = 1;
      break;
    default:
      mvprintw(0, ROWS + 4, "Invalid Choice\n");
      break;
    }
  }

  refresh();
  display_maze(&maze);
  endwin();
  return 0;
}