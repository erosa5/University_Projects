#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Header.h"

using namespace System;
using namespace System::Text;
using namespace Threading;

const int maxCrossPoints = 100;
char ghostChar = 15;
char pacManChar = 219;
const int maxFood = 500;
int foodCount = 0;
char food = 250;
char especialFood = 254;
ConsoleColor Borders = ConsoleColor::Blue;
ConsoleColor eatableDots = ConsoleColor::DarkGray;

class Entity {
public:
	float x = 20.0f, y = 25.0f, Speed = 0.15f, ghostSpeed = 0.1f;
	int Dir = 4, nextDir = 4, initX, initY, initDir;
	ConsoleColor Color = ConsoleColor::Yellow;
	ConsoleColor Dizzy = ConsoleColor::DarkGray;
	bool Updated = false, isGhost = false, oS = false, oC = false, ghostDizzy = false;

	void move() {
		if (Dir == 0) {
			if (x > Console::BufferWidth - 2) { x = Console::BufferWidth - 2; }
			else { x += isGhost ? (ghostDizzy ? (ghostSpeed / 3) : ghostSpeed) : Speed; }
		}
		else if (Dir == 2) {
			if (x <= 0) { x = 0; }
			else { x -= isGhost ? (ghostDizzy ? (ghostSpeed / 3) : ghostSpeed) : Speed; }
		}
		else if (Dir == 1) {
			if (y <= 0) { y = 0; }
			else { y -= isGhost ? (ghostDizzy ? (ghostSpeed / 3) : ghostSpeed) : Speed; }
		}
		else if (Dir == 3) {
			if (y > Console::BufferHeight - 1) { y = Console::BufferHeight - 1; }
			else { y += isGhost ? (ghostDizzy ? (ghostSpeed / 3) : ghostSpeed) : Speed; }
		}
		else {}
		update();
	}
	void update() {
		if (int(x) != prevX || int(y) != prevY) {
			if (!isGhost) {
				printC(prevX, prevY, 32, ConsoleColor::Black);
			}
			else {
				if (oS) {
					printC(prevX, prevY, 32, ConsoleColor::Black);
					printC(prevX, prevY, especialFood, ConsoleColor::White);
					oS = false;
				}
				else if (oC) {
					printC(prevX, prevY, 32, ConsoleColor::Black);
					printC(prevX, prevY, food, eatableDots);
					oC = false;
				}
				else {
					printC(prevX, prevY, 32, ConsoleColor::Black);

				}
			}
			printC(x, y, isGhost ? ghostChar : pacManChar, ghostDizzy ? Dizzy : Color);
			prevX = int(x);
			prevY = int(y);
			Updated = true;
		}
	}

private:
	int prevX = 0, prevY = 0;
};
class crossPoints {
public:
	int x = 0, y = 0;
	bool sideOn[4] = { false,false,false,false };
	bool Active = false, Visible = true;

	void setPos(int a, int b) {
		x = a;
		y = b;
		Active = true;
	}
	void setOpen(bool a, bool b, bool c, bool d) {
		if (a) { sideOn[0] = true; }
		else { sideOn[0] = false; }
		if (b) { sideOn[1] = true; }
		else { sideOn[1] = false; }
		if (c) { sideOn[2] = true; }
		else { sideOn[2] = false; }
		if (d) { sideOn[3] = true; }
		else { sideOn[3] = false; }
	}

private:

};
struct foodDots {
	int x, y;
	bool Active, Special = false, Visible = false;
};

foodDots Food[maxFood];

void drawDots(int a, int b, int c, int d, bool wFood) {
	int Amount = 0, Lower = 0;
	bool Has = false;
	if (a == c) {
		Amount = abs(b - d);
		if (b < d) { Lower = 2; }
		else { Lower = 4; }
	}
	else {
		Amount = abs(a - c);
		if (a < c) { Lower = 1; }
		else { Lower = 3; }
	}

	for (int i = 0; i < Amount; i++) {
		if (Lower == 1) {
			if (i > 0 && i != Amount) {
				printC(a + i, b - 1, 196, Borders);
				printC(a + i, b + 1, 196, Borders);
				if (wFood) {
					printC(a + i, b, food, eatableDots);
					Food[foodCount].Visible = true;
					Food[foodCount].Active = true;
					Food[foodCount].x = a + i;
					Food[foodCount].y = b;
					foodCount++;
				}
			}
		}
		else if (Lower == 2) {
			if (i > 0 && i != Amount) {
				printC(a - 1, b + i, 179, Borders);
				printC(a + 1, b + i, 179, Borders);
				if (wFood) {
					printC(a, b + i, food, eatableDots);
					Food[foodCount].Visible = true;
					Food[foodCount].Active = true;
					Food[foodCount].x = a;
					Food[foodCount].y = b + i;
					foodCount++;
				}
			}
		}
		else if (Lower == 3) {
			if (i > 0 && i != Amount) {
				printC(c + i, d - 1, 196, Borders);
				printC(c + i, d + 1, 196, Borders);
				if (wFood) {
					printC(c + i, d, food, eatableDots);
					Food[foodCount].Visible = true;
					Food[foodCount].Active = true;
					Food[foodCount].x = c + i;
					Food[foodCount].y = d;
					foodCount++;
				}
			}
		}
		else if (Lower == 4) {
			if (i > 0 && i != Amount) {
				printC(c - 1, d + i, 179, Borders);
				printC(c + 1, d + i, 179, Borders);
				if (wFood) {
					printC(c, d + i, food, eatableDots);
					Food[foodCount].Visible = true;
					Food[foodCount].Active = true;
					Food[foodCount].x = c;
					Food[foodCount].y = d + i;
					foodCount++;
				}
			}
		}
		if (i == 0 || i == Amount) {
			if (wFood) {
				Has = false;
				for (int j = 0; j < maxFood; j++) {
					if (Food[j].x == a && Food[j].y == b) {
						Has = true;
					}
				}
				if (!Has) {
					printC(a, b, food, eatableDots);
				}
				Has = false;
				for (int j = 0; j < maxFood; j++) {
					if (Food[j].x == c && Food[j].y == d) {
						Has = true;
					}
				}
				if (!Has) {
					printC(c, d, food, eatableDots);
				}
			}
			else {
				Food[foodCount].Visible = false;
			}
		}
	}
}

int main() {
	Console::CursorVisible = false;
	Console::SetWindowSize(32, 37);
	Console::SetBufferSize(32, 37);

	crossPoints B[maxCrossPoints];
	Entity A, G[4];

	srand(time(NULL));

	int Score = -1, x, y, Timer = 0, fps = 60, DizzyTime = 10;
	bool Playing = false, ghostsDizzy = false;

	/* DRAW SCENARIO */
	do {
		x = 1;
		y = 3;
		B[0].setPos(x + 1, y + 29);
		B[1].setPos(x + 13, y + 29);
		B[2].setPos(x + 16, y + 29);
		B[3].setPos(x + 28, y + 29);
		B[4].setPos(x + 1, y + 26);
		B[5].setPos(x + 13, y + 26);
		B[6].setPos(x + 16, y + 26);
		B[7].setPos(x + 28, y + 26);
		B[8].setPos(x + 7, y + 26);
		B[9].setPos(x + 10, y + 26);
		B[10].setPos(x + 19, y + 26);
		B[11].setPos(x + 22, y + 26);
		B[12].setPos(x + 4, y + 26);
		B[13].setPos(x + 25, y + 26);
		B[14].setPos(x + 7, y + 23);
		B[15].setPos(x + 10, y + 23);
		B[16].setPos(x + 19, y + 23);
		B[17].setPos(x + 22, y + 23);
		B[18].setPos(x + 4, y + 23);
		B[19].setPos(x + 25, y + 23);
		B[20].setPos(x + 13, y + 23);
		B[21].setPos(x + 16, y + 23);
		B[22].setPos(x + 1, y + 23);
		B[23].setPos(x + 28, y + 23);
		B[24].setPos(x + 7, y + 20);
		B[25].setPos(x + 22, y + 20);
		B[26].setPos(x + 13, y + 20);
		B[27].setPos(x + 16, y + 20);
		B[28].setPos(x + 1, y + 20);
		B[29].setPos(x + 28, y + 20);
		B[30].setPos(x + 10, y + 20);
		B[31].setPos(x + 19, y + 20);
		B[32].setPos(x + 7, y + 14);
		B[33].setPos(x + 22, y + 14);
		B[34].setPos(x + 10, y + 14);
		B[35].setPos(x + 19, y + 14);
		B[36].setPos(x + 1, y + 14);
		B[37].setPos(x + 28, y + 14);
		B[38].setPos(x + 10, y + 17);
		B[39].setPos(x + 19, y + 17);
		B[40].setPos(x + 10, y + 11);
		B[41].setPos(x + 19, y + 11);
		B[42].setPos(x + 13, y + 11);
		B[43].setPos(x + 16, y + 11);
		B[44].setPos(x + 10, y + 8);
		B[45].setPos(x + 19, y + 8);
		B[46].setPos(x + 13, y + 8);
		B[47].setPos(x + 16, y + 8);
		B[48].setPos(x + 10, y + 5);
		B[49].setPos(x + 19, y + 5);
		B[50].setPos(x + 13, y + 5);
		B[51].setPos(x + 16, y + 5);
		B[52].setPos(x + 13, y + 1);
		B[53].setPos(x + 16, y + 1);
		B[54].setPos(x + 7, y + 8);
		B[55].setPos(x + 22, y + 8);
		B[56].setPos(x + 7, y + 5);
		B[57].setPos(x + 22, y + 5);
		B[58].setPos(x + 7, y + 1);
		B[59].setPos(x + 22, y + 1);
		B[60].setPos(x + 1, y + 8);
		B[61].setPos(x + 1, y + 5);
		B[62].setPos(x + 1, y + 1);
		B[63].setPos(x + 28, y + 8);
		B[64].setPos(x + 28, y + 5);
		B[65].setPos(x + 28, y + 1);
		B[66].setPos(B[43].x, B[43].y + 3);
		B[67].setPos(B[42].x, B[42].y + 3);
		drawDots(B[0].x, B[0].y, B[1].x, B[1].y, true);
		drawDots(B[2].x, B[2].y, B[1].x, B[1].y, true);
		drawDots(B[2].x, B[2].y, B[3].x, B[3].y, true);
		drawDots(B[0].x, B[0].y, B[4].x, B[4].y, true);
		drawDots(B[5].x, B[5].y, B[1].x, B[1].y, true);
		drawDots(B[2].x, B[2].y, B[6].x, B[6].y, true);
		drawDots(B[7].x, B[7].y, B[3].x, B[3].y, true);
		drawDots(B[4].x, B[4].y, B[12].x, B[12].y, true);
		drawDots(B[8].x, B[8].y, B[12].x, B[12].y, true);
		drawDots(B[9].x, B[9].y, B[5].x, B[5].y, true);
		drawDots(B[6].x, B[6].y, B[10].x, B[10].y, true);
		drawDots(B[11].x, B[11].y, B[13].x, B[13].y, true);
		drawDots(B[7].x, B[7].y, B[13].x, B[13].y, true);
		drawDots(B[12].x, B[12].y, B[18].x, B[18].y, true);
		drawDots(B[14].x, B[14].y, B[8].x, B[8].y, true);
		drawDots(B[14].x, B[14].y, B[15].x, B[15].y, true);
		drawDots(B[16].x, B[16].y, B[10].x, B[10].y, true);
		drawDots(B[16].x, B[16].y, B[17].x, B[17].y, true);
		drawDots(B[11].x, B[11].y, B[17].x, B[17].y, true);
		drawDots(B[19].x, B[19].y, B[13].x, B[13].y, true);
		drawDots(B[9].x, B[9].y, B[15].x, B[15].y, true);
		drawDots(B[20].x, B[20].y, B[15].x, B[15].y, true);
		drawDots(B[20].x, B[20].y, B[21].x, B[21].y, true);
		drawDots(B[16].x, B[16].y, B[21].x, B[21].y, true);
		drawDots(B[18].x, B[18].y, B[22].x, B[22].y, true);
		drawDots(B[19].x, B[19].y, B[23].x, B[23].y, true);
		drawDots(B[28].x, B[28].y, B[22].x, B[22].y, true);
		drawDots(B[28].x, B[28].y, B[24].x, B[24].y, true);
		drawDots(B[14].x, B[14].y, B[24].x, B[24].y, true);
		drawDots(B[26].x, B[26].y, B[20].x, B[20].y, true);
		drawDots(B[21].x, B[21].y, B[27].x, B[27].y, true);
		drawDots(B[25].x, B[25].y, B[17].x, B[17].y, true);
		drawDots(B[25].x, B[25].y, B[29].x, B[29].y, true);
		drawDots(B[23].x, B[23].y, B[29].x, B[29].y, true);
		drawDots(B[24].x, B[24].y, B[30].x, B[30].y, true);
		drawDots(B[30].x, B[30].y, B[26].x, B[26].y, true);
		drawDots(B[27].x, B[27].y, B[31].x, B[31].y, true);
		drawDots(B[31].x, B[31].y, B[25].x, B[25].y, true);
		drawDots(B[32].x, B[32].y, B[24].x, B[24].y, true);
		drawDots(B[32].x, B[32].y, B[34].x, B[34].y, false);
		drawDots(B[33].x, B[33].y, B[25].x, B[25].y, true);
		drawDots(B[33].x, B[33].y, B[35].x, B[35].y, false);
		drawDots(B[36].x, B[36].y, B[32].x, B[32].y, false);
		drawDots(B[37].x, B[37].y, B[33].x, B[33].y, false);
		drawDots(B[38].x, B[38].y, B[39].x, B[39].y, false);
		drawDots(B[32].x, B[32].y, B[54].x, B[54].y, true);
		drawDots(B[33].x, B[33].y, B[55].x, B[55].y, true);
		drawDots(B[54].x, B[54].y, B[60].x, B[60].y, true);
		drawDots(B[61].x, B[61].y, B[60].x, B[60].y, true);
		drawDots(B[61].x, B[61].y, B[62].x, B[62].y, true);
		drawDots(B[58].x, B[58].y, B[62].x, B[62].y, true);
		drawDots(B[58].x, B[58].y, B[52].x, B[52].y, true);
		drawDots(B[50].x, B[50].y, B[52].x, B[52].y, true);
		drawDots(B[58].x, B[58].y, B[56].x, B[56].y, true);
		drawDots(B[61].x, B[61].y, B[56].x, B[56].y, true);
		drawDots(B[48].x, B[48].y, B[56].x, B[56].y, true);
		drawDots(B[48].x, B[48].y, B[50].x, B[50].y, true);
		drawDots(B[54].x, B[54].y, B[56].x, B[56].y, true);
		drawDots(B[48].x, B[48].y, B[44].x, B[44].y, true);
		drawDots(B[46].x, B[46].y, B[44].x, B[44].y, true);
		drawDots(B[50].x, B[50].y, B[51].x, B[51].y, true);
		drawDots(B[53].x, B[53].y, B[51].x, B[51].y, true);
		drawDots(B[49].x, B[49].y, B[51].x, B[51].y, true);
		drawDots(B[49].x, B[49].y, B[45].x, B[45].y, true);
		drawDots(B[49].x, B[49].y, B[57].x, B[57].y, true);
		drawDots(B[47].x, B[47].y, B[45].x, B[45].y, true);
		drawDots(B[53].x, B[53].y, B[59].x, B[59].y, true);
		drawDots(B[57].x, B[57].y, B[59].x, B[59].y, true);
		drawDots(B[65].x, B[65].y, B[59].x, B[59].y, true);
		drawDots(B[65].x, B[65].y, B[64].x, B[64].y, true);
		drawDots(B[63].x, B[63].y, B[64].x, B[64].y, true);
		drawDots(B[63].x, B[63].y, B[55].x, B[55].y, true);
		drawDots(B[57].x, B[57].y, B[64].x, B[64].y, true);
		drawDots(B[57].x, B[57].y, B[55].x, B[55].y, true);
		B[0].setOpen(1, 1, 0, 0);
		B[1].setOpen(1, 1, 1, 0);
		B[2].setOpen(1, 1, 1, 0);
		B[3].setOpen(0, 1, 1, 0);
		B[4].setOpen(1, 0, 0, 1);
		B[5].setOpen(0, 0, 1, 1);
		B[6].setOpen(1, 0, 0, 1);
		B[7].setOpen(0, 0, 1, 1);
		B[8].setOpen(0, 1, 1, 0);
		B[9].setOpen(1, 1, 0, 0);
		B[10].setOpen(0, 1, 1, 0);
		B[11].setOpen(1, 1, 0, 0);
		B[12].setOpen(1, 1, 1, 0);
		B[13].setOpen(1, 1, 1, 0);
		B[14].setOpen(1, 1, 0, 1);
		B[15].setOpen(1, 0, 1, 1);
		B[16].setOpen(1, 0, 1, 1);
		B[17].setOpen(0, 1, 1, 1);
		B[18].setOpen(0, 0, 1, 1);
		B[19].setOpen(1, 0, 0, 1);
		B[20].setOpen(1, 1, 1, 0);
		B[21].setOpen(1, 1, 1, 0);
		B[22].setOpen(1, 1, 0, 0);
		B[23].setOpen(0, 1, 1, 0);
		B[24].setOpen(1, 1, 1, 1);
		B[25].setOpen(1, 1, 1, 1);
		B[26].setOpen(0, 0, 1, 1);
		B[27].setOpen(1, 0, 0, 1);
		B[28].setOpen(1, 0, 0, 1);
		B[29].setOpen(0, 0, 1, 1);
		B[30].setOpen(1, 1, 1, 0);
		B[31].setOpen(1, 1, 1, 0);
		B[32].setOpen(1, 1, 1, 1);
		B[33].setOpen(1, 1, 1, 1);
		B[34].setOpen(0, 1, 1, 1);
		B[35].setOpen(1, 1, 0, 1);
		B[36].setOpen(1, 0, 1, 0);
		B[37].setOpen(1, 0, 1, 0);
		B[38].setOpen(1, 1, 0, 1);
		B[39].setOpen(0, 1, 1, 1);
		B[40].setOpen(1, 0, 0, 1);
		B[41].setOpen(0, 0, 1, 1);
		B[42].setOpen(1, 1, 1, 0);
		B[43].setOpen(1, 1, 1, 0);
		B[44].setOpen(1, 1, 0, 0);
		B[45].setOpen(0, 1, 1, 0);
		B[46].setOpen(0, 0, 1, 1);
		B[47].setOpen(1, 0, 0, 1);
		B[48].setOpen(1, 0, 1, 1);
		B[49].setOpen(1, 0, 1, 1);
		B[50].setOpen(1, 1, 1, 0);
		B[51].setOpen(1, 1, 1, 0);
		B[52].setOpen(0, 0, 1, 1);
		B[53].setOpen(1, 0, 0, 1);
		B[54].setOpen(0, 1, 1, 1);
		B[55].setOpen(1, 1, 0, 1);
		B[56].setOpen(1, 1, 1, 1);
		B[57].setOpen(1, 1, 1, 1);
		B[58].setOpen(1, 0, 1, 1);
		B[59].setOpen(1, 0, 1, 1);
		B[60].setOpen(1, 1, 0, 0);
		B[61].setOpen(1, 1, 0, 1);
		B[62].setOpen(1, 0, 0, 1);
		B[63].setOpen(0, 1, 1, 0);
		B[64].setOpen(0, 1, 1, 1);
		B[65].setOpen(0, 0, 1, 1);
		B[66].setOpen(0, 1, 0, 0);
		B[67].setOpen(0, 1, 0, 0);

		A.x = B[15].x + abs((B[15].x - B[16].x) / 2);
		A.y = B[15].y;
		for (int i = 0; i < maxCrossPoints; i++) {
			if (B[i].Active && i != 66 && i != 67) {
				//printC(B[i].x, B[i].y, 254, ConsoleColor::White);

				// Up-Right
				if (B[i].sideOn[0] && B[i].sideOn[1]) {
					printC(B[i].x + 1, B[i].y - 1, 192, Borders); // Up-Right
				}
				else if (!B[i].sideOn[0] && B[i].sideOn[1]) {
					printC(B[i].x + 1, B[i].y - 1, 179, Borders); // Up-Right
				}
				else if (B[i].sideOn[0] && !B[i].sideOn[1]) {
					printC(B[i].x + 1, B[i].y - 1, 196, Borders); // Up-Right
				}
				else {
					printC(B[i].x + 1, B[i].y - 1, 191, Borders); // Up-Right
				}

				// Down-Right
				if (B[i].sideOn[0] && B[i].sideOn[3]) {
					printC(B[i].x + 1, B[i].y + 1, 218, Borders); // Down-Right
				}
				else if (!B[i].sideOn[0] && B[i].sideOn[3]) {
					printC(B[i].x + 1, B[i].y + 1, 179, Borders); // Down-Right
				}
				else if (B[i].sideOn[0] && !B[i].sideOn[3]) {
					printC(B[i].x + 1, B[i].y + 1, 196, Borders); // Down-Right
				}
				else {
					printC(B[i].x + 1, B[i].y + 1, 217, Borders); // Down-Right
				}

				// Up-Left
				if (B[i].sideOn[1] && B[i].sideOn[2]) {
					printC(B[i].x - 1, B[i].y - 1, 217, Borders); // Up-Left
				}
				else if (!B[i].sideOn[1] && B[i].sideOn[2]) {
					printC(B[i].x - 1, B[i].y - 1, 196, Borders); // Up-Left
				}
				else if (B[i].sideOn[1] && !B[i].sideOn[2]) {
					printC(B[i].x - 1, B[i].y - 1, 179, Borders); // Up-Left
				}
				else {
					printC(B[i].x - 1, B[i].y - 1, 218, Borders); // Up-Left
				}

				// Down-Left
				if (B[i].sideOn[3] && B[i].sideOn[2]) {
					printC(B[i].x - 1, B[i].y + 1, 191, Borders);
				}
				else if (!B[i].sideOn[3] && B[i].sideOn[2]) {
					printC(B[i].x - 1, B[i].y + 1, 196, Borders); // Up-Left
				}
				else if (B[i].sideOn[3] && !B[i].sideOn[2]) {
					printC(B[i].x - 1, B[i].y + 1, 179, Borders); // Up-Left
				}
				else {
					printC(B[i].x - 1, B[i].y + 1, 192, Borders); // Up-Left
				}

				// Right = 0 | Left = 2 | Down = 3 | Up = 1
				if (!B[i].sideOn[0]) {
					printC(B[i].x + 1, B[i].y, 179, Borders);
				}
				if (!B[i].sideOn[1]) {
					printC(B[i].x, B[i].y - 1, 196, Borders);
				}
				if (!B[i].sideOn[2]) {
					printC(B[i].x - 1, B[i].y, 179, Borders);
				}
				if (!B[i].sideOn[3]) {
					printC(B[i].x, B[i].y + 1, 196, Borders);
				}

			}
		}

		printC(B[22].x, B[22].y, especialFood, ConsoleColor::White);
		Food[240].Active = true;
		Food[240].Special = true;
		Food[240].x = B[22].x;
		Food[240].y = B[22].y;
		printC(B[23].x, B[23].y, especialFood, ConsoleColor::White);
		Food[241].Active = true;
		Food[241].Special = true;
		Food[241].x = B[23].x;
		Food[241].y = B[23].y;
		printC(B[64].x, B[64].y - 2, especialFood, ConsoleColor::White);
		Food[242].Active = true;
		Food[242].Special = true;
		Food[242].x = B[64].x;
		Food[242].y = B[64].y - 2;
		printC(B[61].x, B[61].y - 2, especialFood, ConsoleColor::White);
		Food[243].Active = true;
		Food[243].Special = true;
		Food[243].x = B[61].x;
		Food[243].y = B[61].y - 2;

		G[0].x = B[42].x - 1;
		G[0].y = B[42].y + 3;
		G[0].Color = ConsoleColor::Cyan;
		G[1].x = B[42].x + 1;
		G[1].y = B[42].y + 3;
		G[1].Color = ConsoleColor::Magenta;
		G[2].x = B[43].x;
		G[2].y = B[43].y + 3;
		G[2].Color = ConsoleColor::Red;
		G[3].x = B[43].x;
		G[3].y = B[43].y;
		G[3].Color = ConsoleColor::Green;
		G[0].initX = G[0].x;
		G[0].initY = G[0].y;
		G[1].initX = G[1].x;
		G[1].initY = G[1].y;
		G[2].initX = G[2].x;
		G[2].initY = G[2].y;
		G[3].initX = G[2].x;
		G[3].initY = G[2].y;
		G[0].Dir = 0;
		G[1].Dir = 2;
		G[2].Dir = 1;
		G[3].Dir = rand() % 4;
		G[0].initDir = 0;
		G[1].initDir = 2;
		G[2].initDir = 1;
		G[3].initDir = 1;

		for (int i = 0; i < 4; i++) {
			G[i].isGhost = true;
		}

		A.update();
		for (int i = 0; i < 4; i++) { G[i].update(); }

	} while (false);


	int g = maxFood - 1;
	for (int i = 0; i < maxCrossPoints; i++) {
		if (i != 40 && i != 42 && i != 43 && i != 41 && i != 35 &&
			i != 39 && i != 38 && i != 34 && i != 66 && i != 67 &&
			i != 36 && i != 37) {
			Food[g].Visible = true;
			Food[g].x = B[i].x;
			Food[g].y = B[i].y;
			Food[g].Active = true;
			g--;
		}

	}

	do {

		if (Console::KeyAvailable) {
			ConsoleKeyInfo tecla;
			tecla = Console::ReadKey(true);
			if (!Playing) { Score++; Playing = true; }
			if (tecla.Key == ConsoleKey::RightArrow) {
				if (A.Dir == 1 || A.Dir == 3) {
					A.nextDir = 0;
				}
				else if (A.Dir == 0) {
					A.nextDir = 0;
				}
				else {
					A.Dir = 0;
					A.nextDir = 0;
				}
			}
			else if (tecla.Key == ConsoleKey::LeftArrow) {
				if (A.Dir == 1 || A.Dir == 3) {
					A.nextDir = 2;
				}
				else if (A.Dir == 2) {
					A.nextDir = 2;
				}
				else {
					A.Dir = 2;
					A.nextDir = 2;
				}
			}
			else if (tecla.Key == ConsoleKey::DownArrow) {
				if (A.Dir == 2 || A.Dir == 0) {
					A.nextDir = 3;
				}
				else if (A.Dir == 3) {
					A.nextDir = 3;
				}
				else {
					A.Dir = 3;
					A.nextDir = 3;
				}
			}
			else if (tecla.Key == ConsoleKey::UpArrow) {
				if (A.Dir == 2 || A.Dir == 0) {
					A.nextDir = 1;
				}
				else if (A.Dir == 1) {
					A.nextDir = 1;
				}
				else {
					A.Dir = 1;
					A.nextDir = 1;
				}
			}
		}

		if (Playing) {
			A.move();
			for (int i = 0; i < 4; i++) { G[i].move(); }
		}

		if (int(A.x) < B[36].x - 1) {
			A.x = float(B[37].x) + 1;
			A.move();
		}
		else if (int(A.x) > B[37].x + 1) {
			A.x = float(B[36].x) - 1;
			A.move();
		}

		for (int i = 0; i < maxCrossPoints; i++) {
			if (B[i].Active) {
				if (int(A.x) == B[i].x && int(A.y) == B[i].y) {
					if (!B[i].sideOn[A.Dir] && !B[i].sideOn[A.nextDir]) {
						A.Dir = 4;
						A.nextDir = 4;
					}
					else {
						if (B[i].sideOn[A.nextDir]) {
							A.Dir = A.nextDir;
						}
					}
				}
			}
		}
		for (int i = 0; i < maxCrossPoints; i++) {
			if (B[i].Active) {
				for (int j = 0; j < 4; j++) {
					if (int(G[j].x) == B[i].x && int(G[j].y) == B[i].y) {
						if (!B[i].sideOn[G[j].Dir] && !B[i].sideOn[G[j].nextDir]) {
							G[j].Dir = 4;
							G[j].nextDir = 4;
						}
						else {
							if (B[i].sideOn[G[j].nextDir]) {
								G[j].Dir = G[j].nextDir;
							}
						}
						if (G[j].Dir == 4) {
							G[j].nextDir = rand() % 4;
						}
					}
					for (int h = 0; h < 4; h++) {
						if (h != j) {
							if (int(G[j].x) == int(G[h].x) && int(G[j].y) == int(G[h].y) && G[j].Dir == G[h].Dir) {
								if (G[h].nextDir + 2 > 3) {
									G[h].nextDir - 2;
								}
								else {
									G[h].nextDir + 2;
								}
							}
						}
					}
				}
			}
		}

		for (int j = 0; j < 4; j++) {
			if (G[j].Updated) {
				for (int i = 0; i < maxFood; i++) {
					if (Food[i].Active &&
						Food[i].x == int(G[j].x) &&
						Food[i].y == int(G[j].y)) {
						if (Food[i].Special) {
							G[j].oS = true;
						}
						else {
							if (Food[i].Visible) {
								G[j].oC = true;
							}
						}
					}
				}
			}
			if (int(G[j].x) == int(A.x) && int(G[j].y) == int(A.y)) {
				if (G[j].ghostDizzy) {
					Score += 50;
					G[j].x = G[j].initX;
					G[j].y = G[j].initY;
					G[j].ghostDizzy = false;
					G[j].Dir = G[j].initDir;
					G[j].nextDir = G[j].initDir;
				}
				else {
					printC(x + 10, y + 32, "GAME OVER", ConsoleColor::White);
					Timer = 0;
					while (true) {
						Timer += 1000 / fps;
						if (Timer >= 1000) {
							if (Console::KeyAvailable) {
								goto GAMEOVER;
							}
						}
						Thread::Sleep(1000 / fps);
					}
				}
			}
		}

		if (A.Updated) {
			for (int i = 0; i < maxFood; i++) {
				if (Food[i].Active && Food[i].x == int(A.x) && Food[i].y == int(A.y)) {
					Food[i].Active = false;
					if (Food[i].Special) {
						for (int j = 0; j < 4; j++) {
							G[j].ghostDizzy = true;
							if (G[j].Dir == 0) { G[j].Dir = 2; }
							else if (G[j].Dir == 1) { G[j].Dir = 3; }
							else if (G[j].Dir == 2) { G[j].Dir = 0; }
							else if (G[j].Dir == 3) { G[j].Dir = 1; }
						}
						ghostsDizzy = true;
						Score += 9;
					}
					else {
						Score++;
					}
				}
			}
		}
		if (ghostsDizzy) {
			Timer += 1000 / fps;
			if ((Timer / 1000) == DizzyTime) {
				for (int j = 0; j < 4; j++) {
					G[j].ghostDizzy = false;
					if (G[j].Dir == 0) { G[j].Dir = 2; }
					else if (G[j].Dir == 1) { G[j].Dir = 3; }
					else if (G[j].Dir == 2) { G[j].Dir = 0; }
					else if (G[j].Dir == 3) { G[j].Dir = 1; }
				}
				Timer = 0;
			}
		}

		printC(x, 1, "SCORE: ", int(Score), ConsoleColor::White);

		Thread::Sleep(1000 / fps);

	} while (true);
GAMEOVER:
	return 0;
}