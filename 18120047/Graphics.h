#pragma once
#include <iostream>
#include <string>
using namespace std;

void clearScreen();
void GotoXY(int x, int y);
void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0);
void FixConsoleWindow();
void HideCursor();
void ShowCursor();
void drawRect(int x, int y, int width, int height, int color, int bgColor);
void setColor(int color, int bgColor);
void drawMenu(int x, int y);
void notifyBoard(int x, int y, string noti);
void deadBoard();