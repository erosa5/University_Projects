#pragma once

#include <stdio.h>
#include <stdlib.h>

using namespace System;
using namespace System::Text;

void printC(int x, int y, char str[150], ConsoleColor cor) {
	Console::ForegroundColor = cor;
	for (int i = 0; str[i] != '\0'; i++) {
		if ((x + i) < Console::BufferWidth && (x + i) >= 0 && y >= 0 && y<Console::BufferHeight) {
			Console::SetCursorPosition(x + i, y);
			printf("%c", str[i]);
		}
	}
}

void printC(int x, int y, char str, ConsoleColor cor) {
	Console::ForegroundColor = cor;
	Console::SetCursorPosition(x, y);
	printf("%c", str);
}

void printC(int x, int y, char str[150], int str2, ConsoleColor cor) {
	Console::ForegroundColor = cor;
	Console::SetCursorPosition(x, y);
	printf("%s%d", str, str2);
}

void printC(int x, int y, int d, ConsoleColor cor, bool Int) {
	Console::ForegroundColor = cor;
	Console::SetCursorPosition(x, y);
	printf("%d", d);
}