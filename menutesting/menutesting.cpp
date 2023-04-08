#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <stdlib.h>
using namespace std;
#define MAUNEN 249
#define MAUCHU 240
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

typedef char str[31];
str thaotac[6] = { "NEW GAME", "LOAD GAME" ,"SETTINGS", "CREDITS", "HELPS" , "EXIT" };
str newgame[2] = { "SINGLEPLAYER (PVB)", "MULTIPLAYER (PVP)" };
str settings[3] = { "MUSIC", "SFX", "BACK" };
str credits[1] = { "BACK" };
str helps[1] = { "BACK" };
enum TRANGTHAI { UP, DOWN, LEFT, RIGHT, ENTER, NONE };

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

void TakeBack(_POINT LastMove)
{
	for (int i = 0; i < _BOARD_SIZE; i++)
		for (int j = 0; j < _BOARD_SIZE; j++) {
			if (_A[i][j].x == LastMove.x && _A[i][j].y == LastMove.y)
				if (_A[i][j].c != 0) {
					_A[i][j].c = 0;
					_TURN = !_TURN;
					_count--;
					_X = LastMove.x;
					_Y = LastMove.y;
					GotoXY(_X, _Y); TextColor(240); printf(" "); GotoXY(_X, _Y);//Xóa dấu đã đánh 
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
	GotoXY(_X,_Y);
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
		TextColor(240);
		//printf("Nguoi choi %d da thang va nguoi choi %d da thua\n", true, false);
		printf("Nguoi choi X da thang va nguoi choi O da thua\n");
		break;
	case 1:
		TextColor(240);
		//printf("Nguoi choi %d da thang va nguoi choi %d da thua\n", false, true);
		printf("Nguoi choi O da thang va nguoi choi X da thua\n");
		break;
	case 0:
		TextColor(240);
		//printf("Nguoi choi %d da hoa nguoi choi %d\n", false, true);
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
int CheckBoard(int pX, int pY, _POINT& LastMove) {
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
				if (_A[i][j].c == 1)
				{
					TextColor(241);
					printf("O");
				}
				else {
					TextColor(244);
					printf("X");
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
	moveEffect();
}

void MoveLeft() {
	_moveEffect();
	if (_X > _A[0][0].x) {
		_X -= 4;
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
	moveEffect();
}
void MoveUp() {
	_moveEffect();
	if (_Y > _A[0][0].y) {
		_Y -= 2;
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
	if(loadGame() == 0) return;
	bool validEnter = true;
	GotoXY(68, 4);
	TextColor(15);
	cout << "   1P's INFO    "<< endl;
	GotoXY(93, 4);
	cout << "   2P's INFO    ";
	GotoXY(_X, _Y);
	while (1)
	{
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
			else if (_COMMAND == 'R') TakeBack(LastMove);
			else if (_COMMAND == 13) {// Người dùng đánh dấu trên màn hình bàn cờ
				switch (CheckBoard(_X, _Y, LastMove)) {
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



TRANGTHAI key(int z) {
	
	if (z == 224) {
		char c;
		c = _getch();
		if (c == 72)
			return UP;
		if (c == 80)
			return DOWN;
		if (c == 77)
			return RIGHT;
		if (c == 75)
			return LEFT;
	}
	
	else if (z == 13)
		return ENTER;
	
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

void rainbowmenu() {
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
}

void settingsheadline() {
	SetConsoleTextAttribute(hConsoleColor, 240);
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
};

int MainMenu(str thaotac[6], int n, int m, int s, int& mode) {
	int tt = 0;//Biến chỉ ra đang ở thao tác nào hiện tạo tt= 0 là chỉ thao tác thứ nhất
	int* mau = new int[n];
	for (int i = 0; i < n; i++)
		mau[i] = MAUCHU;
	mau[0] = MAUNEN;//Hiện đang ở thao tác thứ nhất
	while (1)
	{
		SetWindowSize(118, 60);
		SetScreenBufferSize(118, 60);
		Nocursortype();
		rainbowmenu();
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

		//line1
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

		for (int i = 0; i < n; i++) {
			switch (i) {
				//5
			case 0: {
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
				cout << "   ";
				break;
			}
				  //6
			case 1: {
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
				cout << " ";
				break;
			}
				  //7
			case 2: {
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
				cout << " ";
				break;
			}
				  //8
			case 3: {
				cout << "         ";
				cout << "                                   ";
				break; }
				  //9
			case 4: {
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
				break;
			}
				  //10
			case 5: {
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
				break;
			}
			}
			SetConsoleTextAttribute(hConsoleColor, mau[i]);
			cout << "           ";
			cout << thaotac[i];
			SetConsoleTextAttribute(hConsoleColor, 255);
			switch (i) {
			case 0: {
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
				break;
			}
			case 1: {
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
				break;
			}
			case 2: {
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
				break;
			}
			case 3: {
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
				break;
			}
			case 4: {
				cout << "             ";
				SetConsoleTextAttribute(hConsoleColor, 0);
				cout << "##";
				SetConsoleTextAttribute(hConsoleColor, 255);
				cout << "                          ";
				SetConsoleTextAttribute(hConsoleColor, 0);
				cout << "##";
				SetConsoleTextAttribute(hConsoleColor, 255);
				cout << "         ";
				cout << "      " << endl;
				break;
			}
			case 5: {
				cout << "              ";
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
				break;
			}
			}
		}
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
		int z = _getch();

		TRANGTHAI trangthai = key(z);
		switch (trangthai) {
		case UP: {
			if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			if (tt == 0)
				tt = n - 1;
			else
				tt--;
			break;
		}
		case DOWN: {
			if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			if (tt == n - 1)
				tt = 0;
			else
				tt++;
			break;
		}
		case ENTER: {
			if (s % 2 == 0)
				PlaySound(TEXT("enter.wav"), NULL, SND_SYNC);
			return tt;//Tức là mình chọn thao tác đó khi nhấn phím enter
			break;
		}
		}
		mode = tt;
		//Reset lại màu sau khi chọn lại thao tác khác để in ra
		for (int i = 0; i < n; i++)
			mau[i] = MAUCHU;
		mau[tt] = MAUNEN;
		system("cls");
	}
}

int NEWGAME(str newgame[2], int n, int m, int s, int& pmode) {
	int tt = 0;
	int* mau = new int[n];
	for (int i = 0; i < n; i++)
		mau[i] = MAUCHU;
	mau[0] = MAUNEN;
	while (1) {
		SetWindowSize(118, 60);
		SetScreenBufferSize(118, 60);
		DisableResizeWindow();
		DisableCtrButton(1, 1, 1);
		Nocursortype();
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
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
		cout << "          ";
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
		cout << "  " << endl;
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
		cout << "##" << endl;
		SetConsoleTextAttribute(hConsoleColor, 255);
		cout << "         ";
		//3
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
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		cout << "               ";
		SetConsoleTextAttribute(hConsoleColor, mau[0]);
		cout << newgame[0];
		cout << "      ";
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
		cout << "##" << endl;
		SetConsoleTextAttribute(hConsoleColor, 255);
		cout << "         ";
		//4
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
		cout << "##" << endl;
		SetConsoleTextAttribute(hConsoleColor, 255);
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
		cout << "##" << endl;
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
		cout << "    " << endl;
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
		cout << "##" << endl;
		SetConsoleTextAttribute(hConsoleColor, 255);

		//8
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
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		cout << "               ";
		SetConsoleTextAttribute(hConsoleColor, mau[1]);
		cout << newgame[1];
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);

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
		cout << "##" << endl;
		//9
		SetConsoleTextAttribute(hConsoleColor, 255);
		cout << "      ";
		cout << "         ";
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
		cout << "##" << endl;
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
		cout << "##" << endl;
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
		cout << "##" << endl;
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
		cout << "##" << endl;
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
		cout << "  " << endl;
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
		cout << "        " << endl;
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
		cout << "        " << endl;
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
		cout << "        " << endl;
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
		cout << "        " << endl;
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
		cout << "        " << endl;
		//19
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

		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		cout << "                    ";
		SetConsoleTextAttribute(hConsoleColor, mau[2]);
		cout << newgame[2];
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
		cout << "          " << endl;
		//20
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
		cout << "          " << endl;
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
		cout << "          " << endl;
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
		cout << "            " << endl;
		cout << "                                                                                                                      " << endl
			<< "                                                                                                                      " << endl
			<< "                                                                                                                      " << endl
			<< "                                                                                                                      " << endl
			<< "                                                                                                                      " << endl;
		int z = _getch();
		TRANGTHAI trangthai = key(z);
		switch (trangthai) {
		case UP: {
			if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			if (tt == 0)
				tt = n - 1;
			else
				tt--;
			break;
		}
		case DOWN: {
			if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			if (tt == n - 1)
				tt = 0;
			else
				tt++;
			break;
		}
		case ENTER: {
			if (s % 2 == 0)
				PlaySound(TEXT("enter.wav"), NULL, SND_SYNC);
			return tt;//Tức là mình chọn thao tác đó khi nhấn phím enter
			break;
		}
		}
		pmode = tt;
		for (int i = 0; i < n; i++)
			mau[i] = MAUCHU;
		mau[tt] = MAUNEN;
		system("cls");
	}
}

int HELPS(str helps[1], int m, int s, int n) {
	int tt = 0;
	int* mau = new int[n];
	for (int i = 0; i < n; i++)
		mau[i] = MAUCHU;
	mau[0] = MAUNEN;
	while (1) {
		SetWindowSize(118, 60);
		SetScreenBufferSize(118, 60);
		DisableResizeWindow();
		DisableCtrButton(1, 1, 1);
		Nocursortype();
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
		for (int i = 0; i < n; i++) {
			SetConsoleTextAttribute(hConsoleColor, 222);
			cout << helps[0];
		}
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


		int z = _getch();
		TRANGTHAI trangthai = key(z);
		switch (trangthai) {
		case ENTER: {
			if (s % 2 == 0)
				PlaySound(TEXT("enter.wav"), NULL, SND_SYNC);
			return tt;//Tức là mình chọn thao tác đó khi nhấn phím enter
			break;
		}
		}
		for (int i = 0; i < n; i++)
			mau[i] = MAUCHU;
		mau[tt] = MAUNEN;
		system("cls");
	}
}

int SETTINGS(str settings[3], int n, int& m, int& s, int& set) {
	int tt = 0;
	int* mau = new int[n];
	for (int i = 0; i < n; i++)
		mau[i] = MAUCHU;
	mau[0] = MAUNEN;
	while (1) {
		SetWindowSize(118, 60);
		SetScreenBufferSize(118, 60);
		DisableResizeWindow();
		DisableCtrButton(1, 1, 1);
		Nocursortype();
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		settingsheadline();
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
		SetConsoleTextAttribute(hConsoleColor, mau[0]);
		cout << settings[0];
		if (m % 2 == 0)
			cout << ": ON                  " << "         " << endl;
		else cout << ": OFF                 " << "         " << endl;
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
		SetConsoleTextAttribute(hConsoleColor, mau[1]);
		cout << settings[1];
		if (s % 2 == 0)
			cout << ": ON                        " << "         " << endl;
		else
			cout << ": OFF                       " << "         " << endl;
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
		SetConsoleTextAttribute(hConsoleColor, mau[2]);
		cout << settings[2];
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

		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		set = tt;
		int z = _getch();
		TRANGTHAI trangthai = key(z);
		switch (trangthai) {
		case UP: {
			if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			if (tt == 0)
				tt = n - 1;
			else
				tt--;
			break;
		}
		case DOWN: {
			if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			if (tt == n - 1)
				tt = 0;
			else
				tt++;
			break;
		}
		case ENTER: {
			if (s % 2 == 0)
				PlaySound(TEXT("enter.wav"), NULL, SND_SYNC);
			return tt;
			break;
		}
		}
		set = tt;
		for (int i = 0; i < n; i++)
			mau[i] = MAUCHU;
		mau[tt] = MAUNEN;
		system("cls");
	}
}

int CREDITS(str credits[1], int n, int m, int s) {
	int tt = 0;
	int* mau = new int[n];
	for (int i = 0; i < n; i++)
		mau[i] = MAUCHU;
	mau[0] = MAUNEN;
	while (1) {
		SetWindowSize(118, 60);
		SetScreenBufferSize(118, 60);
		DisableResizeWindow();
		DisableCtrButton(1, 1, 1);
		Nocursortype();
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		//1 
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
		cout << "                ";
		for (int i = 0; i < n; i++) {
			SetConsoleTextAttribute(hConsoleColor, 222);
			cout << helps[0];
		}
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


		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
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
		int z = _getch();
		TRANGTHAI trangthai = key(z);
		switch (trangthai) {
		case ENTER: {
			if (s % 2 == 0)
				PlaySound(TEXT("enter.wav"), NULL, SND_SYNC);
			return tt;//Tức là mình chọn thao tác đó khi nhấn phím enter
			break;
		}
		}
		for (int i = 0; i < n; i++)
			mau[i] = MAUCHU;
		mau[tt] = MAUNEN;
		system("cls");
	}
}

void main() {
	PlaySound(TEXT("ycg_logo.wav"), NULL, SND_ASYNC);
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	int m = 2, s = 2;
	LastMove = _A[0][0];
	while (1) {
		if (m == 10) m = 2;
		if (s == 10) s = 2;
		int mode = 0, pmode = 0, set = 0;
		DisableResizeWindow();
		DisableCtrButton(1, 1, 1);
		MainMenu(thaotac, 6, m, s, mode);
		system("cls");
		switch (mode) {
		case 0: {
			NEWGAME(newgame, 2, m, s, pmode);
			system("cls");
			if (pmode = 1) {
				StartGame();
				GotoXY(68, 4);
				TextColor(15);
				cout << "   1P's INFO    "<< endl;
				GotoXY(93, 4);
				cout << "   2P's INFO    ";
				GotoXY(_X, _Y);
				_count = 0;
				bool validEnter = true;
				while (1)
				{
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
						else if (_COMMAND == 'R') TakeBack(LastMove);
						else if (_COMMAND == 13) {// Người dùng đánh dấu trên màn hình bàn cờ
							switch (CheckBoard(_X, _Y, LastMove)) {
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
			break;
		}
		case 1: {
			playAfterLoad();
			system("cls");
			break;
		}
		case 2: {
			while (1) {
				SETTINGS(settings, 3, m, s, set);
				system("cls");
				if (set == 2) break;
				else
					if (set == 0) m++;
					else
						if (set == 1) s++;
			}
			break;
		}
		case 3: {
			CREDITS(credits, 1, m, s);
			system("cls");
			break;
		}
		case 4: {
			HELPS(helps, m, s, 1);
			system("cls");
			break;
		}
		case 5: {
			cout << "Ok bye!";
			return;
		}
		}
	}
}
