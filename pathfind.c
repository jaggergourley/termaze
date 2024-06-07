#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "pathfind.h"
#include "termaze.h"

int endpoint_found = 0;

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

Queue *create_queue(int capacity) {
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  queue->capacity = capacity;
  queue->front = queue->size = 0;
  queue->rear = capacity - 1;
  queue->data = (Node *)malloc(queue->capacity * sizeof(Node));
  return queue;
}

bool is_full(Queue *queue) { return (queue->size == queue->capacity); }

bool is_empty(Queue *queue) { return (queue->size == 0); }

void enqueue(Queue *queue, Node item) {
  if (is_full(queue))
    return;
  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->data[queue->rear] = item;
  queue->size = queue->size + 1;
}

Node dequeue(Queue *queue) {
  if (is_empty(queue))
    exit(EXIT_FAILURE);
  Node item = queue->data[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;
  return item;
}

void bfs(Maze *maze) {
  int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  int found = 0;

  Queue *queue = create_queue(ROWS * COLS);
  Node start_node = {maze->start, {-1, -1}};
  enqueue(queue, start_node);

  maze->grid[maze->start.row][maze->start.col] = VISITED;
  visualization(maze);

  while (!is_empty(queue) && !found) {

    Node current = dequeue(queue);
    Point currentPoint = current.point;

    for (int i = 0; i < 4; i++) {
      int nextRow = currentPoint.row + directions[i][0];
      int nextCol = currentPoint.col + directions[i][1];

      if (nextRow == maze->end.row && nextCol == maze->end.col) {
        found = 1;
        break;
      }

      if (is_valid_cell(maze, nextRow, nextCol) &&
          maze->grid[nextRow][nextCol] == EMPTY) {
        maze->grid[nextRow][nextCol] = VISITED;
        Node nextNode = {{nextRow, nextCol}, currentPoint};
        enqueue(queue, nextNode);

        visualization(maze);
      }
    }
  }

  free(queue->data);
  free(queue);
}