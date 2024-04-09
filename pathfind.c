#include <stdlib.h>

#include "termaze.h"

int endpoint_found = 0;

/*
pathfinding algorithms:
    - dfs
    - bfs
    - a*
    - dijkstra's
    - bellman-ford
    - floyd-warshall
*/

// Recusive depth first search to solve the maze
void dfs(Maze *maze, int row, int col) {

  // Mark current cell as visited
  maze->grid[row][col] = VISITED;

  visualization(maze);

  // Check if current position is the end point
  if (row == maze->end.row && col == maze->end.col) {
    // Set flag to indicate endpoint is found
    endpoint_found = 1;
    // Terminate DFS
    return;
  }

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

  // Explore each neighbor
  for (int i = 0; i < 4; i++) {
    int nextRow = row + directions[i][0];
    int nextCol = col + directions[i][1];

    // Check if next cell is valid and not a wall or already visited
    if (is_valid_cell(maze, nextRow, nextCol) &&
        maze->grid[nextRow][nextCol] != WALL &&
        maze->grid[nextRow][nextCol] != VISITED) {
      dfs(maze, nextRow, nextCol);

      if (endpoint_found) {
        // Terminate DFS
        return;
      }
    }
  }
}