//#include "common.h"
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <cstring>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <mmsystem.h>
#include <string.h>
#include <string>

using std::cin;
using std::cout;
using std::ifstream;
using std::min;
using std::ofstream;
using std::string;
using std::swap;

//COLOR
const int BLACK = 0;
const int WHITE = 7;
const int RED = 4;
const int GRAY = 8;
const int GREEN = 2;
const int YELLOW = 6;
const int MAX_COLOR_CODE = 15;

//KEY CHAR
const int KEY_UP = 'w';
const int KEY_DOWN = 's';
const int KEY_LEFT = 'a';
const int KEY_RIGHT = 'd';

//LIMITATION
const int MAX_PAS = 10;
const int MAX_ROW = 14;
const int MAX_COL = 20;
const int SPACE_BETWEEN_POINT = 2;
const int NAME_DISPLAY = 8;
const int COL_SIZE = 9;
const int ROW_SIZE = 5;
const int MAX_ACCOUNT = 100;
const int NOTICE_SIZE = 8;
const int WINDOWS_WIDTH = (COL_SIZE + 2) * (MAX_COL - 2) + NAME_DISPLAY - 1 + 1;
const int WINDOWS_HEGHT = ROW_SIZE * (MAX_ROW - 2) + 2;
int currentrow = 1;
int currentcol = 1;

bool isSingleMode = false;
HANDLE OutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE InHamdle = GetStdHandle(STD_INPUT_HANDLE);
COORD CursorPosition;
int Notice_Col = 0;

struct HighScore
{
	string name;
	int time;
};

HighScore people[6];
HighScore new_Result;

// struct Point 
// {
// 	int x;
// 	int y;
// };

struct Coordinate
{
	int Row = 0, Col = 0, Turn = 0;
	Coordinate(int row = 0, int col = 0, int t = 0)
	{
		Row = row;
		Col = col;
		Turn = t;
	}
};

struct CoordinateStack
{
	Coordinate Root[MAX_COL * MAX_ROW];
	int Size = 0;
	void push(Coordinate item)
	{
		Root[Size] = item;
		Size++;
	}
	Coordinate pop()
	{
		Size--;
		return (Root[Size]);
	}
	void clear()
	{
		Size = 0;
	}
	bool empty()
	{
		return !Size;
	}
};

void setColor(int backgound_color, int text_color)
{
	int color_code = backgound_color * 16 + text_color;
	SetConsoleTextAttribute(OutHandle, color_code);
}

void moveCursor(int row, int col)
{
	CursorPosition.X = col;
	CursorPosition.Y = row;
	SetConsoleCursorPosition(OutHandle, CursorPosition);
}

void showCursor(bool CursorVisibility)
{
	CONSOLE_CURSOR_INFO ConCurInf;

	ConCurInf.dwSize = 10;
	ConCurInf.bVisible = CursorVisibility;

	SetConsoleCursorInfo(OutHandle, &ConCurInf);
}

void changeWindows(int height, int width)
{
	SMALL_RECT WindowSize;
	WindowSize.Top = 0;
	WindowSize.Left = 0;
	WindowSize.Right = width;
	WindowSize.Bottom = height;
	COORD NewSize;
	NewSize.X = WindowSize.Right;
	NewSize.Y = WindowSize.Bottom;

	//change buffer size
	SetConsoleScreenBufferSize(OutHandle, NewSize);
	WindowSize.Right = width - 1;
	WindowSize.Bottom = height - 1;
	//change windows size
	SetConsoleWindowInfo(OutHandle, 1, &WindowSize);

	auto hWnd = GetConsoleWindow();
	HMENU hMenu = GetSystemMenu(hWnd, false);

	//disable changing windows and buffer size
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
	DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
	ShowScrollBar(hWnd, SB_BOTH, false);
}

void inputCharArray(char c[], const int max, char hide = '\0')
{
	char x;
	int i = strlen(c);
	if (hide == '\0')
		cout << c;
	else
	{
		for (int j = 0; j < i; ++j)
			cout << hide;
	}
	showCursor(true);
	do
	{
		x = _getch();
		if (x == '\n' || x == '\r' || x == '\0')
		{
			if (i <= 0) continue;
			cout << '\n';
			break;
		}
		if (x == '\b' && i > 0)
		{
			cout << x;
			cout << ' ';
			cout << x;
			i--;
		}
		else if (x != '\b' && i < max - 1)
		{
			if (hide == '\0')
				cout << x;
			else
				cout << hide;
			c[i] = x;
			i++;
		}
	} while ((x != '\n' && x != '\r' && x != '\0') || i <= 0);
	if (i > 0)
		c[i] = '\0';
	showCursor(false);
}

int printName(string filetext)
{
	//change windows size

	changeWindows(40, 90);
	int i = 5;
	const int col = 7;

	ifstream file;
	file.open(filetext);
	string str;
	moveCursor(i, col);
	setColor(BLACK, 11);
	while (getline(file, str))
	{

		i++;
		moveCursor(i, col);
		cout << str;
	}
	file.close();
	setColor(BLACK, WHITE);

	return i;

}

int printbackground(string filetext)
{
	//change windows size

	changeWindows(40, 90);
	int i = 1;
	// const int col = 7;
	system("cls");
	ifstream file;
	file.open(filetext);
	string str;
	moveCursor(i, 3);
	setColor(BLACK, 11);
	while (getline(file, str))
	{

		i++;
		moveCursor(i, 3);
		cout << str;
	}
	file.close();
	setColor(BLACK, WHITE);

	return i;

}

int printGameModeSelection()
{
	system("cls");
	showCursor(false);
	string filetext = "Name.txt";
	int LastRow = printName(filetext) + 6;
	const int col = 36;
	int pos = 0;
	int pre = 0;
	moveCursor(LastRow, col - 19);
	cout << "Please only use English Typing for the best experience";
	LastRow++;
	moveCursor(LastRow, col - 7);
	cout << "Use W,A,S,D to move around";
	LastRow++;
	moveCursor(LastRow, col - 15);
	cout << "Use ENTER or SPACE to select, edit and confirm";
	LastRow += 2;

	moveCursor(LastRow, col);
	setColor(GREEN, BLACK);
	cout << " Play Game    ";
	setColor(GRAY, BLACK);
	moveCursor(LastRow + 2, col);
	cout << " Leader Board ";
	moveCursor(LastRow + 4, col);
	cout << " Exit         ";
	while (true)
	{
		char c = _getch();
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		{
			pre = pos;
			switch (c)
			{
			case 'W':
			case 'w':
				if (pos == 0)
					pos = 3;
				pos = abs((pos - 1) % 3);
				break;
			case 'S':
			case 's':
				pos = abs((pos + 1) % 3);
				break;
			}
		}
		else if (c == '\n' || c == '\r' || c == ' ')
		{
			setColor(BLACK, WHITE);
			return pos;
		}
		else
		{
			c = _getch();
			switch (c)
			{
			case KEY_UP:
				pre = pos;
				if (pos == 0)
					pos = 3;
				pos = abs((pos - 1) % 3);
				break;
			case KEY_DOWN:
				pre = pos;
				pos = abs((pos + 1) % 3);
				break;
			}
		}
		moveCursor(LastRow + pre * 2, col);
		setColor(GRAY, BLACK);
		switch (pre)
		{
		case 0:
			cout << " Play Game    ";
			break;
		case 1:
			cout << " Leader Board ";
			break;
		case 2:
			cout << " Exit         ";
			break;
		}
		moveCursor(LastRow + pos * 2, col);
		setColor(GREEN, BLACK);
		switch (pos)
		{
		case 0:
			cout << " Play Game    ";
			break;
		case 1:
			cout << " Leader Board ";
			break;
		case 2:
			cout << " Exit         ";
			break;
		}
		setColor(BLACK, WHITE);
	}
}

string printlogin() {

	setColor(BLACK, WHITE);
	system("cls");
	string filetext = "Name.txt";
	int LastRow = printName(filetext) + 6;
	const int col = 28;

	char Username[NAME_DISPLAY + 1];
	moveCursor(LastRow, col);
	cout << "Username:  ";
	Username[0] = '\0';
	inputCharArray(Username, NAME_DISPLAY + 1);
	return Username;
}


// void moveTo(int addrow, int addcol, int* currentrow, int* currentcol,int size){
// 	int descol = *currentcol + addcol;
// 	int desrow = *currentrow + addrow;
// 	if(desrow <=1 || descol <=1 || desrow >= size || descol >=size )
// 		return;
// 	currentcol = &descol;
// 	cout << *currentcol;
// 	currentrow = &desrow;
// 	cout << *currentrow;
// }

int random(int minx, int maxx) {

	return minx + rand() % (maxx + 1 - minx);
}
void randomlocation(int**& mang, int size, int* count) {
	for (int i = 1; i < size - 1; i++)
		for (int j = 1; j < size - 1; j++) {
			if (mang[i][j] != 32) {
				int tempi = random(1, size);
				int tempj = random(1, size);
				if (mang[tempi][tempj] != 32) {
					int temp = mang[tempi][tempj];
					mang[tempi][tempj] = mang[i][j];
					mang[i][j] = temp;
				}
			}
		}
	*count = *count - 1;
}

void printboard(int**& mang, int sizeofBoard) {
	setColor(BLACK, WHITE);
	for (int i = 0; i < sizeofBoard; i++)
		for (int j = 0; j < sizeofBoard; j++) {
			if (mang[i][j] != 32) {
				moveCursor(i * 3 - 1, j * 6 - 3);
				cout << "      ";
				// moveCursor(i*3-2,j*6-2);
				// cout <<"      ";
				moveCursor(i * 3 - 1, j * 6 + 3);
				cout << "      ";
				moveCursor(i * 3, j * 6 - 3);
				cout << "      ";
				moveCursor(i * 3 + 1, j * 6 - 3);
				cout << "      ";
				moveCursor(i * 3 + 1, j * 6 - 2);
				cout << "      ";
				moveCursor(i * 3 + 1, j * 6 + 3);
				cout << "      ";
				moveCursor(i * 3, j * 6);

				moveCursor(i * 3 - 1, j * 6 - 3);
				cout << "|";
				moveCursor(i * 3 - 2, j * 6 - 2);
				cout << "_____";
				moveCursor(i * 3 - 1, j * 6 + 3);
				cout << "|";
				moveCursor(i * 3, j * 6 - 3);
				cout << "|";
				moveCursor(i * 3 + 1, j * 6 - 3);
				cout << "|";
				moveCursor(i * 3 + 1, j * 6 - 2);
				cout << "_____";
				moveCursor(i * 3 + 1, j * 6 + 3);
				cout << "|";
				moveCursor(i * 3, j * 6 + 3);
				cout << "|";
				moveCursor(i * 3, j * 6);
				char temp = mang[i][j];
				cout << temp;
			}

		}
}

int findMax(int a, int b)
{
	int max;
	if (a < b)
		max = b;
	else
		max = a;

	return max;
}

int findMin(int a, int b)
{
	int min;
	if (a > b)
		min = b;
	else
		min = a;

	return min;
}

bool CheckLineY(int** mang, int x1, int x2, int y)
{
	int min = findMin(x1, x2) + 1;
	int max = findMax(x1, x2);

	bool valid;

	int x = min;
	while (x < max)
	{
		if (mang[x][y] != 32)
		{
			return false;
		}
		x++;
	}

	return true;
}

bool CheckLineX(int** mang, int y1, int y2, int x)
{
	int min = findMin(y1, y2) + 1;
	int max = findMax(y1, y2);

	bool valid;

	int y = min;
	while (y < max)
	{
		if (mang[x][y] != 32)
		{
			return false;
		}
		y++;
	}

	return true;
}

bool I_matching(int** mang, int lockcol, int lockrow, int currentcol, int currentrow)
{
	if (mang[lockcol][lockrow] != mang[currentcol][currentrow])
		return false;
	if (lockrow == currentrow)
	{
		if (CheckLineY(mang, lockcol, currentcol, lockrow) == true)
			return true;
		else
			return false;
	}
	else if (lockcol == currentcol)
	{
		if (CheckLineX(mang, lockrow, currentrow, lockcol) == true)
			return true;
		else
			return false;
	}
	else
		return false;
}

bool L_matching(int** mang, int lockcol, int lockrow, int currentcol, int currentrow) {
	if (mang[lockcol][lockrow] != mang[currentcol][currentrow])
		return false;
	if (mang[lockcol][currentrow] == 32 && CheckLineX(mang, currentrow, lockrow, lockcol) && CheckLineY(mang, lockcol, currentcol, currentrow))
		return true;
	else if (mang[currentcol][lockrow] == 32 && CheckLineX(mang, lockrow, currentrow, currentcol) && CheckLineY(mang, currentcol, lockcol, lockrow))
		return true;
	else
		return false;
}

bool checkRectX(int** mang, int x1, int x2, int y1, int y2) {
	int minY = findMin(y1, y2);
	int maxY = findMax(y1, y2);
	int minX = findMin(x1, x2);
	int maxX = findMax(x1, x2);

	for (int y = minY + 1; y < maxY; y++) {
		if (mang[minX][y] != 32)
			return false;
		if (CheckLineY(mang, minX, maxX + 1, y) && CheckLineX(mang, y - 1, maxY, maxX))
			return true;
	}
	return false;
}

bool checkRectY(int** mang, int x1, int x2, int y1, int y2) {
	int minY = findMin(y1, y2);
	int maxY = findMax(y1, y2);
	int minX = findMin(x1, x2);
	int maxX = findMax(x1, x2);

	for (int x = minX + 1; x < maxX; x++) {
		if (mang[x][minY] != 32)
			return false;
		if (CheckLineY(mang, x - 1, maxX, maxY) && CheckLineX(mang, minY, maxY + 1, x))
			return true;
	}
	return false;
}

bool Z_matching(int** mang, int lockcol, int lockrow, int currentcol, int currentrow) {
	if (mang[lockcol][lockrow] != mang[currentcol][currentrow])
		return false;

	return checkRectX(mang, lockcol, currentcol, lockrow, currentrow) || checkRectY(mang, lockcol, currentcol, lockrow, currentrow);
}

bool checkX(int** mang, int x1, int x2, int y1, int y2, int size) {
	int minX = findMin(x1, x2);
	int maxX = findMax(x1, x2);

	for (int y = 0; y <= size; y++) {
		if (y == y1)
			y++;

		if (y < y1 && CheckLineX(mang, y - 1, y1, x1) && CheckLineY(mang, minX - 1, maxX + 1, y) && CheckLineX(mang, y - 1, y2, x2))
			return true;

		if (y > y1 && CheckLineX(mang, y + 1, y1, x1) && CheckLineY(mang, minX - 1, maxX + 1, y) && CheckLineX(mang, y + 1, y2, x2))
			return true;
	}
	return false;
}

bool checkY(int** mang, int x1, int x2, int y1, int y2, int size) {
	int minY = findMin(y1, y2);
	int maxY = findMax(y1, y2);

	for (int x = 0; x <= size; x++) {
		if (x == x1)
			x++;

		if (x < x1 && CheckLineY(mang, x - 1, x1, y1) && CheckLineX(mang, minY - 1, maxY + 1, x) && CheckLineY(mang, x - 1, x2, y2))
			return true;

		if (x > x1 && CheckLineY(mang, x + 1, x1, y1) && CheckLineX(mang, minY - 1, maxY + 1, x) && CheckLineY(mang, x + 1, x2, y2))
			return true;
	}
	return false;
}

bool U_matching(int** mang, int lockcol, int lockrow, int currentcol, int currentrow, int size) {
	if (mang[lockcol][lockrow] != mang[currentcol][currentrow])
		return false;

	return checkX(mang, lockcol, currentcol, lockrow, currentrow, size) || checkY(mang, lockcol, currentcol, lockrow, currentrow, size);
}


bool checkgamewin(int** mang, int sizeofboard) {
	for (int i = 0; i < sizeofboard; i++) {
		for (int j = 0; j < sizeofboard; j++) {
			if (mang[i][j] != 32)
				return false;
		}
	}
	return true;
}

bool checkmove(int** mang, int sizeofboard, int type) {
	for (int i = 0; i < sizeofboard; i++) {
		for (int j = 0; j < sizeofboard; j++) {
			for (int i2 = 1; i2 < sizeofboard; i2++) {
				for (int j2 = 1; j2 < sizeofboard; j2++) {
					if (mang[i][j] == mang[i2][j2] && mang[i][j] != 32 && (i != i2 || j != j2)) {
						if (I_matching(mang, i, j, i2, j2) || L_matching(mang, i, j, i2, j2) || Z_matching(mang, i, j, i2, j2) || U_matching(mang, i, j, i2, j2, sizeofboard - 1)) {
							if (type == 1) {
								moveCursor(i * 3, j * 6);
								setColor(11, YELLOW);
								char kytu = mang[i][j];
								cout << kytu;
								moveCursor(i2 * 3, j2 * 6);
								kytu = mang[i2][j2];
								cout << kytu;
							}
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool board(int size) {
	int count = 9;

	clock_t start, end;
	double time_use;
	start = clock();

	srand((int)time(0));
	int** mang;
	int sizeofBoard = size + 2;
	mang = new int* [sizeofBoard];
	for (int i = 0; i < sizeofBoard; i++)
		mang[i] = new int[sizeofBoard];


	for (int i = 0; i < sizeofBoard; i++) {
		mang[0][i] = 32;
		mang[sizeofBoard - 1][i] = 32;
	}
	for (int j = 0; j < sizeofBoard; j++) {
		mang[j][0] = 32;
		mang[j][sizeofBoard - 1] = 32;
	}

	for (int i = 1; i <= size; i++)
		for (int j = 1; j <= size / 2; j++) {
			mang[i][j] = random(65, 90);
			mang[i][size - j + 1] = mang[i][j];
		}
	for (int i = 0; i < size / 2; i++)
		randomlocation(mang, size, &count);

	int* contro;
	int lockcol = 0;
	int lockrow = 0;

	while (true) {
		setColor(BLACK, WHITE);
		printbackground("art_background.txt");
		printboard(mang, sizeofBoard);


		moveCursor(size * 3 + 5, 5);
		cout << "Press R to random the board, you have ";
		setColor(BLACK, GREEN);
		cout << count;
		setColor(BLACK, WHITE);
		cout << " times";


		moveCursor(size * 3 + 7, 5);
		cout << "Press M to random the board";

		moveCursor(size * 3 + 9, 5);
		cout << "Press Esc to escape";
		//in ô đang lock
		if (lockcol != 0 && lockrow != 0) {
			// cout << lockcol<<lockrow;
			moveCursor(lockcol * 3, lockrow * 6);
			setColor(GREEN, RED);
			char kytu = mang[lockcol][lockrow];
			cout << kytu;
		}

		//in ô đang trỏ vào
		moveCursor(currentcol * 3, currentrow * 6);
		setColor(GREEN, WHITE);
		char kytu = mang[currentcol][currentrow];
		cout << kytu;

		char c;
		c = _getch();
		switch (c)
		{
		case KEY_UP:
			if (currentcol > 1)
				currentcol--;
			break;
		case KEY_DOWN:
			if (currentcol < size)
				currentcol++;
			break;
		case KEY_LEFT:
			if (currentrow > 1)
				currentrow--;
			break;
		case KEY_RIGHT:
			if (currentrow < size)
				currentrow++;
			break;
		}

		if (c == '\n' || c == '\r' || c == ' ') {
			if (lockcol == 0 && lockrow == 0 && mang[currentcol][currentrow] != 32) {
				lockcol = currentcol;
				lockrow = currentrow;
			}
			else if (lockcol == currentcol && lockrow == currentrow) {
				lockcol = 0;
				lockrow = 0;
			}
			else if (I_matching(mang, lockcol, lockrow, currentcol, currentrow) || L_matching(mang, lockcol, lockrow, currentcol, currentrow) || Z_matching(mang, lockcol, lockrow, currentcol, currentrow) || U_matching(mang, lockcol, lockrow, currentcol, currentrow, sizeofBoard - 1)) {
				mang[lockcol][lockrow] = 32;
				mang[currentcol][currentrow] = 32;
				lockcol = 0;
				lockrow = 0;
			}
			else {
				lockcol = 0;
				lockrow = 0;
			}
			// contro = &mang[currentcol][currentrow];
			// cout << *contro;



		}
		if (c == 'R' || c == 'r')
			randomlocation(mang, size, &count);

		if (c == 'm' || c == 'M') {
			if (checkmove(mang, sizeofBoard, 1) == true)
				Sleep(500);
		}

		if (c == 27)
			return false;

		if (count <= 0) {
			setColor(BLACK, WHITE);
			system("cls");
			printName("lose.txt");
			Sleep(5000);
			return false;
		}

		if (checkgamewin(mang, sizeofBoard)) {
			end = clock();
			time_use = (double)(end - start) / CLOCKS_PER_SEC;
			new_Result.time = time_use;
			setColor(BLACK, WHITE);
			system("cls");
			int lastrow = printName("win.txt");
			moveCursor(lastrow + 3, 10);
			Sleep(5000);
			return true;
		}
		if (checkmove(mang, sizeofBoard, 0) == false)
			randomlocation(mang, size, &count);


	}

}

void SortList(HighScore people[])
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = i + 1; j < 6; j++)
		{
			if (people[i].time > people[j].time)
			{
				swap(people[i], people[j]);
			}
		}
	}
}

void UpdateLeaderBoard(HighScore new_Result, HighScore people[])
{
	ifstream old_List; // MO RA DANH SACH CU DE LUU THONG TIN

	char TIME[5][100];
	string NAME[5];

	old_List.open("LeaderBoard.txt");
	for (int i = 0; i < 5; i++)
	{
		getline(old_List, NAME[i], '/');
		old_List.getline(TIME[i], 20);
	}

	for (int i = 0; i < 6; i++)
	{
		if (i < 5)
		{
			people[i].name = NAME[i];
			people[i].time = atoi(TIME[i]);
		}
		else if (i == 5)
		{
			people[i].name = new_Result.name;
			people[i].time = new_Result.time;
		}
	}
	old_List.close();

	SortList(people);

	ofstream new_List;
	new_List.open("LeaderBoard.txt");
	for (int i = 0; i < 5; i++)
	{
		new_List << people[i].name << "/" << people[i].time << "\n";
	}
	new_List.close();
}


void ShowLeaderBoard()
{
	setColor(BLACK, WHITE);
	system("cls");
	printName("boardname.txt");
	string TEN[5], THOIGIAN[5];

	ifstream file_Leader("LeaderBoard.txt");
	for (int i = 0; i < 5; i++)
	{
		getline(file_Leader, TEN[i], '/');
		getline(file_Leader, THOIGIAN[i], '\n');
	}

	file_Leader.close();

	int LastRow = printName("boardname.txt") + 6;
	const int col = 36;
	moveCursor(LastRow, col - 19);
	cout << "RANK";
	moveCursor(LastRow, col);
	cout << "NAME";
	moveCursor(LastRow, col + 31);
	cout << "TIME";

	for (int i = 0; i < 5; i++)
	{
		moveCursor(LastRow + i * 2 + 2, col - 19);
		cout << i + 1;
		moveCursor(LastRow + i * 2 + 2, col);
		cout << TEN[i];
		moveCursor(LastRow + i * 2 + 2, col + 31);
		cout << THOIGIAN[i];
	}
	LastRow++;
	moveCursor(LastRow + 20, col - 8);
	cout << "Press any key to countinue";
	char out;
	out = _getch();


}

int main()
{
	SetConsoleTitle(TEXT("Pikachu Game"));
	SetConsoleMode(InHamdle, ~ENABLE_QUICK_EDIT_MODE);
	string name;
	//	loadAccounts();
	while (true) {
		int b = printGameModeSelection();
		switch (b) {
		case 0: {
			name = printlogin();
			new_Result.name = name;
			if (board(8)) {
				UpdateLeaderBoard(new_Result, people);
			}
		}
		case 1: {
			ShowLeaderBoard();
			break;
		}
		default:
			exit(2);
		}
	}

	return 0;

}
