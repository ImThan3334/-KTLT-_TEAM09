#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>
using namespace std;
#define MAUNEN 249
#define MAUCHU 240

HANDLE hConsoleColor;

typedef char str[31];
str thaotac[6] = { "NEW GAME", "LOAD GAME" ,"SETTINGS", "CREDITS", "HELPS" , "EXIT"};
str newgame[2] = {"SINGLEPLAYER (PVB)", "MULTIPLAYER (PVP)"};
str settings[3] = { "MUSIC", "SFX", "BACK" };
str credits[1] = { "BACK" };
str helps[1] = { "BACK"};
enum TRANGTHAI { UP, DOWN, LEFT, RIGHT, ENTER, BACK };

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

void verticalmenu() {
	SetConsoleTextAttribute(hConsoleColor, 244);
	cout << "   /------------------------------------------------------------------------------------------\\    " << endl;
	SetConsoleTextAttribute(hConsoleColor, 246);
	cout << "   | _________     _____ _________  ________       ________    _____      _____   ___________ |    " << endl;
	SetConsoleTextAttribute(hConsoleColor, 254);
	cout << "   | \\_   ___ \\   /  _  \\ \\______ \\ \\_____  \\     /  _____/   /  _  \\    /     \\  \\_   _____/ |    " << endl;
	SetConsoleTextAttribute(hConsoleColor, 250);
	cout << "   | /    \\  \\/  /  /_\\  \\ |     _/ /   |    \\   /   \\  ___  /  /_\\  \\  /  \\ /  \\  |    __)_  |    " << endl;
	SetConsoleTextAttribute(hConsoleColor, 242);
	cout << "   | \\     \\____/    |    \\    |  \\/    |     \\  \\    \\_\\  \\/    |    \\/    Y    \\ |        \\ |    " << endl;
	SetConsoleTextAttribute(hConsoleColor, 241);
	cout << "   |  \\______ / \\____|__  /____| _/\\________  /   \\______  /\\____|__  /\\____|__  / _______  / |    " << endl;
	SetConsoleTextAttribute(hConsoleColor, 249);
	cout << "   |        \\/          \\/      \\/          \\/           \\/         \\/         \\/         \\/  |    " << endl;
	SetConsoleTextAttribute(hConsoleColor, 253);
	cout << "   \\------------------------------------------------------------------------------------------/    " << endl;
}

void rainbowmenu() {
	int a = 0;
	SetConsoleTextAttribute(hConsoleColor, MAUCHU);
	cout << "    /------------------------------------------------------------------------------------------\\    " << endl;
	cout << "    | ";
	//line1
	SetConsoleTextAttribute(hConsoleColor, 244 - a);
	cout << "_________  ";
	SetConsoleTextAttribute(hConsoleColor, 252 - a);
	cout << "   _____ ___";
	SetConsoleTextAttribute(hConsoleColor, 246 - a);
	cout << "______  ____";
	SetConsoleTextAttribute(hConsoleColor, 250 - a);
	cout << "____       _";
	SetConsoleTextAttribute(hConsoleColor, 242 - a);
	cout << "_______    __";
	SetConsoleTextAttribute(hConsoleColor, 241 - a);
	cout << "___      _";
	SetConsoleTextAttribute(hConsoleColor, 249 - a);
	cout << "____   ___";
	SetConsoleTextAttribute(hConsoleColor, 253 - a);
	cout << "________";
	SetConsoleTextAttribute(hConsoleColor, MAUCHU);
	cout <<" |    " << endl;
	//line2
	cout << "    | ";
	SetConsoleTextAttribute(hConsoleColor, 244 - a);
	cout << "\\_   ___ \\";
	SetConsoleTextAttribute(hConsoleColor, 252 - a);
	cout << "   /  _  \\ \\";
	SetConsoleTextAttribute(hConsoleColor, 246 - a);
	cout << "______ \\ \\__";
	SetConsoleTextAttribute(hConsoleColor, 250 - a);
	cout << "___  \\     /";
	SetConsoleTextAttribute(hConsoleColor, 242 - a);
	cout << "  _____/   / ";
	SetConsoleTextAttribute(hConsoleColor, 241 - a);
	cout << " _  \\    /";
	SetConsoleTextAttribute(hConsoleColor, 249 - a);
	cout << "     \\  \\_";
	SetConsoleTextAttribute(hConsoleColor, 253 - a);
	cout << "   _____/";
	SetConsoleTextAttribute(hConsoleColor, MAUCHU);
	cout << " |    " << endl;
	//line3
	cout << "    | ";
	SetConsoleTextAttribute(hConsoleColor, 244 - a);
	cout << "/    \\  \\";
	SetConsoleTextAttribute(hConsoleColor, 252 - a);
	cout << "/  /  /_\\  \\";
	SetConsoleTextAttribute(hConsoleColor, 246 - a);
	cout << " |     _/ / ";
	SetConsoleTextAttribute(hConsoleColor, 250 - a);
	cout << "  |    \\   /";
	SetConsoleTextAttribute(hConsoleColor, 242 - a);
	cout << "   \\  ___  / ";
	SetConsoleTextAttribute(hConsoleColor, 241 - a);
	cout << " /_\\  \\  /";
	SetConsoleTextAttribute(hConsoleColor, 249 - a);
	cout << "  \\ /  \\  ";
	SetConsoleTextAttribute(hConsoleColor, 253 - a);
	cout << "|    __)_ ";
	SetConsoleTextAttribute(hConsoleColor, MAUCHU);
	cout << " |    " << endl;
	//line4
	cout << "    | ";
	SetConsoleTextAttribute(hConsoleColor, 244 - a);
	cout << "\\     \\_";
	SetConsoleTextAttribute(hConsoleColor, 252 - a);
	cout << "___/    |   ";
	SetConsoleTextAttribute(hConsoleColor, 246 - a);
	cout << " \\    |  \\/ ";
	SetConsoleTextAttribute(hConsoleColor, 250 - a);
	cout << "   |     \\  ";
	SetConsoleTextAttribute(hConsoleColor, 242 - a);
	cout << "\\    \\_\\  \\/ ";
	SetConsoleTextAttribute(hConsoleColor, 241 - a);
	cout << "   |    \\/";
	SetConsoleTextAttribute(hConsoleColor, 249 - a);
	cout << "    Y    \\";
	SetConsoleTextAttribute(hConsoleColor, 253 - a);
	cout << " |        \\";
	SetConsoleTextAttribute(hConsoleColor, MAUCHU);
	cout << " |    " << endl;
	//line5
	cout << "    | ";
	SetConsoleTextAttribute(hConsoleColor, 244 - a);
	cout << " \\_____";
	SetConsoleTextAttribute(hConsoleColor, 252 - a);
	cout << "_ / \\____|__";
	SetConsoleTextAttribute(hConsoleColor, 246 - a);
	cout << "  /____| _/\\";
	SetConsoleTextAttribute(hConsoleColor, 250 - a);
	cout << "________  / ";
	SetConsoleTextAttribute(hConsoleColor, 242 - a);
	cout << "  \\______  /\\";
	SetConsoleTextAttribute(hConsoleColor, 241 - a);
	cout << "____|__  /";
	SetConsoleTextAttribute(hConsoleColor, 249 - a);
	cout << "\\____|__  ";
	SetConsoleTextAttribute(hConsoleColor, 253 - a);
	cout << "/ _______  /";
	SetConsoleTextAttribute(hConsoleColor, MAUCHU);
	cout << " |    " << endl;
	//line6
	cout << "    | ";
	SetConsoleTextAttribute(hConsoleColor, 244 - a);
	cout << "      ";
	SetConsoleTextAttribute(hConsoleColor, 252 - a);
	cout << " \\/         ";
	SetConsoleTextAttribute(hConsoleColor, 246-a);
	cout << " \\/      \\/ ";
	SetConsoleTextAttribute(hConsoleColor, 250 - a);
	cout << "         \\/ ";
	SetConsoleTextAttribute(hConsoleColor, 242 - a);
	cout << "          \\/ ";
	SetConsoleTextAttribute(hConsoleColor, 241 - a);
	cout << "        \\/";
	SetConsoleTextAttribute(hConsoleColor, 249 - a);
	cout << "         \\";
	SetConsoleTextAttribute(hConsoleColor, 253 - a);
	cout << "/         \\/ ";
	SetConsoleTextAttribute(hConsoleColor, MAUCHU);
	cout <<" |    " << endl;
	cout << "    \\------------------------------------------------------------------------------------------/    " << endl;
}

int MainMenu(str thaotac[6], int n,int m,int s,int &mode) {
	int tt = 0;//Biến chỉ ra đang ở thao tác nào hiện tạo tt= 0 là chỉ thao tác thứ nhất
	int* mau = new int[n];
	for (int i = 0; i < n; i++)
		mau[i] = MAUCHU;
	mau[0] = MAUNEN;//Hiện đang ở thao tác thứ nhất
	while (1)
	{
		SetWindowSize(100, 60);
		SetScreenBufferSize(100, 60);
		Nocursortype();
		rainbowmenu();
		cout << "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl;
		//line1
		cout << "                                                                    ";
		SetConsoleTextAttribute(hConsoleColor, 0);
		cout << "######";
		SetConsoleTextAttribute(hConsoleColor, 255);
		cout << "          ";
		SetConsoleTextAttribute(hConsoleColor, 0);
		cout << "######";
		SetConsoleTextAttribute(hConsoleColor, 255);
		cout << "          " << endl;
		//line2
		cout << "                                                                ";
		cout << "  ";
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
		cout << "        " << endl;
		//line3
		cout<< "                                                                ";
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
		cout << "      " << endl;
		for (int i = 0; i < n; i++) {
			SetConsoleTextAttribute(hConsoleColor, mau[i]);//Hàm này là đổi màu chữ cho chữ chuẩn bị đc in ra màn hình
			cout << "                                              ";
			cout << thaotac[i];
			SetConsoleTextAttribute(hConsoleColor, 255);
			switch (i) {
			case 0: {
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
				cout << "      " << endl;
				break;
			}
			case 1: {
				cout << "         ";
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
				cout << "      " << endl;
				break;
			}
			case 2: {
				cout << "          ";
				SetConsoleTextAttribute(hConsoleColor, 0);
				cout << "##";
				SetConsoleTextAttribute(hConsoleColor, 255);
				cout << "                          ";
				SetConsoleTextAttribute(hConsoleColor, 0);
				cout << "##";
				SetConsoleTextAttribute(hConsoleColor, 255);
				cout << "      " << endl;
				break; 
			}
			case 3: {
				cout << "           ";
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
				cout << "      " << endl;
				break; 
			}
			case 4: {
				cout << "             ";
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
				cout << "      " << endl;
				break;
			}
			case 5: {
				cout << "              ";
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
				cout << "      " << endl;
				break;
			}
			}
		}
		cout << "                                                                ";//line10
		SetConsoleTextAttribute(hConsoleColor, 255);
		cout << "    ";
		SetConsoleTextAttribute(hConsoleColor, 0);
		cout << "####";
		SetConsoleTextAttribute(hConsoleColor, 255);
		cout << "              ";
		SetConsoleTextAttribute(hConsoleColor, 0);
		cout << "####";
		SetConsoleTextAttribute(hConsoleColor, 255);
		cout << "          " << endl;
		
		cout << "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl

				
			<< "                                                                                                    " << endl;
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

int NEWGAME(str newgame[2], int n, int m, int s, int &pmode) {
	int tt = 0;
	int* mau = new int[n];
	for (int i = 0; i < n; i++)
		mau[i] = MAUCHU;
	mau[0] = MAUNEN;
	while (1) {
		SetWindowSize(100, 60);
		SetScreenBufferSize(100, 60);
		DisableResizeWindow();
		DisableCtrButton(1, 1, 1);
		Nocursortype();
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		cout << "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl;
		for (int i = 0; i < n; i++) {
			SetConsoleTextAttribute(hConsoleColor, MAUCHU);
			cout << "                                         ";
			SetConsoleTextAttribute(hConsoleColor, mau[i]);
			cout << newgame[i];
			SetConsoleTextAttribute(hConsoleColor, MAUCHU);
			if (i == 0)
			cout << "                                        " << endl;
			else cout << "                                         " << endl;;
		}
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

int HELPS(str helps[1],int m,int s, int n) {
	int tt = 0;
	int* mau = new int[n];
	for (int i = 0; i < n; i++)
		mau[i] = MAUCHU;
	mau[0] = MAUNEN;
	while (1) {
		SetWindowSize(100, 60);
		SetScreenBufferSize(100, 60);
		DisableResizeWindow();
		DisableCtrButton(1, 1, 1);
		Nocursortype();
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		cout << "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                               Developing!                                          " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl;
		cout << "                                                ";
		for (int i = 0; i < n; i++) {
			SetConsoleTextAttribute(hConsoleColor, 222);
			cout << helps[0];
		}
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		cout << "                                                ";
		
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

int SETTINGS(str settings[3], int n, int m, int s,int &set) {
	int tt = 0;
	int* mau = new int[n];
	for (int i = 0; i < n; i++)
		mau[i] = MAUCHU;
	mau[0] = MAUNEN;
	while (1) {
		SetWindowSize(100, 60);
		SetScreenBufferSize(100, 60);
		DisableResizeWindow();
		DisableCtrButton(1, 1, 1);
		Nocursortype();
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		cout << "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl;
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		cout << "                              ";
		SetConsoleTextAttribute(hConsoleColor, mau[0]);
		cout << settings[0];
		if (m % 2 == 0)
			cout << ": ON                       ";
		else 
			cout << ": OFF                      ";
		SetConsoleTextAttribute(hConsoleColor, mau[1]);
		cout << settings[1];
		if (s % 2 == 0)
			cout << ": ON                                 " << endl;
		else
			cout  << ": OFF                                " << endl;
		cout << "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl;
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		cout << "                                               ";
		SetConsoleTextAttribute(hConsoleColor, mau[2]);
		cout << settings[2];
		cout << "                                                ";
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		set = tt;
		int z = _getch();
		TRANGTHAI trangthai = key(z);
		switch (trangthai) {
		case LEFT: {
			if (s % 2 == 0)
				PlaySound(TEXT("menu_file_cursor_move.wav"), NULL, SND_ASYNC);
			if (tt == 0)
				tt = n - 1;
			else
				tt--;
			break;
		}
		case RIGHT: {
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
		SetWindowSize(100, 60);
		SetScreenBufferSize(100, 60);
		DisableResizeWindow();
		DisableCtrButton(1, 1, 1);
		Nocursortype();
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		cout << "                                                                                                    " << endl
			<< "                                  Group 9 - Program technicques                                     " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                   Nguyen Quoc Thang - 2212738                                      " << endl
			<< "                                   Nguyen Dinh Kien - 22127216                                      " << endl
			<< "                                   Nguyen Gia Nguyen - 22127301                                     " << endl
			<< "                                   Nguyen Tran Duc Thien - 22127397                                 " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl
			<< "                                    GVHD. Truong Toan Thinh                                         " << endl
			<< "                                                                                                    " << endl
			<< "                                                                                                    " << endl;
		cout << "                                                ";
		for (int i = 0; i < n; i++) {
			SetConsoleTextAttribute(hConsoleColor, 222);
			cout << helps[0];
		}
		SetConsoleTextAttribute(hConsoleColor, MAUCHU);
		cout << "                                                " << endl
			<< "                                                                                                   " << endl
			<< "                                                                                                   " << endl
			<< "                                                                                                   " << endl
			<< "                                                                                                   " << endl;
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
	while (1) {
		if (m == 10) m = 2;
		if (s == 10) s = 2;
		int mode = 0, pmode = 0, set = 0;
		DisableResizeWindow();
		DisableCtrButton(1, 1, 1);
		MainMenu(thaotac, 6, m , s, mode);
		system("cls");
		switch (mode) {
		case 0: {
			NEWGAME(newgame, 2, m, s, pmode);
			system("cls");
			break;
		}
		case 1: {
			cout << "Still developing!";
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
			return ;
		}
		}
	}
}

//hello test git