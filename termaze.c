#include <stdio.h>

#include "generate.h"
#include "pathfind.h"

/*
maze generation algorithms:
    - recursive backtracking
    - binary tree algorithm
    - prim's algorithm
    - kruskal's algorithm
    - eller's algorithm

pathfinding algorithms:
    - dfs
    - bfs
    - a*
    - dijkstra's
    - bellman-ford
    - floyd-warshall

visualization:
    - ascii art w/ colors in terminal
    - show process of generate and pathfind

main program logic:
    - coordinate generation, pathfinding, and visualization
    - user options for algorithms and visualization process

*/

/*
colors in terminal:
\033[0;31m red
\033[0;32m green
\033[0;33m yellow
\033[0;34m blue
\033[0;35m magenta
\033[0;36m cyan
\033[0;37m white
*/

/*
maze:
    - 2d array where each cell is location
    - 0: empty cell
    - 1: wall
    - 2: start point
    - 3: end point
*/

#define ROWS 21
#define COLS 80

// Define maze data structure
int maze[ROWS][COLS];

void initialize_maze() {}

int main() { return 0; }