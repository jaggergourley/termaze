#ifndef GENERATE_H
#define GENERATE_H

#include "termaze.h"

// Function prototypes
void binary_tree(Maze *maze);
void recursive_backtracking(Maze *maze, int row, int col);

void add_edges(Maze *maze, Point cell, Edge edges[], int *edge_count);
void remove_edge(Edge edges[], int *edge_count, int index);
void prims(Maze *maze);

#endif /* GENERATE_H */