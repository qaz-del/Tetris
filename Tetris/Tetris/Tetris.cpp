#include<iostream>
#include<string>
#include<cstdlib>
#include<windows.h>
#include<ctime>
#include<conio.h>
#include<cstdio>

using namespace std;

//获取标准输出句柄
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

//隐藏光标
void Hide_Handle() 
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(handle, &CursorInfo);
}

class Tetris
{
private:
	int rank;				
	//游戏难度等级
	int score;				
	// 得分
	int id;					
	//图形ID
	int x0, y0;
	//两基点
	int top;					
	//最高点高度
	int color;
public:
	Tetris();
	void Welocme();			
	//首界面
	void DrawMap();			
	//游戏界面
	void SetColor(int);		
	//控制颜色
	void Draw(int, int, int);		
	//画图形
	void Run();				
	//运行游戏
	void Erase(int, int, int);			
	//清除图形
	bool Judge(int, int, int);
	void Turn(int);				
	//旋转
	void Updata();				
	// 更新界面
	void Pause();				
	//游戏暂停
	void Input_score();
	void left();
	void right();
	void quickdown();
};
//组成图形的各个点的各个坐标，先纵后横
//两个数字一组，表示一个方块在一个4*4的空白板上有方格的位置
const int sharp[15][8] =					
{
	//直线
	{0,0,1,0,2,0,3,0},{0,0,0,1,0,2,0,3},
	//方块
{0,0,1,0,0,1,1,1},
//L形
{0,0,1,0,1,1,1,2},{0,1,1,1,2,0,2,1},
{0,0,0,1,0,2,1,2},{0,0,0,1,1,0,2,0},
{1,0,1,1,1,2,0,2},{0,0,0,1,1,1,2,1},
{0,0,0,1,0,2,1,0},{0,0,1,0,2,0,2,1},
//Z形
{0,0,0,1,1,1,1,2},{0,1,1,0,1,1,2,0},
{0,1,0,2,1,0,1,1},{0,0,1,0,1,1,2,1}


};

const int high[15] = { 4,1,2,2,3,2,3,2,3,2,3,2,3,2,3 };

int map[28][16];

#define a1  0			//条形
#define a2  1

#define b 2					// 方块

#define c1 3					//L形
#define c2 4
#define c3 5
#define c4 6

#define d1 7					//T形
#define d2 8 
#define d3 9
#define d4 10

#define e1 11				//闪电1形
#define e2 12

#define f1 13				//闪电2形
#define f2 14

Tetris::Tetris()				//构造函数， 初始化各个值
{
	x0 = 0;
	y0 = 5;
	score = 0;
	top = 25;
	srand((int)time(0));
	color = rand() % 5;
}

void Tetris::Turn(int num)				//旋转函数
{
	switch (num)
	{
	case a1: id = a2; break;					//条形互换
	case a2: id = a1; break;

	case b: id = b; break;					//方块无法旋转

	case c1: id = c2; break;					//各种L形互换
	case c2: id = c3; break;
	case c3: id = c4; break;
	case c4: id = c1; break;

	case d1: id = d2; break;					//各种T形互换
	case d2: id = d3; break;
	case d3: id = d4; break;
	case d4: id = d1; break;

	case e1: id = e2; break;					//两种闪电形互换
	case e2: id = e1; break;

	case f1: id = f2; break;
	case f2: id = f1; break;
	}
	return;

}

void SetPos(int i, int j)			//控制光标位置， 列， 行
{
	COORD pos = { 2*i,j };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Tetris::Pause()				// 暂停函数
{
	SetPos(16, 10);
	cout << "游戏暂停!" << endl;
	SetPos(15, 11);
	cout << "你的分数为 " << score;
	char temp;
	while (1)
	{
		while (1)
		{
			if (_kbhit())
			{
				temp = _getch();
				break;
			}
		}
		if (temp == 32)
			break;
	}
	SetPos(16, 10);					// 清除暂停时显示的信息
	cout << "         ";
	SetPos(15, 11);
	cout << "              ";
	return;

}

void Tetris::Updata()					//更新函数
{
	int i, flag;
	int nx, ny;
	for (i = 0; i < 4; i++)
	{
		nx = x0 + sharp[id][i * 2];
		ny = y0 + sharp[id][i * 2 + 1];
		SetPos(ny + 1, nx + 1);
		cout << "■";
		map[nx][ny] = 1;					
	}
	//界面各个点是否为空的更新

	if (x0 < top)
		top = x0;					
	//最高点的更新

	//消除行
	for (i = x0; i < x0 + high[id]; i++)			
	{
		flag = 1;
		for (int j = 0; j < 13; j++)					
			//判定某一行是否满， 用flag来标记
			if (map[i][j] == 0)
				flag = 0;
		if (flag == 1)
		{
			for (int k = i; k >= top; k--)
			{
				for (int p = 0; p < 13; p++)
				{
					map[k][p] = map[k - 1][p];
					SetPos(p + 1, k + 1);
					if (map[k][p] == 1)
						cout << "■";
					else cout << "  ";

				}
			}
			score += 10;
			Input_score();

		}
	}
	return;


}

void Tetris::Input_score()
{
	SetColor(2);
	SetPos(15, 14);
	cout << "得分: " << score;
	return;

}

void Tetris::Welocme()			//欢迎界面
{
	char x;
	while (1)
	{
		system("cls");
		cout << "		俄罗斯方块		" << endl;
		cout << "		操作方式：" << endl;
		cout << "■ 按1—3选择难度■" << endl;
		SetPos(10, 10);
		x = getchar();
		if (x <= '9' && x >= '0')
		{
			rank = x - '0';
			break;
		}

	}
	return;


}

void Tetris::SetColor(int color_num)			//设置颜色
{
	int n=0;
	switch (color_num)
	{
	case 0: n = 0x08; break;
	case 1: n = 0x0C; break;
	case 2: n = 0x0D; break;
	case 3: n = 0x0E; break;
	case 4: n = 0x0A; break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
	return;

}

void Tetris::DrawMap()				//画游戏时界面
{
	int i;
	SetColor(0);

	for (i = 0; i < 24; i++)		//宽24格
	{
		SetPos(i , 0);
		cout << "■";
		SetPos(i , 26);
		cout << "■";
	}

	for (i = 0; i < 26; i++)		//高26格
	{
		SetPos(0, i);
		cout << "■";
		SetPos(14, i);
		cout << "■";
		SetPos(23, i);
		cout << "■";
	}

	for (i = 14; i < 24; i++)
	{
		SetPos(i , 13);
		cout << "■";
	}

	SetColor(0);
	Input_score();
	SetPos(15, 17);
	cout << "难度等级: " << rank;
	SetPos(15, 19);
	cout << "↑ 旋转";
	SetPos(15, 20);
	cout << "←→ 移动";
	SetPos(15, 21);
	cout << "↓ 快下";
	SetPos(15, 19);
	cout << "空格  暂停";
	SetPos(16, 2);
	cout << "下一图形";
	return;

}

void Tetris::Draw(int x, int y, int num)				//画图形
{
	int nx, ny;
	for (int i = 0; i < 4; i++)
	{
		SetColor(color);
		nx = x + sharp[num][2 * i];
		ny = y + sharp[num][2 * i + 1];
		SetPos(ny + 1, nx + 1);
		cout << "■";
	}
	return;

}

void Tetris::Erase(int x, int y, int num)				//为更新图形的位置清除图形
{
	int nx, ny;
	for (int i = 0; i < 4; i++)
	{
		nx = x + sharp[num][2 * i];
		ny = y + sharp[num][2 * i + 1];
		SetPos(ny + 1, nx + 1);
		cout << "  ";
	}
	return;

}

bool Tetris::Judge(int x, int y, int num)				//判定在x, y 所指位置是否可画编号为
{													//num 的图形， 若不可画则反回true
	int nx, ny;
	for (int i = 0; i < 4; i++)
	{
		nx = x + sharp[num][2 * i];
		ny = y + sharp[num][2 * i + 1];
		if (!(nx < 25 && nx >= 0 && ny < 13 && ny >= 0 && !map[nx][ny]))
			return true;
	}
	return false;
}

void Tetris::left()
{
	if (!Judge(x0, y0 - 1, id))
	{
		Erase(x0, y0, id);
		y0--;
		Draw(x0, y0, id);
	}
	return;

}

void Tetris::right()
{
	if (!Judge(x0, y0 + 1, id))
	{
		Erase(x0, y0, id);
		y0++;
		Draw(x0, y0, id);
	}
	return;

	

}

void Tetris::quickdown()
{
	while (!Judge(x0 + 1, y0, id))
	{
		Erase(x0, y0, id);
		x0 += 1;
		Draw(x0, y0, id);
	}
	return;

}

void Tetris::Run()					//运行游戏
{
	int next_id;
	srand((int)time(0));

	id = rand() % 15;
	next_id = rand() % 15;

	Draw(x0, y0, id);
	Draw(5, 16, next_id);

	int count;
	if (rank == 1)
		count = 150;
	else if (rank == 2)
		count = 100;
	else 
		count =50;
	int i = 0;  //不同等级对应不同count

	while (1)
	{
		//i 与 count 用于控制时间
		if (!(i < count))				
		{
			i = 0;
			//在某一位置不能下落的话
			if (Judge(x0 + 1, y0, id))			
			{
				Updata();
				id = next_id;

				Erase(5, 16, next_id);
				next_id = rand() % 15;

				x0 = 0; y0 = 5;
				color = (color + 1) % 5;
				Draw(x0, y0, id);
				Draw(5, 16, next_id);

				if (Judge(x0, y0, id))
				{
					SetColor(1);
					system("cls");
					SetPos(10, 10);
					cout << "游戏结束！" << endl;
					SetPos(10, 11);
					cout << "你的分数为 " << score << endl;
					system("pause");
					exit(1);
				}
			}
			else					//继续下落
			{
				Erase(x0, y0, id);
				x0++;
				Draw(x0, y0, id);
			}
		}

		if (_kbhit())				//键盘输入值时 
		{
			int key, key2;
			key = _getch();
			if (key == 224)
			{
				key2 = _getch();

				if (key2 == 72)			//按向上方向键时
				{
					int temp = id;
					Turn(id);
					if (Judge(x0, y0, id))
						id = temp;
					Erase(x0, y0, temp);
					Draw(x0, y0, id);
				}
				if (key2 == 80)				//按向下方向键时
				{
					quickdown();
				}
				else if (key2 == 75)				//按向左方向键时
				{
					left();
				}
				else if (key2 == 77)					//按向右方向键时
				{
					right();
				}
			}
			else if (key == 32)					// 按下空格暂停
				Pause();
		}

		Sleep(1);		//等待1毫秒
		i++;				//控制下落间隔
	}
	return;


}

int main()
{
	Hide_Handle();
	Tetris game;
	game.Welocme();
	system("cls");				//清除欢迎界面
	game.DrawMap();
	game.Run();
	return 0;
}