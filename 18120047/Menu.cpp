#include "pch.h"
#include <iostream>
#include <fstream>
#include <istream>
#include "windows.h"
#include <thread>
#include <ctime>
#include <string>
#include "conio.h"

#include "Graphics.h"
#include "Variables.h"
#include "Game.h"
using namespace std;

string USERID;
bool KEYPRESS;


bool isValidName(string str)
{
	for (int i = 0; i < str.size();i++)
		if (int(str[i])<48||int(str[i])>57)
			return false;
	return true;
}

void StartScreen()
{
	while (1)
	{
		clearScreen();
		GotoXY(40, 13);
		setColor(14, 5);
		cout << "Type your ID number:";
		setColor(7, 0);
		cout << " ";
		cin >> USERID;
		if (!isValidName(USERID))
			notifyBoard(45, 10, "USE ONLY NUMBERS!");
		else
			break;
	}
	
}

void gameScreen()
{
	clearScreen();
	if (PORTAL)
		DrawPortal('X');
	DrawBoard(TOPX, 0, WIDTH_CONSOLE, HEIGHT_CONSOLE);
	DrawSnake(USERID, 4);
	HideCursor();
}

void StartGame()
{
	clearScreen();
	ResetData();
	DrawBoard(TOPX, 0, WIDTH_CONSOLE, HEIGHT_CONSOLE);
	PORTAL = 0;
	CHECKPORTAL = 0;
	HideCursor();
	STATE = 1;
}

void ExitGame(HANDLE t)
{
	clearScreen();
	TerminateThread(t, 0);
	exit(0);
}

void PauseGame(HANDLE t)
{
	SuspendThread(t);
}

bool LoadGame(int num)
{
	ifstream fs;
	string name;
	while (1)
	{
		GotoXY(40, 13);
		setColor(14, 5);
		ShowCursor();
		cout << "Input filename to load game: ";
		setColor(7, 0);
		cout << " ";

		cin >> name;
		fs.open(name);
		if (!fs.is_open())
		{
			HideCursor();
			notifyBoard(45, 10, "PRESS TO TRY AGAIN");
			char ch = _getch();
			if (ch == 27)
			{
				if (num == 1)
				{
					drawMenu(45, 15);
					return 0;
				}
				else
					gameScreen();
				return 1;
			}
			else
			{
				drawRect(45, 10, 25, 4, 7, 0);
				GotoXY(40, 13);
				for (int i = 0;i < 50;i++)
					cout << " ";
			}
		}
		else
			break;
	}
	fs >> USERID;
	fs >> STATE;
	fs >> CHAR_LOCK >> MOVING >> SPEED >> FOOD_INDEX >> SIZE_SNAKE;
	WIDTH_CONSOLE = 75, HEIGHT_CONSOLE = 20;
	for (int i = 0;i < SIZE_SNAKE;i++)
	{
		fs >> snake[i].x >> snake[i].y;
	}
	fs >> food.x >> food.y;
	fs >> PORTAL >> CHECKPORTAL >> KEYPRESS;
	for (int i = 0;i < 5;i++)
		fs >> portal[i].x >> portal[i].y;
	fs.close();
	gameScreen();
	return 1;
}

void LoadGameIngame(HANDLE t)
{
	PauseGame(t);
	if (LoadGame(2))
		ResumeThread(t);
	else
		ExitGame(t);
}

void SaveGame(HANDLE t)
{
	ShowCursor();
	PauseGame(t);
	string name;
	GotoXY(40, 13);
	setColor(14, 5);
	cout << "Input filename to save:";
	setColor(7, 0);
	cout << " ";
	cin >> name;
	ofstream fs;
	fs.open(name, ios::trunc);
	fs << USERID << endl;
	fs << STATE << endl;
	fs << CHAR_LOCK << " " << MOVING << " " << SPEED << " " << FOOD_INDEX << " " << SIZE_SNAKE << endl;
	for (int i = 0;i < SIZE_SNAKE;i++)
		fs << snake[i].x << " " << snake[i].y << " ";
	fs << endl << food.x << " " << food.y << endl;
	fs << PORTAL << " " << CHECKPORTAL << " " << KEYPRESS << endl;
	for (int i = 0;i < 5;i++)
		fs << portal[i].x << " " << portal[i].y << " ";
	fs.close();

	HideCursor();
	drawRect(45, 10, 25, 3, 14, 4);
	setColor(14, 4);
	GotoXY(52, 11);
	cout << "GAME SAVED!";
	setColor(7, 0);
	Sleep(1000);
}

int Game()
{
	int temp;
	thread t1(ThreadFunc);
	HANDLE handle_t1 = t1.native_handle();
	KEYPRESS = 1;
	while (1)
	{
		temp = toupper(_getch());
		if (STATE == 1)
		{
			if (temp == 'P')
			{
				PauseGame(handle_t1);
			}
			else if (temp == 'L')
			{
				SaveGame(handle_t1);
				ExitGame(handle_t1);
				return 0;
			}
			else if (temp == 'T')
			{
				LoadGameIngame(handle_t1);
			}
			else if (temp == 27)
			{
				PauseGame(handle_t1);
				ExitGame(handle_t1);
				return 0;
			}
			else
			{
				ResumeThread(handle_t1);
				if (KEYPRESS)
				{
					if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S'))
					{
						KEYPRESS = 0;
						if (temp == 'D') CHAR_LOCK = 'A';
						else if (temp == 'W') CHAR_LOCK = 'S';
						else if (temp == 'S') CHAR_LOCK = 'W';
						else CHAR_LOCK = 'D';
						MOVING = temp;
					}
				}
			}
		}
		else
		{
			if (KEYPRESS)
			{
				if (temp == 'Y')
				{
					StartGame();
				}
				else if (temp=='N')
				{
					ExitGame(handle_t1);
					return 0;
				}
			}
		}
	}
}

void NewGame()
{
	StartScreen();
	StartGame();
	Game();
}

void LoadGameMenu()
{
	LoadGame(1);
	Game();
}
