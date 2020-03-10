#pragma once
#include <iostream>
#include "windows.h"
#include <string>
using namespace std;

extern POINT snake[10];
extern POINT food;
extern POINT portal[5];
extern int CHAR_LOCK;
extern int MOVING;
extern int SPEED;
extern int HEIGHT_CONSOLE, WIDTH_CONSOLE;
extern int FOOD_INDEX;
extern int SIZE_SNAKE;
extern int STATE;
extern string USERID;
extern int PORTAL;
extern int CHECKPORTAL;
extern int TOPX;
extern bool KEYPRESS;