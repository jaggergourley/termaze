#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "generate.h"
#include "pathfind.h"
#include "termaze.h"

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
  clear(); // Clear the screen

  for (int i = 0; i < maze->rows; i++) {
    for (int j = 0; j < maze->cols; j++) {
      switch (maze->grid[i][j]) {
      case EMPTY:
        attron(COLOR_PAIR(2));
        mvprintw(i, j, " "); // Empty
        attroff(COLOR_PAIR(2));
        break;
      case WALL:
        attron(COLOR_PAIR(1));
        mvprintw(i, j, " "); // Wall
        attroff(COLOR_PAIR(1));
        break;
      case NODE:
        attron(COLOR_PAIR(3));
        mvprintw(i, j, " "); // Node
        attroff(COLOR_PAIR(3));
        break;
      case VISITED:
        attron(COLOR_PAIR(4));
        mvprintw(i, j, " "); // Visited
        attroff(COLOR_PAIR(4));
        break;
      case START:
        attron(COLOR_PAIR(5));
        mvprintw(i, j, "@"); // Start
        attroff(COLOR_PAIR(5));
        break;
      case END:
        attron(COLOR_PAIR(5));
        mvprintw(i, j, "?"); // End
        attroff(COLOR_PAIR(5));
        break;
      }
    }
  }
  refresh(); // Refresh the screen
}

int is_valid_cell(const Maze *maze, int row, int col) {
  // Valid cell is inside the borders of the maze
  return (row > 0 && row < ((maze->rows) - 1) && col > 0 &&
          col < ((maze->cols) - 1));
}

void visualization(Maze *maze) {
  usleep(57500); // Sleep before reprinting
  display_maze(maze);
}

void init_ncurses() {
  initscr();
  noecho();
  start_color();
  curs_set(FALSE);

  init_pair(1, COLOR_WHITE, COLOR_WHITE);
  init_pair(2, COLOR_BLACK, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_RED);
  init_pair(4, COLOR_GREEN, COLOR_GREEN);
  init_pair(5, COLOR_WHITE, COLOR_GREEN);
}

int main() {
  srand(time(NULL));

  Maze maze;

  init_ncurses();

  char restart;
  do {
    initialize_maze(&maze);
    display_maze(&maze);

    char generate;
    int valid = 0;

    while (!valid) {
      mvprintw(ROWS, 0, "Select the maze generation algorithm:\n");
      mvprintw(ROWS + 1, 0, "B/b: Binary Tree, ");
      mvprintw(ROWS + 1, 25, "R/r: Resursive Backtracking,\n");
      mvprintw(ROWS + 2, 0, "P/p: Prim's Algorithm, ");
      mvprintw(ROWS + 2, 25, "K/k: Kruskal's Algorithm\n");
      mvprintw(ROWS + 3, 0, "Make your choice and press enter: ");
      refresh(); // Ensure the prompt is displayed

      echo();
      generate = getch();
      noecho();

      while (getch() != '\n')
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
        mvprintw(ROWS + 4, 0, "Invalid Choice, Try Again\n");
        refresh();
        break;
      }
    }

    set_end_point(&maze);
    maze.grid[maze.start.row][maze.start.col] = START;
    display_maze(&maze);

    char pathfind;
    valid = 0;

    while (!valid) {
      mvprintw(ROWS, 0, "Select the maze pathfinding algorithm:\n");
      mvprintw(ROWS + 1, 0, "B/b: Breadth First Search, ");
      mvprintw(ROWS + 1, 30, "D/d: Depth First Search\n");
      mvprintw(ROWS + 2, 0, "Make your choice and press enter: ");
      refresh(); // Ensure the prompt is displayed

      echo();
      pathfind = getch();
      noecho();
      while (getch() != '\n')
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
        mvprintw(ROWS + 3, 0, "Invalid Choice, Try Again\n");
        refresh();
        break;
      }
    }

    display_maze(&maze);

    // Wait for the user to press 'q' to quit or 'r' to restart
    mvprintw(ROWS, 0, "Press 'q' to quit or 'r' to restart.");
    refresh();

    do {
      restart = getch();
    } while (restart != 'q' && restart != 'r');

  } while (restart == 'r');

  endwin();
  return 0;
}
