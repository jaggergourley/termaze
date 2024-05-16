#ifndef GENERATE_H
#define GENERATE_H

#include "termaze.h"

// Function prototypes

void binary_tree(Maze *maze);

void recursive_backtracking(Maze *maze, int row, int col);

void add_edges_prim(Maze *maze, Point cell, Edge edges[], int *edge_count);
void remove_edge(Edge edges[], int *edge_count, int index);
void prim(Maze *maze);

void add_edges_kruskals(Edge edges[], int *edge_count);
int find(UnionFind *uf, int i);
void union_sets(UnionFind *uf, int i, int j);
void shuffle_edges(Edge edges[], int edge_count);
void kruskal(Maze *maze);

int find_set(int set[], int cell);
void join_sets(int set[], int cell1, int cell2);
void create_vertical_connections(Maze *maze, int set[], int row);
void finalize_maze(Maze *maze, int set[], int row);
void eller(Maze *maze);

#endif /* GENERATE_H */