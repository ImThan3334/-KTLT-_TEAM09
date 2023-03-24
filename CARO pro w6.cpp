

#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include <time.h>


#define BOARD_SIZE 12
#define LEFT 10
#define TOP 5
struct _POINT { int x, y, c; };
_POINT _A[BOARD_SIZE][BOARD_SIZE];
bool _TURN;
int _COMMAND;
int _X, _Y;
int count; //Đếm số nước đã đi để tính hòa
clock_t start = clock();


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
	int dong = (_Y - 1 - TOP) / 2, cot = (_X - 2 - LEFT) / 4;

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


void SetScreenBufferSize(SHORT width, SHORT height)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD NewSize;
	NewSize.X = width;
	NewSize.Y = height;

	SetConsoleScreenBufferSize(hStdout, NewSize);
}
void ShowScrollbar(BOOL Show)
{
	HWND hWnd = GetConsoleWindow();
	ShowScrollBar(hWnd, SB_BOTH, Show);
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
void FixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void GotoXY(int x, int y) {

	COORD coord;

	coord.X = x;

	coord.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);


}





//Hàm khởi tạo dữ liệu mặc định ban đầu cho ma trận bàn cờ (hàm nhóm Model)

void ResetData()
{

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			_A[i][j].x = 4 * j + LEFT + 2;
			_A[i][j].y = 2 * i + TOP + 1; //Có thay đổi
			_A[i][j].c = 0; // 0 nghĩa là chưa ai đánh dấu, nếu đánh dấu phải theo quy
			//định như sau: -1 là lượt true đánh, 1 là lượt false đánh
		}
	}
	_TURN = true; _COMMAND = -1; // Gán lượt và phím mặc định
	_X = _A[0][0].x; _Y = _A[0][0].y; // Thiết lập lại tọa độ hiện hành ban đầu

}

void HieuUngBanCo() {
	GotoXY(LEFT, TOP);
	printf("%c", 218);
	for (int i = 0; i < BOARD_SIZE * 4; i++)
	{
		printf("%c", 196);
		Sleep(10);
	}
	GotoXY(LEFT + BOARD_SIZE * 4, TOP); printf("%c", 191);


	for (int i = 1; i < BOARD_SIZE * 2; i++)
	{
		GotoXY(LEFT + BOARD_SIZE * 4, i + TOP);
		printf("%c", 179);
		Sleep(20);
	}
	GotoXY(LEFT + BOARD_SIZE * 4, BOARD_SIZE * 2 + TOP); printf("%c", 217);

	for (int i = BOARD_SIZE * 4 + LEFT - 1; i >= LEFT; i--)
	{
		GotoXY(i, BOARD_SIZE * 2 + TOP);
		printf("%c", 196);
		Sleep(10);
	}
	GotoXY(LEFT, BOARD_SIZE * 2 + TOP); printf("%c", 192);

	for (int i = BOARD_SIZE * 2 - 1 + TOP; i > TOP; i--)
	{
		GotoXY(LEFT, i);
		printf("%c", 179);
		Sleep(20);
	}
}

void DrawBoard(int n) {

	//HieuUngBanCo();

	GotoXY(LEFT, TOP); printf("%c%c%c%c", 218, 196, 196, 196);
	for (int i = 0; i < n - 1; i++)
		printf("%c%c%c%c", 194, 196, 196, 196);
	GotoXY(LEFT + n * 4, TOP); printf("%c", 191);


	for (int i = 0; i < n * 2; i += 2)
	{
		GotoXY(LEFT, TOP + i + 1);
		printf("%c", 179);
		for (int j = 0; j < n; j++) { printf("   "); printf("%c", 179); };
	}


	for (int i = 0; i < n - 1; i++) { GotoXY(LEFT, 2 + TOP + 2 * i); printf("%c%c%c%c", 195, 196, 196, 196); for (int j = 0; j < n - 1; j++) printf("%c%c%c%c", 197, 196, 196, 196); printf("%c", 180); }


	GotoXY(LEFT, TOP + (n * 2));
	printf("%c%c%c%c", 192, 196, 196, 196); for (int i = 0; i < n - 1; i++) printf("%c%c%c%c", 193, 196, 196, 196); printf("%c\n", 217);
}

void StartGame() {
	system("cls");
	count = 0;
	ResetData(); // Khởi tạo dữ liệu gốc
	DrawBoard(BOARD_SIZE); // Vẽ màn hình game
	GotoXY(LEFT + 2, TOP + 1);
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
	GotoXY(0, _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y + 2); // Nhảy tới vị trí
	// thích hợp để in chuỗi thắng/thua/hòa
	switch (pWhoWin) {
	case -1:
		//printf("Nguoi choi %d da thang va nguoi choi %d da thua\n", true, false);
		printf("Nguoi choi X da thang va nguoi choi O da thua\n");
		break;
	case 1:
		//printf("Nguoi choi %d da thang va nguoi choi %d da thua\n", false, true);
		printf("Nguoi choi O da thang va nguoi choi X da thua\n");
		break;
	case 0:
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
	GotoXY(0, _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y + 4);
	printf("Nhan 'y/n' de tiep tuc/dung: ");
	return toupper(_getch());
}
int CheckBoard(int pX, int pY, _POINT& LastMove) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (_A[i][j].x == pX && _A[i][j].y == pY && _A[i][j].c == 0) {
				if (_TURN == true) { _A[i][j].c = -1; LastMove = _A[i][j]; } // Nếu lượt hiện hành là true thì c = -1
				else { _A[i][j].c = 1; LastMove = _A[i][j]; } // Nếu lượt hiện hành là false thì c = 1
				return _A[i][j].c;
			}
		}
	}
	return 0;
}


void MoveRight() {
	if (_X < _A[BOARD_SIZE - 1][BOARD_SIZE - 1].x)
	{
		_X += 4;
		GotoXY(_X, _Y);
	}
}
void MoveLeft() {
	if (_X > _A[0][0].x) {
		_X -= 4;
		GotoXY(_X, _Y);
	}

}
void MoveDown() {
	if (_Y < _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y)
	{
		_Y += 2;
		GotoXY(_X, _Y);
	}
}
void MoveUp() {
	if (_Y > _A[0][0].y) {
		_Y -= 2;
		GotoXY(_X, _Y);
	}
}





int TestBoard()
{
	if (count == BOARD_SIZE * BOARD_SIZE) return 0; // Hòa
	else {
		if (WinnerCheck())
			return (_TURN == true ? -1 : 1); // -1 nghĩa là lượt ‘true’ thắng, 1 la false thang
		else
			return 2; // 2 nghĩa là chưa ai thắng
	}
}

void DisableSelection()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(hStdin, ~ENABLE_QUICK_EDIT_MODE);
}

void TakeBack(_POINT LastMove)
{
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (_A[i][j].x == LastMove.x && _A[i][j].y == LastMove.y)
				if (_A[i][j].c != 0) {
					_A[i][j].c = 0;
					_TURN = !_TURN;
					count--;
					_X = LastMove.x;
					_Y = LastMove.y;
					GotoXY(_X, _Y); printf(" "); GotoXY(_X, _Y);//Xóa dấu đã đánh 
				}
		}

}

int main()
{
	SetScreenBufferSize(70, 20);
	SetWindowSize(70, 30);
	ShowScrollbar(0);
	FixConsoleWindow();
	DisableSelection(); //Chặn bôi đen

	StartGame();
	bool validEnter = true;
	_POINT LastMove = _A[0][0];
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
			_COMMAND = toupper(_getch());
			if (_COMMAND == 27)
			{
				ExitGame();
				return 0;
			}
			else {
				if (_COMMAND == 'A') MoveLeft();
				else if (_COMMAND == 'W') MoveUp();
				else if (_COMMAND == 'S') MoveDown();
				else if (_COMMAND == 'D') MoveRight();
				else if (_COMMAND == 'R') TakeBack(LastMove);
				else if (_COMMAND == 13) {// Người dùng đánh dấu trên màn hình bàn cờ
					switch (CheckBoard(_X, _Y, LastMove)) {
					case -1:
						printf("X"); break;
					case 1:
						printf("O");
						break;
					case 0: validEnter = false; break; // Khi đánh vào ô đã đánh rồi
					}
					// Tiếp theo là kiểm tra và xử lý thắng/thua/hòa/tiếp tục
					if (validEnter == true) {

						switch (ProcessFinish(TestBoard())) {
						case -1: case 1: case 0:
							if (AskContinue() != 'Y') {
								ExitGame(); return 0;
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