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

  usleep(10000);
  system("clear"); // Clear screen before displaying
  display_maze(maze);

  if (row == maze->end.row && col == maze->end.col) {
    endpoint_found = 1;
    return;
  }

  // Define an array of directions (up, down, left, right)
  int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  for (int i = 0; i < 4; i++) {
    int nextRow = row + directions[i][0];
    int nextCol = col + directions[i][1];

    if (is_valid_cell(maze, nextRow, nextCol) &&
        maze->grid[nextRow][nextCol] != WALL &&
        maze->grid[nextRow][nextCol] != VISITED) {
      dfs(maze, nextRow, nextCol);
      if (endpoint_found) {
        return;
      }
    }
  }
}