#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For sleep function on Unix-based

#include "termaze.h"

/*
maze generation algorithms:
    - recursive backtracking
    - binary tree algorithm
    - prim's algorithm
    - kruskal's algorithm
    - eller's algorithm
*/

/*
recursive backtracking:
    - start at current cell
    - choose random neighbor cell that has not been visited
    - if all neighbor cells have been visited or there are no
    unvisited neighbors, backtrack to previous cell
    - repeat until all cells have been visited
    - visit ells during process, update maze gird to remove walls
    and create passages (EMPTY)
    - define termination condition
*/

// Recursive backtracking function to generate the maze
void recursive_backtracking(Maze *maze, int row, int col) {

  // Mark current cell as empty
  maze->grid[row][col] = EMPTY;

  // usleep(57500);
  // system("clear"); // Clear screen before displaying
  // display_maze(maze);

  // Define an array of directions (up, down, left, right)
  int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  // Randomize the order of directions
  for (int i = 0; i < 4; i++) {
    int j = rand() % 4;
    int tempX = directions[i][0];
    int tempY = directions[i][1];
    directions[i][0] = directions[j][0];
    directions[i][1] = directions[j][1];
    directions[j][0] = tempX;
    directions[j][1] = tempY;
  }

  // Explore each direction
  for (int i = 0; i < 4; i++) {
    // Find next NODE
    int nextRow = row + directions[i][0] * 2;
    int nextCol = col + directions[i][1] * 2;
    // If the next cell is within borders and is currently a NODE, set to EMPTY
    if (is_valid_cell(maze, nextRow, nextCol) &&
        maze->grid[nextRow][nextCol] == NODE) {
      // Mark next cell in proper direction as empty
      maze->grid[row + directions[i][0]][col + directions[i][1]] = EMPTY;
      recursive_backtracking(maze, nextRow, nextCol);
    }
  }
}
