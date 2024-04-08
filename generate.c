#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "termaze.h"
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

/*
will want to generate only in space not including border, from
start point to end point. so 1 -> ROWS - 2 and 1 -> COLS - 2
*/
int is_valid_cell(const Maze *maze, int row, int col) {
  // Valid cell is inside the borders of the maze
  return (row > 0 && row < ((maze->rows) - 1) && col > 0 &&
          col < ((maze->cols) - 1));
}

// Generate maze using recursive backtracking
void recursive_backtracking(Maze *maze, int row, int col) {

  // Mark start cell as empty
  maze->grid[row][col] = EMPTY;

  // Define an array of directions (up, down, left, right) (x, y)
  int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  // Randomize the order of directions
  for (int i = 0; i < 4; i++) {
    // 0-3
    int j = rand() % 4;
    // original x
    int tempX = directions[i][0];
    // original y
    int tempY = directions[i][1];
    // ith entry in dirs set to random
    directions[i][0] = directions[j][0];
    directions[i][1] = directions[j][1];
    // random entry set to original
    directions[j][0] = tempX;
    directions[j][1] = tempY;
  }

  // Explore each direction
  for (int i = 0; i < 4; i++) {
    // current (x, y) + random direction
    int nextRow = row + directions[i][0] * 2;
    int nextCol = col + directions[i][1] * 2;
    // if within borders and is currently a WALL, set to EMPTY
    if (is_valid_cell(maze, nextRow, nextCol) &&
        maze->grid[nextRow][nextCol] == NODE) {
      // Carve passage between current cell and next
      maze->grid[row + directions[i][0]][col + directions[i][1]] = EMPTY;
      // maze->grid[1 + row + directions[i][0]][1 + col + directions[i][1]] =
      // EMPTY;
      recursive_backtracking(maze, nextRow, nextCol);
    }
  }
}
