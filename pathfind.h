#ifndef PATHFIND_H
#define PATHFIND_H

#include <stdbool.h>

#include "termaze.h"

typedef struct {
  Point point;
  Point parent;
} Node;

typedef struct {
  Node *data;
  int front, rear, size, capacity;
} Queue;

// Function prototypes
void dfs(Maze *maze, int row, int col);

Queue *create_queue(int capacity);
bool is_full(Queue *queue);
bool is_empty(Queue *queue);
void enqueue(Queue *queue, Node item);
Node dequeue(Queue *queue);
void bfs(Maze *maze);

#endif /* PATHFIND_H */