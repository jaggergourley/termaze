#ifndef TERMAZE_H
#define TERMAZE_H

#define ROWS 21
#define COLS 79

// Define maze cell types
enum CellType { WALL, NODE, EMPTY, START, END };

// Define a struct for a point
typedef struct {
  int row;
  int col;
} Point;

// Define maze struct
typedef struct {
  int rows;
  int cols;
  enum CellType grid[ROWS][COLS];
  Point start;
  Point end;
} Maze;

// Function prototypes
void initialize_maze(Maze *maze);
void set_end_point(Maze *maze);
void display_maze(const Maze *maze);

#endif /* TERMAZE_H */