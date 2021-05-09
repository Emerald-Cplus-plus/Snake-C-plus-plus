#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <vector>
#include <string>

using namespace std;

COORD apple_c, tail_end;
char direction = 'u', apple_ch = '@', head_ch = 1, tail_ch = 'o', wall = 35;
short sleep_time = 200, cols = 50, lines = 31;
vector <COORD> snake;
HANDLE h;

void retention() {
	while (true);
}

COORD rand_apple_pos() {
	COORD answer;
	bool flag = false;
	while (flag == false) {
		answer.X = rand() % (cols - 3) + 1;
		answer.Y = rand() % (lines - 3) + 1;
		flag = true;
		for (int i = 0; i < snake.size() && flag == true; ++i) {
			if (snake[i].X == answer.X && snake[i].Y == answer.Y) flag = false;
		}
	}
	return answer;
}

void make_wall() {
	for (int i = 0; i < cols + 1; ++i)putchar(wall);
	for (int j = 0; j < lines - 3; ++j) {
		for (int i = 0; i < cols - 2; ++i)putchar(' ');
		putchar(wall);
		putchar(wall);
	}
	for (int i = 0; i < cols - 1; ++i)putchar(wall);
}

bool snake_live() {
	for (int i = 1; i < snake.size(); ++i) {
		if (snake[0].X == snake[i].X && snake[0].Y == snake[i].Y)return false;
	}
	if (snake[0].X == 0 || snake[0].X == cols - 1 || snake[0].Y == 0 || snake[0].Y == lines - 2)return false;
	return true;
}

void paint_snake(char dir) {
	if (dir == ' ') {
		SetConsoleCursorPosition(h, snake[snake.size() - 1]);
		putchar(tail_ch);
		return;
	}
	SetConsoleCursorPosition(h, snake[snake.size() - 1]);
	putchar(' ');
	tail_end = snake[snake.size() - 1];
	for (int i = snake.size() - 1; i > 0; --i) {
		snake[i] = snake[i - 1];
	}
	if (dir == 'u')snake[0] = { snake[0].X, snake[0].Y - 1 };
	if (dir == 'd')snake[0] = { snake[0].X, snake[0].Y + 1 };
	if (dir == 'l')snake[0] = { snake[0].X - 1, snake[0].Y };
	if (dir == 'r')snake[0] = { snake[0].X + 1, snake[0].Y };
	for (int i = 1; i < snake.size(); ++i) {
		SetConsoleCursorPosition(h, snake[i]);
		putchar(tail_ch);
	}
	SetConsoleCursorPosition(h, snake[0]);
	putchar(head_ch);
}

void game() {
	snake.clear();
	snake.push_back({ ((cols - 2) / 2), ((lines - 3) / 2) });
	SetConsoleCursorPosition(h, { 0, 0 });
	direction = 'u';
	apple_c = rand_apple_pos();

	SetConsoleTextAttribute(h, 14);
	make_wall();
	SetConsoleTextAttribute(h, 12);
	SetConsoleCursorPosition(h, apple_c);
	putchar(apple_ch);
	SetConsoleTextAttribute(h, 10);
	SetConsoleCursorPosition(h, snake[0]);
	putchar(head_ch);

	while (snake_live()) {
		Sleep(sleep_time);
		if (_kbhit()) {
			int k = _getch();
			if (k == 0 || k == 224) k = _getch();
			switch (k) {
			case 80:
				direction = 'd';
				break;
			case 72:
				direction = 'u';
				break;
			case 75:
				direction = 'l';
				break;
			case 77:
				direction = 'r';
				break;
			case 27:// Esc
				exit(0);
				break;
			}
		}
		paint_snake(direction);
		if (snake[0].X == apple_c.X && snake[0].Y == apple_c.Y) {
			snake.push_back(tail_end);
			paint_snake(' ');
			apple_c = rand_apple_pos();
			SetConsoleTextAttribute(h, 12);
			SetConsoleCursorPosition(h, apple_c);
			putchar(apple_ch);
			SetConsoleTextAttribute(h, 10);
		}
	}
	SetConsoleTextAttribute(h, 4);
	SetConsoleCursorPosition(h, { (cols - 12) / 2, (lines - 3) / 2 - 2 });
	cout << "Game over!";
	Sleep(750);
}

bool play_again() {
	SetConsoleTextAttribute(h, 10);
	SetConsoleCursorPosition(h, { (cols - 28) / 2, (lines - 3) / 2 });
	cout << "Do you want to play again?";
	SetConsoleCursorPosition(h, { (cols - 28) / 2 + 8, (lines - 3) / 2 + 2 });
	cout << "Yes       No";
	SetConsoleCursorPosition(h, { (cols - 28) / 2 + 8, (lines - 3) / 2 + 3 });
	cout << "```";
	char flag = 'Y';
	auto input = [&](auto&& input) -> int {
		int k = _getch();
		switch (k) {
		case 75:
			if (flag == 'Y')
				flag = 'N';
			else
				flag = 'Y';
			break;
		case 77:
			if (flag == 'Y')
				flag = 'N';
			else
				flag = 'Y';
			break;
		case 13:
			if (flag == 'Y')
				return 1;
			else
				return 0;
			break;
		default:
			input(input);
		}
		return 3;
	};
	while (true) {
		int inputq = input(input);
		if (inputq == 1)
			return true;
		if (inputq == 0)
			return false;
		SetConsoleCursorPosition(h, { (cols - 28) / 2 + 8, (lines - 3) / 2 + 3 });
		if (flag == 'Y')
			cout << "```         ";
		else
			cout << "          ``";
	}
}

void card_size() {
	SetConsoleTextAttribute(h, 10);
	SetConsoleCursorPosition(h, { (cols - 18) / 2, (lines - 3) / 2 - 1 });
	cout << "Choose map size:";
	SetConsoleCursorPosition(h, { (cols - 26) / 2, (lines - 3) / 2 + 1 });
	cout << "Small    Medium    Large";
	SetConsoleCursorPosition(h, { (cols - 26) / 2, (lines - 3) / 2 + 2 });
	cout << "`````";
	char flag = 'S';
	auto input = [&](auto&& input) -> bool {
		int k = _getch();
		switch (k) {
		case 75:
			if (flag == 'S')
				flag = 'L';
			else
				if (flag == 'M')
					flag = 'S';
				else
					if (flag == 'L')
						flag = 'M';
			break;
		case 77:
			if (flag == 'S')
				flag = 'M';
			else
				if (flag == 'M')
					flag = 'L';
				else
					if (flag == 'L')
						flag = 'S';
			break;
		case 13:
			return true;
			break;
		default:
			input(input);
		}
		return false;
	};
	while (true) {
		if (input(input)) {
			if (flag == 'M') {
				cols = 65;
				lines = 41;
			}
			if (flag == 'L') {
				cols = 80;
				lines = 51;
			}
			return;
		}
		SetConsoleCursorPosition(h, { (cols - 26) / 2, (lines - 3) / 2 + 2 });
		if (flag == 'S')
			cout << "`````                   ";
		if (flag == 'M')
			cout << "         ``````         ";
		if (flag == 'L')
			cout << "                   `````";
	}
}

int main()
{
	system("mode con cols=50 lines=31");
	MoveWindow(GetConsoleWindow(), 50, 50, 2000, 2000, true);
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci = { sizeof(cci), false };
	SetConsoleCursorInfo(h, &cci);
	SetConsoleTextAttribute(h, 14);
	make_wall();
	card_size();
	if (cols == 65) {
		system("mode con cols=65 lines=41");
	}
	if (cols == 80) {
		system("mode con cols=80 lines=51");
	}
	srand(time(0));
	do {
		game();
	} while (play_again());
	//retention();
	return 0;
}