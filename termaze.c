#include <stdio.h>

#define ROWS 24
#define COLS 80

int main()
{
    // 80 x 24
    char terminal[ROWS][COLS];

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            terminal[i][j] = '*';
        }
    }

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%c", terminal[i][j]);
        }
        printf("\n");
    }
    return 0;
}