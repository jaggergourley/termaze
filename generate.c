#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For sleep function on Unix-based

#include "termaze.h"

/*
maze generation algorithms:
    - recursive backtracking
    - binary tree algorithm
    - prim's algorithm (randomized)
    - kruskal's algorithm (randomized)
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

void add_edges_prim(Maze *maze, Point cell, Edge edges[], int *edge_count) {
  // Directions of movement to next NODE
  int directions[4][2] = {{0, 2}, {0, -2}, {2, 0}, {-2, 0}};

  // Add each NODE from all four directions of current cell
  for (int i = 0; i < 4; i++) {
    int nextRow = cell.row + directions[i][0];
    int nextCol = cell.col + directions[i][1];

    if (is_valid_cell(maze, nextRow, nextCol)) {
      // If a valid cell, form edge between current cell and cell in given
      // direction
      edges[(*edge_count)++] = (Edge){cell, {nextRow, nextCol}};
    }
  }
}

void remove_edge(Edge edges[], int *edge_count, int index) {
  edges[index] = edges[--(*edge_count)];
}

// Randomlize Prim's algorithm which selects random edge from set to carve maze
void prim(Maze *maze) {

  Point start = {maze->start.row, maze->start.col};
  // Mark start cell as empty
  maze->grid[start.row][start.col] = EMPTY;

  // Initialize list of edges
  Edge edges[ROWS * COLS];
  int edge_count = 0;
  // Add edges of start cell to list
  add_edges_prim(maze, start, edges, &edge_count);

  while (edge_count > 0) {

    visualization(maze);

    // Randomly select an edge from the list
    int rand_index = rand() % edge_count;
    Edge edge = edges[rand_index];

    // Get the two cells connected by this edge
    Point cell1 = edge.cell1;
    Point cell2 = edge.cell2;

    // Check if one cell is EMPTY and the other is a NODE
    if (maze->grid[cell1.row][cell1.col] == EMPTY &&
        maze->grid[cell2.row][cell2.col] == NODE) {
      // Make NODE cell EMPTY
      maze->grid[cell2.row][cell2.col] = EMPTY;
      // Remove wall between the NODE cells
      maze->grid[(cell1.row + cell2.row) / 2][(cell1.col + cell2.col) / 2] =
          EMPTY;
      // Add the new cell's edge to the list
      add_edges_prim(maze, cell2, edges, &edge_count);
      // Same as before if opposite situation
    } else if (maze->grid[cell2.row][cell2.col] == EMPTY &&
               maze->grid[cell1.row][cell1.col] == NODE) {
      maze->grid[cell1.row][cell1.col] = EMPTY;
      maze->grid[(cell1.row + cell2.row) / 2][(cell1.col + cell2.col) / 2] =
          EMPTY;
      add_edges_prim(maze, cell1, edges, &edge_count);
    }

    // Remove the edge from the list
    remove_edge(edges, &edge_count, rand_index);
  }
}

void add_edges_kruskals(Edge edges[], int *edge_count) {
  // Iterate through NODE cells to create all possible edges
  for (int i = 1; i < ROWS; i += 2) {
    for (int j = 1; j < COLS; j += 2) {
      // Add to edges while in bounds between current cell and next in row
      if (i + 2 < ROWS) {
        edges[(*edge_count)++] = (Edge){{i, j}, {i + 2, j}};
      }
      // Add to edges while in bounds between current cell and next in col
      if (j + 2 < COLS) {
        edges[(*edge_count)++] = (Edge){{i, j}, {i, j + 2}};
      }
    }
  }
}

// Find the root a set containing i
int find(UnionFind *uf, int i) {
  // Path compression, point directly to the root
  if (uf->parent[i] != i) {
    uf->parent[i] = find(uf, uf->parent[i]);
  }
  return uf->parent[i];
}

// Merge two sets containing i and j
void union_sets(UnionFind *uf, int i, int j) {
  // Find the roots of the sets
  int ri = find(uf, i);
  int rj = find(uf, j);

  // Union by rank, attach samller tree to larger tree
  if (ri != rj) {
    if (uf->rank[ri] > uf->rank[rj]) {
      uf->parent[rj] = ri;
    } else if (uf->rank[ri] < uf->rank[rj]) {
      uf->parent[ri] = rj;
    } else {
      uf->parent[rj] = ri;
      uf->rank[ri]++;
    }
  }
}

// Randomly shuffle list of edges to make generation random
void shuffle_edges(Edge edges[], int edge_count) {
  for (int i = edge_count - 1; i > 0; --i) {
    int j = rand() % (i + 1);
    Edge temp = edges[i];
    edges[i] = edges[j];
    edges[j] = temp;
  }
}

// Randomized Kruskal's algorithm selecting random edges to carve maze
void kruskal(Maze *maze) {

  // Initialize edges
  Edge edges[ROWS * COLS];
  int edge_count = 0;
  add_edges_kruskals(edges, &edge_count);
  shuffle_edges(edges, edge_count);

  // Initialize union-find structure
  UnionFind uf;
  for (int i = 0; i < ROWS * COLS; i++) {
    // Each set to its own parents, rank 0
    uf.parent[i] = i;
    uf.rank[i] = 0;
  }

  for (int i = 0; i < edge_count; ++i) {
    Point cell1 = edges[i].cell1;
    Point cell2 = edges[i].cell2;
    int index1 = cell1.row * COLS + cell1.col; // Convert 2D to 1D index
    int index2 = cell2.row * COLS + cell2.col;

    // Check if two cells connected by the edge are in different sets
    if (find(&uf, index1) != find(&uf, index2)) {

      visualization(maze);

      // Connect the cells and union the sets
      maze->grid[cell1.row][cell1.col] = EMPTY;
      maze->grid[cell2.row][cell2.col] = EMPTY;
      maze->grid[(cell1.row + cell2.row) / 2][(cell1.col + cell2.col) / 2] =
          EMPTY;
      union_sets(&uf, index1, index2);
    }
  }
}

int find_set(int set[], int cell) {
  // Return identifier for given cell
  return set[cell];
}

void join_sets(int set[], int cell1, int cell2) {
  // Merge two sets by updating set array
  // Ensure all cells in same set have the same identifier
  int set1 = find_set(set, cell1);
  int set2 = find_set(set, cell2);
  for (int i = 0; i < COLS; i++) {
    if (set[i] == set2) {
      set[i] = set1;
    }
  }
}

void create_vertical_connections(Maze *maze, int set[], int row) {
  // Ensure each set has at least one vertical connection to the next row

  // size == COLS
  int connections[COLS] = {0};

  // For each NODE cell in the row
  for (int col = 1; col < COLS; col += 2) {
    // Randomly create vertical passage
    if (rand() % 2 == 0 || connections[find_set(set, col)] == 0) {
      maze->grid[row + 1][col] = EMPTY;
      // Mark set as connected
      connections[find_set(set, col)] = 1;
    }
  }
}

void finalize_maze(Maze *maze, int set[], int row) {
  // Ensure last row is fully connected
  // Iterate through cols in last row
  for (int col = 1; col < COLS - 1; col += 2) {
    if (find_set(set, col) != find_set(set, col + 2)) {
      // Create horizontal passage
      maze->grid[row][col + 1] = EMPTY;
      // Merge sets
      join_sets(set, col, col + 2);
    }
  }
}

void eller(Maze *maze) {

  // set represents a group of connected cells, each cell belongs to a set
  int set[COLS];

  for (int i = 0; i < COLS; i++) {
    // Each cell in first row starts in its own set (initally isolated)
    set[i] = i;
  }

  // Iterate through all rows where NODE cell are present
  for (int row = 1; row < ROWS; row += 2) {

    // Join adjacent cells in current row by iterating through all columns
    for (int col = 1; col < COLS - 1; col += 2) {

      // visualization(maze);
      //   Set current NODE to EMPTY
      maze->grid[row][col] = EMPTY;

      // Randomly decide where to join adjacent cell (create horizonal passage)
      if (rand() % 2 == 0 && find_set(set, col) != find_set(set, col + 2)) {
        // Create passage horizonally
        maze->grid[row][col + 1] = EMPTY;
        // Only merge sets that are not already in the same set to avoid cycles
        join_sets(set, col, col + 2);
      }
    }

    // Handle last cell in row separately
    if (row < ROWS - 2) {
      maze->grid[row][COLS - 2] = EMPTY;
    }

    // Create vertical connections to next row, passing the set array
    create_vertical_connections(maze, set, row);

    // Prepare set array for next row, move to next row
    for (int col = 1; col < COLS; col += 2) {
      // Update set array for new row, maintain connections from previous row
      if (maze->grid[row + 1][col] == EMPTY) {
        // Inherit set identifier from previous col
        set[col] = set[col - 2];
      } else {
        // New set identifier
        set[col] = col + row * COLS;
      }
    }
  }

  // Finalize maze for last row
  finalize_maze(maze, set, ROWS - 2);
}