
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

  // Explor each direction
  for (int i = 0; i < 4; i++) {
    int nextRow = row + directions[i][0];
    int nextCol = col + directions[i][1];
    if (is_valid_cell(maze, nextRow, nextCol) &&
        maze->grid[nextRow][nextCol == WALL]) {
      // Carve passage between current cell and neighbor
      maze->grid[row + directions[i][0] / 2][col + directions[i][1] / 2] =
          EMPTY;
      recursive_backtracking(maze, nextRow, nextCol);
    }
  }
}
