#include "headers\interface.h"
#include <Windows.h>

Interface::Interface(){
}

void Interface::setCursosrPos(int cursorX, int cursorY){

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = cursorX;
	pos.Y = cursorY;
	SetConsoleCursorPosition(hConsole, pos);
}
