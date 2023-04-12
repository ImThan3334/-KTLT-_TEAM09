#include <iostream>
#include <windows.h>
#include <conio.h>
#include <mmsystem.h>
#include <vector>
#include <time.h>

using namespace std;
#define _BOARD_SIZE 12
#define _LEFT 10
#define _TOP 5


#define trentrai 201
#define ngang 205
#define	trenphai 187
#define trenngang 203
#define doc 186
#define doctrai 204 
#define	docphai 185
#define docngang 206
#define duoitrai 200
#define duoiphai 188
#define duoingnang 202


struct _POINT { int x, y, c; };
_POINT LastMove;
_POINT _A[_BOARD_SIZE][_BOARD_SIZE];
bool _TURN;
int _COMMAND;
int _X, _Y;
int _loop;
int _count; //Đếm số nước đã đi để tính hòa
HANDLE hConsoleColor;
clock_t start;

void sleep(clock_t wait)
{
	clock_t goal;
	goal = wait + clock();
	while (goal > clock());
}

int DemSoXungQuanh()
{
	int i = 1;
	int a = 1, b = 1, c = 1, d = 1;
	int dong = (_Y - 1 - _TOP) / 2, cot = (_X - 2 - _LEFT) / 4;

	while (_A[dong][cot].c == _A[dong + i][cot].c)
	{
		if (_A[dong + i][cot].c == 0) break;
		a++; i++;
	} i = 1;
	while (_A[dong][cot].c == _A[dong - i][cot].c)
	{
		if (_A[dong - i][cot].c == 0) break;
		a++; i++;
	}i = 1;

	while (_A[dong][cot].c == _A[dong][cot + i].c)
	{
		if (_A[dong][cot + i].c == 0) break;
		b++; i++;
	}i = 1;
	while (_A[dong][cot].c == _A[dong][cot - i].c)
	{
		if (_A[dong][cot - i].c == 0) break;
		b++; i++;
	}i = 1;

	while (_A[dong][cot].c == _A[dong + i][cot + i].c)
	{
		if (_A[dong + i][cot + i].c == 0) break;
		c++; i++;
	}i = 1;
	while (_A[dong][cot].c == _A[dong - i][cot - i].c)
	{
		if (_A[dong - i][cot - i].c == 0) break;
		c++; i++;
	}i = 1;

	while (_A[dong][cot].c == _A[dong + i][cot - i].c)
	{
		if (_A[dong + i][cot - i].c == 0) break;
		d++; i++;
	}i = 1;
	while (_A[dong][cot].c == _A[dong - i][cot + i].c)
	{
		if (_A[dong - i][cot + i].c == 0) break;
		d++; i++;
	}i = 1;

	return max(a, max(b, max(c, d)));
}

bool WinnerCheck()
{
	if (DemSoXungQuanh() >= 5) return true;
	else return false;
}

void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void TextColor(int x) //X là mã màu
{
	//Các hàm này là hàm thao tác API với windows bạn cứ coppy thôi không cần phải hiểu quá sâu
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, x);
}

int countCol()
{
	int i = 1;
	int a = 1;
	int dong = (_Y - 1 - _TOP) / 2, cot = (_X - 2 - _LEFT) / 4;

	while (_A[dong][cot].c == _A[dong + i][cot].c)
	{
		if (_A[dong + i][cot].c == 0) break;
		a++; i++;
	} i = 1;
	while (_A[dong][cot].c == _A[dong - i][cot].c)
	{
		if (_A[dong - i][cot].c == 0) break;
		a++; i++;
	}

	return a;

}
int countRow()
{
	int i = 1;
	int a = 1;
	int dong = (_Y - 1 - _TOP) / 2, cot = (_X - 2 - _LEFT) / 4;

	while (_A[dong][cot].c == _A[dong][cot + i].c)
	{
		if (_A[dong][cot + i].c == 0) break;
		a++; i++;
	}i = 1;
	while (_A[dong][cot].c == _A[dong][cot - i].c)
	{
		if (_A[dong][cot - i].c == 0) break;
		a++; i++;
	}
	return a;
}
int countHuyen()
{
	int i = 1;
	int a = 1;
	int dong = (_Y - 1 - _TOP) / 2, cot = (_X - 2 - _LEFT) / 4;

	while (_A[dong][cot].c == _A[dong + i][cot + i].c)
	{
		if (_A[dong + i][cot + i].c == 0) break;
		a++; i++;
	}i = 1;
	while (_A[dong][cot].c == _A[dong - i][cot - i].c)
	{
		if (_A[dong - i][cot - i].c == 0) break;
		a++; i++;
	}
	return a;
}
int countSac()
{
	int i = 1;
	int a = 1;
	int dong = (_Y - 1 - _TOP) / 2, cot = (_X - 2 - _LEFT) / 4;

	while (_A[dong][cot].c == _A[dong + i][cot - i].c)
	{
		if (_A[dong + i][cot - i].c == 0) break;
		a++; i++;
	}i = 1;
	while (_A[dong][cot].c == _A[dong - i][cot + i].c)
	{
		if (_A[dong - i][cot + i].c == 0) break;
		a++; i++;
	}
	return a;
}

void changeColor(int x, int y, int a, int b) //Đổi màu ô x,y sang màu a, sau đó trả về màu b
{
	GotoXY(x, y);
	TextColor(a);
	for (int i = 0; i < _BOARD_SIZE; i++)
		for (int j = 0; j < _BOARD_SIZE; j++)
			if (_A[i][j].x == x && _A[i][j].y == y) {
				if (_A[i][j].c == -1)
					printf("X");
				else if (_A[i][j].c == 1) printf("O");
			}
	TextColor(b);
}

void winLine_on()
{
	int i = 1;
	int dong = (_Y - 1 - _TOP) / 2, cot = (_X - 2 - _LEFT) / 4;
	changeColor(_X, _Y, 248, 245);

	if (countCol() >= 5)
	{
		while (_A[dong][cot].c == _A[dong + i][cot].c)
		{
			if (_A[dong + i][cot].c == 0) break;
			changeColor(_A[dong + i][cot].x, _A[dong + i][cot].y, 248, 245);
			i++;
		} i = 1;
		while (_A[dong][cot].c == _A[dong - i][cot].c)
		{
			if (_A[dong - i][cot].c == 0) break;
			changeColor(_A[dong - i][cot].x, _A[dong - i][cot].y, 248, 245);
			i++;
		}
	}
	if (countRow() >= 5)
	{
		while (_A[dong][cot].c == _A[dong][cot + i].c)
		{
			if (_A[dong][cot + i].c == 0) break;
			changeColor(_A[dong][cot + i].x, _A[dong][cot + i].y, 248, 245);
			i++;
		}i = 1;
		while (_A[dong][cot].c == _A[dong][cot - i].c)
		{
			if (_A[dong][cot - i].c == 0) break;
			changeColor(_A[dong][cot - i].x, _A[dong][cot - i].y, 248, 245);
			i++;
		}
	}
	if (countSac() >= 5)
	{
		while (_A[dong][cot].c == _A[dong + i][cot - i].c)
		{
			if (_A[dong + i][cot - i].c == 0) break;
			changeColor(_A[dong + i][cot - i].x, _A[dong + i][cot - i].y, 248, 245);
			i++;
		}i = 1;
		while (_A[dong][cot].c == _A[dong - i][cot + i].c)
		{
			if (_A[dong - i][cot + i].c == 0) break;
			changeColor(_A[dong - i][cot + i].x, _A[dong - i][cot + i].y, 248, 245);
			i++;
		}
	}

	if (countHuyen() >= 5)
	{
		while (_A[dong][cot].c == _A[dong + i][cot + i].c)
		{
			if (_A[dong + i][cot + i].c == 0) break;
			changeColor(_A[dong + i][cot + i].x, _A[dong + i][cot + i].y, 248, 245);
			i++;
		}i = 1;
		while (_A[dong][cot].c == _A[dong - i][cot - i].c)
		{
			if (_A[dong - i][cot - i].c == 0) break;
			changeColor(_A[dong - i][cot - i].x, _A[dong - i][cot - i].y, 248, 245);
			i++;
		}

	}



}

void winLine_off()
{
	int i = 1;
	int dong = (_Y - 1 - _TOP) / 2, cot = (_X - 2 - _LEFT) / 4;

	changeColor(_X, _Y, 245, 245);
	if (countCol() >= 5)
	{
		while (_A[dong][cot].c == _A[dong + i][cot].c)
		{
			if (_A[dong + i][cot].c == 0) break;
			changeColor(_A[dong + i][cot].x, _A[dong + i][cot].y, 245, 245);
			i++;
		} i = 1;
		while (_A[dong][cot].c == _A[dong - i][cot].c)
		{
			if (_A[dong - i][cot].c == 0) break;
			changeColor(_A[dong - i][cot].x, _A[dong - i][cot].y, 245, 245);
			i++;
		}
	}
	if (countRow() >= 5)
	{
		while (_A[dong][cot].c == _A[dong][cot + i].c)
		{
			if (_A[dong][cot + i].c == 0) break;
			changeColor(_A[dong][cot + i].x, _A[dong][cot + i].y, 245, 245);
			i++;
		}i = 1;
		while (_A[dong][cot].c == _A[dong][cot - i].c)
		{
			if (_A[dong][cot - i].c == 0) break;
			changeColor(_A[dong][cot - i].x, _A[dong][cot - i].y, 245, 245);
			i++;
		}
	}

	if (countSac() >= 5)
	{
		while (_A[dong][cot].c == _A[dong + i][cot - i].c)
		{
			if (_A[dong + i][cot - i].c == 0) break;
			changeColor(_A[dong + i][cot - i].x, _A[dong + i][cot - i].y, 245, 245);
			i++;
		}i = 1;
		while (_A[dong][cot].c == _A[dong - i][cot + i].c)
		{
			if (_A[dong - i][cot + i].c == 0) break;
			changeColor(_A[dong - i][cot + i].x, _A[dong - i][cot + i].y, 245, 245);
			i++;
		}
	}

	if (countHuyen() >= 5)
	{
		while (_A[dong][cot].c == _A[dong + i][cot + i].c)
		{
			if (_A[dong + i][cot + i].c == 0) break;
			changeColor(_A[dong + i][cot + i].x, _A[dong + i][cot + i].y, 245, 245);
			i++;
		}i = 1;
		while (_A[dong][cot].c == _A[dong - i][cot - i].c)
		{
			if (_A[dong - i][cot - i].c == 0) break;
			changeColor(_A[dong - i][cot - i].x, _A[dong - i][cot - i].y, 245, 245);
			i++;
		}

	}

}

void winLineBlink()
{
	while (1) {
		winLine_on();
		sleep(100);
		winLine_off();
		sleep(100);
		if (_kbhit()) break;
	}
}

void winEffect()
{
	winLineBlink();
}

//Hàm khởi tạo dữ liệu mặc định ban đầu cho ma trận bàn cờ (hàm nhóm Model)
void ResetData()
{
	for (int i = 0; i < _BOARD_SIZE; i++) {
		for (int j = 0; j < _BOARD_SIZE; j++) {
			_A[i][j].x = 4 * j + _LEFT + 2;
			_A[i][j].y = 2 * i + _TOP + 1; //Có thay đổi
			_A[i][j].c = 0; // 0 nghĩa là chưa ai đánh dấu, nếu đánh dấu phải theo quy
			//định như sau: -1 là lượt true đánh, 1 là lượt false đánh
		}
	}
	_TURN = true; _COMMAND = -1; // Gán lượt và phím mặc định
	_X = _A[0][0].x; _Y = _A[0][0].y; // Thiết lập lại tọa độ hiện hành ban đầu

}

void TakeBack()
{
	for (int i = 0; i < _BOARD_SIZE; i++)
		for (int j = 0; j < _BOARD_SIZE; j++) {
			if (_A[i][j].x == LastMove.x && _A[i][j].y == LastMove.y)
				if (_A[i][j].c != 0) {
					_A[i][j].c = 0;
					GotoXY(_X, _Y);
					printf(" ");
					_TURN = !_TURN;
					_count--;
					_X = LastMove.x;
					_Y = LastMove.y;
					GotoXY(_X, _Y); printf(" "); GotoXY(_X, _Y);//Xóa dấu đã đánh 
				}
		}

}

void HieuUngBanCo() {
	GotoXY(_LEFT, _TOP);
	printf("%c", 218);
	for (int i = 0; i < _BOARD_SIZE * 4; i++)
	{
		printf("%c", 196);
		Sleep(10);
	}
	GotoXY(_LEFT + _BOARD_SIZE * 4, _TOP); printf("%c", 191);


	for (int i = 1; i < _BOARD_SIZE * 2; i++)
	{
		GotoXY(_LEFT + _BOARD_SIZE * 4, i + _TOP);
		printf("%c", 179);
		Sleep(20);
	}
	GotoXY(_LEFT + _BOARD_SIZE * 4, _BOARD_SIZE * 2 + _TOP); printf("%c", 217);

	for (int i = _BOARD_SIZE * 4 + _LEFT - 1; i >= _LEFT; i--)
	{
		GotoXY(i, _BOARD_SIZE * 2 + _TOP);
		printf("%c", 196);
		Sleep(10);
	}
	GotoXY(_LEFT, _BOARD_SIZE * 2 + _TOP); printf("%c", 192);

	for (int i = _BOARD_SIZE * 2 - 1 + _TOP; i > _TOP; i--)
	{
		GotoXY(_LEFT, i);
		printf("%c", 179);
		Sleep(20);
	}
}

void DrawBoard(int n) {

	//	HieuUngBanCo();

	GotoXY(_LEFT, _TOP);
	printf("%c%c%c%c", trentrai, ngang, ngang, ngang);
	for (int i = 0; i < n - 1; i++)
		printf("%c%c%c%c", trenngang, ngang, ngang, ngang);
	printf("%c", trenphai);

	for (int i = 0; i < n * 2; i += 2)
	{
		GotoXY(_LEFT, _TOP + i + 1);
		printf("%c", doc);
		for (int j = 0; j < n; j++) { printf("   "); printf("%c", doc); };
	}

	for (int i = 0; i < n - 1; i++) { GotoXY(_LEFT, 2 + _TOP + 2 * i); printf("%c%c%c%c", doctrai, ngang, ngang, ngang); for (int j = 0; j < n - 1; j++) printf("%c%c%c%c", docngang, ngang, ngang, ngang); printf("%c", docphai); }

	GotoXY(_LEFT, _TOP + (n * 2));
	printf("%c%c%c%c", duoitrai, ngang, ngang, ngang); for (int i = 0; i < n - 1; i++) printf("%c%c%c%c", duoingnang, ngang, ngang, ngang); printf("%c\n", duoiphai);
}

void drawGameTable(int width, int height, int x, int y) {
	GotoXY(x, y);
	printf("%c", trentrai); for (int i = 0; i < width; i++) printf("%c", ngang); printf("%c", trenphai);

	for (int i = 0; i < height - 1; i++) { GotoXY(x + width + 1, y + i + 1), printf("%c", doc); }

	for (int i = 0; i < height; i++) { GotoXY(x, y + 1 + i); printf("%c", doc); }

	GotoXY(x, y + height); printf("%c", duoitrai);

	for (int i = 0; i < width; i++) { printf("%c", ngang); }
	printf("%c", duoiphai);
}

void drawX() {
	//line 1
	GotoXY(69, 6);
	TextColor(17);
	cout << "####";
	TextColor(255);
	cout << "      ";
	TextColor(17);
	cout << "####" << endl;

	//line 2
	GotoXY(69, 7);
	TextColor(17);
	cout << "##";
	TextColor(153);
	cout << "&&";
	TextColor(17);
	cout << "##";
	TextColor(255);
	cout << "  ";
	TextColor(17);
	cout << "##";
	TextColor(153);
	cout << "&&";
	TextColor(17);
	cout << "##" << endl;

	//line 3
	GotoXY(69, 8);
	TextColor(255);
	cout << "  ";
	TextColor(17);
	cout << "##";
	TextColor(153);
	cout << "&&";
	TextColor(17);
	cout << "##";
	TextColor(153);
	cout << "&&";
	TextColor(17);
	cout << "##";
	TextColor(255);
	cout << "  " << endl;

	//line 4
	GotoXY(69, 9);
	TextColor(255);
	cout << "    ";
	TextColor(17);
	cout << "##";
	TextColor(153);
	cout << "&&";
	TextColor(17);
	cout << "##";
	TextColor(255);
	cout << "    " << endl;

	//line 3
	GotoXY(69, 10);
	TextColor(255);
	cout << "  ";
	TextColor(17);
	cout << "##";
	TextColor(153);
	cout << "&&";
	TextColor(17);
	cout << "##";
	TextColor(153);
	cout << "&&";
	TextColor(17);
	cout << "##";
	TextColor(255);
	cout << "  " << endl;

	//line 2
	GotoXY(69, 11);
	TextColor(17);
	cout << "##";
	TextColor(153);
	cout << "&&";
	TextColor(17);
	cout << "##";
	TextColor(255);
	cout << "  ";
	TextColor(17);
	cout << "##";
	TextColor(153);
	cout << "&&";
	TextColor(17);
	cout << "##" << endl;

	//line 1
	GotoXY(69, 12);
	TextColor(17);
	cout << "####";
	TextColor(255);
	cout << "      ";
	TextColor(17);
	cout << "####" << endl;

	TextColor(240);
}

void drawO() {
	//line 1
	GotoXY(94, 6);
	TextColor(255);
	cout << "    ";
	TextColor(68);
	cout << "######";
	TextColor(255);
	cout << "    " << endl;

	//line 2
	GotoXY(94, 7);
	TextColor(255);
	cout << "  ";
	TextColor(68);
	cout << "##";
	TextColor(204);
	cout << "&&";
	TextColor(204);
	cout << "&&";
	TextColor(204);
	cout << "&&";
	TextColor(68);
	cout << "##";
	TextColor(255);
	cout << "  " << endl;

	//line 3
	GotoXY(94, 8);
	TextColor(68);
	cout << "##";
	TextColor(204);
	cout << "&&";
	TextColor(255);
	cout << "  ";
	TextColor(255);
	cout << "  ";
	TextColor(255);
	cout << "  ";
	TextColor(204);
	cout << "&&";
	TextColor(68);
	cout << "##" << endl;

	//line 4
	GotoXY(94, 9);
	TextColor(68);
	cout << "##";
	TextColor(204);
	cout << "&&";
	TextColor(255);
	cout << "  ";
	TextColor(255);
	cout << "  ";
	TextColor(255);
	cout << "  ";
	TextColor(204);
	cout << "&&";
	TextColor(68);
	cout << "##" << endl;

	//line 3
	GotoXY(94, 10);
	TextColor(68);
	cout << "##";
	TextColor(204);
	cout << "&&";
	TextColor(255);
	cout << "  ";
	TextColor(255);
	cout << "  ";
	TextColor(255);
	cout << "  ";
	TextColor(204);
	cout << "&&";
	TextColor(68);
	cout << "##" << endl;

	//line 2
	GotoXY(94, 11);
	TextColor(255);
	cout << "  ";
	TextColor(68);
	cout << "##";
	TextColor(204);
	cout << "&&";
	TextColor(204);
	cout << "&&";
	TextColor(204);
	cout << "&&";
	TextColor(68);
	cout << "##";
	TextColor(255);
	cout << "  " << endl;

	//line 1
	GotoXY(94, 12);
	TextColor(255);
	cout << "    ";
	TextColor(68);
	cout << "######";
	TextColor(255);
	cout << "    " << endl;

	TextColor(240);
}


void StartGame() {
	system("cls");
	ResetData(); // Khởi tạo dữ liệu gốc
	TextColor(240);
	DrawBoard(_BOARD_SIZE); // Vẽ màn hình game
	drawGameTable(20, 13, 65, 5);
	drawX();
	drawGameTable(20, 13, 90, 5);
	drawO();
	drawGameTable(45, 10, 65, 19);
	TextColor(240);
	GotoXY(_X, _Y);
	cout << "x";
	GotoXY(_X, _Y);
}

//Hàm dọn dẹp tài nguyên (hàm nhóm Model)
void GabageCollect()
{
	// Dọn dẹp tài nguyên nếu có khai báo con trỏ
}

//Hàm thoát game (hàm nhóm Control)
void ExitGame() {
	system("cls");
	GabageCollect();
	//Có thể lưu game trước khi exit
}
int ProcessFinish(int pWhoWin) {
	GotoXY(0, _A[_BOARD_SIZE - 1][_BOARD_SIZE - 1].y + 2); // Nhảy tới vị trí
	// thích hợp để in chuỗi thắng/thua/hòa
	switch (pWhoWin) {
	case -1:
		//TextColor(240);
		winEffect();
		break;
	case 1:
		//TextColor(240);
		winEffect();
		break;
	case 0:
		TextColor(240);
		printf("Hoa");
		break;
	case 2:
		_TURN = !_TURN; // Đổi lượt nếu không có gì xảy ra
	}
	GotoXY(_X, _Y); // Trả về vị trí hiện hành của con trỏ màn hình bàn cờ
	return pWhoWin;
}
int AskContinue() {
	GotoXY(0, _A[_BOARD_SIZE - 1][_BOARD_SIZE - 1].y + 4);
	printf("Nhan 'y/n' de tiep tuc/dung: ");
	return toupper(_getch());
}
int CheckBoard(int pX, int pY) {
	for (int i = 0; i < _BOARD_SIZE; i++) {
		for (int j = 0; j < _BOARD_SIZE; j++) {
			if (_A[i][j].x == pX && _A[i][j].y == pY && _A[i][j].c == 0) {
				if (_TURN == true) { _A[i][j].c = -1; LastMove = _A[i][j]; } // Nếu lượt hiện hành là true thì c = -1
				else { _A[i][j].c = 1; LastMove = _A[i][j]; } // Nếu lượt hiện hành là false thì c = 1
				return _A[i][j].c;
			}
		}
	}
	return 0;
}

void moveEffect()
{
	if (_TURN) {
		GotoXY(_X, _Y);
		TextColor(240);
		printf("x");
	}
	else {
		GotoXY(_X, _Y);
		TextColor(240);
		printf("o");
	}
	GotoXY(_X, _Y);
}

void _moveEffect() {
	for (int i = 0; i < _BOARD_SIZE; i++) {
		for (int j = 0; j < _BOARD_SIZE; j++) {
			if (_A[i][j].x == _X && _A[i][j].y == _Y && _A[i][j].c == 0)
			{
				TextColor(240);
				printf(" ");
				GotoXY(_X, _Y);
			}
		}
	}
	for (int i = 0; i < _BOARD_SIZE; i++) {
		for (int j = 0; j < _BOARD_SIZE; j++) {
			if (_A[i][j].x == _X && _A[i][j].y == _Y && _A[i][j].c != 0)
			{
				if (_A[i][j].x != LastMove.x || _A[i][j].y != LastMove.y)
				{
					if (_A[i][j].c == 1)
					{
						TextColor(241);
						printf("O");
					}
					else {
						TextColor(244);
						printf("X");
					}
				}
				else if (_A[i][j].x == LastMove.x && _A[i][j].y == LastMove.y)
				{
					if (_A[i][j].c == 1)
					{
						TextColor(177);
						printf("O");
					}
					else {
						TextColor(180);
						printf("X");
					}
				}
				GotoXY(_X, _Y);
			}
		}
	}

}


void MoveRight() {
	_moveEffect();
	if (_X < _A[_BOARD_SIZE - 1][_BOARD_SIZE - 1].x)
	{
		_X += 4;
		GotoXY(_X, _Y);
	}
	else
	{
		_X = _A[0][0].x;
		GotoXY(_X, _Y);
	}
	moveEffect();
}
void MoveLeft() {
	_moveEffect();
	if (_X > _A[0][0].x) {
		_X -= 4;
		GotoXY(_X, _Y);
	}
	else
	{
		_X = _A[_BOARD_SIZE - 1][_BOARD_SIZE - 1].x;
		GotoXY(_X, _Y);

	}
	moveEffect();

}
void MoveDown() {
	_moveEffect();
	if (_Y < _A[_BOARD_SIZE - 1][_BOARD_SIZE - 1].y)
	{
		_Y += 2;
		GotoXY(_X, _Y);
	}
	else
	{
		_Y = _A[0][0].y;
		GotoXY(_X, _Y);
	}
	moveEffect();
}
void MoveUp() {
	_moveEffect();
	if (_Y > _A[0][0].y) {
		_Y -= 2;
		GotoXY(_X, _Y);
	}
	else
	{
		_Y = _A[_BOARD_SIZE - 1][_BOARD_SIZE - 1].y;
		GotoXY(_X, _Y);
	}
	moveEffect();

}


int TestBoard()
{
	if (_count == _BOARD_SIZE * _BOARD_SIZE) return 0; // Hòa
	else {
		if (WinnerCheck())
			return (_TURN == true ? -1 : 1); // -1 nghĩa là lượt ‘true’ thắng, 1 la false thang
		else
			return 2; // 2 nghĩa là chưa ai thắng
	}
}

int getCommand() { // Nhập lệnh
	return _COMMAND;
}

char waitKeyBoard() { // Nhập phím 
	_COMMAND = toupper(_getwch());
	return _COMMAND;
}

char askSave() { // Hỏi để lưu
	GotoXY(_LEFT, _TOP - 1);
	TextColor(240);
	cout << "Save this game for the next visit, press Y to save";
	waitKeyBoard();
	return getCommand();
}

class _Point {
	int _X;
	int _Y;
	int _check;
public:
	int GetCheck() { return _check; }
	int GetX() { return _X; }
	int GetY() { return _Y; }
	bool setCheck(int value) {
		if (value == -1 || value == 1 || value == 0) {
			_check = value;
			return true;
		}
		return false;
	}
};


void saveGame() {
	char name[30];
	GotoXY(50, 15);
	printf("Your name: ");
	gets_s(name);
	char fileName[40] = ".txt";
	strcat(name, fileName);
	FILE* f = fopen(name, "w");
	fprintf(f, "%d\n", _TURN);
	for (int i = 0; i < _BOARD_SIZE; i++) {
		for (int j = 0; j < _BOARD_SIZE; j++)
		{
			fprintf(f, "%d", _A[i][j].c);
			fprintf(f, "\n");
		}
	}
	fclose(f);

}

int loadGame() {
	vector<int> data;
	char name[30];
	TextColor(240);
	GotoXY(50, 0);
	cout << "Your name: ";
	gets_s(name);
	char fileName[10] = ".txt";
	strcat(name, fileName);
	cout << name << endl;
	system("cls");
	FILE* f = fopen(name, "rb");
	if (f == NULL) {
		GotoXY(30, 10);
		cout << "Khong tim thay ten nguoi choi !";
		Sleep(2000);
		return 0;
		system("cls");
		//play();
	}
	int j = 0;
	int i = 0;
	int a;
	StartGame();
	fscanf(f, "%d", &_TURN);
	fscanf(f, "\n");
	while (!feof(f)) {
		fscanf(f, "%d", &a);
		data.push_back(a);
	}
	int getdata = 0;
	for (int i = 0; i < _BOARD_SIZE; i++)
	{
		for (int j = 0; j < _BOARD_SIZE; j++)
		{
			int x = _A[i][j].x;
			int y = _A[i][j].y;
			GotoXY(x, y);
			_A[i][j].c = data[getdata];
			if (data[getdata] == -1) {
				TextColor(244);
				cout << "X";

				////countX++;
				////GotoXY(7, 3);
				////cout << countX;
				//GotoXY(71, 3);
				//cout << "O";
				//_b->setCheckAt(i, j, data[getdata]);
			}
			if (data[getdata] == 1) {
				TextColor(241);
				cout << "O";
				////countO++;
				////GotoXY(15, 3);
				////cout << countO;
				//GotoXY(71, 3);
				//cout << "X";
				//_b->setCheckAt(i, j, data[getdata]);
			}
			getdata++;
		}
	}
	/*_X = _b->getXAt(0, 0);
	_Y = _b->getYAt(0, 0);
	GotoXY(_X, _Y);*/
	// bí ẩn đừng thêm vào 
	//fclose(f);
	return 1;
}

void playAfterLoad() {
	if (loadGame() == 0) return;
	bool validEnter = true;
	GotoXY(68, 4);
	TextColor(15);
	cout << "   1P's INFO    " << endl;
	GotoXY(93, 4);
	cout << "   2P's INFO    ";
	GotoXY(_X, _Y);
	while (1)
	{
		if (_kbhit()) {
			waitKeyBoard();
			if (_COMMAND == 27) // Nhập phím "ESC" 
			{

				if (askSave() == 'Y') // Nhập Y để lưu 
				{
					saveGame();
					ExitGame();
					break;
				}
				else ExitGame();
				break;
			}

			else {
				if (_COMMAND == 'A' || _COMMAND == 75) MoveLeft();
				else if (_COMMAND == 'W' || _COMMAND == 72) MoveUp();
				else if (_COMMAND == 'S' || _COMMAND == 80) MoveDown();
				else if (_COMMAND == 'D' || _COMMAND == 77) MoveRight();
				else if (_COMMAND == 'R') TakeBack();
				else if (_COMMAND == 13) {// Người dùng đánh dấu trên màn hình bàn cờ
					switch (CheckBoard(_X, _Y)) {
					case -1:
						TextColor(244);
						printf("X"); break;
					case 1:
						TextColor(241);
						printf("O"); break;
					case 0: validEnter = false; break; // Khi đánh vào ô đã đánh rồi
					}
					// Tiếp theo là kiểm tra và xử lý thắng/thua/hòa/tiếp tục
					if (validEnter == true) {
						TextColor(7);
						switch (ProcessFinish(TestBoard())) {
						case -1: case 1: case 0:
							if (AskContinue() != 'Y') {
								ExitGame(); return;
							}
							else StartGame();
						}
					}
					validEnter = true; // Mở khóa
				}
			}
		}
	}
}


void gotoxy(int column, int line)

{

	COORD coord;

	coord.X = column;

	coord.Y = line;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}

void Nocursortype()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

void SetWindowSize(SHORT width, SHORT height)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	SMALL_RECT WindowSize;
	WindowSize.Top = 0;
	WindowSize.Left = 0;
	WindowSize.Right = width;
	WindowSize.Bottom = height;

	SetConsoleWindowInfo(hStdout, 1, &WindowSize);
}

void SetScreenBufferSize(SHORT width, SHORT height)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD NewSize;
	NewSize.X = width;
	NewSize.Y = height;

	SetConsoleScreenBufferSize(hStdout, NewSize);
}

void DisableResizeWindow()
{
	HWND hWnd = GetConsoleWindow();
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void DisableCtrButton(bool Close, bool Min, bool Max)
{
	HWND hWnd = GetConsoleWindow();
	HMENU hMenu = GetSystemMenu(hWnd, false);

	if (Close == 1)
	{
		DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
	}
	if (Min == 1)
	{
		DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
	}
	if (Max == 1)
	{
		DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
	}
}

void ShowScrollbar(BOOL Show)
{
	HWND hWnd = GetConsoleWindow();
	ShowScrollBar(hWnd, SB_BOTH, Show);
}


int move() {

	char c = _getch();

	if ((int)c == -32) c = _getch();

	switch ((int)c) {

	case 80:
		return 1; //cout << "Xuong";

	case 72:
		return 2; //cout << "Len";

	case 77:
		return 3; //cout << "Phai";       

	case 75:
		return 4; //cout << "Trai";

	case 27:
		return 8; //Nut ESC thoat

	case 13:
		return 5; //Nut Enter

	default:
		return 0; //cout << "Sai";

	}

}
void Mainmenu() {
	SetConsoleTextAttribute(hConsoleColor, 240);
	cout << "         ";
	cout << "/--------------------------------------------------------------------------------------------------\\" << "         " << endl;
	cout << "         ";
	cout << "| ";
	//line1
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << " ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "     ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 240);
	cout << "  |" << "         " << endl;
	//line2
	cout << "         ";
	cout << "| ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << " ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << " ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "     ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "     ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << " ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 240);
	cout << "  |" << "         " << endl;
	//line3
	cout << "         ";
	cout << "| ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "     ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << " ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << " ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << " ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << " ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << " ";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@@@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 240);
	cout << "  |" << "         " << endl;
	//line4
	cout << "         ";
	cout << "| ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "       ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "   ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@@@@@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "   ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "     ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "@@@";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@@@@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 240);
	cout << "  |" << "         " << endl;
	//line5
	cout << "         ";
	cout << "| ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "   ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "   ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << " ";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "   ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "   ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 240);
	cout << "  |" << "         " << endl;
	//line6
	cout << "         ";
	cout << "| ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "     ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "   ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "     ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@@@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << " ";
	SetConsoleTextAttribute(hConsoleColor, 240);
	cout << " |" << "         " << endl;
	cout << "         ";
	cout << "\\--------------------------------------------------------------------------------------------------/" << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                                                                                      " << endl;
	cout << "                                                                                                                      " << endl;
	cout << "                                                                                                                      " << endl;
	//1
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 85);
	cout << "@@@@@@@@@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "                                                                        " << endl;
	//2
	cout << "         ";
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 85);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 85);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "                                                                      " << endl;
	//3
	cout << "    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 85);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 85);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "                                                                      " << endl;
	//4
	cout << "    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 85);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 85);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "######";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "######";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "          " << endl;

	cout << "         ";
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 85);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 85);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                      ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "            ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##########";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "        " << endl;

	cout << "         ";
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 85);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 221);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 85);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                     ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "              ";
	SetConsoleTextAttribute(hConsoleColor, 51);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "      " << endl;

	cout << "         ";
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 85);
	cout << "@@@@@@@@@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "**********";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "######";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "      " << endl;

	cout << "         ";
	cout << "                                                     ";
	cout << "           ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "              ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "      " << endl;

	cout << "         ";
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "######";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "     ";
	cout << "                             ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                          ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "      " << endl;

	cout << "         ";
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "   ";
	cout << "                             ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##########";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "      " << endl;

	//11
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "      " << endl;
	//12
	cout << "         ";
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "      " << endl;
	//13
	cout << "      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << " ";
	cout << "                             ";
	//line10
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "              ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "          " << endl;
	//14
	cout << "         ";
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 204);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "                                                                    " << endl;
	//15
	cout << "         ";
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "!!!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "######";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "                                                                      " << endl;
	cout << "                                                                                                                      " << endl


		<< "                                                                                                                      " << endl;

}
void newgame() {
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                                                                                      " << endl
		<< "                                                                                                                      " << endl
		<< "                                                                                                                      " << endl
		<< "                                                                                                                      " << endl;
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                           ";
	cout << "                                                         " << "         " << endl;
	//1
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "######";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                    ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "       ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "################################";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "        " << endl;
	//2
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                    ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "     ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	cout << "                        ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      " << endl;
	//3
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%%%%%%%%%%%%%%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      " << endl;

	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                    ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "     ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "                    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      " << endl;
	//5
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                    ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "     ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "                    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      " << endl;
	//6
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 51);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "ww";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                       ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "                    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      " << endl;
	//7
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 51);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "ww";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                       ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "              ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "            ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                ";
	cout << "       ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      " << endl;
	cout << "               ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	cout << "                                       ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "@@@@@@@@@@";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      " << endl;
	//10
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "########";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "rr";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "cc";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	cout << "                                       ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "                    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      " << endl;
	//11
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "yy";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "cc";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	cout << "                                       ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "                    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      " << endl;
	//12
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "rr";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "yy";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "yy";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	cout << "                                       ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      " << endl;
	//13
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "cc";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "rr";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	cout << "                                       ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "######";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%%%%%%%%%%%%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "######";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "        " << endl;
	//14
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "          ";
	cout << "                                       ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "              ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "              " << endl;

	//15
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "yy";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 68);
	cout << "rr";
	SetConsoleTextAttribute(hConsoleColor, 170);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                       ";
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "              " << endl;
	//16
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!!!!!!!";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                       ";
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "              " << endl;
	//17
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 51);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 51);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "######";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                       ";
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "              " << endl;
	//18
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 51);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 51);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                       ";
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "              ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "              " << endl;


	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "&&&&&&";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                        ";
	cout << "             ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "            ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                " << endl;
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "&&&&";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "**";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                       ";
	cout << "            ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                " << endl;
	//21
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "****";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                       ";
	cout << "            ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                " << endl;
	//22
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##################";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";


	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                       ";
	cout << "              ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                  " << endl;
	cout << "                                                                                                                      " << endl
		<< "                                                                                                                      " << endl
		<< "                                                                                                                      " << endl
		<< "                                                                                                                      " << endl
		<< "                                                                                                                      " << endl;



}
void settings() {
	cout << "         ";
	cout << "          /-------------------------------------------------------------------------------\\         " << "         " << endl;
	//1
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "              ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "            " << "         " << endl;
	//2
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "            ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "              " << "         " << endl;
	//3
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "            ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "            " << "         " << endl;
	//4
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "            ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "            " << "         " << endl;
	//5
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "            ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "            " << "         " << endl;
	//6
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "            ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "              " << "         " << endl;
	SetConsoleTextAttribute(hConsoleColor, 240);
	cout << "         ";
	cout << "          \\-------------------------------------------------------------------------------/         " << "         " << endl;

	//1 	
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!!!!!!!!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                                     " << "         " << endl;
	//2
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                  ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                                   " << "         " << endl;
	//3
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "          ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                           " << "         " << endl;
	//4
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "        ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                         " << "         " << endl;
	//5
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "              ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                       " << "         " << endl;
	//6
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                       " << "         " << endl;
	//7
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                       ";
	SetConsoleTextAttribute(hConsoleColor, 240);
	cout << "       ";
	cout << "ON                  " << "         " << endl;
	//8
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                     " << "         " << endl;
	//9
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@@@";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 153);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@@@";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                   " << "         " << endl;
	//10
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@@@";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&&&&&&&&&";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@@@";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                   " << "         " << endl;
	//11
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&&&&&&&&&&&&&";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                   " << "         " << endl;
	//12
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&&&&&&&&&&&&&";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^^^";
	SetConsoleTextAttribute(hConsoleColor, 34);
	cout << "!!";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	SetConsoleTextAttribute(hConsoleColor, 240);
	cout << "     ";

	cout << "ON                        " << "         " << endl;

	//13
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "  ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&&&&&&&&&";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                   " << "         " << endl;
	//15
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "                              ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                     " << "         " << endl;
	//16
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "                ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                       " << "         " << endl;
	//17
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%%%";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "              ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                       " << "         " << endl;
	//18
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%%%";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&&&";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 187);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%%%";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                          ";
	SetConsoleTextAttribute(hConsoleColor, 240);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                         " << "         " << endl;
	//19
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "        ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&&&";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 119);
	cout << "^^^^^^^^^^";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&&&";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                         " << "         " << endl;
	//20
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "          ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "%%%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                           " << "         " << endl;
	//21
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                  ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 136);
	cout << "%%%%%%%%%%";
	SetConsoleTextAttribute(hConsoleColor, 17);
	cout << "@@";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                  " << endl;
	//22
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "&&&&&&&&&&";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                                     " << "         " << endl;
}
void credits() {	//1 
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "            ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####################################";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "            ";
	cout << "                    " << "         " << endl;
	//2
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                            ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                            " << "         " << endl;
	//3
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                          ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                          " << "         " << endl;
	//4
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                        ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 96);
	cout << "Group 9 - Program technicques";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "###";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                        " << "         " << endl;
	//5
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                      " << "         " << endl;
	//6
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                        ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                      " << "         " << endl;
	//7
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//8
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;

	//9
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//10
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "Nguyen Quoc Thang - 2212738";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << endl;
	//11
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "Nguyen Dinh Kien - 22127216";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "        ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//12
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "Nguyen Gia Nguyen - 22127301";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "       ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//13
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "       ";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "Nguyen Tran Duc Thien - 22127397";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "     ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//14
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//15
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//16
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//17
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "           ";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "GVHD. Truong Toan Thinh";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//18
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//19
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//20
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//21
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//22
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
	//23
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                        ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                      " << "         " << endl;
	//24
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                      " << "         " << endl;
	//25
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                        ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####################################";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                        " << "         " << endl;
	//26
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                          ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                          " << "         " << endl;
	//27
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                            ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                    ";
	SetConsoleTextAttribute(hConsoleColor, 238);
	cout << "                ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                            " << "         " << endl;
	//28
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####################################";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                " << "         " << endl;


	cout << "         ";
	cout << "                      ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "                                                        ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                      " << "         " << endl;
	cout << "                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 0);
	cout << "                                                            ";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                    " << "         " << endl;
}
void helps() {
	cout << "                                                                                                    " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "                                            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                    " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "                                          ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                    " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "                                          ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "######";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                  " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "                              ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "################################";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "                                  ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    W - up arrow:             UP    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                            " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    A - left arrow:         LEFT      ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                          " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    S - down arrow:         DOWN        ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                        " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    D - right arrow:       RIGHT        ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                        " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    Enter:          CONFIRM MOVE      ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                          " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    ESC:                   PAUSE    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                            " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "                                  ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                              ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "################################";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "  ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                          ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "######";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                  " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                         ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "               ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "          ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                    " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                       ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "#################################";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                            " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                     ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "                             ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                            " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                   ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "                     ";
	cout << "BACK";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                            " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                     ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "                             ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                            " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                       ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "#################################";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                            " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                         ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "               ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                  " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                          ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                  " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "         ";
	cout << "                                          ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "##";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                  " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                        ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                                " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                      ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "######";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "    ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "######";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                              " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 224);
	cout << "            ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                            " << "         " << endl;

	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                    ";
	cout << "         ";
	SetConsoleTextAttribute(hConsoleColor, 102);
	cout << "####################";
	SetConsoleTextAttribute(hConsoleColor, 255);
	cout << "                                            " << "         " << endl;

	cout << "                                                                                                    " << endl;

}

class Menu {

public:

	Menu();

	~Menu() {}

	void printMenu();

	void deleteMenu(); //Xóa menu khi kết thúc chương trình bằng cách viết đè kí tự trắng

	int numberOfItem() {
		return _numberOfItem;
	}

	string* getItem() {
		return item;
	}

private:

	string* item; //Mảng lưu tên của các menu

	int _numberOfItem; //Số lượng menu

};
Menu::Menu() {

	item = new string[100];

	_numberOfItem = 6;

	item[0] = "NEW GAME";

	item[1] = "LOAD GAME";

	item[2] = "SETTINGS";

	item[3] = "CREDITS ";

	item[4] = "HELPS";

	item[5] = "EXIT";

}
void Menu::printMenu() {
	Mainmenu();
	SetConsoleTextAttribute(hConsoleColor, 240);
	for (int i = 0 + 14; i < _numberOfItem + 14; i++) {

		gotoxy(55, i);

		cout << item[i - 14] << endl;

		Sleep(100); //Tạm dừng 100ms

	}
}
void Menu::deleteMenu() {

	for (int i = 0; i < 30; i++) {

		Sleep(30);

		gotoxy(0, i);

		for (int j = 0; j < 119; j++) {
			cout << " "; //Xóa bằng cách ghi đè kí tự trắng

		}

	}

}

class NewGameMenu {
public:

	NewGameMenu();

	~NewGameMenu() {}

	void printMenu();

	void deleteMenu(); //Xóa menu khi kết thúc chương trình bằng cách viết đè kí tự trắng

	int numberOfItem() {
		return _numberOfItem;
	}

	string* getItem() {
		return item;
	}

private:

	string* item; //Mảng lưu tên của các menu

	int _numberOfItem; //Số lượng menu

};
NewGameMenu::NewGameMenu() {
	item = new string[100];

	_numberOfItem = 3;

	item[0] = "SINGLEPLAYER (PvB)";

	item[1] = "MULTIPLAYER (PvP)";

	item[2] = "      BACK        ";

};
void NewGameMenu::printMenu() {
	newgame();
	SetConsoleTextAttribute(hConsoleColor, 240);
	gotoxy(48, 6);
	cout << item[0] << endl;
	Sleep(100);
	gotoxy(48, 6 + 5);
	cout << item[1] << endl;
	Sleep(100);
	gotoxy(48, 6 + 5 + 5);
	cout << item[2] << endl;
	Sleep(100);
}
void NewGameMenu::deleteMenu() {

	for (int i = 0; i < 30; i++) {

		Sleep(10);

		gotoxy(0, i);

		for (int j = 0; j < 119; j++) {
			cout << " "; //Xóa bằng cách ghi đè kí tự trắng

		}

	}

}
void NewGame(int& pmode, int m, int s) {
	NewGameMenu newGame;
	int x_NG;
	int line_NG = 6;
	bool BACK_NG = false;
	newGame.printMenu();
	SetConsoleTextAttribute(hConsoleColor, 240);
	gotoxy(45, line_NG);
	cout << ">";
	while (!BACK_NG) {
		if (_kbhit()) {
			x_NG = move();
			gotoxy(45, line_NG);
			SetConsoleTextAttribute(hConsoleColor, 255);
			cout << " "; //Xóa con trỏ ở vị trí cũ
			SetConsoleTextAttribute(hConsoleColor, 240);
			switch (x_NG) {
			case 1:if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			case 3:
			{
				if (s % 2 == 0)
					PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
				line_NG += 5;
				if (line_NG >= newGame.numberOfItem() + 5 + 5 + 6) line_NG = 6;
				break;
			}
			case 2:if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			case 4:
			{
				if (s % 2 == 0)
					PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
				line_NG -= 5;
				if (line_NG < 6) line_NG = newGame.numberOfItem() - 1 + 6 + 3 + 5;
				break;
			}

			case 5:
			{
				pmode = line_NG;
				if (s % 2 == 0)
					PlaySound(TEXT("enter.wav"), NULL, SND_ASYNC);
				BACK_NG = true;
				break;
			}
			}
			gotoxy(45, line_NG);
			SetConsoleTextAttribute(hConsoleColor, 240);
			cout << ">";
		}
	}
}

class Settings {
public:

	Settings();

	~Settings() {}

	void printMenu();


	int numberOfItem() {
		return _numberOfItem;
	}

	string* getItem() {
		return item;
	}

private:

	string* item; //Mảng lưu tên của các menu

	int _numberOfItem; //Số lượng menu

};
Settings::Settings() {
	item = new string[100];
	_numberOfItem = 3;
	item[0] = "MUSIC: ";
	item[1] = "  SFX: ";
	item[2] = "  BACK";
}
void Settings::printMenu() {
	settings();
	SetConsoleTextAttribute(hConsoleColor, 240);
	gotoxy(79, 14);
	cout << item[0] << endl;
	Sleep(10);
	gotoxy(79, 14 + 5);
	cout << item[1] << endl;
	Sleep(10);
	gotoxy(79, 14 + 5 + 5);
	cout << item[2] << endl;
	Sleep(10);
}

void SETTINGS(int& m, int& s) {
	Settings set;
	int x_SET;
	bool BACK_SET = false;
	int line_SET = 14;
	set.printMenu();
	if (m % 2 == 0) {
		gotoxy(88, 14);
		cout << "ON ";
	}
	else { gotoxy(88, 14); cout << "OFF"; }
	if (s % 2 == 0) {
		gotoxy(88, 19);
		cout << "ON ";
	}
	else { gotoxy(88, 19); cout << "OFF"; }
	gotoxy(76, 14);
	SetConsoleTextAttribute(hConsoleColor, 240);
	cout << ">";
	while (!BACK_SET) {
		if (_kbhit) {
			x_SET = move();
			gotoxy(76, line_SET);
			SetConsoleTextAttribute(hConsoleColor, 255);
			cout << " "; //Xóa con trỏ ở vị trí cũ
			SetConsoleTextAttribute(hConsoleColor, 240);
			switch (x_SET) {
			case 1:if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			case 3:
			{
				if (s % 2 == 0)
					PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
				line_SET += 5;
				if (line_SET >= set.numberOfItem() + 14 + 5 + 5) line_SET = 14;
				break;
			}
			case 2:if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			case 4:
			{
				if (s % 2 == 0)
					PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
				line_SET -= 5;
				if (line_SET < 14) line_SET = set.numberOfItem() - 1 + 14 + 5 + 3;
				break;
			}
			case 5:
			{
				if (s % 2 == 0)
					PlaySound(TEXT("enter.wav"), NULL, SND_ASYNC);
				if (line_SET == 14) {
					m++;
					if (m % 2 == 0) {
						gotoxy(88, 14);
						cout << "ON ";
					}
					else { gotoxy(88, 14); cout << "OFF"; }
				}
				if (line_SET == 19) {
					s++;
					if (s % 2 == 0) {
						gotoxy(88, 19);
						cout << "ON ";
					}
					else { gotoxy(88, 19); cout << "OFF"; }
				}
				if (line_SET == 14 + 5 + 5) BACK_SET = true;
				break;
			}
			}
			gotoxy(76, line_SET);
			SetConsoleTextAttribute(hConsoleColor, 240);
			cout << ">";

		}
	}
}

void redToWhite()//-1:244   1"241
{

	for (int i = 0; i < _BOARD_SIZE; i++)
		for (int j = 0; j < _BOARD_SIZE; j++)
			if (_A[i][j].x == LastMove.x && _A[i][j].y == LastMove.y)
			{
				GotoXY(LastMove.x, LastMove.y);
				if (_A[i][j].c == -1)
				{
					TextColor(244);
					printf("X");
				}
				else if (_A[i][j].c == 1)
				{
					TextColor(241);
					printf("O");
				}
				TextColor(240);
				GotoXY(_X, _Y);
			}
}

void CREDITS(int s) {
	int x_CRE;
	bool BACK_CRE = false;
	int line_CRE = 26;
	credits();
	gotoxy(52, 26);
	SetConsoleTextAttribute(hConsoleColor, 222);
	cout << "RETURN TO MENU";
	SetConsoleTextAttribute(hConsoleColor, 255);
	while (!BACK_CRE) {
		if (_kbhit()) {
			x_CRE = move();
			if (x_CRE == 5) {
				if (s % 2 == 0)
					PlaySound(TEXT("enter.wav"), NULL, SND_ASYNC);
				BACK_CRE = true;
			}
		}
	}
}

void HELPS(int s) {
	int x_HELP;
	bool BACK_HELP = false;
	int line_HELP = 18;
	helps();
	gotoxy(53, 18);
	SetConsoleTextAttribute(hConsoleColor, 222);
	cout << "BACK";
	SetConsoleTextAttribute(hConsoleColor, 255);
	while (!BACK_HELP) {
		if (_kbhit()) {
			x_HELP = move();
			if (x_HELP == 5) {
				if (s % 2 == 0)
					PlaySound(TEXT("enter.wav"), NULL, SND_ASYNC);
				BACK_HELP = true;
			}
		}
	}
}

void multiplay() {
	StartGame();
	GotoXY(68, 4);
	TextColor(15);
	cout << "   1P's INFO    " << endl;
	GotoXY(93, 4);
	cout << "   2P's INFO    ";
	GotoXY(_X, _Y);
	_count = 0;
	LastMove = _A[0][0];
	bool validEnter = true;
	start = clock();
	int temp_time = 0;
	while (1)
	{
		clock_t end = clock();

		if (((end - start) / 1000) != temp_time)
		{
			GotoXY(0, 0);
			printf("                                                     ");// Xóa thời gian trước đó rồi mới in
			GotoXY(0, 0);
			printf("%i:%i", ((end - start)) / 1000 / 60, ((end - start) / 1000) % 60);
			GotoXY(_X, _Y);
		}
		temp_time = (end - start) / 1000;
		if (_kbhit()) {
			waitKeyBoard();
			if (_COMMAND == 27) // Nhập phím "ESC" 
			{

				if (askSave() == 'Y') // Nhập Y để lưu 
				{
					saveGame();
					ExitGame();
					break;
				}
				else ExitGame();
				break;
			}

			else {
				if (_COMMAND == 'A' || _COMMAND == 75) MoveLeft();
				else if (_COMMAND == 'W' || _COMMAND == 72) MoveUp();
				else if (_COMMAND == 'S' || _COMMAND == 80) MoveDown();
				else if (_COMMAND == 'D' || _COMMAND == 77) MoveRight();
				else if (_COMMAND == 'R') TakeBack();
				else if (_COMMAND == 13) {// Người dùng đánh dấu trên màn hình bàn cờ
					redToWhite();
					switch (CheckBoard(_X, _Y)) {
					case -1:
						TextColor(180);
						printf("X");
						TextColor(240);
						break;
					case 1:
						TextColor(177);
						printf("O");
						TextColor(240);
						break;
					case 0: validEnter = false; break; // Khi đánh vào ô đã đánh rồi
					}
					// Tiếp theo là kiểm tra và xử lý thắng/thua/hòa/tiếp tục
					if (validEnter == true) {
						TextColor(7);
						switch (ProcessFinish(TestBoard())) {
						case -1: case 1: case 0:
							if (AskContinue() != 'Y') {
								ExitGame(); return;
							}
							else StartGame();
						}
					}
					validEnter = true; // Mở khóa
				}
			}
		}
	}
}


void main()
{
	PlaySound(TEXT("ycg_logo.wav"), NULL, SND_ASYNC);
	int m = 2, s = 2;
	SetWindowSize(118, 60);
	SetScreenBufferSize(118, 60);
	DisableResizeWindow();
	DisableCtrButton(1, 1, 1);
	Nocursortype();
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	Menu menu;
	int x;
	int pmode;
	int line = 14; //Vị trí dòng của menu
	bool EXIT = false;
	menu.printMenu();
	Sleep(3000);
	SetConsoleTextAttribute(hConsoleColor, 240);
	gotoxy(53, line);
	cout << ">";

	//Vẽ con trỏ trỏ tới menu
	while (!EXIT) {
		if (m == 100) m = 2;
		if (s == 100) s = 2;
		if (_kbhit()) {
			x = move();
			gotoxy(53, line);
			SetConsoleTextAttribute(hConsoleColor, 255);
			cout << " "; //Xóa con trỏ ở vị trí cũ
			SetConsoleTextAttribute(hConsoleColor, 240);
			switch (x) {
			case 1:if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			case 3:
			{if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			line++;
			if (line >= menu.numberOfItem() + 14) line = 14;
			break;
			}
			case 2:if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			case 4:
			{if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			line--;
			if (line < 14) line = menu.numberOfItem() - 1 + 14;
			break;
			}
			case 5:
			{	if (s % 2 == 0)
				PlaySound(TEXT("enter.wav"), NULL, SND_ASYNC);
			if (line == 14) {
				system("cls");
				NewGame(pmode, m, s);
				if (pmode == 6);
				if (pmode == 11) {
					multiplay();
				}
				system("cls");
				menu.printMenu();
			}
			if (line == 15) {
				system("cls");
				playAfterLoad();
				system("cls");
				menu.printMenu();
			}
			if (line == 16) {
				system("cls");
				SETTINGS(m, s);
				system("cls");
				menu.printMenu();
			}
			if (line == 17) {
				system("cls");
				HELPS(s);
				system("cls");
				menu.printMenu();
			}
			if (line == 18) {
				system("cls");
				CREDITS(s);
				system("cls");
				menu.printMenu();
			}
			if (line == 19) EXIT = true;
			break;
			}
			}
			gotoxy(53, line);
			SetConsoleTextAttribute(hConsoleColor, 240);
			cout << ">";

		}
	}

	menu.deleteMenu();
	gotoxy(55, 14);
	SetConsoleTextAttribute(hConsoleColor, 240);
	cout << "GOODBYE!";
	gotoxy(0, 0);
	TextColor(255);

}