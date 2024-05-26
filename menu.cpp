#include <iostream>
#include <conio.h>

#include "core.h"

using namespace std;

void menu() {
	clearScren();
	int choice;
	choice = _getch();
	switch (choice) {
	case '1':
		startGame();
		break;
	case '2':
		print_exit();
		exit(0);
		break;
	}
}

void clearScren() {
	system("cls");
}

void print_menu() {
	cout << "1. 开始游戏" << endl;
	cout << "2. 退出游戏" << endl;
	cout << "请输入你的选择：";
}

void print_exit()
{
	cout << "感谢游玩！" << endl;
}