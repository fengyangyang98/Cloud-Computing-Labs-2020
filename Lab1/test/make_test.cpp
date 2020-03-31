#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <cstring>
#include <fstream>

using namespace std;

string filename;

int sudoku[10][10];
int problem[10][10]; //生成的数独
int row[10][10];	 //行
int column[10][10];	 //列
int block[4][4][10];
int known[10][10];
int f = 0;
int cnt = 0;
void init2()
{
	memset(&sudoku[0][0], 0, sizeof(sudoku));
	memset(&row[0][0], 0, sizeof(row));
	memset(&column[0][0], 0, sizeof(column));
	memset(&block[0][0], 0, sizeof(block));
	memset(&known[0][0], 0, sizeof(known));
	f = 0;
	cnt = 0;
}
void init()
{ //初始化为1 //构造候选数表
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
}
void print(int sudoku[][10])
{
	ofstream data2;
	data2.open(filename.c_str(), ios::app); //打开文件
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 9; j++)
		{
			if (sudoku[i][j])
			{
				data2 << sudoku[i][j] << "";
			}
			else
			{
				data2 << "0";
			}
		}
	}
	data2 << endl;
	data2.close();
}
void search(int x, int y)
{
	if (f)
		return;
	if (known[x][y])
	{												//known非0，为数字； 则搜索下一个单元格
		search((9 * x + y - 9) / 9 + 1, y % 9 + 1); //逐行搜索：[1,2],[1,3]...[1,9],[2,1]...
	}
	else
	{
		if (x == 10 && y == 1)
		{		   //已填完所有的数并合法
			f = 1; //标记为1
			cnt++;
		}
		for (int j = 1; j < 40; j++)
		{
			int i = rand() % 9 + 1;																		//生成1-9之间的随机整数
			if (row[x][i] == 0 && column[y][i] == 0 && block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] == 0) //判断此数是否合法
			{
				sudoku[x][y] = i;
				row[x][i] = 1;
				column[y][i] = 1;
				block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] = 1;
				search((9 * x + y - 9) / 9 + 1, y % 9 + 1); //逐行搜索：[1,2],[1,3]...[1,9],[2,1]...
				if (!f)
				{ //f=0：未填完所有的数
					sudoku[x][y] = 0;
					row[x][i] = 0;
					column[y][i] = 0;
					block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] = 0;
				}
			}
		}
	}
}
void search2(int x, int y)
{
	if (x != 10 && known[x][y])
	{												 //known为数字,且未填完所有的数
		search2((9 * x + y - 9) / 9 + 1, y % 9 + 1); //逐行搜索：[1,2],[1,3]...[1,9],[2,1]...
	}
	else
	{
		if (x == 10 && y == 1)
		{ //已填完所有的数并合法
			cnt++;
		}
		else
			for (int i = 1; i <= 9; i++)
			{
				if (row[x][i] == 0 && column[y][i] == 0 && block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] == 0) //判断此数是否合法
				{
					sudoku[x][y] = i;
					row[x][i] = 1;
					column[y][i] = 1;
					block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] = 1;
					search2((9 * x + y - 9) / 9 + 1, y % 9 + 1);
					sudoku[x][y] = 0;
					row[x][i] = 0;
					column[y][i] = 0;
					block[(x - 1) / 3 + 1][(y - 1) / 3 + 1][i] = 0;
				}
			}
	}
}

int main(int argc, const char **argv)
{
	string num = argv[1];
	filename = argv[2];
	srand(time(0));
	int nnn = atoi(num.c_str());
	while (nnn--)
	{
		init2();
		search(1, 1);
		int sum = 0;
		int sudoku2[10][10] = {0};

		for (int i = 1; i <= 9; i++)
		{
			for (int j = 1; j <= 9; j++)
			{
				problem[i][j] = sudoku2[i][j] = sudoku[i][j];
			}
		}

		cnt = 0;
		while (cnt != 1)
		{
			for (int i = 1; i <= 9; i++)
			{
				for (int j = 1; j <= 9; j++)
				{
					known[i][j] = 1;
				}
			}
			//	cout<<"-3-"<<endl;
			sum = 0;
			while (sum < 50)
			{
				int x = rand() % 9 + 1; //随机生成1-9
				int y = rand() % 9 + 1;
				if (known[x][y] == 0)
					continue; //0:空白
				else
				{
					sum++;
					known[x][y] = 0;
				}
			}
			//	cout<<"-4-"<<endl;
			for (int i = 1; i <= 9; i++)
			{
				for (int j = 1; j <= 9; j++)
				{
					if (known[i][j] == 0)
						sudoku[i][j] = problem[i][j] = 0;
				}
			}
			cnt = 0;
			f = 0;
			memset(row, 0, sizeof(row)); //初始化为0
			memset(column, 0, sizeof(column));
			memset(block, 0, sizeof(block));
			init();		   //初始化为1
			search2(1, 1); //从（1,1）开始搜索
			for (int i = 1; i <= 9; i++)
			{
				for (int j = 1; j <= 9; j++)
				{
					sudoku[i][j] = sudoku2[i][j];
					if (cnt != 1)
						problem[i][j] = sudoku2[i][j];
				}
			}
		}
		print(problem);
	}
	return 0;
}
