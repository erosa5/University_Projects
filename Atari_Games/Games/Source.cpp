#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace System;
using namespace System::Text;
using namespace Threading;

#pragma region definitions
#define WW 50 // Windows Width
#define WH 34 // Windows Height
#define GW 24 // Game Width
#define GH 28 // Game Height
#pragma endregion

int main() {

#pragma region variables
	/* OVERALL VARIABLES */
	int x[2], y[2], i, n, option; // Main Integers
	int fps = 60, frame = 0, Score = 0, Color, prevState; // Gameplay Variables
	float gameSpeed = 0.5f, x1 = 0.0f, y1 = 0.0f, defaultSpeed; // GameSp eed
	char str[50]; // Multi-use string

				  /* TETRIS VARIABLES */
	bool Block[GW + 2][GH], FullLines[GH], Collided; // Game Matrix
	int Shape, Rotation[2], Blocks, bColor[GW + 2][GH]; // Pieces Variables
	int PieceX[4][2], PieceY[4][2]; // Blocks Position
	int bColX[4] = { 0,0,0,0 }, bColY[4] = { 0,0,0,0 }; // Bottom Collision
	int rColX[4] = { 0,0,0,0 }, rColY[4] = { 0,0,0,0 }; // Right Collision
	int lColX[4] = { 0,0,0,0 }, lColY[4] = { 0,0,0,0 }; // Left Collision
	int rotColX[3] = { 0,0,0 }, rotColY[3] = { 0,0,0 }; // Rotation Collision
	int nextShape, nextRotation, nextColor; // Preview Next Piece
	for (i = 0; i < GH; i++) { FullLines[i] = false; }

	/* SNAKE VARIABLES */
	bool SnakeSegments[GW*GH], Able = true, AbleMove[2] = { false,false }, randColors = false;
	int SnakeSegmentsX[GW*GH][2], SnakeSegmentsY[GW*GH][2], SnakeSegColor[GW*GH], SnakeSegDir[GW*GH][2], SDir = 0, Segments = 3, DotX, DotY;

	/* FORMULA 1 VARIABLES */
	int F1X[9] = { 0,0,0,0,0,0,0,0,0 }, F1Y[9] = { 0,0,0,0,0,0,0,0,0 }; // Car Coordinates
	int wallX[2] = { 0,0 }, hVar = 0, aVar = 0, roadMargin = 2;
	const int maxFoes = 15;
	int foeX[maxFoes][2], foeY[maxFoes][2], xF, yF;
	float fY[maxFoes], tVar = 0.0f;
	bool foe[maxFoes], F1Begin = false, Crashed = false;
	int foesOn = 0, carColor[maxFoes];

	/* MENU INDEX */
	int STATE = 1;
	const int MAIN = 1;
	const int GAMEOVER = 2;
	const int WIN = 3;
	const int PAUSE = 4;
	const int TETRIS = 5;
	const int SNAKE = 6;
	const int OTHERS = 7;
	const int F1 = 8;
	const int ARKANOID = 9;
#pragma endregion 
#pragma region windows_settings
	/* GENERAL WINDOWS SETTINGS */
	Console::SetWindowSize(WW + 2, WH); // Windows Size
	Console::SetBufferSize(WW + 2, WH); // Buffer Size
	Console::CursorVisible = false; // Hide Cursor
	Console::BackgroundColor = ConsoleColor::Black; // Background Color
	srand(time(NULL));

	/* WINDOW BORDER */
Redraw:
	Console::Clear();
	Console::ForegroundColor = ConsoleColor::DarkGray; // Foreground Color
	for (x[0] = 0, y[0] = -1; y[0] < GH + 1; x[0]++) {
		if (y[0] == -1) {
			Console::SetCursorPosition(x[0] + ((WW - GW) / 2) - 1, y[0] + ((WH - GH) / 2));
			printf("%c", x[0] == 0 ? 201 : x[0] == GW + 3 ? 187 : 205);
		}
		else if (y[0] == GH) {
			Console::SetCursorPosition(x[0] + ((WW - GW) / 2) - 1, y[0] + ((WH - GH) / 2));
			printf("%c", x[0] == 0 ? 200 : x[0] == GW + 3 ? 188 : 205);
		}
		else if (x[0] == 0 || x[0] == GW + 3) {
			Console::SetCursorPosition(x[0] + ((WW - GW) / 2) - 1, y[0] + ((WH - GH) / 2));
			printf("%c", 186);
		}
		Block[x[0]][y[0]] = 1;
		if (x[0] > GW + 2) { y[0]++; x[0] = -1; }
	}
	option = 0;
	Score = 0;
	if (STATE == TETRIS) {
		Console::SetCursorPosition(WW / 2 - 3, (WH - GH) / 2 - 1);
		printf(" TETRIS ");
		Console::SetCursorPosition((WW - GW) / 2 + GW + 2, (WH - GH) / 2 - 1);
		printf("%c%c%c%c%c%c%c%c%c", 203, 205, 205, 205, 205, 205, 205, 205, 187);
		for (i = 0; i < 12; i++) {
			if (i == 11) {
				Console::SetCursorPosition((WW - GW) / 2 + GW + 2, (WH - GH) / 2 + i);
				printf("%c%c%c%c%c%c%c%c%c", 204, 205, 205, 205, 205, 205, 205, 205, 188);
			}
			else {
				Console::SetCursorPosition((WW - GW) / 2 + GW + 10, (WH - GH) / 2 + i);
				printf("%c", 186);
			}
		}
		Console::SetCursorPosition((WW - GW) / 2 + GW + 3, (WH - GH) / 2); printf(" Next");
		Console::SetCursorPosition((WW - GW) / 2 + GW + 3, (WH - GH) / 2 + 7); printf(" Score");
		Console::ForegroundColor = ConsoleColor::DarkGray;
		Console::SetCursorPosition((WW - GW) / 2 + GW + 4, (WH - GH) / 2 + 9); printf("00000");
		sprintf_s(str, "%d", Score*(GW + 2));
		for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
		Console::SetCursorPosition((WW - GW) / 2 + GW + 9 - i, (WH - GH) / 2 + 9); printf(str);
		x[0] = 0; y[0] = 0;
		for (i = 0; i < (GW + 2)*GH; i++) {
			Block[i % (GW + 2)][i / (GW + 2)] = false;
		}
		nextShape = rand() % 7;
		nextRotation = rand() % 4;
		nextColor = rand() % 15;
		Shape = rand() % 7;
		Rotation[1] = rand() % 4;
		if (randColors) {
			Color = rand() % 15; // Randomizing new dot
		}
		else {
			Color = Color++;
			if (Color > 15) { Color = 0; }
		}
		goto tetrisPieceGenerator2;
	}
	else if (STATE == SNAKE) {
		Color = rand() % 15;
		for (i = 0; i < GW*GH; i++) {
			SnakeSegments[i] = false;
			SnakeSegmentsX[i][0] = 0;
			SnakeSegmentsY[i][0] = 0;
			SnakeSegDir[i][0] = 0;
		}
		Console::SetCursorPosition(WW / 2 - 3, (WH - GH) / 2 - 1);
		printf(" SNAKE ");
		Console::SetCursorPosition((WW - GW) / 2 + GW + 2, (WH - GH) / 2 - 1);
		printf("%c%c%c%c%c%c%c%c%c", 203, 205, 205, 205, 205, 205, 205, 205, 187);
		for (i = 0; i < 4; i++) {
			if (i == 3) {
				Console::SetCursorPosition((WW - GW) / 2 + GW + 2, (WH - GH) / 2 + i);
				printf("%c%c%c%c%c%c%c%c%c", 204, 205, 205, 205, 205, 205, 205, 205, 188);
			}
			else {
				Console::SetCursorPosition((WW - GW) / 2 + GW + 10, (WH - GH) / 2 + i);
				printf("%c", 186);
			}
		}
		Console::SetCursorPosition((WW - GW) / 2 + GW + 3, (WH - GH) / 2); printf(" Score");
		Console::ForegroundColor = ConsoleColor::White; // Foreground Color
		for (i = 0; i < Segments; i++) {
			SnakeSegments[i] = true;
			if (randColors) {
				SnakeSegColor[i] = rand() % 15;
			}
			else {
				SnakeSegColor[i] = Color++;
				if (Color > 15) { Color = 0; }
			}
		}
		DotX = rand() % GW;
		DotY = rand() % GH;
		for (i = 0; i < GW*GH; i++) {
			while (DotX == SnakeSegmentsX[i][0] && DotY == SnakeSegmentsY[i][0]) {
				DotX = rand() % GW;
				DotY = rand() % GH;
			}
		}
		SnakeSegColor[0] = 9;
		SDir = 0;
		x1 = 0;
		y1 = 0;
		SnakeSegmentsY[0][0] = 0;
		Console::ForegroundColor = ConsoleColor::White; // White Printing
		sprintf_s(str, "%d", Segments * 10 - 30); // Writting string
		for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
		Console::SetCursorPosition((WW - GW) / 2 + GW + 4, (WH - GH) / 2 + 2); printf("     "); // Cleaning Score
		Console::SetCursorPosition((WW - GW) / 2 + GW + 8 - i, (WH - GH) / 2 + 2); printf(str); // Printing new score
		if (randColors) {
			Color = rand() % 15;
		}
		else {
			Color = Color++;
			if (Color > 15) { Color = 0; }
		}
	}
	else if (STATE == F1) {
		Console::SetCursorPosition(WW / 2 - 5, (WH - GH) / 2 - 1);
		printf(" FORMULA 1 ");
		Console::SetCursorPosition((WW - GW) / 2 + GW + 2, (WH - GH) / 2 - 1);
		printf("%c%c%c%c%c%c%c%c%c", 203, 205, 205, 205, 205, 205, 205, 205, 187);
		for (i = 0; i < 4; i++) {
			if (i == 3) {
				Console::SetCursorPosition((WW - GW) / 2 + GW + 2, (WH - GH) / 2 + i);
				printf("%c%c%c%c%c%c%c%c%c", 204, 205, 205, 205, 205, 205, 205, 205, 188);
			}
			else {
				Console::SetCursorPosition((WW - GW) / 2 + GW + 10, (WH - GH) / 2 + i);
				printf("%c", 186);
			}
		}
		Console::SetCursorPosition((WW - GW) / 2 + GW + 3, (WH - GH) / 2); printf(" Score");
		Console::ForegroundColor = ConsoleColor::White; // White Printing
		sprintf_s(str, "%d", Segments * 10 - 30); // Writting string
		for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
		Console::SetCursorPosition((WW - GW) / 2 + GW + 4, (WH - GH) / 2 + 2); printf("     "); // Cleaning Score
		Console::SetCursorPosition((WW - GW) / 2 + GW + 8 - i, (WH - GH) / 2 + 2); printf(str); // Printing new score
		F1X[0] = WW / 2 - 1;
		F1Y[0] = GH - 1;
		wallX[0] = (WW - GW) / 2 + 1 + roadMargin;
		wallX[1] = (WW - GW) / 2 + GW - roadMargin;
		F1Begin = true;
		Crashed = false;
		for (i = 0; i < maxFoes; i++) {
			fY[i] = 0;
			foe[i] = false;
			foeX[i][0] = 0;
			foeY[i][0] = 0;
			foeX[i][1] = 0;
			foeY[i][1] = 0;
		}
		Console::ForegroundColor = ConsoleColor::DarkGray; // White Printing
		for (i = 0; i < (GW + 2)*GH; i++) {
			if (i % (GW + 2) <= roadMargin || i % (GW + 2) > GW - roadMargin) {
				Console::SetCursorPosition((WW - GW) / 2 + i % (GW + 2), (WH - GH) / 2 + i / (GW + 2));
				printf("/");
			}
		}
	}
	else if (STATE == ARKANOID) {
		Console::SetCursorPosition(WW / 2 - 4, (WH - GH) / 2 - 1);
		printf(" ARKANOID ");
		Console::SetCursorPosition((WW - GW) / 2 + GW + 2, (WH - GH) / 2 - 1);
		printf("%c%c%c%c%c%c%c%c%c", 203, 205, 205, 205, 205, 205, 205, 205, 187);
		for (i = 0; i < 4; i++) {
			if (i == 3) {
				Console::SetCursorPosition((WW - GW) / 2 + GW + 2, (WH - GH) / 2 + i);
				printf("%c%c%c%c%c%c%c%c%c", 204, 205, 205, 205, 205, 205, 205, 205, 188);
			}
			else {
				Console::SetCursorPosition((WW - GW) / 2 + GW + 10, (WH - GH) / 2 + i);
				printf("%c", 186);
			}
		}
		Console::SetCursorPosition((WW - GW) / 2 + GW + 3, (WH - GH) / 2); printf(" Score");
		Console::ForegroundColor = ConsoleColor::White; // White Printing
		sprintf_s(str, "%d", Score); // Writting string
		for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
		Console::SetCursorPosition((WW - GW) / 2 + GW + 4, (WH - GH) / 2 + 2); printf("     "); // Cleaning Score
		Console::SetCursorPosition((WW - GW) / 2 + GW + 8 - i, (WH - GH) / 2 + 2); printf(str); // Printing new score
	}
	Score = 0;
	defaultSpeed = gameSpeed;

#pragma endregion 
#pragma region loop_start
	do {
		switch (STATE) {
#pragma endregion
#pragma region menu_main
		case MAIN: // MAIN MENU
				   /* TETRIS WRITTINGS */
			Console::ForegroundColor = ConsoleColor::White;
			Console::SetCursorPosition(WW / 2 - 9, WH / 5);     printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 218, 196, 196, 194, 196, 196, 191, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32);
			Console::SetCursorPosition(WW / 2 - 9, WH / 5 + 1); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 192, 217, 32, 179, 32, 192, 217, 218, 196, 194, 196, 191, 32, 32, 32, 218, 196, 196, 196, 191);
			Console::SetCursorPosition(WW / 2 - 9, WH / 5 + 2); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 32, 32, 32, 179, 32, 218, 196, 196, 191, 179, 218, 196, 196, 191, 111, 192, 196, 196, 196, 191);
			Console::SetCursorPosition(WW / 2 - 9, WH / 5 + 3); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 32, 32, 32, 179, 32, 195, 196, 196, 217, 179, 195, 196, 194, 217, 179, 218, 191, 32, 32, 179);
			Console::SetCursorPosition(WW / 2 - 9, WH / 5 + 4); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 32, 32, 32, 193, 32, 192, 196, 196, 217, 193, 192, 32, 192, 217, 192, 192, 196, 196, 196, 217);

			/* PLAY OPTION */
			if (option == 0) {
				if (frame % fps >= 0 && frame % fps < fps / 2) { Console::ForegroundColor = ConsoleColor::Gray; }
				else { Console::ForegroundColor = ConsoleColor::Black; }
			}
			else { Console::ForegroundColor = ConsoleColor::Gray; }
			sprintf_s(str, "PLAY"); // Writting string
			for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
			if (i % 2 == 1) { i--; } // Breaking uneven count
			Console::SetCursorPosition(WW / 2 - (i / 2) + 1, WH / 1.4); printf(str); // Centering and positioning text
			if (option == 0) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, (WH / 1.4)); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, (WH / 1.4)); printf("%c", 175);
			}
			/* OTHER GAMES OPTION */
			if (option == 1) {
				if (frame % fps >= 0 && frame % fps < fps / 2) { Console::ForegroundColor = ConsoleColor::Gray; }
				else { Console::ForegroundColor = ConsoleColor::Black; }
			}
			else { Console::ForegroundColor = ConsoleColor::Gray; }
			sprintf_s(str, "All games"); // Writting string
			for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
			if (i % 2 == 1) { i--; } // Breaking uneven count
			Console::SetCursorPosition(WW / 2 - (i / 2) + 1, WH / 1.4 + 2); printf(str); // Centering and positioning text
			if (option == 1) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, (WH / 1.4) + 2); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, (WH / 1.4) + 2); printf("%c", 175);
			}
			/* EXIT OPTION */
			if (option == 2) {
				if (frame % fps >= 0 && frame % fps < fps / 2) { Console::ForegroundColor = ConsoleColor::Gray; }
				else { Console::ForegroundColor = ConsoleColor::Black; }
			}
			else { Console::ForegroundColor = ConsoleColor::Gray; }
			sprintf_s(str, "EXIT"); // Writting string
			for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
			if (i % 2 == 1) { i--; } // Breaking uneven count
			Console::SetCursorPosition(WW / 2 - (i / 2) + 1, WH / 1.3 + 3); printf(str); // Centering and positioning text
			if (option == 2) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, (WH / 1.3) + 3); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, (WH / 1.3) + 3); printf("%c", 175);
			}

			/* GETTING INPUT */
			if (Console::KeyAvailable) {
				ConsoleKeyInfo tecla;
				tecla = Console::ReadKey(true);
				if (tecla.Key == ConsoleKey::Enter) {
					Console::Beep(300, 100);
					if (option == 0) {
						STATE = TETRIS; goto Redraw;
					}
					else if (option == 1) {
						STATE = OTHERS; goto Redraw;
					}
					else if (option == 2) {
						goto end;
					}
				}
				else if (tecla.Key == ConsoleKey::DownArrow && option <= 2) {
					option++; Console::Beep(300, 100);
					frame = 0;
					if (option == 3) { option = 0; }
				}
				else if (tecla.Key == ConsoleKey::UpArrow && option >= 0) {
					option--; Console::Beep(300, 100);
					frame = 0;
					if (option == -1) { option = 2; }
				}
			}
			/* FPS DELAY */
			Thread::Sleep(1000 / fps);
			break;
#pragma endregion
#pragma region menu_other_games
		case OTHERS:
			/* TETRIS OPTION */
			if (option == 0) {
				if (frame % fps >= 0 && frame % fps < fps / 2) { Console::ForegroundColor = ConsoleColor::Gray; }
				else { Console::ForegroundColor = ConsoleColor::Black; }
			}
			else { Console::ForegroundColor = ConsoleColor::Gray; }
			Console::SetCursorPosition(WW / 3, WH / 5); printf("TETRIS"); // Centering and positioning text
			if (option == 0) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 3 - 2, (WH / 5)); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 3 - 2, (WH / 5)); printf("%c", 175);
			}
			/* SNAKE OPTION */
			if (option == 1) {
				if (frame % fps >= 0 && frame % fps < fps / 2) { Console::ForegroundColor = ConsoleColor::Gray; }
				else { Console::ForegroundColor = ConsoleColor::Black; }
			}
			else { Console::ForegroundColor = ConsoleColor::Gray; }
			Console::SetCursorPosition(WW / 3, WH / 5 + 2); printf("SNAKE"); // Centering and positioning text
			if (option == 1) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 3 - 2, (WH / 5) + 2); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 3 - 2, (WH / 5) + 2); printf("%c", 175);
			}
			/* FORMULA 1 */
			if (option == 2) {
				if (frame % fps >= 0 && frame % fps < fps / 2) { Console::ForegroundColor = ConsoleColor::Gray; }
				else { Console::ForegroundColor = ConsoleColor::Black; }
			}
			else { Console::ForegroundColor = ConsoleColor::Gray; }
			Console::SetCursorPosition(WW / 3, WH / 5 + 4); printf("FORMULA 1"); // Centering and positioning text
			if (option == 2) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 3 - 2, (WH / 5) + 4); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 3 - 2, (WH / 5) + 4); printf("%c", 175);
			}
			/* ARKANOID */
			if (option == 3) {
				if (frame % fps >= 0 && frame % fps < fps / 2) { Console::ForegroundColor = ConsoleColor::Gray; }
				else { Console::ForegroundColor = ConsoleColor::Black; }
			}
			else { Console::ForegroundColor = ConsoleColor::Gray; }
			Console::SetCursorPosition(WW / 3, WH / 5 + 6); printf("ARKANOID"); // Centering and positioning text
			if (option == 3) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 3 - 2, (WH / 5) + 6); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 3 - 2, (WH / 5) + 6); printf("%c", 175);
			}
			/* GAME SPEED */
			Console::ForegroundColor = ConsoleColor::DarkGray;
			sprintf_s(str, "Game Speed: %.2f", gameSpeed); // Writting string
			for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
			if (i % 2 == 1) { i--; } // Breaking uneven count
			Console::SetCursorPosition(WW / 2 - (i / 2) + 1, WH / 1.3 - 2); printf(str); // Centering and positioning text
			if (option == 4) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, WH / 1.3 - 2); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, WH / 1.3 - 2); printf("%c", 175);
			}
			/* RANDOM COLORS */
			Console::ForegroundColor = ConsoleColor::DarkGray;
			sprintf_s(str, "Random Colors: %s", randColors ? "Yes" : "No "); // Writting string
			for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
			if (i % 2 == 1) { i--; } // Breaking uneven count
			Console::SetCursorPosition(WW / 2 - (i / 2) + 1, WH / 1.3 - 1); printf(str); // Centering and positioning text
			if (option == 5) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, WH / 1.3 - 1); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, WH / 1.3 - 1); printf("%c", 175);
			}
			/* SNAKE SEGMENTS */
			Console::ForegroundColor = ConsoleColor::DarkGray;
			sprintf_s(str, "Snake Segments: %2.d", Segments); // Writting string
			for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
			if (i % 2 == 1) { i--; } // Breaking uneven count
			Console::SetCursorPosition(WW / 2 - (i / 2) + 1, WH / 1.3); printf(str); // Centering and positioning text
			if (option == 6) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, WH / 1.3); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, WH / 1.3); printf("%c", 175);
			}
			/* BACK OPTION */
			if (option == 7) {
				if (frame % fps >= 0 && frame % fps < fps / 2) { Console::ForegroundColor = ConsoleColor::Gray; }
				else { Console::ForegroundColor = ConsoleColor::Black; }
			}
			else { Console::ForegroundColor = ConsoleColor::Gray; }
			sprintf_s(str, "Go back!"); // Writting string
			for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
			if (i % 2 == 1) { i--; } // Breaking uneven count
			Console::SetCursorPosition(WW / 2 - (i / 2) + 1, WH / 1.3 + 3); printf(str); // Centering and positioning text
			if (option == 7) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, WH / 1.3 + 3); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, WH / 1.3 + 3); printf("%c", 175);
			}
			/* GETTING INPUT */
			if (Console::KeyAvailable) {
				ConsoleKeyInfo tecla;
				tecla = Console::ReadKey(true);
				if (tecla.Key == ConsoleKey::Enter) {
					Console::Beep(300, 100);
					if (option == 0) {
						STATE = TETRIS; goto Redraw;
					}
					else if (option == 1) {
						STATE = SNAKE; goto Redraw;
					}
					else if (option == 2) {
						STATE = F1; goto Redraw;
					}
					else if (option == 3) {
						STATE = ARKANOID; goto Redraw;
					}
					else if (option == 7) {
						STATE = MAIN; goto Redraw;
					}
				}
				else if (tecla.Key == ConsoleKey::DownArrow && option <= 7) {
					option++; Console::Beep(300, 100);
					frame = 0;
					if (option == 8) { option = 0; }
				}
				else if (tecla.Key == ConsoleKey::UpArrow && option >= 0) {
					option--; Console::Beep(300, 100);
					frame = 0;
					if (option == -1) { option = 7; }
				}
				else if (tecla.Key == ConsoleKey::LeftArrow) {
					if (option == 4 && gameSpeed >= 0.15) {
						gameSpeed -= 0.05;
						defaultSpeed = gameSpeed;
					}
					else if (option == 5) {
						if (randColors == true) { randColors = false; }
						else { randColors = true; }
					}
					else if (option == 6 && Segments > 3) {
						Segments--;
					}
				}
				else if (tecla.Key == ConsoleKey::RightArrow) {
					if (option == 4 && gameSpeed < 1) {
						gameSpeed += 0.05;
						defaultSpeed = gameSpeed;
					}
					else if (option == 5) {
						if (randColors == true) { randColors = false; }
						else { randColors = true; }
					}
					else if (option == 6 && Segments < 50) {
						Segments++;
					}
				}
			}
			/* FPS DELAY */
			Thread::Sleep(1000 / fps);
			break;
#pragma endregion
#pragma region menu_pause
		case PAUSE:
			if (Console::KeyAvailable) {
				ConsoleKeyInfo tecla;
				tecla = Console::ReadKey(true);
				if (tecla.Key == ConsoleKey::Escape) {
					gameSpeed = defaultSpeed;
					STATE = prevState;
					Console::SetCursorPosition((WW - GW) / 2 - 1, GH + (WH - GH) / 2 + 1); printf("                            ");
				}
				else if (tecla.Key == ConsoleKey::Enter) {
					STATE = MAIN;
					gameSpeed = defaultSpeed;
					goto Redraw;
				}
			}
			break;
#pragma endregion
#pragma region menu_gameover
			gameover :
		case GAMEOVER:
			gameSpeed = defaultSpeed;
			Console::ForegroundColor = ConsoleColor::White;
			Console::SetCursorPosition(WW / 2 - 9, WH / 3);     printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", 218, 196, 191, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32);
			Console::SetCursorPosition(WW / 2 - 9, WH / 3 + 1); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", 179, 32, 32, 218, 196, 191, 218, 191, 218, 191, 218, 196, 191);
			Console::SetCursorPosition(WW / 2 - 9, WH / 3 + 2); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", 179, 218, 191, 195, 196, 180, 179, 192, 217, 179, 195, 196, 217);
			Console::SetCursorPosition(WW / 2 - 9, WH / 3 + 3); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", 192, 196, 217, 192, 32, 192, 192, 217, 32, 192, 192, 196, 217);
			Console::SetCursorPosition(WW / 2, WH / 3 + 4); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", 218, 196, 191, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32);
			Console::SetCursorPosition(WW / 2, WH / 3 + 5); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", 179, 32, 179, 191, 32, 218, 218, 196, 191, 218, 196, 196, 191);
			Console::SetCursorPosition(WW / 2, WH / 3 + 6); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", 179, 32, 179, 179, 32, 179, 195, 196, 217, 195, 196, 194, 217);
			Console::SetCursorPosition(WW / 2, WH / 3 + 7); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", 192, 196, 217, 192, 196, 180, 192, 196, 217, 192, 32, 192, 217);
			/* PLAY OPTION */
			if (option == 0) {
				if (frame % fps >= 0 && frame % fps < fps / 2) { Console::ForegroundColor = ConsoleColor::Gray; }
				else { Console::ForegroundColor = ConsoleColor::Black; }
			}
			else { Console::ForegroundColor = ConsoleColor::Gray; }
			sprintf_s(str, "TRY AGAIN"); // Writting string
			for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
			if (i % 2 == 1) { i--; } // Breaking uneven count
			Console::SetCursorPosition(WW / 2 - (i / 2) + 1, WH / 1.4); printf(str); // Centering and positioning text
			if (option == 0) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, (WH / 1.4)); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, (WH / 1.4)); printf("%c", 175);
			}
			/* OTHER GAMES OPTION */
			if (option == 1) {
				if (frame % fps >= 0 && frame % fps < fps / 2) { Console::ForegroundColor = ConsoleColor::Gray; }
				else { Console::ForegroundColor = ConsoleColor::Black; }
			}
			else { Console::ForegroundColor = ConsoleColor::Gray; }
			sprintf_s(str, "Main menu"); // Writting string
			for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
			if (i % 2 == 1) { i--; } // Breaking uneven count
			Console::SetCursorPosition(WW / 2 - (i / 2) + 1, WH / 1.4 + 2); printf(str); // Centering and positioning text
			if (option == 1) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, (WH / 1.4) + 2); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, (WH / 1.4) + 2); printf("%c", 175);
			}
			/* EXIT OPTION */
			if (option == 2) {
				if (frame % fps >= 0 && frame % fps < fps / 2) { Console::ForegroundColor = ConsoleColor::Gray; }
				else { Console::ForegroundColor = ConsoleColor::Black; }
			}
			else { Console::ForegroundColor = ConsoleColor::Gray; }
			sprintf_s(str, "EXIT"); // Writting string
			for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
			if (i % 2 == 1) { i--; } // Breaking uneven count
			Console::SetCursorPosition(WW / 2 - (i / 2) + 1, WH / 1.3 + 3); printf(str); // Centering and positioning text
			if (option == 2) {
				Console::ForegroundColor = ConsoleColor::Gray;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, (WH / 1.3) + 3); printf("%c", 175);
			}
			else {
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - (i / 2) - 1, (WH / 1.3) + 3); printf("%c", 175);
			}
			/* GETTING INPUT */
			if (Console::KeyAvailable) {
				ConsoleKeyInfo tecla;
				tecla = Console::ReadKey(true);
				if (tecla.Key == ConsoleKey::Enter) {
					Console::Beep(300, 100);
					if (option == 0) {
						STATE = prevState;
						Segments = 3;
						goto Redraw;
					}
					else if (option == 1) {
						STATE = MAIN;
						Segments = 3;
						goto Redraw;
					}
					else if (option == 2) {
						goto end;
					}
				}
				else if (tecla.Key == ConsoleKey::DownArrow && option <= 2) {
					option++; Console::Beep(300, 100);
					if (option == 3) { option = 0; }
				}
				else if (tecla.Key == ConsoleKey::UpArrow && option >= 0) {
					option--; Console::Beep(300, 100);
					if (option == -1) { option = 2; }
				}
			}
			/* FPS DELAY */
			Thread::Sleep(1000 / fps);
			frame++;
			break;
#pragma endregion
#pragma region game_snake
		case SNAKE:
			/* PRINTING AND BLINKING DOT */
			if (frame % (fps / 2) >= 0 && frame % (fps / 2) < fps / 4) {
				if (Color == 0) { Console::ForegroundColor = ConsoleColor::White; }
				else if (Color == 1) { Console::ForegroundColor = ConsoleColor::Gray; }
				else if (Color == 2) { Console::ForegroundColor = ConsoleColor::DarkGray; }
				else if (Color == 3) { Console::ForegroundColor = ConsoleColor::Cyan; }
				else if (Color == 4) { Console::ForegroundColor = ConsoleColor::DarkCyan; }
				else if (Color == 5) { Console::ForegroundColor = ConsoleColor::Blue; }
				else if (Color == 6) { Console::ForegroundColor = ConsoleColor::DarkBlue; }
				else if (Color == 7) { Console::ForegroundColor = ConsoleColor::Green; }
				else if (Color == 8) { Console::ForegroundColor = ConsoleColor::DarkGreen; }
				else if (Color == 9) { Console::ForegroundColor = ConsoleColor::Yellow; }
				else if (Color == 10) { Console::ForegroundColor = ConsoleColor::DarkYellow; }
				else if (Color == 11) { Console::ForegroundColor = ConsoleColor::Magenta; }
				else if (Color == 12) { Console::ForegroundColor = ConsoleColor::DarkMagenta; }
				else if (Color == 13) { Console::ForegroundColor = ConsoleColor::Red; }
				else if (Color == 14) { Console::ForegroundColor = ConsoleColor::DarkRed; }
			}
			else { Console::ForegroundColor = ConsoleColor::Black; }
			Console::SetCursorPosition(DotX + (WW - GW) / 2, DotY + (WH - GH) / 2); printf("%c", 4);
			/* GETTING INPUT */
			if (Console::KeyAvailable) {
				ConsoleKeyInfo tecla;
				tecla = Console::ReadKey(true);
				if (tecla.Key == ConsoleKey::RightArrow && SDir != 2 && Able) { SDir = 0; Able = false; }
				else if (tecla.Key == ConsoleKey::UpArrow && SDir != 3 && Able) { SDir = 1; Able = false; }
				else if (tecla.Key == ConsoleKey::LeftArrow && SDir != 0 && Able) { SDir = 2; Able = false; }
				else if (tecla.Key == ConsoleKey::DownArrow && SDir != 1 && Able) { SDir = 3; Able = false; }
				else if (tecla.Key == ConsoleKey::Escape) {
					Console::ForegroundColor = ConsoleColor::DarkGray; // Foreground Color
					Console::SetCursorPosition((WW - GW) / 2 - 1, GH + (WH - GH) / 2 + 1); printf("ESC to resume, ENTER to quit");
					defaultSpeed = gameSpeed;
					gameSpeed = 0;
					prevState = STATE;
					STATE = PAUSE;
				}
			}
			/* CHECKING DIRECTION */
			if (SDir == 0) { x1 += gameSpeed / 1.5; }
			else if (SDir == 1) { y1 -= gameSpeed / 1.5; }
			else if (SDir == 2) { x1 -= gameSpeed / 1.5; }
			else if (SDir == 3) { y1 += gameSpeed / 1.5; }
			/* CONVERTING TO INTEGER */
			x[0] = (int)x1;
			y[0] = (int)y1;
			/* ON MAIN COORDINATE CHANGE */
			if (x[0] != SnakeSegmentsX[0][0] || y[0] != SnakeSegmentsY[0][0]) {
				Able = true;
				for (i = 0; i < GW*GH; i++) {
					if (SnakeSegments[i]) {
						SnakeSegmentsX[i][1] = SnakeSegmentsX[i - 1][0];
						SnakeSegmentsY[i][1] = SnakeSegmentsY[i - 1][0];
						SnakeSegDir[i][1] = SnakeSegDir[i - 1][0];
					}
				}
				for (i = 0; i < GW*GH; i++) {
					if (i < Segments) {
						if (i == 0) {
							if (x[0] < 0 || x[0] > GW + 1 || y[0] < 0 || y[0] > GH - 1) {
								goto snakeGameOver;
							}
							else {
								Console::ForegroundColor = ConsoleColor::White;
								Console::SetCursorPosition(SnakeSegmentsX[i][0] + (WW - GW) / 2, SnakeSegmentsY[i][0] + (WH - GH) / 2); printf(" ");
								SnakeSegmentsX[0][0] = x[0];
								SnakeSegmentsY[0][0] = y[0];
								SnakeSegDir[0][0] = SDir;
								Console::SetCursorPosition(SnakeSegmentsX[i][0] + (WW - GW) / 2, SnakeSegmentsY[i][0] + (WH - GH) / 2); printf("%c", 254);
							}
						}
						else {
							if (SnakeSegmentsX[i][1] == SnakeSegmentsX[0][0] && SnakeSegmentsY[i][1] == SnakeSegmentsY[0][0] && SnakeSegments[i]) {
								goto snakeGameOver;
							}
							else {
								if (SnakeSegColor[i] == 0) { Console::ForegroundColor = ConsoleColor::White; }
								else if (SnakeSegColor[i] == 1) { Console::ForegroundColor = ConsoleColor::Gray; }
								else if (SnakeSegColor[i] == 2) { Console::ForegroundColor = ConsoleColor::DarkGray; }
								else if (SnakeSegColor[i] == 3) { Console::ForegroundColor = ConsoleColor::Cyan; }
								else if (SnakeSegColor[i] == 4) { Console::ForegroundColor = ConsoleColor::DarkCyan; }
								else if (SnakeSegColor[i] == 5) { Console::ForegroundColor = ConsoleColor::Blue; }
								else if (SnakeSegColor[i] == 6) { Console::ForegroundColor = ConsoleColor::DarkBlue; }
								else if (SnakeSegColor[i] == 7) { Console::ForegroundColor = ConsoleColor::Green; }
								else if (SnakeSegColor[i] == 8) { Console::ForegroundColor = ConsoleColor::DarkGreen; }
								else if (SnakeSegColor[i] == 9) { Console::ForegroundColor = ConsoleColor::Yellow; }
								else if (SnakeSegColor[i] == 10) { Console::ForegroundColor = ConsoleColor::DarkYellow; }
								else if (SnakeSegColor[i] == 11) { Console::ForegroundColor = ConsoleColor::Magenta; }
								else if (SnakeSegColor[i] == 12) { Console::ForegroundColor = ConsoleColor::DarkMagenta; }
								else if (SnakeSegColor[i] == 13) { Console::ForegroundColor = ConsoleColor::Red; }
								else if (SnakeSegColor[i] == 14) { Console::ForegroundColor = ConsoleColor::DarkRed; }
								Console::SetCursorPosition(SnakeSegmentsX[i][0] + (WW - GW) / 2, SnakeSegmentsY[i][0] + (WH - GH) / 2); printf(" ");
								SnakeSegmentsX[i][0] = SnakeSegmentsX[i][1];
								SnakeSegmentsY[i][0] = SnakeSegmentsY[i][1];
								SnakeSegDir[i][0] = SnakeSegDir[i][1];
								Console::SetCursorPosition(SnakeSegmentsX[i][0] + (WW - GW) / 2, SnakeSegmentsY[i][0] + (WH - GH) / 2);
								if (i == Segments - 1) {
									printf("%c", SnakeSegDir[i][1] == 0 ? 273 : SnakeSegDir[i][1] == 1 ? 287 : SnakeSegDir[i][1] == 2 ? 272 : 286);
								}
								else {
									printf("%c", 254);
								}
							}
						}
					}
				}
			}
			/* ON EATING THE DOT */
			if (SnakeSegmentsX[0][0] == DotX && SnakeSegmentsY[0][0] == DotY) {
				Segments++; // Increasing snake Segment
				Console::Beep(100 + rand() % 200, 100); // Beep
				SnakeSegColor[Segments] = Color; // Applying new snake color
				Score++; // Increasing score
				if (randColors) {
					Color = rand() % 15; // Randomizing new dot
				}
				else {
					Color = Color++;
					if (Color > 15) { Color = 0; }
				}
				Console::ForegroundColor = ConsoleColor::White; // White Printing
				sprintf_s(str, "%d", Segments * 10 - 30); // Writting string
				for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
				Console::SetCursorPosition((WW - GW) / 2 + GW + 4, (WH - GH) / 2 + 2); printf("     "); // Cleaning Score
				Console::SetCursorPosition((WW - GW) / 2 + GW + 8 - i, (WH - GH) / 2 + 2); printf(str); // Printing new score
				for (i = 0; i < Segments; i++) { SnakeSegments[i] = true; } // Turning on new segment
				DotX = rand() % GW; // Generating new dot X Position
				DotY = rand() % GH; // Generating new dot Y Position
									/* IF POSITION OVERLAPS SNAKE, TRY AGAIN! */
				for (i = 0; i < GW*GH; i++) {
					while (DotX == SnakeSegmentsX[i][0] && DotY == SnakeSegmentsY[i][0]) {
						DotX = rand() % GW;
						DotY = rand() % GH;
					}
				}
			}
			goto Skip1;
			/* SNAKE GAME OVER */
		snakeGameOver:
			option = 0;
			for (i = 0; i < GW*GH; i++) {
				if (SnakeSegments[i]) {
					Console::ForegroundColor = ConsoleColor::White;
					Console::SetCursorPosition(SnakeSegmentsX[i][0] + (WW - GW) / 2, SnakeSegmentsY[i][0] + (WH - GH) / 2);
					if (i == Segments - 1) {
						printf("%c", SnakeSegDir[i][1] == 0 ? 273 : SnakeSegDir[i][1] == 1 ? 287 : SnakeSegDir[i][1] == 2 ? 272 : 286);
					}
					else {
						printf("%c", 254);
					}
				}
			}
			Console::Beep(300, 600);
			for (i = 0; i < GW*GH; i++) {
				if (SnakeSegments[i]) {
					Console::ForegroundColor = ConsoleColor::Gray;
					Console::SetCursorPosition(SnakeSegmentsX[i][0] + (WW - GW) / 2, SnakeSegmentsY[i][0] + (WH - GH) / 2);
					if (i == Segments - 1) {
						printf("%c", SnakeSegDir[i][1] == 0 ? 273 : SnakeSegDir[i][1] == 1 ? 287 : SnakeSegDir[i][1] == 2 ? 272 : 286);
					}
					else {
						printf("%c", 254);
					}
				}
			}
			Console::Beep(200, 600);
			for (i = 0; i < GW*GH; i++) {
				if (SnakeSegments[i]) {
					Console::ForegroundColor = ConsoleColor::DarkGray;
					Console::SetCursorPosition(SnakeSegmentsX[i][0] + (WW - GW) / 2, SnakeSegmentsY[i][0] + (WH - GH) / 2);
					if (i == Segments - 1) {
						printf("%c", SnakeSegDir[i][1] == 0 ? 273 : SnakeSegDir[i][1] == 1 ? 287 : SnakeSegDir[i][1] == 2 ? 272 : 286);
					}
					else {
						printf("%c", 254);
					}
				}
			}
			Console::Beep(100, 1000);
			Console::ForegroundColor = ConsoleColor::Black;
			Console::SetCursorPosition(DotX + (WW - GW) / 2, DotY + (WH - GH) / 2); printf(" ");
			/* ERASING SNAKE */
			for (i = 0; i < GW*GH; i++) {
				if (SnakeSegments[i]) {
					Console::ForegroundColor = ConsoleColor::Black;
					Console::SetCursorPosition(SnakeSegmentsX[i][0] + (WW - GW) / 2, SnakeSegmentsY[i][0] + (WH - GH) / 2); printf("%c", 254);
				}
			}
			prevState = STATE;
			STATE = GAMEOVER;
			option = 0;
			goto gameover;
		Skip1:
			/* FPS DELAY */
			Thread::Sleep(1000 / fps);
			break;
#pragma endregion
#pragma region game_tetris
		case TETRIS:
			goto Skip3; // Skipping piece generation
						/* BLOCK GENERATION */
		tetrisPieceGenerator:
			Shape = nextShape;
			Rotation[1] = nextRotation;
			Color = nextColor;
			nextShape = rand() % 7;
			nextRotation = rand() % 4;
			nextColor = rand() % 15;

		tetrisPieceGenerator2:
			if (y1 <= ((WH - GH) / 2 + 2) && Collided) {
				option = 0;
				for (i = 0; i < (GW + 2)*GH; i++) {
					if (Block[i % (GW + 2)][i / (GW + 2)]) {
						Console::ForegroundColor = ConsoleColor::White;
						Console::SetCursorPosition(((i % (GW + 2)) + (WW - GW) / 2), ((i / (GW + 2)) + (WH - GH) / 2));
						printf("%c", 254);
					}
				}
				Console::Beep(300, 600);
				for (i = 0; i < (GW + 2)*GH; i++) {
					if (Block[i % (GW + 2)][i / (GW + 2)]) {
						Console::ForegroundColor = ConsoleColor::Gray;
						Console::SetCursorPosition(((i % (GW + 2)) + (WW - GW) / 2), ((i / (GW + 2)) + (WH - GH) / 2));
						printf("%c", 254);
					}
				}
				Console::Beep(200, 600);
				for (i = 0; i < (GW + 2)*GH; i++) {
					if (Block[i % (GW + 2)][i / (GW + 2)]) {
						Console::ForegroundColor = ConsoleColor::DarkGray;
						Console::SetCursorPosition(((i % (GW + 2)) + (WW - GW) / 2), ((i / (GW + 2)) + (WH - GH) / 2));
						printf("%c", 254);
					}
				}
				Console::Beep(100, 1000);
				for (i = 0; i < (GW + 2)*GH; i++) {
					if (Block[i % (GW + 2)][i / (GW + 2)]) {
						Console::ForegroundColor = ConsoleColor::Black;
						Console::SetCursorPosition(((i % (GW + 2)) + (WW - GW) / 2), ((i / (GW + 2)) + (WH - GH) / 2));
						printf("%c", 254);
					}
				}
				Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf("    ");
				Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf("    ");
				Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 4); printf("    ");
				Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 5); printf("    ");
				Collided = false;
				prevState = STATE;
				STATE = GAMEOVER;
				option = 0;
				goto gameover;
			}
			Collided = false;
			if (nextColor == 0) { Console::ForegroundColor = ConsoleColor::White; }
			else if (nextColor == 1) { Console::ForegroundColor = ConsoleColor::Gray; }
			else if (nextColor == 2) { Console::ForegroundColor = ConsoleColor::DarkGray; }
			else if (nextColor == 3) { Console::ForegroundColor = ConsoleColor::Cyan; }
			else if (nextColor == 4) { Console::ForegroundColor = ConsoleColor::DarkCyan; }
			else if (nextColor == 5) { Console::ForegroundColor = ConsoleColor::Blue; }
			else if (nextColor == 6) { Console::ForegroundColor = ConsoleColor::DarkBlue; }
			else if (nextColor == 7) { Console::ForegroundColor = ConsoleColor::Green; }
			else if (nextColor == 8) { Console::ForegroundColor = ConsoleColor::DarkGreen; }
			else if (nextColor == 9) { Console::ForegroundColor = ConsoleColor::Yellow; }
			else if (nextColor == 10) { Console::ForegroundColor = ConsoleColor::DarkYellow; }
			else if (nextColor == 11) { Console::ForegroundColor = ConsoleColor::Magenta; }
			else if (nextColor == 12) { Console::ForegroundColor = ConsoleColor::DarkMagenta; }
			else if (nextColor == 13) { Console::ForegroundColor = ConsoleColor::Red; }
			else if (nextColor == 14) { Console::ForegroundColor = ConsoleColor::DarkRed; }

			Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf("    ");
			Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf("    ");
			Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 4); printf("    ");
			Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 5); printf("    ");
			if (nextShape == 0) {
				if (nextRotation == 0) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf(" %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf("%c%c", 254, 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 4); printf(" %c", 254);
				}
				else if (nextRotation == 1) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf(" %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf("%c%c%c", 254, 254, 254);
				}
				else if (nextRotation == 2) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf(" %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf(" %c%c", 254, 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 4); printf(" %c", 254);
				}
				else if (nextRotation == 3) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf("%c%c%c", 254, 254, 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 4); printf(" %c", 254);
				}
			}
			else if (nextShape == 1) {
				if (nextRotation == 0 || nextRotation == 2) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf("%c%c%c%c", 254, 254, 254, 254);
				}
				else if (nextRotation == 1 || nextRotation == 3) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf(" %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf(" %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 4); printf(" %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 5); printf(" %c", 254);
				}
			}
			else if (nextShape == 2) {
				Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf(" %c%c", 254, 254);
				Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf(" %c%c", 254, 254);
			}
			else if (nextShape == 3) {
				if (nextRotation == 1 || nextRotation == 3) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf("%c%c", 254, 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf(" %c%c", 254, 254);
				}
				else if (nextRotation == 0 || nextRotation == 2) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf("  %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf(" %c%c", 254, 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 4); printf(" %c  ", 254);
				}
			}
			else if (nextShape == 4) {
				if (nextRotation == 1 || nextRotation == 3) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf(" %c%c", 254, 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf("%c%c", 254, 254);
				}
				else if (nextRotation == 0 || nextRotation == 2) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf(" %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf(" %c%c", 254, 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 4); printf("  %c", 254);
				}
			}
			else if (nextShape == 5) {
				if (nextRotation == 0) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf(" %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf(" %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 4); printf(" %c%c", 254, 254);
				}
				else if (nextRotation == 1) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf("%c%c%c", 254, 254, 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf("%c", 254);
				}
				else if (nextRotation == 2) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf(" %c%c", 254, 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf("  %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 4); printf("  %c", 254);
				}
				else if (nextRotation == 3) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf("  %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf("%c%c%c", 254, 254, 254);
				}
			}
			else if (nextShape == 6) {
				if (nextRotation == 0) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf("  %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf("  %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 4); printf(" %c%c", 254, 254);
				}
				else if (nextRotation == 1) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf("%c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf("%c%c%c", 254, 254, 254);
				}
				else if (nextRotation == 2) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf(" %c%c", 254, 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf(" %c", 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 4); printf(" %c", 254);
				}
				else if (nextRotation == 3) {
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 2); printf("%c%c%c", 254, 254, 254);
					Console::SetCursorPosition((WW - GW) / 2 + GW + 5, (WH - GH) / 2 + 3); printf("  %c", 254);
				}
			}

			for (i = 0; i < GH; i++) {
				for (n = 0, Blocks = 0; n < (GW + 2); n++) {
					if (Block[n][i]) {
						Blocks++;
					}
				}
				if (Blocks == (GW + 2)) {
					FullLines[i] = true;
				}
				else if (Blocks < GW + 2) {
					FullLines[i] = false;
				}
			}
			for (i = 0; i < GH; i++) {
				if (FullLines[i]) {
					for (n = 0; n < (GW + 2); n++) {
						Console::ForegroundColor = ConsoleColor::White;
						Console::SetCursorPosition(n + (WW - GW) / 2, i + (WH - GH) / 2); printf("%c", 254);
					}
					Console::Beep(150, 100);
					for (n = 0; n < (GW + 2); n++) {
						Console::ForegroundColor = ConsoleColor::DarkGray;
						Console::SetCursorPosition(n + (WW - GW) / 2, i + (WH - GH) / 2); printf("%c", 254);
					}
					Console::Beep(200, 300);
					Console::Beep(300, 300);
					for (n = 0; n < (GW + 2); n++) {
						Console::SetCursorPosition(n + (WW - GW) / 2, i + (WH - GH) / 2); printf(" ");
					}
					Console::Beep(500, 600);
					for (n = (GW + 2)*GH; n > 0; n--) {
						if ((n / (GW + 2)) <= i && (n / (GW + 2)) != 0) {
							if (Block[n % (GW + 2)][n / (GW + 2) - 1]) {
								Block[n % (GW + 2)][n / (GW + 2) - 1] = false;
								Console::SetCursorPosition(n % (GW + 2) + (WW - GW) / 2, (n / (GW + 2)) + (WH - GH) / 2 - 1); printf(" ");
								Block[n % (GW + 2)][n / (GW + 2)] = true;
								bColor[n % (GW + 2)][n / (GW + 2)] = bColor[n % (GW + 2)][n / (GW + 2) - 1];
								if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 0) { Console::ForegroundColor = ConsoleColor::White; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 1) { Console::ForegroundColor = ConsoleColor::Gray; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 2) { Console::ForegroundColor = ConsoleColor::DarkGray; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 3) { Console::ForegroundColor = ConsoleColor::Cyan; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 4) { Console::ForegroundColor = ConsoleColor::DarkCyan; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 5) { Console::ForegroundColor = ConsoleColor::Blue; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 6) { Console::ForegroundColor = ConsoleColor::DarkBlue; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 7) { Console::ForegroundColor = ConsoleColor::Green; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 8) { Console::ForegroundColor = ConsoleColor::DarkGreen; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 9) { Console::ForegroundColor = ConsoleColor::Yellow; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 10) { Console::ForegroundColor = ConsoleColor::DarkYellow; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 11) { Console::ForegroundColor = ConsoleColor::Magenta; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 12) { Console::ForegroundColor = ConsoleColor::DarkMagenta; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 13) { Console::ForegroundColor = ConsoleColor::Red; }
								else if (bColor[n % (GW + 2)][n / (GW + 2) - 1] == 14) { Console::ForegroundColor = ConsoleColor::DarkRed; }
								Console::SetCursorPosition(n % (GW + 2) + (WW - GW) / 2, (n / (GW + 2)) + (WH - GH) / 2); printf("%c", 254);
							}
							else {
								Block[n % (GW + 2)][n / (GW + 2)] = false;
								Console::SetCursorPosition(n % (GW + 2) + (WW - GW) / 2, (n / (GW + 2)) + (WH - GH) / 2); printf(" ");
							}
						}
					}
					Score += 50;
				}
			}
			Console::ForegroundColor = ConsoleColor::DarkGray;
			Console::SetCursorPosition((WW - GW) / 2 + GW + 4, (WH - GH) / 2 + 9); printf("00000");
			Console::ForegroundColor = ConsoleColor::White;
			if (Score > 0) {
				sprintf_s(str, "%d", Score * 5);
				for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
				Console::SetCursorPosition((WW - GW) / 2 + GW + 9 - i, (WH - GH) / 2 + 9); printf(str);
			}

			Console::Beep(100 + rand() % 200, 100); // Beep
			x[0] = (WW - GW) / 2 + 2 + rand() % (GW - 2);
			y1 = (WH - GH) / 2;
			for (i = 0; i < 4; i++) {
				PieceX[i][0] = x[0];
				PieceY[i][0] = y[0];
				bColX[i] = 0;
				bColY[i] = 0;
				rColX[i] = 0;
				rColY[i] = 0;
				lColX[i] = 0;
				lColY[i] = 0;
			}
			Rotation[0] = Rotation[1];
			PieceX[0][0] = 0;
			PieceY[0][0] = 0;
			PieceX[1][0] = 0;
			PieceY[1][0] = 0;
			PieceX[2][0] = 0;
			PieceY[2][0] = 0;
			PieceX[3][0] = 0;
			PieceY[3][0] = 0;
		Skip3:

			/* MECHANICS */
			PieceX[0][1] = PieceX[0][0];
			PieceY[0][1] = PieceY[0][0];
			PieceX[1][1] = PieceX[1][0];
			PieceY[1][1] = PieceY[1][0];
			PieceX[2][1] = PieceX[2][0];
			PieceY[2][1] = PieceY[2][0];
			PieceX[3][1] = PieceX[3][0];
			PieceY[3][1] = PieceY[3][0];
			y1 += (gameSpeed*0.33) / 1.5;
			y[0] = (int)y1;

			/* ON CHANGE */
			if (y[1] != y[0] || x[1] != x[0] || Rotation[0] != Rotation[1]) {
				Rotation[0] = Rotation[1];
				y[1] = y[0];
				x[1] = x[0];

				/* DETECTING COLLISIONS */
				for (i = 0; i < 4; i++) {
					if (bColX[i] == 0 || bColY[i] == 0) {}
					else {
						if (Block[bColX[i] - (WW - GW) / 2][bColY[i] - (WH - GH) / 2]) {
							Collided = true;
						}
						else if (bColY[i] >= (WH - GH) / 2 + GH) {
							Collided = true;
						}
					}
				}
				if (Collided) {
					Score++;
					goto tetrisPieceGenerator;
				}

				/* DEFINE NEW POSITION/VALUES */
				PieceX[0][0] = x[0];
				PieceY[0][0] = y[0];
				//Shape = 4;
				// T Shape
				if (Shape == 0) {
					if (Rotation[0] == 0) {
						// 1
						//20
						// 3
						PieceX[1][0] = x[0];
						PieceY[1][0] = y[0] - 1;
						PieceX[2][0] = x[0] - 1;
						PieceY[2][0] = y[0];
						PieceX[3][0] = x[0];
						PieceY[3][0] = y[0] + 1;
						// Rotation Collision
						rotColX[0] = PieceX[0][0] + 1;
						rotColY[0] = PieceY[0][0];
						rotColX[1] = 0;
						rotColY[1] = 0;
						rotColX[2] = 0;
						rotColY[2] = 0;
						// Bottom collision
						bColX[0] = 0;
						bColY[0] = 0;
						bColX[1] = 0;
						bColY[1] = 0;
						bColX[2] = PieceX[2][0];
						bColY[2] = PieceY[2][0] + 1;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = PieceX[0][0] + 1;
						rColY[0] = PieceY[0][0];
						rColX[1] = PieceX[1][0] + 1;
						rColY[1] = PieceY[1][0];
						rColX[2] = 0;
						rColY[2] = 0;
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = 0;
						lColY[0] = 0;
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = PieceX[2][0] - 1;
						lColY[2] = PieceY[2][0];
						lColX[3] = PieceX[3][0] - 1;
						lColY[3] = PieceY[3][0];
					}
					else if (Rotation[0] == 1) {
						// 1
						//203
						PieceX[1][0] = x[0];
						PieceY[1][0] = y[0] - 1;
						PieceX[2][0] = x[0] - 1;
						PieceY[2][0] = y[0];
						PieceX[3][0] = x[0] + 1;
						PieceY[3][0] = y[0];
						// Rotation Collision
						rotColX[0] = PieceX[0][0];
						rotColY[0] = PieceY[0][0] + 1;
						rotColX[1] = 0;
						rotColY[1] = 0;
						rotColX[2] = 0;
						rotColY[2] = 0;
						// Bottom collision
						bColX[0] = PieceX[0][0];
						bColY[0] = PieceY[0][0] + 1;
						bColX[1] = 0;
						bColY[1] = 0;
						bColX[2] = PieceX[2][0];
						bColY[2] = PieceY[2][0] + 1;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = 0;
						rColY[0] = 0;
						rColX[1] = PieceX[1][0] + 1;
						rColY[1] = PieceY[1][0];
						rColX[2] = 0;
						rColY[2] = 0;
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = 0;
						lColY[0] = 0;
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = PieceX[2][0] - 1;
						lColY[2] = PieceY[2][0];
						lColX[3] = 0;
						lColY[3] = 0;
					}
					else if (Rotation[0] == 2) {
						//1
						//02
						//3
						PieceX[1][0] = x[0];
						PieceY[1][0] = y[0] - 1;
						PieceX[2][0] = x[0] + 1;
						PieceY[2][0] = y[0];
						PieceX[3][0] = x[0];
						PieceY[3][0] = y[0] + 1;
						// Rotation Collision
						rotColX[0] = PieceX[0][0] - 1;
						rotColY[0] = PieceY[0][0];
						rotColX[1] = 0;
						rotColY[1] = 0;
						rotColX[2] = 0;
						rotColY[2] = 0;
						// Bottom collision
						bColX[0] = 0;
						bColY[0] = 0;
						bColX[1] = 0;
						bColY[1] = 0;
						bColX[2] = PieceX[2][0];
						bColY[2] = PieceY[2][0] + 1;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = 0;
						rColY[0] = 0;
						rColX[1] = PieceX[1][0] + 1;
						rColY[1] = PieceY[1][0];
						rColX[2] = PieceX[2][0] + 1;
						rColY[2] = PieceY[2][0];
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = PieceX[0][0] - 1;
						lColY[0] = PieceY[0][0];
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = 0;
						lColY[2] = 0;
						lColX[3] = PieceX[3][0] - 1;
						lColY[3] = PieceY[3][0];
					}
					else if (Rotation[0] == 3) {
						//102
						// 3
						PieceX[1][0] = x[0] - 1;
						PieceY[1][0] = y[0];
						PieceX[2][0] = x[0] + 1;
						PieceY[2][0] = y[0];
						PieceX[3][0] = x[0];
						PieceY[3][0] = y[0] + 1;
						// Rotation Collision
						rotColX[0] = PieceX[0][0];
						rotColY[0] = PieceY[0][0] - 1;
						rotColX[1] = 0;
						rotColY[1] = 0;
						rotColX[2] = 0;
						rotColY[2] = 0;
						// Bottom collision
						bColX[0] = 0;
						bColY[0] = 0;
						bColX[1] = PieceX[1][0];
						bColY[1] = PieceY[1][0] + 1;
						bColX[2] = PieceX[2][0];
						bColY[2] = PieceY[2][0] + 1;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = 0;
						rColY[0] = 0;
						rColX[1] = 0;
						rColY[1] = 0;
						rColX[2] = PieceX[2][0] + 1;
						rColY[2] = PieceY[2][0];
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = 0;
						lColY[0] = 0;
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = 0;
						lColY[2] = 0;
						lColX[3] = PieceX[3][0] - 1;
						lColY[3] = PieceY[3][0];
					}
				}
				// I Shape
				else if (Shape == 1) {
					if (Rotation[0] == 1 || Rotation[0] == 3) {
						PieceX[1][0] = x[0];
						PieceY[1][0] = y[0] - 2;
						PieceX[2][0] = x[0];
						PieceY[2][0] = y[0] - 1;
						PieceX[3][0] = x[0];
						PieceY[3][0] = y[0] + 1;
						// Rotation Collision
						rotColX[0] = PieceX[0][0] - 1;
						rotColY[0] = PieceY[0][0];
						rotColX[1] = PieceX[0][0] + 1;
						rotColY[1] = PieceY[0][0];
						rotColX[2] = PieceX[0][0] + 2;
						rotColY[2] = PieceY[0][0];
						// Bottom collision
						bColX[0] = 0;
						bColY[0] = 0;
						bColX[1] = 0;
						bColY[1] = 0;
						bColX[2] = 0;
						bColY[2] = 0;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = PieceX[0][0] + 1;
						rColY[0] = PieceY[0][0];
						rColX[1] = PieceX[1][0] + 1;
						rColY[1] = PieceY[1][0];
						rColX[2] = PieceX[2][0] + 1;
						rColY[2] = PieceY[2][0];
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = PieceX[0][0] - 1;
						lColY[0] = PieceY[0][0];
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = PieceX[2][0] - 1;
						lColY[2] = PieceY[2][0];
						lColX[3] = PieceX[3][0] - 1;
						lColY[3] = PieceY[3][0];
					}
					if (Rotation[0] == 0 || Rotation[0] == 2) {
						PieceX[1][0] = x[0] + 2;
						PieceY[1][0] = y[0];
						PieceX[2][0] = x[0] + 1;
						PieceY[2][0] = y[0];
						PieceX[3][0] = x[0] - 1;
						PieceY[3][0] = y[0];
						// Rotation Collision
						rotColX[0] = PieceX[0][0];
						rotColY[0] = PieceY[0][0] - 1;
						rotColX[1] = PieceX[0][0];
						rotColY[1] = PieceY[0][0] + 1;
						rotColX[2] = PieceX[0][0];
						rotColY[2] = PieceY[0][0] + 2;
						// Bottom collision
						bColX[0] = PieceX[0][0];
						bColY[0] = PieceY[0][0] + 1;
						bColX[1] = PieceX[1][0];
						bColY[1] = PieceY[1][0] + 1;
						bColX[2] = PieceX[2][0];
						bColY[2] = PieceY[2][0] + 1;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = 0;
						rColY[0] = 0;
						rColX[1] = PieceX[1][0] + 1;
						rColY[1] = PieceY[1][0];
						rColX[2] = 0;
						rColY[2] = 0;
						rColX[3] = 0;
						rColY[3] = 0;
						// Left collision
						lColX[0] = 0;
						lColY[0] = 0;
						lColX[1] = 0;
						lColY[1] = 0;
						lColX[2] = 0;
						lColY[2] = 0;
						lColX[3] = PieceX[3][0] - 1;
						lColY[3] = PieceY[3][0];
					}
				}
				// Cube Shape
				else if (Shape == 2) {
					//12
					//03
					PieceX[1][0] = x[0];
					PieceY[1][0] = y[0] - 1;
					PieceX[2][0] = x[0] + 1;
					PieceY[2][0] = y[0] - 1;
					PieceX[3][0] = x[0] + 1;
					PieceY[3][0] = y[0];
					// Rotation Collision
					rotColX[0] = 0;
					rotColY[0] = 0;
					rotColX[1] = 0;
					rotColY[1] = 0;
					rotColX[2] = 0;
					rotColY[2] = 0;
					// Bottom collision
					bColX[0] = PieceX[0][0];
					bColY[0] = PieceY[0][0] + 1;
					bColX[1] = 0;
					bColY[1] = 0;
					bColX[2] = 0;
					bColY[2] = 0;
					bColX[3] = PieceX[3][0];
					bColY[3] = PieceY[3][0] + 1;
					// Right collision
					rColX[0] = 0;
					rColY[0] = 0;
					rColX[1] = 0;
					rColY[1] = 0;
					rColX[2] = PieceX[2][0] + 1;
					rColY[2] = PieceY[2][0];
					rColX[3] = PieceX[3][0] + 1;
					rColY[3] = PieceY[3][0];
					// Left collision
					lColX[0] = PieceX[0][0] - 1;
					lColY[0] = PieceY[0][0];
					lColX[1] = PieceX[1][0] - 1;
					lColY[1] = PieceY[1][0];
					lColX[2] = 0;
					lColY[2] = 0;
					lColX[3] = 0;
					lColY[3] = 0;
				}
				// S Shape 1
				else if (Shape == 3) {
					if (Rotation[0] == 1 || Rotation[0] == 3) {
						//12
						// 03
						PieceX[1][0] = x[0] - 1;
						PieceY[1][0] = y[0] - 1;
						PieceX[2][0] = x[0];
						PieceY[2][0] = y[0] - 1;
						PieceX[3][0] = x[0] + 1;
						PieceY[3][0] = y[0];
						// Rotation Collision
						rotColX[0] = PieceX[0][0] - 1;
						rotColY[0] = PieceY[0][0];
						rotColX[1] = PieceX[0][0] - 1;
						rotColY[1] = PieceY[0][0] + 1;
						rotColX[2] = 0;
						rotColY[2] = 0;
						// Bottom collision
						bColX[0] = PieceX[0][0];
						bColY[0] = PieceY[0][0] + 1;
						bColX[1] = PieceX[1][0];
						bColY[1] = PieceY[1][0] + 1;
						bColX[2] = 0;
						bColY[2] = 0;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = 0;
						rColY[0] = 0;
						rColX[1] = 0;
						rColY[1] = 0;
						rColX[2] = PieceX[2][0] + 1;
						rColY[2] = PieceY[2][0];
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = PieceX[0][0] - 1;
						lColY[0] = PieceY[0][0];
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = 0;
						lColY[2] = 0;
						lColX[3] = 0;
						lColY[3] = 0;
					}
					if (Rotation[0] == 0 || Rotation[0] == 2) {
						// 1
						//20
						//3
						PieceX[1][0] = x[0];
						PieceY[1][0] = y[0] - 1;
						PieceX[2][0] = x[0] - 1;
						PieceY[2][0] = y[0];
						PieceX[3][0] = x[0] - 1;
						PieceY[3][0] = y[0] + 1;
						// Rotation Collision
						rotColX[0] = PieceX[0][0] - 1;
						rotColY[0] = PieceY[0][0] - 1;
						rotColX[1] = PieceX[0][0] + 1;
						rotColY[1] = PieceY[0][0];
						rotColX[2] = 0;
						rotColY[2] = 0;
						// Bottom collision
						bColX[0] = PieceX[0][0];
						bColY[0] = PieceY[0][0] + 1;
						bColX[1] = 0;
						bColY[1] = 0;
						bColX[2] = 0;
						bColY[2] = 0;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = PieceX[0][0] + 1;
						rColY[0] = PieceY[0][0];
						rColX[1] = PieceX[1][0] + 1;
						rColY[1] = PieceY[1][0];
						rColX[2] = 0;
						rColY[2] = 0;
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = 0;
						lColY[0] = 0;
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = PieceX[2][0] - 1;
						lColY[2] = PieceY[2][0];
						lColX[3] = PieceX[3][0] - 1;
						lColY[3] = PieceY[3][0];
					}
				}
				// S Shape 2
				else if (Shape == 4) {
					if (Rotation[0] == 1 || Rotation[0] == 3) {
						// 12
						//30
						PieceX[1][0] = x[0];
						PieceY[1][0] = y[0] - 1;
						PieceX[2][0] = x[0] + 1;
						PieceY[2][0] = y[0] - 1;
						PieceX[3][0] = x[0] - 1;
						PieceY[3][0] = y[0];
						// Rotation Collision
						rotColX[0] = PieceX[0][0] - 1;
						rotColY[0] = PieceY[0][0] - 1;
						rotColX[1] = PieceX[0][0];
						rotColY[1] = PieceY[0][0] + 1;
						rotColX[2] = 0;
						rotColY[2] = 0;
						// Bottom collision
						bColX[0] = PieceX[0][0];
						bColY[0] = PieceY[0][0] + 1;
						bColX[1] = 0;
						bColY[1] = 0;
						bColX[2] = PieceX[2][0];
						bColY[2] = PieceY[2][0] + 1;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = PieceX[0][0] + 1;
						rColY[0] = PieceY[0][0];
						rColX[1] = 0;
						rColY[1] = 0;
						rColX[2] = PieceX[2][0] + 1;
						rColY[2] = PieceY[2][0];
						rColX[3] = 0;
						rColY[3] = 0;
						// Left collision
						lColX[0] = 0;
						lColY[0] = 0;
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = 0;
						lColY[2] = 0;
						lColX[3] = PieceX[3][0] - 1;
						lColY[3] = PieceY[3][0];
					}
					if (Rotation[0] == 0 || Rotation[0] == 2) {
						//1
						//20
						// 3
						PieceX[1][0] = x[0] - 1;
						PieceY[1][0] = y[0] - 1;
						PieceX[2][0] = x[0] - 1;
						PieceY[2][0] = y[0];
						PieceX[3][0] = x[0];
						PieceY[3][0] = y[0] + 1;
						// Rotation Collision
						rotColX[0] = PieceX[0][0];
						rotColY[0] = PieceY[0][0] - 1;
						rotColX[1] = PieceX[0][0] + 1;
						rotColY[1] = PieceY[0][0] - 1;
						rotColX[2] = 0;
						rotColY[2] = 0;
						// Bottom collision
						bColX[0] = 0;
						bColY[0] = 0;
						bColX[1] = 0;
						bColY[1] = 0;
						bColX[2] = PieceX[2][0];
						bColY[2] = PieceY[2][0] + 1;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = PieceX[0][0] + 1;
						rColY[0] = PieceY[0][0];
						rColX[1] = PieceX[1][0] + 1;
						rColY[1] = PieceY[1][0];
						rColX[2] = 0;
						rColY[2] = 0;
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = 0;
						lColY[0] = 0;
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = PieceX[2][0] - 1;
						lColY[2] = PieceY[2][0];
						lColX[3] = PieceX[3][0] - 1;
						lColY[3] = PieceY[3][0];
					}
				}
				// L Shape 1
				else if (Shape == 5) {
					if (Rotation[0] == 0) {
						//1
						//0
						//23
						PieceX[1][0] = x[0];
						PieceY[1][0] = y[0] - 1;
						PieceX[2][0] = x[0];
						PieceY[2][0] = y[0] + 1;
						PieceX[3][0] = x[0] + 1;
						PieceY[3][0] = y[0] + 1;
						// Rotation Collision
						rotColX[0] = PieceX[0][0] - 1;
						rotColY[0] = PieceY[0][0];
						rotColX[1] = PieceX[0][0] - 1;
						rotColY[1] = PieceY[0][0] + 1;
						rotColX[2] = PieceX[0][0] + 1;
						rotColY[2] = PieceY[0][0];
						// Bottom collision
						bColX[0] = 0;
						bColY[0] = 0;
						bColX[1] = 0;
						bColY[1] = 0;
						bColX[2] = PieceX[2][0];
						bColY[2] = PieceY[2][0] + 1;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = PieceX[0][0] + 1;
						rColY[0] = PieceY[0][0];
						rColX[1] = PieceX[1][0] + 1;
						rColY[1] = PieceY[1][0];
						rColX[2] = 0;
						rColY[2] = 0;
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = PieceX[0][0] - 1;
						lColY[0] = PieceY[0][0];
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = PieceX[2][0] - 1;
						lColY[2] = PieceY[2][0];
						lColX[3] = 0;
						lColY[3] = 0;
					}
					else if (Rotation[0] == 1) {
						//102
						//3
						PieceX[1][0] = x[0] - 1;
						PieceY[1][0] = y[0];
						PieceX[2][0] = x[0] + 1;
						PieceY[2][0] = y[0];
						PieceX[3][0] = x[0] - 1;
						PieceY[3][0] = y[0] + 1;
						// Rotation Collision
						rotColX[0] = PieceX[0][0] - 1;
						rotColY[0] = PieceY[0][0] - 1;
						rotColX[1] = PieceX[0][0];
						rotColY[1] = PieceY[0][0] - 1;
						rotColX[2] = PieceX[0][0];
						rotColY[2] = PieceY[0][0] + 1;
						// Bottom collision
						bColX[0] = PieceX[0][0];
						bColY[0] = PieceY[0][0] + 1;
						bColX[1] = 0;
						bColY[1] = 0;
						bColX[2] = PieceX[2][0];
						bColY[2] = PieceY[2][0] + 1;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = 0;
						rColY[0] = 0;
						rColX[1] = 0;
						rColY[1] = 0;
						rColX[2] = PieceX[2][0] + 1;
						rColY[2] = PieceY[2][0];
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = 0;
						lColY[0] = 0;
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = 0;
						lColY[2] = 0;
						lColX[3] = PieceX[3][0] - 1;
						lColY[3] = PieceY[3][0];
					}
					else if (Rotation[0] == 2) {
						//12
						// 0
						// 3
						PieceX[1][0] = x[0] - 1;
						PieceY[1][0] = y[0] - 1;
						PieceX[2][0] = x[0];
						PieceY[2][0] = y[0] - 1;
						PieceX[3][0] = x[0];
						PieceY[3][0] = y[0] + 1;
						// Rotation Collision
						rotColX[0] = PieceX[0][0] - 1;
						rotColY[0] = PieceY[0][0];
						rotColX[1] = PieceX[0][0] + 1;
						rotColY[1] = PieceY[0][0];
						rotColX[2] = PieceX[0][0] + 1;
						rotColY[2] = PieceY[0][0] - 1;
						// Bottom collision
						bColX[0] = 0;
						bColY[0] = 0;
						bColX[1] = PieceX[1][0];
						bColY[1] = PieceY[1][0] + 1;
						bColX[2] = 0;
						bColY[2] = 0;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = PieceX[0][0] + 1;
						rColY[0] = PieceY[0][0];
						rColX[1] = 0;
						rColY[1] = 0;
						rColX[2] = PieceX[2][0] + 1;
						rColY[2] = PieceY[2][0];
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = PieceX[0][0] - 1;
						lColY[0] = PieceY[0][0];
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = 0;
						lColY[2] = 0;
						lColX[3] = PieceX[3][0] - 1;
						lColY[3] = PieceY[3][0];
					}
					else if (Rotation[0] == 3) {
						//  1
						//203
						PieceX[1][0] = x[0] + 1;
						PieceY[1][0] = y[0] - 1;
						PieceX[2][0] = x[0] - 1;
						PieceY[2][0] = y[0];
						PieceX[3][0] = x[0] + 1;
						PieceY[3][0] = y[0];
						// Rotation Collision
						rotColX[0] = PieceX[0][0];
						rotColY[0] = PieceY[0][0] - 1;
						rotColX[1] = PieceX[0][0];
						rotColY[1] = PieceY[0][0] + 1;
						rotColX[2] = PieceX[0][0] + 1;
						rotColY[2] = PieceY[0][0] + 1;
						// Bottom collision
						bColX[0] = PieceX[0][0];
						bColY[0] = PieceY[0][0] + 1;
						bColX[1] = 0;
						bColY[1] = 0;
						bColX[2] = PieceX[2][0];
						bColY[2] = PieceY[2][0] + 1;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = 0;
						rColY[0] = 0;
						rColX[1] = PieceX[1][0] + 1;
						rColY[1] = PieceY[1][0];
						rColX[2] = 0;
						rColY[2] = 0;
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = 0;
						lColY[0] = 0;
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = PieceX[2][0] - 1;
						lColY[2] = PieceY[2][0];
						lColX[3] = 0;
						lColY[3] = 0;
					}
				}
				// L Shape 2
				else if (Shape == 6) {
					if (Rotation[0] == 0) {
						// 1
						// 0
						//23
						PieceX[1][0] = x[0];
						PieceY[1][0] = y[0] - 1;
						PieceX[2][0] = x[0] - 1;
						PieceY[2][0] = y[0] + 1;
						PieceX[3][0] = x[0];
						PieceY[3][0] = y[0] + 1;
						// Rotation Collision
						rotColX[0] = PieceX[0][0] - 1;
						rotColY[0] = PieceY[0][0];
						rotColX[1] = PieceX[0][0] - 1;
						rotColY[1] = PieceY[0][0] - 1;
						rotColX[2] = PieceX[0][0] + 1;
						rotColY[2] = PieceY[0][0];
						// Bottom collision
						bColX[0] = 0;
						bColY[0] = 0;
						bColX[1] = 0;
						bColY[1] = 0;
						bColX[2] = PieceX[2][0];
						bColY[2] = PieceY[2][0] + 1;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = PieceX[0][0] + 1;
						rColY[0] = PieceY[0][0];
						rColX[1] = PieceX[1][0] + 1;
						rColY[1] = PieceY[1][0];
						rColX[2] = 0;
						rColY[2] = 0;
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = PieceX[0][0] - 1;
						lColY[0] = PieceY[0][0];
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = PieceX[2][0] - 1;
						lColY[2] = PieceY[2][0];
						lColX[3] = 0;
						lColY[3] = 0;
					}
					else if (Rotation[0] == 1) {
						//1
						//203
						PieceX[1][0] = x[0] - 1;
						PieceY[1][0] = y[0] - 1;
						PieceX[2][0] = x[0] - 1;
						PieceY[2][0] = y[0];
						PieceX[3][0] = x[0] + 1;
						PieceY[3][0] = y[0];
						// Rotation Collision
						rotColX[0] = PieceX[0][0];
						rotColY[0] = PieceY[0][0] + 1;
						rotColX[1] = PieceX[0][0];
						rotColY[1] = PieceY[0][0] - 1;
						rotColX[2] = PieceX[0][0] + 1;
						rotColY[2] = PieceY[0][0] - 1;
						// Bottom collision
						bColX[0] = PieceX[0][0];
						bColY[0] = PieceY[0][0] + 1;
						bColX[1] = 0;
						bColY[1] = 0;
						bColX[2] = PieceX[2][0];
						bColY[2] = PieceY[2][0] + 1;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = 0;
						rColY[0] = 0;
						rColX[1] = PieceX[1][0] + 1;
						rColY[1] = PieceY[1][0];
						rColX[2] = 0;
						rColY[2] = 0;
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = 0;
						lColY[0] = 0;
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = PieceX[2][0] - 1;
						lColY[2] = PieceY[2][0];
						lColX[3] = 0;
						lColY[3] = 0;
					}
					else if (Rotation[0] == 2) {
						//12
						//0
						//3
						PieceX[1][0] = x[0];
						PieceY[1][0] = y[0] - 1;
						PieceX[2][0] = x[0] + 1;
						PieceY[2][0] = y[0] - 1;
						PieceX[3][0] = x[0];
						PieceY[3][0] = y[0] + 1;
						// Rotation Collision
						rotColX[0] = PieceX[0][0] - 1;
						rotColY[0] = PieceY[0][0];
						rotColX[1] = PieceX[0][0] + 1;
						rotColY[1] = PieceY[0][0];
						rotColX[2] = PieceX[0][0] + 1;
						rotColY[2] = PieceY[0][0] + 1;
						// Bottom collision
						bColX[0] = 0;
						bColY[0] = 0;
						bColX[1] = 0;
						bColY[1] = 0;
						bColX[2] = PieceX[2][0];
						bColY[2] = PieceY[2][0] + 1;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = PieceX[0][0] + 1;
						rColY[0] = PieceY[0][0];
						rColX[1] = 0;
						rColY[1] = 0;
						rColX[2] = PieceX[2][0] + 1;
						rColY[2] = PieceY[2][0];
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = PieceX[0][0] - 1;
						lColY[0] = PieceY[0][0];
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = 0;
						lColY[2] = 0;
						lColX[3] = PieceX[3][0] - 1;
						lColY[3] = PieceY[3][0];
					}
					else if (Rotation[0] == 3) {
						//102
						//  3
						PieceX[1][0] = x[0] - 1;
						PieceY[1][0] = y[0];
						PieceX[2][0] = x[0] + 1;
						PieceY[2][0] = y[0];
						PieceX[3][0] = x[0] + 1;
						PieceY[3][0] = y[0] + 1;
						// Rotation Collision
						rotColX[0] = PieceX[0][0];
						rotColY[0] = PieceY[0][0] - 1;
						rotColX[1] = PieceX[0][0];
						rotColY[1] = PieceY[0][0] + 1;
						rotColX[2] = PieceX[0][0] - 1;
						rotColY[2] = PieceY[0][0] + 1;
						// Bottom collision
						bColX[0] = PieceX[0][0];
						bColY[0] = PieceY[0][0] + 1;
						bColX[1] = PieceX[1][0];
						bColY[1] = PieceY[1][0] + 1;
						bColX[2] = 0;
						bColY[2] = 0;
						bColX[3] = PieceX[3][0];
						bColY[3] = PieceY[3][0] + 1;
						// Right collision
						rColX[0] = 0;
						rColY[0] = 0;
						rColX[1] = 0;
						rColY[1] = 0;
						rColX[2] = PieceX[2][0] + 1;
						rColY[2] = PieceY[2][0];
						rColX[3] = PieceX[3][0] + 1;
						rColY[3] = PieceY[3][0];
						// Left collision
						lColX[0] = 0;
						lColY[0] = 0;
						lColX[1] = PieceX[1][0] - 1;
						lColY[1] = PieceY[1][0];
						lColX[2] = 0;
						lColY[2] = 0;
						lColX[3] = PieceX[3][0] - 1;
						lColY[3] = PieceY[3][0];
					}
				}

				/* SET COLOR */
				if (Color == 0) { Console::ForegroundColor = ConsoleColor::White; }
				else if (Color == 1) { Console::ForegroundColor = ConsoleColor::Gray; }
				else if (Color == 2) { Console::ForegroundColor = ConsoleColor::DarkGray; }
				else if (Color == 3) { Console::ForegroundColor = ConsoleColor::Cyan; }
				else if (Color == 4) { Console::ForegroundColor = ConsoleColor::DarkCyan; }
				else if (Color == 5) { Console::ForegroundColor = ConsoleColor::Blue; }
				else if (Color == 6) { Console::ForegroundColor = ConsoleColor::DarkBlue; }
				else if (Color == 7) { Console::ForegroundColor = ConsoleColor::Green; }
				else if (Color == 8) { Console::ForegroundColor = ConsoleColor::DarkGreen; }
				else if (Color == 9) { Console::ForegroundColor = ConsoleColor::Yellow; }
				else if (Color == 10) { Console::ForegroundColor = ConsoleColor::DarkYellow; }
				else if (Color == 11) { Console::ForegroundColor = ConsoleColor::Magenta; }
				else if (Color == 12) { Console::ForegroundColor = ConsoleColor::DarkMagenta; }
				else if (Color == 13) { Console::ForegroundColor = ConsoleColor::Red; }
				else if (Color == 14) { Console::ForegroundColor = ConsoleColor::DarkRed; }

				/* RE-PRINT PIECE */
				for (i = 0; i < 4; i++) {
					if (PieceY[i][1]>(WH - GH) / 2 - 1) {
						Console::SetCursorPosition(PieceX[i][1], PieceY[i][1]); printf("%c", 32);
						Block[PieceX[i][1] - (WW - GW) / 2][PieceY[i][1] - (WH - GH) / 2] = false;
					}
				}
				for (i = 0; i < 4; i++) {
					if (PieceY[i][0]>(WH - GH) / 2 - 1) {
						Console::SetCursorPosition(PieceX[i][0], PieceY[i][0]); printf("%c", 254);
						Block[PieceX[i][0] - (WW - GW) / 2][PieceY[i][0] - (WH - GH) / 2] = true;
						bColor[PieceX[i][0] - (WW - GW) / 2][PieceY[i][0] - (WH - GH) / 2] = Color;
					}
				}

			}
			gameSpeed = defaultSpeed;
			AbleMove[0] = false;

			/* GETTING INPUT */
			if (Console::KeyAvailable) {
				ConsoleKeyInfo tecla;
				tecla = Console::ReadKey(true);
				if (tecla.Key == ConsoleKey::RightArrow) {
					if (rColX[0] >= (WW - GW) / 2 + GW + 2 && rColX[0] != 0 || rColX[1] >= (WW - GW) / 2 + GW + 2 && rColX[1] != 0 || rColX[2] >= (WW - GW) / 2 + GW + 2 && rColX[2] != 0 || rColX[3] >= (WW - GW) / 2 + GW + 2 && rColX[3] != 0) {}
					else if (Block[rColX[0] - (WW - GW) / 2][rColY[0] - (WH - GH) / 2] && rColX[0] != 0 || Block[rColX[1] - (WW - GW) / 2][rColY[1] - (WH - GH) / 2] && rColX[1] != 0 || Block[rColX[2] - (WW - GW) / 2][rColY[2] - (WH - GH) / 2] && rColX[2] != 0 || Block[rColX[3] - (WW - GW) / 2][rColY[3] - (WH - GH) / 2] && rColX[3] != 0) {
						AbleMove[0] = false;
					}
					else {
						AbleMove[0] = true;
					}
					if (AbleMove[0]) {
						x[1] = x[0];
						y[1] = y[0];
						x[0]++;
						AbleMove[0] = false;
					}
				}
				else if (tecla.Key == ConsoleKey::UpArrow) {
					if (rotColX[0] >= (WW - GW) / 2 + GW + 2 && rotColX[0] != 0 || rotColX[1] >= (WW - GW) / 2 + GW + 2 && rotColX[1] != 0 || rotColX[2] >= (WW - GW) / 2 + GW + 2 && rotColX[2] != 0) {}
					else if (rotColX[0] < (WW - GW) / 2 && rotColX[0] != 0 || rotColX[1] < (WW - GW) / 2 && rotColX[1] != 0 || rotColX[2] < (WW - GW) / 2 && rotColX[2] != 0) {}
					else if (rotColY[0] >= (WH - GH) / 2 + GH && rotColY[0] != 0 || rotColY[1] >= (WH - GH) / 2 + GH && rotColY[1] != 0 || rotColY[2] >= (WH - GH) / 2 + GH && rotColY[2] != 0) {}
					else if (Block[rotColX[0] - (WW - GW) / 2][rotColY[0] - (WH - GH) / 2] && rotColX[0] != 0 || Block[rotColX[1] - (WW - GW) / 2][rotColY[1] - (WH - GH) / 2] && rotColX[1] != 0 || Block[rotColX[2] - (WW - GW) / 2][rotColY[2] - (WH - GH) / 2] && rotColX[2] != 0) {
						AbleMove[0] = false;
					}
					else {
						AbleMove[0] = true;
					}
					if (AbleMove[0]) {
						Rotation[1]++;
						if (Rotation[1] > 3) { Rotation[1] = 0; }
					}
				}
				else if (tecla.Key == ConsoleKey::LeftArrow) {
					if (lColX[0] < (WW - GW) / 2 && lColX[0] != 0 || lColX[1] < (WW - GW) / 2 && lColX[1] != 0 || lColX[2] < (WW - GW) / 2 && lColX[2] != 0 || lColX[3] < (WW - GW) / 2 && lColX[3] != 0) {}
					else if (Block[lColX[0] - (WW - GW) / 2][lColY[0] - (WH - GH) / 2] && lColX[0] != 0 || Block[lColX[1] - (WW - GW) / 2][lColY[1] - (WH - GH) / 2] && lColX[1] != 0 || Block[lColX[2] - (WW - GW) / 2][lColY[2] - (WH - GH) / 2] && lColX[2] != 0 || Block[lColX[3] - (WW - GW) / 2][lColY[3] - (WH - GH) / 2] && lColX[3] != 0) {
						AbleMove[0] = false;
					}
					else {
						AbleMove[0] = true;
					}
					if (AbleMove[0]) {
						x[1] = x[0];
						y[1] = y[0];
						x[0]--;
						AbleMove[0] = false;
					}
				}
				else if (tecla.Key == ConsoleKey::DownArrow) {
					gameSpeed = 3;
				}
				else if (tecla.Key == ConsoleKey::Escape) {
					Console::ForegroundColor = ConsoleColor::DarkGray; // Foreground Color
					Console::SetCursorPosition((WW - GW) / 2 - 1, GH + (WH - GH) / 2 + 1); printf("ESC to resume, ENTER to quit");
					defaultSpeed = gameSpeed;
					gameSpeed = 0;
					prevState = STATE;
					STATE = PAUSE;
				}
			}

			Thread::Sleep(1000 / fps);
			break;
#pragma endregion
#pragma region game_formula1
		case F1:
			/* REDRAW PLAYER */
			Console::ForegroundColor = ConsoleColor::White;
			if (F1X[8] != F1X[0] || F1Begin) {
				for (i = 1; i < 8; i++) {
					Console::SetCursorPosition(F1X[i], F1Y[i]); printf(" ");
				}

				F1X[1] = F1X[0];
				F1Y[1] = F1Y[0] - 1;
				F1X[2] = F1X[0] - 1;
				F1Y[2] = F1Y[0];
				F1X[3] = F1X[0];
				F1Y[3] = F1Y[0];
				F1X[4] = F1X[0] + 1;
				F1Y[4] = F1Y[0];
				F1X[5] = F1X[0];
				F1Y[5] = F1Y[0] + 1;
				F1X[6] = F1X[0] - 1;
				F1Y[6] = F1Y[0] + 2;
				F1X[7] = F1X[0] + 1;
				F1Y[7] = F1Y[0] + 2;

				for (i = 1; i < 8; i++) {
					Console::SetCursorPosition(F1X[i], F1Y[i]); printf("%c", 254);
				}
				F1X[8] = F1X[0];
			}
			/* SIDE WALLS */
			Console::ForegroundColor = ConsoleColor::DarkGray;
			for (i = 0; i < GH + 10; i++) {
				hVar = (WH - GH) / 2 - 1 + i + ((int)(aVar*(gameSpeed * 5)) % 4) - 4;
				if (hVar < (WH - GH) / 2 + GH && hVar >= (WH - GH) / 2) {
					if (i % 4 == 0) {
						Console::SetCursorPosition(wallX[0], hVar);
						printf(" ");
						Console::SetCursorPosition(wallX[1], hVar);
						printf(" ");
					}
					else {
						Console::SetCursorPosition(wallX[0], hVar);
						printf("%c", 254);
						Console::SetCursorPosition(wallX[1], hVar);
						printf("%c", 254);
					}
				}
			}
			aVar++;
			tVar += gameSpeed;
			/* BEGINNING */
			if (F1Begin) {
				Console::ForegroundColor = ConsoleColor::White;
				for (i = 0; i < 54; i++) {
					Console::SetCursorPosition(WW / 2 - 2 + i % 6, WH / 3 - 1 + i / 6);
					if (i % 6 == 0) {
						if (i / 6 == 0) {
							printf("%c", 218);
						}
						else if (i / 6 == 8) {
							printf("%c", 192);
						}
						else if ((i / 6) % 2 == 0) {
							printf("%c", 195);
						}
						else {
							printf("%c", 179);
						}
					}
					else if (i % 6 == 5) {
						if (i / 6 == 0) {
							printf("%c", 191);
						}
						else if (i / 6 == 8) {
							printf("%c", 217);
						}
						else if ((i / 6) % 2 == 0) {
							printf("%c", 180);
						}
						else {
							printf("%c", 179);
						}
					}
					else {
						printf("%c", 196);
					}
				}
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - 1, WH / 3);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::SetCursorPosition(WW / 2 - 1, WH / 3 + 2);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::SetCursorPosition(WW / 2 - 1, WH / 3 + 4);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::ForegroundColor = ConsoleColor::Red;
				Console::SetCursorPosition(WW / 2 - 1, WH / 3 + 6);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::Beep(200, 700);

				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - 1, WH / 3);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::SetCursorPosition(WW / 2 - 1, WH / 3 + 2);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::ForegroundColor = ConsoleColor::Yellow;
				Console::SetCursorPosition(WW / 2 - 1, WH / 3 + 4);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - 1, WH / 3 + 6);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::Beep(250, 700);

				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - 1, WH / 3);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::ForegroundColor = ConsoleColor::Yellow;
				Console::SetCursorPosition(WW / 2 - 1, WH / 3 + 2);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - 1, WH / 3 + 4);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::SetCursorPosition(WW / 2 - 1, WH / 3 + 6);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::Beep(250, 700);

				Console::ForegroundColor = ConsoleColor::Green;
				Console::SetCursorPosition(WW / 2 - 1, WH / 3);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::ForegroundColor = ConsoleColor::Black;
				Console::SetCursorPosition(WW / 2 - 1, WH / 3 + 2);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::SetCursorPosition(WW / 2 - 1, WH / 3 + 4);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::SetCursorPosition(WW / 2 - 1, WH / 3 + 6);
				printf("%c%c%c%c", 219, 219, 219, 219);
				Console::Beep(400, 1000);



				for (i = 0; i < 54; i++) {
					Console::SetCursorPosition(WW / 2 - 2 + i % 6, WH / 3 - 1 + i / 6);
					printf(" ");
				}
				F1Begin = false;

			}
			/* FOES SPAWN */
			if (int(tVar) % 10 == 9) {
				foesOn++;
				if (!(foe[foesOn % maxFoes])) {
					carColor[foesOn % maxFoes] = rand() % 14;
					foeY[foesOn % maxFoes][0] = 0;
					foe[foesOn % maxFoes] = true;
					foeX[foesOn % maxFoes][0] = (WW - GW) / 2 + roadMargin + 3 + (rand() % 6) * 3;
				}
				tVar += 1;
			}
			for (i = 0; i < maxFoes; i++) {
				if (carColor[i] == 0) { Console::ForegroundColor = ConsoleColor::DarkRed; }
				else if (carColor[i] == 1) { Console::ForegroundColor = ConsoleColor::Gray; }
				else if (carColor[i] == 2) { Console::ForegroundColor = ConsoleColor::DarkGray; }
				else if (carColor[i] == 3) { Console::ForegroundColor = ConsoleColor::Cyan; }
				else if (carColor[i] == 4) { Console::ForegroundColor = ConsoleColor::DarkCyan; }
				else if (carColor[i] == 5) { Console::ForegroundColor = ConsoleColor::Blue; }
				else if (carColor[i] == 6) { Console::ForegroundColor = ConsoleColor::DarkBlue; }
				else if (carColor[i] == 7) { Console::ForegroundColor = ConsoleColor::Green; }
				else if (carColor[i] == 8) { Console::ForegroundColor = ConsoleColor::DarkGreen; }
				else if (carColor[i] == 9) { Console::ForegroundColor = ConsoleColor::Yellow; }
				else if (carColor[i] == 10) { Console::ForegroundColor = ConsoleColor::DarkYellow; }
				else if (carColor[i] == 11) { Console::ForegroundColor = ConsoleColor::Magenta; }
				else if (carColor[i] == 12) { Console::ForegroundColor = ConsoleColor::DarkMagenta; }
				else if (carColor[i] == 13) { Console::ForegroundColor = ConsoleColor::Red; }

				if (foe[i]) {
					fY[i] += (gameSpeed*1.5);
					foeY[i][0] = int(fY[i]);
					for (n = 0; n < 12; n++) {
						xF = (foeX[i][1] - 1) + n % 3;
						yF = foeY[i][1] + n / 3;
						for (int t = 0; t < 8; t++) {
							if (xF == F1X[t] && yF == F1Y[t]) {
								Crashed = true;
							}
						}
						if (yF >(WH - GH) / 2 && yF < (WH - GH) / 2 + GH && xF > 0) {
							Console::SetCursorPosition(xF, yF); printf(" ");
						}
					}
					for (n = 0; n < 12; n++) {
						xF = (foeX[i][0] - 1) + n % 3;
						yF = foeY[i][0] + n / 3;
						if (yF >(WH - GH) / 2 && yF < (WH - GH) / 2 + GH) {
							if (n == 0 || n == 2 || n == 6 || n == 8 || n == 10) {}
							else {
								Console::SetCursorPosition(xF, yF); printf("%c", 254);
							}
						}
					}
					if (foeY[i][0] >(WH - GH) / 2 + GH + 2) {
						foe[i] = false;
						foeY[i][0] = 0;
						fY[i] = 0;
						Score++;
						Console::ForegroundColor = ConsoleColor::White; // White Printing
						sprintf_s(str, "%d", Score * 10); // Writting string
						for (i = 0; str[i] != '\0'; i++) {} // Counting Letters
						Console::SetCursorPosition((WW - GW) / 2 + GW + 4, (WH - GH) / 2 + 2); printf("     "); // Cleaning Score
						Console::SetCursorPosition((WW - GW) / 2 + GW + 8 - i, (WH - GH) / 2 + 2); printf(str); // Printing new score
					}
					foeX[i][1] = foeX[i][0];
					foeY[i][1] = foeY[i][0];
				}
				else {
					foeY[i][0] = 0;
				}
			}
			/* GAME OVER */
			if (Crashed) {
				for (i = 0; i < (GW + 2)*GH; i++) {
					Console::SetCursorPosition((WW - GW) / 2 + i % (GW + 2), (WH - GH) / 2 + i / (GW + 2)); printf(" ");
				}
				prevState = STATE;
				STATE = GAMEOVER;
				option = 0;
				goto gameover;
			}
			/* RESET SPEED */
			gameSpeed = defaultSpeed;
			/* GETTING INPUT */
			if (Console::KeyAvailable) {
				ConsoleKeyInfo tecla;
				tecla = Console::ReadKey(true);
				if (tecla.Key == ConsoleKey::RightArrow) {
					if (F1X[0] < wallX[1] - 2) { F1X[0] += 3; }
				}
				else if (tecla.Key == ConsoleKey::UpArrow) { gameSpeed = 1; }
				else if (tecla.Key == ConsoleKey::LeftArrow) {
					if (F1X[0] > wallX[0] + 2) { F1X[0] -= 3; }
				}
				else if (tecla.Key == ConsoleKey::Escape) {
					Console::ForegroundColor = ConsoleColor::DarkGray; // Foreground Color
					Console::SetCursorPosition((WW - GW) / 2 - 1, GH + (WH - GH) / 2 + 1); printf("ESC to resume, ENTER to quit");
					defaultSpeed = gameSpeed;
					gameSpeed = 0;
					prevState = F1;
					STATE = PAUSE;
				}
			}
			Thread::Sleep(1000 / fps);
			break;
#pragma endregion
#pragma region game_arkanoid
		case ARKANOID:
			/* GETTING INPUT */
			gameSpeed = defaultSpeed;
			if (Console::KeyAvailable) {
				ConsoleKeyInfo tecla;
				tecla = Console::ReadKey(true);
				if (tecla.Key == ConsoleKey::RightArrow) {
				}
				else if (tecla.Key == ConsoleKey::UpArrow) {
					gameSpeed = 1;
				}
				else if (tecla.Key == ConsoleKey::LeftArrow) {
				}
				else if (tecla.Key == ConsoleKey::Escape) {
					Console::ForegroundColor = ConsoleColor::DarkGray; // Foreground Color
					Console::SetCursorPosition((WW - GW) / 2 - 1, GH + (WH - GH) / 2 + 1); printf("ESC to resume, ENTER to quit");
					defaultSpeed = gameSpeed;
					gameSpeed = 0;
					prevState = STATE;
					STATE = PAUSE;
				}
			}
			Thread::Sleep(1000 / fps);
			break;
#pragma endregion
#pragma region loop_end
		}
		frame++;
	} while (true);
end:
#pragma endregion

	return 0;
}