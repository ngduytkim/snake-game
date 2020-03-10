#include "pch.h"
#include <iostream>
#include <string>
#include "windows.h"
#include <ctime>
#include "Menu.h"
#include "Graphics.h"
#include "Variables.h"
using namespace std;

#define MAX_SIZE_SNAKE 10
#define MAX_SIZE_FOOD 4
#define MAX_SPEED 3

POINT snake[10];
POINT food;
POINT portal[5];
int CHAR_LOCK;
int MOVING;
int SPEED;
int HEIGHT_CONSOLE, WIDTH_CONSOLE;
int FOOD_INDEX;
int SIZE_SNAKE;
int STATE;
int PORTAL; // bien kiem tra xem co cong ko
int CHECKPORTAL; // bien kiem tra xem ran bat dau chui qua cong chua

int TOPX = 20;


bool isValid(int x, int y)
{
	for (int i = 0;i < SIZE_SNAKE; i++)
		if (snake[i].x == x && snake[i].y == y)
			return false;
	return true;
}

void GenerateFood()
{
	int x, y;
	srand(unsigned int(time(NULL)));
	do {
			x = rand() % (WIDTH_CONSOLE - 1) + 1 + TOPX;
			y = rand() % (HEIGHT_CONSOLE - 1) + 1;
	} while (isValid(x, y) == false);
	food = { x,y };
}

void ResetData()
{
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1;
	FOOD_INDEX = 0, WIDTH_CONSOLE = 75, HEIGHT_CONSOLE = 20, SIZE_SNAKE = 6;
	snake[0] = { TOPX+10,6 }; snake[1] = { TOPX+11,6 };
	snake[2] = { TOPX+12,6 }; snake[3] = { TOPX+13,6 };
	snake[4] = { TOPX+14,6 }; snake[5] = { TOPX+15,6 };
	for (int i = 0;i < 5;i++)
		portal[i] = { 0,0 };
	GenerateFood();
}


bool checkPortalpos(int x, int y)
{
	for (int i = -1;i < 5;i++)
	{
		for (int j = -1;j < 5;j++)
		{
			if (!isValid(x + i, y + j))
				return false;
		}
	}
	return true;
}

void GeneratePortal()
{
	PORTAL = 1;
	CHECKPORTAL = 0;
	int x, y;
	srand(unsigned int(time(NULL)));
	do {
		x = rand() % (WIDTH_CONSOLE - 3) + 1 + TOPX;
		y = rand() % (HEIGHT_CONSOLE - 3) + 1;
	} while (!checkPortalpos(x,y));
	portal[0] = { x,y };
	portal[1] = { x + 1,y };
	portal[2] = { x + 2,y };
	portal[3] = { x,y + 1 };
	portal[4] = { x + 2,y + 1 };
}

bool isPortalGate(int x, int y)
{
	POINT c;
	c.x = portal[0].x + 1;
	c.y = portal[0].y + 1;
	if (x == c.x&&y == c.y)
		return true;
	return false;
}

int snakeatGate()
{
	for (int i = 0;i < SIZE_SNAKE;i++)
	{
		if (isPortalGate(snake[i].x, snake[i].y))
			return i;
	}
	return -1;
}

bool isPortal(int x, int y)
{
	for (int i = 0;i < 5;i++)
	{
		if (x == portal[i].x&&y == portal[i].y)
			return true;
	}
	return false;
}

void DrawPortal(char ch)
{
	for (int i = 0;i < 5;i++)
	{
		GotoXY(portal[i].x, portal[i].y);
		cout << ch;
	}
}

void LevelUp()
{
	PORTAL = 0;
	FOOD_INDEX = 0;
	if (SPEED == MAX_SPEED)
	{
		SIZE_SNAKE = 6;
		SPEED = 1;
	}
	else
	{
		SPEED++;
	}
	GenerateFood();
}

void Eat()
{
	snake[SIZE_SNAKE] = food;
	if (SIZE_SNAKE < 10)
		SIZE_SNAKE++;
	if (FOOD_INDEX < 3)
	{
		FOOD_INDEX++;
		GenerateFood();
	}
	else
	{
		if (PORTAL == 0)
		{
			GeneratePortal();
			DrawPortal('X');
			food = { 0,0 };
		}
	}
}

POINT Portal()
{
	int x, y;
	srand(unsigned int(time(NULL)));
	do {
		x = rand() % (WIDTH_CONSOLE - 3) + 1 + TOPX;
		y = rand() % (HEIGHT_CONSOLE - 5) + 5;
	} while (x == portal[0].x || x == portal[0].x + 1 || x == portal[0].x + 2);
	return { x,y };
}

void DeletePortal()
{
	PORTAL = 0;
	CHECKPORTAL = 0;
	for (int i = 0;i < 5;i++)
	{
		GotoXY(portal[i].x, portal[i].y);
		cout << " ";
		portal[i] = { -1,-1 };
	}
	LevelUp();
}

void DrawSnake(string str, int headcolor)
{
	unsigned int idx = 0;
	for (int i = 0;i < SIZE_SNAKE - 1;i++)
	{
		if (idx >= str.size())
			idx = 0;
		GotoXY(snake[i].x, snake[i].y);
		cout << str[idx];
		idx++;
	}
	GotoXY(snake[SIZE_SNAKE - 1].x,snake[SIZE_SNAKE - 1].y);
	setColor(headcolor, 0);
	if (idx >= str.size())
		idx = 0;
	cout << str[idx];
	setColor(7, 0);
}

void DrawFood(char str)
{
	GotoXY(food.x, food.y);
	cout << str;
}

void ProcessDead()
{
	STATE = 0;
	KEYPRESS = 0;
	for (int i = 0;i < 10;i++)
	{
		DrawSnake(USERID, 14);
		Sleep(50);
		DrawSnake(USERID, 12);
		Sleep(50);
		DrawSnake(USERID, 9);
	}
	setColor(7, 0);
	deadBoard();
	KEYPRESS = 1;
}

bool SelfEat(int x, int y)
{
	for (int i = 0;i < SIZE_SNAKE - 1;i++)
	{
		if (x == snake[i].x&&y == snake[i].y)
			return true;
	}
	return false;
}

void MoveRight()
{
	if (snake[SIZE_SNAKE - 1].x + 1 == TOPX + WIDTH_CONSOLE || SelfEat(snake[SIZE_SNAKE - 1].x + 1,snake[SIZE_SNAKE - 1].y)
		|| isPortal(snake[SIZE_SNAKE - 1].x + 1, snake[SIZE_SNAKE - 1].y))
	{
		ProcessDead();
	}
	else
	{
		if (CHECKPORTAL && snakeatGate() == -1)
			DeletePortal();
		if (snake[SIZE_SNAKE - 1].x + 1 == food.x&&snake[SIZE_SNAKE - 1].y == food.y)
		{
			Eat();
		}
		for (int i = 0;i < SIZE_SNAKE - 1;i++)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x++;
		KEYPRESS = 1;
	}
}

void MoveLeft()
{
	if (snake[SIZE_SNAKE - 1].x - 1 == TOPX || SelfEat(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y)
		|| isPortal(snake[SIZE_SNAKE - 1].x - 1, snake[SIZE_SNAKE - 1].y))
		{
			ProcessDead();
		}
	else
	{
		if (CHECKPORTAL && snakeatGate() == -1)
			DeletePortal();
		if (snake[SIZE_SNAKE - 1].x - 1 == food.x&&snake[SIZE_SNAKE - 1].y == food.y)
		{
			Eat();
		}
		for (int i = 0;i < SIZE_SNAKE - 1;i++)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x--;
		KEYPRESS = 1;
	}
}

void MoveDown()
{
	if (snake[SIZE_SNAKE - 1].y + 1 == HEIGHT_CONSOLE || SelfEat(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1)
		|| isPortal(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y + 1))
	{
		ProcessDead();
	}
	else
	{
		if (CHECKPORTAL && snakeatGate() == -1)
			DeletePortal();
		if (snake[SIZE_SNAKE - 1].x == food.x&&snake[SIZE_SNAKE - 1].y + 1 == food.y)
			Eat();
		for (int i = 0;i < SIZE_SNAKE - 1;i++)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y++;
		KEYPRESS = 1;
	}
}

void MoveUp()
{
	if (snake[SIZE_SNAKE - 1].y - 1 == 0 || SelfEat(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y - 1)
		|| isPortal(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y - 1))
	{
		int tmp = snakeatGate();
		if (tmp != -1)
		{
			CHECKPORTAL = 1;
			for (int i = 0;i<tmp;i++)
				snake[i] = snake[i+1];
			snake[tmp] = Portal();
		}
		else
			ProcessDead();
	}
	else
	{
		if (CHECKPORTAL && snakeatGate() == -1)
			DeletePortal();
		if (snake[SIZE_SNAKE - 1].x == food.x&&snake[SIZE_SNAKE - 1].y - 1 == food.y)
			Eat();
		for (int i = 0;i < SIZE_SNAKE - 1;i++)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y--;
		KEYPRESS = 1;
	}
}

void ThreadFunc()
{
	while (1)
	{
		if (STATE == 1)
		{
			DrawFood(' ');
			DrawSnake(" ", 7);
			switch (MOVING)
			{
			case 'A':
				MoveLeft();
				break;
			case 'D':
				MoveRight();
				break;
			case 'W':
				MoveUp();
				break;
			case 'S':
				MoveDown();
				break;
			}
			if (!PORTAL)
				DrawFood('0');
			DrawSnake(USERID, 4);
			Sleep(400 / SPEED);
		}
	}
}

