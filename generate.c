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

// Binary tree algorithm to generate the maze
void binary_tree(Maze *maze) {
  // Iterate over every NODE
  for (int i = 1; i < ROWS - 1; i += 2) {
    for (int j = 1; j < COLS - 1; j += 2) {
      // Mark current NODE as EMPTY
      maze->grid[i][j] = EMPTY;

      visualization(maze);

      // If i and j are not in last row/col
      if (i != ROWS - 2 && j != COLS - 2) {
        // Randomly choose to carve right or down
        if (rand() % 2 == 0) {
          // Right
          maze->grid[i][j + 1] = EMPTY;
        } else {
          // Down
          maze->grid[i + 1][j] = EMPTY;
        }
      } else if (i != ROWS - 2) {
        // If i is not in the last row and j is in last col
        // Down
        maze->grid[i + 1][j] = EMPTY;
      } else if (j != COLS - 2) {
        // If j is not in the last col and i is in last row
        // Right
        maze->grid[i][j + 1] = EMPTY;
      }
    }
  }
}

// Recursive backtracking function to generate the maze
void recursive_backtracking(Maze *maze, int row, int col) {

  // Mark current cell as empty
  maze->grid[row][col] = EMPTY;

  visualization(maze);

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
