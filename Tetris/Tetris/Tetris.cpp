#include<iostream>
#include<string>
#include<cstdlib>
#include<windows.h>
#include<ctime>
#include<conio.h>
#include<cstdio>

using namespace std;

//��ȡ��׼������
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

//���ع��
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
	//��Ϸ�Ѷȵȼ�
	int score;				
	// �÷�
	int id;					
	//ͼ��ID
	int x0, y0;
	//������
	int top;					
	//��ߵ�߶�
	int color;
public:
	Tetris();
	void Welocme();			
	//�׽���
	void DrawMap();			
	//��Ϸ����
	void SetColor(int);		
	//������ɫ
	void Draw(int, int, int);		
	//��ͼ��
	void Run();				
	//������Ϸ
	void Erase(int, int, int);			
	//���ͼ��
	bool Judge(int, int, int);
	void Turn(int);				
	//��ת
	void Updata();				
	// ���½���
	void Pause();				
	//��Ϸ��ͣ
	void Input_score();
	void left();
	void right();
	void quickdown();
};
//���ͼ�εĸ�����ĸ������꣬���ݺ��
//��������һ�飬��ʾһ��������һ��4*4�Ŀհװ����з����λ��
const int sharp[15][8] =					
{
	//ֱ��
	{0,0,1,0,2,0,3,0},{0,0,0,1,0,2,0,3},
	//����
{0,0,1,0,0,1,1,1},
//L��
{0,0,1,0,1,1,1,2},{0,1,1,1,2,0,2,1},
{0,0,0,1,0,2,1,2},{0,0,0,1,1,0,2,0},
{1,0,1,1,1,2,0,2},{0,0,0,1,1,1,2,1},
{0,0,0,1,0,2,1,0},{0,0,1,0,2,0,2,1},
//Z��
{0,0,0,1,1,1,1,2},{0,1,1,0,1,1,2,0},
{0,1,0,2,1,0,1,1},{0,0,1,0,1,1,2,1}


};

const int high[15] = { 4,1,2,2,3,2,3,2,3,2,3,2,3,2,3 };

int map[28][16];

#define a1  0			//����
#define a2  1

#define b 2					// ����

#define c1 3					//L��
#define c2 4
#define c3 5
#define c4 6

#define d1 7					//T��
#define d2 8 
#define d3 9
#define d4 10

#define e1 11				//����1��
#define e2 12

#define f1 13				//����2��
#define f2 14

Tetris::Tetris()				//���캯���� ��ʼ������ֵ
{
	x0 = 0;
	y0 = 5;
	score = 0;
	top = 25;
	srand((int)time(0));
	color = rand() % 5;
}

void Tetris::Turn(int num)				//��ת����
{
	switch (num)
	{
	case a1: id = a2; break;					//���λ���
	case a2: id = a1; break;

	case b: id = b; break;					//�����޷���ת

	case c1: id = c2; break;					//����L�λ���
	case c2: id = c3; break;
	case c3: id = c4; break;
	case c4: id = c1; break;

	case d1: id = d2; break;					//����T�λ���
	case d2: id = d3; break;
	case d3: id = d4; break;
	case d4: id = d1; break;

	case e1: id = e2; break;					//���������λ���
	case e2: id = e1; break;

	case f1: id = f2; break;
	case f2: id = f1; break;
	}
	return;

}

void SetPos(int i, int j)			//���ƹ��λ�ã� �У� ��
{
	COORD pos = { 2*i,j };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Tetris::Pause()				// ��ͣ����
{
	SetPos(16, 10);
	cout << "��Ϸ��ͣ!" << endl;
	SetPos(15, 11);
	cout << "��ķ���Ϊ " << score;
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
	SetPos(16, 10);					// �����ͣʱ��ʾ����Ϣ
	cout << "         ";
	SetPos(15, 11);
	cout << "              ";
	return;

}

void Tetris::Updata()					//���º���
{
	int i, flag;
	int nx, ny;
	for (i = 0; i < 4; i++)
	{
		nx = x0 + sharp[id][i * 2];
		ny = y0 + sharp[id][i * 2 + 1];
		SetPos(ny + 1, nx + 1);
		cout << "��";
		map[nx][ny] = 1;					
	}
	//����������Ƿ�Ϊ�յĸ���

	if (x0 < top)
		top = x0;					
	//��ߵ�ĸ���

	//������
	for (i = x0; i < x0 + high[id]; i++)			
	{
		flag = 1;
		for (int j = 0; j < 13; j++)					
			//�ж�ĳһ���Ƿ����� ��flag�����
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
						cout << "��";
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
	cout << "�÷�: " << score;
	return;

}

void Tetris::Welocme()			//��ӭ����
{
	char x;
	while (1)
	{
		system("cls");
		cout << "		����˹����		" << endl;
		cout << "		������ʽ��" << endl;
		cout << "�� ��1��3ѡ���Ѷȡ�" << endl;
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

void Tetris::SetColor(int color_num)			//������ɫ
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

void Tetris::DrawMap()				//����Ϸʱ����
{
	int i;
	SetColor(0);

	for (i = 0; i < 24; i++)		//��24��
	{
		SetPos(i , 0);
		cout << "��";
		SetPos(i , 26);
		cout << "��";
	}

	for (i = 0; i < 26; i++)		//��26��
	{
		SetPos(0, i);
		cout << "��";
		SetPos(14, i);
		cout << "��";
		SetPos(23, i);
		cout << "��";
	}

	for (i = 14; i < 24; i++)
	{
		SetPos(i , 13);
		cout << "��";
	}

	SetColor(0);
	Input_score();
	SetPos(15, 17);
	cout << "�Ѷȵȼ�: " << rank;
	SetPos(15, 19);
	cout << "�� ��ת";
	SetPos(15, 20);
	cout << "���� �ƶ�";
	SetPos(15, 21);
	cout << "�� ����";
	SetPos(15, 19);
	cout << "�ո�  ��ͣ";
	SetPos(16, 2);
	cout << "��һͼ��";
	return;

}

void Tetris::Draw(int x, int y, int num)				//��ͼ��
{
	int nx, ny;
	for (int i = 0; i < 4; i++)
	{
		SetColor(color);
		nx = x + sharp[num][2 * i];
		ny = y + sharp[num][2 * i + 1];
		SetPos(ny + 1, nx + 1);
		cout << "��";
	}
	return;

}

void Tetris::Erase(int x, int y, int num)				//Ϊ����ͼ�ε�λ�����ͼ��
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

bool Tetris::Judge(int x, int y, int num)				//�ж���x, y ��ָλ���Ƿ�ɻ����Ϊ
{													//num ��ͼ�Σ� �����ɻ��򷴻�true
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

void Tetris::Run()					//������Ϸ
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
	int i = 0;  //��ͬ�ȼ���Ӧ��ͬcount

	while (1)
	{
		//i �� count ���ڿ���ʱ��
		if (!(i < count))				
		{
			i = 0;
			//��ĳһλ�ò�������Ļ�
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
					cout << "��Ϸ������" << endl;
					SetPos(10, 11);
					cout << "��ķ���Ϊ " << score << endl;
					system("pause");
					exit(1);
				}
			}
			else					//��������
			{
				Erase(x0, y0, id);
				x0++;
				Draw(x0, y0, id);
			}
		}

		if (_kbhit())				//��������ֵʱ 
		{
			int key, key2;
			key = _getch();
			if (key == 224)
			{
				key2 = _getch();

				if (key2 == 72)			//�����Ϸ����ʱ
				{
					int temp = id;
					Turn(id);
					if (Judge(x0, y0, id))
						id = temp;
					Erase(x0, y0, temp);
					Draw(x0, y0, id);
				}
				if (key2 == 80)				//�����·����ʱ
				{
					quickdown();
				}
				else if (key2 == 75)				//���������ʱ
				{
					left();
				}
				else if (key2 == 77)					//�����ҷ����ʱ
				{
					right();
				}
			}
			else if (key == 32)					// ���¿ո���ͣ
				Pause();
		}

		Sleep(1);		//�ȴ�1����
		i++;				//����������
	}
	return;


}

int main()
{
	Hide_Handle();
	Tetris game;
	game.Welocme();
	system("cls");				//�����ӭ����
	game.DrawMap();
	game.Run();
	return 0;
}