#include "pch.h"
#include <iostream>
#include <string>
#include "windows.h"
#include <conio.h>
#include "Variables.h"
#include "Menu.h"
using namespace std;



/*
	XX --->BackgroundText
	0 black
	1 blue
	2 green
	3 aqua
	4 red
	5 purple
	6 yellow
	7 white
	8 gray
	9 light blue
	10 light green
	11 light aqua
	12 light red
	13 light purple
	14 light yellow
	15 bright white
	colorattribute = foreground + background * 16

*/

//hide cursor
void HideCursor()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

//showcursor
void ShowCursor()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = TRUE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

void clearScreen()
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

void FixConsoleWindow()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);

	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hstdout, &csbi);
	csbi.dwSize.X = csbi.dwMaximumWindowSize.X;
	csbi.dwSize.Y = csbi.dwMaximumWindowSize.Y - 14;
	SetConsoleScreenBufferSize(hstdout, csbi.dwSize);

	// an thanh scrollbar
	ShowScrollBar(consoleWindow, SB_BOTH, FALSE);
}

void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0)
{
	GotoXY(x, y); cout << "X";
	for (int i = 1;i < width;i++)
		cout << "X";
	cout << "X";
	GotoXY(x, height + y);
	cout << "X";
	for (int i = 1;i < width;i++)
		cout << "X";
	cout << "X";
	for (int i = y + 1;i < height + y;i++)
	{
		GotoXY(x, i);
		cout << "X";
		GotoXY(x + width, i);
		cout << "X";
	}
	GotoXY(curPosX, curPosY);
}

void setColor(int color, int bgColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgColor << 4) | color);
}

void drawRect(int x, int y, int width, int height, int color, int bgColor)
{
	setColor(color, bgColor);
	for (int j = 0; j < height; j++)
	{
		GotoXY(x, y + j);
		for (int i = 0; i < width;i++)
			cout << " ";
	}
	setColor(7, 0);
}

void doAction(int num)
{
	switch (num)
	{
	case 0:
		NewGame();
		break;
	case 1:
		LoadGameMenu();
		break;
	case 2:
		GotoXY(25, 22);
		exit(0);
		break;
	default:	
		break;
	}
}

void drawTitle(int x, int y)
{
	
	//S
	drawRect(x, 3, 6, 1, 0, 10);
	drawRect(x, 4, 2, 2, 0, 10);
	drawRect(x, 5, 6, 1, 0, 10);
	drawRect(x+4, 6, 2, 2, 0, 10);
	drawRect(x, 7, 6, 1, 0, 10);

	//N
	drawRect(x+8, 3, 2, 5, 0, 10);
	drawRect(x+14, 3, 2, 5, 0, 10);
	setColor(0, 10);
	GotoXY(x+10, 4); cout << " ";
	GotoXY(x+11, 5); cout << " ";
	GotoXY(x+12, 6); cout << " ";
	GotoXY(x+13, 7); cout << " ";
	
	//A
	drawRect(x+18, 3, 2, 5, 0, 10);
	drawRect(x+24, 3, 2, 5, 0, 10);
	drawRect(x+18, 3, 6, 1, 0, 10);
	drawRect(x+18, 5, 6, 1, 0, 10);

	//K
	drawRect(x+28, 3, 2, 5, 0, 10);
	setColor(0, 10);
	GotoXY(x+30, 5); cout << "  ";
	GotoXY(x + 32, 4); cout << "  ";
	GotoXY(x + 34, 3); cout << "  ";
	GotoXY(x + 32, 6); cout << "  ";
	GotoXY(x + 34, 7); cout << "  ";

	//A
	drawRect(x + 38, 3, 2, 5, 0, 10);
	drawRect(x + 38, 3, 6, 1, 0, 10);
	drawRect(x + 38, 5, 6, 1, 0, 10);
	drawRect(x + 38, 7, 6, 1, 0, 10);
}

void drawMenu(int x, int y)
{
	clearScreen();
	HideCursor();
	drawTitle(35,0);
	int list = 3;
	int current = 0;
	string feature[3];
	feature[0] = "PLAY GAME";
	feature[1] = "LOAD GAME";
	feature[2] = "EXIT GAME";
	drawRect(x, y, 25, list, 14, 3);
	for (int i = 0;i < list;i++)
	{
		if (i == 0)
		{
			drawRect(x, y, 25, 1, 14, 1);
			setColor(14, 1);
		}
		else
			setColor(14, 3);
		GotoXY(x + 8, y + i);
		cout << feature[i];
	}
	GotoXY(x, y);
	setColor(7, 0);
	// up = 72 down = 80 esc 27
	char ch = _getch();
	while (1)
	{
		if (ch == 'T')
		{
			doAction(1);
			break;
		}
		// move up
		if (ch == 72 && current > 0)
		{
			drawRect(x, y + current, 25, 1, 14, 3);
			GotoXY(x + 8, y + current);
			setColor(14, 3);
			cout << feature[current];

			current--;
			drawRect(x, y+current, 25, 1, 14, 1);
			GotoXY(x+8, y + current);
			setColor(14, 1);
			cout << feature[current];
			GotoXY(x, y + current);
			setColor(7, 0);
		}
		if (ch == 80 && current < list - 1)
		{
			drawRect(x, y + current, 25, 1, 14, 3);
			GotoXY(x + 8, y + current);
			setColor(14, 3);
			cout << feature[current];

			current++;
			drawRect(x, y + current, 25, 1, 14, 1);
			GotoXY(x + 8, y + current);
			setColor(14, 1);
			cout << feature[current];
			GotoXY(x, y + current);
			setColor(7, 0);
		}
		if (ch == 13)
		{
			doAction(current);
			break;
		}
		ch = _getch();
	}
}

void notifyBoard(int x, int y, string noti)
{
	drawRect(x, y, 25, 4, 14, 4);
	setColor(14, 4);
	GotoXY(x + 6, y + 1);
	cout << "INVALID INPUT";
	GotoXY(x + 4, y + 2);
	cout << noti;
	setColor(7, 0);
	Sleep(1000);
}

void deadBoard()
{
	int x = 45, y = 10;

	drawRect(x, y, 24, 4, 6, 12);
	setColor(14, 12);
	GotoXY(x + 6, y + 1);
	cout << "YOU ARE DEAD!";
	GotoXY(x + 7, y + 2);
	cout << "TRY AGAIN?";

	drawRect(x, y + 4, 12, 1, 14, 4);
	setColor(14, 4);
	GotoXY(x+4, y + 4);
	cout << "[Y]ES";
	drawRect(x+12, y + 4, 12, 1, 14, 4);
	GotoXY(x+16, y + 4);
	setColor(14, 4);
	cout << "[N]O";

	setColor(7, 0);
}