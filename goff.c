#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#define SIZE (30 + 2)
#define XY_TO_I(x, y) y *SIZE + x
#define alive '@'
#define dead ' '
#define ext_size SIZE *SIZE + 2 * SIZE

// random spawn of cells in 1st generation ( рандомное появление клеток в 1ом поколении)
void spawncells(char *grid)
{
    int i = 0;
    for (; i < SIZE * SIZE; ++i)
    {
        grid[i] = (rand() % 2) ? alive : dead;
    }
}

// draw a walls( нарисуем стены )
void drawborders(char *grid)
{
    int i = 0;
    for (i = 1; i < SIZE - 1; ++i)
    {
        grid[i] = '-';
        grid[XY_TO_I(i, (SIZE - 1))] = '-';
    }
    for (i = 1; i < SIZE - 1; ++i)
    {
        grid[XY_TO_I(0, i)] = '|';
        grid[XY_TO_I((SIZE - 1), i)] = '|';
    }
    grid[XY_TO_I(0, 0)] = '+';
    grid[XY_TO_I((SIZE - 1), 0)] = '+';
    grid[XY_TO_I((SIZE - 1), (SIZE - 1))] = '+';
    grid[XY_TO_I(0, (SIZE - 1))] = '+';
}

void initGrid(char *grid)
{
    spawncells(grid);
    drawborders(grid);
}

// Neighbors counting( подсчет соседей )
int countNeighbors(char *grid, int x, int y)
{
    int count = 0;
    int i = -1;
    int j = -1;
    for (; i <= 1; ++i)
    {
        for (j = -1; j <= 1; ++j)
        {
            int neighborX = x + i;
            int neighborY = y + j;

            // Skip current cell ( пропускаем текущую ячейку )
            if (i == 0 && j == 0)
            {
                continue;
            }

            // Check if neighbor is within grid boundaries ( проверяем наличие соседей в окрестностях )
            if (neighborX > 0 && neighborY > 0 && neighborX < SIZE - 1 && neighborY < SIZE - 1)
            {
                count += grid[XY_TO_I(neighborX, neighborY)] == alive;
            }
        }
    }
    return count;
}

// Grid printing( отрисовка поля )
void printGrid(char *grid)
{
    int i = 0;
    int j = 0;
    for (; i < SIZE; ++i)
    {
        for (j = 0; j < SIZE; ++j)
        {
            printf("%c", grid[XY_TO_I(j, i)]);
        }
        printf("\n");
    }
}

// Updating grid to next generation ( переход к новому поколению )
void updateGrid(char *grid)
{
    char nextGrid[ext_size];
    drawborders(nextGrid);
    int j = 1;
    int i = 1;
    for (; i < SIZE - 1; ++i)
    {
        for (j = 1; j < SIZE - 1; ++j)
        {
            int neighbors = countNeighbors(grid, i, j);

            // Apply game of life rules ( опишем правила игры )
            if (grid[XY_TO_I(i, j)])
            {
                if (neighbors < 2 || neighbors > 3)
                {
                    nextGrid[XY_TO_I(i, j)] = dead;
                }
                else
                {
                    nextGrid[XY_TO_I(i, j)] = alive;
                }
            }
            else
            {
                if (neighbors == 3)
                {
                    nextGrid[XY_TO_I(i, j)] = alive;
                }
                else
                {
                    nextGrid[XY_TO_I(i, j)] = dead;
                }
            }
        }
    }

    // Copy next grid to current grid ( копируем следующее поле в текущее )
    memcpy(grid, nextGrid, ext_size);
}

// Clear screen function( функция очистки экрана )
int clear_screen()
{
    system("cls");
    return 0;
}

int main()
{
    srand(time(NULL));
    char grid[ext_size];
    char prev_grid[ext_size];
    memset(grid, 0, ext_size);
    // Initialize grid randomly ( задаем поле рандомно )
    initGrid(grid);
    // Run game of life iterations ( запуск итераций игры )
    while (1)
    {
        clear_screen();
        printGrid(grid);
        memcpy(prev_grid, grid, ext_size);
        updateGrid(grid);
        if (!memcmp(grid, prev_grid, ext_size))
            break;
        Sleep(1000);
    }
    return 0;
}
// The game lasts until the current generation coincides with the previous one.
// Игра длится до тех пор, пока нынешнее поколение не совпадет с предыдущим.