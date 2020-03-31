#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;

int sudoku[10][10];
int row[10][10];
int column[10][10];
int block[4][4][10];
int known[10][10];
int counter = 0;

void init()
{
    memset(&sudoku[0][0], 0, sizeof(sudoku));
    memset(&row[0][0], 0, sizeof(row));
    memset(&column[0][0], 0, sizeof(column));
    memset(&block[0][0], 0, sizeof(block));
    memset(&known[0][0], 0, sizeof(known));
}

void print()
{
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            cout << sudoku[i][j];
        }
    }
    cout << '\n';
}
void search(int x, int y)
{
    if (known[x][y])
    {
        search((9 * x + y - 9) / 9 + 1, y % 9 + 1);
    }
    else
    {
        if (x == 10 && y == 1)
        {
            print();
            return;
        }
        else
            for (int i = 1; i <= 9; i++)
            {
                if (row[x][i] == 0 && column[y][i] == 0 && block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] == 0)
                {
                    sudoku[x][y] = i;
                    row[x][i] = 1;
                    column[y][i] = 1;
                    block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] = 1;
                    search((9 * x + y - 9) / 9 + 1, y % 9 + 1);
                    sudoku[x][y] = 0;
                    row[x][i] = 0;
                    column[y][i] = 0;
                    block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] = 0;
                }
            }
    }
}
int main(int argc, char **argv)
{
    ifstream data1;
    data1.open(argv[1], ios::in);
    while (1)
    {
        if (data1.eof() || !data1.is_open())
        {
            break;
        }
        init();
        char ch[81];
        for (int i = 0; i < 81; i++)
        {
            data1 >> ch[i];
        }
        int k = 0;
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                sudoku[i][j] = int(ch[k] - '0');
                k++;
                if (sudoku[i][j])
                    known[i][j] = 1;
            }
        }

        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                if (sudoku[i][j])
                {
                    row[i][sudoku[i][j]] = 1;
                    column[j][sudoku[i][j]] = 1;
                    block[(i - 1) / 3 + 1][(j - 1) / 3 + 1][sudoku[i][j]] = 1;
                }
            }
        }
        search(1, 1);
    }
    data1.close();

    return 0;
}
